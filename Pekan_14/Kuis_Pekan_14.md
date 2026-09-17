# Kuis Pekan 14: Integrasi End-to-End (PjBL)

Jawablah pertanyaan-pertanyaan berikut dengan jelas dan ringkas!

1. Apa yang dimaksud dengan integrasi sistem "End-to-End" dalam proyek arsitektur analitik IoT?

2. Pada arsitektur final proyek akhir, seluruh fungsi penerimaan, pembersihan, dan analisis data biasanya digabungkan ke dalam satu file pengendali utama. File pengendali alur otomatis ini sering disebut dengan istilah:

3. Berdasarkan alur logika program terintegrasi, tugas pertama dari sistem setelah program dijalankan adalah melakukan **MQTT Subscribe**. Apa arti dari proses ini?

4. Setelah paket data diterima dari MQTT, langkah kritis selanjutnya pada blok *Data Cleaning & ML (On-the-fly)* adalah memeriksa kekosongan data. Mengapa hal ini harus dilakukan sebelum data diserahkan ke *Machine Learning*?

5. Dalam tahap implementasi integrasi sistem, jika Anda sudah berhasil melatih (men-*training*) sebuah model kecerdasan buatan (*Isolation Forest*) di *Jupyter Notebook* kemarin, bagaimana cara memasukkan kepintaran model tersebut ke dalam *script* `main_pipeline.py` yang berjalan saat ini?

6. Jika model AI *Isolation Forest* yang diintegrasikan dalam *pipeline* memproses suhu baru dan mengeluarkan nilai output **-1**, keputusan apa yang sebenarnya diambil oleh AI?

7. Di lapisan arsitektur **Data Storage**, apa yang paling esensial untuk dikirimkan secara paralel bersama dengan data suhu dan kelembaban ke dalam *database Time-Series* (seperti InfluxDB)?

8. Mengapa penting untuk ikut menyimpan label status anomali ke dalam InfluxDB, dan tidak sekadar menampilkan peringatan sementara di layar *Terminal*?

9. Dalam sistem peringatan dini (EWS) otomatis, bagaimana cara Grafana mengirim peringatan ke *smartphone* seorang manajer kebun saat kondisi anomali terdeteksi secara *live*?

10. Seluruh konsep yang dikerjakan pada proyek akhir terpadu ini di dunia pendidikan dan industri dikenal dengan pendekatan "Project-Based Learning (PjBL)". Makna utama pendekatan ini bagi arsitektur IoT adalah:
