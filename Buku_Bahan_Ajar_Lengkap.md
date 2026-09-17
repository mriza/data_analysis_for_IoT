# BUKU AJAR TEORI: ANALISIS DATA UNTUK IOT
**Mata Kuliah:** Analisis Data untuk IoT (Semester 5)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK)  
**Institusi:** Politeknik Pertanian Negeri Payakumbuh

---

## TOPIK 1: Filosofi Data pada Era IoT
### Mengapa Data Sensor Itu "Kotor"?
Dalam dunia Internet of Things (IoT), terutama di lingkungan pertanian luar ruangan (*outdoor farming*), perangkat keras sensor rentan terhadap gangguan lingkungan (suhu ekstrem, hewan, kotoran, atau degradasi baterai). Hal ini menyebabkan data yang dikirimkan ke server seringkali memiliki nilai kosong (*missing values*) atau nilai yang tidak masuk akal (*outlier*).
### Karakteristik 3V pada Data IoT
- **Volume:** Sensor IoT mengirimkan data tanpa henti (misal: setiap 5 detik), menghasilkan gigabyte bahkan terabyte data dalam hitungan bulan.
- **Velocity:** Kecepatan aliran data (*streaming*) sangat tinggi sehingga membutuhkan sistem *ingestion* khusus seperti MQTT atau Kafka.
- **Variety:** Data datang dalam berbagai format (teks JSON, CSV, nilai analog), dan jenis yang beragam (suhu, gambar kamera IoT, pH tanah).

---

## TOPIK 2: Arsitektur Edge, Fog, dan Cloud Analytics
### Cloud Analytics
Data mentah dikirim ke *server cloud* sentral (seperti AWS, GCP, atau server kampus). Keuntungannya adalah daya komputasi yang tak terbatas, namun kelemahannya adalah tingginya latensi (keterlambatan) dan ketergantungan pada koneksi internet.
### Edge Analytics
Analisis data dilakukan sedekat mungkin dengan sumber data, yaitu langsung di dalam mikrokontroler (misal: ESP32) atau *Single Board Computer* (seperti Raspberry Pi) di lahan pertanian. Hal ini menghemat *bandwidth* karena hanya informasi penting (misal: "Suhu Bahaya!") yang dikirim ke *cloud*.
### Fog Analytics
Lapisan menengah berupa *gateway* lokal (misal: PC di ruang server kebun) yang menjembatani banyak *node Edge* sebelum mencapai *Cloud*.

---

## TOPIK 3: Karakteristik Rangkaian Waktu (*Time-Series*)
Data sensor IoT selalu terikat dengan dimensi waktu (*timestamp*). Dalam analitik data, *Time-Series* memiliki anatomi khusus:
- **Trend:** Kecenderungan pergerakan data dalam jangka panjang (misal: suhu rata-rata bumi yang naik akibat pemanasan global).
- **Seasonality (Musiman):** Pola berulang dalam periode tertentu (misal: suhu tanah yang selalu naik di siang hari dan turun di malam hari).
- **Noise (Residu):** Fluktuasi acak pada sensor akibat gangguan teknis yang tidak berpola.

---

## TOPIK 4: Anatomi Masalah Data IoT
1. **Missing Packet:** Terjadi ketika koneksi internet putus sehingga *payload* data MQTT gagal diterima server. Data menjadi *NaN* (Not a Number) di dalam database.
2. **Noise Tinggi:** Fluktuasi tegangan pada sensor analog (seperti sensor kelembaban tanah kapasitif) yang menyebabkan grafik bergerigi. Solusinya adalah *Filtering* (seperti *Moving Average*).
3. **Keterbatasan Daya (Power Constraint):** Jika sensor menggunakan baterai, pengiriman data tidak bisa dilakukan setiap detik. Ini menghasilkan *sampling rate* yang rendah atau jeda waktu pengukuran yang tidak beraturan.

---

## TOPIK 5: Statistik Deskriptif dan Peluang
Sebelum menggunakan AI, seorang *Data Analyst* harus menguasai statistik:
- **Mean (Rata-rata):** Nilai tengah data. Rentan terhadap tarikan nilai *outlier*.
- **Median:** Nilai tengah yang diurutkan. Lebih kebal terhadap *outlier* sensor yang rusak.
- **Standar Deviasi / Z-Score:** Ukuran seberapa jauh sebuah nilai data menyimpang dari rata-rata normal. Sangat berguna untuk mendeteksi *outlier* batas keras (*hard threshold*).

---

## TOPIK 6: Data Storytelling & Hipotesis Pertanian Presisi
*Data Storytelling* adalah kemampuan menerjemahkan grafik teknis menjadi informasi bisnis atau operasional yang dipahami petani/manajer kebun.
- **Eksplorasi Hipotesis:** Melalui *Exploratory Data Analysis* (EDA), kita bisa menjawab pertanyaan: *"Apakah penurunan curah hujan pekan ini berkorelasi langsung dengan lonjakan suhu kanopi daun?"* Ini dibuktikan dengan *Correlation Matrix* (Heatmap).

---

## TOPIK 7: Masa Depan Komputasi: TinyML
*Tiny Machine Learning* (TinyML) adalah paradigma baru di mana model kecerdasan buatan dikompresi sedemikian rupa (menggunakan teknik *quantization*) agar bisa berjalan di dalam RAM sekecil puluhan Kilobyte (KB) pada mikrokontroler. Ini memungkinkan deteksi hama dari suara atau gambar langsung di lahan tanpa butuh internet.

---

## TOPIK 8: EVALUASI TENGAH SEMESTER (UTS)
*(Pekan Ujian)*

---

## TOPIK 9: Paradigma Machine Learning vs Pemrograman Tradisional
- **Pemrograman Tradisional:** `Aturan (Rule) + Data = Jawaban`. Contoh: `If Suhu > 30 THEN Kipas Nyala`.
- **Machine Learning:** `Data + Jawaban Historis = Aturan (Model)`. Mesin mencari sendiri pola kapan kipas seharusnya menyala berdasarkan jutaan log data masa lalu, tanpa harus di-hardcode oleh manusia.

---

## TOPIK 10: Pendekatan Regresi dan Peramalan (*Forecasting*)
*Forecasting* (Peramalan) bertujuan memprediksi nilai sensor di masa depan.
- **Metode Klasik:** ARIMA (*Auto-Regressive Integrated Moving Average*) atau *Exponential Smoothing*.
- **Metode Deep Learning:** LSTM (*Long Short-Term Memory*) jaringan saraf tiruan yang sangat andal mengingat pola musiman masa lalu untuk memprediksi kelembaban tanah 24 jam ke depan.
- **Evaluasi:** Menggunakan metrik RMSE (*Root Mean Square Error*), semakin kecil nilainya, semakin akurat peramalannya.

---

## TOPIK 11: Anomali Jaringan Sensor & Pembelajaran Mesin
Anomali (*Outlier*) pada IoT bisa berarti dua hal: (1) Perangkat kerasnya yang rusak, atau (2) Terjadi fenomena ekstrem yang langka (misal kebakaran hutan). 
Algoritma *Unsupervised Learning* seperti **Isolation Forest** sangat populer untuk IoT karena mampu mendeteksi pola anomali tanpa perlu dilatih dengan label data secara manual.

---

## TOPIK 12: Prinsip UI/UX dalam Desain Dashboard Industri
Sebuah *Dashboard* (seperti Grafana) yang baik harus mematuhi prinsip visibilitas operasional:
- **Jangan menumpuk grafik:** Gunakan *Single Stat* atau *Gauge* untuk informasi *real-time* utama (suhu saat ini).
- **Gunakan warna berkonteks:** Merah untuk bahaya/kritis, hijau untuk normal.
- **Hirarki Visual:** Letakkan peringatan di bagian paling atas *dashboard*.

---

## TOPIK 13: EWS (Early Warning System) dan Mitigasi
Analisis data tidak berguna jika tidak menghasilkan aksi (Actionable Insight).
*Dashboard* harus diintegrasikan dengan *Alerting System* (Sistem Peringatan Dini) menggunakan mekanisme *Webhook*. Misalnya, mengirimkan pesan *Telegram* atau *WhatsApp* ke ponsel manajer kebun saat *Machine Learning* memprediksi tanaman akan layu dalam 6 jam.

---

## TOPIK 14: Review Metodologi Proyek Enterprise
Dalam skala enterprise/industri, arsitektur IoT Analytics menggunakan prinsip *Microservices* dan skalabilitas:
1. **Perangkat Sensor (Edge):** Mengirim MQTT.
2. **Broker (Message Hub):** Apache Kafka atau HiveMQ.
3. **Database Time-Series:** InfluxDB atau TimescaleDB.
4. **Analitik Engine:** Skrip Python/Spark untuk pembersihan dan deteksi.
5. **Aplikasi Presentasi:** Grafana atau aplikasi web khusus. 
Mahasiswa merangkum seluruh prinsip ini melalui Proyek Akhir.
