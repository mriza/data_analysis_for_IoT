---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Kuis Pekan 10: Pemodelan ML (Prediksi dan Klasifikasi Anomali)"
---

# Kuis Pekan 10: Pemodelan ML (Prediksi dan Klasifikasi Anomali)

Jawablah pertanyaan-pertanyaan berikut dengan jelas, ringkas, dan berdasarkan penalaran analitik Anda!

1. Dalam aturan emas *Machine Learning*, Anda dilarang keras menguji akurasi model menggunakan data yang sama dengan yang Anda gunakan saat melatih (*training*) model tersebut. Jelaskan bahaya fatal dari fenomena *Overfitting* (menghafal) yang akan terjadi jika aturan pembagian dataset (Train-Test Split) ini dilanggar!

2. Saat Anda membelah data tabular biasa (contoh: prediksi harga rumah), pengaturan parameter `shuffle=True` (mengacak baris data sebelum dibelah) sangat direkomendasikan. Namun, jika Anda menggunakan `train_test_split` pada *Data Time-Series* IoT, mengapa penyetelan parameter `shuffle=False` justru bersifat mutlak wajib?

3. Pada kasus prediksi beruntun waktu (Forecasting), kita membuat *Feature Engineering* yang disebut pergeseran data (*Lagging*), menggunakan perintah `df['Suhu_Sebelumnya'] = df['suhu'].shift(1)`. Jelaskan secara logika matematis mengapa proses menciptakan "kolom masa lalu" ini merupakan syarat mutlak agar sebuah algoritma Regresi Linier biasa mampu menebak masa depan!

4. Setelah Anda mengeksekusi operasi pembuatan Lag-1 (*shift(1)*), Anda pasti mendapati bahwa pada indeks (baris) ke-0, nilai pada kolom *Suhu_Sebelumnya* menjadi `NaN` (kosong). Analisis mengapa Pandas secara logis melakukan pengosongan ini, dan langkah pembersihan wajib apa (`.dropna()`) yang harus dilakukan sebelum data disuapkan ke *Machine Learning*!

5. Saat menyiapkan variabel Independen (X) untuk Scikit-Learn, *Data Engineer* pemula sering mendapatkan pesan galat (*error*): `ValueError: Expected 2D array, got 1D array instead`. Jelaskan secara struktural mengapa Scikit-Learn menuntut Anda membungkus *feature* dengan sepasang kurung siku ganda (`X = df[['Suhu_Sebelumnya']]`), bukan kurung siku tunggal!

6. Anda melatih model *Linear Regression* untuk meramalkan suhu jam depan, kemudian mengukur kinerjanya dan mendapati nilai *Root Mean Squared Error (RMSE)* sebesar 4.5. Interpretasikan makna spesifik dan praktis dari angka 4.5 ini jika dilihat dari kacamata operator industri di lapangan yang sedang mengawasi sistem prediksi tersebut!

7. Saat terjadi kebocoran gas beracun secara sangat pelan di malam hari, suhu (yang berfluktuasi tipis) dan kecepatan angin (yang pelan) masih berada di ambang batas kewajaran, namun *kombinasi* keduanya menciptakan *outlier* multidimensi yang mematikan. Mengapa batas *Interquartile Range (IQR)* kemungkinan besar akan gagal bereaksi, dan bagaimana algoritma *Isolation Forest* secara teoretis berhasil "mengisolasi" kejadian mematikan tersebut?

8. Pada implementasi model pendeteksi anomali *Isolation Forest*, terdapat sebuah *hyperparameter* bernama `contamination=0.01`. Analisislah secara analitik: apa dampak fungsional yang akan terjadi pada sensitivitas alarm pendeteksi jika Anda secara keliru mengubah angka *contamination* tersebut menjadi 0.50 (50%)?

9. Model regresi sederhana (seperti regresi linier dengan *Lag-1*) berasumsi bahwa kejadian tepat 1 langkah di masa lalu (*t-1*) memiliki hubungan langsung dengan kejadian masa depan (*t+1*). Namun, bagaimana jika *pattern* (pola) fisik iklim di sebuah area sejatinya terjadi setiap 24 jam sekali (misal: embun selalu turun pada jam 3 pagi)? Solusi rekayasa fitur (*Feature Engineering*) seperti apa yang harus Anda modifikasi pada kode program Anda?

10. Dari hasil prediksi model *Isolation Forest*, Anda mendapatkan kolom *output* baru bersaldo nilai `1` dan `-1`. Jika kemudian Anda melempar visualisasinya ke layar menggunakan diagram pencar 2 Dimensi (*Scatter Plot*), deskripsikan secara visual (geometris) di area manakah titik-titik data berlabel `-1` itu akan bergerombol atau menyebar jika model Anda telah bekerja dengan benar!
