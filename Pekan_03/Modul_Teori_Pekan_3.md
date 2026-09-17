---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Kuliah Pekan 3: Anatomi dan Karakteristik Data Time-Series"
---

# Materi Kuliah Pekan 3: Anatomi dan Karakteristik Data Time-Series

## 1. Tujuan Pembelajaran Khusus
- Mahasiswa mampu mengidentifikasi komponen penyusun data beruntun waktu (Time-Series): *Trend, Seasonality, dan Noise*.
- Mahasiswa mampu memahami penyebab utama fenomena *Missing Packet*, *Jitter*, dan anomali pada lingkungan IoT (*IoT Data Anomalies*).

## 2. Pendahuluan & Konteks Industri
Seluruh perangkat IoT berbagi satu dimensi yang sama saat merekam data: **Waktu (Timestamp)**. Jika data perbankan berfokus pada "Siapa yang bertransaksi?", data IoT selalu berfokus pada "Kapan sebuah kondisi terjadi?". Data ini disebut *Time-Series*. Analisis tidak bisa dilakukan dengan mengacak urutan data, karena urutan waktu menyimpan pola penting yang merepresentasikan kondisi fisik lapangan.

## 3. Materi Inti (Komponen Time-Series)
Sebuah data *Time-Series* (misal grafik suhu selama 1 tahun) sebenarnya merupakan gabungan dari 3 komponen (Dekomposisi):

### A. Trend (Tren)
Kecenderungan pergerakan data dalam jangka panjang. Tren bisa naik (*uptrend*), turun (*downtrend*), atau stabil.
- *Contoh di IoT:* Peningkatan perlahan namun konstan pada suhu modul baterai yang menandakan baterai akan segera rusak (degradasi).

### B. Seasonality (Musiman/Siklis)
Pola yang berulang secara periodik dan teratur dalam rentang waktu tertentu (harian, mingguan, tahunan).
- *Contoh di IoT:* Grafik cahaya matahari (*Light Intensity*) selalu membentuk kurva bukit yang puncaknya ada di pukul 12.00 siang, dan datar (0) pada pukul 20.00 malam. Pola ini terus berulang setiap hari.

### C. Noise / Residual (Residu/Gangguan Acak)
Fluktuasi tak beraturan yang tersisa setelah *Trend* dan *Seasonality* dihilangkan. Ini adalah komponen acak akibat gangguan sinyal elektronika.
- *Contoh di IoT:* Lompatan nilai kelembaban tanah 0.5% ke atas dan ke bawah dalam hitungan detik akibat kualitas tembaga sensor yang buruk.

## 4. Anatomi Masalah Data di IoT
Mengapa data time-series IoT sulit diolah?
1. **Irregular Sampling Rate:** Sensor yang bertenaga surya akan sangat aktif mengirim data di siang hari (misal tiap 10 detik), namun menurunkan kecepatan pengiriman di malam hari (misal tiap 1 jam) untuk hemat daya. Ini membuat indeks waktu menjadi tidak beraturan (*irregular*).
2. **Missing Packet & Outage:** Ketika listrik di ladang mati selama 3 jam, maka ada "lubang" kosong pada grafik selama 3 jam (NaN). AI tidak bisa memproses nilai kosong, sehingga lubang ini harus di-"tambal" (*Imputation*).
3. **Sensor Drift:** Seiring berjalannya waktu, sensor yang tertanam di tanah kotorannya semakin menebal, membuat nilai bacaannya secara perlahan "bergeser" dari nilai aslinya. Analis data perlu melakukan deteksi *drift* ini untuk merekalibrasi ulang model Machine Learning.

## 5. Bahan Diskusi Kelas
1. Jika kita memiliki data "Pengunjung Toko Pintar (Smart Retail) berbasis Kamera IoT", sebutkan contoh *Trend*, *Seasonality*, dan *Noise* pada dataset tersebut!
2. Jika sebuah data suhu mengalami "lubang" (*missing values*) selama 2 detik versus selama 2 hari, apakah teknik penambalan (*imputation*) yang sebaiknya digunakan sama? Mengapa?

## 6. Referensi Spesifik
- Minteer, A. (2017). *Analytics for the Internet of Things (IoT)*. (Bab: Exploring IoT Data). Packt Publishing Ltd. (ISBN-13: 978-1787120730)  
- Lihat `Daftar_Referensi_Lengkap.md` di direktori utama untuk referensi selengkapnya.  
