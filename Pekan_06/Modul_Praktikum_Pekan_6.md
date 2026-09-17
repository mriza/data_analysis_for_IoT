---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Praktikum Pekan 6: Data Preprocessing (Cleaning & Smoothing)"
---

# Materi Praktikum Pekan 6: Data Preprocessing (Cleaning & Smoothing)

## 1. Tujuan Praktikum
- Mahasiswa mampu menulis *script* Python untuk mengidentifikasi dan menangani nilai *Missing Values* pada baris waktu (*time-series*).
- Mahasiswa mampu menerapkan teknik *Moving Average* untuk memperhalus fluktuasi (*noise*) pada data sensor.
- Mahasiswa mampu mengimplementasikan algoritma Z-Score dan IQR (Interquartile Range) untuk memfilter *outlier*.

## 2. Persiapan Alat & Prasyarat
- **Software:** Jupyter Notebook / VS Code.
- **Library Python:** `pandas`, `numpy`, `matplotlib`.
- **Dataset:** `data_sensor_dummy.csv` (dihasilkan pada praktikum pekan sebelumnya).

## 3. Dasar Teori Singkat
Data yang diambil dari lapangan sering mengandung nilai kosong (*NaN*) atau anomali (*Outlier*). 
- **Interpolasi** adalah metode menebak nilai yang hilang berdasarkan nilai sebelum dan sesudahnya dengan menarik garis lurus.
- **Moving Average** digunakan untuk "meratakan" grafik bergelombang dengan mengambil rata-rata *n* data terakhir (disebut ukuran jendela / *window size*).
- **IQR (Interquartile Range)** membagi data menjadi 4 kuartil (25%, 50%, 75%). Titik data yang berada jauh di bawah Kuartil 1 atau jauh di atas Kuartil 3 dianggap sebagai *Outlier*.

## 4. Langkah Kerja (Detail & Anotasi Kode)

### Langkah 1: Simulasi Data Rusak & Penanganan Missing Values
```python
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Load data asli
df = pd.read_csv("data_sensor_dummy.csv")

# Sengaja merusak data (menyuntikkan NaN / Not a Number pada baris ke-10 hingga ke-12)
df.loc[10:12, 'suhu'] = np.nan
print("Data Rusak (Perhatikan baris dengan NaN):")
display(df.loc[8:14])

# Metode 1: Forward Fill (Menyalin nilai terakhir yang masih ada sebelum terputus)
df_ffill = df.ffill()

# Metode 2: Interpolasi Linear (Membuat tebakan garis lurus proporsional)
df_interpolasi = df.interpolate(method='linear')

print("\nData Setelah Interpolasi (Lubang NaN tertambal rapi):")
display(df_interpolasi.loc[8:14])

# Kita gunakan data hasil interpolasi untuk langkah selanjutnya
df = df_interpolasi.copy()
```

### Langkah 2: Menghaluskan Noise dengan Moving Average (MA)
```python
# Menerapkan Moving Average (Rata-rata bergerak) dengan window = 5 jam
# Artinya, nilai di jam ke-5 adalah rata-rata dari jam 1, 2, 3, 4, dan 5.
df['suhu_halus_MA5'] = df['suhu'].rolling(window=5).mean()

# Membandingkan secara visual
plt.figure(figsize=(12, 5))
plt.plot(df.index, df['suhu'], label='Suhu Asli (Noisy)', color='gray', alpha=0.6)
plt.plot(df.index, df['suhu_halus_MA5'], label='Suhu Dihaluskan (MA-5)', color='red', linewidth=2)
plt.title("Perbandingan Data Suhu Asli vs Moving Average")
plt.legend()
plt.show()
```

### Langkah 3: Mendeteksi Outlier menggunakan Interquartile Range (IQR)
Sekarang, kita buat satu anomali palsu yang sangat ekstrem.

```python
# Suntik data anomali ekstrim
df.loc[25, 'suhu'] = 50.0  # Suhu tiba-tiba 50 derajat celcius

# Hitung Q1 (Kuartil 1 / Persentil 25) dan Q3 (Kuartil 3 / Persentil 75)
Q1 = df['suhu'].quantile(0.25)
Q3 = df['suhu'].quantile(0.75)
IQR = Q3 - Q1

# Hitung Batas Bawah dan Batas Atas dengan standar statistik (pengali 1.5)
batas_bawah = Q1 - 1.5 * IQR
batas_atas = Q3 + 1.5 * IQR

print(f"Batas Bawah Suhu Normal: {batas_bawah:.2f}")
print(f"Batas Atas Suhu Normal: {batas_atas:.2f}")

# Deteksi mana baris yang merupakan outlier (Suhu di bawah batas bawah ATAU di atas batas atas)
outlier = df[(df['suhu'] < batas_bawah) | (df['suhu'] > batas_atas)]
print(f"\nDitemukan {len(outlier)} data anomali ekstrim:")
display(outlier)
```

## 5. Hasil Eksperimen yang Diharapkan
- Anda akan mengamati bagaimana fungsi `interpolate()` menambal angka kosong dengan angka transisi yang mulus.
- Grafik *Moving Average* garis merah akan terlihat lebih mulus (tanpa sudut-sudut tajam) dibandingkan grafik abu-abu di belakangnya.
- Pada deteksi IQR, sistem mendeteksi secara otomatis dan presisi bahwa baris indeks 25 (Suhu 50.0) merupakan anomali, tanpa kita harus menebak angka batasannya secara manual.

## 6. Troubleshooting
- **Fungsi `.rolling()` menghasilkan `NaN` di beberapa baris pertama:** Ini adalah perilaku wajar (bukan error). Jika ukuran *window*=5, maka data ke 1 sampai 4 belum memiliki 5 titik ke belakang, sehingga nilai MA-nya kosong.

## 7. Tugas Mandiri/Tantangan
Gunakan kolom `kelembaban_tanah`. Hitung **Z-Score** dari kolom kelembaban tersebut menggunakan fungsi `scipy.stats.zscore`. Titik data yang memiliki Z-Score > 3 atau < -3 dianggap sebagai outlier. Temukan indeks berapa saja yang terdeteksi sebagai outlier berdasarkan algoritma Z-Score ini!
