# Materi Kuliah Pekan 1: Pengantar Analisis Data untuk IoT

## 1. Tujuan Pembelajaran Khusus
- Mahasiswa mampu menjelaskan karakteristik data IoT (Velocity, Volume, Variety).
- Mahasiswa mampu memahami dan membedakan arsitektur aliran data: Edge, Fog, dan Cloud Analytics.
- Mahasiswa mampu memberikan contoh kasus penerapan analitik data pada sektor pertanian (Smart Agriculture).

## 2. Pendahuluan & Konteks Industri
Di era industri 4.0, sensor IoT tidak hanya berfungsi sebagai pengumpul data pasif, namun sebagai fondasi bagi sistem otonom. Pada sektor *Smart Agriculture*, ratusan sensor kelembaban tanah, cuaca, dan kamera daun (CCTV) tersebar di lahan pertanian untuk memonitor kondisi tanaman. Namun, tantangan utama bukanlah pada pengumpulan data, melainkan pada bagaimana mengolah jutaan baris data "kotor" tersebut menjadi informasi yang dapat ditindaklanjuti (*Actionable Insights*).

## 3. Materi Inti
### 3.1 Karakteristik 3V pada Data IoT
Data IoT berbeda dari data tradisional (seperti data transaksi perbankan) karena memiliki karakteristik 3V:
- **Volume (Kapasitas Besar):** Sensor memproduksi data setiap detiknya. 1000 sensor yang mengirim data setiap 5 detik akan menghasilkan ~17 juta baris data dalam sehari.
- **Velocity (Kecepatan Tinggi):** Aliran data terjadi secara *streaming* dan *real-time*. Data terlambat 1 menit bisa berakibat fatal pada sistem otomatisasi (misal aktuator penyiraman tertunda).
- **Variety (Keberagaman Format):** Data tidak hanya berupa teks/angka dalam CSV, tetapi juga deret waktu (Time-Series), JSON dari API, dan data tidak terstruktur seperti gambar infra-merah dari drone.

### 3.2 Mengapa Data Sensor IoT Disebut "Kotor"?
Perangkat keras (sensor) di lapangan beroperasi pada kondisi ekstrem:
- **Interferensi Lingkungan:** Debu, kelembaban embun, atau hewan yang menutupi permukaan sensor dapat mengubah pembacaan nilai secara drastis (menjadi *Outlier*).
- **Fluktuasi Daya:** Baterai yang hampir habis dapat menyebabkan sensor mengirimkan nilai yang tidak konsisten atau melewatkan siklus pengiriman (*Missing Values*).
- **Lossy Network:** Sinyal LoRa atau WiFi yang lemah di tengah kebun menyebabkan paket data hilang (*Packet Loss*).

### 3.3 Arsitektur Analitik Data IoT
Memproses seluruh data di satu tempat tidak lagi efisien. Oleh karena itu, arsitektur dibagi menjadi tiga:
1. **Edge Analytics:** Komputasi dilakukan langsung di mikrokontroler (misal ESP32/Raspberry Pi) di lahan. 
   - *Kelebihan:* Latensi sangat rendah, hemat bandwidth internet.
   - *Kekurangan:* Daya komputasi dan memori sangat terbatas.
2. **Fog Analytics:** Gateway lokal (misal: Mini PC di pos jaga kebun) yang menerima data dari banyak *Edge* sebelum dikirim ke Cloud. Berguna untuk agregasi data (merangkum rata-rata suhu per jam).
3. **Cloud Analytics:** Pemrosesan di *data center* berskala besar (AWS, GCP, server kampus). 
   - *Kelebihan:* Mampu menjalankan model *Machine Learning* yang berat (Deep Learning).
   - *Kekurangan:* Bergantung penuh pada internet, latensi lebih tinggi.

## 4. Studi Kasus (Real-world Example)
**Kasus: Sistem Irigasi Cerdas di Perkebunan Kopi**
Alih-alih mengirimkan nilai kelembaban tanah setiap 1 detik ke server Cloud di Jakarta (yang memakan kuota internet dan baterai), mikrokontroler di lahan kopi (*Edge*) diprogram untuk mengkalkulasi rata-rata kelembaban setiap 15 menit. Jika nilainya turun drastis, *Edge* akan langsung menghidupkan pompa irigasi tanpa perlu instruksi dari Cloud, sementara laporan rekapitulasinya baru dikirim ke Cloud setiap 1 jam.

## 5. Bahan Diskusi Kelas
1. Jika Anda diminta membangun sistem peringatan dini kebakaran hutan berbasis IoT di pedalaman tanpa sinyal 4G, arsitektur mana yang akan lebih banyak mengambil peran: Edge, Fog, atau Cloud? Mengapa?
2. Bagaimana cara membedakan apakah nilai suhu yang melonjak tiba-tiba adalah "kebakaran" atau sekadar "sensor rusak karena kepanasan"?

## 6. Referensi Spesifik
- Hanes, D., et al. (2017). *IoT Fundamentals: Networking Technologies, Protocols, and Use Cases for the Internet of Things*. Cisco Press. (Bab: Data Analytics for IoT)
