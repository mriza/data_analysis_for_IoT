# Materi Praktikum Pekan 4: Membaca Data & Exploratory Data Analysis (EDA)

## 1. Tujuan Praktikum
- Mahasiswa mampu memuat (*load*) data *time-series* dari format CSV menggunakan pustaka Pandas.
- Mahasiswa mampu mengeksplorasi ringkasan statistik dasar dan mendeteksi anomali pada dataset sensor.
- Mahasiswa mampu memvisualisasikan data sensor (menggunakan plot garis dan *scatter plot*) untuk memonitor tren.
- Mahasiswa mampu melakukan analisis korelasi antar berbagai variabel sensor (suhu, kelembaban, curah hujan) menggunakan matriks korelasi dan *Heatmap*.

## 2. Persiapan Alat & Prasyarat
- **Software:** Jupyter Notebook atau VS Code dengan ekstensi Jupyter.
- **Library Python:** `pandas`, `matplotlib`, `seaborn`, `numpy`.
- File dataset `data_sensor_dummy.csv` (dapat dibuat secara otomatis melalui *script* pada Langkah 0).

## 3. Dasar Teori Singkat
**Exploratory Data Analysis (EDA)** adalah tahap di mana *Data Analyst* "berkenalan" dengan data. Melalui fungsi statistik dasar dan plot grafis, analis mencari tahu pola umum, melihat batas atas/bawah nilai sensor, mendeteksi nilai yang hilang (kosong), dan membuktikan hipotesis korelasi. Korelasi sendiri berkisar antara -1 (berlawanan arah sempurna), 0 (tidak ada hubungan), dan 1 (searah secara sempurna).

## 4. Langkah Kerja (Detail & Anotasi Kode)

### Langkah 0: Membuat Dataset Time-Series (Sumber Data)
Jika Anda belum memiliki file `data_sensor_dummy.csv`, Anda dapat membuatnya terlebih dahulu dengan menjalankan *script* Python berikut di *cell* Jupyter Notebook baru. *Script* ini akan menghasilkan data suhu tiruan (*dummy*) selama satu pekan.

```python
import pandas as pd
import numpy as np

# Mengatur seed agar data acak selalu sama
np.random.seed(42)

# Membuat rentang waktu (1 pekan, per jam)
waktu = pd.date_range(start="2023-10-01", end="2023-10-07 23:00:00", freq="h")

# Membuat data suhu acak (misalnya rata-rata 28 derajat)
suhu = np.random.normal(loc=28, scale=3, size=len(waktu))

# Menyimpan ke dalam DataFrame
df_sensor = pd.DataFrame({
    'timestamp': waktu,
    'suhu': suhu
})

# Menyimpan DataFrame ke file CSV
df_sensor.to_csv('data_sensor_dummy.csv', index=False)
print("File 'data_sensor_dummy.csv' berhasil dibuat!")
```

### Langkah 1: Memuat Data (Loading Data)
Buat notebook baru bernama `modul_eda.ipynb`. Setelah file CSV tersedia, kita muat data tersebut. Pastikan kita memberitahu Pandas bahwa kolom 'timestamp' adalah Waktu.

```python
import pandas as pd
import matplotlib.pyplot as plt

# parse_dates=True memerintahkan Pandas membaca kolom tanggal sebagai tipe objek datetime, bukan string teks biasa.
# index_col='timestamp' menjadikan kolom waktu sebagai baris sumbu (index) dari tabel.
df = pd.read_csv("data_sensor_dummy.csv", parse_dates=['timestamp'], index_col='timestamp')

# Tampilkan 5 data pertama untuk memastikan formatnya benar
display(df.head())
```

### Langkah 2: Inspeksi Statistik Dasar (Inti dari EDA)
Sebelum menggambar grafik, kita harus memeriksa "kesehatan" angka-angka kita.

```python
# 1. Melihat informasi tipe data dan jumlah data kosong (Non-Null Count)
print("--- Informasi Dataset ---")
df.info()

# 2. Mengecek secara eksplisit apakah ada data yang hilang (Missing Values)
print("\n--- Jumlah Data Kosong ---")
print(df.isnull().sum())

# 3. Melihat ringkasan statistik (Rata-rata, Min, Max, Kuartil)
print("\n--- Ringkasan Statistik ---")
display(df.describe())
```
*Penjelasan:* Dari hasil `df.describe()`, Anda bisa langsung melihat suhu terendah (Min) dan tertinggi (Max). Jika sensor suhu mencatat angka -100 atau 9999, Anda langsung tahu bahwa sensor tersebut rusak (anomali) tanpa perlu melihat grafiknya terlebih dahulu!

### Langkah 3: Visualisasi Data Time-Series
Setelah yakin angkanya masuk akal, mari kita visualisasikan fluktuasi suhunya.

```python
# Konfigurasi ukuran kanvas gambar (lebar, tinggi)
plt.figure(figsize=(12, 5))

# Plot sumbu X (indeks waktu) dan Sumbu Y (nilai suhu)
plt.plot(df.index, df['suhu'], label='Suhu Udara', color='#E74C3C', linewidth=1.5)

# Penambahan aksesoris visual agar profesional
plt.title("Grafik Suhu Sensor (Simulasi 1 Pekan)", fontsize=14, fontweight='bold')
plt.xlabel("Waktu (Jam)", fontsize=12)
plt.ylabel("Derajat Celcius (°C)", fontsize=12)
plt.legend(loc='upper right')
plt.grid(True, linestyle='--', alpha=0.6)

plt.tight_layout()
plt.show() # Tampilkan gambar!
```

### Langkah 4: Simulasi Korelasi antar Sensor (Matriks Korelasi)
Dalam IoT pertanian, beberapa sensor saling terkait secara fisik. Kita akan menambah data tiruan (Kelembaban dan Hujan) untuk membuktikan relasinya.

```python
import seaborn as sns

# Ambil data suhu yang sudah ada
suhu_eda = df['suhu'].values

# Simulasi: Kelembaban sangat dipengaruhi suhu. Semakin panas, makin kering (KORELASI NEGATIF)
np.random.seed(42)
kelembaban_eda = 100 - (suhu_eda * 1.5) + np.random.normal(0, 5, len(suhu_eda))

# Curah hujan terjadi sangat acak (eksponensial)
curah_hujan_eda = np.random.exponential(5, len(suhu_eda))

# Gabungkan menjadi DataFrame baru
df_korelasi = pd.DataFrame({
    'Suhu': suhu_eda,
    'Kelembaban_Tanah': kelembaban_eda,
    'Curah_Hujan': curah_hujan_eda
})

# Menghitung nilai Pearson Correlation
# Ini akan mengembalikan matriks (tabel silang) 3x3 berisi angka dari -1 hingga 1
matriks_corr = df_korelasi.corr()
display(matriks_corr)
```

### Langkah 5: Visualisasi Heatmap (Peta Panas)
Angka di tabel korelasi sulit dianalisis cepat. Kita ubah matriks tersebut menjadi balok warna (Heatmap).

```python
plt.figure(figsize=(6, 5))

# annot=True menampilkan angka korelasi di dalam kotak warna
# cmap='coolwarm' (biru untuk korelasi negatif, merah untuk positif)
sns.heatmap(matriks_corr, annot=True, cmap='coolwarm', vmin=-1, vmax=1, fmt=".2f", linewidths=.5)

plt.title("Korelasi Antar Sensor Pertanian", fontweight='bold')
plt.show()
```

## 5. Hasil Eksperimen yang Diharapkan
- Melalui tabel `describe()`, Anda dapat mengetahui profil matematis suhu ruangan dengan presisi.
- Anda akan melihat plot garis (*line plot*) merah yang mewakili suhu udara berfluktuasi seiring waktu.
- Pada plot *Heatmap*, perpotongan baris *Suhu* dan kolom *Kelembaban_Tanah* akan berwarna biru tua dengan nilai negatif kuat (mendekati -1), yang membuktikan secara data bahwa saat suhu naik, kelembaban menurun.

## 6. Troubleshooting
- **Error `ModuleNotFoundError: No module named 'seaborn'`:** Eksekusi `!pip install seaborn` langsung di dalam sel (*cell*) Jupyter Notebook Anda.
- **Plot tidak tampil di IDE (seperti PyCharm/VSCode biasa tanpa Jupyter):** Pastikan memanggil `plt.show()` di akhir baris plotting.

## 7. Tugas Mandiri / Tantangan
Buat visualisasi **Scatter Plot** (`plt.scatter`) dengan sumbu X adalah Suhu dan sumbu Y adalah Kelembaban_Tanah menggunakan `df_korelasi`. Beri warna titik data berdasarkan nilai Curah Hujan (buatlah argumen `c=df_korelasi['Curah_Hujan']` dan gunakan `cmap='Blues'`). Amati pola penyebarannya, apakah membentuk garis menurun yang membuktikan korelasi negatif?


