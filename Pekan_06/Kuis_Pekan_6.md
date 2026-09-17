---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Kuis Pekan 6: Data Preprocessing (Cleaning & Smoothing)"
---

# Kuis Pekan 6: Data Preprocessing (Cleaning & Smoothing)

Jawablah pertanyaan-pertanyaan berikut dengan jelas, ringkas, dan berdasarkan penalaran analitik Anda!

1. Anda sedang memproses data sensor suhu menggunakan pustaka Pandas di Python dan menemukan lubang data (*Missing Values*) selama 10 menit beruntun akibat koneksi terputus. Jelaskan secara logis mengapa menggunakan metode penambalan `df.interpolate(method='linear')` akan memberikan estimasi suhu yang jauh lebih natural secara fisik dibandingkan menggunakan metode `df.ffill()`!

2. Dalam metode penambalan data maju (*Forward Fill* / `ffill`), sistem akan menyalin nilai sehat terakhir sebelum koneksi terputus untuk mengisi kekosongan data. Buatlah satu skenario fisik di lapangan (misal: terkait perubahan cuaca atau jam) di mana penerapan *Forward Fill* justru akan menghasilkan data palsu yang sangat menyesatkan bagi algoritma!

3. Anda mengaplikasikan fungsi penghalusan data (*Smoothing*) menggunakan *Moving Average* dengan perintah `df['suhu'].rolling(window=5).mean()`. Mengapa teknik perhitungan "rata-rata bergerak" ini sangat efektif meredam *electrical noise* (gerigi acak frekuensi tinggi) pada grafik sensor analog?

4. Setelah Anda menjalankan fungsi `rolling(window=5).mean()` pada dataset berukuran 1000 baris, Anda panik melihat bahwa baris ke-1 hingga ke-4 pada kolom hasilnya justru bernilai `NaN` (kosong). Analisislah mengapa Pandas secara matematis tidak menghasilkan *error*, melainkan memang sengaja mengosongkan 4 baris pertama tersebut!

5. Dalam penentuan batas anomali (*Outlier*) menggunakan metode statistik **Interquartile Range (IQR)**, kita tidak lagi menebak batas wajar secara serampangan. Jelaskan secara konseptual bagaimana nilai batas bawah (`Q1 - 1.5 * IQR`) dan batas atas (`Q3 + 1.5 * IQR`) bekerja sebagai "pagar pengaman" matematis untuk memvonis sebuah titik data sebagai anomali ekstrem!

6. Bandingkan metode deteksi *Outlier* berbasis **Z-Score** dengan **IQR**. Jika dataset suhu ruangan Anda sudah tercemar parah oleh banyak sekali *outlier* (sehingga nilai rata-ratanya ikut rusak), jelaskan mengapa metode IQR jauh lebih kebal (*robust*) dan direkomendasikan dibandingkan Z-Score pada situasi tersebut!

7. Anda membangun aktuator katup air otomatis yang dikendalikan secara *real-time* berdasarkan data kelembaban tanah. Jika Anda memasang filter *Moving Average* dengan ukuran *window* yang teramat besar (misal: `window=3600` detik), jelaskan konsekuensi fatal (*lag/delay*) apa yang akan terjadi pada respons fisik penyiraman air di lahan Anda!

8. Saat melakukan visualisasi grafik dengan Matplotlib untuk membandingkan "Data Suhu Asli" dan "Data Suhu Dihaluskan (MA-5)", analisislah mengapa penting menempatkan kurva data asli di latar belakang dengan opasitas rendah (*alpha=0.6*), alih-alih hanya menampilkan kurva yang sudah dihaluskan saja?

9. Jika Anda menggunakan sensor pengukur tingkat kekeruhan air (*Turbidity*) pada kolam ikan, nilai sensor sering berfluktuasi tajam setiap kali ada ikan berenang tepat di depan lensa sensor. Apakah fluktuasi tajam sesaat ini sebaiknya dihapus menggunakan deteksi IQR, atau disamarkan menggunakan *Moving Average*? Berikan argumentasi teknis Anda!

10. Seluruh tahap pengerjaan (Interpolasi, Moving Average, Deteksi IQR) pada praktikum pekan ini dijalankan menggunakan **Jupyter Notebook**. Menurut Anda, apa kelemahan terbesar Jupyter Notebook jika *script pipeline* pembersihan data ini ingin diimplementasikan ke dalam *server* produksi yang harus berjalan otomatis 24 jam nonstop tanpa campur tangan manusia?
