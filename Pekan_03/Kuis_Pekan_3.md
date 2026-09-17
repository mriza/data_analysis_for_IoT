---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Kuis Pekan 3: Anatomi dan Karakteristik Data Time-Series"
---

# Kuis Pekan 3: Anatomi dan Karakteristik Data Time-Series

Jawablah pertanyaan-pertanyaan berikut dengan jelas, ringkas, dan berdasarkan penalaran analitik Anda!

1. Berbeda dengan data tabular survei penduduk yang berfokus pada "Siapa" subjeknya, data sensor IoT secara eksklusif berakar pada dimensi *Time-Series* (Runtun Waktu). Analisis mengapa mengacak atau mengurutkan ulang (*sorting*) baris data IoT berdasarkan nilai suhunya (dari kecil ke besar) akan merusak seluruh validitas model analitik Anda?

2. Anda mengamati grafik kelembaban tanah selama satu tahun. Anda melihat bahwa grafik selalu melengkung naik pada bulan November-Februari dan turun tajam di bulan Juli-September, kejadian ini terus berulang setiap tahunnya. Namun di saat yang sama, rata-rata kelembaban keseluruhan dari tahun ke tahun secara perlahan terus menurun. Dekonstruksi dan tunjukkan mana bagian yang disebut **Seasonality** (Musiman) dan mana yang disebut **Trend** (Tren) dari kasus tersebut!

3. Pada grafik intensitas cahaya matahari, terdapat kurva naik di siang hari dan turun di malam hari (*Seasonality*). Namun, jika kita melihat lebih dekat pada grafik garis jam 12 siang, garis tersebut tidak mulus melainkan bergerigi naik-turun secara acak dalam hitungan detik. Fenomena komponen *time-series* apakah yang merepresentasikan gerigi acak tersebut, dan faktor perangkat keras apa yang biasanya menyebabkannya?

4. Jika Anda diminta untuk memvalidasi algoritma kecerdasan buatan baru untuk *Smart City*, mengapa seorang *Data Engineer* seringkali lebih merekomendasikan mengunduh data klasik dari *Kaggle* atau *UCI Machine Learning Repository* pada fase awal (*prototyping*) dibandingkan langsung memasang sensor fisik di jalan raya?

5. Kasus *Irregular Sampling Rate* sering terjadi pada perangkat IoT bertenaga surya (solar panel). Bagaimana perilaku sensor (penghematan daya) di malam hari dapat merusak interval indeks waktu (*timestamp*) pada *database*, dan mengapa hal ini menjadi tantangan besar saat data tersebut akan disuapkan ke dalam *Machine Learning* standar yang mengharapkan interval tetap?

6. Selama badai petir, *gateway* kebun kopi kehilangan daya selama 4 jam penuh, menghasilkan "lubang" kosong (*Missing Values/NaN*) berturut-turut pada data *database Time-Series* Anda. Analisis mengapa membiarkan baris `NaN` tersebut tetap kosong atau sekadar menghapusnya (*drop*) adalah langkah yang berisiko bagi kelangsungan sistem prediksi!

7. Sensor pH tanah berbasis probe besi (elektroda) sering mengalami fenomena **Sensor Drift**. Secara perlahan dari minggu ke minggu, angka pH yang terbaca semakin melenceng dari angka keasaman tanah yang sesungguhnya akibat karat. Jika tidak dideteksi, bagaimana fenomena ini bisa menyesatkan sistem aktuator otomatis di lapangan (misal pompa pupuk cair)?

8. Berdasarkan praktikum menggunakan Jupyter Notebook, mengapa format berbasis *Notebook* (yang memadukan teks penjelasan *Markdown* dan blok eksekusi kode *Python* secara interaktif) jauh lebih populer bagi kalangan *Data Scientist* untuk mengeksplorasi data sensor dibandingkan menggunakan *script* panjang pada *Code Editor* biasa?

9. Anda baru saja mengaktifkan lingkungan virtual Python (`venv`) dari terminal Jupyter. Jika Anda secara keliru membuka Jupyter di luar *virtual environment* tersebut, mengapa perintah `import pandas` kemungkinan besar akan menghasilkan pesan `ModuleNotFoundError`, padahal Anda sangat yakin baru saja menginstalnya 5 menit lalu?

10. Mari gunakan penalaran komparatif. Jika sebuah nilai sensor hilang selama 2 detik versus hilang berturut-turut selama 3 hari, analisislah secara logis mengapa metode "Menambal nilai kosong dengan nilai 1 baris sebelumnya" (Forward Fill) mungkin sukses diterapkan pada kasus pertama, namun akan sangat keliru dan berbahaya jika dipaksakan pada kasus kedua!
