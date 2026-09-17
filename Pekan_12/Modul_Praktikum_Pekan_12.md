# Materi Praktikum Pekan 12: InfluxDB & Grafana

## MODUL 12: Pengenalan Time-Series Database (InfluxDB)
**Tujuan:** Mahasiswa mampu menyimpan data *time-series* berkecepatan tinggi ke dalam InfluxDB menggunakan Python.

### 12.1 Instalasi dan Setup InfluxDB
1. Unduh dan instal InfluxDB v2.x (menyesuaikan OS masing-masing).
2. Jalankan service `influxd` melalui terminal.
3. Buka browser dan akses antarmuka InfluxDB di `http://localhost:8086`.
4. Lakukan setup awal: Buat *Organization* (contoh: `Politani`), *Bucket* (contoh: `sensor_pertanian`), dan *generate API Token*.

### 12.2 Menulis Data dari Python ke InfluxDB
Gunakan terminal untuk menginstal pustaka InfluxDB: `pip install influxdb-client`.

```python
import influxdb_client
from influxdb_client.client.write_api import SYNCHRONOUS
import time
import random

# Konfigurasi Token dan Koneksi
token = "MASUKKAN_API_TOKEN_ANDA_DI_SINI"
org = "Politani"
url = "http://localhost:8086"
bucket = "sensor_pertanian"

# Membangun Koneksi
client = influxdb_client.InfluxDBClient(url=url, token=token, org=org)
write_api = client.write_api(write_options=SYNCHRONOUS)

# Simulasi mengirim data sensor secara berulang setiap 2 detik
for i in range(5):
    suhu = round(random.uniform(28.0, 32.0), 2)
    kelembaban = round(random.uniform(60.0, 80.0), 2)
    
    # Format data Point untuk InfluxDB
    p = influxdb_client.Point("iklim_mikro") \
        .tag("lokasi", "Lahan_A") \
        .field("suhu_udara", suhu) \
        .field("kelembaban_tanah", kelembaban)
    
    write_api.write(bucket=bucket, org=org, record=p)
    print(f"Mengirim data -> Suhu: {suhu} C, Kelembaban: {kelembaban}%")
    time.sleep(2)

print("Penulisan selesai.")
```
**Tugas Mahasiswa:** Ubah script Modul 2 (*Subscriber* MQTT) agar data yang ditangkap langsung disimpan otomatis ke InfluxDB!

## MODUL 13: Desain Dashboard Grafana
**Tujuan:** Menampilkan visualisasi data langsung (*live*) dari InfluxDB dan memasang sistem *Alerting*.

### 13.1 Instalasi Grafana dan Koneksi Data Source
1. Unduh dan instal Grafana. Jalankan service-nya dan akses di `http://localhost:3000` (User/Pass default: `admin/admin`).
2. Masuk ke menu **Connections > Data Sources > Add Data Source**.
3. Pilih **InfluxDB**. Ubah query language ke **Flux** (jika menggunakan InfluxDB v2).
4. Masukkan URL `http://localhost:8086`, masukkan Token, Default Bucket, dan Organization. Klik *Save & Test*.

### 13.2 Membuat Panel Time-Series
1. Masuk ke menu **Dashboards > New Dashboard > Add Visualization**.
2. Pilih Data Source InfluxDB yang baru dibuat.
3. Di bagian Query (menggunakan *Query Builder*), pilih `iklim_mikro` > `suhu_udara`.
4. Di bagian kanan layar, ubah tipe panel menjadi **Time series**. Beri judul "Pemantauan Suhu Real-time".
5. Ulangi untuk menambahkan panel tipe **Gauge** yang menunjukkan suhu terkini.

### 13.3 Mengatur Alert (Sistem Peringatan)
1. Edit panel Time-series Suhu.
2. Pergi ke tab **Alerting > Create Alert rule**.
3. Buat kondisi: *Jika Suhu (A) melampaui (*Is above*) 33.0 derajat Celcius selama 1 menit.*
4. Pada bagian *Contact points*, tambahkan **Telegram Webhook**.
