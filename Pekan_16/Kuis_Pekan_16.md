---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Kuis Pekan 16: Evaluasi Akhir Semester (UAS) - Ujian Analitik Berbasis Kasus (HOTS)"
---

# Kuis Pekan 16: Evaluasi Akhir Semester (UAS) - Ujian Analitik Berbasis Kasus (HOTS)

Jawablah studi kasus komprehensif berikut dengan mengintegrasikan seluruh pemahaman *Data Science*, Pemrograman Python, dan Arsitektur Jaringan IoT yang telah Anda pelajari!

1. **(Arsitektur Jaringan)** Sebuah perusahaan agrikultur besar meminta Anda merancang topologi komunikasi data untuk 5.000 sensor cerdas di perkebunannya. Analisislah keuntungan strategis dari penggunaan protokol **MQTT** (sebagai *Publish-Subscribe Broker*) dibandingkan HTTP *Request-Response* biasa, terutama saat menghadapi kondisi jaringan seluler (3G/4G) yang sering putus-nyambung (*lossy network*) di area pedalaman!

2. **(Paradigma AI vs Sistem Pakar)** Direktur kebun meragukan usulan Anda menggunakan *Machine Learning* untuk mengatur jadwal penyiraman otomatis dan bersikeras agar Anda cukup menggunakan sistem *IF-ELSE* kuno. Rancanglah sebuah argumen teknis untuk meyakinkan beliau, mengapa aturan *IF-ELSE* (*Hard-Coded*) akan hancur berantakan ketika dihadapkan pada interaksi 15 variabel iklim ekstrem yang berubah tak terduga akibat pemanasan global!

3. **(Pra-pemrosesan Data)** Anda menemukan bahwa grafik data kecepatan angin memiliki 5 "lubang" berurutan (mati total selama 5 jam). Jika seorang rekan kerja menyarankan Anda untuk langsung menggunakan metode `Forward Fill` (*ffill*) pada pustaka Pandas untuk menambal ke-5 jam tersebut demi kejar tayang, evaluasi secara matematis dan logis, mengapa saran rekan Anda tersebut dapat berujung pada bencana prediksi cuaca!

4. **(Algoritma Deteksi)** Pada kasus kualitas udara di kota besar, polusi PM2.5 yang normalnya hanya 30 tiba-tiba melambung tinggi hingga 350 akibat pembakaran pabrik ilegal. Jelaskan mengapa metode deteksi anomali berbasis sebaran persentil (**Interquartile Range / IQR**) terbukti jauh lebih kuat (*robust*) dalam memvonis lonjakan 350 tersebut sebagai *Outlier*, dibandingkan metode **Z-Score** yang menggunakan perhitungan Rata-rata (*Mean*)!

5. **(Time-Series Database)** Bandingkan arsitektur **InfluxDB** dengan basis data relasional standar (MySQL/PostgreSQL). Jika Anda harus me-*render* grafik rentang waktu (*Time-Series*) suhu selama 1 tahun ke belakang (dengan jeda pengiriman tiap 1 detik), analisislah mengapa *query* MySQL akan membutuhkan waktu sangat lama (*loading* lambat) dibandingkan bahasa kueri *Flux* milik InfluxDB!

6. **(Edge vs Cloud Analytics)** Di sebuah pabrik penggergajian kayu pintar, getaran gergaji dipantau sensor akustik resolusi tinggi yang menghasilkan data sebesar 1 Gigabyte per menit. Analisislah mengapa mengirimkan *file* suara mentah tersebut ke *Cloud* untuk diolah oleh algoritma *Predictive Maintenance* adalah rancangan yang sia-sia, dan bagaimana *Tiny Machine Learning (TinyML)* pada *Edge Node* menjadi penyelamat arsitektur ini!

7. **(Integrasi End-to-End)** Dalam penulisan skrip perantara (*Orchestrator* / `main_pipeline.py`), posisi pengolahan data pembersihan (*Cleaning*) mutlak harus mendahului tahap *Machine Learning*, dan *Machine Learning* harus mendahului tahap penyimpanan ke *Database*. Analisis logika struktural aliran data ini: Apa yang terjadi jika penyimpanan *Database* diletakkan sebelum proses *Cleaning*?

8. **(Visualisasi dan Actionable Insights)** Di ruang kontrol bendungan pintar, terdapat ratusan *dashboard* Grafana. Terdapat sebuah panel pengukur debit air bah berdesain *Gauge* besar berwarna hijau terang. Evaluasi konsep *Data Storytelling* dan *Visibilitas Operasional* pada panel tersebut: Mengapa warna panel tersebut harus seketika berubah menjadi **Merah Menyala** dan membunyikan sirine (*Webhook Telegram*) ketika ambang batas keselamatan dilanggar?

9. **(Unsupervised Learning)** Terdapat jenis *Outlier* multidimensi (misal: suhu 25°C dan kelembaban 80% adalah normal secara terpisah, namun mustahil terjadi bersamaan di padang pasir pada siang bolong). Jelaskan secara konsep mengapa algoritma *Unsupervised Learning* (seperti **Isolation Forest**) secara inheren jauh lebih cerdas mendeteksi keganjilan relasional seperti ini dibandingkan aturan matematika statistika manual!

10. **(Portofolio dan Kode Etik Engineering)** Anda membangun sistem *Early Warning System* untuk memprediksi tanah longsor berbasis sensor kelembaban tanah dengan akurasi model AI sebesar 95%. Secara etika keteknikan (*Engineering Ethics*), analisislah mengapa Anda tetap wajib merancang skenario terburuk (*Fallback Procedure*)—seperti alarm manual berbahan mekanis/analog—dan tidak boleh menggantungkan seratus persen nyawa penduduk pada prediksi kecerdasan buatan Anda tersebut!
