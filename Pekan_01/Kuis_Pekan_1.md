# Kuis Pekan 1: Pengantar Analisis Data untuk IoT

Jawablah pertanyaan-pertanyaan berikut dengan jelas, ringkas, dan berdasarkan penalaran analitik Anda!

1. Anda ditugaskan mengelola 5000 sensor kelembaban tanah di sebuah area perkebunan kelapa sawit skala besar. Berdasarkan karakteristik **Volume** dalam data IoT, tantangan komputasi seperti apa yang akan segera Anda hadapi dalam minggu pertama implementasi, dan mengapa menggunakan aplikasi hamparan (*spreadsheet*) tradisional seperti Microsoft Excel tidak lagi memungkinkan untuk menganalisis data tersebut?

2. Konsep **Velocity** pada aliran data sensor menuntut pemrosesan *real-time*. Berikan satu contoh kasus di sektor pertanian cerdas (*Smart Farming*) di mana keterlambatan analisis data selama 5 menit saja dapat berakibat sangat fatal bagi hasil panen atau infrastruktur.

3. Di lingkungan pertanian luar ruangan (*open field*), data yang dikirimkan oleh sensor suhu seringkali dianggap sebagai "data kotor". Uraikan dua faktor lingkungan fisik utama yang berpotensi menyuntikkan data *outlier* (angka yang tiba-tiba melompat tidak wajar) ke dalam *database* Anda!

4. Konektivitas nirkabel (*wireless*) di wilayah pedesaan sangat rentan terhadap *packet loss*. Jika sebuah perangkat *gateway* LoRa gagal mengirimkan data selama 2 jam akibat cuaca buruk, bagaimana fenomena ini mempengaruhi integritas susunan data runtun waktu (*time-series*) yang Anda terima di *server* pusat?

5. Jelaskan perbedaan mendasar mengenai "tempat di mana data diolah" pada arsitektur **Edge Analytics** dibandingkan dengan **Cloud Analytics**!

6. Mikrokontroler (seperti ESP32) memiliki kapasitas memori dan daya komputasi yang sangat terbatas. Jika demikian, mengapa pendekatan *Edge Analytics* tetap sangat direkomendasikan untuk sistem peringatan dini (contoh: irigasi otomatis saat tanah sangat kering)?

7. Sebuah pos jaga di tengah lahan pertanian dilengkapi dengan perangkat *Mini PC* yang menerima ribuan data mentah dari *Edge*, kemudian menghitung rata-ratanya, dan hanya mengirimkan hasil rekapitulasi setiap 1 jam ke *Cloud* di Jakarta. Peran arsitektur penengah (*intermediary*) apakah yang sedang dijalankan oleh *Mini PC* tersebut, dan apa keuntungan utamanya terkait beban internet?

8. Anda ingin melatih sebuah model *Artificial Intelligence* (AI) menggunakan teknik *Deep Learning* yang sangat berat untuk memprediksi pola cuaca ekstrem berdasarkan 5 tahun riwayat cuaca. Lapisan komputasi manakah (Edge, Fog, atau Cloud) yang paling wajib dan logis untuk mengeksekusi tugas pelatihan (*training*) ini, serta jelaskan alasannya secara teknis!

9. Bandingkan dua skenario ini: (A) Sensor mengirimkan data setiap 1 detik ke *Cloud*, atau (B) Sensor mengumpulkan data selama 1 jam, mencari nilai rata-ratanya secara lokal, lalu mengirim 1 nilai ke *Cloud*. Ditinjau dari sudut pandang konsumsi daya baterai perangkat IoT jarak jauh, skenario manakah yang lebih menguntungkan dan mengapa?

10. Ketika Anda mendapati grafik suhu di *dashboard* melonjak drastis dari 30°C menjadi 95°C dalam satu detik, logika analitik awal apa yang harus Anda bangun untuk memverifikasi apakah telah terjadi "kebakaran nyata" di lokasi, atau sekadar "sensor yang korslet/rusak karena kepanasan"?
