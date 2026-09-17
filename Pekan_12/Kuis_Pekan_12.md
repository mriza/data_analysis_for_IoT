# Kuis Pekan 12: InfluxDB & Grafana

Jawablah pertanyaan-pertanyaan berikut dengan jelas, ringkas, dan berdasarkan penalaran analitik Anda!

1. Anda sedang merancang *database* untuk menampung data suhu dari 10.000 sensor yang masuk setiap detik. Jika Anda menggunakan sistem basis data relasional klasik seperti MySQL, analisis mengapa arsitektur tabel relasional (SQL) tersebut akan segera mengalami "hambatan kinerja" (*bottleneck*) luar biasa dibandingkan jika Anda menggunakan spesialis **Time-Series Database** (seperti InfluxDB)!

2. Dalam konsep struktur data di InfluxDB v2, kita mengenal hierarki `Organization` dan `Bucket`. Mengapa perancangan struktur penyimpanan data (seperti pemisahan *Bucket* khusus untuk 'Sensor Suhu' dan *Bucket* untuk 'Sensor Kelembaban') sangat krusial bagi kemudahan kueri (*query*) dan manajemen siklus hidup data (*Data Retention*) di kemudian hari?

3. Skrip Python pengirim data ke InfluxDB memerlukan *API Token* sebagai kunci verifikasi. Mengapa dalam praktik *software engineering* standar industri, menyematkan *Token* secara polos (teks telanjang) di dalam kode sumber (seperti `token="XYZ123"`) lalu mengunggahnya ke GitHub adalah sebuah kecerobohan keamanan (*security flaw*) yang sangat fatal, dan bagaimana sebaiknya token itu disimpan?

4. Pada saat mengeksekusi kode Python untuk memasukkan data, *Data Engineer* sering menggunakan tipe *write_options* berupa `SYNCHRONOUS` (Sinkron) atau `ASYNCHRONOUS` (Asinkron/Batching). Untuk skenario sensor IoT yang mengirimkan ribuan baris data per detik secara terus-menerus, mengapa pemilihan mode penulisan secara `ASYNCHRONOUS` atau *Batching* mutlak diwajibkan demi mencegah sistem operasi peladen (*server*) membeku?

5. Grafana dirancang untuk menjadi penampil data (*Visualization Tool*), bukan penyimpan data. Saat mengonfigurasi koneksi dari Grafana ke InfluxDB, analisislah proses komunikasi apa yang sebenarnya terjadi di balik layar (secara *network*) ketika Anda menekan tombol "Save & Test" pada menu *Data Source*!

6. Anda menggunakan bahasa **Flux** untuk memanggil (*querying*) data dari InfluxDB ke Grafana. Bandingkan secara konseptual pendekatan *Flux* (yang berfokus pada aliran data baris waktu) dengan bahasa kueri *SQL* tradisional. Mengapa *Flux* dianggap jauh lebih luwes (*flexible*) saat harus melakukan agregasi berbasis waktu (misal: "hitung rata-rata suhu per 15 menit terakhir")?

7. Di dalam Grafana, Anda menjejerkan panel *Time-Series* (grafik garis riwayat) dan panel *Gauge* (jarum ukur persentase tunggal). Dalam skenario kepanikan darurat, di mana panel peringatan di ruang kontrol berkedip merah, jelaskan secara psikologi kognitif operator mengapa panel tipe *Gauge* jauh lebih efektif menyelamatkan situasi dibandingkan panel tipe *Time-Series*!

8. Saat menyetel aturan *Alerting* (Sistem Peringatan) di Grafana, Anda mengonfigurasi peringatan agar memicu jika "suhu > 33C selama 1 menit". Evaluasi mengapa memberikan jeda prasyarat waktu ("selama 1 menit") jauh lebih profesional secara analitik dibandingkan memicu alarm secara instan seketika tepat saat suhu menyentuh 33C untuk pertama kalinya!

9. Integrasi peringatan Grafana ke *Contact Point* **Telegram Webhook** merupakan sebuah revolusi operasional di IoT. Secara arsitektur, jelaskan bagaimana mekanisme *Webhook* menghindarkan aplikasi Telegram di HP operator dari keharusan "bertanya/menarik data terus-menerus" (*polling*) ke server Grafana!

10. Modul menugaskan Anda memodifikasi skrip *Subscriber MQTT* agar langsung menyimpan datanya ke *InfluxDB*. Analisislah secara sistem mengapa arsitektur "Sensor $\rightarrow$ Broker MQTT $\rightarrow$ Subscriber Script Python $\rightarrow$ InfluxDB $\rightarrow$ Grafana" adalah topologi aliran pipa (*pipeline*) data yang sangat tangguh (*robust*) untuk diimplementasikan di industri perkebunan cerdas berskala besar!
