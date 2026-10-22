---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Praktikum Pekan 10: Implementasi Pemodelan Machine Learning (Time-Series Forecasting & Isolation Forest)"
---

# Materi Praktikum Pekan 10: Implementasi Pemodelan Machine Learning (Time-Series Forecasting & Isolation Forest)

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 10 (Praktikum Terbimbing — $1 \times 170$ Menit)  
**Capaian Pembelajaran (Sub-CPMK 10):** Mahasiswa mampu melakukan rekayasa fitur waktu mundur (*Lag Feature Engineering*), membagi dataset deret waktu secara kronologis (`shuffle=False`), melatih serta mengevaluasi model **Regresi Time-Series Forecasting** (MAE, RMSE, $R^2$), mengimplementasikan deteksi anomali multivariat menggunakan **Isolation Forest**, serta menyimpan model terlatih (`.pkl`) untuk diintegrasikan ke dalam *pipeline* produksi.

---

## 1. Tujuan Praktikum
1. Mahasiswa mampu mentransformasikan data runtun waktu sensor menjadi matriks fitur *Supervised Learning* menggunakan teknik **Multi-Lag Feature Engineering** (`Lag-1`, `Lag-2`, `Lag-3`).
2. Mahasiswa mampu menerapkan pembagian data kronologis (**Train-Test Split** dengan `shuffle=False`) menggunakan pustaka `scikit-learn`.
3. Mahasiswa mampu melatih model **Linear Regression** dan **Random Forest Regressor** untuk meramal nilai sensor di masa depan (*Forecasting*), memvisualisasikan kurva prediksinya, serta menghitung metrik evaluasi **MAE, RMSE, dan $R^2$**.
4. Mahasiswa mampu mengimplementasikan algoritma *Unsupervised Learning* **Isolation Forest** untuk menangkap anomali sensor dua dimensi yang lolos dari deteksi Z-Score/IQR biasa, serta mengekspor model ke dalam berkas `.pkl` menggunakan `joblib`/`pickle`.

---

## 2. Persiapan Alat & Prasyarat
- **Perangkat Lunak:** Jupyter Notebook (Lokal / Ubuntu Server VM) atau Google Colab.
- **Pustaka Python:** `pandas`, `numpy`, `matplotlib`, `seaborn`, `scikit-learn`, `joblib`.
- **Instalasi Pustaka:**
  ```bash
  pip install pandas numpy matplotlib seaborn scikit-learn joblib
  ```

---

## 3. Dasar Teori Singkat
1. **Larangan Membocorkan Data Uji (*Data Leakage*):** Dalam pemodelan ML, kita membagi data menjadi **Train Set (80% awal)** untuk proses belajar (`.fit()`) dan **Test Set (20% akhir)** untuk menguji kemampuan generalisasi model (`.predict()`). Pada data *time-series*, parameter `shuffle=False` wajib diberikan agar urutan waktu masa lalu dan masa depan tidak tercampur.
2. **Format Dimensi Input Scikit-Learn (`X` 2D vs `y` 1D):** Seluruh algoritma di `scikit-learn` menuntut matriks fitur input **`X` berbentuk 2 Dimensi** (`DataFrame` dengan tanda kurung siku ganda `df[['fitur1', 'fitur2']]`), sedangkan target **`y` berbentuk 1 Dimensi** (`Series` `df['target']`).
3. **Isolation Forest Output Convention:** Fungsi `model_iso.fit_predict(X)` menghasilkan nilai **`1` untuk data normal (*inlier*)** dan **`-1` untuk data anomali (*outlier*)**.

---

## 4. Langkah Kerja Praktikum (Detail & Anotasi Kode)

### Langkah 1: Membangkitkan Dataset Time-Series & Rekayasa Fitur Lag (*Lagging*)
Buat notebook baru bernama `Praktikum_Pekan10_ML_IoT.ipynb`. Kita akan membuat data suhu dan kelembaban rumah kaca selama 200 jam yang memiliki tren kenaikan perlahan serta gelombang diurnal, lalu membangun fitur **Lag-1, Lag-2, dan Lag-3** untuk memprediksi suhu 1 jam ke depan:

```python
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

np.random.seed(42)

# 1. Membuat deret waktu 200 jam
waktu = pd.date_range("2026-10-01 00:00:00", periods=200, freq="h")
t = np.arange(200)

# 2. Pola suhu: Tren naik perlahan + Siklus harian (sinus) + Noise sensor
suhu = 25.0 + (0.03 * t) + (3.5 * np.sin(2 * np.pi * t / 24)) + np.random.normal(0, 0.4, 200)

df = pd.DataFrame({"waktu": waktu, "suhu_aktual": np.round(suhu, 2)}).set_index("waktu")

# 3. FEATURE ENGINEERING: Membuat fitur waktu mundur (Lag-1, Lag-2, Lag-3)
# Kita ingin menebak 'suhu_aktual' pada jam t berdasarkan suhu pada jam (t-1), (t-2), dan (t-3)
df['suhu_lag_1'] = df['suhu_aktual'].shift(1)
df['suhu_lag_2'] = df['suhu_aktual'].shift(2)
df['suhu_lag_3'] = df['suhu_aktual'].shift(3)

# 4. Hapus 3 baris teratas yang mengandung NaN akibat pergeseran shift()
df = df.dropna()

print(f"Total baris siap latih: {len(df)} baris")
display(df.head(6))
```

---

### Langkah 2: Chronological Splitting, Pelatihan Model Regresi, & Evaluasi Metrik
Sekarang kita bagi data menjadi 80% *Training* dan 20% *Testing* (`shuffle=False`), latih model `LinearRegression`, dan ukur akurasinya menggunakan **MAE**, **RMSE**, dan **$R^2$**:

```python
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_absolute_error, mean_squared_error, r2_score

# Memisahkan Matriks Fitur Independen (X - 2 Dimensi) dan Target Dependen (y - 1 Dimensi)
fitur_kolom = ['suhu_lag_1', 'suhu_lag_2', 'suhu_lag_3']
X = df[fitur_kolom]
y = df['suhu_aktual']

# PENTING: shuffle=False wajib untuk menjaga urutan waktu masa lalu -> masa depan!
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, shuffle=False)

# Inisialisasi dan Pelatihan (Fitting) Model Regresi Linier
model_lr = LinearRegression()
model_lr.fit(X_train, y_train)

# Melakukan Peramalan (Forecasting) pada Data Uji (20% periode waktu terakhir)
prediksi_suhu = model_lr.predict(X_test)

# Menghitung Metrik Evaluasi Kinerja Model
mae = mean_absolute_error(y_test, prediksi_suhu)
rmse = np.sqrt(mean_squared_error(y_test, prediksi_suhu))
r2 = r2_score(y_test, prediksi_suhu)

print("=== HASIL EVALUASI MODEL FORECASTING (TEST SET) ===")
print(f"Bobot Koefisien Lag (B1, B2, B3) : {np.round(model_lr.coef_, 3)}")
print(f"Intercept (B0)                   : {model_lr.intercept_:.3f}")
print(f"Mean Absolute Error (MAE)        : {mae:.3f} °C")
print(f"Root Mean Squared Error (RMSE)   : {rmse:.3f} °C")
print(f"R-Squared Score (R2)             : {r2:.3f} (Mendekati 1.0 = Sangat Akurat)")

# Visualisasi Perbandingan Data Latih, Data Aktual Uji, dan Hasil Ramalan AI
plt.figure(figsize=(13, 5))
plt.plot(y_train.index[-60:], y_train.iloc[-60:], label='Data Historis (Training Set - 60 Jam Terakhir)', color='#7F8C8D')
plt.plot(y_test.index, y_test, label='Suhu Sebenarnya (Actual Test Set)', color='#2980B9', marker='o', markersize=4, linewidth=2)
plt.plot(y_test.index, prediksi_suhu, label=f'Ramalan AI (Linear Regression | RMSE={rmse:.2f}°C)', color='#E74C3C', linestyle='--', marker='x', markersize=4, linewidth=2)
plt.axvline(y_test.index[0], color='black', linestyle=':', label='Batas Waktu Split (Train | Test)')

plt.title("Langkah 2: Peramalan Suhu Rumah Kaca (Time-Series Forecasting dengan Fitur Lag)", fontweight='bold')
plt.xlabel("Waktu")
plt.ylabel("Suhu Udara (°C)")
plt.legend()
plt.grid(True, alpha=0.4)
plt.tight_layout()
plt.show()
```

---

### Langkah 3: Deteksi Anomali Multivariat dengan *Isolation Forest*
Sekarang kita beralih ke skenario **Unsupervised Anomaly Detection**. Kita akan membuat dataset 2 variabel sensor (**Suhu Tanah** dan **pH Tanah**) sebanyak 300 baris, lalu menyuntikkan 4 titik anomali multivariat (termasuk anomali kontekstual yang nilainya tidak terdeteksi oleh Z-Score tunggal):

```python
from sklearn.ensemble import IsolationForest
import joblib

np.random.seed(42)

# 1. Membangkitkan 300 data normal (Suhu sekitar 28°C, pH tanah berkorelasi ringan sekitar 6.5)
suhu_tanah = np.random.normal(loc=28.0, scale=1.8, size=300)
ph_tanah = 6.5 - 0.08 * (suhu_tanah - 28.0) + np.random.normal(loc=0.0, scale=0.25, size=300)

df_anomali = pd.DataFrame({
    'suhu_tanah': np.round(suhu_tanah, 2),
    'ph_tanah': np.round(ph_tanah, 2)
})

# 2. Menyuntikkan 4 titik Anomali (Korsleting & Kegagalan Sistem Fertigasi)
df_anomali.loc[50]  = [45.0, 3.8]  # Anomali 1: Panas ekstrem & sangat asam
df_anomali.loc[120] = [14.0, 8.9]  # Anomali 2: Dingin ekstrem & sangat basa
df_anomali.loc[200] = [34.5, 8.2]  # Anomali 3: Kombinasi suhu tinggi & pH basa menyimpang
df_anomali.loc[260] = [21.5, 4.9]  # Anomali 4: Kombinasi suhu rendah & pH asam menyimpang

# 3. Melatih Model Isolation Forest
# contamination=0.02 berarti kita memperkirakan ~2% dari populasi data adalah anomali
fitur_sensor = ['suhu_tanah', 'ph_tanah']
model_iso = IsolationForest(n_estimators=100, contamination=0.02, random_state=42)

# Melatih model sekaligus memprediksi label (+1 = Normal, -1 = Anomali)
df_anomali['label_ai'] = model_iso.fit_predict(df_anomali[fitur_sensor])
df_anomali['skor_anomali'] = model_iso.decision_function(df_anomali[fitur_sensor])

# 4. Menampilkan baris yang divonis sebagai Anomali (-1) oleh AI
data_terdeteksi = df_anomali[df_anomali['label_ai'] == -1]
print(f"Total baris yang terdeteksi sebagai Anomali (-1): {len(data_terdeteksi)} baris")
display(data_terdeteksi)

# 5. Menyimpan model terlatih ke disk agar siap dipakai di Pipeline Real-Time (Pekan 14)
joblib.dump(model_iso, 'model_isolation_forest.pkl')
print("\n[SUKSES] Model berhasil disimpan ke berkas 'model_isolation_forest.pkl'!")
```

---

### Langkah 4: Visualisasi Batas Keputusan (*Decision Boundary*) Isolation Forest
Mari kita gambar peta kontur keputusan (*Decision Boundary*) dari model `IsolationForest` untuk melihat bagaimana AI mengisolasi kerumunan data normal dari ruang anomali di sekelilingnya:

```python
# Membuat grid koordinat 2D untuk memetakan area keputusan model
xx, yy = np.meshgrid(np.linspace(10, 48, 200), np.linspace(3.0, 9.5, 200))
grid_points = pd.DataFrame(np.c_[xx.ravel(), yy.ravel()], columns=fitur_sensor)
Z = model_iso.decision_function(grid_points)
Z = Z.reshape(xx.shape)

plt.figure(figsize=(10, 6))
# Menggambar kontur skor anomali dan garis batas keputusan (Z = 0)
plt.contourf(xx, yy, Z, levels=np.linspace(Z.min(), 0, 7), cmap=plt.cm.PuBu, alpha=0.5)
plt.contour(xx, yy, Z, levels=[0], linewidths=2, colors='darkred')

# Memisahkan titik Normal (+1) dan Anomali (-1)
normal = df_anomali[df_anomali['label_ai'] == 1]
anomali = df_anomali[df_anomali['label_ai'] == -1]

plt.scatter(normal['suhu_tanah'], normal['ph_tanah'], c='#2ECC71', edgecolors='k', s=45, label='Data Normal (+1)')
plt.scatter(anomali['suhu_tanah'], anomali['ph_tanah'], c='#E74C3C', edgecolors='k', marker='D', s=90, label='Anomali Terdeteksi (-1)')

plt.title("Langkah 4: Peta Isolasi Anomali Multivariat dengan Isolation Forest", fontweight='bold')
plt.xlabel("Suhu Tanah (°C)")
plt.ylabel("pH Tanah")
plt.legend(loc='upper right')
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.show()
```

---

## 5. Panduan Troubleshooting
1. **Error `ValueError: Expected 2D array, got 1D array instead`:**
   Terjadi jika Anda memanggil `model.fit(df['suhu_lag_1'], y)` dengan satu tanda kurung siku. Pastikan selalu menggunakan kurung siku ganda `df[['suhu_lag_1']]` agar bertipe `DataFrame` 2 dimensi.
2. **Error `TypeError: mean_squared_error() got an unexpected keyword argument 'squared'`:**
   Pada `scikit-learn` versi 1.4 ke atas, argumen `squared=False` telah dihapus. Gunakan `np.sqrt(mean_squared_error(y_test, prediksi))` seperti pada modul ini agar kompatibel di semua versi Python.

---

## 6. Tugas Mandiri Praktikum (Laporan Praktikum — Bobot RPS 10%)
1. **Perbandingan Model Forecasting (`LinearRegression` vs `RandomForestRegressor`):**
   Pada Langkah 2, impor `from sklearn.ensemble import RandomForestRegressor`. Latih model `RandomForestRegressor(n_estimators=100, random_state=42)` menggunakan `X_train` dan `y_train` yang sama. Bandingkan nilai **MAE, RMSE, dan $R^2$** antara `LinearRegression` dan `RandomForestRegressor` dalam sebuah tabel, serta plot kedua garis prediksinya!
2. **Eksperimen Parameter `contamination` pada Isolation Forest:**
   Pada Langkah 3, uji tiga nilai parameter `contamination` yang berbeda: `0.005` ($0,5\%$), `0.02` ($2\%$), dan `0.10` ($10\%$). Amati bagaimana perubahan parameter tersebut mempersempit atau memperlebar lingkaran garis batas merah (*decision boundary*) dan berapa banyak data normal yang ikut tertangkap (*False Positive*) saat `contamination=0.10`!

---

## 7. Referensi & Acuan Materi Pekan Ini

1. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730) — **Bab 6 & Bab 7 (Applying Machine Learning to IoT Data).**  
2. **Fawcett, T., & Provost, F. (2013).** *Data Science for Business*. O'Reilly Media. (ISBN-13: 978-1449361327)  
3. **Goodfellow, I., Bengio, Y., & Courville, A. (2016).** *Deep Learning*. MIT Press. (ISBN-13: 978-0262035613)  
4. **Scikit-learn Developers. (2024).** *Scikit-learn Documentation: `LinearRegression`, `IsolationForest`, & `metrics`*. Tersedia: [https://scikit-learn.org/](https://scikit-learn.org/)  
5. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
