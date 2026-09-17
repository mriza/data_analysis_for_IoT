---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Kuis Pekan 5: Statistik Deskriptif dan Pra-pemrosesan Data (Pre-processing)"
---

# Kuis Pekan 5: Statistik Deskriptif dan Pra-pemrosesan Data (Pre-processing)

Jawablah pertanyaan-pertanyaan berikut dengan jelas, ringkas, dan berdasarkan penalaran analitik Anda!

1. Mengapa semboyan *Garbage In, Garbage Out* (GIGO) menjadi prinsip paling ditakuti oleh seorang *Data Scientist* yang membangun model *Machine Learning* untuk Internet of Things? Jelaskan dampaknya jika tahap pra-pemrosesan (*pre-processing*) dilewati!

2. Anda memiliki deretan pembacaan sensor kelembaban ruangan (dalam persen): 60, 61, 59, 60, 62, lalu akibat adanya *glitch* perangkat keras, satu pembacaan melonjak menjadi 5000. Analisislah secara matematis mengapa menggunakan fungsi **Rata-rata (Mean)** untuk membuat laporan harian pada skenario ini sangat menyesatkan, dan mengapa **Median** adalah metrik yang jauh lebih kebal (*robust*) terhadap anomali ekstrem tersebut!

3. Pada data tabular seperti tabel sensus penduduk, jika kita menemukan satu baris data kosong, kita bisa dengan mudah menghapusnya (*drop row*) tanpa merusak analisis. Namun, jika kita melakukan *drop row* pada data sensor *Time-Series*, hal itu dilarang keras. Mengapa penghapusan baris dapat menghancurkan validitas model prediksi data runtun waktu?

4. Jika *gateway* sensor cuaca mengalami *restart* otomatis dan menyebabkan nilai data kosong (`NaN`) selama tepat 3 detik, evaluasi mengapa metode penambalan **Forward-Fill (ffill)** merupakan pendekatan yang sangat masuk akal dan aman secara analitik untuk durasi pemutusan singkat ini!

5. Bandingkan metode penambalan *Forward-Fill* dengan **Linear Interpolation**. Jika data suhu mengalami kekosongan selama 15 menit dari jam 06.00 hingga 06.15 (periode di mana matahari terbit perlahan), mengapa *Linear Interpolation* memberikan hasil rekonstruksi data yang lebih merepresentasikan kejadian fisik di dunia nyata dibandingkan sekadar mengkopi nilai (*ffill*)?

6. Metode penambalan data (imputasi) tidak boleh digunakan secara membabi buta. Jika sensor pH air tambak mati total selama 3 hari berturut-turut, paparkan secara kritis mengapa menambal seluruh kekosongan 3 hari tersebut menggunakan metode *Linear Interpolation* atau *Forward Fill* berpotensi menghasilkan wawasan fiktif yang sangat berbahaya!

7. Grafik garis dari sebuah mikrokontroler yang membaca intensitas cahaya tampak sangat "berambut" atau bergerigi berantakan (*electrical noise*) setiap detiknya. Jika analis data ingin meredam gerigi ini agar grafiknya lebih mulus untuk dipresentasikan tanpa merusak bentuk kurva besarnya, fungsi operasi dasar Pandas apakah yang harus diaplikasikan?

8. Anda menerapkan fungsi **Moving Average** (*Rolling Mean*) dengan ukuran jendela (*window*) sebesar 300 data (5 menit) pada sistem pengereman otomatis robot pengantar pupuk. Jelaskan *trade-off* atau kelemahan fisik/mekanis maut apa yang akan ditimbulkan oleh ukuran parameter *window* yang terlalu besar pada skenario waktu-nyata (*real-time*) tersebut!

9. Pada sebuah tambak udang cerdas (*smart aquaculture*), nilai sensor pH tiba-tiba melompat dari 7.0 menjadi 1.2 selama 1 detik, lalu kembali ke 7.0. Jika *Data Engineer* gagal mendeteksi nilai 1.2 ini sebagai *Hard Outlier*, jelaskan secara runtut bagaimana rantai kesalahan data sekecil ini bisa memicu aktuator cerdas untuk membunuh seluruh populasi udang di tambak!

10. Dari keseluruhan teknik pra-pemrosesan (perhitungan *Median*, Penambalan/Imputasi, dan Penghalusan/*Smoothing*), sebutkan satu contoh bagaimana penerapan perpaduan teknik-teknik tersebut pada level *Edge Analytics* (di dalam *mikrokontroler* sebelum dikirim ke *Cloud*) dapat menghemat *bandwidth* kuota internet perusahaan secara signifikan!
