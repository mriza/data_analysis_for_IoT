---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Kuliah Pekan 9: Paradigma Machine Learning vs Pemrograman Tradisional"
---

# Materi Kuliah Pekan 9: Paradigma Machine Learning vs Pemrograman Tradisional

## 1. Tujuan Pembelajaran Khusus
- Mahasiswa mampu membedakan logika Pemrograman Berbasis Aturan (*Rule-based*) dan Pembelajaran Mesin (*Machine Learning*).
- Mahasiswa mengerti proses penentuan formulasi masalah ML pada IoT (*Problem Formulation*).
- Mahasiswa mampu merangkum alur prediksi peramalan Time-Series.

## 2. Pendahuluan & Konteks Industri
Selama puluhan tahun, sistem otomasi industri bekerja menggunakan perintah kondisional baku: *Jika A, maka lakukan B*. Ini disebut aturan terprogram (*hard-coded rules*).
Masalahnya pada pertanian presisi, pertumbuhan daun tidak sesederhana "jika suhu > 30 derajat, hidupkan kipas". Pertumbuhan daun bergantung pada kombinasi kompleks antara kelembaban absolut, radiasi sinar inframerah, kecepatan angin, dan pH tanah. Menulis aturan (IF-ELSE) untuk ribuan kombinasi variabel adalah hal mustahil bagi manusia. Kita membutuhkan mesin yang mencari aturannya secara otomatis dari data. Mesin itu disebut **Machine Learning (ML)**.

## 3. Materi Inti
### 3.1 Pemrograman Tradisional vs Machine Learning
1. **Pemrograman Tradisional (Sistem Pakar / Expert System)**
   - *Input:* Data Sensor (Kelembaban 40%) + Aturan (*Rules* yang diketik manusia: `IF kelembaban < 50 THEN STATUS='KERING'`).
   - *Proses Eksekusi pada CPU.*
   - *Output:* Jawaban yang baku ("KERING").
2. **Machine Learning**
   - *Input pada fase Pelatihan (Training):* Data Historis Sensor Masa Lalu + Jawaban yang sudah terjadi (contoh: log kapan petani menyiram air).
   - *Proses Algoritma Pembelajaran.*
   - *Output:* Komputer melahirkan model *Aturan Matematis* (Model).
   - Setelah Model jadi, barulah kita memasukkan "Data Sensor Baru" ke dalam model untuk memprediksi "Jawaban" yang belum terjadi.

### 3.2 Time-Series Forecasting (Peramalan)
Jika kita hanya ingin memprediksi nilai sensor di masa depan, kita menggunakan *Forecasting*.
*Forecasting* IoT mengasumsikan bahwa **"Pola di masa depan berulang dari pola di masa lalu"**.
Algoritma prediksi (seperti Regresi Linier atau Jaringan Saraf Tiruan LSTM) akan melihat fluktuasi kelembaban 3 hari sebelumnya, lalu menebak berapa persen kelembaban besok siang.

### 3.3 Deteksi Anomali Tingkat Lanjut (Unsupervised)
Bayangkan ada sebuah *Outlier* di mana suhu normal, angin normal, namun tanaman mati beramai-ramai. Parameter lingkungan tampak biasa saja bagi statistik dasar Z-score, tapi interaksi antar parameternya keliru.
Algoritma **Unsupervised Learning** (mesin yang belajar tanpa diberi label "benar/salah"), seperti **Isolation Forest** atau **K-Means Clustering**, mampu memisahkan titik data yang secara multidimensi terasa asing dan di luar kelaziman klaster pola normal. 

## 4. Studi Kasus (Real-world Example)
**Pencegahan Kegagalan Traktor Otonom (Predictive Maintenance)**
Sensor getaran (Vibrasi) pada roda gigi traktor bajak pintar dipantau setiap detiknya. Melalui Machine Learning klasifikasi, komputer mempelajari jutaan pola sinyal audio dari traktor yang sehat dan sinyal audio beberapa jam sebelum gigi traktor rontok/patah. Sekarang, traktor tersebut bisa memperingatkan petani: *"Probabilitas 87% mesin transmisi akan patah dalam 3 hari ke depan, segera bawa ke bengkel"*. Ini disebut *Predictive Maintenance*, menghemat ratusan juta rupiah yang melayang jika kerusakan terjadi di tengah masa panen krusial.

## 5. Bahan Diskusi Kelas
1. Menurut Anda, mengapa implementasi Machine Learning pada lingkungan IoT pertanian luar ruangan (*open field*) jauh lebih menantang akurasinya dibanding Machine Learning robot pengelasan di dalam pabrik manufaktur tertutup (*indoor*)?
2. Jika mesin bisa memprediksi masa depan, sebutkan minimal 2 metrik (cara hitung) matematis untuk menguji "Seberapa tepat tebakan mesin itu terhadap kenyataan sebenarnya"?

## 6. Referensi Spesifik
- Goodfellow, I., Bengio, Y., & Courville, A. (2016). *Deep Learning*. MIT Press. (ISBN-13: 978-0262035613). (Membahas filosofi pemodelan data dari data mentah).  
- Lihat `Referensi/Daftar_Referensi_Lengkap.md` untuk referensi selengkapnya.  
