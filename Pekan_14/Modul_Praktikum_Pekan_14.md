---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Praktikum Pekan 14: Implementasi Integrasi Pipeline End-to-End (Project-Based Learning)"
---

# Materi Praktikum Pekan 14: Implementasi Integrasi Pipeline End-to-End (Project-Based Learning)

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 14 (Praktikum Proyek Akhir & Progress Review — $1 \times 170$ Menit)  
**Capaian Pembelajaran (Sub-CPMK 14):** Mahasiswa mampu mengintegrasikan seluruh tahapan analitik (*Data Collector, MQTT Broker, Real-Time Pre-processing, Machine Learning Inference, InfluxDB Storage*, dan *Grafana Alerting*) ke dalam satu sistem orkestrasi utuh (`main_pipeline.py`) yang beroperasi secara otomatis dan kontinu.

---

## 1. Tujuan Praktikum
1. Mahasiswa mampu melatih dan membekukan (*serialize*) model *Machine Learning* (**Isolation Forest** dan **Regresi Forecasting**) ke dalam berkas `.pkl` untuk digunakan pada aliran data waktu nyata.
2. Mahasiswa mampu membangun program pekerja utama (**`main_pipeline.py`**) yang menggabungkan *MQTT Subscriber*, *Streaming Circular Buffer* (`deque`), *Data Cleaning & Smoothing on-the-fly*, *ML Inference*, dan penulisan otomatis ke **InfluxDB**.
3. Mahasiswa mampu membangun simulator sensor multi-skenario (**`sensor_simulator_e2e.py`**) yang mampu menyuntikkan gangguan buatan (*missing value, hard spike*, dan *multivariate anomaly*) untuk menguji ketangguhan *pipeline*.
4. Mahasiswa mampu mendemonstrasikan *progress* sistem proyek akhir kelompok di hadapan dosen pengampu (**Bobot Penilaian Progress Review RPS: 5%**).

---

## 2. Arsitektur Kode Proyek Akhir Terpadu

Pada praktikum pekan ini, kita merangkai 3 berkas program Python yang bekerja secara sinergis bersama **Mosquitto/RabbitMQ**, **InfluxDB**, dan **Grafana**:

```text
  [1. train_model_iot.py]
   (Dijalankan 1x di awal untuk menghasilkan berkas `model_isolation_iot.pkl`)
                                     |
                                     v
  [2. sensor_simulator_e2e.py] ---> (MQTT Broker :1884) ---> [3. main_pipeline.py]
   Mengirim JSON tiap 2 detik        Topik:                   1. Tangkap JSON dari MQTT
   (Normal + Injeksi Anomali)        `kebun/zona_A/telemetri` 2. Filter Batas Fisik & Imputasi Deque
                                                              3. Smoothing Moving Average (SMA/EMA)
                                                              4. Prediksi Isolation Forest (+1 / -1)
                                                              5. Tulis Point ke InfluxDB (:8086)
                                                                            |
                                                                            v
                                                                 [Grafana Dashboard :3000]
                                                                 Visualisasi Live + Alert Telegram!
```

---

## 3. Langkah Kerja Implementasi Referensi End-to-End

### Langkah 1: Membuat Skrip Pelatihan Model AI Offline (`train_model_iot.py`)
Sebelum *pipeline* waktu nyata dijalankan, kita harus melatih model **Isolation Forest** menggunakan profil data historis normal rumah kaca (Suhu Udara, Kelembaban Tanah, dan pH Tanah) lalu menyimpannya ke berkas `model_isolation_iot.pkl`:

```python
import numpy as np
import pandas as pd
from sklearn.ensemble import IsolationForest
import joblib

print("--- Melatih Model Isolation Forest untuk Pipeline Smart Agriculture ---")
np.random.seed(42)

# Membangkitkan 1.000 sampel historis normal:
# Suhu: ~28°C (std 2.0), Kelembaban Tanah: ~68% (berkorelasi negatif dgn suhu), pH: ~6.5
suhu_hist = np.random.normal(28.0, 2.0, 1000)
kelembaban_hist = 100.0 - (1.15 * suhu_hist) + np.random.normal(0.0, 2.5, 1000)
ph_hist = np.random.normal(6.5, 0.3, 1000)

df_train = pd.DataFrame({
    'suhu': suhu_hist,
    'kelembaban': kelembaban_hist,
    'ph_tanah': ph_hist
})

# Inisialisasi dan latih model Isolation Forest
model_iso = IsolationForest(n_estimators=100, contamination=0.02, random_state=42)
model_iso.fit(df_train[['suhu', 'kelembaban', 'ph_tanah']])

# Simpan model ke disk
joblib.dump(model_iso, 'model_isolation_iot.pkl')
print("[SUKSES] Model tersimpan sebagai 'model_isolation_iot.pkl' dan siap digunakan oleh main_pipeline.py!")
```
Jalankan satu kali di terminal:
```bash
python train_model_iot.py
```

---

### Langkah 2: Membangun Orkestrator Utama (`main_pipeline.py`)
Buat berkas `main_pipeline.py`. Skrip ini adalah jantung dari proyek akhir Anda yang berjalan terus-menerus (*daemon worker*):

```python
import json
import statistics
from collections import deque
import pandas as pd
import joblib
import paho.mqtt.client as mqtt
import influxdb_client
from influxdb_client.client.write_api import SYNCHRONOUS

# ==============================================================================
# 1. KONFIGURASI INFRASTRUKTUR (SESUAIKAN DENGAN SERVER ANDA)
# ==============================================================================
MQTT_BROKER   = "localhost"
MQTT_PORT     = 1884
MQTT_USER     = "admin"
MQTT_PASS     = "passwordrahasia"
MQTT_TOPIC    = "kebun/+/telemetri"

INFLUX_URL    = "http://localhost:8086"
INFLUX_TOKEN  = "MASUKKAN_API_TOKEN_INFLUXDB_ANDA_DI_SINI"
INFLUX_ORG    = "Politani"
INFLUX_BUCKET = "sensor_pertanian"

# ==============================================================================
# 2. INISIALISASI MODEL ML, KONEKSI DATABASE, & CIRCULAR BUFFER (O(1) RAM)
# ==============================================================================
print("[INIT] Memuat model Machine Learning 'model_isolation_iot.pkl'...")
model_iso = joblib.load("model_isolation_iot.pkl")

influx_client = influxdb_client.InfluxDBClient(url=INFLUX_URL, token=INFLUX_TOKEN, org=INFLUX_ORG)
write_api = influx_client.write_api(write_options=SYNCHRONOUS)

# Circular Buffer (Deque) ukuran 5 untuk Imputasi & Moving Average Smoothing per sensor
WINDOW_SIZE = 5
buffer_suhu = deque(maxlen=WINDOW_SIZE)
buffer_kelembaban = deque(maxlen=WINDOW_SIZE)
buffer_ph = deque(maxlen=WINDOW_SIZE)

def bersihkan_dan_imputasi(nilai_mentah, batas_bawah, batas_atas, buffer_hist, nilai_default):
    """
    Memeriksa apakah nilai_mentah kosong (None/NaN) atau melanggar batas fisika (Hard Outlier).
    Jika cacat, tambal secara otomatis menggunakan nilai rata-rata dari Circular Buffer (Imputasi).
    """
    try:
        val = float(nilai_mentah)
        if val < batas_bawah or val > batas_atas or pd.isna(val):
            raise ValueError("Di luar batas fisika sensor")
        status_kualitas = "VALID"
    except (ValueError, TypeError):
        # Imputasi instan menggunakan rata-rata buffer terakhir (atau default jika buffer masih kosong)
        val = statistics.mean(buffer_hist) if len(buffer_hist) > 0 else nilai_default
        status_kualitas = "IMPUTED_OR_CLEANED"
        
    buffer_hist.append(val)
    nilai_smooth = statistics.mean(buffer_hist)  # Simple Moving Average (SMA-5)
    return val, round(nilai_smooth, 2), status_kualitas

# ==============================================================================
# 3. CALLBACK PEMROSESAN ALIRAN DATA MQTT SECARA REAL-TIME
# ==============================================================================
def on_connect(client, userdata, flags, reason_code, properties):
    print(f"[PIPELINE AKTIF] Terhubung ke MQTT Broker (kode: {reason_code}). Menunggu data sensor...")
    client.subscribe(MQTT_TOPIC, qos=1)

def on_message(client, userdata, message):
    try:
        payload = json.loads(message.payload.decode("utf-8"))
        lokasi = str(payload.get("lokasi", "Zona_A"))
        node_id = str(payload.get("node_id", "ESP32_01"))
        
        # TAHAP 1: DATA CLEANING, IMPUTATION, & SMOOTHING (ON-THE-FLY)
        suhu_clean, suhu_sma, flag_s = bersihkan_dan_imputasi(
            payload.get("suhu"), batas_bawah=10.0, batas_atas=48.0,
            buffer_hist=buffer_suhu, nilai_default=28.0
        )
        rh_clean, rh_sma, flag_rh = bersihkan_dan_imputasi(
            payload.get("kelembaban"), batas_bawah=15.0, batas_atas=100.0,
            buffer_hist=buffer_kelembaban, nilai_default=68.0
        )
        ph_clean, ph_sma, flag_ph = bersihkan_dan_imputasi(
            payload.get("ph_tanah"), batas_bawah=3.0, batas_atas=9.5,
            buffer_hist=buffer_ph, nilai_default=6.5
        )
        
        # TAHAP 2: MACHINE LEARNING INFERENCE (ISOLATION FOREST MULTIVARIAT)
        df_fitur = pd.DataFrame([[suhu_sma, rh_sma, ph_sma]], columns=['suhu', 'kelembaban', 'ph_tanah'])
        label_anomali = int(model_iso.predict(df_fitur)[0])           # +1 = Normal, -1 = Anomali
        skor_anomali = float(model_iso.decision_function(df_fitur)[0])
        
        # TAHAP 3: PENYIMPANAN KE TIME-SERIES DATABASE (INFLUXDB)
        point = (
            influxdb_client.Point("smart_agriculture_pipeline")
            .tag("lokasi", lokasi)
            .tag("node_id", node_id)
            .field("suhu_bersih", float(suhu_clean))
            .field("suhu_sma", float(suhu_sma))
            .field("kelembaban_sma", float(rh_sma))
            .field("ph_tanah_sma", float(ph_sma))
            .field("status_anomali", int(label_anomali))
            .field("skor_anomali", round(skor_anomali, 4))
        )
        write_api.write(bucket=INFLUX_BUCKET, org=INFLUX_ORG, record=point)
        
        status_teks = "NORMAL (+1)" if label_anomali == 1 else "!!! ANOMALI TERDETEKSI (-1) !!!"
        print(f"[{lokasi}] Suhu={suhu_sma}°C | RH={rh_sma}% | pH={ph_sma} | Kualitas={flag_s} | AI: {status_teks}")
        
    except Exception as e:
        print(f"[ERROR PIPELINE] Gagal memproses paket: {e}")

if __name__ == "__main__":
    mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, client_id="Main_Pipeline_Orchestrator")
    mqtt_client.username_pw_set(MQTT_USER, MQTT_PASS)
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message
    
    mqtt_client.connect(MQTT_BROKER, MQTT_PORT, keepalive=60)
    try:
        mqtt_client.loop_forever()
    except KeyboardInterrupt:
        print("\nMenghentikan Main Pipeline Worker...")
        influx_client.close()
```

---

### Langkah 3: Membangun Simulator Sensor Penguji (`sensor_simulator_e2e.py`)
Untuk menguji apakah `main_pipeline.py` benar-benar mampu menangkap *missing value*, memotong *hard outlier* (`999.0°C`), dan mendeteksi *multivariate anomaly* (`-1`), jalankan skrip simulator berikut di terminal terpisah:

```python
import time
import json
import random
import paho.mqtt.client as mqtt

MQTT_BROKER = "localhost"
MQTT_PORT   = 1884
MQTT_TOPIC  = "kebun/Zona_A/telemetri"

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, client_id="Simulator_Edge_ESP32")
client.username_pw_set("admin", "passwordrahasia")
client.connect(MQTT_BROKER, MQTT_PORT, 60)
client.loop_start()

print("=== SIMULATOR SENSOR SMART AGRICULTURE AKTIF ===")
try:
    siklus = 0
    while True:
        siklus += 1
        suhu = round(random.gauss(28.0, 1.2), 2)
        kelembaban = round(100.0 - (1.15 * suhu) + random.gauss(0, 1.5), 2)
        ph_tanah = round(random.gauss(6.5, 0.15), 2)
        skenario = "NORMAL"

        # Setiap kelipatan 10: Suntikkan Missing Value (None)
        if siklus % 10 == 0:
            suhu = None
            skenario = "UJI MISSING VALUE (NaN)"
        # Setiap kelipatan 15: Suntikkan Hard Outlier Korsleting (999.0°C)
        elif siklus % 15 == 0:
            suhu = 999.0
            skenario = "UJI HARD OUTLIER (999°C)"
        # Setiap kelipatan 12: Suntikkan Anomali Multivariat (Panas, kering, tanah sangat asam)
        elif siklus % 12 == 0:
            suhu = 39.5
            kelembaban = 32.0
            ph_tanah = 4.2
            skenario = "UJI ANOMALI MULTIVARIAT AI (-1)"

        payload = {
            "lokasi": "Zona_A",
            "node_id": "ESP32_Cabai_01",
            "suhu": suhu,
            "kelembaban": kelembaban,
            "ph_tanah": ph_tanah
        }
        client.publish(MQTT_TOPIC, json.dumps(payload), qos=1)
        print(f"[Kirim #{siklus:03d} | {skenario:30s}] Payload: {payload}")
        time.sleep(2)
except KeyboardInterrupt:
    print("\nSimulator dihentikan.")
    client.loop_stop()
    client.disconnect()
```

---

### Langkah 4: Konfigurasi Panel Deteksi Anomali AI di Grafana
1. Buka Grafana (`http://[IP_UBUNTU]:3000`) dan buat panel baru yang membaca `_measurement == "smart_agriculture_pipeline"`.
2. Buat panel **State Timeline / Stat** untuk menampilkan `_field == "status_anomali"`:
   - Atur **Value Mappings**: Nilai `1` ditampilkan sebagai teks **`AMAN (NORMAL)`** berwarna **Hijau**, dan nilai `-1` ditampilkan sebagai teks **`BAHAYA (ANOMALI AI)`** berwarna **Merah Menyala**.
3. Buat **Alert Rule** pada Grafana yang memicu pengiriman pesan **Webhook Telegram** setiap kali nilai `status_anomali` bernilai **`IS BELOW 0`** (yaitu bernilai `-1`)!

---

## 4. Checklist Progress Review Proyek Akhir (Bobot Penilaian RPS: 5%)

Pada akhir sesi praktikum Pekan 14, Dosen Pengampu / Asisten akan memeriksa kemajuan sistem setiap kelompok berdasarkan daftar periksa berikut:

- [ ] **Komponen 1 (Data Collector):** Skrip simulator atau mikrokontroler ESP32 berhasil mempublikasikan payload JSON ke Broker MQTT.
- [ ] **Komponen 2 (Message Broker):** Mosquitto / RabbitMQ berjalan stabil dengan autentikasi aktif.
- [ ] **Komponen 3 (Pipeline Worker):** `main_pipeline.py` berjalan tanpa *crash* ketika diuji dengan nilai `None` (*missing value*) maupun lonjakan angka ekstrem.
- [ ] **Komponen 4 (Model Machine Learning):** Model *Isolation Forest* atau *Forecasting* berhasil dimuat dan mengeksekusi prediksi secara *real-time*.
- [ ] **Komponen 5 (InfluxDB & Grafana):** Data mengalir secara langsung (*live*) ke panel Grafana dan aturan *Alert* telah terkonfigurasi.

---

## 5. Referensi & Acuan Materi Pekan Ini

1. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730)  
2. **McKinney, W. (2022).** *Python for Data Analysis* (3rd ed.). O'Reilly Media. (ISBN-13: 978-1098104030)  
3. **Scikit-learn, InfluxDB, Grafana, & Eclipse Paho Official Documentation (2024).**  
4. Lihat spesifikasi lengkap Tugas Besar pada [`../RTM_Tugas_Besar.md`](../RTM_Tugas_Besar.md) dan [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
