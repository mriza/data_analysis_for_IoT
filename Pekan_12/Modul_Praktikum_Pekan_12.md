---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Praktikum Pekan 12: Instalasi dan Integrasi InfluxDB, Grafana Dashboard, serta Notifikasi Alert Telegram"
---

# Materi Praktikum Pekan 12: Instalasi dan Integrasi InfluxDB, Grafana Dashboard, serta Notifikasi Alert Telegram

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 12 (Praktikum Mandiri & Tugas 2 Dashboard — $1 \times 170$ Menit)  
**Capaian Pembelajaran (Sub-CPMK 12):** Mahasiswa mampu melakukan instalasi dan konfigurasi *Time-Series Database* **InfluxDB v2.x** dan platform visualisasi **Grafana**, menulis skrip Python jembatan (*bridge*) dari **MQTT ke InfluxDB**, merancang panel visualisasi *real-time* menggunakan kueri **Flux**, serta mengonfigurasi sistem peringatan dini otomatis (**Alerting Webhook**) ke aplikasi **Telegram**.

---

## 1. Tujuan Praktikum
1. Mahasiswa mampu menginstal dan menjalankan layanan **InfluxDB v2.x** (port `8086`) dan **Grafana Server** (port `3000`) di Ubuntu Server / lingkungan lokal.
2. Mahasiswa mampu mengonfigurasi *Organization*, *Bucket*, dan *API Token* pada InfluxDB serta menyuntikkan data telemetri dari Python menggunakan pustaka `influxdb-client`.
3. Mahasiswa mampu menggabungkan *Subscriber* MQTT (dari Pekan 2) dengan penulis InfluxDB sehingga setiap paket sensor yang masuk ke broker langsung tersimpan otomatis ke dalam database runtun waktu.
4. Mahasiswa mampu membangun *Dashboard* interaktif di Grafana (panel *Time-Series*, *Gauge*, dan *Stat*) menggunakan bahasa kueri **Flux** serta menghubungkan pemicu **Alert Rule** ke **Bot Telegram**.

---

## 2. Persiapan Alat & Prasyarat
- **Lingkungan Server:** Ubuntu Server 24.04 LTS VM (yang telah terpasang Mosquitto/RabbitMQ dari Pekan 2) atau komputer lokal.
- **Pustaka Python:** `influxdb-client`, `paho-mqtt`.
  ```bash
  source ~/env_iot/bin/activate
  pip install influxdb-client paho-mqtt
  ```

---

## 3. Langkah Kerja Bagian A: Instalasi & Konfigurasi InfluxDB v2.x

### 3.1 Instalasi InfluxDB v2 di Ubuntu Server 24.04 LTS
Jalankan perintah berikut melalui terminal SSH Ubuntu Server Anda untuk menambahkan repositori resmi InfluxData dan menginstal `influxdb2`:

```bash
# 1. Unduh dan tambahkan GPG Key resmi InfluxData
curl --silent --location -O https://repos.influxdata.com/influxdata-archive.key
gpg --show-keys --with-fingerprint --with-colons ./influxdata-archive.key 2>&1 \
| grep -q '^fpr:\+24C975CBA61A024EE1B631787C3D57159FC2F927:$' \
&& cat influxdata-archive.key | gpg --dearmor \
| sudo tee /etc/apt/keyrings/influxdata-archive.gpg > /dev/null

# 2. Tambahkan repositori InfluxDB ke daftar apt
echo 'deb [signed-by=/etc/apt/keyrings/influxdata-archive.gpg] https://repos.influxdata.com/debian stable main' \
| sudo tee /etc/apt/sources.list.d/influxdata.list

# 3. Update apt dan instal InfluxDB v2
sudo apt update && sudo apt install -y influxdb2

# 4. Aktifkan dan jalankan service influxdb
sudo systemctl enable --now influxdb
sudo systemctl status influxdb
```

### 3.2 Onboarding Awal InfluxDB (Web UI Port `8086`)
1. Buka browser di laptop Windows Anda dan akses **`http://[IP_UBUNTU]:8086`** (atau `http://localhost:8086`).
2. Klik **Get Started** dan isikan formulir inisialisasi awal:
   - **Username:** `admin`
   - **Password:** `passwordrahasia` (minimal 8 karakter)
   - **Initial Organization Name:** `Politani`
   - **Initial Bucket Name:** `sensor_pertanian`
3. Klik **Continue**, lalu pilih **Configure Later**.
4. **Membuat / Menyalin API Token:**
   - Masuk ke menu navigasi kiri: **Load Data (ikon panah atas) $\rightarrow$ API Tokens**.
   - Klik **Generate API Token $\rightarrow$ All Access API Token**, beri deskripsi `Token_Python_Grafana`, lalu **salin (*copy*) string Token tersebut ke Notepad Anda** (Token ini hanya ditampilkan satu kali!).

---

## 4. Langkah Kerja Bagian B: Ingesti Data dari Python & MQTT ke InfluxDB

### 4.1 Skrip Pengujian Tulis Langsung Python ke InfluxDB (`write_influx.py`)
Buat berkas `write_influx.py` untuk menguji pengiriman data menggunakan struktur `Point` (*Measurement*, *Tag*, dan *Field*):

```python
import influxdb_client
from influxdb_client.client.write_api import SYNCHRONOUS
import time
import random

# Konfigurasi Koneksi InfluxDB v2
URL = "http://localhost:8086"  # Ganti dengan IP Ubuntu jika dijalankan dari luar VM
TOKEN = "MASUKKAN_API_TOKEN_INFLUXDB_ANDA_DI_SINI"
ORG = "Politani"
BUCKET = "sensor_pertanian"

# Inisialisasi Klien dan Write API Sinkronus
client = influxdb_client.InfluxDBClient(url=URL, token=TOKEN, org=ORG)
write_api = client.write_api(write_options=SYNCHRONOUS)

print("Mulai mengirim data telemetri ke InfluxDB... (Tekan Ctrl+C untuk berhenti)")
try:
    for i in range(1, 21):
        suhu = round(random.uniform(26.0, 34.5), 2)
        kelembaban = round(random.uniform(55.0, 82.0), 2)
        
        # Menyusun objek Point sesuai standar Line Protocol:
        # Measurement: "iklim_mikro" | Tag: lokasi="Lahan_A" | Fields: suhu_udara, kelembaban_tanah
        point = (
            influxdb_client.Point("iklim_mikro")
            .tag("lokasi", "Lahan_A")
            .tag("node_id", "ESP32_01")
            .field("suhu_udara", float(suhu))
            .field("kelembaban_tanah", float(kelembaban))
        )
        
        write_api.write(bucket=BUCKET, org=ORG, record=point)
        print(f"[{i:02d}] Tersimpan di InfluxDB -> Suhu: {suhu}°C | Kelembaban: {kelembaban}%")
        time.sleep(2)
except KeyboardInterrupt:
    print("\nPengiriman dihentikan.")
finally:
    client.close()
```

### 4.2 Skrip Jembatan Otomatis: MQTT Subscriber ke InfluxDB (`mqtt_to_influx_bridge.py`)
Dalam arsitektur IoT nyata, sensor mengirim data ke Broker MQTT, lalu skrip *worker* menangkapnya dan menyimpannya ke InfluxDB. Berikut adalah kode lengkap jembatan tersebut:

```python
import json
import paho.mqtt.client as mqtt
import influxdb_client
from influxdb_client.client.write_api import SYNCHRONOUS

# 1. Konfigurasi InfluxDB
INFLUX_URL = "http://localhost:8086"
INFLUX_TOKEN = "MASUKKAN_API_TOKEN_INFLUXDB_ANDA_DI_SINI"
INFLUX_ORG = "Politani"
INFLUX_BUCKET = "sensor_pertanian"

influx_client = influxdb_client.InfluxDBClient(url=INFLUX_URL, token=INFLUX_TOKEN, org=INFLUX_ORG)
write_api = influx_client.write_api(write_options=SYNCHRONOUS)

# 2. Konfigurasi MQTT Broker
MQTT_BROKER = "localhost"
MQTT_PORT = 1884
MQTT_TOPIC = "kebun/+/telemetri"

def on_connect(client, userdata, flags, reason_code, properties):
    print(f"[BRIDGE AKTIF] Terhubung ke MQTT Broker (kode: {reason_code})")
    client.subscribe(MQTT_TOPIC, qos=1)

def on_message(client, userdata, message):
    try:
        # Asumsi payload JSON: {"lokasi": "Zona_A", "suhu": 29.5, "kelembaban": 68.0}
        payload_str = message.payload.decode("utf-8")
        data = json.loads(payload_str)
        
        lokasi = str(data.get("lokasi", "Zona_Default"))
        suhu = float(data["suhu"])
        kelembaban = float(data["kelembaban"])
        
        # Validasi Batas Fisik Sederhana sebelum masuk Database
        if 10.0 <= suhu <= 50.0 and 0.0 <= kelembaban <= 100.0:
            p = (
                influxdb_client.Point("iklim_mikro")
                .tag("lokasi", lokasi)
                .field("suhu_udara", suhu)
                .field("kelembaban_tanah", kelembaban)
            )
            write_api.write(bucket=INFLUX_BUCKET, org=INFLUX_ORG, record=p)
            print(f"[DB SAVED] {lokasi} -> Suhu: {suhu}°C, RH: {kelembaban}%")
        else:
            print(f"[OUTLIER REJECTED] Nilai di luar batas fisika: {data}")
            
    except Exception as e:
        print(f"[ERROR PARSING/WRITE] {e}")

mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, client_id="InfluxDB_Bridge_Worker")
mqtt_client.username_pw_set("admin", "passwordrahasia")
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message

mqtt_client.connect(MQTT_BROKER, MQTT_PORT, 60)
mqtt_client.loop_forever()
```

---

## 5. Langkah Kerja Bagian C: Instalasi Grafana, Kueri Flux, & Alerting Telegram

### 5.1 Instalasi Grafana Enterprise/OSS di Ubuntu Server
```bash
sudo apt install -y apt-transport-https software-properties-common wget
sudo mkdir -p /etc/apt/keyrings/
wget -q -O - https://apt.grafana.com/gpg.key | gpg --dearmor | sudo tee /etc/apt/keyrings/grafana.gpg > /dev/null
echo "deb [signed-by=/etc/apt/keyrings/grafana.gpg] https://apt.grafana.com stable main" | sudo tee /etc/apt/sources.list.d/grafana.list

sudo apt update && sudo apt install -y grafana
sudo systemctl enable --now grafana-server
```
Akses antarmuka web Grafana melalui browser di **`http://[IP_UBUNTU]:3000`**. Login menggunakan kredensial bawaan: username **`admin`** dan password **`admin`** (lalu buat password baru saat diminta).

### 5.2 Menghubungkan Data Source InfluxDB (Bahasa Flux)
1. Di menu kiri Grafana, pilih **Connections $\rightarrow$ Data sources $\rightarrow$ Add new data source**.
2. Pilih **InfluxDB**.
3. Pada bagian **Query Language**, ubah dari `InfluxQL` menjadi **`Flux`**.
4. Pada bagian **HTTP $\rightarrow$ URL**, ketikkan `http://localhost:8086`.
5. Pada bagian **InfluxDB Details**, isikan:
   - **Organization:** `Politani`
   - **Token:** *(Tempelkan API Token InfluxDB Anda)*
   - **Default Bucket:** `sensor_pertanian`
6. Klik tombol **Save & test**. Pastikan muncul kotak hijau *"datasource is working. 1 buckets found"*.

### 5.3 Membangun Panel Visualisasi dengan Kueri Flux
1. Masuk ke menu **Dashboards $\rightarrow$ New $\rightarrow$ New Dashboard $\rightarrow$ Add visualization**, lalu pilih `InfluxDB`.
2. Masukkan skrip kueri **Flux** berikut pada editor kueri untuk menampilkan grafik rata-rata bergerak suhu udara:
   ```flux
   from(bucket: "sensor_pertanian")
     |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
     |> filter(fn: (r) => r["_measurement"] == "iklim_mikro")
     |> filter(fn: (r) => r["_field"] == "suhu_udara")
     |> aggregateWindow(every: v.windowPeriod, fn: mean, createEmpty: false)
     |> yield(name: "mean")
   ```
3. Pada panel sebelah kanan:
   - Pilih tipe visualisasi **Time series**, beri judul `"Tren Suhu Udara Real-Time (°C)"`, dan atur *Standard options $\rightarrow$ Unit* menjadi `Celsius (°C)`.
4. Tambahkan panel kedua bertipe **Gauge** untuk menampilkan nilai suhu terakhir (*Last*), dengan mengatur *Thresholds*:
   - `< 31°C` = Hijau (Normal)
   - `31°C - 33°C` = Kuning (Waspada)
   - `>= 33°C` = Merah (Bahaya Panas)
5. Atur interval penyegaran otomatis (*Auto-refresh*) di pojok kanan atas *dashboard* menjadi **`5s`** (setiap 5 detik).

### 5.4 Konfigurasi Notifikasi Peringatan Dini (*Alerting*) ke Telegram
1. **Membuat Bot Telegram & Mendapatkan Token + Chat ID:**
   - Buka aplikasi Telegram, cari akun resmi **`@BotFather`**, kirim perintah `/newbot`, ikuti instruksi, dan salin **HTTP API Bot Token** (contoh: `7123456:AAHxyz...`).
   - Kirim satu pesan sapaan (`Halo`) ke bot baru Anda, lalu buka URL berikut di browser: `https://api.telegram.org/bot<TOKEN_BOT_ANDA>/getUpdates` untuk melihat angka **`chat.id`** Anda (contoh: `123456789`).
2. **Menambahkan Contact Point di Grafana:**
   - Masuk ke menu **Alerting $\rightarrow$ Contact points $\rightarrow$ Add contact point**.
   - Beri nama `Telegram_Kebun`, pilih *Integration* **Telegram**, masukkan **BOT API Token** dan **Chat ID**, lalu klik **Test** $\rightarrow$ **Save contact point**.
3. **Membuat Alert Rule Anti-Flapping:**
   - Buka panel grafik Suhu, masuk ke tab **Alert $\rightarrow$ Create alert rule from this panel**.
   - Atur kondisi pemicu: `IS ABOVE 33.0`.
   - Atur *Evaluation behavior*: **Evaluate every `1m` for `2m`** (*Pending period* 2 menit untuk mencegah *alert flapping* akibat lonjakan derau sesaat).
   - Pilih *Contact point* `Telegram_Kebun` dan klik **Save rule and exit**.

---

## 6. Tugas 2 (Instalasi & Dashboard Real-Time — Bobot Penilaian RPS 10%)
Bangunlah satu halaman *Dashboard* Grafana utuh untuk skenario **Smart Agriculture** yang memiliki minimal **4 Panel Visualisasi Berbeda**:
1. Panel **Time-Series Line Chart** (menampilkan kurva `suhu_udara` dan `kelembaban_tanah`).
2. Panel **Gauge** (menampilkan nilai terakhir `suhu_udara` dengan indikator warna Hijau/Kuning/Merah).
3. Panel **Stat** (menampilkan rata-rata, nilai minimum, atau maksimum harian).
4. Satu aturan **Alert Rule** aktif yang terbukti berhasil mengirimkan pesan peringatan ke aplikasi Telegram saat Anda menyuntikkan angka suhu $> 35,0^\circ\text{C}$ melalui skrip Python! Lampirkan *screenshot* Dashboard Grafana beserta *screenshot* pesan Telegram yang masuk di dalam laporan praktikum Anda.

---

## 7. Referensi & Acuan Materi Pekan Ini

1. **InfluxData. (2024).** *InfluxDB v2 Documentation & Python Client Library (`influxdb-client-python`)*. Tersedia: [https://docs.influxdata.com/influxdb/v2/](https://docs.influxdata.com/influxdb/v2/)  
2. **Grafana Labs. (2024).** *Grafana Documentation: InfluxDB Data Source, Flux Queries, & Telegram Alerting*. Tersedia: [https://grafana.com/docs/](https://grafana.com/docs/)  
3. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730)  
4. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
