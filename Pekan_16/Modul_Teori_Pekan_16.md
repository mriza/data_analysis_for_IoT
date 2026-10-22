---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Pekan 16: Evaluasi Akhir Semester (UAS) — Panduan Pengumpulan Proyek Akhir & Evaluasi Komprehensif"
---

# Pekan 16: Evaluasi Akhir Semester (UAS) — Panduan Pengumpulan Proyek Akhir & Evaluasi Komprehensif

> [!IMPORTANT]
> **STATUS PERTEMUAN PEKAN 16: EVALUASI AKHIR SEMESTER (UAS)**  
> Pekan ke-16 merupakan jadwal pelaksanaan **Ujian Akhir Semester (UAS)** dan **bukan pertemuan penyampaian materi kuliah baru**. Sesuai dengan Kurikulum Berbasis OBE ([`../Kurikulum_Analisis_Data_IoT.md`](../Kurikulum_Analisis_Data_IoT.md)) dan RPS ([`../RPS_Analisis_Data_IoT.md`](../RPS_Analisis_Data_IoT.md)), evaluasi akhir semester ditekankan pada **Pengumpulan Final Project (*End-to-End IoT Data Analytics Pipeline*), Laporan Akhir Tugas Besar, serta Ujian Studi Kasus Komprehensif (HOTS)**.

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Cakupan Evaluasi:** Integrasi Komprehensif Materi Pekan 1 s.d. Pekan 15 (CPMK 1 s.d. CPMK 5)  
**Bobot Akumulatif Proyek Akhir / UAS:** **45% dari Nilai Akhir Mata Kuliah**

---

## 1. Struktur & Bobot Penilaian Evaluasi Akhir Semester (UAS)

Berdasarkan standar *Outcome-Based Education* (OBE) pada [`../Kurikulum_Analisis_Data_IoT.md`](../Kurikulum_Analisis_Data_IoT.md) dan [`../Instrumen_Penilaian.md`](../Instrumen_Penilaian.md), komponen UAS mengukur ketercapaian seluruh CPMK (khususnya **CPMK 4: Machine Learning IoT** dan **CPMK 5: Real-Time Dashboard & End-to-End Pipeline**) dengan rincian bobot sebagai berikut:

| Komponen Evaluasi UAS | Bobot Terhadap Nilai Akhir | Indikator Penilaian Utama | Acuan Dokumen |
| :--- | :---: | :--- | :--- |
| **1. Implementasi Sistem End-to-End & Dashboard** | **20%** | Kestabilan aliran data 5 lapis (Collector $\rightarrow$ Broker $\rightarrow$ Worker $\rightarrow$ TSDB $\rightarrow$ Dashboard), ketepatan kode *data cleaning* & inferensi *Machine Learning*, serta berfungsinya panel Grafana dan *Alert Webhook* Telegram secara *real-time*. | [`../RTM_Tugas_Besar.md`](../RTM_Tugas_Besar.md) & [`../Instrumen_Penilaian.md`](../Instrumen_Penilaian.md) |
| **2. Laporan Proyek Akhir & Dokumentasi Kode** | **15%** | Kelengkapan sistematika laporan (Bab I – Bab V, minimal 7 halaman), diagram arsitektur, ketajaman analisis *insight* pertanian presisi, serta kerapian repositori GitHub. | [`../Format_Laporan_Tugas_Besar.md`](../Format_Laporan_Tugas_Besar.md) |
| **3. Presentasi, Live Demo, & Ujian Kasus Komprehensif (HOTS)** | **10%** | Kemampuan *Data Storytelling*, kelancaran *live demo*, ketepatan menjawab pertanyaan penguji, serta penyelesaian soal analitik komprehensif pada [`./Kuis_Pekan_16.md`](./Kuis_Pekan_16.md). | [`./Kuis_Pekan_16.md`](./Kuis_Pekan_16.md) & [`../Kisi_Kisi_Soal_Ujian.md`](../Kisi_Kisi_Soal_Ujian.md) |
| **TOTAL AKUMULATIF PROYEK AKHIR / UAS** | **45%** | | |

---

## 2. Daftar Periksa Pengumpulan Luaran UAS (*Final Deliverables Checklist*)

Setiap kelompok mahasiswa wajib mengumpulkan berkas-berkas luaran berikut sebelum batas waktu jadwal UAS berakhir:

### A. Repositori Kode Sumber (*Source Code Repository*)
Kode program dikumpulkan dalam bentuk tautan repositori GitHub (atau arsip `.zip` bernama `UAS_KelompokX_AnalisisDataIoT.zip`) dengan struktur direktori standar berikut:
```text
Proyek_Akhir_Kelompok_X/
├── README.md                      # Panduan instalasi, arsitektur, dan cara menjalankan sistem
├── requirements.txt               # Daftar pustaka Python (pandas, scikit-learn, paho-mqtt, influxdb-client, joblib)
├── docker-compose.yml             # (Opsional) Konfigurasi kontainer Mosquitto, InfluxDB, & Grafana
├── src/
│   ├── sensor_simulator.py        # Skrip Data Collector / Firmware ESP32 (.ino)
│   ├── train_model.py             # Skrip pelatihan model Machine Learning offline
│   ├── model_iot.pkl              # Berkas model Machine Learning terlatih
│   └── main_pipeline.py           # Skrip orkestrator utama (Subscriber + Cleaning + ML + InfluxDB Writer)
├── grafana/
│   └── dashboard_export.json      # Hasil ekspor konfigurasi JSON Dashboard Grafana kelompok
└── docs/
    └── Laporan_Tugas_Besar.pdf    # Dokumen Laporan Akhir lengkap (Bab I - Bab V)
```

### B. Dokumen Laporan Tugas Besar (PDF)
Disusun mengikuti format baku pada [`../Format_Laporan_Tugas_Besar.md`](../Format_Laporan_Tugas_Besar.md):
- **Sampul & Abstrak** (maksimal 250 kata).
- **Bab I (Pendahuluan):** Latar belakang masalah pertanian/lingkungan, rumusan masalah, dan tujuan.
- **Bab II (Arsitektur Sistem & Dasar Teori):** Tinjauan pustaka dan **Diagram Blok 5 Lapis Arsitektur** wajib.
- **Bab III (Metodologi & Implementasi):** Penjelasan skema topik MQTT/AMQP, algoritma *data cleaning/imputation*, model *Machine Learning*, skema *Measurement/Tags/Fields* InfluxDB, dan kueri Flux Grafana.
- **Bab IV (Hasil & Pembahasan):** Bukti *screenshot Dashboard* saat normal dan saat terjadi anomali, bukti tangkapan layar notifikasi *Alert* Telegram, analisis metrik akurasi/latensi, serta kendala dan solusi (*troubleshooting*).
- **Bab V (Kesimpulan & Saran)**, **Daftar Pustaka (IEEE/APA)**, dan **Lampiran Matriks Kontribusi Anggota**.

### C. Penyelesaian Ujian Analitik Komprehensif UAS (`Kuis_Pekan_16.md`)
Selain luaran proyek kelompok, setiap mahasiswa wajib memahami dan mampu menjawab 10 studi kasus komprehensif pada berkas [`./Kuis_Pekan_16.md`](./Kuis_Pekan_16.md) yang menguji sintesis konsep dari Pekan 1 hingga Pekan 15.

---

## 3. Ringkasan Peta Materi Pasca-UTS (Pekan 9 – 15) untuk Review UAS

| Pekan | Topik Utama | Intisari Konsep yang Wajib Dikuasai pada Evaluasi Akhir (UAS) |
| :---: | :--- | :--- |
| **9** | Paradigma Machine Learning IoT | Kelemahan aturan `IF-ELSE` statis pada interaksi multivariat; perbedaan *Supervised Regression* (*Forecasting*), *Classification*, dan *Unsupervised Anomaly Detection*; perbedaan **Sensor Drift vs Concept Drift**. |
| **10** | Implementasi Forecasting & Isolation Forest | Pembuatan fitur waktu mundur (**Lag-1, Lag-2**), aturan wajib **`shuffle=False`** pada `train_test_split` deret waktu, evaluasi **MAE, RMSE, $R^2$**, serta prinsip isolasi pohon acak pada **Isolation Forest** (`+1` Normal vs `-1` Anomali). |
| **11** | Time-Series Database & Desain Visualisasi | Keunggulan arsitektur **TSM InfluxDB** dibandingkan B+Tree MySQL; anatomi **Line Protocol** (`Measurement`, **Tags berindeks** vs **Fields tak berindeks**, `Timestamp`); prinsip UI/UX *5-Second Rule*; pencegahan **Alert Flapping** dengan *Pending (`For`) Duration* & *Hysteresis*. |
| **12** | Praktik InfluxDB, Flux, & Grafana Alerting | Penulisan data dari Python (`influxdb-client`), penyusunan kueri **Flux** (`from |> range |> filter |> aggregateWindow`), pembuatan panel *Time-Series / Gauge / Stat*, dan integrasi *Contact Point* **Telegram Bot Webhook**. |
| **13** | Arsitektur Pipeline End-to-End & Troubleshooting | Integrasi 5 lapis komponen; urutan mutlak **`Ingestion -> Cleaning -> ML -> Database -> Dashboard`**; pencegahan *Field Type Conflict*; serta prosedur isolasi masalah pada 5 titik antarmuka. |
| **14** | Orkestrasi Real-Time (`main_pipeline.py`) | Penggabungan *MQTT Subscriber*, *Circular Buffer (`deque`)*, *imputation & smoothing on-the-fly*, pemanggilan model `.pkl` (`joblib`), dan penyimpanan ke InfluxDB dalam satu proses *worker* kontinu. |
| **15** | System Testing, Data Storytelling, & Etika AI | Pengujian injeksi kesalahan (*Fault-Injection Testing*), penyampaian *insight* dengan kerangka **SCR (*Situation-Complication-Resolution*)**, serta kewajiban merancang **Fallback Mekanis/Manual** demi keselamatan keteknikan. |

---

## 4. Referensi & Acuan Komprehensif Mata Kuliah

1. **Hanes, D., Salgueiro, G., Grossetete, P., Barton, R., & Henry, J. (2017).** *IoT Fundamentals: Networking Technologies, Protocols, and Use Cases for the Internet of Things*. Cisco Press. (ISBN-13: 978-1587144561)  
2. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730)  
3. **McKinney, W. (2022).** *Python for Data Analysis: Data Wrangling with Pandas, NumPy, and Jupyter* (3rd ed.). O'Reilly Media. (ISBN-13: 978-1098104030)  
4. **Warden, P., & Situnayake, D. (2019).** *TinyML: Machine Learning with TensorFlow Lite on Arduino and Ultra-Low-Power Microcontrollers*. O'Reilly Media. (ISBN-13: 978-1492052043)  
5. **Goodfellow, I., Bengio, Y., & Courville, A. (2016).** *Deep Learning*. MIT Press. (ISBN-13: 978-0262035613)  
6. **Fawcett, T., & Provost, F. (2013).** *Data Science for Business: What You Need to Know about Data Mining and Data-Analytic Thinking*. O'Reilly Media. (ISBN-13: 978-1449361327)  
7. **ACM/IEEE-CS. (2020).** *Computing Curricula 2020: Paradigms for Global Computing Education*. (ISBN-13: 978-1450390804)  
8. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
