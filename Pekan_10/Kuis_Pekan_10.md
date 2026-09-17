# Kuis Pekan 10: Pemodelan ML (Prediksi dan Klasifikasi Anomali)

Jawablah pertanyaan-pertanyaan berikut dengan jelas dan ringkas!

1. Dalam dunia *Machine Learning*, mengapa sangat diharamkan (dilarang) untuk melatih model AI dan mengujinya menggunakan seluruh porsi dataset yang sama persis?

2. Standar yang sering dipakai oleh analis data untuk membelah (*Splitting*) sebuah dataset menjadi data latih (*Train Set*) dan data uji (*Test Set*) adalah:

3. Ketika Anda menggunakan fungsi `train_test_split()` dari *Scikit-Learn* pada data **Time-Series** (sensor runtun waktu), parameter spesifik manakah yang Wajib diset nilainya agar urutan waktu masa lalu dan masa depan tidak menjadi acak/kacau?

4. Pada teknik *Feature Engineering* prediksi runtun waktu (*Forecasting*), apa tujuan dari menggunakan fungsi pergeseran baris data Pandas `df['Suhu'].shift(1)`?

5. Dalam model *Machine Learning* berbasis *Scikit-Learn*, perintah apakah yang secara spesifik dieksekusi agar komputer mulai fase "belajar" (pelatihan) dari data yang Anda sediakan?

6. Metrik statistik *Root Mean Squared Error* (RMSE) digunakan untuk menguji keakuratan sebuah mesin *Forecasting* (seperti Linear Regresi). Sistem AI dianggap semakin pintar memprediksi masa depan apabila:

7. *Scikit-Learn* mewajibkan input fitur `X` berupa struktur tabel (*DataFrame*/2D array), bukan sebarisan angka panjang (*Series*/1D array). Apabila Anda tak sengaja hanya mengetikkan kode `X = df['Suhu_Sebelumnya']` (dengan kurung siku tunggal), pesan *error* apa yang pasti muncul?

8. Algoritma *Machine Learning* di *Scikit-Learn* yang berakar dari cabang *Unsupervised Learning* dan sangat sakti untuk mendeteksi kejanggalan multi-variabel (memisahkan data yang melenceng jauh dari populasi/kelompok utama normal) adalah:

9. Jika Anda mengeksekusi metode deteksi *Isolation Forest* dan fungsi tersebut meludahkan deretan prediksi angka `1` dan `-1`, apa interpretasi atau makna dari angka **-1** pada baris data tersebut?

10. Saat inisialisasi `IsolationForest(contamination=0.01)`, argumen parameter `contamination` berfungsi untuk memberi tahu mesin bahwa:
