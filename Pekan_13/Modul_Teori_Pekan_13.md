---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Kuliah Pekan 13: Arsitektur Sistem Analitik IoT End-to-End, Metodologi Troubleshooting, dan Perancangan Proyek Akhir"
---

# Materi Kuliah Pekan 13: Arsitektur Sistem Analitik IoT End-to-End, Metodologi Troubleshooting, dan Perancangan Proyek Akhir

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 13 (Kuliah Teori *Project-Based Learning* — $2 \times 50$ Menit)  
**Capaian Pembelajaran (Sub-CPMK 13):** Mahasiswa mampu merancang arsitektur *pipeline* analisis data IoT *end-to-end* yang mengintegrasikan *Data Collector, Message Broker, Data Pipeline Worker (Cleaning & ML), Time-Series Database*, dan *Analytic Dashboard*, serta menerapkan metodologi *troubleshooting* sistematis pada setiap titik antarmuka aliran data.

---

## 1. Tujuan Pembelajaran Khusus
Setelah mengikuti perkuliahan pekan ini, mahasiswa diharapkan mampu:
1. Merancang cetak biru (*blueprint*) arsitektur **5 Lapis Sistem Analitik IoT Terpadu (*5-Layer End-to-End Pipeline*)** sesuai standar industri dan spesifikasi Proyek Akhir ([`../RTM_Tugas_Besar.md`](../RTM_Tugas_Besar.md)).
2. Menganalisis alasan struktural mengapa tahapan **Data Cleaning** wajib mendahului **Machine Learning Inference**, dan mengapa hasil keduanya perlu dipisahkan/ditandai sebelum masuk ke **Time-Series Database**.
3. Membandingkan peran protokol komunikasi pada lapisan tepi (**MQTT**) dan lapisan antrean internal server (**AMQP RabbitMQ / Valkey Pub-Sub**).
4. Menerapkan metodologi **Isolasi & Troubleshooting 5 Titik Antarmuka** ketika aliran data dari sensor menuju *dashboard* mengalami kemacetan, kehilangan paket, atau kegagalan layanan.
5. Menyusun dokumen spesifikasi teknis dan pembagian kerja kelompok untuk implementasi **Proyek Akhir (*Tugas Besar*)** pada Pekan 14–16.

---

## 2. Arsitektur Sistem Terpadu 5-Lapis (*End-to-End IoT Analytics Pipeline*)

Dari Pekan 1 hingga Pekan 12, kita telah mempelajari kepingan-kepingan teknologi analitik IoT secara terpisah. Di dunia industri (*Enterprise IoT*), seluruh kepingan tersebut dirangkai menjadi satu aliran otomatis tanpa henti (*24/7 Automated Data Pipeline*) yang terdiri atas **lima lapisan arsitektur utama**:

```text
+===================================================================================================+
| LAPIS 1: DATA COLLECTOR (EDGE NODE / SIMULATOR SENSOR)                                            |
| • Perangkat : Mikrokontroler ESP32 (C/C++) atau Simulator Sensor Python (`sensor_simulator.py`)   |
| • Tugas     : Akuisisi sinyal fisik (Suhu, RH, pH, Cahaya), Circular Buffer lokal, paketkan JSON  |
+===================================================================================================+
                                                 |
                                                 | Protokol Transmisi Ringan: MQTT (Port 1883/1884)
                                                 | Topik: `kebun/zona_A/telemetri` (QoS 1)
                                                 v
+===================================================================================================+
| LAPIS 2: MESSAGE BROKER & TRANSPORT HUB                                                           |
| • Perangkat : Eclipse Mosquitto (MQTT) / RabbitMQ (MQTT + AMQP) / Valkey Pub-Sub                  |
| • Tugas     : Menerima ribuan paket konkuren, autentikasi klien, menyangga antrean (Queue Buffer) |
+===================================================================================================+
                                                 |
                                                 | Konsumsi Aliran (MQTT Subscribe / AMQP Consume)
                                                 v
+===================================================================================================+
| LAPIS 3: DATA PIPELINE WORKER (ORCHESTRATOR & ANALYTICS ENGINE — `main_pipeline.py`)              |
| • Tahap 3A (Pre-processing) : Validasi skema JSON, Filter batas fisika/IQR, Imputasi NaN, EMA/SMA |
| • Tahap 3B (ML Inference)   : Load model `.pkl` -> Prediksi Forecasting & Isolation Forest (+1/-1)|
| • Tahap 3C (Enrichment)     : Tambahkan `status_anomali` & `flag_kualitas` ke objek Point data    |
+===================================================================================================+
                                                 |
                                                 | HTTP Write API — InfluxDB Line Protocol (:8086)
                                                 v
+===================================================================================================+
| LAPIS 4: TIME-SERIES DATABASE (TSDB STORAGE ENGINE)                                               |
| • Perangkat : InfluxDB v2 / TimescaleDB (PostgreSQL) / MongoDB Time-Series Collections            |
| • Tugas     : Penyimpanan kolom terkompresi cepat, pengindeksan Tag (`lokasi`), Retention Policy  |
+===================================================================================================+
                                                 |
                                                 | Kueri Analitik Waktu Nyata (Flux / SQL Query)
                                                 v
+===================================================================================================+
| LAPIS 5: ANALYTIC DASHBOARD & EARLY WARNING SYSTEM (PRESENTATION & ACTION)                        |
| • Perangkat : Grafana (:3000) / Streamlit + Integrasi Webhook Bot Telegram                        |
| • Tugas     : Visualisasi Time-Series, Gauge, Stat, & Eksekusi Alert Otomatis saat Anomali = -1   |
+===================================================================================================+
```

---

## 3. Materi Inti: Prinsip Rekayasa & Integrasi Pipeline

### 3.1 Mengapa Urutan Tahapan di Dalam *Pipeline Worker* Tidak Boleh Terbalik?
Dalam merancang skrip orkestrator (`main_pipeline.py`), urutan eksekusi logika wajib mengikuti alur:
$$\text{Ingestion} \longrightarrow \text{Data Cleaning \& Imputation} \longrightarrow \text{ML Inference} \longrightarrow \text{Database Storage} \longrightarrow \text{Dashboard \& Alerting}$$

**Apa yang terjadi jika urutan ini dilanggar (misalnya menyimpan langsung ke Database sebelum *Cleaning*)?**
1. **Kerusakan Kueri Agregasi di Dashboard:** Jika angka *hard outlier* akibat korsleting (`999.0°C` atau `-127.0°C`) langsung masuk ke kolom `suhu_udara` di InfluxDB, maka kueri `aggregateWindow(fn: mean)` dan panel `Max` di Grafana akan rusak terdistorsi, serta memicu **alarm palsu (*False Positive Alert*)** ke Telegram manajer kebun.
2. **Kegagalan Eksekusi Model Machine Learning:** Model Scikit-Learn (`LinearRegression` maupun `IsolationForest`) akan langsung mengalami *crash* (`ValueError: Input X contains NaN`) apabila disuapi nilai kosong yang belum diimputasi pada tahap *Cleaning*.
3. **Praktik Terbaik Industri (*Dual-Field / Enriched Storage Pattern*):**
   Agar jejak forensik data asli tidak hilang namun *dashboard* tetap bersih, simpanlah keduanya sebagai *field* terpisah di dalam satu titik waktu (*Point*) InfluxDB:
   - `suhu_mentah`: Menyimpan nilai asli apa adanya dari sensor (untuk audit diagnostik perangkat keras).
   - `suhu_bersih`: Menyimpan nilai yang telah melewati filter *outlier*, imputasi, dan *smoothing* (digunakan untuk grafik utama Grafana).
   - `status_anomali`: Menyimpan hasil vonis model *Isolation Forest* (`1` = Normal, `-1` = Anomali).

---

### 3.2 Pemisahan Peran Protokol: MQTT di Tepi vs AMQP / Valkey di Inti Server
Sesuai spesifikasi [`../RTM_Tugas_Besar.md`](../RTM_Tugas_Besar.md), sistem skala industri sering memadukan dua protokol:
1. **Rute Edge-ke-Broker (Wajib MQTT):**
   Jaringan nirkabel dari lahan pertanian menuju server berpotensi memiliki sinyal lemah (*lossy network*) dan keterbatasan daya baterai. Protokol **MQTT** sangat ideal di jalur ini karena *overhead* bingkainya sangat ringan (2 byte header).
2. **Rute Broker-ke-Worker Internal (MQTT / AMQP RabbitMQ / Valkey Pub-Sub):**
   Di dalam server lokal, ketika *Data Pipeline Worker* harus memproses pembersihan data dan inferensi *Machine Learning* yang memakan waktu CPU, antrean **AMQP (RabbitMQ)** atau **Valkey** berperan sebagai peredam kejut (*shock absorber / message queue*). Jika *Worker* sedang sibuk atau di-*restart*, pesan tetap mengantre dengan aman di dalam *Queue* tanpa ada satu pun paket yang tercecer.

---

### 3.3 Metodologi Sistematis *Troubleshooting* Aliran Data End-to-End

Ketika Anda menjalankan kelima komponen proyek akhir secara bersamaan dan mendapati **"Grafik di Grafana kosong (`No Data`) atau tidak bergerak"**, jangan menebak-nebak secara acak! Lakukan teknik **Divide and Conquer (Isolasi Titik Tengah)** mengikuti tabel diagnostik 5 titik berikut:

| Titik Pemeriksaan | Gejala Kegagalan | Perintah / Cara Isolasi Masalah | Solusi Perbaikan |
| :--- | :--- | :--- | :--- |
| **1. Sensor $\rightarrow$ Broker MQTT** | Publisher tampak berjalan, tapi Broker tidak menerima paket. | Jalankan `mosquitto_sub -h localhost -p 1884 -u admin -P passwordrahasia -t "kebun/#" -v` di terminal server. | Jika tidak ada teks muncul: periksa IP address VM, *firewall* (`ufw`), port (`1883` vs `1884`), dan kecocokan *username/password*. |
| **2. Broker $\rightarrow$ Pipeline Worker** | `mosquitto_sub` menerima data, tetapi skrip `main_pipeline.py` diam saja. | Periksa *string* topik pada `client.subscribe()` dan pastikan `client.loop_forever()` dipanggil. | Pastikan tidak ada salah ketik huruf besar/kecil pada topik dan `client_id` antara Publisher & Worker tidak kembar. |
| **3. Parsing & ML di dalam Worker** | Worker menerima pesan, tapi muncul *error* dan berhenti menyimpan. | Bungkus fungsi `on_message` dengan `try ... except Exception as e:` dan cetak isi `message.payload`. | Tangani JSON yang rusak (`json.JSONDecodeError`), pastikan input model ML berbentuk 2D (`[[suhu, kelembaban]]`), dan tangani nilai `NaN`. |
| **4. Worker $\rightarrow$ InfluxDB** | Worker mencetak sukses di terminal, tapi data tidak masuk ke InfluxDB. | Buka Web UI InfluxDB (`:8086`) $\rightarrow$ menu **Data Explorer**, cek *Bucket* `sensor_pertanian`. | Pastikan nama `ORG`, `BUCKET`, dan `TOKEN` benar. Pastikan tipe data *field* konsisten (`float(suhu)`, jangan kadang `int` kadang `str`!). |
| **5. InfluxDB $\rightarrow$ Grafana** | Data ada di *Data Explorer* InfluxDB, tetapi panel Grafana bertuliskan `No Data`. | Periksa **Rentang Waktu (*Time Range*)** di pojok kanan atas Grafana dan uji kueri Flux di menu *Explore*. | Ubah *Time Range* Grafana ke `Last 15 minutes` atau `Last 1 hour`, dan pastikan zona waktu *timestamp* pengirim menggunakan **UTC**! |

> **Jebakan Klasik Nomor 1 pada InfluxDB (`Field Type Conflict Error`):**
> Jika pada pengiriman pertama Anda menulis `.field("suhu", 28)` (bertipe **Integer**) lalu pada pengiriman kedua menulis `.field("suhu", 28.5)` (bertipe **Float**), InfluxDB akan **menolak pengiriman kedua (`422 Unprocessable Entity: field type conflict`)**! Oleh karena itu, di dalam kode Python, **selalu paksa konversi ke `float()`** secara eksplisit: `.field("suhu", float(nilai_suhu))`.

---

## 4. Lembar Kerja Perancangan Proyek Akhir Kelompok (Pekan 13)

Pada sesi diskusi kelas Pekan 13 ini, setiap kelompok wajib menyusun dokumen **Desain Arsitektur Proyek Akhir** (yang akan menjadi **Bab I dan Bab II** pada Laporan Tugas Besar sesuai [`../Format_Laporan_Tugas_Besar.md`](../Format_Laporan_Tugas_Besar.md)):

1. **Identitas & Studi Kasus Kelompok:**
   - Judul Kasus Pertanian/Lingkungan (misal: *Sistem Peringatan Dini Stres Air Tanaman Melon Hidroponik*, *Monitoring Kualitas Air Tambak Udang Vaname*, atau *Deteksi Dini Kebakaran Gudang Penyimpanan Padi*).
   - Daftar Parameter Sensor (minimal 2–3 variabel berkorelasi, misal: `suhu`, `kelembaban`, `ph_air` atau `amonia`).
2. **Diagram Blok Arsitektur End-to-End:**
   - Gambarkan diagram aliran data (menggunakan Draw.io / Mermaid / ASCII) lengkap dengan alamat IP, Port, nama Topik MQTT, nama *Measurement/Tags/Fields* InfluxDB, dan skema tabel.
3. **Spesifikasi Algoritma Pra-pemrosesan & Machine Learning:**
   - Batas fisika (*hard outlier threshold*) untuk setiap sensor.
   - Metode imputasi dan ukuran jendela *smoothing* (`deque(maxlen=k)` / `EMA`).
   - Model ML yang digunakan (*Isolation Forest* untuk deteksi anomali multivariat dan/atau *Linear Regression* untuk *forecasting*).
4. **Spesifikasi Dashboard & Aturan Alerting:**
   - Daftar panel Grafana yang akan dibangun dan rumus kondisi *Alert* ke Telegram.

---

## 5. Bahan Diskusi & Berpikir Kritis (*HOTS*) di Kelas

1. **Desain Ketahanan Sistem (*Fault Tolerance*) Saat Database Mati Sementara:**
   Apa yang terjadi pada skrip `main_pipeline.py` Anda apabila layanan InfluxDB tiba-tiba mengalami *restart* selama 2 menit saat sensor di kebun terus mengirimkan data via MQTT? Bagaimana Anda memodifikasi blok `try-except` di Python (atau memanfaatkan antrean *durable queue* RabbitMQ / *local CSV buffer*) agar data selama 2 menit tersebut tidak hilang terbuang?
2. **Masalah Perbedaan Jam Server vs Jam Laptop (*Timezone Mismatch*):**
   Seorang mahasiswa menjalankan simulator sensor di laptop Windows (jam lokal WIB `UTC+7`) dan mengirim *timestamp* lokal tanpa informasi zona waktu ke InfluxDB di Ubuntu Server (yang menggunakan waktu `UTC+0`). Ketika ia membuka Grafana dan memilih rentang waktu `"Last 15 minutes"`, grafik tampak kosong melompong. Jelaskan secara matematis mengapa hal itu terjadi dan bagaimana memperbaikinya!

---

## 6. Referensi & Acuan Materi Pekan Ini

1. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730) — **Bab 2 (IoT Analytics for the Cloud) & Bab 11 (Putting It All Together: Designing End-to-End IoT Analytics Pipelines).**  
2. **Hanes, D., et al. (2017).** *IoT Fundamentals: Networking Technologies, Protocols, and Use Cases for the Internet of Things*. Cisco Press. (ISBN-13: 978-1587144561)  
3. **Fawcett, T., & Provost, F. (2013).** *Data Science for Business*. O'Reilly Media. (ISBN-13: 978-1449361327)  
4. **InfluxData, Grafana Labs, & Eclipse Mosquitto Official Documentation (2024).**  
5. Lihat panduan tugas besar pada [`../RTM_Tugas_Besar.md`](../RTM_Tugas_Besar.md), [`../Format_Laporan_Tugas_Besar.md`](../Format_Laporan_Tugas_Besar.md), serta kompilasi acuan pada [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
