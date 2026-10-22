---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Kuliah Pekan 1: Pengantar Analisis Data untuk IoT dan Arsitektur Aliran Data"
---

# Materi Kuliah Pekan 1: Pengantar Analisis Data untuk IoT dan Arsitektur Aliran Data

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 1 (Kuliah Teori — $2 \times 50$ Menit)  
**Capaian Pembelajaran (Sub-CPMK 1):** Mahasiswa mampu menjelaskan konsep dasar analitik IoT, karakteristik data sensor (3V/5V), penyebab kerusakan data fisik di lapangan, serta membedakan arsitektur aliran data (*Edge*, *Fog*, dan *Cloud Analytics*) sesuai standar industri.

---

## 1. Tujuan Pembelajaran Khusus
Setelah mengikuti perkuliahan pekan ini, mahasiswa diharapkan mampu:
1. Menjelaskan transformasi nilai data melalui piramida **DIKW (*Data, Information, Knowledge, Wisdom / Actionable Insight*)** pada ekosistem *Internet of Things* (IoT).
2. Menganalisis karakteristik unik data IoT (**Volume, Velocity, Variety, Veracity, Value**) beserta implikasi matematisnya terhadap kebutuhan kapasitas penyimpanan dan *bandwidth* jaringan.
3. Mengidentifikasi faktor-faktor fisik dan elektronika yang menyebabkan data sensor IoT bersifat "kotor" (*noisy, incomplete, drifting*).
4. Membandingkan arsitektur komputasi **Edge, Fog, dan Cloud Analytics** berdasarkan parameter latensi, kebutuhan *bandwidth*, kapasitas komputasi, keandalan (*reliability*), dan keamanan/privasi.
5. Merancang penempatan logika analitik yang tepat pada studi kasus nyata di bidang **Smart Agriculture** (Pertanian Presisi) dan pemantauan lingkungan.

---

## 2. Pendahuluan & Konteks Industri: Dari Sensor Pasif Menuju Sistem Otonom

Di era Industri 4.0 dan Pertanian Presisi (*Precision Agriculture*), perangkat IoT tidak lagi dipasang hanya sebagai "pencatat angka digital" yang pasif. Pada sebuah perkebunan cerdas (*Smart Greenhouse* atau lahan kelapa sawit), ratusan hingga ribuan titik sensor—mulai dari sensor kelembaban tanah kapasitif, suhu dan kelembaban udara (DHT22/BME280), pH tanah, kecepatan angin (anemometer), intensitas radiasi matahari (lux/PAR), hingga kamera multispektral—bekerja 24 jam sehari tanpa henti.

Namun, masalah terbesar di industri saat ini adalah fenomena **"Data Rich, Information Poor" (Kaya Data, Miskin Informasi)**. Banyak perusahaan atau institusi pertanian berhasil mengumpulkan jutaan baris angka sensor ke dalam server, tetapi data tersebut hanya menumpuk menjadi "kuburan data" (*data graveyard*) karena tidak dianalisis. Nilai ekonomi sesungguhnya dari IoT bukan terletak pada perangkat keras sensornya, melainkan pada **Analisis Data (*Data Analytics*)** yang mengubah deretan angka mentah menjadi keputusan otomatis (*Actionable Insights*).

### 2.1 Piramida DIKW dalam Konteks Smart Agriculture
Untuk memahami esensi analisis data IoT, perhatikan bagaimana satu pembacaan sensor bertransformasi melalui empat lapisan piramida **DIKW**:

```text
                        /\
                       /  \        4. WISDOM / ACTION (Tindakan Otonom / Keputusan)
                      /----\          "Buka katup solenoid Zona A selama 12 menit
                     /      \          pada pukul 05.30 pagi sebelum penguapan puncak."
                    /--------\     3. KNOWLEDGE (Pengetahuan Pola & Prediksi ML)
                   /          \       "Laju penurunan kelembaban 2%/jam pada suhu >33°C
                  /------------\       menandakan tanaman akan mengalami stres air (wilting)
                 /              \      dalam 3 jam ke depan."
                /----------------\ 2. INFORMATION (Data Terkontekstualisasi & Bersih)
               /                  \   "Rata-rata kelembaban tanah Zona A pukul 13.00 WIB
              /--------------------\   adalah 28.4% (di bawah ambang batas normal 40%)."
             /                      \ 1. DATA MENTAH (Raw Sensor Telemetry)
            /________________________\   Payload biner/teks: `{"ts":1730440800, "adc":2890}`
```

1. **Data Mentah (*Raw Data*):** Nilai tegangan analog atau nilai register ADC (*Analog-to-Digital Converter*) yang dikirim mikrokontroler, misalnya angka `2890` dari ADC 12-bit ESP32. Angka ini belum memiliki arti bagi petani.
2. **Informasi (*Information*):** Hasil kalibrasi, pembersihan derau (*cleaning*), dan pemberian konteks waktu/lokasi, misalnya: *"Kelembaban tanah di Blok Cabai A pada pukul 13.00 WIB adalah $28,4\%$"*.
3. **Pengetahuan (*Knowledge*):** Hasil penggabungan beberapa variabel (*multivariate correlation*) dan pemodelan historis, misalnya: *"Ketika suhu udara menembus $34^\circ\text{C}$ dan kelembaban tanah turun di bawah $30\%$, laju evapotranspirasi meningkat dua kali lipat sehingga tanaman akan layu dalam $3\text{ jam}$"*.
4. **Kebijaksanaan / Aksi (*Wisdom / Actionable Insight*):** Eksekusi keputusan otomatis berbasis *knowledge*, baik berupa perintah langsung ke aktuator (*closed-loop control* menyalakan pompa irigasi) maupun peringatan dini (*Early Warning System*) ke ponsel manajer kebun.

---

## 3. Materi Inti

### 3.1 Karakteristik 5V pada Data IoT (Mengapa Berbeda dari Data Tradisional?)
Data yang dihasilkan oleh jaringan sensor IoT memiliki sifat yang sangat berbeda dibandingkan data transaksional bisnis biasa (seperti data kasir minimarket, sistem akademik kampus, atau transaksi perbankan). Dalam literatur *Big Data* dan IoT (Hanes et al., 2017; Minteer, 2017), karakteristik ini dirumuskan menjadi **3V Utama** dan diperluas menjadi **5V**:

1. **Volume (Kapasitas Masif Akumulatif):**
   Satu paket pesan sensor mungkin berukuran sangat kecil (hanya $50 - 200\text{ Bytes}$). Namun, karena dikirimkan secara terus-menerus oleh ribuan perangkat selama 24/7, akumulasi volumenya menjadi raksasa.
2. **Velocity (Kecepatan Aliran *Streaming* Tinggi):**
   Data IoT mengalir secara waktu nyata (*real-time streaming*) dengan frekuensi tetap (misal setiap $1\text{ detik}$, $100\text{ milidetik}$, atau $1\text{ kHz}$ pada sensor getaran mesin). Keterlambatan pemrosesan (*latency*) selama beberapa detik saja dapat menyebabkan kegagalan fatal pada sistem kendali otomatis.
3. **Variety (Keberagaman Format & Struktur):**
   Ekosistem IoT memadukan data terstruktur (tabel angka waktu/suhu di CSV atau database), data semi-terstruktur (payload bersarang JSON/XML dari broker MQTT), hingga data tidak terstruktur (*unstructured data* seperti citra kamera termal drone, rekaman spektrum akustik mikrofon kandang, atau sinyal gelombang getaran).
4. **Veracity (Ketidakpastian & Kebenaran Data):**
   Seberapa dapat dipercaya data tersebut? Data IoT memiliki tingkat *veracity* (kualitas kebenaran awal) yang rendah karena penuh dengan derau listrik (*noise*), nilai hilang (*missing values*), dan pergeseran kalibrasi (*sensor drift*).
5. **Value (Nilai Guna Ekstraksi):**
   Satu titik data suhu pada pukul 02.00 dini hari yang bernilai normal hampir tidak memiliki nilai bisnis jika berdiri sendiri. Nilai (*value*) baru muncul ketika jutaan titik data tersebut diekstraksi polanya untuk mencegah gagal panen atau kerusakan mesin (*Predictive Maintenance*).

#### Contoh Perhitungan Kuantitatif Beban Data IoT (Studi Kasus Perkebunan Sawit)
Bayangkan sebuah perusahaan perkebunan kelapa sawit memasang **$N = 2.000\text{ unit}$** node sensor kelembaban dan suhu tanah. Setiap node mengirimkan data telemetri setiap **$\Delta t = 1\text{ detik}$** ($f_s = 1\text{ Hz}$) selama 24 jam penuh. Ukuran rata-rata satu baris rekaman dalam format JSON/CSV adalah **$B = 100\text{ Bytes}$**.

Mari kita hitung akumulasi datanya:
- **Jumlah baris data per detik:** $2.000\text{ baris/detik}$.
- **Jumlah baris data per hari (24 jam):**
  $$\text{Baris}_{\text{hari}} = 2.000\text{ sensor} \times 86.400\text{ detik/hari} = 172.800.000\text{ baris/hari (172,8 Juta baris!)}$$
- **Jumlah baris data dalam 1 minggu (7 hari):**
  $$\text{Baris}_{\text{minggu}} = 172.800.000 \times 7 = 1.209.600.000\text{ baris (1,209 Miliar baris!)}$$
- **Kapasitas penyimpanan mentah per minggu:**
  $$\text{Ukuran}_{\text{minggu}} = 1.209.600.000 \times 100\text{ Bytes} \approx 120,96\text{ Gigabytes (GB)}$$

> **Implikasi Rekayasa (*Engineering Insight*):**
> Aplikasi *spreadsheet* konvensional seperti **Microsoft Excel** memiliki batas keras (*hard limit*) maksimal **1.048.576 baris** per lembar kerja. Artinya, data dari 2.000 sensor di atas akan membuat Excel **penuh dan *crash* hanya dalam waktu 8 menit 44 detik**! Inilah alasan mutlak mengapa seorang *IoT Data Engineer* wajib menguasai pemrograman **Python (Pandas/NumPy)** dan **Time-Series Database (InfluxDB)**.

---

### 3.2 Anatomi Fisik Mengapa Data Sensor IoT Selalu "Kotor" (*Dirty Data*)
Dalam sistem perbankan, jika nasabah mentransfer Rp100.000, angka di database pasti tercatat tepat `100000` (bersih dan deterministik). Sebaliknya, perangkat IoT berinteraksi langsung dengan hukum fisika alam yang keras dan tidak sempurna. Berikut adalah lima penyebab teknis mengapa data IoT selalu "kotor":

```text
[Fenomena Fisik Alam] ---> [Transduser Sensor] ---> [Sirkuit Analog & ADC] ---> [Transmisi Radio] ---> [Database Server]
       |                           |                         |                         |                       |
  Gangguan Lingkungan:        Degradasi Fisik:          Derau Elektronika:        Lossy Network:          Masalah Waktu:
  - Embun / hujan ekstrem     - Korosi elektroda        - Ripple catu daya        - Redaman pohon/hujan   - Clock drift RTC
  - Kotoran hewan / debu      - Bio-fouling (lumut)     - Induksi relay/motor     - Tabrakan paket RF     - Perbedaan zona
  - Bayangan daun sesaat      - Sensor Drift lambat     - Tegangan baterai drop   - Paket hilang (NaN)      waktu (WIB/UTC)
```

1. **Derau Elektronika & Induksi Elektromagnetik (*Electrical & Thermal Noise*):**
   Sinyal tegangan lemah dari sensor analog (orde milivolt) sangat rentan terganggu oleh riak catu daya (*power supply ripple*) atau induksi medan magnet saat motor pompa air/relay AC menyala di dekat kabel sensor. Hal ini memicu lonjakan angka sesaat (*spike / hard outlier*), misalnya suhu ruangan $25^\circ\text{C}$ tiba-tiba terbaca $95^\circ\text{C}$ selama 1 detik.
2. **Fluktuasi Catu Daya & Penurunan Baterai (*Brownout & Battery Sag*):**
   Node sensor di tengah ladang umumnya ditenagai baterai Lithium atau panel surya. Saat baterai hampir habis di malam hari, tegangan referensi ($V_{\text{ref}}$) pada sirkuit ADC mikrokontroler menjadi tidak stabil, menyebabkan pembacaan sensor melenceng jauh atau perangkat mengalami *restart* berulang.
3. **Pergeseran Kalibrasi Bertahap (*Sensor Drift*):**
   Sensor yang tertanam di tanah atau terendam di kolam nutrisi hidroponik akan mengalami penumpukan kerak mineral, korosi tembaga, atau lumut (*bio-fouling*). Akibatnya, nilai bacaan bergeser secara perlahan dan monoton dari nilai sebenarnya (misal pH air murni $7,0$ bergeser naik $0,08\text{ pH}$ setiap hari hingga terbaca $8,6$ walaupun air tidak berubah).
4. **Kegagalan Sensor Macet (*Stuck-at Fault / Zero-Variance*):**
   Kerusakan komponen internal atau *hang* pada register komunikasi bus I2C/OneWire dapat menyebabkan sensor mengirimkan angka yang persis sama tanpa perubahan desimal sedikit pun selama berjam-jam (misal `27.450000°C` terus-menerus selama 18 jam), padahal suhu alam tidak pernah benar-benar datar sempurna.
5. **Jaringan Nirkabel yang Rentan Rugi (*Lossy Network & Packet Loss*):**
   Sinyal WiFi, LoRaWAN, atau seluler di area perkebunan terhalang oleh pepohonan lebat (*foliage attenuation*), kontur bukit, dan cuaca hujan. Paket yang hilang di udara menyebabkan "lubang waktu" (*Missing Values* / `NaN`) pada urutan data di server.

---

### 3.3 Arsitektur Aliran & Komputasi Data IoT: Edge, Fog, dan Cloud Analytics
Jika kita memiliki ribuan sensor yang memproduksi data setiap detik, **di manakah proses analisis data sebaiknya dilakukan?** Memindahkan seluruh data mentah langsung ke server pusat di *Cloud* seringkali tidak realistis karena keterbatasan *bandwidth* dan tuntutan waktu respons cepat. Oleh karena itu, standar arsitektur IoT modern (Cisco IoT Reference Model — Hanes et al., 2017) membagi komputasi menjadi tiga lapisan hierarkis:

```text
+===================================================================================================+
|                               3. CLOUD ANALYTICS (Data Center / Server Pusat)                     |
|  Perangkat : Server Kampus, AWS IoT, Google Cloud, InfluxDB Cloud, Cluster GPU                    |
|  Peran     : Penyimpanan historis jangka panjang (Big Data Lake), Pelatihan (Training) model      |
|              Machine Learning / Deep Learning skala besar, Dashboard pemantauan multi-wilayah.    |
|  Latensi   : Tinggi (500 ms - beberapa detik/menit) | Bandwidth: Tinggi (teragregasi)             |
+===================================================================================================+
                                                 ^
                                                 |  Koneksi WAN / Internet (4G / Satelit / Fiber)
                                                 |  Data yang dikirim: Ringkasan per 15-60 menit & Alert
                                                 v
+===================================================================================================+
|                               2. FOG ANALYTICS (Edge Gateway Lokal / Pos Kebun)                   |
|  Perangkat : Raspberry Pi 4/5, Industrial Mini PC, NVIDIA Jetson, Server Lokal (Ubuntu VM)        |
|  Peran     : Broker MQTT lokal (Mosquitto/RabbitMQ), Agregasi data dari puluhan sensor,           |
|              Pembersihan outlier multivariat, Inferensi ML lokal, Buffering saat internet mati.   |
|  Latensi   : Rendah (10 ms - 100 ms)                | Jaringan: LAN / WiFi Lokal / LoRa Gateway   |
+===================================================================================================+
                                                 ^
                                                 |  Koneksi Nirkabel Lokal (WiFi / BLE / LoRa / Zigbee)
                                                 |  Data yang dikirim: Telemetri terfilter / Delta
                                                 v
+===================================================================================================+
|                               1. EDGE ANALYTICS (Extreme Edge / Node Sensor-Aktuator)             |
|  Perangkat : Mikrokontroler ESP32, Arduino, STM32 (SRAM terbatas ~32 KB - 520 KB)                 |
|  Peran     : Akuisisi ADC, Filtering derau cepat (Moving Average / Circular Buffer),              |
|              Thresholding darurat (< 10 ms langsung memutus/menyalakan relay), TinyML (INT8).     |
|  Latensi   : Sangat Rendah / Instan (< 1 - 10 ms)   | Ketergantungan Internet: NOL (Mandiri)      |
+===================================================================================================+
```

#### Tabel Perbandingan Teknis Arsitektur Edge vs Fog vs Cloud Analytics

| Parameter Pembanding | Edge Analytics (Node Sensor) | Fog Analytics (Gateway Lokal) | Cloud Analytics (Server Pusat) |
| :--- | :--- | :--- | :--- |
| **Lokasi Fisik** | Menempel langsung pada sensor/aktuator di lahan | Pos jaga kebun, ruang panel gedung, menara lokal | Pusat data (*Data Center*) berjarak ratusan/ribuan km |
| **Contoh Perangkat Keras** | ESP32, STM32, Arduino Nano 33 BLE | Raspberry Pi, Mini PC Ubuntu, Industrial Gateway | Server Rack Xeon, AWS EC2, Google Cloud Platform |
| **Kapasitas Memori (RAM)** | Sangat terbatas ($2\text{ KB} - 520\text{ KB}$ SRAM) | Menengah ($2\text{ GB} - 16\text{ GB}$ RAM) | Sangat besar ($64\text{ GB}$ hingga Terabyte RAM) |
| **Latensi Respons Kendali** | **$< 1\text{ ms} - 10\text{ ms}$ (Real-Time Keras)** | $10\text{ ms} - 100\text{ ms}$ | $500\text{ ms} - \text{beberapa detik}$ (bergantung sinyal) |
| **Ketergantungan Internet** | **Tidak butuh internet sama sekali** | Tetap berjalan lokal meski internet ke pusat putus | **Lumpuh total jika koneksi internet lapangan putus** |
| **Konsumsi Bandwidth WAN** | Sangat hemat (hanya kirim *exception/alert*) | Hemat (mengirim data yang sudah diringkas/agregasi) | Sangat boros jika menerima seluruh *raw stream* |
| **Jenis Algoritma Analitik** | *Moving Average* sederhana, *Thresholding*, *TinyML* (*Quantized INT8*) | Pembersihan *Outlier* (IQR/Z-Score), *Isolation Forest*, Broker MQTT, InfluxDB lokal | *Training Deep Learning*, *Big Data Historical Forecasting*, korelasi lintas provinsi |

#### Kapan Harus Memilih Edge, Fog, atau Cloud? (Aturan Praktis Engineer)
1. **Gunakan Edge Analytics** apabila keputusan harus diambil dalam hitungan **milidetik** demi keselamatan fisik (*safety-critical*) atau ketika koneksi jaringan sama sekali tidak boleh menjadi titik kegagalan (*single point of failure*).
   - *Contoh:* Mematikan katup solenoid pipa irigasi bertekanan tinggi dalam $50\text{ ms}$ saat tekanan melonjak agar pipa tidak meledak, atau sistem pengereman darurat traktor otonom.
2. **Gunakan Fog Analytics** apabila Anda perlu menggabungkan (*fusion*) data dari puluhan sensor di satu lokasi (misal 50 sensor di dalam 1 rumah kaca), membersihkan data dengan Python, menampilkan *dashboard* lokal bagi operator lapangan, dan menyangga (*buffering*) data saat koneksi internet seluler sedang gangguan.
3. **Gunakan Cloud Analytics** apabila Anda membutuhkan penyimpanan historis jangka panjang (berbulan-bulan/bertahun-tahun), melatih (*training*) model *Machine Learning* yang membutuhkan komputasi berat, atau menyajikan *dashboard* pemantauan terpusat bagi manajemen di kantor pusat.

---

## 4. Studi Kasus Industri (*Real-World Engineering Case Study*)

### Kasus: Desain Arsitektur Sistem Irigasi & Peringatan Embun Beku (*Frost Warning*) di Perkebunan Kopi Arabika Dataran Tinggi

Sebuah perkebunan kopi Arabika di lereng gunung (ketinggian $1.500\text{ mdpl}$) memiliki 100 titik sensor kelembaban tanah dan suhu udara yang ditenagai oleh baterai dan panel surya kecil. Sinyal internet seluler 4G di area lereng gunung tersebut sangat lemah, berbiaya mahal, dan sering terputus saat turun hujan lebat.

**Kesalahan Desain Naif (Murni Cloud-Centric):**
Jika pengembang memprogram ke-100 mikrokontroler ESP32 untuk menyalakan WiFi/Modem dan mengirim angka kelembaban tanah setiap 1 detik langsung ke server Cloud di Jakarta:
1. Baterai sensor akan habis hanya dalam waktu kurang dari 24 jam karena transmisi radio kontinu memakan arus besar ($\sim 150 - 250\text{ mA}$).
2. Kuota internet akan habis untuk mengirim jutaan angka suhu/kelembaban yang nilainya hampir tidak berubah dari detik ke detik.
3. Saat terjadi cuaca ekstrem dan internet terputus, pompa irigasi tidak menerima perintah dari Cloud sehingga gagal bekerja.

**Solusi Arsitektur Hibrida (Edge + Fog + Cloud):**
1. **Pada Level Edge (ESP32 di Lahan):**
   - Mikrokontroler membaca sensor setiap 1 menit, menyimpannya ke dalam *Circular Buffer* di RAM, dan menghitung nilai rata-rata serta deteksi lonjakan lokal.
   - Jika kelembaban tanah anjlok di bawah batas kritis $25\%$, **ESP32 langsung memicu relay pompa irigasi lokal secara mandiri** tanpa menunggu instruksi server luar.
   - Di luar kondisi darurat, ESP32 hanya mengirimkan paket data setiap 15 menit sekali (atau menerapkan *Report-by-Exception* / *Delta Reporting*: hanya mengirim jika nilai berubah lebih dari $\pm 1\%$).
2. **Pada Level Fog (Raspberry Pi Gateway di Pos Kebun):**
   - Menerima transmisi lokal dari 100 node, memvalidasi silang antar-sensor tetangga (*cross-sensor validation*), membuang *outlier* akibat gangguan elektrik, dan menyimpan log harian di database lokal.
   - Setiap 1 jam sekali, Fog Gateway mengirimkan paket ringkasan statistik (*Mean, Min, Max, StdDev*) ke Cloud.
3. **Pada Level Cloud (Server Pusat):**
   - Menerima ringkasan data per jam dari seluruh kebun untuk melatih model prediksi panen tahunan dan menampilkan tren agroklimatologi jangka panjang kepada direksi.

---

## 5. Bahan Diskusi & Berpikir Kritis (*HOTS*) di Kelas

1. **Analisis Kritis Arsitektur Tanpa Sinyal:**
   Jika Anda ditugaskan membangun sistem peringatan dini kebakaran hutan gambut berbasis IoT di pedalaman Sumatera/Kalimantan yang sama sekali tidak terjangkau sinyal 4G seluler, bagaimana Anda mendistribusikan peran antara *Edge Node*, *Fog Gateway*, dan komunikasi radio jarak jauh (seperti LoRaWAN/Satelit)? Jelaskan data apa yang diproses di tempat dan data apa yang dikirim!
2. **Membedakan Kejadian Bahaya vs Kerusakan Sensor:**
   Sebuah sensor suhu di lahan mengirimkan pembacaan berurutan setiap menit: `28°C, 28°C, 120°C, 28°C, 28°C`. Sementara sensor di titik lain mengirimkan: `28°C, 35°C, 52°C, 78°C, 110°C`. Secara analisis data, bagaimana algoritma Anda membedakan mana yang merupakan **"sensor rusak/noise elektrik"** dan mana yang merupakan **"kejadian kebakaran nyata"**? Variabel sensor tambahan apa yang dapat Anda gunakan untuk melakukan validasi silang (*cross-validation*)?
3. **Trade-off Downsampling di Edge:**
   Untuk menghemat kuota internet, seorang *engineer* merangkum pembacaan sensor kecepatan angin (yang dicatat setiap 0,1 detik) menjadi 1 angka rata-rata (*Mean*) setiap 15 menit sebelum dikirim ke Cloud. Mengapa mengirim **hanya nilai Mean** sangat berbahaya bagi analisis risiko robohnya tanaman akibat angin puting beliung sesaat (*peak wind gust*)? Metrik statistik apa lagi yang wajib disertakan dalam paket ringkasan 15 menit tersebut?

---

## 6. Referensi & Acuan Materi Pekan Ini

Materi pada pekan ini disusun berdasarkan acuan buku teks dan dokumentasi berikut (tersedia pada folder `Referensi/`):

1. **Hanes, D., Salgueiro, G., Grossetete, P., Barton, R., & Henry, J. (2017).** *IoT Fundamentals: Networking Technologies, Protocols, and Use Cases for the Internet of Things*. Cisco Press. (ISBN-13: 978-1587144561) — **Bab 1 (What Is IoT? / IoT Network Architecture: Edge, Fog, and Cloud) & Bab 7 (Data and Analytics for IoT: Big Data, 3V Characteristics, Edge vs Cloud Analytics).**  
2. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730) — **Bab 1 (Defining IoT Analytics and Challenges: Dirty Sensor Data, Network Constraints, Time-Series Nature).**  
3. **Fawcett, T., & Provost, F. (2013).** *Data Science for Business: What You Need to Know about Data Mining and Data-Analytic Thinking*. O'Reilly Media. (ISBN-13: 978-1449361327) — **Bab 1 (Introduction: Data-Analytic Thinking) & Bab 2 (Business Problems and Data Science Solutions).**  
4. **ACM/IEEE-CS. (2020).** *Computing Curricula 2020 (CC2020): Paradigms for Global Computing Education*. ACM. (ISBN-13: 978-1450390804) — **Domain Kompetensi CE-IoT (Internet of Things) & CE-CEI (Cloud and Edge Infrastructure).**  
5. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
