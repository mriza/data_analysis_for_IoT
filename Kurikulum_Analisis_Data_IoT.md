---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Kurikulum Berbasis OBE & ACM: Analisis Data untuk IoT"
---

# Kurikulum Berbasis OBE & ACM: Analisis Data untuk IoT

**Program Studi:** Teknologi Rekayasa Komputer (TRK)
**Institusi:** Politeknik Pertanian Negeri Payakumbuh
**Semester:** 5
**Bobot:** 2 SKS (1 SKS Teori, 1 SKS Praktikum)
**Sifat Mata Kuliah:** Wajib/Pilihan

---

## 1. Deskripsi Mata Kuliah
Mata kuliah **Analisis Data untuk IoT** membekali mahasiswa dengan pengetahuan dan keterampilan komprehensif untuk menangani, memproses, menganalisis, dan memvisualisasikan data yang dihasilkan oleh jaringan *Internet of Things* (IoT). Mengingat konteks Politeknik Pertanian, penerapan analitik difokuskan pada studi kasus yang relevan seperti *Smart Agriculture*, *Smart Farming*, dan *Environmental Monitoring*. 

Mahasiswa akan mempelajari karakteristik unik data IoT (time-series, berisik, volume tinggi), teknik pra-pemrosesan data sensor, analitik di Edge vs Cloud, pengenalan *Machine Learning* untuk deteksi anomali dan *predictive maintenance*, serta perancangan *dashboard* interaktif secara real-time.

---

## 2. Rujukan ACM Curricula
Mata kuliah ini dirancang dengan merujuk pada standar **ACM/IEEE Computing Curricula** (khususnya *Computer Engineering* dan *Information Technology*):
- **CE-IoT (Internet of Things):** *Data processing at the edge/cloud, sensor data acquisition.*
- **IT-DAT (Data Management and Analytics):** *Data cleansing, exploratory data analysis, data visualization.*
- **CE-CEI (Cloud and Edge Infrastructure):** *Edge computing analytics, data streaming.*

---

## 3. Capaian Pembelajaran Lulusan (CPL) / *Program Learning Outcomes (PLO)*
CPL yang dibebankan pada mata kuliah ini meliputi:
- **CPL-1 (Sikap):** Menunjukkan sikap bertanggung jawab atas pekerjaan di bidang keahliannya secara mandiri dan mengedepankan etika pemanfaatan data.
- **CPL-2 (Pengetahuan):** Menguasai konsep teoretis arsitektur sistem IoT, analisis data, dan kecerdasan buatan (*Machine Learning* dasar).
- **CPL-3 (Keterampilan Umum):** Mampu menerapkan pemikiran logis, kritis, dan inovatif dalam memecahkan masalah analisis data nyata pada domain pertanian dan sistem embedded.
- **CPL-4 (Keterampilan Khusus):** Mampu merancang, mengembangkan, dan mengimplementasikan sistem analisis data IoT (dari *edge* ke *cloud*) serta memvisualisasikannya menjadi informasi yang *actionable*.

---

## 4. Capaian Pembelajaran Mata Kuliah (CPMK) / *Course Learning Outcomes (CLO)*
Setelah menyelesaikan mata kuliah ini, mahasiswa diharapkan mampu:
- **CPMK 1:** Menjelaskan karakteristik data IoT dan arsitektur analitik data (Edge, Fog, Cloud) sesuai dengan standar industri. (C2)
- **CPMK 2:** Menerapkan teknik pra-pemrosesan (*data cleaning, smoothing, outlier detection*) pada data sensor mentah berformat *time-series*. (C3, P3)
- **CPMK 3:** Melakukan *Exploratory Data Analysis* (EDA) untuk mengekstrak *insight* dari data IoT pertanian/lingkungan. (C4, P4)
- **CPMK 4:** Menerapkan algoritma *Machine Learning* dasar (klasifikasi, regresi, deteksi anomali) untuk analisis data IoT prediktif. (C3, P3)
- **CPMK 5:** Merancang dan membangun *dashboard* visualisasi data IoT interaktif secara *real-time*. (C5, P4)

---

## 5. Rencana Pembelajaran Semester (RPS) - 16 Pertemuan

| Pekan | Kemampuan Akhir (Sub-CPMK) | Bahan Kajian / Topik Pembelajaran (ACM Topics) | Metode Pembelajaran | Bobot Penilaian |
|:---:|---|---|---|:---:|
| **1** | Memahami konsep dasar analitik IoT dan arsitektur aliran data | - Pengantar Analisis Data untuk IoT<br>- Karakteristik Data IoT<br>- Arsitektur Edge, Fog, dan Cloud Analytics | Kuliah interaktif, Diskusi (Teori) | 5% |
| **2** | Melakukan akuisisi dan *ingestion* data dari protokol IoT | - Pengenalan Python untuk IoT<br>- Pengenalan *Message Broker* (RabbitMQ)<br>- Simulasi Publikasi & *Subscribe* Sensor | Praktikum (Node-RED/Python) | 5% |
| **3** | Memahami sifat data *time-series* dari perangkat sensor | - Anatomi Data *Time-Series* (Trend, Seasonality, Noise)<br>- Sampling frequency dan agregasi data | Kuliah, Studi Kasus (Teori) | 5% |
| **4** | Melakukan eksplorasi data (*Exploratory Data Analysis*) pada data sensor | - Pemanfaatan Pandas untuk Data Sensor<br>- Visualisasi Korelasi Antar Sensor | Praktikum (Python - Pandas/NumPy) | 5% |
| **5** | Menerapkan teknik pra-pemrosesan data (*cleaning, smoothing, outlier*) | - Penanganan *Missing Values* pada data sensor<br>- Teknik *Filtering* & *Smoothing*<br>- Deteksi Anomali Statistik (Z-score, IQR) | Kuliah interaktif, Diskusi (Teori) | 5% |
| **6** | Mengimplementasikan pembersihan data anomali pada *dataset* | - Penerapan *Moving Average* menggunakan Python<br>- Identifikasi dan pembersihan outlier suhu/kelembaban | Praktikum, Tugas 1 (Data Cleaning) | 10% |
| **7** | Memahami konsep komputasi pada *Edge* (*Edge Analytics*) | - Analisis data pada perangkat Edge (Raspberry Pi)<br>- *Review* Materi Persiapan UTS | Kuliah, Demo Edge Computing (Teori) | 5% |
| **8** | **Evaluasi Tengah Semester (UTS)** | **Materi Pekan 1 - 7 (Ujian Tulis/Studi Kasus)** | **Ujian Tulis / Praktik** | **15%** |
| **9** | Memahami penerapan *Machine Learning* untuk data IoT | - Pengantar ML untuk IoT<br>- Kasus Penggunaan: *Predictive Maintenance* & *Smart Farming* | Kuliah, Diskusi (Teori) | 5% |
| **10** | Mengimplementasikan algoritma prediksi dan klasifikasi | - Evaluasi Model Regresi Time-Series<br>- *Unsupervised Anomaly Detection* (*Isolation Forest*) | Praktikum (Scikit-Learn/TensorFlow) | 10% |
| **11** | Merencanakan visualisasi dan *Dashboard* berbasis Time-Series | - Visualisasi Data IoT & Konsep Time-Series Database<br>- Desain *Alerting Rules & Triggers* | Kuliah interaktif, Diskusi (Teori) | 5% |
| **12** | Melakukan instalasi dan konfigurasi dasar *Dashboard* (Grafana/InfluxDB) | - Instalasi InfluxDB & Pembuatan Panel Grafana<br>- *Query* data dan Notifikasi | Praktikum Instalasi & Konfigurasi | 10% |
| **13** | Merancang *pipeline* analisis data IoT *end-to-end* (Proyek Akhir) | - Arsitektur Sistem Terpadu (Sensor -> Analitik -> Dashboard)<br>- *Troubleshooting* aliran data | *Project-based Learning* (Teori) | - |
| **14** | Mengimplementasikan rancangan analisis data secara terpadu | - Penyelesaian Proyek Akhir *Smart Agriculture* (Integrasi Data) | Praktikum Proyek Akhir | 5% |
| **15** | Mempresentasikan hasil rancangan analitik IoT (*Live Demo*) | - Pengujian Keseluruhan Sistem<br>- Presentasi Hasil Analisis Data (*Data Storytelling*) | Presentasi, Tanya Jawab (Praktikum) | 10% |
| **16** | **Evaluasi Akhir Semester (UAS)** | **Penilaian Proyek Akhir (*Final Project Delivery*)** | **Penilaian Proyek & Laporan** | **-** |

---

## 6. Sistem Penilaian (Standar OBE)
Evaluasi tidak hanya mengukur kemampuan kognitif, tetapi juga *skill* psikomotorik (implementasi kode/sistem) berdasarkan rubrik penilaian (*Assessment Rubrics*):
- **Tugas Mandiri / Praktikum (30%)**: Penilaian tugas mingguan dan praktikum (Data cleaning, ML script, Dashboard setup).
- **Ujian Tengah Semester - UTS (25%)**: Tes pemahaman konsep analitik data dan arsitektur (Soal objektif & essay analitis) serta praktik kecil pemrosesan data.
- **Proyek Akhir / UAS (45%)**: 
  - Laporan Proyek (15%)
  - Implementasi Sistem & Dashboard (20%)
  - Presentasi & Tanya Jawab (10%)

---

## 7. Teknologi & Perangkat Lunak (Tools)
- **Bahasa Pemrograman:** Python (Pandas, NumPy, Scikit-Learn, Matplotlib/Seaborn)
- **Platform/Tools IoT:** Node-RED, MQTT (RabbitMQ)
- **Database & Visualisasi:** InfluxDB, Grafana, ThingsBoard
- **Dataset:** Dataset publik Kaggle terkait IoT pertanian (misal: *soil moisture, temperature, humidity dataset*), data sensor cuaca.

---

## 8. Daftar Referensi
**Referensi Utama:**  
1. Hanes, D., et al. (2017). *IoT Fundamentals: Networking Technologies, Protocols, and Use Cases for the Internet of Things*. Cisco Press. (ISBN-13: 978-1587144561)  
2. Minteer, A. (2017). *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730)  
3. McKinney, W. (2022). *Python for Data Analysis: Data Wrangling with Pandas, NumPy, and Jupyter* (3rd ed.). O'Reilly Media. (ISBN-13: 978-1098104030)  
4. Fawcett, T., & Provost, F. (2013). *Data Science for Business*. O'Reilly Media. (ISBN-13: 978-1449361327)  
5. ACM/IEEE-CS. (2020). *Computing Curricula 2020: Paradigms for Global Computing Education*. (ISBN-13: 978-1450390804)  

**Referensi Pendukung:**  
1. Artikel Jurnal/Prosiding terbaru terkait implementasi *IoT Analytics* pada *Smart Agriculture*.  
2. Dokumentasi resmi infrastruktur IoT & Data Science (Scikit-Learn, Pandas, InfluxDB, Grafana, Mosquitto, RabbitMQ).  
