---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Kuis Pekan 13: Arsitektur Sistem Terpadu & Proyek Akhir"
---

# Kuis Pekan 13: Arsitektur Sistem Terpadu & Proyek Akhir

Jawablah pertanyaan-pertanyaan berikut dengan jelas, ringkas, dan berdasarkan penalaran analitik Anda!

1. Arsitektur data IoT skala *Enterprise* tidak pernah menggunakan aplikasi tunggal (*Monolithic*), melainkan memecah tugasnya menggunakan prinsip *Microservices* (Sensor, Broker, DB, Analytics, Dashboard secara terpisah). Analisislah keuntungan paling utama dari pemisahan tugas ini (*decoupling*) ketika perusahaan ingin melakukan *upgrade* versi pada mesin *Database* tanpa harus menghentikan operasional perekaman data sensor!

2. Dalam tumpukan teknologi industri, Apache Kafka atau HiveMQ sering digunakan menggantikan Mosquitto biasa sebagai *Message Hub* (Broker). Mengapa pada sistem cerdas dengan jutaan sensor yang masuk setiap detik (*high-throughput*), penggunaan Apache Kafka dianggap sebagai fondasi infrastruktur yang jauh lebih tahan banting dan berskala enterprise?

3. Anda ditugaskan merancang *Analitik Engine* yang berjalan di atas klaster komputasi terdistribusi (seperti Apache Spark) alih-alih sekadar *script Python* biasa di satu *server*. Evaluasi kondisi volume data dan *workload* seperti apa yang mengharuskan Anda melakukan *upgrade* ekstrem dari sekadar *Python script* menjadi arsitektur komputasi terdistribusi (Spark) ini!

4. Proyek akhir Anda menuntut pembangunan arsitektur terpadu. Gambarkan dan jelaskan secara singkat alur perjalanan sepotong data suhu dari saat ia ditangkap secara fisik di lahan pertanian (*Edge*) hingga data tersebut memicu bunyi alarm di HP operator (*Alerting*), lengkap dengan menyebutkan nama tumpukan *software* (Stack) pada setiap lapisannya!

5. Mengapa pemilihan dataset untuk *Machine Learning* pada rancangan Proyek Akhir sangat memengaruhi jenis algoritma prapemrosesan (*Pre-processing*) yang harus Anda koding nantinya? (Berikan contoh: dataset cuaca terbuka vs dataset getaran mesin yang dipenuhi *outlier*).

6. Sebuah sistem IoT Enterprise sukses mengalirkan 500 MB data per detik ke InfluxDB, namun saat pengguna membuka Grafana, *dashboard*-nya *loading* sangat lambat hingga *crash*. Analisis kesalahan arsitektur logika *query* seperti apa (antara InfluxDB dan Grafana) yang biasanya menjadi biang keladi kebuntuan (*bottleneck*) visualisasi ini, dan apa fungsi dari *Data Downsampling* (Agregasi) untuk menyelesaikannya!

7. Jika Anda harus mempresentasikan rancangan Proyek Akhir IoT di hadapan pemodal (*Investor*), analisislah mengapa Anda tidak boleh hanya menjual metrik "akurasi model AI 98%", melainkan wajib menekankan pada metrik bisnis terkait "Seberapa cepat Sistem Notifikasi (*Alerting System*) Anda mencegah kerugian finansial"!

8. Dalam fase Pra-pemrosesan Data di Proyek Akhir, Anda memutuskan menggunakan metode interpolasi *Spline* (bukan *Linear*) untuk menambal data sensor gelombang pasang air laut yang hilang. Apa alasan logis dari pemilihan metode interpolasi melengkung (*Spline*) tersebut jika dikaitkan dengan hukum fisika pergerakan air laut yang alami?

9. Sebagai arsitek sistem, evaluasi keamanan siber dari arsitektur *pipeline* Anda. Jika sebuah kelompok peretas (hacker) berhasil mengetahui nomor port dan IP dari *Message Broker* Anda, strategi pengamanan ganda apa yang seharusnya sudah Anda pasang (mulai dari *Edge* hingga *Broker*) untuk memastikan mereka tidak bisa menyuntikkan data sensor palsu?

10. Sistem notifikasi/peringatan (EWS) pada Proyek Akhir IoT sering dihubungkan menggunakan layanan pihak ketiga seperti *Telegram Bot API*. Jelaskan kerentanan tunggal (*Single Point of Failure*) dari arsitektur ketergantungan pihak ketiga ini, dan bagaimana prosedur *fallback* (cadangan) lokal (contoh: sirine fisik) memitigasi risiko saat API Telegram sedang *down*!
