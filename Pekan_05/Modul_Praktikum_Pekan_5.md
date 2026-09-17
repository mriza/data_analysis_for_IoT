---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Praktikum Pekan 5: Pra-pemrosesan Data (Cleaning, Imputation, Smoothing)"
---

# Materi Praktikum Pekan 5: Pra-pemrosesan Data (Cleaning, Imputation, Smoothing)

## 1. Tujuan Praktikum
- Mahasiswa mampu menyimulasikan data sensor yang rusak (*noise, missing values, outliers*).
- Mahasiswa dapat mempraktekkan pengisian data kosong (*imputation*) menggunakan metode ffill, bfill, dan interpolasi.
- Mahasiswa dapat mengaplikasikan filter *Rolling Mean* (Moving Average) untuk menghaluskan data sensor yang bergerigi.
- Mahasiswa dapat menggunakan visualisasi untuk membandingkan data kotor dengan data yang telah dibersihkan.

## 2. Persiapan Praktikum
- Buka dan aktifkan lingkungan virtual (venv) Jupyter Notebook Anda seperti biasa.
- Buat *notebook* baru dengan nama `modul_preprocessing.ipynb`.
- *Library* utama yang digunakan hari ini: `pandas`, `numpy`, dan `matplotlib.pyplot`.

## 3. Langkah Kerja (Detail & Anotasi Kode)

### Langkah 1: Membangun Data Sensor "Kotor" Secara Sengaja
Berbeda dengan pekan lalu yang menggunakan file CSV yang rapi, hari ini kita akan menggunakan *script* Python untuk merakit (*generate*) data gelombang sinus (menyerupai pola alami suhu harian). Kemudian gelombang murni ini akan kita "rusak" secara artifisial.

Ketikkan blok kode berikut di *cell* Jupyter pertama Anda:
```python
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Kunci generator acak agar hasil 'rusak' kita selalu sama di setiap komputer
np.random.seed(10)

# Membuat 200 titik waktu dengan interval 1 jam
waktu = pd.date_range(start='2023-11-01', periods=200, freq='h')

# 1. Membuat pola dasar alam (Sinyal Asli): Gelombang Sinus murni
sinyal_asli = np.sin(np.linspace(0, 20, 200)) * 5 + 25 

# 2. Menambahkan "Noise" (Fluktuasi kelistrikan mikro pada sensor)
noise = np.random.normal(0, 0.8, 200)
sinyal_kotor = sinyal_asli + noise

# 3. Membuat tabel DataFrame Pandas
df = pd.DataFrame({'timestamp': waktu, 'suhu': sinyal_kotor})
df.set_index('timestamp', inplace=True)

# 4. Skenario Outlier: Korsleting Sensor!
# Suhu tiba-tiba meloncat ke 80 derajat dan anjlok ke -10 derajat
df.loc[df.index[50], 'suhu'] = 80.0
df.loc[df.index[150], 'suhu'] = -10.0

# 5. Skenario Missing Values: Sensor Putus Koneksi!
# Baris ke 80 sampai 95 koneksinya hilang, datanya menjadi kosong (NaN)
df.iloc[80:95, 0] = np.nan

# Mari kita visualisasikan betapa kotornya data ini
plt.figure(figsize=(14, 5))
plt.plot(df.index, df['suhu'], label='Data Sensor Mentah (Sangat Kotor)', color='red', marker='.')
plt.title("Langkah 1: Data Sensor Sebelum Tahap Pre-processing")
plt.legend()
plt.show()
```

### Langkah 2: Mengamputasi Outlier (Batas Logis/Thresholding)
Anda akan melihat adanya dua lonjakan ekstrim yang tajam menembus 80°C dan terjun bebas ke -10°C pada plot sebelumnya. Secara iklim, hal ini mustahil.

```python
# Mari cek keanehan statistik terlebih dahulu
print("Statistik sebelum dibersihkan:")
display(df.describe())

# Asumsi fisika: Suhu ruangan terbuka di wilayah tersebut hanya berkisar antara 10 C hingga 40 C. 
# Jika sensor mencatat di luar batas itu, itu PASTI error. 
# Kita ubah angka error tersebut menjadi kosong (NaN) agar bisa di-interpolasi nanti.
batas_bawah = 10
batas_atas = 40

# Menggunakan fungsi lambda: "Ganti x dengan NaN jika x < batas_bawah atau x > batas_atas, selain itu biarkan x"
df['suhu_no_outlier'] = df['suhu'].apply(lambda x: np.nan if x < batas_bawah or x > batas_atas else x)

# Cek hasil pembuangan outlier
plt.figure(figsize=(14, 5))
plt.plot(df.index, df['suhu_no_outlier'], label='Suhu (Tanpa Outlier Ekstrem)', color='orange')
plt.title("Langkah 2: Setelah Outlier Dibuang (Perhatikan, kini terdapat lubang tambahan)")
plt.legend()
plt.show()
```

### Langkah 3: Menangani Missing Values (Teknik Imputation)
Kini dataset kita penuh dengan lubang kosong: Lubang besar akibat koneksi terputus (baris 80-95) dan lubang kecil akibat amputasi Outlier di Langkah 2. Kita harus menambalnya karena algoritma tidak bisa memproses nilai kosong (`NaN`).

```python
# 1. Metode Forward Fill (Menambal dengan memegang nilai terakhir yang terbaca)
df['suhu_ffill'] = df['suhu_no_outlier'].ffill()

# 2. Metode Interpolasi Linear (Menarik garis miring lurus di antara lubang)
df['suhu_interpolated'] = df['suhu_no_outlier'].interpolate(method='linear')

# Mari kita Zoom-In visualisasinya khusus di area yang putus koneksi (index ke-70 hingga 110)
plt.figure(figsize=(14, 5))
# Tampilkan hasil Forward-Fill
plt.plot(df.index[70:110], df['suhu_ffill'][70:110], label='Hasil Forward-Fill (Datar)', linestyle='--', marker='o')
# Tampilkan hasil Interpolasi
plt.plot(df.index[70:110], df['suhu_interpolated'][70:110], label='Hasil Interpolasi (Miring Lurus)', linestyle='-', marker='x')
# Tampilkan data kotor asli sebagai referensi dasar
plt.plot(df.index[70:110], df['suhu_no_outlier'][70:110], label='Asli (Berlubang)', marker='s', color='black')

plt.title("Langkah 3: Zoom In - Membandingkan Akurasi Metode Imputasi")
plt.legend()
plt.grid(True)
plt.show()
```

*Analisis Hasil Visual:* Coba perhatikan baik-baik. Interpolasi menarik garis lurus yang jauh lebih natural menjembatani pola bentuk gelombang dibandingkan Forward-Fill yang hanya menahan/menarik angka lurus mendatar (*statis*). Oleh karena itu, mulai tahap ini, kita hanya akan menggunakan kolom **`suhu_interpolated`**.

### Langkah 4: Menghaluskan Noise Sinyal dengan Moving Average
Data kita kini sudah sembuh total dari kekosongan (`NaN`). Namun, jika Anda perhatikan, gelombang sinusnya masih bergerigi sangat tajam bagaikan gergaji akibat *electrical noise*. Kita akan menyetrikanya (*smoothing*) dengan *Moving Average*.

```python
# Menerapkan Moving Average (Rolling Mean)
# Kita hitung rata-rata bergerak setiap 5 jam (window=5)
df['suhu_smooth_5'] = df['suhu_interpolated'].rolling(window=5).mean()

# Membandingkan dengan window yang TERLALU BESAR (window=24, yaitu di-rata-rata harian)
df['suhu_smooth_24'] = df['suhu_interpolated'].rolling(window=24).mean()

# Visualisasi Akhir Keseluruhan Proses
plt.figure(figsize=(14, 6))
plt.plot(df.index, df['suhu_interpolated'], label='Data Interpolasi (Masih Bergerigi)', color='lightgray', alpha=0.9)
plt.plot(df.index, df['suhu_smooth_5'], label='Smooth MA (Window=5)', color='green', linewidth=2)
plt.plot(df.index, df['suhu_smooth_24'], label='Smooth MA (Window=24) - Terjadi LAG Parah', color='blue', linewidth=2)

plt.title("Langkah 4: Hasil Akhir Menghaluskan Sinyal Sensor (Smoothing)")
plt.legend()
plt.show()
```

*Kesimpulan Analisis Tahap 4:* 
- Garis Hijau (`window=5`) memotong duri *noise* dengan sangat proporsional tanpa merusak bentuk bukit gelombang utama. 
- Garis Biru (`window=24`) terlihat terlalu mulus, dan perhatikan bahwa puncaknya bergeser ke kanan (terlambat / *lag*) dan tingginya juga menyusut! Ukuran *window* yang berlebihan merusak integritas informasi aslinya.

## 4. Tugas Mandiri / Eksperimen Bebas
Gantilah filter penghalus data Anda menggunakan metode **Median Filter** (alih-alih *Mean Filter* yang kita pakai di atas) dengan sintaks berikut:
`df['suhu_median_5'] = df['suhu_interpolated'].rolling(window=5).median()`

Lalu, buatlah plot perbandingan baru yang membandingkan performa visual antara `suhu_smooth_5` (Mean) dengan `suhu_median_5` (Median)! Apakah Anda menemukan perbedaan karakteristik yang tajam, terutama saat filter ini melewati area bekas peninggalan lonjakan tajam? Tulis pendapat Anda.


