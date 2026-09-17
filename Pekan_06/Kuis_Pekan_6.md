# Kuis Pekan 6: Data Preprocessing Praktikal (Cleaning & Smoothing)

Jawablah pertanyaan-pertanyaan berikut dengan jelas dan ringkas!

1. Pada ekosistem Pandas, jika kita mendapati ada beberapa baris data sensor yang kosong (dinyatakan sebagai `NaN` atau *Not a Number*), fungsi spesifik manakah yang digunakan untuk membuat garis tebakan lurus secara matematis untuk mengisi lubang tersebut?

2. Apa bedanya metode `ffill()` dengan metode interpolasi saat menambal *Missing Values*?

3. Fungsi `.rolling(window=5).mean()` pada Pandas digunakan untuk:

4. Saat Anda memanggil fungsi `.rolling(window=5).mean()` pada DataFrame, Anda mungkin mendapati bahwa 4 baris pertama (*index* 0 sampai 3) pada kolom hasilnya berubah menjadi `NaN`. Mengapa hal ini terjadi?

5. Dalam metode deteksi *Outlier* menggunakan IQR (*Interquartile Range*), data dibagi menjadi empat kuartil. Apakah kepanjangan dari singkatan Q1 dan Q3?

6. Rumus matematika standar untuk mencari nilai rentang IQR adalah:

7. Berdasarkan standar statistik deteksi *outlier* menggunakan IQR, sebuah titik data akan secara resmi dicap sebagai *Outlier* jika titik tersebut:

8. Selain IQR, teknik statistik lain yang sering digunakan untuk memfilter *outlier* pada data berdistribusi normal adalah menghitung penyimpangan standar setiap titik dari rata-ratanya. Metode ini dinamakan:

9. Jika sebuah titik suhu data memiliki Z-Score lebih dari 3 (atau kurang dari -3), apa arti fisik/statistik dari titik tersebut?

10. Keunggulan utama dari mendeteksi *outlier* menggunakan algoritma seperti IQR dibandingkan menebak manual (misal: "pokoknya yang di atas 40 derajat itu *outlier*") adalah:
