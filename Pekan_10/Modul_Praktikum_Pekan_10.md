# Materi Praktikum Pekan 10: Pemodelan ML (Prediksi dan Klasifikasi Anomali)

## 1. Tujuan Praktikum
- Mahasiswa mampu menerapkan konsep *Data Splitting* (Training & Testing Data) menggunakan Scikit-Learn.
- Mahasiswa mampu melatih (*fit*) model Regresi untuk melakukan *Forecasting* data *Time-Series* berlag (*lagged feature*).
- Mahasiswa mengimplementasikan *Isolation Forest* untuk mendeteksi *outlier* tersembunyi berdimensi ganda (multi-variabel).

## 2. Persiapan Alat & Prasyarat
- **Software:** Jupyter Notebook / VS Code.
- **Library Python:** `pandas`, `scikit-learn`, `numpy`.
- Pustaka tambahan: Jalankan `pip install scikit-learn` jika belum tersedia.

## 3. Dasar Teori Singkat
Dalam Machine Learning standar, Anda tidak boleh melatih model AI menggunakan seluruh data yang Anda miliki lalu mengujinya menggunakan data yang sama. Hal itu seperti memberi soal ujian yang bocor kepada siswa. Anda harus membelah data: **Train Set (80%)** untuk pelatihan dan **Test Set (20%)** yang disembunyikan. Model dinilai berdasar akurasinya menjawab *Test Set*. 
*Regression* memprediksi nilai desimal berkelanjutan (misal memprediksi kelembaban 77.5%). *Isolation Forest* bertugas memisahkan (mengisolasi) titik data spesifik yang sifat distribusinya melenceng jauh dari populasi mayoritas.

## 4. Langkah Kerja (Detail & Anotasi Kode)

### Langkah 1: Persiapan Dataset & Feature Engineering (Lag-1)
Untuk memprediksi masa depan (Suhu Jam + 1), kita butuh data masa lalu sebagai inputnya (Suhu Saat Ini). Proses pergeseran kolom ini disebut *Lagging*.

```python
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error
import numpy as np

# Load Data bersih (anggap saja ini data suhu riil berfrekuensi jam)
# (Untuk kemudahan praktik, kita bangun dummy pendek yang berpola linear naik lambat)
waktu = pd.date_range("2023-01-01", periods=100, freq="H")
suhu = np.linspace(25.0, 35.0, 100) + np.random.normal(0, 0.5, 100) # Suhu perlahan naik, ditambah sedikit noise

df = pd.DataFrame({"waktu": waktu, "suhu": suhu}).set_index("waktu")

# Feature Engineering: Membuat Lag-1
# Kolom 'Suhu_Sebelumnya' adalah fitur utama kita. Kolom 'suhu' adalah target (masa depan).
df['Suhu_Sebelumnya'] = df['suhu'].shift(1)

# Hapus baris teratas yang pasti bernilai NaN karena tidak punya nilai 'sebelumnya'
df = df.dropna()
display(df.head())
```

### Langkah 2: Splitting Data dan Model Regresi Sederhana
```python
# Membagi variabel Independen (X) dan Dependen (y)
X = df[['Suhu_Sebelumnya']] # Harus 2 dimensi (DataFrame)
y = df['suhu']              # Target akhir (Series)

# Split (80% Train, 20% Test). shuffle=False krusial untuk Time-Series agar urutan masa tidak acak!
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, shuffle=False)

# Inisiasi Algoritma Mesin
model_forecasting = LinearRegression()

# Training! (Komputer mempelajari hubungan Suhu Sebelum ke Suhu Sesudah)
model_forecasting.fit(X_train, y_train)

# Mesin Menebak (Prediksi) pada Test Data (Sisa 20% terakhir)
prediksi_suhu = model_forecasting.predict(X_test)

# Uji Akurasi menggunakan Root Mean Squared Error (RMSE) (semakin mendekati 0 semakin presisi)
rmse = mean_squared_error(y_test, prediksi_suhu, squared=False)
print(f"Kinerja Prediksi: Error rata-rata hanya meleset sebesar {rmse:.3f} Derajat Celcius.")
```

### Langkah 3: Deteksi Anomali dengan Isolation Forest
Kita melompat dari Prediksi (Supervised) ke Pencari Kejanggalan (Unsupervised).

```python
from sklearn.ensemble import IsolationForest

# Buat dataframe 2 variabel
df_anomali = pd.DataFrame({
    'suhu': np.random.normal(30, 2, 200),
    'ph_tanah': np.random.normal(6.5, 0.5, 200)
})

# Sisipkan data aneh (Suhu sangat tinggi, tanah sangat asam)
df_anomali.loc[101] = [48.0, 3.0]
df_anomali.loc[199] = [10.0, 9.0]

# Inisialisasi Model Hutan Isolasi
# contamination=0.01 berarti kita asumsikan 1% dari total populasi data adalah sensor yg rusak/anomali
model_iso = IsolationForest(contamination=0.01, random_state=42)

# Mesin belajar & langsung mencari anomali secara bersamaan
prediksi_outlier = model_iso.fit_predict(df_anomali)

# Memasukkan hasil pelabelan AI kembali ke tabel
# Nilai 1 berarti AMAN (Normal), -1 berarti ANOMALI!
df_anomali['Label_AI'] = prediksi_outlier

# Filter baris mana saja yang divonis bersalah (-1) oleh AI
data_tercyduk = df_anomali[df_anomali['Label_AI'] == -1]
print("\nBaris Data yang dicurigai AI sebagai Anomali Mutlak:")
display(data_tercyduk)
```

## 5. Hasil Eksperimen yang Diharapkan
- Anda akan melihat output nilai Error RMSE. Jika nilainya 0.5, artinya tebakan AI ke masa depan rata-rata hanya meleset setengah derajat celcius. Ini tergolong model prediktif yang baik.
- Di sesi algoritma *Isolation Forest*, baris 101 dan 199 akan seketika "ditangkap" oleh program sebagai `-1` (Anomali) tanpa kita perlu merumuskan logika IQR manual di atas dua dimensi secara ribet.

## 6. Troubleshooting
- **Error `ValueError: Expected 2D array, got 1D array instead`:** Ini terjadi jika saat inisialisasi `X` Anda menggunakan 1 kurung siku `df['Suhu_Sebelumnya']`. Scikit-Learn menuntut array 2 dimensi, pastikan Anda menggunakan dua tanda kurung siku ganda `df[['Suhu_Sebelumnya']]`!

## 7. Tugas Mandiri/Tantangan
Buat visualisasi 2 Dimensi berbasis Scatter Plot untuk model Isolation Forest tersebut! Plot titik-titik data `suhu` pada Sumbu X dan `ph_tanah` pada Sumbu Y. Berikan **warna merah cerah** (*color='red'*) pada titik data yang dilabeli nilai `-1` oleh Model Isolation Forest, dan berikan warna abu-abu untuk data berlabel `1` (Normal). Tunjukkan pada dosen pengampu bagaimana algoritma mesin berhasil menemukan titik jauh yang terisolasi di pojok grafik!
