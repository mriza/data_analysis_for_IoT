---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Kuis Pekan 9: Paradigma Machine Learning vs Pemrograman Tradisional"
---

# Kuis Pekan 9: Paradigma Machine Learning vs Pemrograman Tradisional

Jawablah pertanyaan-pertanyaan berikut dengan jelas, ringkas, dan berdasarkan penalaran analitik Anda!

1. Dalam membangun sistem otomasi penyiraman kebun kelapa sawit skala besar, seorang *programmer* tradisional mungkin akan menulis kode "*IF suhu > 35 AND kelembaban < 40 THEN siram*". Analisislah mengapa pendekatan logika *Hard-Coded Rules* (berbasis aturan baku) semacam ini akan gagal total dan tidak bisa berskala (*not scalable*) ketika jumlah parameter iklim yang memengaruhi pertumbuhan sawit bertambah menjadi 20 variabel yang saling berkaitan!

2. Paradigma *Machine Learning* membalik logika input-output komputer secara drastis. Jelaskan secara konseptual fase Pelatihan (*Training Phase*) di mana mesin menerima "Data Sensor Masa Lalu" beserta "Log Hasil Panen Historis", dan apa bentuk *output* final (yang dulunya ditulis oleh manusia) yang dilahirkan oleh mesin pada akhir fase ini!

3. Anda ditugaskan membangun sistem AI untuk memprediksi debit air saluran irigasi besok pagi. Dalam domain *Time-Series Forecasting*, algoritma membuat proyeksi ke masa depan dengan berasumsi pada sebuah hukum alam probabilistik. Jelaskan hukum asumsi dasar (terkait "pola masa lalu") yang menjadi fondasi beroperasinya seluruh algoritma *Forecasting*!

4. Sistem *Predictive Maintenance* (Pemeliharaan Prediktif) di pabrik pemrosesan teh mendeteksi sinyal audio dan getaran dari bantalan poros (*bearing*) mesin penggiling. Bagaimana *Machine Learning* mampu mencegah ledakan pabrik atau kerugian puluhan juta Rupiah dengan cara menganalisis sinyal getaran tersebut jauh berhari-hari sebelum logamnya benar-benar patah?

5. *Outlier* tunggal (seperti suhu melompat ke angka 100°C) mudah dideteksi menggunakan batas nilai rata-rata biasa. Namun, ada kalanya suhu menunjuk angka normal 28°C, kelembaban menunjuk normal 70%, namun kedua kondisi normal tersebut terjadi bersamaan di tengah badai hujan yang notabene merupakan fenomena alam yang aneh/mustahil. Mengapa algoritma deteksi batas (seperti Z-Score atau IQR) biasanya gagal menangkap *outlier multidimensi* semacam ini?

6. Menghadapi anomali ganjil (multidimensi) seperti pada kasus nomor 5, analis data IoT industri sering mengandalkan algoritma canggih tipe *Unsupervised Learning* (seperti *Isolation Forest* atau *K-Means Clustering*). Jelaskan logika dasar mengapa pendekatan tanpa label ("belajar sendiri/mengelompokkan sendiri") ini lebih ahli mendeteksi "keganjilan" yang tidak pernah terpikirkan oleh *Data Engineer* sebelumnya!

7. Kualitas model AI (Machine Learning) bergantung secara proporsional pada kualitas dan volume dataset pelatihannya (GIGO). Berikan penalaran analitik, mengapa implementasi *Machine Learning* yang sangat akurat untuk mendeteksi kecacatan daun tomat di dalam *Greenhouse* tertutup (hidroponik) kemungkinan besar akurasinya akan anjlok drastis jika model yang sama di-*deploy* ke kebun tomat konvensional (lahan terbuka / *open field*)!

8. Terdapat dua jenis keluaran (*output*) Machine Learning yang umum: Klasifikasi (*Classification*) dan Regresi (*Regression*). Jika Anda membangun sistem pengingat panen otomatis, analisis mengapa menebak "Jumlah hari tersisa sebelum padi siap panen (misal: 14 hari)" disebut masalah Regresi, sedangkan mendeteksi citra drone untuk menyimpulkan "Padi Sehat / Padi Kena Hama" disebut masalah Klasifikasi!

9. Saat algoritma prediktif (Forecasting) meramalkan suhu esok hari adalah 30°C, namun keesokan harinya sensor riil mencatat 32°C, maka terdapat penyimpangan (*error*) sebesar 2°C. Mengapa sangat krusial bagi seorang perancang IoT untuk terus menghitung besaran metrik *error* (seperti MAE atau RMSE) setiap harinya dari sistem yang sudah berjalan (di-*deploy*) di lapangan?

10. *Machine Learning* telah menggantikan banyak pekerjaan pemrograman manual. Namun sebagai insinyur *IoT Data Analytics*, kemukakan satu skenario sistem keselamatan (*safety-critical system*, misal: proteksi kebocoran gas beracun) di industri agrikultur di mana penggunaan *Hard-Coded Rules (IF-ELSE)* kuno justru sangat diwajibkan, sementara penggunaan AI yang cerdas malah terlarang atau sangat dihindari secara regulasi!
