# Rencana Pembelajaran Semester (RPS)
**Mata Kuliah:** Analisis Data untuk IoT
**Program Studi:** Teknologi Rekayasa Komputer (TRK), Politeknik Pertanian Negeri Payakumbuh

## 1. Identitas Mata Kuliah
- **Kode Mata Kuliah:** (Disesuaikan)
- **Semester:** 5
- **Bobot SKS:** 2 SKS (1 SKS Teori, 1 SKS Praktikum)
- **Sifat:** Wajib / Pilihan
- **Dosen Pengampu:** Mohammad Riza Nurtam (Praktikum / Penanggung Jawab) & Ega Evinda Putri (Teori)

## 2. Capaian Pembelajaran Mata Kuliah (CPMK)
1. **CPMK 1:** Menjelaskan karakteristik data IoT dan arsitektur analitik data (Edge, Fog, Cloud) sesuai dengan standar industri.
2. **CPMK 2:** Menerapkan teknik pra-pemrosesan (*data cleaning, smoothing, outlier detection*) pada data sensor mentah berformat *time-series*.
3. **CPMK 3:** Melakukan *Exploratory Data Analysis* (EDA) untuk mengekstrak *insight* dari data IoT pertanian/lingkungan.
4. **CPMK 4:** Menerapkan algoritma *Machine Learning* dasar (klasifikasi, regresi, deteksi anomali) untuk analisis data IoT prediktif.
5. **CPMK 5:** Merancang dan membangun *dashboard* visualisasi data IoT interaktif secara *real-time*.

## 3. Tabel RPS
| Pekan | Sub-CPMK (Kemampuan Akhir yang Diharapkan) | Materi Pembelajaran | Metode Pembelajaran | Estimasi Waktu | Kriteria & Indikator Penilaian | Bobot (%) |
|---|---|---|---|---|---|---|
| 1 | Memahami konsep dasar analitik IoT dan arsitektur aliran data | - Pengantar Analisis Data untuk IoT<br>- Karakteristik Data IoT (Velocity, Volume, Variety)<br>- Arsitektur Edge, Fog, dan Cloud Analytics | Ceramah & Diskusi | Teori: 2x50' | Ketepatan menjelaskan perbedaan Edge, Fog, dan Cloud Analytics serta karakteristik 3V pada data IoT. | 5% |
| 2 | Melakukan akuisisi dan *ingestion* data menggunakan protokol IoT | - Pengenalan Python untuk IoT<br>- Pengenalan *Message Broker* (RabbitMQ)<br>- Simulasi Publikasi & *Subscribe* Sensor | Praktikum Mandiri | Praktik: 1x170' | Kemampuan melakukan setup broker MQTT sederhana dan menangkap data sensor simulasi. Penilaian Praktikum (Rubrik). | 5% |
| 3 | Memahami anatomi dan karakteristik data *time-series* | - Anatomi Data *Time-Series*<br>- *Trend, Seasonality, Noise*<br>- *Sampling frequency* & agregasi data waktu | Ceramah & Studi Kasus | Teori: 2x50' | Ketepatan mengidentifikasi *trend, seasonality*, dan *noise* dari visualisasi data. Kuis Singkat. | 5% |
| 4 | Melakukan eksplorasi data (*Exploratory Data Analysis*) pada data sensor | - Pemanfaatan Pandas untuk Data Sensor<br>- Visualisasi Korelasi Antar Sensor | Praktikum Terbimbing | Praktik: 1x170' | Mampu menghasilkan plot korelasi dan statistik deskriptif dari *dataset Smart Agriculture*. | 5% |
| 5 | Menerapkan teknik pra-pemrosesan (*cleaning, smoothing, outlier*) | - *Handling Missing Values* pada time-series<br>- Teknik *Filtering* & *Smoothing*<br>- Deteksi Anomali Statistik (Z-score, IQR) | Ceramah & Diskusi | Teori: 2x50' | Pemahaman teoritis mengenai metode imputasi nilai kosong dan penghalusan data berisik. | 5% |
| 6 | Mengimplementasikan pembersihan data anomali pada *dataset* pertanian | - Penerapan *Moving Average* menggunakan Python<br>- Identifikasi dan pembersihan outlier suhu/kelembaban | Praktikum Mandiri | Praktik: 1x170' | Kemampuan mengaplikasikan teknik *imputation* dan Z-Score pada Python. Tugas 1 (Data Cleaning). | 10% |
| 7 | Memahami penerapan komputasi analitik pada perangkat *Edge* | - Pengenalan *Edge Analytics*<br>- Komputasi *Resource-Constrained*<br>- *Review* Materi Persiapan UTS | Ceramah & Diskusi | Teori: 2x50' | Ketepatan membandingkan komputasi di *Edge* vs *Cloud*. | 5% |
| 8 | **Evaluasi Tengah Semester (UTS)** | **Materi Pertemuan 1 - 7** | Ujian Sinkronus | 100' | Penguasaan konsep dan praktik (Ujian Tulis/Studi Kasus). | **15%** |
| 9 | Menjelaskan *use-cases* Machine Learning pada data IoT Pertanian | - Pengantar ML untuk IoT<br>- Prediksi (*Forecasting*) & Deteksi Anomali Lanjut | Ceramah & Studi Kasus | Teori: 2x50' | Ketepatan mendefinisikan *problem formulation* ML dari studi kasus IoT Pertanian. | 5% |
| 10 | Mengimplementasikan algoritma prediksi dan klasifikasi | - Evaluasi Model Regresi Time-Series<br>- *Unsupervised Anomaly Detection* (*Isolation Forest*) | Praktikum Terbimbing | Praktik: 1x170' | Keberhasilan membangun model *Forecasting* (misal ARIMA) menggunakan *Scikit-Learn*. Laporan Praktikum. | 10% |
| 11 | Merencanakan visualisasi dan *Dashboard* berbasis Time-Series | - Visualisasi Data IoT & Konsep Time-Series Database<br>- Desain *Alerting Rules & Triggers* | Ceramah & Diskusi | Teori: 2x50' | Ketepatan dalam merencanakan pemicu (alert) dari suatu parameter sensor kritis. | 5% |
| 12 | Melakukan instalasi dan konfigurasi dasar *Dashboard* (Grafana/InfluxDB) | - Instalasi InfluxDB<br>- Pembuatan Panel Grafana<br>- *Query* data dan Notifikasi | Praktikum Mandiri | Praktik: 1x170' | Kemampuan menghubungkan *database* dengan Grafana untuk membuat visualisasi *real-time*. Tugas 2. | 10% |
| 13 | Merancang *pipeline* analisis data IoT *end-to-end* (Proyek Akhir) | - Arsitektur Sistem Terpadu (Sensor -> Analitik -> Dashboard)<br>- *Troubleshooting* aliran data | *Project-based Learning* | Teori: 2x50' | Ketepatan menyusun arsitektur sistem proyek akhir. | - |
| 14 | Mengimplementasikan rancangan analisis data secara terpadu | - Penyelesaian Proyek Akhir *Smart Agriculture* (Fase Integrasi Data dan Analisis) | Praktikum Proyek | Praktik: 1x170' | *Progress review* dari sistem analitik yang sedang dibangun. | 5% |
| 15 | Mempresentasikan hasil rancangan analitik IoT (*Live Demo*) | - Pengujian Keseluruhan Sistem<br>- Presentasi Hasil Analisis Data (*Data Storytelling*) | Presentasi & Demo | Praktik: 1x170' | Kemampuan komunikasi, argumentasi teknis, dan demonstrasi sistem yang berjalan (*Live Demo*). | 10% |
| 16 | **Evaluasi Akhir Semester (UAS)** | **Pengumpulan Final Project dan Laporan Akhir** | Penilaian Proyek | Sesuai Jadwal | Kelengkapan sistem akhir, akurasi analisis ML, dan laporan komprehensif. | **-** |

*(Catatan: Penilaian UAS ditekankan pada luaran Proyek Akhir dengan bobot akumulatif penyelesaian proyek sebesar 45% sesuai dengan Kurikulum yang diusulkan)*

## 4. Daftar Referensi
**Utama:**
1. Hanes, D., et al. (2017). *IoT Fundamentals: Networking Technologies, Protocols, and Use Cases for the Internet of Things*. Cisco Press.
2. Minteer, A. (2017). *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd.
3. McKinney, W. (2022). *Python for Data Analysis: Data Wrangling with Pandas, NumPy, and Jupyter* (3rd ed.). O'Reilly Media.
4. Fawcett, T., & Provost, F. (2013). *Data Science for Business*. O'Reilly Media.

**Pendukung:**
1. Dokumentasi resmi pustaka Python (Pandas, Scikit-Learn).
2. Dokumentasi resmi infrastruktur IoT (InfluxDB, Grafana, Mosquitto, RabbitMQ, Ubuntu Server, VirtualBox).
3. Modul Praktikum Internal (Data Analysis for IoT).
