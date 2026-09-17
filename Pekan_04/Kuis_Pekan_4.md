---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Kuis Pekan 4: Membaca Data & Exploratory Data Analysis (EDA)"
---

# Kuis Pekan 4: Membaca Data & Exploratory Data Analysis (EDA)

Jawablah pertanyaan-pertanyaan berikut dengan jelas, ringkas, dan berdasarkan penalaran analitik Anda!

1. Sebelum sebuah model kecerdasan buatan (*Machine Learning*) dapat dilatih untuk memprediksi hasil panen, para insinyur data wajib melakukan proses *Exploratory Data Analysis* (EDA) terlebih dahulu. Mengapa proses "berkenalan dengan data" ini dianggap krusial, dan sebutkan dua risiko fatal yang mungkin terjadi jika tahap EDA diabaikan begitu saja?

2. Anda menerima dataset sensor kualitas udara berformat CSV yang berisi 2 juta baris data. Analisislah mengapa menggunakan *library* Pandas di Python (`import pandas as pd`) jauh lebih efisien, terukur (*scalable*), dan direkomendasikan untuk fase EDA tingkat lanjut dibandingkan memuat file tersebut secara manual ke aplikasi semacam Microsoft Excel!

3. Tiga pilar utama dalam EDA adalah Statistik Deskriptif, Pembersihan Awal (Deteksi Kotoran), dan Visualisasi. Jelaskan bagaimana fungsi Statistik Deskriptif dapat membantu Anda secara cepat mendeteksi keberadaan *outlier* pada kolom data sensor suhu tanpa perlu menggambar grafik sama sekali!

4. Kolom `timestamp` pada dataset IoT yang baru diimpor ke Pandas awalnya sering terdeteksi sebagai tipe data `object` (sekadar untaian teks biasa). Mengapa sangat penting untuk mengonversinya menjadi tipe data `datetime` (parsing waktu) sebelum melakukan pemotongan data (*slicing*) berbasis bulan atau minggu?

5. Pada *time-series data*, mengubah kolom `timestamp` yang sudah diparsing menjadi indeks utama baris (*DatetimeIndex*) merupakan sebuah *best practice*. Apa keuntungan spesifik secara komputasi maupun fungsionalitas ketika kita menetapkan waktu sebagai *index* di Pandas?

6. Saat Anda menggambar grafik tren harian kelembaban tanah selama musim hujan menggunakan *Matplotlib*, Anda merasa grafiknya terlalu kaku dan ingin memvisualisasikan sebaran distribusi datanya secara statistik (seperti *boxplot* atau kurva densitas) dengan kode seminimal mungkin. Mengapa beralih menggunakan pustaka *Seaborn* adalah keputusan yang logis untuk skenario tersebut?

7. Dalam konteks pembersihan data (deteksi kotoran awal), jika Anda menemukan bahwa 30% nilai pada kolom "Radiasi Matahari" adalah kosong (`NaN`) karena sensor tertutup debu vulkanik, langkah analitik rasional apa yang akan Anda pertimbangkan: menghapus 30% baris tersebut, atau membiarkannya? Berikan argumen untuk pilihan Anda!

8. Melalui perhitungan *Korelasi Pearson* antara variabel X (Suhu Udara) dan variabel Y (Kelembaban Relatif Udara), Anda mendapatkan skor koefisien sebesar **-0.89**. Interpretasikan makna fisik dan matematis dari angka tersebut di lapangan!

9. Jika ada dua sensor, yaitu (A) Sensor pH Tanah dan (B) Sensor Kebisingan Suara Traktor, perhitungan korelasi mereka kemungkinan besar akan menghasilkan angka yang sangat mendekati **0**. Mengapa demikian, dan apa kesimpulan kausalitas (sebab-akibat) yang bisa ditarik dari angka nol tersebut?

10. Ketika menganalisis sistem IoT industri dengan lebih dari 50 parameter sensor yang berbeda, menelaah tabel korelasi angka secara manual menjadi tugas yang mustahil. Jelaskan bagaimana komponen visual **Heatmap** memecahkan kebuntuan kognitif ini dan mempercepat seorang analis dalam menemukan sensor mana yang paling berpengaruh satu sama lain!
