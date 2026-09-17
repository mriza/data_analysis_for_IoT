---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Teori Pekan 4: Membaca Data & Exploratory Data Analysis (EDA)"
---

# Teori Pekan 4: Membaca Data & Exploratory Data Analysis (EDA)

## 1. Apa itu Exploratory Data Analysis (EDA)?
Bayangkan Anda baru saja menerima sebuah kotak besar berisi ribuan laporan cuaca yang ditulis tangan. Apa hal pertama yang Anda lakukan sebelum membuat kesimpulan? Anda pasti akan menyortir kertasnya, melihat apakah ada tulisan yang tidak terbaca, dan mencari tahu angka tertinggi atau terendah. 

**Exploratory Data Analysis (EDA)** adalah versi digital dari proses "berkenalan" dengan data tersebut. EDA adalah langkah pertama dan paling krusial dalam analisis data sebelum Anda menerapkan algoritma *Machine Learning* atau membuat *dashboard* keputusan. 

Tujuan utama dari EDA adalah:
1. **Memahami struktur data:** Mengetahui jumlah baris (sampel) dan kolom (fitur/variabel).
2. **Mendeteksi anomali (*Outliers*):** Apakah ada sensor suhu yang tidak masuk akal mencatat suhu 1000°C? 
3. **Mengidentifikasi data yang hilang (*Missing Values*):** Apakah sensor pernah mati sesaat sehingga datanya kosong (`NaN`)?
4. **Menemukan pola dasar:** Melihat tren naik-turun dari data *time-series*.
5. **Menguji hipotesis awal:** Misalnya, mencari tahu apakah kelembaban akan selalu turun drastis ketika suhu udara naik tajam.

Dalam IoT, data mentah dari sensor seringkali sangat "kotor" (banyak gangguan sinyal, nilai yang hilang akibat koneksi terputus). Oleh karena itu, EDA memastikan bahwa data yang akan kita analisis lebih lanjut sudah valid dan bisa dipercaya.

## 2. Tiga Pilar EDA: Deskripsi, Deteksi, dan Visualisasi
Secara praktik, EDA biasanya mencakup tiga pilar utama:
- **Statistik Deskriptif:** Menggunakan fungsi matematika sederhana untuk mendeskripsikan data, seperti nilai rata-rata (*Mean*), nilai tengah (*Median*), nilai kemunculan terbanyak (*Mode*), nilai minimum, maksimum, dan sebaran data (*Standard Deviation*).
- **Pembersihan Awal (Deteksi Kotoran):** Mengecek jumlah nilai yang hilang (*null/NaN*).
- **Visualisasi Data:** Menerjemahkan jutaan angka menjadi grafik yang mudah dipahami oleh mata manusia.

## 3. Pustaka Python "Wajib" untuk EDA
Untuk melakukan EDA secara profesional di Python, kita memanfaatkan kombinasi tiga pustaka *library* utama:
- **Pandas (`import pandas as pd`)**: Ibarat "Microsoft Excel" versi baris kode. Pandas mampu membaca berbagai format (CSV, SQL, Excel) ke dalam format tabel dua dimensi yang disebut **DataFrame**. Pandas mempermudah kita menghitung rata-rata, mencari data kosong, hingga menyaring data.
- **Matplotlib (`import matplotlib.pyplot as plt`)**: Mesin penggambar grafik tingkat dasar. Anda mengendalikan segalanya: mulai dari ukuran kanvas, ketebalan garis, hingga bentuk titik koordinat.
- **Seaborn (`import seaborn as sns`)**: Dibangun di atas Matplotlib, Seaborn mengotomatisasi pembuatan grafik statistik kompleks (seperti *heatmap*, *boxplot*, atau diagram distribusi) dengan desain yang secara bawaan (*default*) lebih elegan dan modern.

## 4. Anatomi Data Time-Series
Sebagian besar data IoT adalah data runtun waktu (*time-series*). Berbeda dengan data presensi mahasiswa yang bisa diacak urutannya, data suhu harian **tidak boleh diacak**. Urutan waktu adalah inti dari maknanya.

Karakteristik penting saat menangani *time-series* di Pandas:
- **Konversi Tipe Waktu (*Parsing*)**: Teks `"2023-10-01 12:00:00"` awalnya hanya dianggap sebagai kumpulan huruf (teks/*string*) oleh komputer. Kita harus memerintahkan Pandas untuk membaca teks tersebut sebagai *objek waktu* (`datetime`) agar komputer mengerti bahwa jam 13:00 lebih lambat dari jam 12:00. Ini dilakukan melalui argumen `parse_dates`.
- **Index Waktu (*DatetimeIndex*)**: Pada tabel biasa, nomor baris adalah 0, 1, 2, dst. Pada data runtun waktu, kita sebaiknya mengubah kolom Waktu menjadi penunjuk baris utama (*index*). Tujuannya agar kita bisa melakukan operasi khusus seperti "hitung rata-rata per pekan" (*resampling*) atau memotong rentang waktu tertentu dengan jauh lebih cepat.

## 5. Analisis Korelasi dan Heatmap
Salah satu pertanyaan penting dalam IoT adalah bagaimana sebuah sensor dipengaruhi oleh fenomena dari sensor lainnya. Ini dijawab oleh **Korelasi**. Korelasi (Pearson) adalah angka matematis yang bergerak dari rentang -1 hingga 1.

- **Korelasi Positif (Mendekati 1):** Kedua variabel bergerak searah. Contoh: Semakin tinggi radiasi cahaya matahari (Lux), suhu (Celcius) juga semakin tinggi.
- **Korelasi Negatif (Mendekati -1):** Kedua variabel berbanding terbalik. Contoh: Semakin tinggi suhu, tingkat kelembaban tanah (*Soil Moisture*) biasanya menurun drastis.
- **Tidak Berkorelasi (Mendekati 0):** Variabel bergerak tanpa mempengaruhi satu sama lain. Contoh: Kecepatan putaran baling-baling kipas angin di ruangan (RPM) dengan tingkat keasaman tanah luar ruangan (pH) tidak memiliki hubungan logis.

**Heatmap (Peta Panas)** mengubah tabel silang angka korelasi ini menjadi blok-blok warna yang mencolok. Warna merah tua untuk hubungan positif yang sangat kuat, dan warna biru/gelap untuk hubungan negatif yang kuat, sehingga Anda tidak perlu pusing menelusuri lautan angka tabel.
