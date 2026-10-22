---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Kuliah Pekan 3: Anatomi dan Karakteristik Data Time-Series pada Sensor IoT"
---

# Materi Kuliah Pekan 3: Anatomi dan Karakteristik Data Time-Series pada Sensor IoT

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 3 (Kuliah Teori — $2 \times 50$ Menit)  
**Capaian Pembelajaran (Sub-CPMK 3):** Mahasiswa mampu menganalisis anatomi dan dekomposisi data runtun waktu (*Time-Series*) yang meliputi *Trend, Seasonality, Cyclic*, dan *Noise*, memahami konsep frekuensi *sampling* dan agregasi waktu (*downsampling*), serta mendiagnosis pola anomali struktural pada sensor IoT.

---

## 1. Tujuan Pembelajaran Khusus
Setelah mengikuti perkuliahan pekan ini, mahasiswa diharapkan mampu:
1. Membedakan karakteristik matematis antara **Data Runtun Waktu (*Time-Series Data*)** dan **Data Potong Lintang (*Cross-Sectional Data*)**, serta menjelaskan mengapa urutan indeks waktu tidak boleh diacak (*shuffled*).
2. Menguraikan empat komponen dekomposisi deret waktu (**Trend, Seasonality, Cyclic, dan Noise/Residual**) baik menggunakan model **Aditif** maupun **Multiplikatif**.
3. Mengidentifikasi fenomena **Musiman Ganda (*Multi-Seasonality*)** pada sistem IoT pertanian dan peternakan modern.
4. Menganalisis hubungan antara **Frekuensi Sampling ($f_s$)**, **Downsampling (Agregasi Waktu)**, dan risiko hilangnya informasi ekstrem (*peak event loss*) jika hanya mengandalkan metrik rata-rata tunggal.
5. Mendiagnosis perbedaan visual dan matematis antara **Sensor Drift**, **High-Frequency Noise**, **Missing Packet**, dan **Stuck-at Fault (Zero-Variance)**, serta menerapkan standar sinkronisasi waktu lintas wilayah (**UTC & NTP**).

---

## 2. Pendahuluan & Konteks Industri: Mengapa Waktu Adalah Dimensi Utama IoT?

Dalam ilmu data tradisional, kita sering mengolah **Data Cross-Sectional (Potong Lintang)**, misalnya tabel daftar nilai IPK 500 mahasiswa atau tabel harga rumah di suatu kota. Pada tabel *cross-sectional*, urutan baris ke-1 hingga ke-500 dapat diacak (*shuffled*) tanpa mengubah makna statistiknya sama sekali.

Sebaliknya, hampir $100\%$ data telemetri IoT adalah **Data Runtun Waktu (*Time-Series Data*)**, yaitu himpunan pengamatan berurutan yang diindeks berdasarkan cap waktu kronologis ($t_1 < t_2 < \dots < t_n$):
$$Y = \{y(t_1), y(t_2), y(t_3), \dots, y(t_n)\}$$

Dalam data *time-series*, **urutan waktu menyimpan informasi kausalitas dan dinamika fisika**:
- Angka suhu `30°C -> 35°C -> 45°C -> 60°C` memiliki makna yang sangat berbeda dengan `60°C -> 45°C -> 35°C -> 30°C`, meskipun rata-rata (*Mean*) dan standar deviasinya sama persis! Deret pertama menandakan bahaya kebakaran (*thermal runaway* yang sedang membesar), sedangkan deret kedua menandakan proses pendinginan mesin yang aman.
- Oleh karena itu, dalam analisis data IoT, **mengacak urutan baris waktu adalah kesalahan fatal** yang menghancurkan informasi tren, kecepatan perubahan ($\frac{dy}{dt}$), dan autokorelasi sinyal.

---

## 3. Materi Inti: Dekomposisi Komponen Data Time-Series

Sebuah grafik sinyal sensor IoT yang tampak rumit sebenarnya merupakan superposisi (gabungan) dari empat komponen dasar. Proses membedah sinyal mentah menjadi komponen-komponen penyusunnya disebut **Time-Series Decomposition**:

```text
1. SINYAL ASLI SENSOR (Y_t) — Gabungan seluruh komponen:
   Nilai ^        /\          /\          /\
         |   /\  /  \    /\  /  \    /\  /  \
         |  /  \/    \  /  \/    \  /  \/    \
         +-----------------------------------------> Waktu (t)

2. KOMPONEN TREND (T_t) — Kecenderungan jangka panjang:
   Nilai ^                                  /
         |                        /--------/
         |              /--------/
         |    /--------/
         +-----------------------------------------> Waktu (t)

3. KOMPONEN SEASONALITY (S_t) — Siklus berulang periodik tetap (misal: 24 jam Siang-Malam):
   Nilai ^     /\        /\        /\        /\
         |    /  \      /  \      /  \      /  \
         |   /    \    /    \    /    \    /    \
         |  /      \  /      \  /      \  /      \
         +-----------------------------------------> Waktu (t)

4. KOMPONEN NOISE / RESIDUAL (e_t) — Fluktuasi acak frekuensi tinggi:
   Nilai ^  .  , .   .  ,   . ,  .   , .  .   ,
         |--|--|-|--|--|---|--|--|--|-|--|--|------> Waktu (t)
         |   '    '   '  '     '  '    '   '
```

### 3.1 Empat Komponen Penyusun Time-Series
1. **Trend ($T_t$ — Kecenderungan Jangka Panjang):**
   Pergerakan arah data secara perlahan dalam jangka panjang, baik cenderung naik (*uptrend*), turun (*downtrend*), maupun mendatar (*stationary*).
   - *Contoh di IoT Pertanian:* Penurunan bertahap cadangan air tanah di sumur irigasi selama 3 bulan musim kemarau, atau kenaikan bertahap suhu rata-rata bantalan (*bearing*) motor pompa air akibat keausan mekanis.
2. **Seasonality ($S_t$ — Pola Musiman Periodik Tetap):**
   Pola gelombang yang berulang secara teratur dengan **periode waktu yang tetap dan diketahui pasti** (misal setiap 24 jam / *diurnal cycle*, setiap 7 hari, atau setiap 12 bulan).
   - *Contoh di IoT Pertanian:* Grafik intensitas cahaya matahari (*lux*) dan suhu udara rumah kaca yang selalu mulai naik pukul 06.00 pagi, mencapai puncak parabolik pukul 12.00–13.00 siang, lalu turun ke titik terendah pada malam hari secara konsisten setiap 24 jam.
3. **Cyclic ($C_t$ — Siklus Jangka Panjang Tak Tetap):**
   Fluktuasi naik-turun menyerupai gelombang namun **periodenya tidak tetap** dan durasinya biasanya jauh lebih panjang (например fenomena iklim *El Niño* dan *La Niña* yang berulang setiap 2 hingga 7 tahun secara tidak menentu).
4. **Noise / Residual ($\epsilon_t$ — Derau Acak / Residu):**
   Komponen sisa yang bersifat acak (*random fluctuation*) setelah *Trend* dan *Seasonality* dikeluarkan dari sinyal asli.
   - *Contoh di IoT Pertanian:* Gerigi halus naik-turun sebesar $\pm 0,3^\circ\text{C}$ setiap detik akibat derau termal elektronika ADC atau bayangan dedaunan yang bergoyang tertiup angin di atas sensor cahaya.

---

### 3.2 Model Dekomposisi: Aditif vs Multiplikatif
Secara matematis, penggabungan komponen di atas dibagi menjadi dua model utama (McKinney, 2022):

1. **Model Aditif (*Additive Decomposition*):**
   $$Y_t = T_t + S_t + \epsilon_t$$
   - Digunakan apabila **lebar ayunan (amplitudo) musiman tetap konstan** dari waktu ke waktu, tidak peduli apakah nilai *Trend* sedang rendah maupun tinggi.
   - *Contoh:* Suhu udara di daerah tropis yang fluktuasi siang-malamnya selalu sekitar $\pm 5^\circ\text{C}$ sepanjang tahun.
2. **Model Multiplikatif (*Multiplicative Decomposition*):**
   $$Y_t = T_t \times S_t \times \epsilon_t$$
   - Digunakan apabila **amplitudo fluktuasi musiman ikut membesar seiring naiknya nilai *Trend*** (membentuk pola corong melebar).
   - *Contoh:* Dataset klasik *Air Passengers* (jumlah penumpang pesawat meningkat setiap tahun, dan lonjakan puncak musim liburan ikut membesar secara proporsional), atau konsumsi air irigasi tanaman yang fluktuasi harian siang-malamnya makin besar seiring bertambahnya ukuran kanopi daun tanaman dari minggu ke minggu.

---

### 3.3 Fenomena Multi-Seasonality (Musiman Ganda) pada IoT Pertanian
Di dunia nyata, sebuah sensor IoT sering kali merekam **lebih dari satu siklus musiman sekaligus (*Multi-Seasonal Time-Series*)**:
$$Y_t = T_t + S_{\text{harian}, t} + S_{\text{produksi}, t} + \epsilon_t$$

**Contoh Nyata — Konsumsi Daya Listrik Blower Kandang Ayam *Closed-House*:**
1. **Siklus Musiman 1 (Periode Diurnal 24 Jam):** Setiap siang hari (pukul 11.00–15.00), suhu luar kandang panas sehingga kipas *blower* bekerja maksimal (konsumsi listrik naik setiap siang dan turun setiap malam).
2. **Siklus Musiman 2 (Periode Produksi Broiler 35 Hari):** Ayam pedaging (*broiler*) dipelihara dari umur 1 hari (DOC — *Day Old Chick*) hingga panen pada hari ke-35. Saat masih DOC, ayam membutuhkan sedikit ventilasi; semakin mendekati hari ke-35, bobot dan panas tubuh ribuan ayam meningkat drastis sehingga *blower* menyala semakin kencang setiap minggunya, lalu kembali ke titik terendah saat kandang dikosongkan usai panen.

---

## 4. Frekuensi Sampling, Agregasi Waktu (*Downsampling*), dan Integritas Timestamp

### 4.1 Frekuensi Sampling ($f_s$) dan Masalah *Irregular Sampling*
Frekuensi pengambilan sampel didefinisikan sebagai jumlah pengukuran per satuan waktu:
$$f_s = \frac{1}{\Delta t}$$
- Jika sensor mencatat data setiap $\Delta t = 0,1\text{ detik}$, maka $f_s = 10\text{ Hz}$.
- **Irregular Sampling Rate (Jarak Waktu Tidak Seragam):** Pada perangkat IoT bertenaga surya, mikrokontroler sering menerapkan *Adaptive Sampling*—misalnya mengirim data setiap **15 detik** saat hujan lebat dan air sungai naik cepat ($\frac{dh}{dt} > 5\text{ cm/menit}$), namun melambat menjadi setiap **10 menit** saat air sungai tenang demi menghemat baterai.
- Sebelum dianalisis oleh algoritma *Machine Learning* yang menuntut interval waktu seragam, data *irregular* ini wajib diseragamkan menggunakan operasi **Resampling** di Pandas (`.resample('5min')`).

### 4.2 Strategi Downsampling (Agregasi Waktu) di Edge/Fog Gateway
Ketika kita meringkas data berfrekuensi tinggi (misal sensor kecepatan angin $10\text{ Hz}$ / 10 data per detik) menjadi laporan per **15 menit** ($9.000$ titik data diringkas menjadi 1 baris) untuk menghemat kuota internet:
- **Bahaya Mengirim Hanya Nilai Rata-Rata (*Mean*):**
  Angin puting beliung yang menerjang kebun jagung sering berupa hembusan kejut sesaat (*peak wind gust*, misal mencapai $85\text{ km/jam}$ selama 5 detik), sementara sisa 14 menit 55 detik lainnya angin bertiup pelan ($10\text{ km/jam}$). Jika gateway hanya mengirimkan nilai *Mean* ($\approx 10,4\text{ km/jam}$), server pusat akan mengira cuaca sangat tenang, padahal tanaman jagung di lapangan sudah roboh!
- **Solusi Terbaik (*Multivariate Summary Aggregation*):**
  Saat melakukan *downsampling* di gateway, kirimkan paket statistik ringkasan yang mencakup 4 metrik sekaligus untuk setiap jendela 15 menit:
  1. **`mean`** (Rata-rata kondisi umum)
  2. **`max`** (*Peak Gust* / Nilai puncak ekstrem untuk deteksi bahaya)
  3. **`min`** (Nilai minimum terendah)
  4. **`std`** (Standar deviasi untuk mengukur gejolak/fluktuasi selama jendela waktu tersebut)

### 4.3 Integritas Timestamp Lintas Zona Waktu (UTC & NTP)
Bayangkan perusahaan agribisnis memiliki kebun di **Sumatera Barat (WIB / UTC+7)**, **Kalimantan Timur (WITA / UTC+8)**, dan **Papua (WIT / UTC+9)** yang seluruhnya mengirim data ke satu server database di Jakarta:
1. **Wajib Menggunakan Standar UTC atau UNIX Epoch:** Setiap *gateway* wajib mencatat dan mengirimkan *timestamp* dalam format **UTC (*Coordinated Universal Time*, misal `2026-10-22T05:00:00Z`)** atau **UNIX Epoch Timestamp** (jumlah detik sejak 1 Januari 1970 UTC). Konversi ke zona waktu lokal (WIB/WITA/WIT) hanya dilakukan di lapisan *Dashboard* (Grafana) saat ditampilkan ke mata manusia.
2. **Sinkronisasi NTP (*Network Time Protocol*):** Kristal kuarsa pencatat waktu internal pada mikrokontroler/SBC murah mengalami pergeseran waktu (*clock drift*) beberapa detik setiap minggunya. Setiap *gateway* wajib menjalankan layanan **NTP Client** agar jam perangkat selalu tersinkronisasi presisi dengan server waktu atom internet.
3. **Gunakan *Event Time* (Waktu Akuisisi Sensor), Bukan *Ingestion Time* (Waktu Tiba di Server):** Jika koneksi internet kebun putus selama 30 menit dan ratusan paket data baru terkirim sekaligus saat sinyal pulih, *timestamp* yang disimpan di database haruslah waktu saat sensor mengukur suhu tersebut di kebun, bukan waktu saat paket tiba di server.

---

## 5. Anatomi Kerusakan Sensor pada Grafik Time-Series

Seorang *Data Analyst* IoT harus terlatih mengenali 4 pola patologis (penyakit data) dari grafik *time-series*:

```text
A. HARD OUTLIER (Spike Sesaat)             B. SENSOR DRIFT (Pergeseran Gradual)
   ^          |                               ^                      /------- (Bacaan Sensor)
   |          |  <-- Korsleting/Induksi       |              /------/
   |   /\    / \    /\                        |      /------/
   |  /  \  /   \  /  \                       |-----/------------------------ (Nilai Asli Sebenarnya)
   +---------------------> t                  +-----------------------------> t

C. MISSING VALUES (Lubang Data / NaN)      D. STUCK-AT FAULT (Zero-Variance / Flatline)
   ^                                          ^
   |   /\              /\                     |   /\    /\
   |  /  \            /  \                    |  /  \  /  \------------------ <-- Sensor Macet (Variansi = 0)
   | /    \  [Kosong]     \                   | /    \/
   +---------------------> t                  +-----------------------------> t
```

| Jenis Anomali | Ciri Matematis pada Data | Penyebab Fisik di Lapangan | Penanganan Analitik |
| :--- | :--- | :--- | :--- |
| **1. Hard Outlier (*Point Spike*)** | Nilai melonjak keluar batas $\pm 3\sigma$ atau pagar IQR hanya pada 1–2 titik waktu lalu langsung kembali normal. | Induksi elektromagnetik motor/relay, lonjakan petir, *bit-flip* komunikasi. | Filter *Thresholding* fisik, *Median Filter*, atau *IQR Outlier Removal* lalu interpolasi. |
| **2. Sensor Drift** | Rata-rata bergerak (*rolling mean*) bergeser naik/turun secara monoton ($\Delta \mu / \Delta t \neq 0$) tanpa kembali ke garis dasar. | Penumpukan kerak mineral pada elektroda pH, lumut (*bio-fouling*), penuaan elemen kimia sensor. | Peringatan jadwal **Rekalibrasi Fisik**; koreksi *offset* matematis terhadap sensor referensi. |
| **3. Missing Values (`NaN`)** | Hilangnya baris *timestamp* atau nilai kolom berisi `null` / `NaN` selama rentang $\Delta t$ tertentu. | Mati listrik lokal, paket radio LoRa/WiFi hilang (*packet loss*), baterai habis di malam hari. | Imputasi (*Forward-Fill* untuk jeda pendek; *Linear/Polynomial/Seasonal Interpolation* untuk jeda menengah). |
| **4. Stuck-at Fault (*Flatline*)** | Nilai standar deviasi bergerak dalam jendela waktu bernilai nol mutlak ($\sigma_k = 0$), padahal nilai berada di rentang normal. | *Hang* pada chip sensor digital (I2C/OneWire *deadlock*), kabel ADC putus mengambang di satu tegangan. | Deteksi **Zero-Variance ($\sigma^2 == 0$)** selama $N$ menit berturut-turut $\rightarrow$ kirim perintah *hardware reset* sensor. |

---

## 6. Bahan Diskusi & Berpikir Kritis (*HOTS*) di Kelas

1. **Diagnosa Sensor Macet vs Kondisi Stabil:**
   Pada kolam budidaya ikan nila, sensor oksigen terlarut (*Dissolved Oxygen* / DO) mengirimkan angka `5.250000 mg/L` tanpa berubah satu digit desimal pun selama 12 jam berturut-turut. Mengapa filter pendeteksi anomali biasa (seperti batas aman DO $3,0 - 8,0\text{ mg/L}$) **gagal** mendeteksi masalah ini, dan rumus statistik sederhana apa yang harus Anda pasang di skrip Python untuk menangkap kerusakan ini?
2. **Strategi Imputasi Jangka Pendek vs Jangka Panjang:**
   Jika data suhu rumah kaca mengalami kekosongan (*missing values*) selama **2 menit** dibandingkan kosong selama **3 hari penuh**, apakah teknik penambalan data (*imputation*) yang digunakan boleh sama? Jelaskan apa yang terjadi pada kurva siklus siang-malam jika kekosongan 3 hari ditambal menggunakan *Linear Interpolation* atau *Forward-Fill*!
3. **Dekomposisi Multi-Seasonality pada Smart Retail / Pertanian:**
   Sebutkan contoh komponen *Trend*, *Seasonality* Harian, *Seasonality* Mingguan/Bulanan, dan *Noise* pada dataset **"Debit Pemakaian Air Irigasi Perkebunan"** selama 1 tahun penuh!

---

## 7. Referensi & Acuan Materi Pekan Ini

1. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730) — **Bab 4 (Exploring IoT Data: Exploring and Visualizing Time-Series Data, Seasonality, and Anomalies).**  
2. **McKinney, W. (2022).** *Python for Data Analysis: Data Wrangling with Pandas, NumPy, and Jupyter* (3rd ed.). O'Reilly Media. (ISBN-13: 978-1098104030) — **Bab 11 (Time Series: Date and Time Data Types, Resampling, and Frequency Conversion).**  
3. **Fawcett, T., & Provost, F. (2013).** *Data Science for Business*. O'Reilly Media. (ISBN-13: 978-1449361327) — **Bab 3 (Introduction to Predictive Modeling & Data Attributes).**  
4. **Pandas Development Team. (2024).** *Pandas Documentation: Time Series / Date Functionality*. Tersedia: [https://pandas.pydata.org/docs/user_guide/timeseries.html](https://pandas.pydata.org/docs/user_guide/timeseries.html)  
5. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
