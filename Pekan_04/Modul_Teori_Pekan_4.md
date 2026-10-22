---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Kuliah Pekan 4: Statistik Deskriptif, Exploratory Data Analysis (EDA), dan Analisis Korelasi Sensor"
---

# Materi Kuliah Pekan 4: Statistik Deskriptif, Exploratory Data Analysis (EDA), dan Analisis Korelasi Sensor

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 4 (Pengantar Teori untuk Praktikum EDA)  
**Capaian Pembelajaran (Sub-CPMK 4):** Mahasiswa mampu menerapkan teknik *Exploratory Data Analysis* (EDA) menggunakan statistik deskriptif, pemeriksaan bentuk distribusi data, serta analisis matriks korelasi multivariat (*Heatmap*) untuk mengekstrak *insight* dari dataset sensor pertanian/lingkungan.

---

## 1. Tujuan Pembelajaran Khusus
Setelah mempelajari modul ini, mahasiswa diharapkan mampu:
1. Menjelaskan peran strategis **Exploratory Data Analysis (EDA)** sebagai tahap audit kualitas data sebelum pemodelan *Machine Learning*.
2. Menghitung dan menginterpretasikan metrik **Statistik Deskriptif** (Mean, Median, Mode, Variansi, Standar Deviasi, Kuartil, dan Kemencengan/*Skewness*) pada data sensor IoT.
3. Memahami arsitektur struktur data `Series` dan `DataFrame` berindeks `DatetimeIndex` pada pustaka **Pandas**.
4. Menghitung koefisien **Korelasi Pearson ($r$)** dan membaca visualisasi **Matriks Korelasi (*Heatmap*)** antar-parameter sensor mikroklimat.
5. Menganalisis perbedaan kritis antara **Korelasi vs Kausalitas (*Correlation does not imply Causation*)** serta mengidentifikasi keberadaan **Variabel Perancu (*Confounding Variable / Spurious Correlation*)** di lapangan.

---

## 2. Pendahuluan: Mengapa EDA Wajib Dilakukan Sebelum Membuat Model AI?

Bayangkan seorang insinyur pertanian langsung memasukkan berkas CSV berisi 500.000 baris rekaman sensor rumah kaca ke dalam algoritma *Machine Learning* tanpa memeriksa isinya terlebih dahulu. Ternyata, di dalam berkas tersebut:
- Kolom kelembaban tanah tercatat dalam satuan persen ($0-100\%$) pada bulan Januari, namun berubah menjadi nilai tegangan mentah ADC ($0-4095$) pada bulan Februari akibat pembaruan *firmware*.
- Sensor suhu sempat mati selama 4 hari dan perangkat mengirim angka *default* `-999.0°C`.

Jika data tersebut langsung dilatih tanpa dieksplorasi, model prediksi yang dihasilkan akan kacau balau. **Exploratory Data Analysis (EDA)**—konsep yang dipelopori oleh matematikawan John Tukey—adalah prosedur investigasi sistematis menggunakan ringkasan numerik dan representasi grafis untuk memahami struktur, mendeteksi anomali, menguji asumsi, dan menemukan pola hubungan antar-variabel di dalam data.

---

## 3. Materi Inti

### 3.1 Pilar Statistik Deskriptif pada Data Sensor IoT
Ketika memanggil fungsi `df.describe()` di Pandas, komputer merangkum jutaan baris data menjadi beberapa parameter statistik kunci:

#### A. Ukuran Pemusatan Data (*Measures of Central Tendency*)
1. **Mean (Rata-Rata Aritmatika — $\mu$ atau $\bar{x}$):**
   $$\bar{x} = \frac{1}{n} \sum_{i=1}^{n} x_i$$
   Menunjukkan titik keseimbangan bobot seluruh data. Sangat berguna pada kondisi normal, namun **sangat rentan terdistorsi oleh *outlier* ekstrem**.
2. **Median (Nilai Tengah / Kuartil ke-2 — $Q_2$):**
   Nilai yang membagi kumpulan data yang telah diurutkan menjadi dua bagian sama banyak ($50\%$ di bawah dan $50\%$ di atas). Median bersifat **resisten/tangguh (*robust*)** terhadap lonjakan derau sesaat.

#### B. Ukuran Penyebaran Data (*Measures of Dispersion*)
Dua lahan pertanian bisa memiliki suhu rata-rata harian yang sama persis ($\bar{x} = 28^\circ\text{C}$), namun lahan A berada di dalam *greenhouse* terkontrol ($27^\circ\text{C} - 29^\circ\text{C}$) sedangkan lahan B berada di padang terbuka ekstrem ($15^\circ\text{C}$ malam hari dan $41^\circ\text{C}$ siang hari). Untuk membedakannya, kita membutuhkan ukuran penyebaran:
1. **Variansi ($\sigma^2$ atau $s^2$) dan Standar Deviasi ($\sigma$ atau $s$):**
   $$s = \sqrt{\frac{1}{n-1} \sum_{i=1}^{n} (x_i - \bar{x})^2}$$
   - Jika $\sigma$ mendekati $0$: Sinyal sangat stabil (atau justru indikasi bahaya **Stuck-at Fault** jika bernilai $0,0000$ mutlak!).
   - Jika $\sigma$ sangat besar: Terjadi fluktuasi lingkungan yang ekstrem atau derau listrik (*noise*) yang parah.
2. **Kuartil ($Q_1, Q_2, Q_3$) dan Rentang Interkuartil ($IQR$):**
   - **$Q_1$ (Persentil 25%):** Batas nilai di mana $25\%$ data terendah berada.
   - **$Q_3$ (Persentil 75%):** Batas nilai di mana $75\%$ data berada.
   - **$IQR = Q_3 - Q_1$:** Rentang yang memuat $50\%$ populasi data utama di bagian tengah distribusi.

#### C. Bentuk Distribusi: Simetris (Normal) vs Condong (*Skewed*)
Tidak semua data sensor IoT membentuk kurva lonceng simetris (*Gaussian/Normal*):

```text
  1. DISTRIBUSI NORMAL (Simetris)               2. DISTRIBUSI RIGHT-SKEWED (Condong Kanan)
     Contoh: Suhu Ruangan Terkontrol               Contoh: Curah Hujan, Polusi PM2.5, Kecepatan Angin
                 _.-._                                        _._
               .'  |  '.                                     / | \
              /    |    \                                   /  |  \
             /     |     \                                 /   |   \._
        ____/      |      \____                        ___/    |      '---._______
           Mean = Median = Mode                              Mode < Median < Mean (tertarik ekor kanan)
```
- **Data Simetris (Normal):** Nilai $\text{Mean} \approx \text{Median}$. Contoh: Suhu inkubator atau kelembaban ruang tertutup.
- **Data Condong ke Kanan (*Right-Skewed / Positive Skew*):** Sebagian besar waktu bernilai rendah/nol, namun sesekali memiliki ekor panjang bernilai tinggi. Contoh: **Data Curah Hujan harian** (banyak hari bernilai $0\text{ mm}$, sesekali hujan badai $120\text{ mm}$) atau **Konsentrasi Debu Polusi $PM_{2.5}$**. Pada distribusi ini, nilai $\text{Mean} > \text{Median}$ karena rata-rata tertarik oleh nilai puncak di ekor kanan.

---

### 3.2 Arsitektur Pustaka Python untuk EDA: Pandas, Matplotlib, dan Seaborn
1. **Pandas (`pandas`):** Menyediakan struktur data **Series** (vektor 1 dimensi berindeks) dan **DataFrame** (tabel 2 dimensi). Dibangun di atas array bahasa C milik **NumPy**, sehingga operasi vektorisasi (menghitung rata-rata 1 juta baris) berjalan dalam hitungan milidetik tanpa perulangan `for` manual.
2. **Matplotlib (`matplotlib.pyplot`):** Pustaka *rendering* grafik dasar yang mengontrol kanvas (*Figure*), sumbu koordinat (*Axes*), garis, warna, dan teks label.
3. **Seaborn (`seaborn`):** Pustaka visualisasi statistik tingkat tinggi di atas Matplotlib yang mempermudah pembuatan *Heatmap*, *Boxplot*, *Violin Plot*, dan *Pairplot* hanya dengan satu baris perintah.

---

### 3.3 Analisis Korelasi Multivariat & Matriks Heatmap

Dalam sistem IoT pertanian, variabel-variabel lingkungan saling berinteraksi secara fisika dan termodinamika. Untuk mengukur kekuatan dan arah hubungan linier antara variabel sensor $X$ dan variabel sensor $Y$, digunakan **Koefisien Korelasi Pearson ($r$)**:

$$r_{xy} = \frac{\sum_{i=1}^{n} (x_i - \bar{x})(y_i - \bar{y})}{\sqrt{\sum_{i=1}^{n} (x_i - \bar{x})^2} \sqrt{\sum_{i=1}^{n} (y_i - \bar{y})^2}}$$

Nilai $r$ selalu berada pada rentang **$-1,0 \le r \le +1,0$**:

| Rentang Nilai $r$ | Arah & Kekuatan Hubungan | Contoh Fenomena Fisik pada Sensor IoT Pertanian |
| :---: | :--- | :--- |
| **$+0,70 \text{ s/d } +1,00$** | **Korelasi Positif Kuat** (Searah: $X \uparrow \implies Y \uparrow$) | Intensitas Radiasi Matahari (*Solar Lux*) vs Suhu Udara *Greenhouse* ($r \approx +0,85$). Semakin terik matahari, suhu udara semakin naik. |
| **$+0,30 \text{ s/d } +0,69$** | **Korelasi Positif Sedang** | Kecepatan Angin vs Laju Penguapan Air (*Evapotranspirasi*). |
| **$-0,29 \text{ s/d } +0,29$** | **Tidak Berkorelasi / Lemah** (Independen) | Tegangan Baterai Node Sensor vs Tingkat Keasaman (pH) Air Kolam ($r \approx 0,02$). |
| **$-0,69 \text{ s/d } -0,30$** | **Korelasi Negatif Sedang** | Suhu Udara vs Kadar Oksigen Terlarut (*Dissolved Oxygen* / DO) di kolam ikan (air makin hangat, kelarutan gas oksigen menurun). |
| **$-1,00 \text{ s/d } -0,70$** | **Korelasi Negatif Kuat** (Berlawanan: $X \uparrow \implies Y \downarrow$) | Suhu Udara vs Kelembaban Relatif Udara / *Relative Humidity* ($r \approx -0,88$). Udara panas menampung uap air lebih renggang sehingga % RH turun. |

#### Contoh Perhitungan Manual Korelasi Pearson Sederhana
Misalkan terdapat 3 pengamatan pasangan sensor **Intensitas Cahaya ($X$, dalam ribuan Lux)** dan **Suhu Udara ($Y$, dalam °C)**:
- Sampel 1 (Pagi): $X_1 = 10$, $Y_1 = 24$
- Sampel 2 (Siang): $X_2 = 50$, $Y_2 = 32$
- Sampel 3 (Sore): $X_3 = 30$, $Y_3 = 28$

Langkah perhitungan:
1. Hitung rata-rata: $\bar{x} = \frac{10+50+30}{3} = 30$; $\quad \bar{y} = \frac{24+32+28}{3} = 28$.
2. Hitung simpangan $(x_i - \bar{x})$ dan $(y_i - \bar{y})$:
   - Sampel 1: $(10-30) = -20$; $\quad (24-28) = -4 \implies (-20)(-4) = +80$
   - Sampel 2: $(50-30) = +20$; $\quad (32-28) = +4 \implies (+20)(+4) = +80$
   - Sampel 3: $(30-30) = 0$; $\quad (28-28) = 0 \implies (0)(0) = 0$
3. Jumlahkan hasil kali pembilang: $\sum (x_i - \bar{x})(y_i - \bar{y}) = 80 + 80 + 0 = 160$.
4. Hitung penyebut:
   $$\sqrt{(-20)^2 + 20^2 + 0^2} \times \sqrt{(-4)^2 + 4^2 + 0^2} = \sqrt{800} \times \sqrt{32} = \sqrt{25.600} = 160$$
5. Koefisien korelasi:
   $$r = \frac{160}{160} = +1,00\ (\text{Korelasi Positif Sempurna})$$

---

### 3.4 Jebakan Analitik: Korelasi Bukan Berarti Sebab-Akibat (*Correlation $\neq$ Causation*)
Prinsip paling penting dalam analisis data ilmiah adalah: **Tingginya angka korelasi statistik antara variabel $A$ dan variabel $B$ tidak secara otomatis membuktikan bahwa $A$ menyebabkan $B$!**

Sering kali muncul fenomena **Korelasi Semu (*Spurious Correlation*)** yang dipicu oleh variabel ketiga yang tersembunyi, yang disebut **Confounding Variable (Variabel Perancu)**:

```text
                         +-----------------------------------------+
                         |         CONFOUNDING VARIABLE            |
                         |  (Radiasi Terik Matahari Siang Hari)    |
                         +-----------------------------------------+
                                      /                       \
            Memanaskan atap          /                         \  Meningkatkan transpirasi
            gedung ruang server     v                           v daun di kebun
        +-----------------------------------+           +-----------------------------------+
        | Variabel X:                       |           | Variabel Y:                       |
        | Suhu AC Ruang Server Naik         | <=======> | Penguapan Air Tanaman Lahan Naik  |
        +-----------------------------------+  Korelasi +-----------------------------------+
                                               r = +0.94
                                           (Korelasi Semu!)
```

**Contoh Kasus Pertanian lainnya:**
Hasil analisis data pada 100 petak sawah menunjukkan korelasi positif kuat ($r = +0,81$) antara **Sensor Kekeruhan Air Irigasi (*Turbidity*)** dan **Tonase Panen Padi**.
- *Kesimpulan Naif yang Salah:* "Mari kita tuangkan lumpur tanah liat ke saluran irigasi agar panen padi meningkat!"
- *Analisis Kritis (Domain Knowledge):* Kekeruhan air bukanlah penyebab langsung suburnya padi, melainkan terdapat *Confounding Variable* berupa **endapan abu vulkanik sungai hulu yang kaya unsur hara Kalium dan Fosfat**. Tanpa uji laboratorium agronomi, menyuntikkan lumpur biasa justru akan mendangkalkan saluran irigasi dan merusak pompa!

---

## 4. Bahan Diskusi & Berpikir Kritis (*HOTS*) di Kelas

1. **Deteksi Kerusakan Sensor via Matriks Korelasi:**
   Secara termodinamika, korelasi antara Suhu Udara dan Kelembaban Relatif (RH) di sebuah *greenhouse* normalnya selalu bernilai negatif kuat ($r \approx -0,85$). Namun pada minggu ini, saat Anda menjalankan `df.corr()`, nilai korelasi keduanya berubah menjadi $r = +0,01$ (mendekati nol). Apa hipotesis diagnostik Anda terkait kondisi perangkat keras sensor di lapangan?
2. **Mean vs Median pada Data Curah Hujan:**
   Mengapa dalam laporan klimatologi pertanian, rata-rata (*Mean*) curah hujan harian selama satu bulan sering kali menyesatkan petani jika dibandingkan dengan melihat nilai *Median* dan *Jumlah Hari Hujan*?

---

## 5. Referensi & Acuan Materi Pekan Ini

1. **McKinney, W. (2022).** *Python for Data Analysis: Data Wrangling with Pandas, NumPy, and Jupyter* (3rd ed.). O'Reilly Media. (ISBN-13: 978-1098104030) — **Bab 5 (Getting Started with Pandas: Summarizing and Computing Descriptive Statistics, Correlation and Covariance) & Bab 9 (Plotting and Visualization).**  
2. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730) — **Bab 4 (Exploring IoT Data: Finding Correlations and Exploring Distributions).**  
3. **Fawcett, T., & Provost, F. (2013).** *Data Science for Business*. O'Reilly Media. (ISBN-13: 978-1449361327) — **Bab 3 (Informative Attributes & Correlations).**  
4. **Pandas Development Team. (2024).** *Pandas Documentation: `DataFrame.describe` & `DataFrame.corr`*. Tersedia: [https://pandas.pydata.org/docs/](https://pandas.pydata.org/docs/)  
5. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
