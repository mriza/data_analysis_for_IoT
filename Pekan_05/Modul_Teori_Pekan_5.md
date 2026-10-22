---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Kuliah Pekan 5: Statistik Deskriptif, Klasifikasi Missing Values, dan Pra-pemrosesan Data Sensor (Imputation & Smoothing)"
---

# Materi Kuliah Pekan 5: Statistik Deskriptif, Klasifikasi Missing Values, dan Pra-pemrosesan Data Sensor (Imputation & Smoothing)

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 5 (Kuliah Teori — $2 \times 50$ Menit)  
**Capaian Pembelajaran (Sub-CPMK 5):** Mahasiswa mampu menganalisis ketahanan metrik statistik terhadap *outlier*, mengklasifikasikan mekanisme hilangnya data sensor (MCAR, MAR, MNAR), menerapkan metode rekonstruksi/imputasi deret waktu, serta merancang filter penghalusan sinyal (*SMA, EMA, Median Filter*) dengan mempertimbangkan *trade-off* antara kehalusan sinyal dan keterlambatan fase (*phase lag*).

---

## 1. Tujuan Pembelajaran Khusus
Setelah mengikuti perkuliahan pekan ini, mahasiswa diharapkan mampu:
1. Membuktikan secara matematis mengapa nilai **Mean** dan **Standar Deviasi** sangat rentan terhadap *outlier* sensor dibandingkan **Median** dan **IQR**.
2. Mengklasifikasikan penyebab hilangnya data sensor IoT ke dalam tiga kategori teoritis: **MCAR (*Missing Completely at Random*)**, **MAR (*Missing at Random*)**, dan **MNAR (*Missing Not at Random*)**.
3. Menghitung secara manual dan mengevaluasi akurasi berbagai teknik **Imputasi Data Time-Series** (*Forward-Fill*, *Backward-Fill*, *Linear Interpolation*, *Polynomial/Spline*, dan *Seasonal Reconstruction*).
4. Menganalisis bahaya kegagalan berantai (*Cascade Control Failure*) akibat penerapan imputasi pasif tanpa mekanisme *Fail-Safe State*.
5. Menghitung dan membandingkan karakteristik **Simple Moving Average (SMA)**, **Exponential Moving Average (EMA)**, dan **Median Filter**, serta menganalisis *trade-off* ukuran jendela (*window size* $k$) terhadap **Phase Lag**.

---

## 2. Pendahuluan & Konteks Industri: Hukum "Garbage In, Garbage Out" (GIGO)

Aksioma paling mendasar dalam rekayasa data dan kecerdasan buatan adalah **"Garbage In, Garbage Out" (GIGO)**: sehebat apa pun algoritma *Machine Learning* atau *Deep Learning* yang Anda gunakan, apabila data sensor yang disuapkan ke dalamnya penuh dengan lonjakan listrik palsu, angka kosong (`NaN`), atau bias penambalan data yang keliru, maka keputusan yang dihasilkan sistem pasti salah dan berpotensi merusak peralatan lapangan.

Dalam proyek analitik IoT industri, tahap **Data Pre-processing (Pra-pemrosesan Data)**—yang mencakup *Data Cleaning*, *Missing Value Imputation*, *Noise Filtering/Smoothing*, dan *Resampling*—umumnya menghabiskan **70% hingga 80% dari total waktu kerja seorang *Data Engineer***. Sensor di perkebunan dan peternakan bekerja di lingkungan terbuka yang ekstrem, sehingga kita tidak pernah boleh mengasumsikan data yang masuk ke server sudah bersih.

---

## 3. Materi Inti

### 3.1 Ketahanan Statistik Deskriptif terhadap Outlier Sensor (Mean vs Median)
Ketika sebuah sensor mengalami gangguan induksi elektromagnetik sesaat (*impulse spike*), metrik pemusatan data yang berbeda akan bereaksi secara drastis:

- **Mean (Rata-rata Aritmatika):**
  $$\mu = \frac{1}{N} \sum_{i=1}^{N} x_i$$
  Memiliki *breakdown point* sebesar $0\%$—artinya **cukup satu titik data yang rusak ekstrem** sudah mampu menghancurkan nilai rata-rata seluruh sampel.
- **Median (Nilai Tengah):**
  Memiliki *breakdown point* hingga $50\%$—artinya selama kurang dari separuh data yang rusak, nilai Median tetap kokoh (*robust*) mewakili kondisi normal.

#### Contoh Perhitungan Numerik Ketahanan Outlier
Sebuah sensor suhu ruang server mencatat 7 pembacaan berturut-turut setiap 1 detik (°C). Pada detik ke-5, relay kompresor AC menyala dan menginduksi lonjakan tegangan palsu sebesar $95,0^\circ\text{C}$:
$$\text{Data Mentah } X = [24,1,\ 24,3,\ 24,0,\ 24,2,\ \mathbf{95,0},\ 24,2,\ 24,1]$$

1. **Perhitungan Mean ($\mu$):**
   $$\mu = \frac{24,1 + 24,3 + 24,0 + 24,2 + 95,0 + 24,2 + 24,1}{7} = \frac{239,9}{7} \approx \mathbf{34,27^\circ\text{C}}$$
   *Dampak:* Nilai Mean melonjak lebih dari $+10^\circ\text{C}$ di atas suhu riil! Jika sistem kendali pendingin menggunakan Mean, alarm kebakaran palsu akan berbunyi.
2. **Perhitungan Median ($Q_2$):**
   Urutkan data dari terkecil ke terbesar:
   $$X_{\text{urut}} = [24,0,\ 24,1,\ 24,1,\ \mathbf{24,2},\ 24,2,\ 24,3,\ 95,0]$$
   Nilai tengah (indeks ke-4 dari 7 data) adalah **$\text{Median} = 24,2^\circ\text{C}$**.
   *Dampak:* Median sama sekali tidak terpengaruh oleh angka $95,0^\circ\text{C}$ dan tetap merepresentasikan suhu ruangan secara akurat!

> **Catatan Kritis Engineer:** Kapan *Mean* justru dibutuhkan dibanding *Median*? Apabila lonjakan tersebut **bukanlah derau listrik**, melainkan kejadian fisik nyata yang singkat namun mematikan (misalnya tumpahan limbah asam pekat $\text{pH } 1,5$ selama 10 menit di sungai pada siang hari), penggunaan *Median* harian justru akan "menyembunyikan" bencana tersebut, sedangkan *Mean* (dan nilai *Min/Max*) akan bereaksi memberi sinyal perubahan!

---

### 3.2 Taksonomi Mekanisme Hilangnya Data Sensor: MCAR, MAR, dan MNAR
Sebelum menambal data yang kosong (`NaN`), seorang analis wajib mendiagnosis **mengapa data tersebut hilang**. Dalam teori statistik (Rubin, 1976; McKinney, 2022), terdapat tiga mekanisme *missing data*:

| Klasifikasi | Definisi Statistik | Contoh Nyata pada Perangkat Keras IoT | Implikasi Penanganan Analitik |
| :--- | :--- | :--- | :--- |
| **1. MCAR (*Missing Completely at Random*)** | Probabilitas hilangnya data murni acak dan **tidak bergantung** pada nilai variabel itu sendiri maupun variabel lain. | Paket radio WiFi/LoRa sesekali bertabrakan di udara; kabel sensor tersenggol burung secara acak. | Aman ditambal menggunakan interpolasi standar (*Linear/Spline*) tanpa menimbulkan bias sistematis. |
| **2. MAR (*Missing at Random*)** | Probabilitas hilangnya data **bergantung pada variabel lain yang ikut tercatat**, tetapi bukan pada nilai variabel yang hilang itu sendiri. | Sensor kelembaban tanah gagal mengirim data setiap kali **tegangan baterai panel surya** (yang juga tercatat di log) turun di bawah $3,1\text{ V}$ pada dini hari; atau sensor optik terganggu saat **suhu casing** $> 50^\circ\text{C}$. | Dapat diestimasi secara akurat menggunakan korelasi multivariat dengan variabel penyebabnya. |
| **3. MNAR (*Missing Not at Random*)** | Probabilitas hilangnya data **berkaitan langsung dengan nilai ekstrem dari variabel yang sedang diukur itu sendiri**. | 1. Sensor gas amonia ($NH_3$) kandang ayam memiliki batas ukur ADC $0-100\text{ ppm}$; saat ventilasi mati dan amonia menembus $140\text{ ppm}$, ADC mengalami *overflow* dan mengirim `NaN`.<br>2. Anemometer macet mekanis tepat ketika angin badai melebihi $100\text{ km/jam}$. | **SANGAT BERBAHAYA jika ditambal dengan Mean/ffill!** Mengisi `NaN` saat kondisi puncak dengan rata-rata normal akan menyebabkan *underestimation* parah dan menutupi kondisi bahaya. |

---

### 3.3 Teknik Rekonstruksi & Imputasi Data Time-Series

Pada data *time-series*, menghapus baris yang bernilai `NaN` (`df.dropna()`) umumnya **dilarang** karena akan merusak keseragaman jarak waktu ($\Delta t$) yang dibutuhkan oleh filter frekuensi dan model peramalan (*Forecasting*). Sebagai gantinya, kita menggunakan teknik **Imputasi (*Imputation*)**:

```text
Skenario Data Suhu Hilang 3 Menit:
10:00 (28.0°C) ---> 10:01 (NaN) ---> 10:02 (NaN) ---> 10:03 (NaN) ---> 10:04 (28.8°C)

1. FORWARD-FILL (ffill / Last Observation Carried Forward):
   Menahan nilai terakhir yang diketahui (28.0°C) secara mendatar hingga data baru muncul.
   Hasil: [28.0,  28.0,  28.0,  28.0,  28.8]

2. BACKWARD-FILL (bfill / Next Observation Carried Backward):
   Menarik mundur nilai masa depan (28.8°C) ke sel kosong sebelumnya.
   Hasil: [28.0,  28.8,  28.8,  28.8,  28.8]

3. LINEAR INTERPOLATION (Garis Lurus Proporsional):
   Menarik garis miring lurus dengan kenaikan tetap (\Delta y = (28.8 - 28.0) / 4 = +0.2°C/menit).
   Hasil: [28.0,  28.2,  28.4,  28.6,  28.8]
```

#### Rumus Matematis Linear Interpolation
Untuk menaksir nilai $y_t$ pada waktu $t$ yang berada di antara dua titik valid $(t_0, y_0)$ dan $(t_1, y_1)$:
$$y_t = y_0 + (t - t_0) \cdot \frac{y_1 - y_0}{t_1 - t_0}$$

#### Kapan Imputasi Standar Gagal? (Kasus Kekosongan Panjang melintasi Siklus Diurnal)
Bayangkan sensor suhu udara di kebun teh mati selama **6 jam penuh dari pukul 11.00 siang hingga pukul 17.00 sore** (periode krusial di mana suhu seharusnya naik mencapai puncak di pukul 13.00 siang lalu turun di sore hari):
- **Kelemahan *Linear Interpolation*:** Akan menarik garis lurus mendatar/menurun dari suhu pukul 11.00 langsung ke suhu sejuk pukul 17.00, sehingga **memotong habis (*clipping*) bukit puncak suhu siang hari**!
- **Kelemahan *Forward-Fill (`ffill`)*:** Akan mempertahankan suhu panas pukul 11.00 secara konstan hingga pukul 17.00 sore, menyebabkan *overestimation* suhu sore hari.
- **Solusi Ilmiah Terbaik untuk Kekosongan Panjang:**
  1. **Polynomial / Cubic Spline Interpolation (`order=2` atau `3`):** Untuk jeda menengah (1–2 jam) agar kurva mengikuti kelengkungan turunan sinyal.
  2. **Seasonal Pattern Reconstruction + Cross-Station Correlation:** Untuk jeda berhari-hari, tambal menggunakan rata-rata profil siklus diurnal (jam yang sama pada hari-hari sebelumnya) yang dikalibrasi dengan stasiun cuaca tetangga terdekat.

#### Bahaya Imputasi Pasif Tanpa *Fail-Safe*: Kasus Kematian Massal Ikan Akuaponik (*Cascade Anomaly*)
> **Pelajaran Rekayasa Kritis:**
> Pada pukul 02.00 dini hari, kabel sensor oksigen terlarut (*Dissolved Oxygen*) di kolam ikan putus digigit tikus sehingga bernilai `NaN`. Skrip pra-pemrosesan yang dirancang secara naif langsung menambal `NaN` tersebut menggunakan **Global Mean Imputation** (rata-rata oksigen siang hari = $7,8\text{ mg/L}$).
> Akibatnya, mikrokontroler pengendali aerator membaca angka $7,8\text{ mg/L}$, menyimpulkan air kolam sudah sangat kaya oksigen, lalu **mematikan pompa aerator**! Padahal pada pukul 02.00 malam tanpa fotosintesis, kadar oksigen riil anjlok ke $1,8\text{ mg/L}$ dan menyebabkan seluruh ikan mati lemas.
> **Aturan Desain Wajib:** Jangan pernah menyuapkan angka hasil tebakan imputasi langsung ke logika aktuator keselamatan tanpa menyertakan bendera kualitas data (`data_quality_flag = IMPUTED/FAULT`) dan mode darurat (**Fail-Safe / Fail-Operational**, misal: *"Jika sensor DO bernilai NaN lebih dari 3 menit, nyalakan pompa aerator 100% dan bunyikan alarm!"*).

---

### 3.4 Penghalusan Sinyal (*Smoothing*): SMA vs EMA vs Median Filter

Setelah data bersih dari nilai kosong, sinyal sensor analog umumnya masih memiliki gerigi tajam frekuensi tinggi (*electrical/thermal noise*). Kita menghaluskannya menggunakan operasi *Rolling Window*:

#### A. Simple Moving Average (SMA — Rata-Rata Bergerak Sederhana)
Menghitung rata-rata aritmatika tanpa bobot dari $k$ titik data terakhir (*trailing window*):
$$SMA_t = \frac{1}{k} \sum_{i=0}^{k-1} x_{t-i} = \frac{x_t + x_{t-1} + \dots + x_{t-k+1}}{k}$$

**Contoh Perhitungan Manual SMA ($k = 3$):**
Diberikan barisan pembacaan sensor suhu inkubator telur dari $t=1$ hingga $t=5$:
$$T = [37,0,\ 38,0,\ 39,0,\ 42,0,\ 36,0]$$
Jika diterapkan $SMA$ dengan jendela $k = 3$:
- $t = 1 \implies \text{NaN}$ (belum cukup 3 data)
- $t = 2 \implies \text{NaN}$ (belum cukup 3 data)
- $t = 3 \implies SMA_3 = \frac{37,0 + 38,0 + 39,0}{3} = \mathbf{38,00^\circ\text{C}}$
- $t = 4 \implies SMA_4 = \frac{38,0 + 39,0 + 42,0}{3} = \frac{119,0}{3} \approx \mathbf{39,67^\circ\text{C}}$
- $t = 5 \implies SMA_5 = \frac{39,0 + 42,0 + 36,0}{3} = \frac{117,0}{3} = \mathbf{39,00^\circ\text{C}}$

#### B. Exponential Moving Average (EMA — Rata-Rata Bergerak Eksponensial)
Kelemahan utama SMA adalah memberikan bobot yang sama rata ($\frac{1}{k}$) kepada data yang baru saja terjadi maupun data yang sudah usang $k$ langkah yang lalu. Hal ini menyebabkan keterlambatan respons (**Phase Lag**) yang besar.
**EMA** mengatasi hal ini dengan memberikan bobot eksponensial yang jauh lebih besar pada pengamatan terbaru ($x_t$):
$$EMA_t = \alpha \cdot x_t + (1 - \alpha) \cdot EMA_{t-1}$$
di mana faktor penghalusan $\alpha \in (0, 1)$, yang pada Pandas (`ewm(span=k)`) didefinisikan sebagai $\alpha = \frac{2}{k + 1}$.

**Keunggulan Strategis EMA di IoT:**
1. **Respons Lebih Cepat (*Lower Phase Lag*):** Sangat ideal untuk mendeteksi kenaikan panas mendadak (*thermal runaway*) pada motor industri karena bereaksi cepat terhadap lonjakan baru.
2. **Sangat Hemat Memori di Mikrokontroler Edge ($O(1)$ Memory):** Berbeda dengan SMA yang mengharuskan mikrokontroler menyimpan $k$ angka terakhir di dalam array, rumus EMA **hanya membutuhkan penyimpanan 1 variabel float (`EMA_sebelumnya`)**!

#### C. Median Filter (*Rolling Median*)
Mengambil nilai tengah (*Median*) dari $k$ titik data terakhir:
$$Y_t = \text{median}(x_t, x_{t-1}, \dots, x_{t-k+1})$$
- **Keunggulan Mutlak:** Jika terdapat 1 lonjakan *outlier* tajam akibat korsleting sesaat (misal `[28, 28, 100, 28, 28]`), *SMA* akan menyeret rata-rata selama 5 langkah waktu menjadi `42.4°C` (*smearing effect*), sedangkan **Median Filter langsung melenyapkan angka `100` tersebut menjadi tepat `28°C` tanpa meninggalkan jejak sedikit pun!**

#### D. Trade-Off Ukuran Jendela (*Window Size* $k$): Kehalusan vs Phase Lag
```text
Sinyal Asli vs Hasil Smoothing Window Kecil (k=3) vs Window Terlalu Besar (k=50):

Nilai ^         /\  <-- Puncak Asli (t = 12:00)
      |        /  \        _.-._  <-- SMA k=50: Puncak menyusut (Amplitude Attenuation)
      |       /    \     .'     '.    DAN bergeser terlambat ke kanan (PHASE LAG = \Delta t)!
      |      /      \   /         \
      |  ---/        \-/           \---
      +-----------------------------------------> Waktu (t)
```

| Ukuran Jendela ($k$) | Karakteristik Kurva | Risiko / Konsekuensi Teknis |
| :--- | :--- | :--- |
| **Terlalu Kecil ($k = 2 \text{ atau } 3$)** | *Under-smoothing*: Latensi waktu hampir nol, namun sinyal masih kasar/bergerigi. | Derau frekuensi tinggi (*noise*) masih lolos dan dapat memicu **Alarm Palsu (*False Positive*)**. |
| **Optimal ($k = 5 \text{ s/d } 12$)** | Derau acak teredam dengan baik sementara bentuk gelombang utama dan tinggi puncak tetap terjaga. | Keseimbangan terbaik antara rasio sinyal-terhadap-derau (*SNR*) dan kecepatan respons. |
| **Terlalu Besar ($k = 50 \text{ atau } 100$)** | *Over-smoothing*: Garis sangat mulus, tetapi puncak gelombang terpangkas pendek (*amplitude reduction*). | Terjadi **Keterlambatan Fase (*Phase Lag*) yang parah**; alarm darurat baru berbunyi jauh setelah kejadian bahaya lewat (**False Negative / Delayed Response**). |

---

## 4. Bahan Diskusi & Berpikir Kritis (*HOTS*) di Kelas

1. **Klasifikasi Missing Value pada Sensor Angin Badai:**
   Sebuah anemometer di perbukitan berhenti mengirimkan data (*missing values*) hanya setiap kali kecepatan angin badai melebihi $100\text{ km/jam}$ karena baling-balingnya dikunci oleh rem pengaman mekanis. Termasuk tipe *missing value* apakah ini (MCAR, MAR, atau MNAR)? Apa dampaknya terhadap perhitungan potensi energi angin jika bagian kosong tersebut ditambal menggunakan rata-rata harian?
2. **Bahaya Moving Average pada Sistem Pengereman Otonom:**
   Pada traktor pertanian otonom yang menggunakan sensor jarak ultrasonik/LiDAR untuk pengereman darurat otomatis saat ada pekerja menyeberang di depan traktor, mengapa menerapkan *Simple Moving Average* dengan jendela $k = 50$ sampel ($5\text{ detik}$) adalah keputusan rekayasa yang mematikan? Filter apa yang lebih tepat digunakan untuk membuang pantulan palsu sesaat tanpa menunda waktu pengereman?

---

## 5. Referensi & Acuan Materi Pekan Ini

1. **McKinney, W. (2022).** *Python for Data Analysis: Data Wrangling with Pandas, NumPy, and Jupyter* (3rd ed.). O'Reilly Media. (ISBN-13: 978-1098104030) — **Bab 7 (Data Cleaning and Preparation: Handling Missing Data) & Bab 11 (Time Series: Moving Window Functions & Exponentially Weighted Functions).**  
2. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730) — **Bab 4 (Exploring IoT Data & Preprocessing Sensor Streams).**  
3. **Fawcett, T., & Provost, F. (2013).** *Data Science for Business*. O'Reilly Media. (ISBN-13: 978-1449361327)  
4. **Pandas Development Team. (2024).** *Pandas Documentation: `DataFrame.fillna`, `DataFrame.interpolate`, `DataFrame.rolling`, `DataFrame.ewm`*. Tersedia: [https://pandas.pydata.org/docs/](https://pandas.pydata.org/docs/)  
5. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
