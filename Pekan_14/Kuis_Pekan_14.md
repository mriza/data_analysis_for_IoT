---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Kuis Pekan 14: Integrasi End-to-End (PjBL)"
---

# Kuis Pekan 14: Integrasi End-to-End (PjBL)

Jawablah pertanyaan-pertanyaan berikut dengan jelas, ringkas, dan berdasarkan penalaran analitik Anda!

1. Dalam arsitektur *End-to-End* IoT, seluruh komponen (*Broker MQTT, Script Python, InfluxDB, Grafana*) dituntut untuk beroperasi secara mandiri namun saling terhubung 24/7. Analisislah titik kerawanan utama (*Single Point of Failure*) dari arsitektur *pipeline* yang linier ini, dan apa yang terjadi pada *dashboard* jika *Script Python* sebagai jembatan (*Orchestrator*) tiba-tiba *crash* di tengah jalan!

2. Anda menggunakan pustaka `pickle` di Python untuk me-load model *Machine Learning* yang sudah dilatih sebelumnya ke dalam skrip `main_pipeline.py`. Jelaskan secara analitik mengapa melatih (*training*) ulang model kecerdasan buatan dari nol *setiap kali* aliran data sensor baru masuk adalah arsitektur yang sangat salah kaprah dan akan membunuh kinerja *server* Anda!

3. Praktikum memandu Anda untuk merancang logika pemrosesan (*Cleaning & ML*) secara **On-the-fly** (langsung memproses saat data lewat). Bandingkan keunggulan respons waktu skenario *On-the-fly* ini dengan skenario *Batch Processing* (menunggu data terkumpul selama 1 jam baru diproses) untuk sistem keselamatan seperti pencegah kelebihan tekanan uap boiler!

4. Skrip *Orchestrator* (`main_pipeline.py`) Anda bertugas mendengarkan (Subscribe) data dari MQTT, memprediksi anomali dengan model ML, lalu mengirimkannya ke InfluxDB. Mengapa operasi pendeteksian anomali ini harus diletakkan *di tengah-tengah* pipa, dan bukannya membiarkan data masuk ke InfluxDB dulu baru kemudian Grafana yang bertugas mencari anomalinya?

5. Hasil prediksi model *Isolation Forest* biasanya berupa nilai `1` (normal) dan `-1` (anomali). Dalam skema perancangan *database*, Anda diinstruksikan menyimpan angka `-1` ini bersama dengan kolom suhu dan kelembaban ke dalam InfluxDB. Analisislah keuntungan analitik jangka panjang (historis) dari tindakan menyimpan rekaman *log* hasil deteksi *error* (Label AI) secara permanen di basis data tersebut!

6. Anda menata pengaturan di Grafana: "Jika field 'status_anomali' bernilai -1, maka kirim pesan ke Telegram". Secara arsitektur perangkat lunak (*software architecture*), evaluasi mengapa pembagian tugas antara Skrip Python (yang bertugas *mendeteksi* bahaya secara matematis) dan Grafana (yang bertugas *menyiarkan/alerting* bahaya) merupakan penerapan prinsip *Separation of Concerns* (Pemisahan Tugas) yang sangat brilian!

7. Saat menjalankan skrip `main_pipeline.py` yang berjalan 24 jam nonstop di Ubuntu Server, *Data Engineer* sering menggunakan perintah `tmux` atau eksekusi *background* `nohup` alih-alih mengeksekusinya di terminal biasa. Jelaskan logika operasional jaringan (terkait sesi *remote SSH* Anda yang bisa putus kapan saja) di balik kewajiban menggunakan aplikasi-aplikasi manajemen latar belakang (*background manager*) tersebut!

8. Selama *pipeline* beroperasi, sensor tiba-tiba kehabisan baterai dan mulai mengirim data `NaN` atau angka-angka sampah (*garbage text*) alih-alih angka suhu. Jika Anda tidak memasang mekanisme proteksi *Data Cleaning* (seperti `try-except` di Python atau pengecekan tipe data) sebelum data tersebut menyentuh algoritma ML, malapetaka program apa yang akan langsung mematikan seluruh aliran pipa data Anda pada detik itu juga?

9. Pembangunan sistem terpadu (*End-to-End*) sangat bergantung pada stabilitas jaringan antar *container/service* (jika menggunakan *Docker* atau IP lokal). Apabila alamat IP dari InfluxDB berganti secara acak setelah *server restart*, analisislah kerepotan (*maintenance overhead*) apa yang harus Anda lakukan terhadap semua *Script Python* dan Grafana di dalam arsitektur yang masih menggunakan pendekatan "Alamat IP *Hardcoded*"!

10. Sebagai manajer sistem IoT, saat melakukan uji coba akhir (*System Integration Testing*), Anda memalsukan (menyuntikkan secara manual) pesan anomali ekstrem dari MQTT untuk melihat apakah pesan Telegram benar-benar masuk ke HP Anda. Jelaskan fungsi filosofis dari tahapan pengujian dari ujung ke ujung (*End-to-End Testing*) ini sebelum sistem benar-benar dipasrahkan kepada petani di ladang!
