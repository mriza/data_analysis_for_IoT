---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Kuis Pekan 2: Instalasi dan Konfigurasi Message Broker"
---

# Kuis Pekan 2: Instalasi dan Konfigurasi Message Broker

Jawablah pertanyaan-pertanyaan berikut dengan jelas, ringkas, dan berdasarkan penalaran analitik Anda!

1. Anda sedang men-deploy sebuah *Virtual Machine* (VM) Ubuntu Server di dalam VirtualBox pada laptop (Windows) untuk simulasi *server* IoT. Jika Anda mengatur jaringan VM secara eksklusif hanya menggunakan mode **NAT**, mengapa *script Python Subscriber* di laptop Windows Anda dipastikan akan gagal (mendapatkan *Connection Refused*) saat mencoba terhubung ke broker MQTT di dalam VM? Jelaskan solusi arsitektur jaringan yang paling stabil untuk menyelesaikan masalah ini!

2. Dalam simulasi lingkungan IoT produksi, Anda memutuskan menggunakan **Bridged Adapter** pada VM server Anda. Namun, saat Anda mempresentasikan hasilnya di tempat lain (misal dari kampus pindah ke *cafe* terbuka), seluruh *script Edge Device* (sensor) tiba-tiba gagal mengirim data ke *server*. Analisis mengapa konfigurasi jaringan ini sangat rentan saat terjadi perpindahan lokasi fisik jaringan, dan bagaimana *Host-Only Adapter* dapat memitigasi risiko tersebut untuk keperluan pengembangan lokal!

3. Saat Anda mengelola server Ubuntu dan mencoba mengaksesnya menggunakan SSH dari Windows (`ssh praktikan@192.168.1.100`), mengapa praktik mengakses server secara *remote* (jarak jauh) melalui terminal lebih direkomendasikan dan efisien di industri dibandingkan mengoperasikan VM langsung dari antarmuka jendela VirtualBox?

4. Anda menginstal *library* Python global di Ubuntu Server dan mendapati banyak paket yang saling bertabrakan versinya (*dependency conflict*). Jika seorang *Data Engineer* menyarankan Anda menjalankan perintah `python -m venv env_iot`, jelaskan secara teknis apa dampak dan keuntungan dari tindakan ini terhadap isolasi ruang kerja analitik IoT Anda!

5. Sistem *Gateway* IoT Anda secara historis menggunakan **Mosquitto** pada port standar 1883. Hari ini, Anda menginstal **RabbitMQ** dan mengaktifkan *plugin* MQTT-nya tanpa mengubah konfigurasi apa pun. Mengapa *service* Mosquitto tiba-tiba mati dengan pesan *error* di `systemctl` dan tidak bisa merespons klien, serta apa langkah konfigurasi *port* yang logis agar keduanya bisa hidup berdampingan secara harmonis?

6. Perusahaan Anda meminta peningkatan keamanan sistem *Message Broker*. Jika Anda membiarkan konfigurasi `allow_anonymous true` tetap menyala pada file `custom.conf` di Mosquitto, apa ancaman siber spesifik (skenario terburuk) yang berisiko menimpa arsitektur aliran data sensor Anda?

7. Dalam arsitektur **RabbitMQ**, pesan tidak dikirimkan secara langsung dari *Publisher* ke antrean penerima. Terdapat komponen bernama **Exchange** dan **Queue**. Analisis secara ringkas fungsi penengah dari *Exchange* dan mengapa pendekatan "pemilahan" ini lebih unggul (untuk sistem kompleks) dibandingkan pola *publish-subscribe* sederhana!

8. Skrip *Publisher Python* Anda bertugas mengirim angka suhu setiap 2 detik. Jika pada baris inisialisasi `client = mqtt.Client()` Anda menamainya dengan ID `"SensorSuhu"`, dan tanpa sengaja ada mahasiswa lain di jaringan yang sama juga menjalankan skrip dengan ID `"SensorSuhu"`, apa fenomena tabrakan koneksi (*connection drop/flap*) yang akan terjadi pada sisi *Broker*?

9. Dalam logika pemrograman *Subscriber* MQTT menggunakan `paho-mqtt`, mengapa fungsi `client.loop_forever()` atau `client.loop_start()` mutlak dibutuhkan pada akhir baris program, dan apa yang akan terjadi pada kemampuan aplikasi dalam 'mendengarkan' data masuk jika fungsi tersebut dihilangkan?

10. Jika Anda menugaskan skrip Python *Subscriber* berlangganan pada topik `kebun/sensor/suhu`, namun alat fisik (mikrokontroler ESP32) dikonfigurasi untuk mempublikasikan datanya ke topik `Kebun/Sensor/Suhu` (dengan huruf kapital). Apa yang akan diamati oleh *Data Analyst* di layar monitornya dan apa prinsip utama protokol MQTT yang dilanggar di sini?
