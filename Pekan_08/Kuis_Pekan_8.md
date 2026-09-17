---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Kuis Pekan 8: Evaluasi Tengah Semester (UTS) - Review Analitik Komprehensif"
---

# Kuis Pekan 8: Evaluasi Tengah Semester (UTS) - Review Analitik Komprehensif

Jawablah studi kasus komprehensif berikut dengan mengintegrasikan seluruh pemahaman Anda dari Pekan 1 hingga Pekan 7!

1. Anda mendesain sistem IoT untuk tambak ikan nila yang berlokasi di daerah pelosok dengan sinyal internet yang sering mati. Analisislah mengapa meletakkan logika pengendali pompa oksigen murni di tingkat *Cloud Analytics* adalah keputusan desain yang sangat berbahaya bagi keselamatan ikan, dan bagaimana arsitektur *Edge Analytics* memecahkan kerentanan (*vulnerability*) tersebut!

2. Ketika mengunduh data sensor cuaca mentah dari *Cloud*, Anda mendapati bahwa dari 10.000 baris data, terdapat sekitar 300 baris yang memiliki nilai suhu `NaN` secara acak. Jika Anda secara serampangan menghapus ke-300 baris tersebut (*drop missing values*), jelaskan secara detail kerusakan matematis apa yang akan dialami oleh struktur waktu (*time-series*) dataset Anda pada fase pemodelan *Machine Learning* nanti!

3. Evaluasi efektivitas metode penambalan data (Imputasi). Jika sensor angin mati total selama 1 menit (karena *reboot*), versus mati total selama 7 hari penuh (karena badai merusak pemancar), mengapa menambal (*imputing*) kasus pertama sangat dianjurkan, namun memaksa menambal kasus kedua dengan *Forward Fill* berpotensi menghancurkan objektivitas analisis bisnis secara menyeluruh?

4. Saat memvisualisasikan data sensor kelembaban, garis tren terlihat sangat kasar dan dipenuhi gerigi-gerigi tajam (*electrical noise*). Anda menerapkan teknik *Moving Average* dengan ukuran *window* yang sangat besar (contoh: rata-rata 1000 data terakhir). Analisislah fenomena "*lagging*" (keterlambatan sinyal) apa yang secara fisik akan terlihat pada grafik tersebut dibandingkan dengan waktu nyata di dunia nyata!

5. Dalam melakukan *Exploratory Data Analysis* (EDA) tingkat lanjut pada ratusan sensor secara bersamaan, matriks angka korelasi menjadi terlalu padat untuk dibaca manusia. Jelaskan secara konseptual bagaimana visualisasi **Heatmap** memecahkan masalah ini dan memandu seorang *Data Analyst* menemukan pasangan sensor yang bergerak berlawanan arah secara instan!

6. Anda memiliki sebuah model *Deep Learning* pengenal jenis gulma berukuran 50 MB, yang jelas tidak muat dimasukkan ke dalam RAM mikrokontroler murah. Jelaskan prinsip kerja teknologi **TinyML**, secara spesifik proses **Quantization**, yang secara ajaib mampu menekan ukuran model tersebut hingga menjadi di bawah 50 KB sehingga siap ditanam di mikrokontroler pinggir sawah!

7. Saat menganalisis data kualitas air sungai (pH), penggunaan nilai **Rata-rata (Mean)** harian seringkali menjadi "kebohongan statistik" jika pada pukul 12 siang terjadi tumpahan limbah kimia asam pekat sesaat yang terekam sensor, sebelum air kembali netral. Buktikan secara nalar mengapa perhitungan **Median** (Nilai Tengah) tidak akan banyak berubah dan gagal membunyikan alarm darurat pencemaran, sedangkan *Mean* akan lebih bereaksi terhadap kasus *outlier* nyata ini!

8. Pada protokol MQTT, Mosquitto dan RabbitMQ memfasilitasi peran sebagai perantara (*Broker*). Jika *Publisher* (sensor) terus mengirim data namun skrip *Subscriber* Python (Dashboard) Anda mati atau mengalami *crash*, jelaskan secara teoritis nasib aliran pesan IoT tersebut di dalam infrastruktur *Broker* sebelum akhirnya dibaca atau dibuang!

9. Untuk mendeteksi nilai tidak normal (*anomali*) dari sensor tanpa perlu menebak-nebak angka batasnya secara manual, *Data Engineer* modern menggunakan metode **Interquartile Range (IQR)**. Analisislah mengapa pendekatan berbasis penyebaran persentil data ini (kuartil) sangat kuat (*robust*) dalam memisahkan suhu kebun yang wajar dengan angka lonjakan palsu akibat korsleting perangkat!

10. Jika Anda diminta merekomendasikan tumpukan perangkat lunak (*software stack*) kepada perusahaan untuk keperluan eksplorasi dan pembersihan data sensor besar, mengapa kombinasi **Jupyter Notebook + Pandas + Matplotlib/Seaborn** selalu menjadi standar emas (industri) dibandingkan memproses CSV berukuran Gigabyte tersebut ke dalam Microsoft Excel?
