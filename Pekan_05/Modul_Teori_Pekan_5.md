# Materi Kuliah Pekan 5: Statistik Deskriptif dan Pra-pemrosesan Data (Pre-processing)

## 1. Tujuan Pembelajaran Khusus
- Mahasiswa memahami secara mendalam batasan perhitungan statistik dasar (Mean) terhadap keberadaan anomali (*Outlier*) pada data sensor.
- Mahasiswa menguasai berbagai konsep dan teknik imputasi untuk mengisi kekosongan data (*Missing Values*) pada data deret waktu (*time-series*).
- Mahasiswa mampu menjelaskan perbedaan teknik *Smoothing* (penghalusan) menggunakan *Moving Average* untuk mengatasi *noise* frekuensi tinggi.
- Mahasiswa menyadari betapa fatalnya dampak mengabaikan pra-pemrosesan data dalam arsitektur sistem analitik IoT.

## 2. Pendahuluan & Konteks Industri
Pepatah paling fundamental dan abadi dalam dunia Data Science dan rekayasa perangkat lunak adalah **"Garbage In, Garbage Out" (GIGO)**. Sehebat apapun algoritma *Machine Learning* yang Anda bangun (bahkan jika Anda menggunakan arsitektur *Deep Learning* mutakhir dari Google atau OpenAI), jika data historis sensor yang Anda masukkan (sebagai data latih/ *training data*) berisikan angka-angka *error*, lompatan nilai tak masuk akal, atau periode kosong yang panjang, model tersebut pasti akan menghasilkan prediksi yang salah besar. 

Pra-pemrosesan (*Data Pre-processing*) seringkali memakan 70% hingga 80% dari total waktu kerja seorang *Data Analyst* atau *Data Engineer*. Di lingkungan IoT dunia nyata, sensor beroperasi 24/7 di luar ruangan. Sensor terkena hujan, debu, pemadaman listrik lokal, gangguan sinyal radio, hingga keausan fisik. Akibatnya, aliran data IoT hampir tidak pernah sempurna. Tugas kita adalah "membersihkan" dan merekonstruksi data tersebut agar layak dianalisis.

## 3. Materi Inti: Memahami Statistik Deskriptif vs Realita Sensor

### 3.1 Mean, Median, dan Rentang (Range)
Seringkali, kita hanya bergantung pada nilai rata-rata (Mean) untuk menyimpulkan sebuah data. Namun dalam IoT, Mean bisa menjadi kebohongan besar jika ada *outlier*.
- **Mean (Rata-rata):** Penjumlahan seluruh data dibagi jumlah data. 
  - *Kelemahan:* Sangat sensitif terhadap satu titik ekstrim (*outlier*). 
  - *Contoh:* Anda mengukur suhu ruangan selama 5 detik: 30°C, 31°C, 30°C, 32°C, lalu tiba-tiba karena korsleting sensor mencatat angka 1000°C. Nilai Rata-rata = (30+31+30+32+1000) / 5 = **224.6°C**. Jika sistem pendingin ruangan bergantung pada Mean, AC akan langsung bekerja maksimal hingga membekukan ruangan, padahal suhu sebenarnya masih sekitar 30°C.
- **Median:** Nilai tengah dari kumpulan data yang diurutkan dari terkecil ke terbesar. 
  - *Kekuatan:* Jauh lebih tangguh (*robust*) terhadap *outlier*. 
  - *Dari contoh di atas:* Urutannya menjadi 30, 30, **31**, 32, 1000. Median = **31°C**. Nilai ini jauh lebih masuk akal dan merepresentasikan kondisi ruangan yang sebenarnya.
- **Standar Deviasi ($\sigma$):** Mengukur seberapa jauh, rata-rata, setiap nilai menyimpang dari *Mean*. Jika standar deviasi pembacaan sensor kelembaban sangat kecil (mendekati 0), artinya kondisi tanah stabil tanpa penguapan mendadak. Jika sangat besar, berarti ada fluktuasi kelembaban yang sangat ekstrim.

### 3.2 Menangani Missing Values pada Data Time-Series
Data IoT yang bolong (kosong) tidak bisa sekadar di-*drop* (dihapus barisnya) begitu saja seperti data tabular biasa (misal: data demografi penduduk). Mengapa? Karena analisis data *time-series* (seperti algoritma prediksi peramalan, FFT, dll) membutuhkan **jarak waktu (*timestamp*) yang berurutan secara konstan**. Jika ada baris waktu yang dihapus, algoritma akan kebingungan menghadapi jeda waktu tersebut.

Metode *Imputation* (Penambalan/Pengisian) adalah solusinya:
1. **Forward-Fill (ffill):** Menambal nilai kosong dengan cara meng-*copy* nilai terakhir yang sehat sebelum data terputus. Asumsinya: "Jika sensor mati di detik ke-5, anggap saja suhu di detik ke-6, 7, 8 sama persis dengan detik ke-4". Cocok untuk pemutusan jaringan yang sangat singkat (beberapa detik/menit).
2. **Backward-Fill (bfill):** Menambal nilai kosong dengan menggunakan nilai sehat *setelah* koneksi kembali tersambung.
3. **Linear Interpolation:** Menggambar (menarik) garis lurus secara diagonal dari titik data sebelum terputus hingga ke titik data setelah tersambung kembali. Cocok jika kita yakin variabel tersebut berubah secara perlahan dan konstan (misal: penurunan suhu ruangan secara konstan saat sore hari).
4. **Polynomial/Spline Interpolation:** Serupa dengan linear, namun menggunakan kurva matematika melengkung yang mengikuti pola tren data historis. Sangat akurat namun membutuhkan komputasi lebih.

### 3.3 Menangani Noise dengan Smoothing (Penghalusan Data)
Sinyal analog dari komponen perangkat keras sensor (misalnya sensor cahaya atau kelembaban) sering mengalami lonjakan voltase mikro (*electrical noise*) yang menyebabkan grafik pembacaan tampak "berambut" atau bergerigi dengan frekuensi tinggi. 

**Moving Average (Rata-rata Bergerak / Rolling Mean)** adalah teknik menggeser sebuah "kotak rentang waktu" (*window*) di sepanjang barisan data, dan mengambil nilai rata-rata pada setiap langkah gesernya. 
- Jika panjang *window* = 5, setiap titik data baru pada grafik adalah hasil rata-rata dari 5 pengukuran (detik) sebelumnya. 
- *Efek visual:* Ini akan memangkas duri-duri tajam (*spikes*) pada grafik akibat *electrical noise*, menjadikannya kurva yang mulus (*smooth*) tanpa menghilangkan arah pergerakan tren utamanya.
- *Trade-off (Risiko):* Semakin besar ukuran batas *window* (misal: 100 detik), grafik akan semakin mulus sempurna, namun pergerakan grafik akan **mengalami penundaan waktu (*lag*)** yang signifikan dari kondisi aktual di lapangan (karena titik datanya terlalu banyak merepresentasikan kejadian masa lalu).

## 4. Studi Kasus Industri
**Insiden Kalibrasi Sensor Kualitas Air (pH)**
Sebuah sensor IoT tambak udang pintar (*smart aquaculture*) mengirim aliran data tingkat keasaman air (pH): `7.0`, `7.1`, `7.0`, `1.2`, `7.1`, `7.0`.
Tiba-tiba ada 1 kali pembacaan data yang anjlok di angka `1.2` (Sangat Asam). Secara ilmu fisika maupun biologi, pH air tambak bervolume ribuan liter **tidak mungkin** anjlok drastis dari 7 ke 1 hanya dalam waktu 2 detik. 

Ini murni *error* pembacaan voltase elektronika sensor (*Hard Outlier*). Jika *Data Engineer* tidak menanamkan filter Median atau tahap pra-pemrosesan untuk membuang anomali ini, maka sistem kendali aktuator cairan basa akan otomatis menembakkan cairan kimia gila-gilaan ke dalam kolam karena mengira kolam sedang terkontaminasi asam. Hasilnya? Air menjadi ekstrem basa dan membunuh seluruh panen udang secara nyata.

## 5. Bahan Diskusi Kelas
1. Jika sebuah sensor suhu udara luar ruangan (BMKG) tiba-tiba terputus koneksinya secara total selama 3 hari berturut-turut akibat petir, metode *imputation* manakah (Forward-fill, Interpolasi, atau dirata-rata) yang paling pantas digunakan untuk menambal data 3 hari tersebut? Ataukah tidak ada yang pantas? Apa rasionalisasi dan risikonya?
2. Dalam rancang bangun sistem kendali pengereman otomatis (menggunakan *proximity sensor* jarak), apa bahaya fisik maut dari menggunakan ukuran perataan data *Moving Average* yang terlalu besar (misalnya mengambil rata-rata 100 pengukuran beruntun)?

## 6. Referensi Spesifik
- McKinney, W. (2022). *Python for Data Analysis: Data Wrangling with Pandas, NumPy, and Jupyter* (3rd ed.). O'Reilly Media. (ISBN-13: 978-1098104030)
- Dokumentasi Pandas Resmi: 
  - Menangani data kosong: `pandas.DataFrame.fillna`, `pandas.DataFrame.interpolate`
  - Windowing operations (Rolling filter): `pandas.DataFrame.rolling`
- Lihat `Daftar_Referensi_Lengkap.md` di direktori utama untuk referensi selengkapnya.
