---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Praktikum Pekan 6: Implementasi Deteksi dan Pembersihan Outlier (Z-Score, IQR, & Boxplot)"
---

# Materi Praktikum Pekan 6: Implementasi Deteksi dan Pembersihan Outlier (Z-Score, IQR, & Boxplot)

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 6 (Praktikum Mandiri & Tugas 1 Data Cleaning — $1 \times 170$ Menit)  
**Capaian Pembelajaran (Sub-CPMK 6):** Mahasiswa mampu mengimplementasikan algoritma deteksi *outlier* menggunakan metode **Z-Score** dan **Interquartile Range (IQR)** pada Python, memvisualisasikannya dengan **Boxplot** dan **Scatter Plot**, serta mengintegrasikan proses pembersihan *outlier*, interpolasi, dan *Moving Average* pada dataset pertanian multivariat.

---

## 1. Tujuan Praktikum
1. Mahasiswa mampu menulis skrip Python menggunakan `pandas`, `numpy`, dan `scipy.stats` untuk mendeteksi serta menandai (*flagging*) *outlier* dengan metode **Z-Score** dan **IQR (Tukey's Fences)**.
2. Mahasiswa mampu memvisualisasikan distribusi kuartil dan pencilan menggunakan **Boxplot** serta membandingkan hasil pembersihan pada **Scatter Plot** dua dimensi.
3. Mahasiswa mampu membuktikan keterbatasan metode univariat (Z-Score & IQR per kolom) saat menghadapi **Multivariate / Contextual Outlier** pada pasangan sensor Suhu dan Kelembaban.
4. Mahasiswa mampu merangkai *pipeline* pembersihan data utuh (Deteksi IQR $\rightarrow$ Konversi ke `NaN` $\rightarrow$ Interpolasi Waktu $\rightarrow$ *Moving Average Smoothing*) untuk menyelesaikan **Tugas 1 (Data Cleaning)**.

---

## 2. Persiapan Alat & Prasyarat
- **Perangkat Lunak:** Jupyter Notebook / Google Colab.
- **Pustaka Python:** `pandas`, `numpy`, `matplotlib`, `seaborn`, `scipy`.
  *(Jalankan `pip install pandas numpy matplotlib seaborn scipy` jika belum tersedia).*

---

## 3. Langkah Kerja Praktikum (Detail & Anotasi Kode)

### Langkah 1: Membangkitkan Dataset Sensor Multivariat dengan 3 Jenis Anomali
Buat notebook baru bernama `Praktikum_Pekan6_Outlier_Cleaning.ipynb`. Kita akan membangkitkan 500 baris data berurutan waktu untuk sensor **Suhu Udara** dan **Kelembaban Relatif (RH)**, lalu menyuntikkan:
1. **Univariate Hard Outlier** (Suhu ekstrem $55^\circ\text{C}$ dan $52^\circ\text{C}$; Kelembaban ekstrem $10\%$).
2. **Multivariate Contextual Outlier** pada indeks ke-300: Suhu = $22,0^\circ\text{C}$ (normal secara univariat) dan Kelembaban = $42,0\%$ (masih di dalam pagar IQR kelembaban), tetapi **mustahil terjadi bersamaan** karena pada suhu dingin $22^\circ\text{C}$ kelembaban normalnya berada di kisaran $65\% - 75\%$!

```python
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from scipy import stats

np.random.seed(101)

# Membuat indeks waktu 500 jam
waktu = pd.date_range(start="2026-10-01 00:00:00", periods=500, freq="h")

# Generate Suhu Normal (Mean = 28°C, Std = 2°C)
suhu = np.random.normal(loc=28.0, scale=2.0, size=500)

# Generate Kelembaban yang berkorelasi terbalik dengan Suhu + sedikit derau
kelembaban = 100.0 - (suhu * 1.5) + np.random.normal(loc=0.0, scale=2.5, size=500)

df = pd.DataFrame({
    'suhu': np.round(suhu, 2),
    'kelembaban': np.round(kelembaban, 2)
}, index=waktu)

# MENYUNTIKKAN OUTLIER SECARA SENGAJA
# 1. Outlier Univariat Ekstrem (Korsleting Sensor)
df.iloc[10, df.columns.get_loc('suhu')] = 55.0
df.iloc[25, df.columns.get_loc('suhu')] = 52.0
df.iloc[150, df.columns.get_loc('kelembaban')] = 10.0

# 2. Outlier Multivariat/Kontekstual (Angka per kolom tampak wajar, tapi relasinya menyimpang)
df.iloc[300, df.columns.get_loc('suhu')] = 22.5
df.iloc[300, df.columns.get_loc('kelembaban')] = 46.0

plt.figure(figsize=(10, 5))
sns.scatterplot(x='suhu', y='kelembaban', data=df, color='#2C3E50', s=45, alpha=0.7)
plt.scatter(df.iloc[[10, 25, 150, 300]]['suhu'], df.iloc[[10, 25, 150, 300]]['kelembaban'],
            color='red', s=100, label='Titik Anomali yang Disuntikkan', zorder=5)
plt.title("Langkah 1: Scatter Plot Suhu vs Kelembaban Beserta Titik Anomali", fontweight='bold')
plt.xlabel("Suhu Udara (°C)")
plt.ylabel("Kelembaban Relatif (%)")
plt.legend()
plt.grid(True, alpha=0.4)
plt.tight_layout()
plt.show()
```

---

### Langkah 2: Inspeksi Visual Kuartil & Outlier Menggunakan Boxplot
Sebelum menulis rumus matematika, gunakan `sns.boxplot` untuk memetakan sebaran kuartil ($Q_1, Q_2, Q_3$) dan melihat titik-titik wajik di luar garis kumis (*whisker*):

```python
fig, axes = plt.subplots(1, 2, figsize=(12, 5))

sns.boxplot(y=df['suhu'], ax=axes[0], color='#F1948A', width=0.35)
axes[0].set_title('Boxplot Sensor Suhu (°C)', fontweight='bold')
axes[0].grid(True, alpha=0.4)

sns.boxplot(y=df['kelembaban'], ax=axes[1], color='#85C1E9', width=0.35)
axes[1].set_title('Boxplot Sensor Kelembaban (%)', fontweight='bold')
axes[1].grid(True, alpha=0.4)

plt.tight_layout()
plt.show()
```

---

### Langkah 3: Deteksi Outlier Menggunakan Metode Z-Score ($|Z| > 3$)
Mari kita hitung nilai Z-Score untuk setiap kolom menggunakan rumus $Z = \frac{x - \mu}{\sigma}$:

```python
# Menghitung Z-Score secara eksplisit untuk seluruh kolom numerik
z_scores = np.abs(stats.zscore(df[['suhu', 'kelembaban']]))

# Baris dinyatakan NORMAL jika SEMUA kolom memiliki |Z| < 3
mask_normal_z = (z_scores < 3.0).all(axis=1)
df_bersih_zscore = df[mask_normal_z]
df_outlier_zscore = df[~mask_normal_z]

print(f"Total baris awal                : {len(df)} baris")
print(f"Baris bersih versi Z-Score      : {len(df_bersih_zscore)} baris")
print(f"Outlier terdeteksi oleh Z-Score : {len(df_outlier_zscore)} baris")
display(df_outlier_zscore)
```

---

### Langkah 4: Deteksi Outlier Menggunakan Metode Tukey's IQR ($1,5 \times IQR$)
Sekarang kita bandingkan dengan metode non-parametrik berbasis kuartil ($Q_1 - 1,5 \times IQR$ dan $Q_3 + 1,5 \times IQR$):

```python
# 1. Menghitung Kuartil Bawah (Q1), Kuartil Atas (Q3), dan IQR untuk setiap kolom
Q1 = df[['suhu', 'kelembaban']].quantile(0.25)
Q3 = df[['suhu', 'kelembaban']].quantile(0.75)
IQR = Q3 - Q1

batas_bawah = Q1 - 1.5 * IQR
batas_atas = Q3 + 1.5 * IQR

print("--- Batas Pagar Dalam Tukey (1.5 x IQR) ---")
for kol in ['suhu', 'kelembaban']:
    print(f"Kolom {kol:10s} : Q1={Q1[kol]:.2f} | Q3={Q3[kol]:.2f} | IQR={IQR[kol]:.2f} -> Valid: [{batas_bawah[kol]:.2f} s.d. {batas_atas[kol]:.2f}]")

# 2. Mengidentifikasi baris yang melanggar pagar bawah atau pagar atas pada salah satu kolom
kondisi_outlier_iqr = ((df[['suhu', 'kelembaban']] < batas_bawah) | (df[['suhu', 'kelembaban']] > batas_atas)).any(axis=1)

df_bersih_iqr = df[~kondisi_outlier_iqr]
df_outlier_iqr = df[kondisi_outlier_iqr]

print(f"\nBaris bersih versi IQR          : {len(df_bersih_iqr)} baris")
print(f"Outlier terdeteksi oleh IQR     : {len(df_outlier_iqr)} baris")

# 3. Visualisasi Perbandingan Titik yang Dibuang oleh IQR
plt.figure(figsize=(10, 5))
plt.scatter(df_outlier_iqr['suhu'], df_outlier_iqr['kelembaban'], color='red', marker='x', s=80, label='Outlier Terdeteksi (IQR)')
plt.scatter(df_bersih_iqr['suhu'], df_bersih_iqr['kelembaban'], color='#2980B9', alpha=0.6, label='Data Bersih (IQR)')
plt.title("Langkah 4: Hasil Pemisahan Outlier Menggunakan Metode IQR", fontweight='bold')
plt.xlabel("Suhu Udara (°C)")
plt.ylabel("Kelembaban Relatif (%)")
plt.legend()
plt.grid(True, alpha=0.4)
plt.tight_layout()
plt.show()
```

---

### Langkah 5: Pipeline Rekonstruksi Time-Series (Amputasi IQR $\rightarrow$ Interpolasi $\rightarrow$ Moving Average)
Ingat bahwa pada data *time-series*, kita **tidak boleh sekadar membuang baris waktu** (`df[~kondisi_outlier_iqr]`) karena akan memutus urutan jam! Prosedur yang benar adalah mengubah sel *outlier* menjadi `NaN`, menambalnya dengan `.interpolate()`, lalu menghaluskannya dengan `.rolling().mean()`:

```python
df_pipeline = df.copy()

# 1. Tandai bendera anomali (Flagging)
df_pipeline['flag_outlier_suhu'] = (df_pipeline['suhu'] < batas_bawah['suhu']) | (df_pipeline['suhu'] > batas_atas['suhu'])

# 2. Ubah nilai outlier menjadi NaN (jangan hapus baris timestamp!)
df_pipeline['suhu_clean'] = df_pipeline['suhu'].mask(df_pipeline['flag_outlier_suhu'], np.nan)

# 3. Rekonstruksi nilai NaN dengan interpolasi waktu linier
df_pipeline['suhu_clean'] = df_pipeline['suhu_clean'].interpolate(method='linear')

# 4. Haluskan fluktuasi derau dengan Moving Average (window = 6 jam)
df_pipeline['suhu_smooth_ma6'] = df_pipeline['suhu_clean'].rolling(window=6, min_periods=1).mean()

# Visualisasi Hasil Pipeline Pembersihan pada 100 Jam Pertama
plt.figure(figsize=(13, 5))
plt.plot(df_pipeline.index[:100], df_pipeline['suhu'].iloc[:100], color='lightcoral', linestyle=':', marker='.', label='Suhu Mentah (Ada Outlier 55°C & 52°C)')
plt.plot(df_pipeline.index[:100], df_pipeline['suhu_clean'].iloc[:100], color='#F39C12', alpha=0.8, label='Setelah IQR Filter + Interpolasi')
plt.plot(df_pipeline.index[:100], df_pipeline['suhu_smooth_ma6'].iloc[:100], color='#1E8449', linewidth=2.2, label='Hasil Akhir (Moving Average k=6)')
plt.title("Langkah 5: Pipeline Utuh Pembersihan Time-Series (100 Jam Pertama)", fontweight='bold')
plt.ylabel("Suhu Udara (°C)")
plt.legend()
plt.grid(True, alpha=0.4)
plt.tight_layout()
plt.show()
```

---

## 4. Evaluasi Kritis & Diskusi Hasil Praktikum
1. **Periksa Baris Indeks ke-300 (`df.iloc[300]`):**
   Coba jalankan `print(kondisi_outlier_iqr.iloc[300])`! Anda akan mendapati bahwa titik `(Suhu = 22.5°C, Kelembaban = 46.0%)` **TIDAK terdeteksi sebagai outlier** baik oleh Z-Score maupun IQR univariat, meskipun pada grafik *Scatter Plot* Langkah 1 titik merah tersebut jelas-jelas memisahkan diri dari gerombolan utama!
   *Mengapa?* Karena secara kolom tunggal, suhu $22,5^\circ\text{C}$ masih berada di dalam batas `[22.6°C, 33.4°C]` (atau sangat dekat batas), dan kelembaban $46,0\%$ masih berada di dalam batas `[45.2%, 70.8%]`. Untuk menangkap anomali dua dimensi seperti ini, kita memerlukan algoritma **Machine Learning Unsupervised (*Isolation Forest*)** yang akan dipelajari pada Pekan 9 & 10.

---

## 5. Tugas 1 (Data Cleaning Mandiri — Bobot Penilaian RPS 10%)
1. **Eksperimen Sensitivitas Pengali Pagar IQR (*Fence Multiplier*):**
   Pada Langkah 4, ubahlah konstanta pengali pagar IQR dari `1.5` (*Inner Fence*) menjadi `2.5` dan `3.0` (*Outer Fence*). Buatlah tabel perbandingan jumlah baris yang terdeteksi sebagai *outlier* antara:
   - Z-Score ($|Z| > 3,0$)
   - IQR Pagar Dalam ($1,5 \times IQR$)
   - IQR Pagar Luar ($3,0 \times IQR$)
2. **Deteksi Sensor Macet (*Stuck-at Zero Variance*):**
   Suntikkan kerusakan sensor macet pada `df['kelembaban']` di mana baris ke-200 hingga ke-220 bernilai konstan `58.00` berturut-turut. Tuliskan logika Python menggunakan `.rolling(window=5).std() == 0` untuk mendeteksi dan menandai rentang baris sensor yang macet tersebut!

---

## 6. Referensi & Acuan Materi Pekan Ini

1. **McKinney, W. (2022).** *Python for Data Analysis: Data Wrangling with Pandas, NumPy, and Jupyter* (3rd ed.). O'Reilly Media. (ISBN-13: 978-1098104030) — **Bab 7 (Detecting and Filtering Outliers).**  
2. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730)  
3. **Fawcett, T., & Provost, F. (2013).** *Data Science for Business*. O'Reilly Media. (ISBN-13: 978-1449361327)  
4. **Pandas & SciPy Documentation. (2024).** `pandas.DataFrame.quantile`, `pandas.Series.mask`, `scipy.stats.zscore`. Tersedia: [https://pandas.pydata.org/docs/](https://pandas.pydata.org/docs/)  
5. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
