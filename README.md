---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Data Analysis for IoT (Analisis Data untuk IoT)"
---

# Mata Kuliah: Analisis Data untuk IoT (*Data Analysis for IoT*)

**Program Studi:** Teknologi Rekayasa Komputer (TRK)  
**Jurusan:** Rekayasa Pertanian dan Komputer — Politeknik Pertanian Negeri Payakumbuh  
**Semester / Bobot SKS:** Semester 5 / 2 SKS (1 SKS Teori, 1 SKS Praktikum)  
**Dosen Pengampu:**  
- **Mohammad Riza Nurtam, M.Kom** (Penanggung Jawab Mata Kuliah & Pengampu Praktikum)  
- **Ega Evinda Putri** (Anggota Tim Pengajar & Pengampu Teori)  

---

## 1. Deskripsi Mata Kuliah

Repositori ini berisi dokumen kurikulum berbasis *Outcome-Based Education* (OBE), Rencana Pembelajaran Semester (RPS), modul kuliah teori yang komprehensif, modul praktikum laboratorium langkah demi langkah, instrumen kuis mingguan, serta paket evaluasi ujian untuk mata kuliah **Analisis Data untuk IoT**.

Perkuliahan dirancang selama **16 Pekan Pertemuan** (dengan **Pekan 8 sebagai Evaluasi Tengah Semester / UTS** dan **Pekan 16 sebagai Evaluasi Akhir Semester / UAS**), yang membekali mahasiswa dengan kompetensi rekayasa analitik data sensor dari hulu ke hilir (*End-to-End IoT Data Analytics Pipeline*) pada domain **Smart Agriculture** (Pertanian Presisi) dan pemantauan lingkungan:
1. **Konsep Dasar & Arsitektur Analitik IoT:** Karakteristik 5V data sensor, anatomi gangguan fisik sensor, serta pembagian beban komputasi *Edge, Fog*, dan *Cloud Analytics*.
2. **Akuisisi Data (*Data Ingestion*) & Message Broker:** Protokol Publish/Subscribe **MQTT (Eclipse Mosquitto)** dan **AMQP (RabbitMQ)** menggunakan Python.
3. **Anatomi Data Time-Series & Exploratory Data Analysis (EDA):** Dekomposisi *Trend, Seasonality, Noise*, *resampling* frekuensi waktu, statistik deskriptif, dan analisis korelasi multivariat (*Heatmap*) menggunakan **Pandas, Matplotlib, & Seaborn**.
4. **Pra-pemrosesan & Pembersihan Data Sensor (*Data Pre-processing*):** Klasifikasi *Missing Values* (MCAR, MAR, MNAR), teknik imputasi deret waktu, penghalusan sinyal (*SMA, EMA, Median Filter*), serta deteksi anomali statistik (**Z-Score & Tukey's IQR**).
5. **Edge Analytics & TinyML:** Pemrograman *streaming* hemat memori menggunakan *Circular Buffer* (`deque`), *Report by Exception*, dan kompresi model **TinyML** (*Quantization & Pruning*).
6. **Machine Learning untuk IoT:** Peramalan masa depan (*Time-Series Forecasting* dengan *Lag Features*) dan deteksi anomali multivariat tanpa label (**Isolation Forest**) menggunakan **Scikit-Learn**.
7. **Time-Series Database, Visualisasi Real-Time, & Integrasi Pipeline End-to-End:** Penyimpanan data berkecepatan tinggi pada **InfluxDB** (Line Protocol & Flux Query), desain *dashboard* operasional **Grafana**, sistem peringatan dini otomatis (**Telegram Webhook Alerting**), serta orkestrasi Proyek Akhir (*Project-Based Learning*).

---

## 2. Dokumen Utama & Administrasi Mata Kuliah (Direktori Root)

1. **Kurikulum Berbasis OBE & ACM:** [`Kurikulum_Analisis_Data_IoT.md`](./Kurikulum_Analisis_Data_IoT.md)
2. **Rencana Pembelajaran Semester (RPS):** [`RPS_Analisis_Data_IoT.md`](./RPS_Analisis_Data_IoT.md) | [`RPS_Analisis_Data_IoT.odt`](./RPS_Analisis_Data_IoT.odt)
3. **Distribusi Jadwal & Tugas Dosen:** [`Distribusi_Tugas_Analisis_Data_IoT.md`](./Distribusi_Tugas_Analisis_Data_IoT.md)
4. **Kontrak Perkuliahan:** [`Kontrak_Perkuliahan.md`](./Kontrak_Perkuliahan.md)
5. **Daftar Referensi Lengkap (Buku Teks & Dokumentasi):** [`Referensi/Daftar_Referensi_Lengkap.md`](./Referensi/Daftar_Referensi_Lengkap.md)
6. **Rencana Tugas Mahasiswa (RTM) Proyek Akhir:** [`RTM_Tugas_Besar.md`](./RTM_Tugas_Besar.md)
7. **Standar Format Laporan Praktikum & Tugas Besar:** [`Format_Laporan_Praktikum.md`](./Format_Laporan_Praktikum.md) | [`Format_Laporan_Tugas_Besar.md`](./Format_Laporan_Tugas_Besar.md) | [`Logbook_Praktikum.md`](./Logbook_Praktikum.md)
8. **Instrumen Penilaian, Kisi-Kisi Ujian, & Portofolio OBE:** [`Instrumen_Penilaian.md`](./Instrumen_Penilaian.md) | [`Kisi_Kisi_Soal_Ujian.md`](./Kisi_Kisi_Soal_Ujian.md) | [`Template_Portofolio_OBE_Analisis_Data_IoT.md`](./Template_Portofolio_OBE_Analisis_Data_IoT.md)
9. **Daftar Kebutuhan Alat & Bahan Laboratorium:** [`Kebutuhan_Alat_dan_Bahan.md`](./Kebutuhan_Alat_dan_Bahan.md)

---

## 3. Distribusi Materi & Navigasi Modul Per Pekan (Pekan 1 – 16)

> [!IMPORTANT]
> **Keterangan Pekan Evaluasi (UTS & UAS):**  
> - **Pekan 8 (`Pekan_08`) adalah pekan khusus EVALUASI TENGAH SEMESTER (UTS)** yang menguji penguasaan konsep dan keterampilan praktik dari **Pekan 1 s.d. Pekan 7** (bukan penyampaian materi kuliah baru).  
> - **Pekan 16 (`Pekan_16`) adalah pekan khusus EVALUASI AKHIR SEMESTER (UAS)** berupa **Pengumpulan Proyek Akhir (*Final Project Delivery*), Laporan Tugas Besar, dan Evaluasi Komprehensif Pekan 1 s.d. Pekan 15** (bukan penyampaian materi kuliah baru).

| Pekan | Jenis Sesi Utama (RPS) | Topik Pembelajaran & Capaian Materi | Modul Teori / Panduan | Modul Praktikum | Kuis / Evaluasi | Bobot RPS |
| :---: | :---: | :--- | :---: | :---: | :---: | :---: |
| **1** | Kuliah Teori (2x50') | **Pengantar Analisis Data untuk IoT & Arsitektur Aliran Data**<br>*(Piramida DIKW, Karakteristik 5V, Anatomi Data Sensor Kotor, Arsitektur Edge vs Fog vs Cloud)* | [Modul Teori 1](./Pekan_01/Modul_Teori_Pekan_1.md) | — | [Kuis Pekan 1](./Pekan_01/Kuis_Pekan_1.md) | 5% |
| **2** | Praktikum (1x170') | **Akuisisi Data (*Data Ingestion*) & Message Broker (MQTT/AMQP)**<br>*(Setup Ubuntu Server VM, Mosquitto, RabbitMQ, Topik/Wildcard/QoS MQTT, Paho-MQTT & Pika Python)* | *(Tercakup di Modul Praktikum 2)* | [Modul Praktikum 2](./Pekan_02/Modul_Praktikum_Pekan_2.md) | [Kuis Pekan 2](./Pekan_02/Kuis_Pekan_2.md) | 5% |
| **3** | Kuliah Teori (2x50') | **Anatomi dan Karakteristik Data Time-Series**<br>*(Dekomposisi Trend, Seasonality, Cyclic, Noise, Multi-Seasonality, Downsampling, UTC/NTP, Drift vs Stuck-at)* | [Modul Teori 3](./Pekan_03/Modul_Teori_Pekan_3.md) | [Modul Praktikum 3](./Pekan_03/Modul_Praktikum_Pekan_3.md) | [Kuis Pekan 3](./Pekan_03/Kuis_Pekan_3.md) | 5% |
| **4** | Praktikum (1x170') | **Statistik Deskriptif & Exploratory Data Analysis (EDA)**<br>*(Pandas DatetimeIndex, Resampling, Skewness, Korelasi Pearson $r$, Heatmap Seaborn, Korelasi vs Kausalitas)* | [Modul Teori 4](./Pekan_04/Modul_Teori_Pekan_4.md) | [Modul Praktikum 4](./Pekan_04/Modul_Praktikum_Pekan_4.md) | [Kuis Pekan 4](./Pekan_04/Kuis_Pekan_4.md) | 5% |
| **5** | Kuliah Teori (2x50') | **Pra-pemrosesan Data Sensor: Missing Values & Smoothing**<br>*(Ketahanan Mean vs Median, Klasifikasi MCAR/MAR/MNAR, Imputasi ffill/Linear/Poly, SMA vs EMA vs Median Filter, Phase Lag)* | [Modul Teori 5](./Pekan_05/Modul_Teori_Pekan_5.md) | [Modul Praktikum 5](./Pekan_05/Modul_Praktikum_Pekan_5.md) | [Kuis Pekan 5](./Pekan_05/Kuis_Pekan_5.md) | 5% |
| **6** | Praktikum (1x170') | **Deteksi & Pembersihan Anomali Statistik (Z-Score & IQR)**<br>*(Taksonomi Outlier, Rumus Z-Score & Masking Effect, Tukey's IQR & Boxplot, Cross-Sensor Validation, Tugas 1)* | [Modul Teori 6](./Pekan_06/Modul_Teori_Pekan_6.md) | [Modul Praktikum 6](./Pekan_06/Modul_Praktikum_Pekan_6.md) | [Kuis Pekan 6](./Pekan_06/Kuis_Pekan_6.md) | 10% |
| **7** | Kuliah Teori (2x50') | **Edge Analytics, Komputasi Resource-Constrained, & TinyML**<br>*(Kendala SRAM/Flash MCU, Circular Buffer Deque $O(1)$, Report by Exception, TinyML Quantization FP32 $\rightarrow$ INT8, Review UTS)* | [Modul Teori 7](./Pekan_07/Modul_Teori_Pekan_7.md) | [Modul Praktikum 7](./Pekan_07/Modul_Praktikum_Pekan_7.md) | [Kuis Pekan 7](./Pekan_07/Kuis_Pekan_7.md) | 5% |
| **8** | **EVALUASI TENGAH SEMESTER (UTS)** | **UJIAN TENGAH SEMESTER (UTS) — Cakupan Materi Pekan 1 s.d. Pekan 7**<br>*(Evaluasi Teori Objektif/Esai & Ujian Praktik Lab Pembersihan/EDA Data Sensor)* | [**Panduan Teori UTS**](./Pekan_08/Modul_Teori_Pekan_8.md) | [**Panduan Praktik UTS**](./Pekan_08/Modul_Praktikum_Pekan_8.md) | [Review Studi Kasus UTS](./Pekan_08/Kuis_Pekan_8.md) / [Kisi-Kisi UTS](./Kisi_Kisi_Soal_Ujian.md) | **15%** |
| **9** | Kuliah Teori (2x50') | **Pengantar Machine Learning untuk IoT & Kick-Off Proyek Akhir**<br>*(Rule-Based vs ML, Formulasi Regresi/Klasifikasi/Unsupervised, Lag Features, MAE/RMSE/$R^2$, Teori Isolation Forest)* | [Modul Teori 9](./Pekan_09/Modul_Teori_Pekan_9.md)<br>[Slide PPTX](./Pekan_09/Slide_Pekan_9_Pengantar_ML_IoT.pptx) / [PDF](./Pekan_09/Slide_Pekan_9_Pengantar_ML_IoT.pdf) | — | [Kuis Pekan 9](./Pekan_09/Kuis_Pekan_9.md) | 5% |
| **10** | Praktikum (1x170') | **Implementasi Pemodelan ML: Forecasting & Isolation Forest**<br>*(Lag Feature Engineering, Chronological Split `shuffle=False`, Linear/RF Regression, Isolation Forest 2D, Ekspor `.pkl`)* | *(Mengacu ke Modul Teori 9)* | [Modul Praktikum 10](./Pekan_10/Modul_Praktikum_Pekan_10.md) | [Kuis Pekan 10](./Pekan_10/Kuis_Pekan_10.md) | 10% |
| **11** | Kuliah Teori (2x50') | **Konsep Time-Series Database (InfluxDB), Visualisasi, & Alerting**<br>*(Arsitektur TSM InfluxDB vs RDBMS, Line Protocol Tags vs Fields, UI/UX Dashboard Grafana, Alert Hysteresis & Anti-Flapping)* | [Modul Teori 11](./Pekan_11/Modul_Teori_Pekan_11.md) | — | [Kuis Pekan 11](./Pekan_11/Kuis_Pekan_11.md) | 5% |
| **12** | Praktikum (1x170') | **Instalasi & Konfigurasi InfluxDB, Grafana, dan Webhook Telegram**<br>*(Setup InfluxDB v2, Python MQTT-to-InfluxDB Bridge, Flux Query, Panel Grafana, Integrasi Bot Telegram Alert — Tugas 2)* | *(Mengacu ke Modul Teori 11)* | [Modul Praktikum 12](./Pekan_12/Modul_Praktikum_Pekan_12.md) | [Kuis Pekan 12](./Pekan_12/Kuis_Pekan_12.md) | 10% |
| **13** | Kuliah Teori (2x50') | **Arsitektur Sistem Terpadu End-to-End & Troubleshooting Pipeline**<br>*(Arsitektur 5-Lapis Proyek Akhir, Urutan Cleaning $\rightarrow$ ML $\rightarrow$ DB, Metodologi Isolasi Masalah 5 Titik Antarmuka)* | [Modul Teori 13](./Pekan_13/Modul_Teori_Pekan_13.md) | — | [Kuis Pekan 13](./Pekan_13/Kuis_Pekan_13.md) | — |
| **14** | Praktikum (1x170') | **Implementasi Integrasi Pipeline End-to-End (PjBL)**<br>*(Orkestrator `main_pipeline.py`: MQTT + Deque Cleaning + Isolation Forest `.pkl` + InfluxDB + Grafana, Progress Review)* | *(Mengacu ke Modul Teori 13)* | [Modul Praktikum 14](./Pekan_14/Modul_Praktikum_Pekan_14.md) | [Kuis Pekan 14](./Pekan_14/Kuis_Pekan_14.md) | 5% |
| **15** | Praktikum (1x170') | **Pengujian Sistem, Data Storytelling, & Live Demo Proyek Akhir**<br>*(5 Skenario Fault-Injection Testing, Kerangka SCR Data Storytelling, Etika Fallback AI, Presentasi & Live Demo)* | — | [Modul Praktikum 15](./Pekan_15/Modul_Praktikum_Pekan_15.md) | [Kuis Pekan 15](./Pekan_15/Kuis_Pekan_15.md) | 10% |
| **16** | **EVALUASI AKHIR SEMESTER (UAS)** | **UJIAN AKHIR SEMESTER (UAS) — Final Project Delivery & Evaluasi Komprehensif**<br>*(Pengumpulan Source Code, Laporan Tugas Besar, Portofolio OBE, & Soal Kasus Analitik HOTS)* | [**Panduan & Checklist UAS**](./Pekan_16/Modul_Teori_Pekan_16.md) | *(Lihat [RTM Tugas Besar](./RTM_Tugas_Besar.md))* | [Soal Kasus UAS](./Pekan_16/Kuis_Pekan_16.md) | **Akumulatif Proyek 45%** |

---

## 4. Daftar Referensi Utama & Pendukung Mata Kuliah

Seluruh modul teori dan praktikum pada setiap pekan merujuk langsung pada bab-bab spesifik dari literatur utama berikut (buku referensi tersedia di dalam folder [`Referensi/`](./Referensi/)):

### Buku Teks Utama:
1. **Hanes, D., Salgueiro, G., Grossetete, P., Barton, R., & Henry, J. (2017).** *IoT Fundamentals: Networking Technologies, Protocols, and Use Cases for the Internet of Things*. Cisco Press. (ISBN-13: 978-1587144561) *(Acuan Utama Pekan 1, 2, 7, 11, 13)*  
2. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730) *(Acuan Utama Pekan 1, 2, 3, 4, 5, 6, 9, 10, 11, 12, 13, 14, 15)*  
3. **McKinney, W. (2022).** *Python for Data Analysis: Data Wrangling with Pandas, NumPy, and Jupyter* (3rd ed.). O'Reilly Media. (ISBN-13: 978-1098104030) *(Acuan Utama Pekan 3, 4, 5, 6, 8, 14)*  
4. **Warden, P., & Situnayake, D. (2019).** *TinyML: Machine Learning with TensorFlow Lite on Arduino and Ultra-Low-Power Microcontrollers*. O'Reilly Media. (ISBN-13: 978-1492052043) *(Acuan Utama Pekan 7)*  
5. **Goodfellow, I., Bengio, Y., & Courville, A. (2016).** *Deep Learning*. MIT Press. (ISBN-13: 978-0262035613) *(Acuan Utama Pekan 9, 10)*  
6. **Fawcett, T., & Provost, F. (2013).** *Data Science for Business: What You Need to Know about Data Mining and Data-Analytic Thinking*. O'Reilly Media. (ISBN-13: 978-1449361327) *(Acuan Utama Pekan 1, 3, 4, 9, 10, 15)*  
7. **ACM/IEEE-CS. (2020).** *Computing Curricula 2020: Paradigms for Global Computing Education*. (ISBN-13: 978-1450390804) *(Acuan Kurikulum OBE & Etika Rekayasa)*  

### Dokumentasi Resmi Perangkat Lunak & Infrastruktur:
- Dokumentasi lengkap pustaka Python (`pandas`, `numpy`, `scipy`, `scikit-learn`, `matplotlib`, `seaborn`), `Jupyter Notebook`, `Ubuntu Server 24.04 LTS`, `VirtualBox`, `Eclipse Mosquitto`, `RabbitMQ`, `InfluxDB v2`, dan `Grafana` tercantum pada [`Referensi/Daftar_Referensi_Lengkap.md`](./Referensi/Daftar_Referensi_Lengkap.md).
