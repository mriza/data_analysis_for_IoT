---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Pekan 8: Evaluasi Tengah Semester (UTS) — Panduan, Kisi-Kisi, dan Ringkasan Materi Teori (Pekan 1–7)"
---

# Pekan 8: Evaluasi Tengah Semester (UTS) — Panduan, Kisi-Kisi, dan Ringkasan Materi Teori

> [!IMPORTANT]
> **STATUS PERTEMUAN PEKAN 8: EVALUASI TENGAH SEMESTER (UTS)**  
> Pekan ke-8 merupakan jadwal pelaksanaan **Ujian Tengah Semester (UTS)** dan **bukan pertemuan penyampaian materi kuliah baru**. Dokumen ini berfungsi sebagai **Panduan Resmi Ujian Teori UTS, Kisi-Kisi Kompetensi, dan Ringkasan Komprehensif Materi Pekan 1 s.d. Pekan 7** untuk membantu mahasiswa mempersiapkan diri menghadapi evaluasi.

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Cakupan Evaluasi:** Materi Pertemuan Pekan 1 s.d. Pekan 7 (CPMK 1, CPMK 2, dan CPMK 3)  
**Alokasi Waktu Ujian Teori:** 100 Menit (Ujian Sinkronus / Tertulis & Studi Kasus)  
**Bobot Penilaian:** Sesuai RPS & Kontrak Perkuliahan (15% – 25% dari Nilai Akhir)  
**Dokumen Terkait:** Lihat [`../Kisi_Kisi_Soal_Ujian.md`](../Kisi_Kisi_Soal_Ujian.md) dan [`./Kuis_Pekan_8.md`](./Kuis_Pekan_8.md).

---

## 1. Peta Kisi-Kisi Capaian Kompetensi UTS (Pekan 1 – 7)

Ujian Tengah Semester dirancang dengan pendekatan **Problem-Based & Higher-Order Thinking Skills (HOTS)** untuk mengukur ketajaman analisis arsitektural dan matematis mahasiswa, bukan sekadar hafalan definisi:

| No | Sumber Pekan | Bahan Kajian / Topik Uji | Indikator Kemampuan yang Diujikan pada UTS | Level Taksonomi Bloom |
| :---: | :---: | :--- | :--- | :---: |
| **1** | **Pekan 1** | Konsep Dasar Analitik IoT & Arsitektur Aliran Data | Menganalisis karakteristik 5V (*Volume, Velocity, Variety, Veracity, Value*), menghitung akumulasi baris/kapasitas data sensor, menjelaskan penyebab fisik data sensor kotor, serta menentukan pembagian komputasi antara **Edge, Fog, dan Cloud Analytics**. | Analisis (C4) & Evaluasi (C5) |
| **2** | **Pekan 2** | Akuisisi Data (*Data Ingestion*) & Protokol MQTT/AMQP | Membedakan pola *Pub/Sub* vs *HTTP*, merancang pola berlangganan topik MQTT dengan *wildcard* (`+` dan `#`), mengevaluasi *trade-off* **QoS 0, 1, dan 2** pada jaringan buruk, serta memilih format serialisasi *payload* (JSON vs Biner). | Penerapan (C3) & Analisis (C4) |
| **3** | **Pekan 3** | Anatomi & Dekomposisi Data Time-Series | Mengidentifikasi komponen **Trend, Seasonality (termasuk Multi-Seasonality), Cyclic, dan Noise**, membedakan model Aditif vs Multiplikatif, merancang metrik *downsampling* (`mean, max, min, std`), menjaga integritas *timestamp* (**UTC & NTP**), serta mendiagnosis **Sensor Drift vs Stuck-at Fault**. | Analisis (C4) |
| **4** | **Pekan 4** | Statistik Deskriptif & Exploratory Data Analysis (EDA) | Menginterpretasikan statistik deskriptif (`Mean, Median, Std, Skewness`), mengoperasikan `DatetimeIndex` dan `resample()` di Pandas, membaca matriks **Korelasi Pearson ($r$)** pada *Heatmap*, serta membedakan **Korelasi vs Kausalitas (*Confounding Variable*)**. | Analisis (C4) |
| **5** | **Pekan 5** | Klasifikasi Missing Values, Imputasi, & Smoothing | Mengklasifikasikan mekanisme hilangnya data (**MCAR, MAR, MNAR**), menghitung estimasi imputasi (`ffill`, `bfill`, `Linear Interpolation`), menganalisis bahaya *Cascade Failure* tanpa *Fail-Safe*, serta menghitung **SMA vs EMA vs Median Filter** beserta konsekuensi **Phase Lag**. | Penerapan (C3) & Evaluasi (C5) |
| **6** | **Pekan 6** | Deteksi Anomali Statistik & Validasi Silang Sensor | Menghitung batas *outlier* dengan metode **Z-Score ($|Z|>3$)** dan **Tukey's IQR ($Q_1 - 1,5\text{IQR}$ s.d. $Q_3 + 1,5\text{IQR}$)**, menjelaskan fenomena *Masking Effect*, serta merancang **Cross-Sensor Spatial/Physics Validation** dan deteksi *False Data Injection*. | Evaluasi (C5) |
| **7** | **Pekan 7** | Edge Analytics, Streaming Memory, & TinyML | Menganalisis keterbatasan SRAM & Flash pada mikrokontroler, menelusuri algoritma **Circular Buffer ($O(1)$ Memory)**, menerapkan *Report by Exception* & *Adaptive Sampling*, serta menganalisis dampak kompresi **TinyML (*Quantization FP32 $\rightarrow$ INT8* & *Pruning*)**. | Analisis (C4) & Evaluasi (C5) |

---

## 2. Ringkasan Rumus & Persamaan Inti (Formula Cheat-Sheet Pekan 1 – 7)

Pastikan Anda memahami makna fisis dan cara menghitung persamaan-persamaan berikut secara manual:

1. **Rata-rata (Mean) & Standar Deviasi Sampel:**
   $$\mu = \frac{1}{N} \sum_{i=1}^{N} x_i \qquad \text{dan} \qquad \sigma = \sqrt{\frac{1}{N} \sum_{i=1}^{N} (x_i - \mu)^2}$$
2. **Interpolasi Linier pada Deret Waktu:**
   $$y_t = y_0 + (t - t_0) \cdot \frac{y_1 - y_0}{t_1 - t_0}$$
3. **Simple Moving Average (SMA) Jendela $k$ (Trailing Window):**
   $$SMA_t = \frac{1}{k} \sum_{i=0}^{k-1} x_{t-i} = \frac{x_t + x_{t-1} + \dots + x_{t-k+1}}{k}$$
4. **Exponential Moving Average (EMA) Rekursif:**
   $$EMA_t = \alpha \cdot x_t + (1 - \alpha) \cdot EMA_{t-1}, \qquad \text{dengan } \alpha = \frac{2}{k + 1}$$
5. **Koefisien Korelasi Pearson ($r \in [-1, +1]$):**
   $$r_{xy} = \frac{\sum (x_i - \bar{x})(y_i - \bar{y})}{\sqrt{\sum (x_i - \bar{x})^2 \sum (y_i - \bar{y})^2}}$$
6. **Deteksi Outlier Z-Score (Parametrik — Asumsi Distribusi Normal):**
   $$Z_i = \frac{x_i - \mu}{\sigma}, \qquad \text{Outlier jika } |Z_i| > 3,0$$
7. **Deteksi Outlier Interquartile Range / Pagar Tukey (Non-Parametrik — Tangguh terhadap Skewness):**
   $$IQR = Q_3 - Q_1$$
   $$\text{Batas Bawah Valid} = Q_1 - (1,5 \times IQR) \qquad \text{dan} \qquad \text{Batas Atas Valid} = Q_3 + (1,5 \times IQR)$$
8. **Indeks Melingkar pada Circular Buffer (Ukuran Tetap $W$):**
   $$\text{head}_{\text{baru}} = (\text{head}_{\text{lama}} + 1) \bmod W$$

---

## 3. Simulasi Studi Kasus UTS & Pembahasan Model Jawaban

### Studi Kasus Simulasi: Sistem Deteksi Gas Metana pada Tambang Bawah Tanah
Sebuah perusahaan pertambangan memasang jaringan sensor gas beracun ($CO_2$ dan Metana $CH_4$) di kedalaman tambang bawah tanah. Jalur komunikasi ke permukaan menggunakan jaringan kabel panjang yang sering putus-nyambung akibat getaran mesin bor.

**Pertanyaan 1:**
*Jika terjadi kebocoran gas metana mendadak yang berisiko meledak dalam 5 detik, apakah logika analisis pemicu sirine evakuasi dan kipas blower darurat sebaiknya diletakkan di Cloud Server pusat Jakarta atau di Edge Gateway lokal di dalam terowongan tambang? Jelaskan berdasarkan parameter Latency dan Reliability!*
- **Pedoman Jawaban:** Wajib diletakkan pada **Edge Node / Edge Gateway lokal** di dalam tambang. Dari sisi **Latency**, pemrosesan di Edge hanya membutuhkan waktu milidetik tanpa jeda transmisi internet. Dari sisi **Reliability (Keandalan)**, kabel komunikasi ke permukaan sering terputus akibat getaran bor; jika logika diletakkan di Cloud, terputusnya kabel saat terjadi kebocoran gas akan membuat sirine gagal berbunyi dan membahayakan nyawa pekerja.

**Pertanyaan 2:**
*Diketahui 5 pembacaan sensor suhu terowongan (°C) setiap menit adalah `[30.0, 31.0, 32.0, 35.0, 29.0]`. Hitunglah nilai $SMA_4$ (pada menit ke-4) dan $SMA_5$ (pada menit ke-5) dengan ukuran jendela $k = 3$!*
- **Pedoman Jawaban:**
  - $SMA_4 = \frac{31,0 + 32,0 + 35,0}{3} = \frac{98,0}{3} = \mathbf{32,67^\circ\text{C}}$
  - $SMA_5 = \frac{32,0 + 35,0 + 29,0}{3} = \frac{96,0}{3} = \mathbf{32,00^\circ\text{C}}$

**Pertanyaan 3:**
*Kuartil bawah ($Q_1$) bacaan sensor kelembaban terowongan adalah $50\%$ dan kuartil atas ($Q_3$) adalah $60\%$. Tentukan batas bawah dan batas atas pagar IQR Tukey, lalu tentukan apakah pembacaan $80\%$ merupakan outlier!*
- **Pedoman Jawaban:**
  - $IQR = 60\% - 50\% = 10\%$
  - $\text{Batas Bawah} = 50\% - (1,5 \times 10\%) = 35\%$
  - $\text{Batas Atas} = 60\% + (1,5 \times 10\%) = 75\%$
  - Karena $80\% > 75\%$, maka pembacaan $80\%$ **terbukti merupakan Outlier**.

---

## 4. Tata Tertib Pelaksanaan Ujian Teori UTS
1. Mahasiswa hadir tepat waktu sesuai jadwal ujian yang ditetapkan Program Studi TRK.
2. Ujian teori terdiri dari soal objektif analitis dan/atau studi kasus esai komprehensif (lihat latihan pada [`./Kuis_Pekan_8.md`](./Kuis_Pekan_8.md)).
3. Diperkenankan menggunakan kalkulator non-program untuk perhitungan statistik (SMA, korelasi, Z-Score, dan IQR).

---

## 5. Referensi & Acuan Materi Ujian (Pekan 1 – 7)

Seluruh soal Evaluasi Tengah Semester mengacu pada literatur utama berikut:
1. **Hanes, D., et al. (2017).** *IoT Fundamentals: Networking Technologies, Protocols, and Use Cases for the Internet of Things*. Cisco Press. (ISBN-13: 978-1587144561)  
2. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730)  
3. **McKinney, W. (2022).** *Python for Data Analysis: Data Wrangling with Pandas, NumPy, and Jupyter* (3rd ed.). O'Reilly Media. (ISBN-13: 978-1098104030)  
4. **Warden, P., & Situnayake, D. (2019).** *TinyML: Machine Learning with TensorFlow Lite on Arduino and Ultra-Low-Power Microcontrollers*. O'Reilly Media. (ISBN-13: 978-1492052043)  
5. **Fawcett, T., & Provost, F. (2013).** *Data Science for Business*. O'Reilly Media. (ISBN-13: 978-1449361327)  
6. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
