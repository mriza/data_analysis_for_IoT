---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Kuliah Pekan 11: Arsitektur Time-Series Database (InfluxDB), Desain Visualisasi Dashboard, dan Sistem Peringatan Dini (Alerting)"
---

# Materi Kuliah Pekan 11: Arsitektur Time-Series Database (InfluxDB), Desain Visualisasi Dashboard, dan Sistem Peringatan Dini (Alerting)

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 11 (Kuliah Teori — $2 \times 50$ Menit)  
**Capaian Pembelajaran (Sub-CPMK 11):** Mahasiswa mampu menjelaskan perbedaan arsitektur internal *Time-Series Database* (InfluxDB) dibandingkan basis data relasional tradisional (MySQL/PostgreSQL), menyusun struktur data *Line Protocol* (*Measurement, Tags, Fields, Timestamp*), menerapkan prinsip UI/UX pada desain *dashboard* operasional (Grafana), serta merancang aturan pemicu peringatan dini (*Alerting Rules, Triggers, & Hysteresis*) yang tahan terhadap *false alarm*.

---

## 1. Tujuan Pembelajaran Khusus
Setelah mengikuti perkuliahan pekan ini, mahasiswa diharapkan mampu:
1. Menganalisis alasan teknis mengapa basis data relasional standar (**RDBMS** seperti MySQL) mengalami penurunan kinerja drastis (*bottleneck*) saat menangani beban tulis dan kueri deret waktu IoT berskala besar dibandingkan **Time-Series Database (TSDB)** seperti **InfluxDB**.
2. Membedah model data **InfluxDB** yang meliputi **Bucket, Measurement, Tags (Berindeks), Fields (Tak Berindeks), dan Timestamp**, serta menuliskan sintaks **InfluxDB Line Protocol**.
3. Menjelaskan fitur siklus hidup data pada TSDB: **Retention Policy (TTL)** dan **Continuous Downsampling Tasks**.
4. Memilih jenis panel visualisasi **Grafana** (*Time-Series Graph, Gauge, Stat, State Timeline, Heatmap*) yang tepat sesuai karakteristik metrik sensor dan prinsip hirarki visual ruang kendali (*Control Room UI/UX*).
5. Merancang logika **Alerting Rules & Triggers** lengkap dengan mekanisme **Pending/For Duration (Debounce)** dan **Hysteresis** untuk mencegah fenomena *Alert Flapping* pada integrasi *Webhook* (Telegram/Email).

---

## 2. Mengapa Database Relasional Standar (MySQL) Kewalahan Menghadapi Data IoT?

Pada mata kuliah Basis Data sebelumnya, Anda terbiasa menyimpan data ke dalam tabel relasional **RDBMS (MySQL / MariaDB / PostgreSQL standar)** yang menggunakan struktur indeks **B+Tree**. RDBMS dirancang untuk data transaksional (seperti biodata mahasiswa atau faktur penjualan) yang sering mengalami operasi `UPDATE` dan `JOIN` antar-tabel.

Namun, beban kerja (*workload*) data sensor IoT memiliki sifat yang sangat spesifik:
1. **99% Operasi adalah `INSERT` (*Append-Only* Berkecepatan Tinggi):** Sensor tidak pernah mengubah (`UPDATE`) suhu kemarin; sensor selalu menembakkan ribuan baris baru setiap detik di ujung waktu terbaru. Pada struktur B+Tree MySQL, setiap kali jutaan baris baru masuk, pohon indeks harus terus-menerus diseimbangkan ulang (*re-balancing*) di disk, yang membuat kecepatan tulis (*write throughput*) anjlok ketika tabel menembus puluhan juta baris.
2. **Kueri Selalu Berbasis Rentang Waktu & Agregasi (`WHERE time > now() - 7d`):** Ketika manajer kebun membuka grafik suhu 1 tahun terakhir (31,5 juta baris untuk 1 sensor berfrekuensi 1 detik), kueri SQL standar harus memindai baris yang tersebar di blok disk dan menghabiskan waktu puluhan detik (*loading* sangat lambat).
3. **Tidak Ada Mekanisme Penghapusan Data Usang Otomatis:** Menjalankan perintah `DELETE FROM sensor_log WHERE waktu < '2025-01-01'` pada 50 juta baris di MySQL akan mengunci tabel (*table lock*) dan menghentikan pemasukan data sensor baru!

---

## 3. Materi Inti

### 3.1 Arsitektur Internal Time-Series Database (InfluxDB)
**Time-Series Database (TSDB)** seperti **InfluxDB** dirancang khusus dari nol menggunakan mesin penyimpanan **TSM (*Time-Structured Merge Tree*)** yang berorientasi kolom (*Columnar Storage*):

| Aspek Arsitektur | Database Relasional (MySQL / RDBMS) | Time-Series Database (InfluxDB) |
| :--- | :--- | :--- |
| **Struktur Penyimpanan Disk** | Berorientasi Baris (*Row-Oriented*) dengan indeks **B+Tree**. | Berorientasi Kolom Waktu (*Columnar*) dengan mesin **TSM (Time-Structured Merge Tree)**. |
| **Kecepatan Tulis (*Write*)** | Melambat drastis saat ukuran indeks melebihi kapasitas RAM. | Sangat cepat (ratusan ribu titik/detik) karena ditulis berurutan ke *Write-Ahead Log (WAL)*. |
| **Rasio Kompresi Disk** | Rendah ($1\text{ baris} \approx \text{puluhan/ratusan Bytes}$). | Sangat tinggi (mencapai **$90\%$ lebih hemat** berkat kompresi *Delta-of-Delta* untuk waktu dan *Gorilla XOR* untuk angka *float*). |
| **Penghapusan Data Usang** | Manual (`DELETE`), lambat dan memicu fragmentasi disk. | Otomatis di latar belakang melalui **Retention Policy (TTL)** dengan membuang blok *shard* waktu sekaligus. |
| **Bahasa Kueri Analitik** | SQL standar (kurang praktis untuk *moving window* & *resampling*). | **Flux / InfluxQL** (memiliki fungsi bawaan `aggregateWindow()`, `derivative()`, `exponentialMovingAverage()`). |

---

### 3.2 Anatomi Model Data InfluxDB & Sintaks *Line Protocol*
Di dalam InfluxDB v2.x, data diorganisasikan ke dalam konsep berikut:

```text
+---------------------------------------------------------------------------------------------------+
| ORGANIZATION ("Politani")                                                                         |
|   └── BUCKET ("sensor_pertanian" — Memiliki Retention Policy: Simpan 90 Hari)                     |
|         └── MEASUREMENT ("iklim_mikro" — Setara dengan nama Tabel pada SQL)                       |
|               ├── TAGS (Metadata Teks Berindeks — Untuk Filter WHERE cepat):                      |
|               |     • lokasi = "Greenhouse_A"                                                     |
|               |     • node_id = "ESP32_01"                                                        |
|               ├── FIELDS (Nilai Angka Sensor Tak Berindeks — Data yang dihitung/diplot):          |
|               |     • suhu_udara = 29.4                                                           |
|               |     • kelembaban_tanah = 64.2                                                     |
|               └── TIMESTAMP (Kunci Utama Waktu — Presisi Nanodetik UTC):                          |
|                     • 1730440800000000000                                                         |
+---------------------------------------------------------------------------------------------------+
```

#### Perbedaan Kritis Antara `Tag` dan `Field` (Jangan Sampai Terbalik!)
1. **Tags (Diindeks di RAM):**
   - Selalu bertipe teks (*string*) dan digunakan untuk menyimpan **identitas/metadata statis**, seperti `lokasi`, `zona`, `tipe_tanaman`, atau `id_perangkat`.
   - Karena *Tag* diindeks di memori, kueri pencarian seperti `filter(fn: (r) => r.lokasi == "Greenhouse_A")` berjalan seketika.
   - **Larangan Keras (*High Cardinality Anti-Pattern*):** Jangan pernah memasukkan angka sensor yang terus berubah (seperti `suhu=28.43`) atau *timestamp* unik ke dalam **Tag**! Hal ini akan menciptakan jutaan kombinasi indeks unik (*Runaway Series Cardinality*) yang menghabiskan memori RAM server InfluxDB.
2. **Fields (Tidak Diindeks):**
   - Menyimpan **nilai pengukuran aktual sensor** (bertipe `float`, `integer`, `boolean`, atau `string`), seperti `suhu_udara=29.4`, `ph_air=6.8`, atau `status_anomali=-1`.

#### Format Teks *InfluxDB Line Protocol*
Saat skrip Python mengirim data ke InfluxDB, data diformat mengikuti standar **Line Protocol**:
```text
iklim_mikro,lokasi=Greenhouse_A,node_id=ESP32_01 suhu_udara=29.4,kelembaban_tanah=64.2 1730440800000000000
|_________| |__________________________________| |___________________________________| |_________________|
Measurement         Tag Set (dipisah koma)              Field Set (dipisah koma)        Timestamp (UTC ns)
           ^                                    ^                                     ^
      (Tanpa Spasi!)                      (Spasi ke-1)                          (Spasi ke-2)
```

---

### 3.3 Prinsip UI/UX Desain Dashboard Operasional IoT (Grafana)
Sebuah *Dashboard* analitik IoT (seperti **Grafana**) di ruang kontrol pertanian atau pabrik bukanlah karya seni dekoratif, melainkan **instrumen pengambilan keputusan cepat (*Situational Awareness*)**. Desain *dashboard* yang baik wajib mematuhi **Aturan 5 Detik (*5-Second Rule*)**: seorang manajer harus dapat mengetahui apakah seluruh kebun dalam kondisi aman atau bahaya hanya dalam waktu 5 detik sejak menatap layar!

```text
+===================================================================================================+
| [LAPIS 1 — HEADER DARURAT & KPI UTAMA (Paling Atas: Langsung Terlihat Mata)]                      |
| +-----------------------+ +-----------------------+ +-----------------------+ +-----------------+ |
| | STATUS SISTEM: NORMAL | | SUHU SAAT INI (GAUGE) | | KELEMBABAN TANAH      | | ANOMALI (24JAM) | |
| |      [ HIJAU ]        | |       29.4 °C         | |        64.2 %         | |    0 Kejadian   | |
| +-----------------------+ +-----------------------+ +-----------------------+ +-----------------+ |
+---------------------------------------------------------------------------------------------------+
| [LAPIS 2 — TREN HISTORIS TIME-SERIES & PREDIKSI ML (Tengah: Konteks Pergerakan Waktu)]            |
| +-------------------------------------------------+ +-------------------------------------------+ |
| | Grafik Garis: Suhu & Kelembaban (24 Jam)        | | Grafik Garis: pH Tanah & Batas Ambang     | |
| |   /\      /\      /\   --- Garis Batas Bahaya   | |   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ (pH 6.5)   | |
| |  /  \    /  \    /  \                           | |                                           | |
| +-------------------------------------------------+ +-------------------------------------------+ |
+---------------------------------------------------------------------------------------------------+
| [LAPIS 3 — DIAGNOSTIK DETAIL & LOG PERINGATAN (Bawah: Untuk Investigasi Teknis)]                  |
| | Tabel Log Anomali Terakhir | Status Baterai & RSSI Sinyal Per Node ESP32                      | |
+===================================================================================================+
```

#### Panduan Pemilihan Jenis Panel Visualisasi
1. **Stat Panel & Gauge:** Untuk menampilkan **1 nilai terkini (*Last / Current Value*)** dari parameter paling kritis (misal Suhu Saat Ini, Tekanan Pipa, atau Level Tangki Air) beserta ambang warna.
2. **Time-Series Line Chart:** Untuk menampilkan **tren perubahan sepanjang waktu**, siklus diurnal, serta membandingkan beberapa sensor sekaligus.
3. **State Timeline / Status History:** Untuk memvisualisasikan status biner/diskrit sepanjang waktu (misal kapan Pompa Irigasi berstatus `ON` vs `OFF`, atau kapan nilai `Label_AI` dari Isolation Forest bernilai `-1 / ANOMALI`).
4. **Kode Warna Semantik (*Semantic Color Coding*):**
   - **Hijau:** Beroperasi dalam rentang normal.
   - **Kuning / Oranye (*Warning*):** Mendekati batas toleransi, butuh perhatian operator.
   - **Merah Menyala (*Critical / Danger*):** Melanggar batas keselamatan atau terdeteksi anomali mutlak. Jangan pernah menggunakan warna merah untuk menghias grafik yang kondisinya sedang normal!

---

### 3.4 Merancang Sistem Peringatan Dini (*Alerting Rules, Triggers, & Hysteresis*)

Visualisasi di layar monitor tidak akan berguna apabila terjadi kebakaran rumah kaca pada pukul 03.00 dini hari saat tidak ada petugas yang menatap layar. Oleh karena itu, *Dashboard* wajib dilengkapi dengan mesin **Alerting (Sistem Peringatan Dini)** yang secara aktif mengirimkan notifikasi (melalui **Telegram Bot Webhook, Email, atau Sirine**) ketika kondisi bahaya terpenuhi.

#### A. Anatomi Aturan Alert (*Alert Rule Anatomy*) di Grafana
Sebuah aturan *alert* yang andal memiliki 4 komponen:
1. **Query & Reduction Expression:** Mengambil data dari InfluxDB (misal rata-rata `suhu_udara` dalam 5 menit terakhir).
2. **Threshold / Condition:** Aturan logika pemicu (misal: `WHEN mean(suhu_udara) IS ABOVE 35.0`).
3. **Evaluation Interval & Pending (`For`) Duration:** Seberapa sering aturan diperiksa (misal tiap `1m`) dan **berapa lama kondisi bahaya harus bertahan secara konsisten sebelum alarm benar-benar ditembakkan** (misal `For: 3m`).
4. **Contact Point (Webhook):** Saluran pengiriman pesan darurat (misal API Bot Telegram ke grup pengelola kebun).

#### B. Mengapa *Pending Duration (`For`)* dan *Hysteresis* Sangat Penting? (Mencegah *Alert Flapping*)
Bayangkan Anda menetapkan aturan sederhana tanpa durasi tunggu: `JIKA suhu > 35.0°C MAKA kirim pesan Telegram`.
Ketika suhu siang hari berada di sekitar ambang batas dan berfluktuasi tipis akibat derau sensor:
`34.9°C -> 35.1°C (KIRIM TELEGRAM!) -> 34.9°C (AMAN!) -> 35.1°C (KIRIM TELEGRAM!) -> 35.0°C -> 35.2°C (KIRIM TELEGRAM!)`

Fenomena bolak-balik ini disebut **Alert Flapping (*Chattering*)**. Ponsel manajer kebun akan dibombardir oleh ratusan pesan Telegram dalam waktu 10 menit hingga akhirnya manajer kesal dan mematikan notifikasi (*Alert Fatigue*).

**Dua Solusi Rekayasa Anti-Flapping:**
```text
1. MEKANISME PENDING / FOR DURATION (Debounce Waktu):
   Suhu harus berada di atas 35.0°C secara terus-menerus selama minimal 3 MENIT (For: 3m).
   Lonjakan derau 5 detik tidak akan memicu pengiriman pesan Telegram!

2. MEKANISME HYSTERESIS (Dual-Threshold / Ambang Ganda):
   - Batas Pemicu Alarm Aktif (Trigger Threshold)   : Suhu > 35.0°C
   - Batas Pemulihan Alarm Mati (Recovery Threshold) : Suhu < 33.5°C
   (Alarm baru dinyatakan pulih/hijau kembali setelah suhu benar-benar turun di bawah 33.5°C!)
```

---

## 4. Bahan Diskusi & Berpikir Kritis (*HOTS*) di Kelas

1. **Desain Skema Tag vs Field pada InfluxDB:**
   Sebuah perusahaan perkebunan teh ingin menyimpan data: `id_kebun`, `jenis_klon_teh`, `kecepatan_angin`, `curah_hujan`, dan `tegangan_baterai_mv` ke dalam InfluxDB. Kelompokkan manakah yang wajib dijadikan **Tag** dan manakah yang wajib dijadikan **Field**, serta jelaskan apa yang terjadi pada RAM server jika `kecepatan_angin` salah dimasukkan sebagai *Tag*!
2. **Perancangan Alert Multi-Kondisi:**
   Bagaimana Anda merancang aturan *Alert* untuk mendeteksi **"Pipa Irigasi Bocor / Pecah"** jika Anda memiliki data dari dua sensor: **Status Pompa (`ON`/`OFF`)** dan **Sensor Kelembaban Tanah**?

---

## 5. Referensi & Acuan Materi Pekan Ini

1. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730) — **Bab 4 & Bab 5 (Visualizing IoT Data, Dashboards, and Alerting).**  
2. **Hanes, D., et al. (2017).** *IoT Fundamentals: Networking Technologies, Protocols, and Use Cases for the Internet of Things*. Cisco Press. (ISBN-13: 978-1587144561)  
3. **InfluxData. (2024).** *InfluxDB v2 Documentation: Data Elements (Buckets, Measurements, Tags, Fields) & Line Protocol*. Tersedia: [https://docs.influxdata.com/influxdb/](https://docs.influxdata.com/influxdb/)  
4. **Grafana Labs. (2024).** *Grafana Documentation: Visualizations, Dashboards, & Alerting Rules*. Tersedia: [https://grafana.com/docs/](https://grafana.com/docs/)  
5. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
