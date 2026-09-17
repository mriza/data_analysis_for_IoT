# Kuis Pekan 5: Pra-pemrosesan Data (Cleaning, Imputation, Smoothing)

Jawablah pertanyaan-pertanyaan berikut dengan jelas dan ringkas!

1. Mengapa nilai rata-rata (*Mean*) sangat dihindari untuk menyimpulkan sebuah kondisi pada sensor IoT yang rentan mengalami *error* pembacaan ekstrem sesaat?

2. Jika diukur menggunakan perhitungan statistik, metrik apa yang jauh lebih tangguh (*robust*) terhadap gangguan *outlier* dibandingkan *Mean*?

3. Pada analisis *time-series*, mengapa kita tidak boleh sekadar menghapus baris data (menggunakan `.dropna()`) saat menemui ada nilai kosong (*Missing Value*) akibat koneksi sensor terputus sesaat?

4. Metode penambalan (*Imputation*) yang bekerja dengan cara meng-*copy* nilai sehat terakhir sebelum data terputus, lalu menempelkannya terus hingga data kembali normal, disebut dengan:

5. Secara visual, bagaimana bentuk garis yang dihasilkan oleh teknik **Interpolasi Linear** (*Linear Interpolation*) saat menambal sebuah lubang kosong pada grafik suhu?

6. Data asli sensor IoT (misalnya sensor kelistrikan/cahaya) sering bergerigi rapat dengan frekuensi tinggi akibat gangguan listrik mikro (*electrical noise*). Teknik untuk menghaluskan gerigi ini tanpa merusak tren utamanya dikenal dengan nama:

7. Apa konsekuensi atau kelemahan (trade-off) yang paling fatal jika kita menggunakan nilai batas jendela (*window size*) yang **terlalu besar** (misal *window*=100) saat melakukan *Moving Average* pada data kendali sistem *real-time* seperti rem otomatis?

8. Saat mendeteksi data suhu ruangan yang tiba-tiba menunjuk angka 80°C, pendekatan pra-pemrosesan yang dipraktikkan adalah mengubah angka "80" tersebut menjadi nilai kosong (`NaN`). Mengapa tidak langsung diubah menjadi angka 0?

9. Manakah di antara pernyataan berikut yang merangkum pepatah **"Garbage In, Garbage Out (GIGO)"** dalam arsitektur AI dan IoT?

10. Jika kita ingin menerapkan *Moving Average* setiap rentang 5 data pada sebuah DataFrame di Pandas, penulisan fungsi yang paling tepat adalah:
