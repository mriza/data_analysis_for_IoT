---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Pekan 8: Evaluasi Tengah Semester (UTS) — Panduan & Instrumen Ujian Praktikum Lab"
---

# Pekan 8: Evaluasi Tengah Semester (UTS) — Panduan & Instrumen Ujian Praktikum Lab

> [!IMPORTANT]
> **STATUS PERTEMUAN PEKAN 8: EVALUASI TENGAH SEMESTER (UTS PRAKTIKUM)**  
> Pekan ke-8 adalah pekan khusus pelaksanaan **Ujian Tengah Semester (UTS)** dan **bukan praktikum materi baru**. Dokumen ini berisi spesifikasi teknis pelaksanaan **UTS Praktik Laboratorium**, skrip pembangkit dataset ujian (`ujian_iot_mentah.csv`), rincian instruksi pengerjaan, serta rubrik penilaian penguji untuk mengukur keterampilan pemrograman analisis data mahasiswa atas materi **Pekan 1 hingga Pekan 7**.

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Bentuk Evaluasi:** Praktik Pemrograman Analisis Data Mandiri di Laboratorium (Jupyter Notebook)  
**Durasi Ujian:** 120 – 170 Menit (Sesuai Jadwal Praktikum)

---

## 1. Tata Tertib Ujian Praktikum (Lab)
1. **Sifat Ujian:** *Open Book, Open Notes*, dan *Open Official Documentation* (diizinkan membuka modul praktikum Pekan 1–7 serta dokumentasi resmi Pandas/Matplotlib/Seaborn).
2. **Larangan Keras Generative AI & Plagiarisme:** Mahasiswa **TIDAK DIPERKENANKAN** menggunakan asisten *Generative AI* (*ChatGPT, Claude, Gemini, GitHub Copilot*) maupun menyalin berkas `.ipynb` milik rekan mahasiswa lain selama ujian berlangsung. Pelanggaran akan dikenakan sanksi nilai **0 (Nol) / Diskualifikasi**.
3. **Dataset Ujian:** Mahasiswa mengunduh berkas `ujian_iot_mentah.csv` yang disediakan dosen pengampu (atau membangkitkannya menggunakan skrip resmi pada Bagian 2 di bawah ini untuk latihan mandiri/simulasi).

---

## 2. Skrip Pembangkit Dataset Ujian (`ujian_iot_mentah.csv`)

Untuk keperluan pelaksanaan ujian di laboratorium (atau simulasi latihan mandiri mahasiswa sebelum UTS), jalankan skrip Python berikut untuk menghasilkan berkas `ujian_iot_mentah.csv` (Kasus: **Pemantauan Iklim Mikro & Kualitas Udara Smart Greenhouse** selama 14 hari / 336 jam yang mengandung *missing values*, *hard outlier*, *sensor drift*, dan *noise*):

```python
import pandas as pd
import numpy as np

np.random.seed(2026)

# Rentang waktu 14 hari (336 jam)
waktu = pd.date_range(start="2026-10-01 00:00:00", periods=336, freq="h")
jam = waktu.hour

# 1. Suhu Udara (°C): Siklus diurnal + noise
suhu = 27.5 + 4.5 * np.sin((jam - 6) * np.pi / 12) + np.random.normal(0, 1.1, len(waktu))

# 2. Kelembaban Relatif (%): Berkorelasi negatif kuat dengan suhu
kelembaban = 98.0 - 1.3 * suhu + np.random.normal(0, 2.8, len(waktu))

# 3. Kadar CO2 / Kualitas Udara (ppm): Berkorelasi positif dengan aktivitas siang
kualitas_udara = 400.0 + 8.5 * (suhu - 22.0) + np.random.normal(0, 15.0, len(waktu))

df_ujian = pd.DataFrame({
    'waktu_rekam': waktu.strftime('%Y-%m-%d %H:%M:%S'),
    'suhu_udara': np.round(suhu, 2),
    'kelembaban_relatif': np.round(kelembaban, 2),
    'kualitas_udara_co2': np.round(kualitas_udara, 1)
})

# Menyuntikkan Missing Values (NaN) secara acak (~5% baris) & 1 blok putus koneksi (8 jam)
idx_missing = np.random.choice(df_ujian.index, size=15, replace=False)
df_ujian.loc[idx_missing, 'suhu_udara'] = np.nan
df_ujian.loc[100:107, 'kelembaban_relatif'] = np.nan

# Menyuntikkan Outlier Ekstrem (Korsleting Sensor)
df_ujian.loc[45, 'suhu_udara'] = 125.0
df_ujian.loc[180, 'suhu_udara'] = -15.0
df_ujian.loc[210, 'kelembaban_relatif'] = 5.0
df_ujian.loc[290, 'kualitas_udara_co2'] = 2500.0

df_ujian.to_csv('ujian_iot_mentah.csv', index=False)
print("Berkas 'ujian_iot_mentah.csv' berhasil dibuat! Total baris:", len(df_ujian))
```

---

## 3. Rincian Soal Praktik & Rubrik Penilaian (Total Skor: 100 Poin)

Buatlah sebuah berkas Jupyter Notebook dengan nama **`UTS_NIM_NamaLengkap.ipynb`** yang menyelesaikan tiga tahapan rekayasa analitik berikut secara berurutan:

### Bagian A: Data Ingestion, Parsing Waktu, & Audit Kualitas Awal (Bobot: 20 Poin)
1. **Instruksi Kerja:**
   - Muat berkas `ujian_iot_mentah.csv` menggunakan Pandas.
   - Konversikan kolom teks `waktu_rekam` menjadi tipe objek waktu (`datetime64[ns]`) dan tetapkan sebagai indeks utama (`DatetimeIndex`).
   - Tampilkan informasi struktur tabel (`df.info()`), statistik deskriptif awal (`df.describe()`), serta hitung **jumlah dan persentase (%) sel yang kosong (`NaN`)** pada setiap kolom sensor.
2. **Kriteria Rubrik (20 Poin):**
   - **(10 Poin):** Berhasil memuat CSV, mengonversi `DatetimeIndex` dengan benar, dan menampilkan 5 baris pertama.
   - **(10 Poin):** Menghitung jumlah dan persentase *missing values* serta mengidentifikasi keberadaan nilai `min`/`max` yang tidak masuk akal dari tabel `describe()`.

---

### Bagian B: Pra-pemrosesan Data — Deteksi Outlier IQR & Imputasi Time-Series (Bobot: 40 Poin)
1. **Instruksi Kerja:**
   - Hitung nilai Kuartil Bawah ($Q_1$), Kuartil Atas ($Q_3$), $IQR$, serta **Batas Bawah ($Q_1 - 1,5 \times IQR$)** dan **Batas Atas ($Q_3 + 1,5 \times IQR$)** untuk ketiga kolom sensor.
   - Tampilkan **Boxplot** sebelum pembersihan.
   - Ubah seluruh nilai yang melanggar batas pagar IQR menjadi `NaN` (**jangan menghapus baris waktu** agar urutan per jam tidak rusak!).
   - Lakukan penambalan nilai `NaN` menggunakan metode **Interpolasi (`interpolate(method='linear')` atau `'polynomial'`)**, lalu berikan komentar penjelasan di sel Markdown mengapa metode tersebut lebih tepat dibandingkan *Forward-Fill (`ffill`)* untuk jeda 8 jam pada kolom kelembaban.
   - Buktikan bahwa setelah imputasi, `df_clean.isnull().sum()` telah bernilai `0` untuk semua kolom.
2. **Kriteria Rubrik (40 Poin):**
   - **(15 Poin):** Perhitungan matematis $Q_1, Q_3, IQR$ dan *masking outlier* menjadi `NaN` berjalan akurat tanpa memotong indeks waktu.
   - **(15 Poin):** Eksekusi `.interpolate()` berhasil menambal seluruh sel kosong hingga bersih (`0 NaN`) dan dibuktikan dengan visualisasi perbandingan sebelum vs sesudah.
   - **(10 Poin):** Ketepatan argumen metodologis pemilihan interpolasi dibandingkan *ffill*.

---

### Bagian C: Smoothing, Exploratory Data Analysis (EDA), & Data Storytelling (Bobot: 40 Poin)
1. **Instruksi Kerja:**
   - Terapkan **Simple Moving Average (Window = 24 Jam)** dan **Exponential Moving Average (Span = 6 Jam)** pada kolom `suhu_udara` yang telah bersih, lalu plot keduanya bersama sinyal bersih asli dalam satu grafik garis yang rapi (lengkap dengan judul, label sumbu, legenda, dan *grid*).
   - Hitung matriks korelasi Pearson (`df_clean.corr()`) dan tampilkan visualisasi **Heatmap** menggunakan Seaborn (`annot=True, cmap='coolwarm', vmin=-1, vmax=1`).
   - **Data Storytelling (Wajib di Sel Markdown):** Tuliskan minimal **2 paragraf analisis** yang menjelaskan:
     1. Makna fisis dari angka korelasi antara `suhu_udara`, `kelembaban_relatif`, dan `kualitas_udara_co2` pada *Heatmap*.
     2. Perbedaan respons waktu (*Phase Lag*) antara kurva *Moving Average 24 Jam* dan *EMA 6 Jam* yang terlihat pada grafik Anda.
2. **Kriteria Rubrik (40 Poin):**
   - **(15 Poin):** Kode *Smoothing* (SMA 24 & EMA 6) berhasil dieksekusi dan divisualisasikan dengan atribut grafik lengkap.
   - **(10 Poin):** Visualisasi *Heatmap* Seaborn tampil sempurna dengan skala warna `-1` s.d. `1` dan anotasi angka.
   - **(15 Poin):** Kedalaman dan ketajaman narasi *Data Storytelling* dalam menerjemahkan grafik statistik menjadi informasi yang bermakna bagi pengelola *Smart Greenhouse*.

---

## 4. Ketentuan Pengumpulan Berkas Ujian
1. Sebelum menyimpan berkas, jalankan menu **Kernel $\rightarrow$ Restart & Run All** di Jupyter Notebook untuk memastikan seluruh sel kode dapat berjalan berurutan dari atas ke bawah tanpa ada *error* satu pun.
2. Pastikan seluruh output tabel dan gambar grafik sudah tercetak di dalam *notebook*.
3. Kumpulkan berkas `UTS_NIM_NamaLengkap.ipynb` (beserta hasil ekspor PDF-nya jika diminta) ke portal pengumpulan sebelum batas waktu ujian berakhir.

---

## 5. Referensi & Acuan Materi Ujian Praktikum

1. **McKinney, W. (2022).** *Python for Data Analysis: Data Wrangling with Pandas, NumPy, and Jupyter* (3rd ed.). O'Reilly Media. (ISBN-13: 978-1098104030)  
2. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730)  
3. **Pandas & Seaborn Documentation. (2024).** Tersedia: [https://pandas.pydata.org/docs/](https://pandas.pydata.org/docs/)  
4. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
