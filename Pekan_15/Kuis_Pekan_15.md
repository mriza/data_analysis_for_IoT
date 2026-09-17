# Kuis Pekan 15: Presentasi Hasil dan Demonstrasi Proyek Akhir

Jawablah pertanyaan-pertanyaan studi kasus komprehensif berikut dengan mengintegrasikan seluruh pemahaman *Data Storytelling* dan Arsitektur Sistem Anda!

1. Dalam sesi demonstrasi (*Live Demo*), kelompok Anda berhasil menampilkan grafik yang sangat estetis dan kompleks di Grafana, namun dosen pengampu mengkritik bahwa *dashboard* Anda "gagal menyampaikan cerita". Jelaskan secara filosofis apa perbedaan mendasar antara sekadar "Menampilkan Data" (*Data Visualization*) dengan "Bercerita dengan Data" (*Data Storytelling*) di hadapan pengambil keputusan!

2. Anda mempresentasikan fitur *Early Warning System* (EWS) yang akan membunyikan alarm ke Telegram jika kelembaban tanah turun di bawah 30%. Saat ditanya "Mengapa angka 30% yang dipilih?", jelaskan dari mana seorang *Data Analyst* seharusnya menggali argumentasi berbasis data (*data-driven*) untuk mempertahankan angka ambang batas (*threshold*) tersebut, alih-alih menjawab "hanya tebakan kami"!

3. Selama pengujian keseluruhan sistem (*End-to-End Testing*), Anda memutuskan untuk mencabut paksa kabel internet pada perangkat *Edge* Anda selama 5 menit. Analisislah apa yang ingin Anda buktikan (terkait penanganan *Missing Values* dan ketangguhan *script* Python Anda) melalui tindakan sabotase mandiri tersebut di depan para penguji!

4. Saat menjelaskan model *Machine Learning* Anda, audiens bisnis (seperti manajer atau investor non-IT) mulai kebingungan dengan istilah *Root Mean Squared Error* (RMSE) atau *Isolation Forest*. Rancang ulang narasi komunikasi teknis Anda: bagaimana cara Anda menjelaskan keberhasilan model kecerdasan buatan tersebut menggunakan metrik bahasa bisnis (*Business Value/Impact*)?

5. *Dashboard* Grafana kelompok Anda mengalami '*lag*' dan melambat drastis saat *Live Demo* karena secara tak sengaja menarik jutaan baris data dari InfluxDB selama setahun terakhir tanpa difilter. Jelaskan teknik pengaturan kueri basis data (*Query Optimization/Data Downsampling*) apa yang seharusnya Anda persiapkan sebelum sesi presentasi agar grafik tetap *real-time* dan ringan!

6. Dalam sebuah presentasi proyek analisis kegagalan dinamo mesin (*Predictive Maintenance*), mengapa sangat krusial bagi kelompok Anda untuk tidak sekadar menampilkan visualisasi "Mesin Rusak pada Hari H", melainkan harus mampu menunjukkan korelasi multidimensi antara getaran (*vibration*) dan suhu dinamo pada hari *H-3* sebelum kerusakan terjadi?

7. Jika algoritma pendeteksi anomali Anda ternyata memiliki tingkat *False Positive* (alarm palsu) yang masih tergolong tinggi, sikap paling profesional dalam presentasi adalah mengakuinya. Jelaskan argumentasi *Data Science* yang dapat Anda sampaikan kepada penguji mengenai mengapa mengkalibrasi ulang model ML adalah tugas iteratif yang tidak pernah benar-benar selesai!

8. Saat mempresentasikan arsitektur keamanan IoT, dosen menguji pemahaman Anda: "Bagaimana jika ada orang yang mengetahui *IP Address* dan topik MQTT Anda, lalu mengirimkan data suhu palsu (999°C) dari luar gedung?". Kemukakan pertahanan minimum (*Security Authentication*) apa yang sudah Anda terapkan pada Mosquitto/RabbitMQ untuk menangkal serangan sabotase data ini!

9. Evaluasi *Trade-off*: Dalam proyek Anda, komputasi *Machine Learning* dilakukan di dalam *script Python* di *Cloud*, bukan di perangkat *Edge*. Jelaskan kelemahan (misal: ketergantungan internet) sekaligus kelebihan (misal: keleluasaan kapasitas RAM) dari keputusan desain arsitektur yang Anda ambil tersebut!

10. *Goal* akhir dari ilmu *Data Analytics* bukanlah laporannya, melainkan tindakan (*Action*). Berikan satu contoh konkret dari proyek IoT agrikultur di mana hasil prediksi kecerdasan buatan langsung dieksekusi secara otonom (Otomatisasi Aktuator) tanpa perlu menunggu manusia membaca *Dashboard* Grafana tersebut terlebih dahulu!
