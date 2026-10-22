---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Kuliah Pekan 9: Pengantar Machine Learning untuk IoT, Time-Series Forecasting, dan Deteksi Anomali Lanjut"
---

# Materi Kuliah Pekan 9: Pengantar Machine Learning untuk IoT, Time-Series Forecasting, dan Deteksi Anomali Lanjut

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 9 (Kuliah Teori Pertama Pasca-UTS & Kick-Off Proyek Akhir — $2 \times 50$ Menit)  
**Capaian Pembelajaran (Sub-CPMK 9):** Mahasiswa mampu membedakan paradigma pemrograman berbasis aturan (*Rule-Based*) dan *Machine Learning* (ML), merumuskan masalah (*Problem Formulation*) ML pada studi kasus IoT pertanian, memahami konsep *Feature Engineering Lag* dan metrik evaluasi pada *Time-Series Forecasting*, serta menjelaskan prinsip kerja deteksi anomali multivariat tanpa label (*Isolation Forest*).

---

## 1. Tujuan Pembelajaran Khusus
Setelah mengikuti perkuliahan pekan ini, mahasiswa diharapkan mampu:
1. Membedakan cara kerja **Pemrograman Berbasis Aturan (*Rule-Based / Hard-Coded IF-ELSE*)** dan **Pembelajaran Mesin (*Machine Learning*)**, serta menjelaskan mengapa aturan `IF-ELSE` gagal pada interaksi multivariat yang kompleks.
2. Mengklasifikasikan permasalahan analitik IoT ke dalam tiga tugas utama ML: **Regresi (*Forecasting*)**, **Klasifikasi**, dan **Unsupervised Anomaly Detection**.
3. Menjelaskan cara mengubah deret waktu 1 dimensi menjadi matriks fitur ber-lag (**Lag Features / Autoregression**), aturan pembagian data kronologis (**Train-Test Split dengan `shuffle=False`**), serta menghitung metrik evaluasi **MAE, MSE, RMSE, dan $R^2$**.
4. Menjelaskan mekanisme kerja algoritma **Isolation Forest** dalam mengisolasi *outlier* multidimensi menggunakan pohon partisi acak (*Isolation Trees*).
5. Membedakan fenomena **Sensor Drift** (kerusakan fisik alat) dan **Concept Drift** (perubahan dinamika lingkungan) pada model ML yang telah beroperasi di lapangan.

---

## 2. Pendahuluan: Mengapa Sistem `IF-ELSE` Tradisional Tidak Lagi Cukup?

Selama puluhan tahun, sistem kendali otomatis di industri dan pertanian dibangun menggunakan logika kondisional eksplisit yang diketik langsung oleh programmer (**Rule-Based Programming / Sistem Pakar**):
```text
IF suhu_udara > 32.0 AND kelembaban_tanah < 40.0 THEN nyalakan_pompa()
```
Pendekatan ini bekerja sangat baik apabila variabel yang dipantau hanya 1 atau 2 buah dan hubungannya bersifat linier sederhana. Namun, pada ekosistem **Pertanian Presisi (*Smart Farming*)** dan **Pemeliharaan Prediktif Mesin (*Predictive Maintenance*)**, pendekatan `IF-ELSE` runtuh karena tiga alasan utama:
1. **Ledakan Kombinasi Variabel (*Curse of Dimensionality*):** Laju stres air tanaman cabai tidak hanya ditentukan oleh suhu dan kelembaban tanah, melainkan interaksi non-linier antara 12+ variabel sekaligus (suhu udara, RH, kecepatan angin, radiasi PAR matahari, pH tanah, EC nutrisi, umur tanaman, hingga prakiraan curah hujan 3 jam ke depan). Menulis ribuan cabang `IF-ELSE` untuk seluruh kombinasi variabel tersebut secara manual adalah hal yang mustahil.
2. **Batas Ambang yang Berubah Dinamis:** Ambang kelembaban tanah $40\%$ mungkin aman bagi tanaman berumur 2 minggu di cuaca mendung, tetapi mematikan bagi tanaman berbuah lebat di siang hari yang berangin kencang.
3. **Pola Tersembunyi pada Sinyal Frekuensi Tinggi:** Manusia tidak mampu merumuskan aturan `IF-ELSE` dari 10.000 angka getaran per detik untuk menebak apakah roda gigi traktor akan patah 3 hari lagi.

### 2.1 Perbandingan Paradigma: Pemrograman Tradisional vs Machine Learning

```text
1. PEMROGRAMAN TRADISIONAL (RULE-BASED / DETERMINISTIC):
   +------------------------+        +-----------------------+        +------------------------+
   |  DATA SENSOR (Input X) | -----> |                       |        |                        |
   +------------------------+        |   KOMPUTER / CPU      | -----> |  JAWABAN / AKSI (y)    |
   |  ATURAN IF-ELSE        | -----> |   (Eksekusi Logika)   |        |  (Hasil Keputusan)     |
   |  (Diketik Manusia)     |        +-----------------------+        +------------------------+
   +------------------------+

2. PARADIGMA MACHINE LEARNING (FASE 1 — TRAINING / PELATIHAN):
   +------------------------+        +-----------------------+        +------------------------+
   |  DATA HISTORIS (X)     | -----> |                       |        |  MODEL MATEMATIS f(X)  |
   +------------------------+        |   ALGORITMA ML        | -----> |  (Aturan/Bobot yang    |
   |  LABEL/HASIL NYATA (y) | -----> |   (Proses Belajar)    |        |   Ditemukan Mesin!)    |
   +------------------------+        +-----------------------+        +------------------------+

3. PARADIGMA MACHINE LEARNING (FASE 2 — INFERENCE / PREDIKSI):
   +------------------------+        +-----------------------+        +------------------------+
   |  DATA SENSOR BARU (X') | -----> |  MODEL MATEMATIS f(X) | -----> |  PREDIKSI MASA DEPAN   |
   +------------------------+        +-----------------------+        |  (\hat{y} / Keputusan) |
                                                                      +------------------------+
```

---

## 3. Materi Inti

### 3.1 Formulasi Masalah (*Problem Formulation*) Machine Learning pada IoT
Langkah pertama seorang *IoT Data Scientist* adalah menerjemahkan masalah operasional di lapangan menjadi formulasi matematis ML yang tepat (Fawcett & Provost, 2013):

| Kategori ML | Sifat Label Target ($y$) | Pertanyaan Operasional di Lapangan | Contoh Algoritma Populer |
| :--- | :--- | :--- | :--- |
| **1. Supervised Regression (*Forecasting*)** | Tersedia label berupa **angka kontinu (desimal)**. | *"Berapa derajat suhu rumah kaca dan berapa persen kelembaban tanah pada **1 jam ke depan**?"* atau *"Berapa jam sisa umur pakai (RUL) pompa air ini?"* | *Linear Regression*, *Random Forest Regressor*, *ARIMA*, *LSTM*. |
| **2. Supervised Classification** | Tersedia label berupa **kategori/kelas diskrit**. | *"Berdasarkan spektrum getaran motor saat ini, apakah kondisi pompa **Normal (0)**, **Kavitasi (1)**, atau **Bearing Aus (2)**?"* | *Logistic Regression*, *Decision Tree*, *Random Forest Classifier*, *SVM*, *CNN*. |
| **3. Unsupervised Anomaly Detection** | **TIDAK ADA LABEL** benar/salah pada data historis. | *"Dari 10 parameter sensor di lahan hari ini, adakah titik waktu yang pola gabungannya **menyimpang/ganjil** dari kebiasaan normal?"* | **Isolation Forest**, *One-Class SVM*, *K-Means Clustering*, *Autoencoder*. |

---

### 3.2 Time-Series Forecasting: Konsep *Lag Features* & Evaluasi Model

Bagaimana cara melatih algoritma Regresi (seperti `LinearRegression` di Scikit-Learn) agar mampu meramal masa depan dari sebaris data suhu beruntun waktu?

#### A. Rekayasa Fitur Waktu Mundur (*Lag Feature Engineering / Autoregression*)
Model *Machine Learning* standar membutuhkan input berupa matriks fitur **$X$ ( penyebab / masa lalu)** dan vektor target **$y$ (akibat / masa depan)**. Pada data deret waktu tunggal, kita menciptakan kolom fitur $X$ dengan cara **menggeser nilai sensor ke bawah sebanyak $1, 2, \dots, p$ langkah waktu** (operasi `.shift(1)` di Pandas). Teknik ini disebut **Autoregression (AR)**:

```text
Transformasi Deret Waktu Tunggal Menjadi Matriks Supervised Learning (Lag-1 & Lag-2):

Waktu (t)    Suhu Saat Ini (Target y_t)    Fitur Lag-1 (y_{t-1})    Fitur Lag-2 (y_{t-2})
---------    --------------------------    ---------------------    ---------------------
08:00                  25.0°C                       NaN                      NaN        <-- Dibuang (dropna)
09:00                  26.2°C                      25.0°C                    NaN        <-- Dibuang (dropna)
10:00                  27.8°C         <===         26.2°C                   25.0°C      <-- Baris Latih 1
11:00                  29.5°C         <===         27.8°C                   26.2°C      <-- Baris Latih 2
12:00                  31.0°C         <===         29.5°C                   27.8°C      <-- Baris Latih 3
```
Persamaan regresi linier dengan fitur *Lag-1* dan *Lag-2* menjadi:
$$\hat{y}_t = \beta_0 + \beta_1 \cdot y_{t-1} + \beta_2 \cdot y_{t-2}$$

#### B. Aturan Emas Pembagian Data Time-Series: `shuffle=False`!
Dalam *Machine Learning*, kita wajib membagi data menjadi **Training Set (80% awal untuk belajar)** dan **Testing Set (20% akhir untuk ujian model)**.
> **PERINGATAN KRITIS:** Pada data *time-series*, Anda **DILARANG KERAS mengacak urutan baris (`shuffle=True`)** saat memanggil `train_test_split()`! Jika baris diacak, data masa depan (misal tanggal 20 Oktober) akan bocor masuk ke dalam data pelatihan untuk menebak masa lalu (tanggal 5 Oktober). Kebocoran masa depan ini disebut **Data Leakage (*Look-Ahead Bias*)** yang membuat akurasi model terlihat bagus palsu di laboratorium namun gagal total saat dipasang di dunia nyata!

```text
PEMBAGIAN DATA TIME-SERIES YANG BENAR (KRONOLOGIS / SHUFFLE = FALSE):
|<------------------------- 80% TRAINING SET ------------------------->|<----- 20% TESTING SET ----->|
[ 1 Okt ] [ 2 Okt ] [ 3 Okt ] .................... [ 24 Okt ] [ 25 Okt ] [ 26 Okt ] ....... [ 31 Okt ]
                 (Masa Lalu: Dipakai Melatih Model)                     (Masa Depan: Dipakai Menguji)
```

#### C. Metrik Evaluasi Akurasi Model Regresi / Forecasting
Untuk mengukur seberapa meleset tebakan mesin ($\hat{y}_i$) terhadap nilai sensor sebenarnya ($y_i$) pada sejumlah $n$ sampel data uji, digunakan tiga metrik utama:

1. **MAE (*Mean Absolute Error* — Rata-Rata Galat Mutlak):**
   $$\text{MAE} = \frac{1}{n} \sum_{i=1}^{n} |y_i - \hat{y}_i|$$
   Mengukur rata-rata selisih melesetnya tebakan dalam satuan asli sensor (misal ${}^\circ\text{C}$).
2. **MSE (*Mean Squared Error*) & RMSE (*Root Mean Squared Error*):**
   $$\text{MSE} = \frac{1}{n} \sum_{i=1}^{n} (y_i - \hat{y}_i)^2 \qquad \implies \qquad \text{RMSE} = \sqrt{\frac{1}{n} \sum_{i=1}^{n} (y_i - \hat{y}_i)^2}$$
   Karena selisih galat dikuadratkan terlebih dahulu sebelum diakarkan, **RMSE memberikan hukuman (penalti) yang jauh lebih berat apabila model melakukan kesalahan tebakan yang besar (*large error spikes*)**. Semakin mendekati $0$, model semakin presisi.
3. **Koefisien Determinasi ($R^2$ Score):**
   $$R^2 = 1 - \frac{\sum (y_i - \hat{y}_i)^2}{\sum (y_i - \bar{y})^2}$$
   Bernilai maksimal $1,0$ ($100\%$ variasi data berhasil dijelaskan oleh model). Jika $R^2 \le 0$, model gagal menangkap pola data.

#### Contoh Perhitungan Manual MAE dan RMSE
Misalkan pada 3 jam pengujian, suhu aktual rumah kaca adalah $y = [30,0,\ 32,0,\ 31,0]$ dan hasil tebakan model regresi adalah $\hat{y} = [29,0,\ 32,0,\ 33,0]$:
- Selisih galat ($y_i - \hat{y}_i$): $[+1,0,\ 0,0,\ -2,0]$
- Galat mutlak $|y_i - \hat{y}_i|$: $[1,0,\ 0,0,\ 2,0] \implies \mathbf{\text{MAE}} = \frac{1,0 + 0,0 + 2,0}{3} = \mathbf{1,00^\circ\text{C}}$
- Galat kuadrat $(y_i - \hat{y}_i)^2$: $[1,0,\ 0,0,\ 4,0] \implies \text{MSE} = \frac{1,0 + 0,0 + 4,0}{3} = 1,67 \implies \mathbf{\text{RMSE}} = \sqrt{1,67} \approx \mathbf{1,29^\circ\text{C}}$

---

### 3.3 Unsupervised Anomaly Detection: Algoritma *Isolation Forest*

Pada Pekan 6, kita melihat bahwa metode Z-Score dan IQR univariat gagal mendeteksi **Multivariate Outlier** (misalnya: Suhu $22^\circ\text{C}$ adalah normal, Kelembaban $46\%$ adalah normal, namun kombinasi keduanya di dalam *greenhouse* yang sama adalah anomali).

Algoritma **Isolation Forest** (Liu et al., 2008) menyelesaikan masalah ini dengan prinsip yang sangat elegan tanpa perlu menghitung jarak atau kepadatan distribusi:
1. Algoritma membangun ratusan pohon keputusan acak (**Isolation Trees / iTrees**) dengan memilih sebuah fitur sensor secara acak (misal Suhu atau pH) lalu memotongnya pada nilai ambang acak.
2. **Titik Anomali (*Outlier*)** jumlahnya sedikit dan terletak jauh terpencil dari kerumunan data normal. Akibatnya, hanya dibutuhkan **sedikit sekali tebasan pemotongan (panjang jalur pohon $h(x)$ sangat pendek)** untuk mengisolasi titik anomali tersebut sendirian di sebuah daun pohon!
3. Sebaliknya, **Titik Normal (*Inlier*)** berkumpul rapat di tengah kerumunan, sehingga membutuhkan **banyak sekali tebasan pemotongan (panjang jalur pohon $h(x)$ panjang)** untuk memisahkannya.

```text
           PRINSIP KERJA ISOLATION FOREST PADA RUANG 2 DIMENSI (SUHU vs pH TANAH)

   pH Tanah ^
            |      . . .                  Titik Normal (X_normal):
            |    . . X . .                Berada di tengah kerumunan padat, butuh 6-10 kali
            |    . . . . .                pemotongan garis acak sampai terisolasi.
            |      . . .                  (Jalur pohon panjang -> Label Scikit-Learn = +1)
            |------------------------
            |                   |
            |                   |   * <-- Titik Anomali Multivariat (X_anomali):
            |                   |         Langsung terisolasi sendirian hanya dengan 2 kali
            +-------------------+--->     pemotongan garis! (Jalur pendek -> Label = -1)
                                Suhu
```
Pada pustaka `scikit-learn` (`sklearn.ensemble.IsolationForest`), parameter kunci yang kita atur adalah **`contamination`** (misal `contamination=0.02` jika kita mengestimasi sekitar $2\%$ dari data historis kita merupakan anomali), dan fungsi `.fit_predict()` akan mengembalikan label **`+1` untuk Data Normal** serta **`-1` untuk Data Anomali**.

---

### 3.4 Degradasi Model ML di Lapangan: *Sensor Drift* vs *Concept Drift*
Setelah sebuah model ML di-*deploy* ke perkebunan dan berjalan selama 6 bulan, sering kali akurasinya menurun drastis ($R^2$ turun dari $0,92$ menjadi $0,45$). Seorang *IoT Engineer* harus mampu mengisolasi dua kemungkinan penyebabnya:
1. **Sensor Drift (Degradasi Perangkat Keras):** Lempeng elektroda sensor mengalami korosi sehingga angka input $X$ yang dikirim ke model tidak lagi akurat.
2. **Concept Drift / Covariate Shift (Perubahan Dinamika Alam/Lingkungan):** Sensor masih akurat, tetapi hubungan fisis antara input $X$ dan target $y$ di alam telah berubah (misalnya kepadatan tanah berubah setelah dilewati traktor berat baru, atau pergantian varietas bibit tanaman).
- **Prosedur Diagnostik:** Lakukan uji kalibrasi sensor fisik di laboratorium menggunakan larutan/alat ukur standar. Jika pembacaan sensor di laboratorium masih akurat, maka penurunan akurasi model terbukti disebabkan oleh **Concept Drift**, sehingga solusinya adalah **melatih ulang (*re-training*) model ML** menggunakan dataset terbaru!

---

## 4. Pengarahan Kick-Off Proyek Akhir (*Tugas Besar*)
Sesuai dengan dokumen [`../RTM_Tugas_Besar.md`](../RTM_Tugas_Besar.md), mulai Pekan 9 setiap kelompok (maksimal 3 mahasiswa) wajib menetapkan topik studi kasus Proyek Akhir *End-to-End IoT Data Analytics Pipeline* yang mencakup 5 komponen wajib:
1. **Data Collector** (Sensor ESP32 / Simulator Python)
2. **Message Broker** (Mosquitto MQTT / RabbitMQ AMQP / Valkey)
3. **Data Pipeline Worker** (Skrip Python *Subscriber + Data Cleaning + Anomaly Detection*)
4. **Time-Series Database** (InfluxDB / TimescaleDB / MongoDB Time-Series)
5. **Analytic Dashboard & Alerting** (Grafana / Streamlit + Telegram Webhook)

---

## 5. Bahan Diskusi & Berpikir Kritis (*HOTS*) di Kelas

1. **Tantangan ML di Pertanian Terbuka vs Pabrik Tertutup:**
   Mengapa model *Machine Learning* yang dipasang pada lahan pertanian terbuka (*open-field agriculture*) jauh lebih rentan mengalami *Concept Drift* dibandingkan model ML pada lengan robot perakitan di dalam gedung pabrik tertutup?
2. **Bahaya `shuffle=True` pada Time-Series:**
   Jelaskan apa yang terjadi jika seorang mahasiswa memanggil `train_test_split(X, y, test_size=0.2, shuffle=True)` saat melatih model peramalan suhu per jam!

---

## 6. Referensi & Acuan Materi Pekan Ini

1. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730) — **Bab 6 (Getting to Know Your Data with Machine Learning) & Bab 7 (Applying Machine Learning to IoT Data: Forecasting & Anomaly Detection).**  
2. **Fawcett, T., & Provost, F. (2013).** *Data Science for Business: What You Need to Know about Data Mining and Data-Analytic Thinking*. O'Reilly Media. (ISBN-13: 978-1449361327) — **Bab 3 (Introduction to Predictive Modeling), Bab 4 (Fitting a Model to Data), & Bab 7 (Decision Analytic Thinking: Evaluating Models).**  
3. **Goodfellow, I., Bengio, Y., & Courville, A. (2016).** *Deep Learning*. MIT Press. (ISBN-13: 978-0262035613) — **Bab 5 (Machine Learning Basics: Capacity, Overfitting and Underfitting, Estimators).**  
4. **Scikit-learn Developers. (2024).** *Scikit-learn User Guide: Supervised Learning & Novelty and Outlier Detection (`IsolationForest`)*. Tersedia: [https://scikit-learn.org/stable/user_guide.html](https://scikit-learn.org/stable/user_guide.html)  
5. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
