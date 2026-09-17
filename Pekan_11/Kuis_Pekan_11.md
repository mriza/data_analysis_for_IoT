---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Kuis Pekan 11: Visualisasi Data IoT & Konsep Time-Series Database"
---

# Kuis Pekan 11: Visualisasi Data IoT & Konsep Time-Series Database

Jawablah pertanyaan-pertanyaan berikut dengan jelas, ringkas, dan berdasarkan penalaran analitik Anda!

1. Seorang *Data Analyst* membangun *dashboard* pemantauan mesin pabrik dan memasukkan 15 jenis grafik garis (*line chart*) yang saling bertumpuk rumit dalam satu layar, dengan alasan "agar semua data terlihat". Tinjau dari sudut pandang prinsip UI/UX *Visibilitas Operasional* industri, mengapa desain *dashboard* semacam ini justru akan mencelakakan operator mesin di lapangan?

2. Dalam desain antarmuka *dashboard* IoT (misal menggunakan Grafana), elemen visual *Single Stat* atau *Gauge* sangat direkomendasikan untuk diletakkan di posisi teratas halaman dibandingkan grafik historis yang memanjang. Apa logika operasional (*Actionable Insight*) di balik peletakan hierarki visual tersebut?

3. Penggunaan "Warna Berkonteks" adalah aturan wajib dalam *dashboard* kendali. Jika Anda mendesain panel pengawas level air bendungan, analisislah mengapa menggunakan warna dekoratif (seperti ungu, pink, atau campuran warna-warni yang estetis) sangat dilarang keras, dan skema pewarnaan seperti apa yang seharusnya mematuhi standar keselamatan (*Safety Standard*)!

4. Bayangkan Anda telah berhasil membangun algoritma *Machine Learning* yang mampu memprediksi dengan akurasi 99% bahwa traktor akan meledak dalam 6 jam. Namun model tersebut hanya menampilkan nilainya di layar laptop Anda di kantor. Jelaskan konsep **Actionable Insight** dan mengapa sehebat apa pun model prediktif Anda, ia akan bernilai "Nol Besar" tanpa integrasi ke *Early Warning System (EWS)*!

5. Dalam arsitektur *Early Warning System* (EWS), mekanisme peringatan sering dihubungkan menggunakan teknologi **Webhook**. Jelaskan secara logis bagaimana *Webhook* bekerja sebagai jembatan komunikasi proaktif antara sistem *Server IoT* yang mendeteksi anomali suhu dengan aplikasi *Telegram/WhatsApp* milik manajer kebun!

6. Evaluasi keefektifan peringatan (*alerting*). Sistem Anda mengirimkan *push notification* ke HP operator setiap 1 menit yang berbunyi "Suhu Normal 28C". Setelah seminggu, mesin benar-benar terbakar (Suhu 100C) namun operator mengabaikan HP-nya. Fenomena psikologis (*Alert Fatigue*) apa yang terjadi pada operator tersebut, dan bagaimana prinsip desain EWS yang benar untuk mencegah kelalaian maut ini?

7. Jika *dashboard* IoT sebuah rumah kaca (Greenhouse) hanya memperlihatkan riwayat suhu kemarin, *dashboard* tersebut disebut sekadar **Deskriptif**. Rancanglah logika tambahan apa yang perlu Anda suntikkan (bekerja sama dengan model ML) agar *dashboard* tersebut naik kelas menjadi **Preskriptif** (memberikan solusi)!

8. Anda menggunakan *Time-Series Database* (seperti InfluxDB) untuk menjadi sumber data *Dashboard* Grafana. Jelaskan mengapa arsitektur basis data relasional klasik (seperti MySQL) kurang cocok dan akan sangat membebani kinerja visualisasi *dashboard* Anda jika harus me-render jutaan titik data sensor dalam rentang waktu sekian detik!

9. Untuk memantau status hidup/mati (*ON/OFF*) pompa irigasi, tipe panel visualisasi Grafana apa (antara *Line Chart*, *Bar Gauge*, atau *State Timeline/Boolean Discrete*) yang paling rasional dan tidak membingungkan pengguna untuk mengamati pola hidup-matinya alat tersebut sepanjang hari? Berikan alasannya!

10. *Early Warning System* berbasis AI seringkali memiliki tingkat *False Positive* (Alarm Palsu). Jika sistem prediksi kebakaran hutan terlalu sensitif dan sering membunyikan sirine palsu, jelaskan kerugian sosial dan finansial nyata yang harus ditanggung oleh pihak pemadam kebakaran dan masyarakat, serta mengapa *tuning threshold* (ambang batas) ML adalah tugas berkelanjutan!
