---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Praktikum Pekan 5: Pra-pemrosesan Data Sensor (Cleaning, Imputation, dan Smoothing)"
---

# Materi Praktikum Pekan 5: Pra-pemrosesan Data Sensor (Cleaning, Imputation, dan Smoothing)

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 5 (Suplemen Praktikum Terbimbing)  
**Capaian Pembelajaran (Sub-CPMK 5):** Mahasiswa mampu mensimulasikan kerusakan sinyal sensor IoT (*noise, missing values, hard outliers*), mengimplementasikan teknik penambalan data (*ffill, bfill, linear & polynomial interpolation*), serta mengevaluasi kinerja filter penghalusan (*SMA, EMA, dan Rolling Median*) menggunakan Python.

---

## 1. Tujuan Praktikum
1. Mahasiswa mampu mensimulasikan data sensor deret waktu yang terkontaminasi oleh derau listrik (*Gaussian noise*), nilai hilang (*missing values*), dan lonjakan ekstrem (*hard outliers*).
2. Mahasiswa mampu mengamputasi *hard outlier* berdasarkan batas fisika sensor (*physical thresholding*) dan menandainya dengan bendera kualitas data (*data quality flag*).
3. Mahasiswa mampu mempraktikkan serta membandingkan akurasi metode imputasi **Forward-Fill (`ffill`)**, **Backward-Fill (`bfill`)**, **Linear Interpolation**, dan **Polynomial Interpolation** pada sinyal bergelombang diurnal.
4. Mahasiswa mampu mengaplikasikan filter **Simple Moving Average (SMA)**, **Exponential Moving Average (EMA)**, dan **Rolling Median** serta membuktikan secara visual fenomena *Phase Lag* dan reduksi amplitudo akibat ukuran jendela (*window size*) yang terlalu besar.

---

## 2. Persiapan Praktikum
- Buka dan aktifkan lingkungan virtual (`venv`) Jupyter Notebook Anda atau gunakan Google Colab.
- Buat *notebook* baru dengan nama: `Praktikum_Pekan5_Preprocessing.ipynb`.
- Pustaka Python yang dibutuhkan: `pandas`, `numpy`, `matplotlib`, `scipy` (opsional untuk interpolasi polinomial/spline: `pip install scipy`).

---

## 3. Langkah Kerja Praktikum (Detail & Anotasi Kode)

### Langkah 1: Membangkitkan Sinyal Sensor "Kotor" Secara Artifisial
Kita akan membuat gelombang sinus murni yang merepresentasikan siklus suhu harian rumah kaca selama 200 jam (~8 hari), lalu menyuntikkan tiga jenis kerusakan fisik: (1) Derau termal/elektrikal, (2) Korsleting sesaat (*Hard Outlier*), dan (3) Putus koneksi komunikasi (*Missing Values*):

```python
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

np.random.seed(42)  # Kunci generator angka acak agar eksperimen dapat direproduksi

# 1. Membuat indeks waktu 200 jam dengan frekuensi per jam ('h')
waktu = pd.date_range(start='2026-11-01 00:00:00', periods=200, freq='h')

# 2. Gelombang Sinus murni (Suhu harian ideal berayun antara 20°C s.d. 30°C)
sinyal_asli = 25.0 + 5.0 * np.sin(np.linspace(0, 20, 200))

# 3. Menambahkan Derau Elektronika (Gaussian Noise dengan std = 1.2°C)
noise = np.random.normal(0, 1.2, 200)
sinyal_kotor = sinyal_asli + noise

# Membangun DataFrame berindeks waktu
df = pd.DataFrame({
    'timestamp': waktu,
    'suhu_asli_ideal': sinyal_asli,
    'suhu': sinyal_kotor
}).set_index('timestamp')

# 4. Menyuntikkan Skenario Hard Outlier (Induksi Relay / Korsleting Sesaat)
df.loc[df.index[50], 'suhu'] = 80.0    # Lonjakan panas mustahil
df.loc[df.index[150], 'suhu'] = -10.0  # Lonjakan beku mustahil

# 5. Menyuntikkan Skenario Missing Values (Modem/WiFi Mati Selama 15 Jam)
df.iloc[80:95, df.columns.get_loc('suhu')] = np.nan

# Visualisasi Data Mentah sebelum Pre-processing
plt.figure(figsize=(14, 5))
plt.plot(df.index, df['suhu'], label='Data Sensor Mentah (Kotor)', color='#E74C3C', marker='.', alpha=0.8)
plt.plot(df.index, df['suhu_asli_ideal'], label='Suhu Fisik Sebenarnya (Ground Truth)', color='black', linestyle='--', linewidth=1.5)
plt.title("Langkah 1: Sinyal Sensor Mentah dengan Noise, Outlier (80°C & -10°C), dan Missing Values", fontweight='bold')
plt.ylabel("Suhu (°C)")
plt.legend()
plt.grid(True, alpha=0.4)
plt.tight_layout()
plt.show()
```

---

### Langkah 2: Amputasi Hard Outlier Berdasarkan Batas Fisika & Data Quality Flagging
Spesifikasi teknis sensor dan kondisi iklim mikro rumah kaca tropis hanya mungkin berada pada rentang **$10^\circ\text{C} \le T \le 45^\circ\text{C}$**. Nilai di luar rentang tersebut pasti merupakan kerusakan perangkat keras dan wajib diubah menjadi `NaN` agar dapat direkonstruksi, sembari dicatat pada kolom `status_kualitas`:

```python
print("--- Statistik Deskriptif Sebelum Pembersihan Outlier ---")
display(df[['suhu']].describe())

BATAS_BAWAH = 10.0
BATAS_ATAS = 45.0

# Membuat kolom bendera kualitas data (Data Quality Flag) untuk audit keamanan
df['status_kualitas'] = 'VALID'
kondisi_missing_awal = df['suhu'].isna()
kondisi_outlier = (df['suhu'] < BATAS_BAWAH) | (df['suhu'] > BATAS_ATAS)

df.loc[kondisi_missing_awal, 'status_kualitas'] = 'MISSING_PACKET'
df.loc[kondisi_outlier, 'status_kualitas'] = 'HARDWARE_OUTLIER'

# Mengubah nilai outlier menjadi NaN (jangan di-drop barisnya agar indeks waktu tetap utuh!)
df['suhu_tanpa_outlier'] = df['suhu'].copy()
df.loc[kondisi_outlier, 'suhu_tanpa_outlier'] = np.nan

print("\n--- Rekapitulasi Audit Kualitas Data Sensor ---")
print(df['status_kualitas'].value_counts())

plt.figure(figsize=(14, 4.5))
plt.plot(df.index, df['suhu_tanpa_outlier'], label='Suhu Setelah Outlier Diamputasi (Menjadi NaN)', color='#F39C12', marker='.')
plt.title("Langkah 2: Setelah Hard Outlier Diubah Menjadi NaN", fontweight='bold')
plt.ylabel("Suhu (°C)")
plt.legend()
plt.grid(True, alpha=0.4)
plt.tight_layout()
plt.show()
```

---

### Langkah 3: Eksperimen Perbandingan Metode Imputasi (`ffill`, `Linear`, `Polynomial`)
Mari kita tambal lubang data pada indeks ke-80 s.d. ke-95 dan bandingkan hasilnya terhadap kurva gelombang suhu ideal (`suhu_asli_ideal`) beserta perhitungan nilai galat rata-rata (**MAE — Mean Absolute Error**):

```python
# 1. Forward-Fill (Menahan nilai terakhir sebelum putus)
df['suhu_ffill'] = df['suhu_tanpa_outlier'].ffill()

# 2. Backward-Fill (Menarik nilai saat sensor hidup kembali)
df['suhu_bfill'] = df['suhu_tanpa_outlier'].bfill()

# 3. Linear Interpolation (Menarik garis miring lurus)
df['suhu_interp_linear'] = df['suhu_tanpa_outlier'].interpolate(method='linear')

# 4. Polynomial Interpolation Orde 2 (Mengikuti kelengkungan kurva kuadratik)
df['suhu_interp_poly'] = df['suhu_tanpa_outlier'].interpolate(method='polynomial', order=2)

# Evaluasi numerik galat imputasi khusus pada rentang yang hilang (indeks 80:95)
area_hilang = df.iloc[80:95]
for metode in ['suhu_ffill', 'suhu_bfill', 'suhu_interp_linear', 'suhu_interp_poly']:
    mae = (area_hilang[metode] - area_hilang['suhu_asli_ideal']).abs().mean()
    print(f"Galat MAE pada area hilang ({metode:18s}): {mae:.3f} °C")

# Visualisasi Zoom-In pada Area Koneksi Terputus (Indeks 70 s.d. 110)
plt.figure(figsize=(14, 5.5))
plt.plot(df.index[70:110], df['suhu_asli_ideal'].iloc[70:110], label='Kurva Ideal Sebenarnya (Ground Truth)', color='black', linestyle=':', linewidth=2)
plt.plot(df.index[70:110], df['suhu_ffill'].iloc[70:110], label='Forward-Fill (Datar Bias)', linestyle='--', marker='v', color='gray')
plt.plot(df.index[70:110], df['suhu_interp_linear'].iloc[70:110], label='Linear Interpolation (Garis Lurus)', linestyle='-.', marker='s', color='#2980B9')
plt.plot(df.index[70:110], df['suhu_interp_poly'].iloc[70:110], label='Polynomial Order 2 (Melengkung Akurat)', linestyle='-', marker='o', color='#27AE60')
plt.plot(df.index[70:110], df['suhu_tanpa_outlier'].iloc[70:110], label='Data Tersedia', color='#E74C3C', linewidth=2.5)

plt.title("Langkah 3: Zoom-In Perbandingan Akurasi Metode Imputasi pada Siklus Gelombang", fontweight='bold')
plt.ylabel("Suhu (°C)")
plt.legend()
plt.grid(True, alpha=0.5)
plt.tight_layout()
plt.show()
```
*Analisis Hasil:* Perhatikan bagaimana `suhu_interp_poly` mampu mengikuti kelengkungan lembah gelombang suhu secara natural dengan nilai galat MAE terkecil dibandingkan `ffill` yang mendatar kaku.

---

### Langkah 4: Penghalusan Derau (*Smoothing*) — SMA vs EMA vs Rolling Median
Sekarang kita gunakan sinyal yang telah terimputasi (`suhu_interp_poly`) untuk menguji peredaman derau frekuensi tinggi menggunakan **SMA ($k=5$)**, **EMA ($\text{span}=5$)**, dan **SMA Jendela Besar ($k=24$)** untuk mengamati fenomena **Phase Lag**:

```python
# 1. Simple Moving Average (Jendela proporsional k = 5 jam)
df['sma_5'] = df['suhu_interp_poly'].rolling(window=5, min_periods=1).mean()

# 2. Exponential Moving Average (Span = 5 jam -> Respons lebih cepat, lag lebih kecil)
df['ema_5'] = df['suhu_interp_poly'].ewm(span=5, adjust=False).mean()

# 3. Simple Moving Average Jendela Terlalu Besar (k = 24 jam -> Over-smoothing & Phase Lag parah!)
df['sma_24'] = df['suhu_interp_poly'].rolling(window=24, min_periods=1).mean()

plt.figure(figsize=(14, 6))
plt.plot(df.index, df['suhu_interp_poly'], label='Sinyal Terimputasi (Masih Berderau / Noisy)', color='lightgray', linewidth=1.2)
plt.plot(df.index, df['suhu_asli_ideal'], label='Sinyal Fisik Ideal (Target)', color='black', linestyle=':', linewidth=1.8)
plt.plot(df.index, df['sma_5'], label='SMA (Window=5)', color='#27AE60', linewidth=2)
plt.plot(df.index, df['ema_5'], label='EMA (Span=5) - Lag Lebih Rendah', color='#E74C3C', linestyle='--', linewidth=2)
plt.plot(df.index, df['sma_24'], label='SMA (Window=24) - OVER-SMOOTHED & PHASE LAG', color='#2980B9', linewidth=2.5)

plt.title("Langkah 4: Perbandingan Karakteristik Smoothing SMA vs EMA & Efek Phase Lag", fontweight='bold')
plt.ylabel("Suhu (°C)")
plt.legend(loc='upper right')
plt.grid(True, alpha=0.4)
plt.tight_layout()
plt.show()
```

---

## 4. Evaluasi Kinerja & Panduan Troubleshooting
1. **Mengapa `.rolling(window=5)` tanpa `min_periods=1` menghasilkan `NaN` pada 4 baris pertama?**
   Karena untuk menghitung rata-rata 5 titik pada baris ke-1 hingga ke-4, jumlah sampel historis belum mencapai 5. Menambahkan argumen `min_periods=1` menginstruksikan Pandas untuk menghitung rata-rata dari sampel yang sudah tersedia di awal deret.
2. **Error `ImportError: Missing optional dependency 'scipy'` saat menjalankan `interpolate(method='polynomial')`:**
   Metode interpolasi polinomial dan spline di Pandas menggunakan mesin matriks dari `scipy`. Instal dengan menjalankan `pip install scipy` di terminal atau `!pip install scipy` di sel Jupyter.

---

## 5. Tugas Mandiri Praktikum
1. **Eksperimen Ketangguhan Median Filter Tanpa Hapus Outlier Manual:**
   Ambil kolom data mentah **`df['suhu']`** dari Langkah 1 (yang masih mengandung lonjakan *outlier* ekstrem `80.0°C` dan `-10.0°C`), lalu tambal bagian kosongnya dengan interpolasi linier tanpa membuang *outlier* terlebih dahulu. Terapkan dua filter sekaligus pada data tersebut:
   - `df['filter_mean'] = df['suhu'].interpolate().rolling(window=5, min_periods=1).mean()`
   - `df['filter_median'] = df['suhu'].interpolate().rolling(window=5, min_periods=1).median()`
   Visualisasikan kedua garis tersebut dalam satu grafik dan jelaskan mengapa **Median Filter** berhasil meredam lonjakan `80.0°C` secara tuntas, sedangkan **Mean Filter** justru ikut tertarik naik selama 5 jam berturut-turut!

---

## 6. Referensi & Acuan Materi Pekan Ini

1. **McKinney, W. (2022).** *Python for Data Analysis: Data Wrangling with Pandas, NumPy, and Jupyter* (3rd ed.). O'Reilly Media. (ISBN-13: 978-1098104030) — **Bab 7 (Data Cleaning and Preparation) & Bab 11 (Moving Window Functions).**  
2. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730)  
3. **Pandas Development Team. (2024).** *Pandas API Reference: `DataFrame.interpolate`, `DataFrame.rolling`, `DataFrame.ewm`*. Tersedia: [https://pandas.pydata.org/docs/](https://pandas.pydata.org/docs/)  
4. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
