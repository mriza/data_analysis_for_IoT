# BUKU PANDUAN PRAKTIKUM: ANALISIS DATA UNTUK IOT

---

## Bagian 1: Pengenalan, Data Ingestion, dan Preprocessing (Modul 1 - 5)

---

### MODUL 1: Pengenalan Environment & Dummy Data Generator
**Tujuan:** Mahasiswa mampu melakukan instalasi pustaka Python untuk *Data Science* dan men-generate data sensor tiruan (dummy).

#### 1.1 Persiapan Lingkungan (Environment)
Pastikan Python 3.x telah terinstal. Buka terminal/CMD dan jalankan:
```bash
pip install pandas numpy matplotlib paho-mqtt scikit-learn
```

#### 1.2 Membuat Dummy Data Generator
Buat file `generator_sensor.py`. Script ini akan membuat data suhu yang sedikit berfluktuasi.
```python
import pandas as pd
import numpy as np
import datetime

# Membuat timestamp 1 pekan dengan interval 1 jam
waktu = pd.date_range(start="2023-10-01", end="2023-10-07", freq="H")

# Generate data suhu acak di sekitar 28 derajat
suhu = np.random.normal(loc=28.0, scale=2.0, size=len(waktu))

# Simpan ke DataFrame
df = pd.DataFrame({"timestamp": waktu, "suhu": suhu})
df.to_csv("data_sensor_dummy.csv", index=False)
print("Data dummy berhasil disimpan ke data_sensor_dummy.csv")
```
**Tugas Mahasiswa:** Modifikasi script di atas untuk menambahkan kolom `kelembaban_tanah` (range 40-80%).

---

### MODUL 2: Setup IoT Message Broker (MQTT)
**Tujuan:** Mahasiswa memahami alur publikasi data sensor *real-time* melalui protokol MQTT.

#### 2.1 Menjalankan MQTT Broker
Instal **RabbitMQ** di PC masing-masing. Buka terminal dan jalankan service rabbitmq pada *default port* 1883 (atau sesuaikan dengan konfigurasi plugin MQTT RabbitMQ).

#### 2.2 Script Publisher (Pengirim Sensor)
Buat file `mqtt_publish.py`:
```python
import paho.mqtt.client as mqtt
import time, random

broker_address = "localhost"
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, "SensorLahan1")
client.connect(broker_address, 1883)

while True:
    suhu = round(random.uniform(25.0, 35.0), 2)
    client.publish("kebun/sensor/suhu", str(suhu))
    print(f"Data dikirim: {suhu} C")
    time.sleep(2)
```

#### 2.3 Script Subscriber (Penerima di Server)
Buat file `mqtt_subscribe.py`:
```python
import paho.mqtt.client as mqtt

def on_message(client, userdata, message):
    print(f"Data masuk dari topik {message.topic}: {str(message.payload.decode('utf-8'))}")

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, "ServerAnalitik")
client.on_message = on_message
client.connect("localhost", 1883)
client.subscribe("kebun/sensor/suhu")
client.loop_forever()
```
**Instruksi Praktikum:** Jalankan *subscriber* di satu terminal, dan *publisher* di terminal lain. Amati aliran datanya!

---

### MODUL 3: Membaca dan Eksplorasi Data Time-Series
**Tujuan:** Mahasiswa memparsing data CSV dan menampilkan plot dasar.

#### 3.1 Visualisasi Data
Buat file/Jupyter Notebook `eksplorasi.ipynb`:
```python
import pandas as pd
import matplotlib.pyplot as plt

# Load data dan jadikan kolom timestamp sebagai indeks waktu
df = pd.read_csv("data_sensor_dummy.csv", parse_dates=['timestamp'], index_col='timestamp')

# Plot garis sederhana
plt.figure(figsize=(10, 4))
plt.plot(df.index, df['suhu'], label='Suhu Udara', color='red')
plt.title("Grafik Suhu Sensor (1 Pekan)")
plt.xlabel("Waktu")
plt.ylabel("Derajat Celcius")
plt.legend()
plt.grid(True)
plt.show()
```

---

### MODUL 4: Data Preprocessing (Bagian 1: Missing Values)
**Tujuan:** Menangani data yang bolong (*NaN*) akibat koneksi sensor putus.

#### 4.1 Simulasi Missing Value & Penanganannya
```python
import pandas as pd
import numpy as np

# Load data
df = pd.read_csv("data_sensor_dummy.csv")

# Sengaja merusak data (membuat NaN di baris ke-5 dan 6)
df.loc[5:6, 'suhu'] = np.nan
print("Data Rusak (Ada NaN):")
print(df.head(10))

# Metode 1: Drop (Menghapus baris yang rusak)
df_drop = df.dropna()

# Metode 2: Forward Fill (Mengisi dengan nilai terakhir yang terekam)
df_ffill = df.ffill()

# Metode 3: Interpolasi Linear (Membuat garis lurus antara titik terputus)
df_interpolasi = df.interpolate(method='linear')

print("\nData Setelah Interpolasi:")
print(df_interpolasi.head(10))
```

---

### MODUL 5: Data Preprocessing (Bagian 2: Smoothing & Outlier)
**Tujuan:** Membersihkan data dari *noise* (grafik bergerigi) dan mendeteksi anomali (nilai ekstrem).

#### 5.1 Menghaluskan Data (Moving Average)
Data sensor murah biasanya melompat-lompat (*noisy*).
```python
# Menerapkan Moving Average (Rata-rata bergerak) dengan jendela (window) = 3 data terakhir
df['suhu_halus'] = df['suhu'].rolling(window=3).mean()
```

#### 5.2 Mendeteksi Outlier menggunakan Interquartile Range (IQR)
```python
# Hitung Q1 (Kuartil 1) dan Q3 (Kuartil 3)
Q1 = df['suhu'].quantile(0.25)
Q3 = df['suhu'].quantile(0.75)
IQR = Q3 - Q1

# Hitung Batas Bawah dan Batas Atas
batas_bawah = Q1 - 1.5 * IQR
batas_atas = Q3 + 1.5 * IQR

# Deteksi mana baris yang merupakan outlier
outlier = df[(df['suhu'] < batas_bawah) | (df['suhu'] > batas_atas)]
print(f"Ditemukan {len(outlier)} data anomali ekstrim.")
```
**Tugas Mahasiswa:** Buat plot yang menampilkan data asli, grafik data yang sudah di-smoothing, dan berikan titik/tanda merah khusus pada bagian yang terdeteksi sebagai outlier IQR.

---

## Bagian 2: EDA, Edge Computing, dan Machine Learning Dasar (Modul 6 - 11)

---

### MODUL 6: Exploratory Data Analysis (EDA) Pertanian
**Tujuan:** Mahasiswa mampu menganalisis korelasi antar variabel sensor dan membuat visualisasi statistik (*Heatmap*).

#### 6.1 Membuat Matriks Korelasi (Correlation Matrix)
Kita akan melihat apakah ada hubungan (korelasi) antara Suhu Udara dengan Kelembaban Tanah. Nilai mendekati 1 berarti korelasi positif kuat, -1 berarti korelasi negatif kuat.

```python
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np

# Generate dummy data multi-variabel untuk simulasi EDA
np.random.seed(42)
suhu = np.random.normal(30, 3, 100)
# Asumsi: Semakin panas suhu, kelembaban tanah semakin turun (Korelasi Negatif)
kelembaban = 100 - (suhu * 1.5) + np.random.normal(0, 5, 100)
curah_hujan = np.random.exponential(5, 100)

df_eda = pd.DataFrame({
    'Suhu (C)': suhu,
    'Kelembaban Tanah (%)': kelembaban,
    'Curah Hujan (mm)': curah_hujan
})

# Menghitung korelasi Pearson
korelasi = df_eda.corr()

# Visualisasi menggunakan Heatmap
plt.figure(figsize=(6, 5))
sns.heatmap(korelasi, annot=True, cmap='coolwarm', vmin=-1, vmax=1)
plt.title("Matriks Korelasi Sensor Pertanian")
plt.show()
```

---

### MODUL 7: Praktik Edge Scripting (Pengenalan)
**Tujuan:** Menulis kode Python seefisien mungkin untuk dijalankan pada perangkat dengan memori terbatas (SBC/Raspberry Pi).

#### 7.1 Menghindari Library Berat di Edge
Di *Edge device*, memuat library `pandas` mungkin memakan waktu dan RAM terlalu besar. Kita akan menggunakan *built-in library* bawaan Python.
```python
import csv
import time
import statistics

# Simulasi membaca file CSV satu per satu baris (streaming) tanpa load seluruh data ke RAM
total_suhu = []
with open('data_sensor_dummy.csv', mode='r') as file:
    reader = csv.DictReader(file)
    for row in reader:
        try:
            suhu = float(row['suhu'])
            total_suhu.append(suhu)
            
            # Jika sudah mengumpulkan 10 data, hitung rata-rata dan bersihkan memori
            if len(total_suhu) == 10:
                rata_rata = statistics.mean(total_suhu)
                print(f"Mengirim agregat ke Cloud: Rata-rata Suhu {rata_rata:.2f}")
                total_suhu.clear() # Kosongkan array untuk menghemat RAM
        except ValueError:
            pass # Abaikan jika ada missing value
```

---

### MODUL 8: Ujian Tengah Semester (Praktik)
*(Modul ini dikosongkan. Mahasiswa akan diberikan studi kasus dataset acak oleh dosen pengampu dan diminta melakukan proses Ingestion, Cleaning, hingga EDA secara mandiri selama 170 menit.)*

---

### MODUL 9: Pengantar Machine Learning (Scikit-Learn)
**Tujuan:** Memahami konsep Data Splitting (Train & Test) serta Regresi Linear Dasar.

#### 9.1 Data Splitting dan Pelatihan Model Regresi
```python
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error

# Menggunakan df_eda dari Modul 6
X = df_eda[['Suhu (C)']] # Fitur (Feature / Variabel Independen)
y = df_eda['Kelembaban Tanah (%)'] # Target (Label / Variabel Dependen)

# Membagi data: 80% untuk Train (Belajar), 20% untuk Test (Ujian)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Inisialisasi dan Latih Model (Training)
model = LinearRegression()
model.fit(X_train, y_train)

# Lakukan Prediksi pada data Test
prediksi = model.predict(X_test)

# Hitung tingkat error (RMSE)
rmse = mean_squared_error(y_test, prediksi, squared=False)
print(f"Tingkat Error (RMSE): {rmse:.2f} %")
```

---

### MODUL 10: Time-Series Forecasting (Smart Farming)
**Tujuan:** Melakukan peramalan (*Forecasting*) suhu udara ke depan menggunakan metode Autoregression sederhana (menggunakan data suhu masa lalu untuk memprediksi masa depan).

```python
import pandas as pd
from sklearn.linear_model import LinearRegression

# Persiapan Data
df = pd.read_csv("data_sensor_dummy.csv").dropna()

# Membuat fitur baru: Suhu 1 jam yang lalu (Lag-1)
df['Suhu_Kemarin'] = df['suhu'].shift(1)
df = df.dropna()

X = df[['Suhu_Kemarin']]
y = df['suhu']

# Latih model
model_ts = LinearRegression()
model_ts.fit(X, y)

# Prediksi jika suhu 1 jam lalu adalah 30 derajat
suhu_prediksi = model_ts.predict([[30.0]])
print(f"Berdasarkan pola historis, prediksi suhu 1 jam ke depan adalah {suhu_prediksi[0]:.2f} C")
```

---

### MODUL 11: Deteksi Anomali dengan Machine Learning
**Tujuan:** Menggunakan algoritma *Isolation Forest* (Unsupervised Learning) untuk mendeteksi kejanggalan pada banyak sensor secara bersamaan.

#### 11.1 Implementasi Isolation Forest
```python
from sklearn.ensemble import IsolationForest
import numpy as np

# Menyiapkan data sensor (suhu dan curah hujan)
X_anomali = df_eda[['Suhu (C)', 'Curah Hujan (mm)']]

# Menyuntikkan 1 data sangat aneh secara manual di akhir (Suhu sangat panas, curah hujan sangat tinggi)
X_anomali.loc[101] = [45.0, 100.0]

# Inisialisasi model Isolation Forest (asumsi sekitar 5% dari data kita adalah anomali)
iso_forest = IsolationForest(contamination=0.05, random_state=42)
iso_forest.fit(X_anomali)

# Memprediksi Anomali: 1 = Normal, -1 = Anomali
prediksi_anomali = iso_forest.predict(X_anomali)
df_eda.loc[df_eda.index.isin(X_anomali.index), 'Status_Anomali'] = prediksi_anomali

# Tampilkan data yang terdeteksi sebagai anomali (-1)
hasil_anomali = df_eda[df_eda['Status_Anomali'] == -1]
print("Tabel Data Anomali yang Ditemukan oleh AI:")
print(hasil_anomali)
```

---

## Bagian 3: Database Time-Series, Dashboarding, dan Integrasi (Modul 12 - 14)

---

### MODUL 12: Pengenalan Time-Series Database (InfluxDB)
**Tujuan:** Mahasiswa mampu menyimpan data *time-series* berkecepatan tinggi ke dalam InfluxDB menggunakan Python.

#### 12.1 Instalasi dan Setup InfluxDB
1. Unduh dan instal InfluxDB v2.x (menyesuaikan OS masing-masing).
2. Jalankan service `influxd` melalui terminal.
3. Buka browser dan akses antarmuka InfluxDB di `http://localhost:8086`.
4. Lakukan setup awal: Buat *Organization* (contoh: `Politani`), *Bucket* (contoh: `sensor_pertanian`), dan *generate API Token*.

#### 12.2 Menulis Data dari Python ke InfluxDB
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
    
    # Format data Point untuk InfluxDB (Measurement, Tags, Fields)
    p = influxdb_client.Point("iklim_mikro") \
        .tag("lokasi", "Lahan_A") \
        .field("suhu_udara", suhu) \
        .field("kelembaban_tanah", kelembaban)
    
    write_api.write(bucket=bucket, org=org, record=p)
    print(f"Mengirim data -> Suhu: {suhu} C, Kelembaban: {kelembaban}%")
    time.sleep(2)

print("Penulisan selesai.")
```
**Tugas Mahasiswa:** Ubah script Modul 2 (*Subscriber* MQTT) agar data yang ditangkap dari broker langsung disimpan secara otomatis ke dalam InfluxDB!

---

### MODUL 13: Desain Dashboard Grafana
**Tujuan:** Menampilkan visualisasi data langsung (*live*) dari InfluxDB dan memasang sistem *Alerting*.

#### 13.1 Instalasi Grafana dan Koneksi Data Source
1. Unduh dan instal Grafana. Jalankan service-nya dan akses di `http://localhost:3000` (User/Pass default: `admin/admin`).
2. Masuk ke menu **Connections > Data Sources > Add Data Source**.
3. Pilih **InfluxDB**. Ubah query language ke **Flux** (jika menggunakan InfluxDB v2).
4. Masukkan URL `http://localhost:8086`, masukkan Token, Default Bucket (`sensor_pertanian`), dan Organization (`Politani`). Klik *Save & Test*.

#### 13.2 Membuat Panel Time-Series
1. Masuk ke menu **Dashboards > New Dashboard > Add Visualization**.
2. Pilih Data Source InfluxDB yang baru dibuat.
3. Di bagian Query (menggunakan *Query Builder*), pilih `iklim_mikro` > `suhu_udara`.
4. Di bagian kanan layar, ubah tipe panel menjadi **Time series**. Beri judul "Pemantauan Suhu Real-time".
5. Ulangi langkah di atas untuk menambahkan panel tipe **Gauge** yang menunjukkan suhu terkini secara spesifik.

#### 13.3 Mengatur Alert (Sistem Peringatan)
1. Edit panel Time-series Suhu yang sudah dibuat.
2. Pergi ke tab **Alerting > Create Alert rule**.
3. Buat kondisi: *Jika Suhu (A) melampaui (*Is above*) 33.0 derajat Celcius selama 1 menit.*
4. Pada bagian *Contact points*, tambahkan **Telegram Webhook** (Mahasiswa harus membuat Bot Telegram melalui BotFather terlebih dahulu dan mendapatkan Chat ID).

---

### MODUL 14: Integrasi End-to-End (PjBL)
**Tujuan:** Menggabungkan seluruh tahapan (*Ingestion, Cleaning/ML, Storage, Dashboard*) menjadi satu sistem utuh yang berjalan otomatis 24/7.

#### 14.1 Arsitektur Final Proyek Akhir
Mahasiswa tidak lagi mengerjakan script terpisah, melainkan membuat file *orchestrator* utama `main_pipeline.py`.

**Alur Logika Program:**
1. **MQTT Subscribe:** Menunggu paket data sensor (atau *dummy* data) masuk.
2. **Data Cleaning & ML (On-the-fly):**
   - Periksa apakah nilainya kosong (NaN).
   - Masukkan ke model ML (*Isolation Forest*) yang sudah dilatih (di-*load* menggunakan modul `pickle` Python).
   - Jika anomali = -1 (berbahaya) atau 1 (normal).
3. **Data Storage:** Simpan nilai suhu, kelembaban, dan "status anomali" tersebut ke InfluxDB.
4. **Visualisasi (Grafana):**
   - Grafana membaca "status anomali". Jika nilainya -1, Grafana memicu *Webhook* otomatis ke Telegram Manajer Kebun.

**Tugas Evaluasi Akhir:** Kelompok akan dinilai berdasarkan keberhasilan demo arsitektur di atas di depan dosen pengampu (berdasarkan Rubrik pada Rencana Tugas Mahasiswa / RTM).
