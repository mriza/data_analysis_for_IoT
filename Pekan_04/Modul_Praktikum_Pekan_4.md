---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Praktikum Pekan 4: Membaca Data & Exploratory Data Analysis (EDA) dengan Pandas dan Seaborn"
---

# Materi Praktikum Pekan 4: Membaca Data & Exploratory Data Analysis (EDA) dengan Pandas dan Seaborn

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 4 (Praktikum Terbimbing — $1 \times 170$ Menit)  
**Capaian Pembelajaran (Sub-CPMK 4):** Mahasiswa mampu memuat (*load*) data *time-series* dari format CSV menggunakan pustaka Pandas, melakukan audit statistik deskriptif, memvisualisasikan tren dan distribusi sensor, serta membangun matriks korelasi dan *Heatmap* dari dataset *Smart Agriculture*.

---

## 1. Tujuan Praktikum
1. Mahasiswa mampu memuat (*load*) data *time-series* dari format CSV menggunakan pustaka Pandas dan mengonversi kolom waktu menjadi `DatetimeIndex`.
2. Mahasiswa mampu mengeksplorasi ringkasan statistik deskriptif (`info()`, `isnull()`, `describe()`) untuk mengaudit kesehatan dataset sensor.
3. Mahasiswa mampu melakukan penyeragaman frekuensi waktu (*resampling*) dan memvisualisasikan data sensor menggunakan *Line Plot*, *Histogram*, dan *Scatter Plot*.
4. Mahasiswa mampu melakukan analisis korelasi multivariat antar berbagai variabel sensor (suhu, kelembaban tanah, intensitas cahaya, curah hujan) menggunakan matriks korelasi Pearson dan visualisasi *Heatmap* Seaborn.

---

## 2. Persiapan Alat & Prasyarat
- **Perangkat Lunak:** Jupyter Notebook (Lokal / Ubuntu Server VM) atau Google Colab.
- **Pustaka Python:** `pandas`, `numpy`, `matplotlib`, `seaborn`.
  *(Jika belum terpasang di dalam virtual environment Anda, jalankan `pip install pandas numpy matplotlib seaborn`).*
- **Dataset:** Berkas `data_sensor_dummy.csv` (dapat dibangkitkan melalui skrip pada Langkah 0 atau menggunakan `generate_data_sensor.py` yang tersedia di folder `Pekan_04`).

---

## 3. Dasar Teori Singkat
**Exploratory Data Analysis (EDA)** adalah tahap investigasi awal di mana seorang *Data Analyst* memeriksa struktur, kebersihan, dan pola hubungan di dalam data sebelum membangun model prediktif. Tiga tahapan utama dalam praktikum EDA meliputi:
1. **Inspeksi Struktur & Kualitas Data:** Memastikan tipe data setiap kolom benar (terutama `datetime64[ns]` untuk waktu dan `float64` untuk bacaan sensor) serta menghitung jumlah sel kosong (`NaN`).
2. **Audit Statistik Deskriptif:** Memeriksa nilai `mean`, `std`, `min`, `25%` ($Q_1$), `50%` (Median), `75%` ($Q_3$), dan `max`. Jika sensor suhu di kebun tropis memiliki nilai `min = -100°C` atau `max = 999°C`, kita langsung mendeteksi keberadaan anomali perangkat keras.
3. **Eksplorasi Visual & Korelasi Multivariat:** Mengukur koefisien korelasi Pearson ($r \in [-1, 1]$) antar-sensor dan memetakannya ke dalam **Heatmap** untuk menemukan pasangan parameter fisik yang bergerak searah ($r > 0$) atau berlawanan arah ($r < 0$).

---

## 4. Langkah Kerja Praktikum (Detail & Anotasi Kode)

### Langkah 0: Membangkitkan Dataset Sensor Smart Agriculture (`data_sensor_dummy.csv`)
Buat notebook baru bernama `Praktikum_Pekan4_EDA.ipynb`. Jalankan kode pada sel pertama berikut untuk menghasilkan dataset simulasi stasiun cuaca pertanian selama 1 pekan (168 jam):

```python
import pandas as pd
import numpy as np

# Mengatur seed agar data acak selalu konsisten di setiap komputer
np.random.seed(42)

# Membuat rentang waktu 1 pekan (168 jam) dengan frekuensi per jam ('h')
waktu = pd.date_range(start="2026-10-01 00:00:00", end="2026-10-07 23:00:00", freq="h")

# Simulasi pola diurnal (siang panas, malam sejuk) + derau acak normal
jam = waktu.hour
suhu = 27.0 + 5.0 * np.sin((jam - 6) * np.pi / 12) + np.random.normal(loc=0, scale=1.2, size=len(waktu))

# Menyimpan ke dalam DataFrame dan mengekspor ke CSV
df_sensor = pd.DataFrame({
    'timestamp': waktu,
    'suhu': np.round(suhu, 2)
})

df_sensor.to_csv('data_sensor_dummy.csv', index=False)
print("File 'data_sensor_dummy.csv' berhasil dibuat dengan jumlah baris:", len(df_sensor))
```

---

### Langkah 1: Memuat Data (*Data Loading*) & Pengaturan Indeks Waktu
Setelah berkas CSV tersedia, muat data tersebut ke dalam Pandas DataFrame dan tetapkan kolom `timestamp` sebagai indeks waktu utama:

```python
import pandas as pd
import matplotlib.pyplot as plt

# parse_dates=['timestamp'] memerintahkan Pandas mengubah teks waktu menjadi objek datetime64
# index_col='timestamp' menjadikan kolom waktu sebagai sumbu indeks utama tabel
df = pd.read_csv("data_sensor_dummy.csv", parse_dates=['timestamp'], index_col='timestamp')

# Menampilkan 5 baris pertama dan 5 baris terakhir
print("--- 5 Baris Pertama Dataset ---")
display(df.head())
```

---

### Langkah 2: Inspeksi Struktur & Statistik Deskriptif (Inti dari EDA)
Sebelum menggambar grafik, lakukan audit terhadap tipe data, nilai kosong, dan profil statistik:

```python
# 1. Memeriksa tipe data, rentang indeks waktu, dan penggunaan memori
print("--- Informasi Struktur Dataset ---")
df.info()

# 2. Mengecek secara eksplisit jumlah data yang hilang (Missing Values / NaN)
print("\n--- Jumlah Data Kosong per Kolom ---")
print(df.isnull().sum())

# 3. Melihat ringkasan statistik deskriptif (Count, Mean, Std, Min, Q1, Median, Q3, Max)
print("\n--- Ringkasan Statistik Deskriptif ---")
display(df.describe())
```
*Analisis Kode:*
- Bandingkan nilai **`mean`** (Rata-rata) dan **`50%`** (Median). Apabila kedua angka tersebut sangat berdekatan, maka distribusi data suhu cenderung simetris dan bebas dari *outlier* sepihak yang ekstrem.
- Perhatikan nilai **`min`** dan **`max`**. Pastikan keduanya berada dalam rentang masuk akal iklim mikro pertanian (sekitar $20^\circ\text{C} - 35^\circ\text{C}$).

---

### Langkah 3: Visualisasi Tren Time-Series & Resampling Harian
Mari kita visualisasikan fluktuasi suhu per jam sekaligus membandingkannya dengan rata-rata harian hasil operasi `.resample('1D').mean()`:

```python
# Menghitung rata-rata suhu harian (Downsampling dari per-jam ke per-hari)
df_harian = df['suhu'].resample('1D').mean()

plt.figure(figsize=(12, 5))

# Plot data asli per jam
plt.plot(df.index, df['suhu'], label='Suhu Per Jam (Raw)', color='#E74C3C', linewidth=1.2, alpha=0.8)

# Plot rata-rata harian
plt.plot(df_harian.index, df_harian, label='Rata-Rata Harian (Resampled 1D)', color='#2C3E50', marker='o', linewidth=2.5, linestyle='--')

plt.title("Grafik Pemantauan Suhu Udara Smart Greenhouse (1 Pekan)", fontsize=13, fontweight='bold')
plt.xlabel("Waktu (Tanggal & Jam)", fontsize=11)
plt.ylabel("Suhu Udara (°C)", fontsize=11)
plt.legend(loc='upper right')
plt.grid(True, linestyle='--', alpha=0.6)
plt.tight_layout()
plt.show()
```

---

### Langkah 4: Eksplorasi Korelasi Multivariat Antar-Sensor Pertanian
Di lahan pertanian nyata, parameter-parameter lingkungan saling berkaitan erat. Kita akan membangun dataset multivariat yang mencakup 4 sensor: **Suhu Udara**, **Intensitas Cahaya (Lux)**, **Kelembaban Tanah (%)**, dan **Curah Hujan (mm)**, lalu menghitung matriks korelasi Pearson-nya:

```python
import seaborn as sns

suhu_eda = df['suhu'].values
np.random.seed(42)

# 1. Intensitas Cahaya berkorelasi POSITIF kuat dengan Suhu Udara
intensitas_cahaya = np.clip((suhu_eda - 20) * 4500 + np.random.normal(0, 5000, len(suhu_eda)), 0, None)

# 2. Kelembaban Tanah berkorelasi NEGATIF kuat dengan Suhu Udara (makin panas, penguapan makin tinggi)
kelembaban_tanah = 105 - (suhu_eda * 1.6) + np.random.normal(0, 3.5, len(suhu_eda))

# 3. Curah Hujan berdistribusi Eksponensial (Right-Skewed) dan independen/acak terhadap suhu sesaat
curah_hujan = np.random.exponential(scale=3.0, size=len(suhu_eda))

# Menggabungkan seluruh variabel ke dalam DataFrame Multivariat
df_korelasi = pd.DataFrame({
    'Suhu_Udara': suhu_eda,
    'Intensitas_Cahaya': intensitas_cahaya,
    'Kelembaban_Tanah': kelembaban_tanah,
    'Curah_Hujan': curah_hujan
}, index=df.index)

print("--- Ringkasan Statistik Dataset Multivariat ---")
display(df_korelasi.describe().round(2))

# Menghitung Matriks Korelasi Pearson (r)
matriks_corr = df_korelasi.corr(method='pearson')
print("\n--- Matriks Korelasi Pearson (4x4) ---")
display(matriks_corr.round(3))
```

---

### Langkah 5: Visualisasi Heatmap (Peta Panas Korelasi) & Distribusi
Untuk memudahkan pembacaan matriks angka di atas secara instan, gunakan `sns.heatmap`:

```python
fig, axes = plt.subplots(1, 2, figsize=(14, 5))

# Panel Kiri: Heatmap Korelasi Pearson
sns.heatmap(
    matriks_corr,
    annot=True,           # Menampilkan angka koefisien korelasi di tiap sel
    cmap='coolwarm',      # Biru tua = korelasi negatif (-1), Merah tua = korelasi positif (+1)
    vmin=-1, vmax=1,      # Mengunci skala warna dari -1 hingga +1
    fmt=".2f",            # Pembulatan 2 angka di belakang koma
    linewidths=0.8,
    ax=axes[0]
)
axes[0].set_title("Heatmap Korelasi Antar-Sensor Pertanian", fontweight='bold')

# Panel Kanan: Histogram Distribusi Curah Hujan (Contoh Data Right-Skewed)
sns.histplot(df_korelasi['Curah_Hujan'], kde=True, color='#3498DB', ax=axes[1])
axes[1].axvline(df_korelasi['Curah_Hujan'].mean(), color='red', linestyle='--', label=f"Mean: {df_korelasi['Curah_Hujan'].mean():.2f}")
axes[1].axvline(df_korelasi['Curah_Hujan'].median(), color='green', linestyle='-', label=f"Median: {df_korelasi['Curah_Hujan'].median():.2f}")
axes[1].set_title("Distribusi Curah Hujan (Right-Skewed: Mean > Median)", fontweight='bold')
axes[1].legend()

plt.tight_layout()
plt.show()
```

---

## 5. Hasil Eksperimen yang Diharapkan
1. Pada `Heatmap`, sel perpotongan **Suhu_Udara** dan **Intensitas_Cahaya** berwarna merah tua dengan nilai $r > +0,80$ (korelasi positif kuat).
2. Sel perpotongan **Suhu_Udara** dan **Kelembaban_Tanah** berwarna biru tua dengan nilai $r < -0,75$ (korelasi negatif kuat: saat suhu meningkat, kelembaban tanah menyusut).
3. Pada histogram **Curah_Hujan**, terlihat jelas kurva condong ke kanan (*right-skewed*) di mana garis merah (*Mean*) tertarik ke kanan melampaui garis hijau (*Median*).

---

## 6. Panduan Troubleshooting
- **Error `ModuleNotFoundError: No module named 'seaborn'`:** Jalankan `!pip install seaborn` langsung di dalam sel Jupyter Notebook Anda.
- **Error `ValueError: could not convert string to float` saat memanggil `df.corr()`:** Terjadi jika di dalam DataFrame masih terdapat kolom bertipe teks (misal kolom `timestamp` belum dijadikan indeks atau ada kolom `nama_lokasi`). Tambahkan argumen `df.corr(numeric_only=True)`.

---

## 7. Tugas Mandiri Praktikum
1. **Visualisasi Scatter Plot Multidimensi:**
   Buatlah visualisasi **Scatter Plot** (`plt.scatter` atau `sns.scatterplot`) menggunakan `df_korelasi` dengan sumbu X adalah `Suhu_Udara`, sumbu Y adalah `Kelembaban_Tanah`, dan warna titik (`c` atau `hue`) merepresentasikan `Intensitas_Cahaya`. Tambahkan garis tren regresi menggunakan `sns.regplot` dan jelaskan arti kemiringan garis tersebut dalam 1 paragraf narasi!
2. **Eksplorasi Resampling Multi-Frekuensi:**
   Tuliskan satu baris rantai kode (*method chaining*) Pandas pada `df_korelasi['Suhu_Udara']` yang menyeragamkan data ke interval **6 Jam (`'6h'`)**, menghitung nilai maksimumnya, lalu menampilkannya dalam grafik batang (*bar chart*)!

---

## 8. Referensi & Acuan Materi Pekan Ini

1. **McKinney, W. (2022).** *Python for Data Analysis: Data Wrangling with Pandas, NumPy, and Jupyter* (3rd ed.). O'Reilly Media. (ISBN-13: 978-1098104030) — **Bab 5 (Summarizing and Computing Descriptive Statistics) & Bab 9 (Plotting and Visualization with Matplotlib & Seaborn).**  
2. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730) — **Bab 4 (Exploring IoT Data).**  
3. **Fawcett, T., & Provost, F. (2013).** *Data Science for Business*. O'Reilly Media. (ISBN-13: 978-1449361327)  
4. **Pandas Development Team. (2024).** *Pandas Documentation*. Tersedia: [https://pandas.pydata.org/docs/](https://pandas.pydata.org/docs/)  
5. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
