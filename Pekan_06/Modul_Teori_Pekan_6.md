---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Kuliah Pekan 6: Deteksi Anomali Statistik (Z-Score & IQR) dan Validasi Silang Sensor Multivariat"
---

# Materi Kuliah Pekan 6: Deteksi Anomali Statistik (Z-Score & IQR) dan Validasi Silang Sensor Multivariat

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 6 (Pengantar Teori untuk Praktikum Deteksi Anomali & Tugas 1)  
**Capaian Pembelajaran (Sub-CPMK 6):** Mahasiswa mampu membedakan taksonomi anomali sensor IoT, menurunkan dan menghitung deteksi *outlier* menggunakan metode parametrik **Z-Score** dan non-parametrik **Interquartile Range (IQR / Tukey's Fences)**, serta merancang logika validasi silang antar-sensor (*Cross-Sensor Validation*) di lapangan.

---

## 1. Tujuan Pembelajaran Khusus
Setelah mempelajari modul ini, mahasiswa diharapkan mampu:
1. Membedakan empat kategori anomali pada sistem IoT: **Point/Hard Outlier**, **Contextual Outlier**, **Collective/Stuck-at Outlier**, dan **Multivariate Outlier**.
2. Menghitung nilai **Z-Score ($Z = \frac{x - \mu}{\sigma}$)** berdasarkan Aturan Empiris Distribusi Normal ($68\% - 95\% - 99,7\%$) serta menganalisis kelemahan *Masking Effect* pada distribusi data yang menceng (*skewed*).
3. Menghitung batas pagar dalam dan pagar luar **Interquartile Range (IQR / Metode Tukey)** serta memetakan komponennya ke dalam anatomi diagram **Boxplot**.
4. Merancang prosedur diagnostik **Validasi Silang Spasial & Fisika (*Cross-Sensor Physics Validation*)** untuk membedakan kerusakan sensor tunggal dari bencana lingkungan nyata (seperti kebakaran lahan atau kebocoran pipa).
5. Menjelaskan metode deteksi serangan manipulasi data siber (**False Data Injection Attack**) menggunakan analisis derau alami dan kekekalan hukum fisika.

---

## 2. Filosofi Anomali IoT: Kerusakan Sensor atau Kejadian Bahaya Nyata?

Pada Pekan 5, kita telah mempelajari cara membuang nilai ekstrem yang melampaui batas spesifikasi perangkat keras (seperti suhu $1.000^\circ\text{C}$ atau $-50^\circ\text{C}$ di kebun tropis). Namun, bagaimana jika sebuah sensor suhu di rumah kaca mencatat angka **$46^\circ\text{C}$**? Angka $46^\circ\text{C}$ masih berada di dalam rentang kerja fisik sensor DHT22 ($-40^\circ\text{C}$ s.d. $+80^\circ\text{C}$), tetapi sangat menyimpang dari kondisi normal harian ($25^\circ\text{C} - 32^\circ\text{C}$).

Sebelum baris kode Anda menghapus angka tersebut, seorang *IoT Data Analyst* wajib menjawab pertanyaan fundamental:
> **"Apakah angka menyimpang ini adalah kesalahan pembacaan alat (*Sensor Fault*) yang harus dihapus, ataukah sebuah peristiwa kritis di lapangan (*Critical Physical Event* seperti kebakaran/kipas mati) yang justru wajib membunyikan alarm darurat?"**

### 2.1 Empat Taksonomi Anomali pada Data Sensor IoT
1. **Point Outlier (*Hard / Global Outlier*):**
   Satu titik data terisolasi yang nilainya melompat keluar jauh dari seluruh populasi data hanya selama 1 siklus *sampling*, lalu seketika kembali normal.
   - *Contoh:* Suhu terbaca `28.1°C, 28.0°C, 79.5°C, 28.2°C`. Hampir pasti disebabkan oleh gangguan induksi listrik sesaat (*EMI spike*).
2. **Contextual Outlier (*Conditional Outlier*):**
   Nilai yang secara angka global terlihat wajar, tetapi **tidak wajar jika dilihat pada konteks waktu atau musim terjadinya**.
   - *Contoh:* Suhu udara $33^\circ\text{C}$ adalah normal pada pukul 13.00 siang, tetapi jika suhu **$33^\circ\text{C}$ tercatat pada pukul 02.30 dini hari** di pegunungan, itu adalah *Contextual Outlier* (misalnya pemanas ruangan mengalami malfungsi tidak mau mati).
3. **Collective / Sequence Outlier:**
   Sekumpulan titik data berurutan yang masing-masing angkanya berada di batas normal, tetapi **pola gabungannya sepanjang waktu tidak normal**.
   - *Contoh:* Sensor oksigen kolam ikan mencatat angka `4.120000 mg/L` tanpa fluktuasi desimal sedikit pun selama 15 jam (**Stuck-at Fault / Zero-Variance**).
4. **Multivariate Outlier (Anomali Relasional Antar-Sensor):**
   Masing-masing sensor tampak normal jika diperiksa sendiri-sendiri (*univariate*), namun **kombinasi antar-sensornya melanggar hukum fisika/termodinamika**.
   - *Contoh:* Pada siang hari bolong yang terik (Sensor Cahaya = $85.000\text{ Lux}$), Sensor Suhu justru mencatat $18^\circ\text{C}$ dan Kelembaban Udara $99\%$.

---

## 3. Metode Parametrik: Deteksi Outlier dengan Z-Score (*Standard Score*)

Metode **Z-Score** mengukur seberapa jauh suatu titik pengamatan ($x$) menyimpang dari nilai Rata-rata populasi ($\mu$), dinyatakan dalam satuan **Standar Deviasi ($\sigma$)**. Metode ini mengasumsikan bahwa data terdistribusi secara **Normal (Gaussian / Kurva Lonceng Simetris)**.

### 3.1 Rumus Matematis & Aturan Empiris (68 – 95 – 99,7)
$$Z_i = \frac{x_i - \mu}{\sigma}$$

```text
                        KURVA DISTRIBUSI NORMAL (GAUSSIAN) & ATURAN EMPIRIS
                                             _.-""""-._
                                           .'    |     '.
                                          /      |       \
                                         /   68.27% data  \
                                        /  (-1 <= Z <= +1) \
                                       /         |          \
                                  _.-'/    95.45% data       \'-._
                        ____..--''    |  (-2 <= Z <= +2)     |    ''--..____
                  +---------+---------+----------+-----------+---------+---------+
                 Z=-3      Z=-2      Z=-1       Z=0         Z=+1      Z=+2      Z=+3
               (\mu-3\sigma)                  (Mean \mu)                      (\mu+3\sigma)
         <========|                                                              |========>
      OUTLIER KIRI (Z < -3)               99.73% DATA NORMAL                   OUTLIER KANAN (Z > +3)
     (Hanya 0.135% peluang)              (-3 <= Z <= +3)                      (Hanya 0.135% peluang)
```

- **Nilai $Z = 0$:** Titik data persis sama dengan nilai rata-rata ($\mu$).
- **Rentang $-1 \le Z \le +1$:** Memuat **$68,27\%$** dari populasi data normal.
- **Rentang $-2 \le Z \le +2$:** Memuat **$95,45\%$** dari populasi data normal.
- **Rentang $-3 \le Z \le +3$:** Memuat **$99,73\%$** dari populasi data normal.
- **Konvensi Standar Outlier ($|Z| > 3$):** Karena peluang munculnya data normal di luar $\pm 3\sigma$ hanya $0,27\%$ ($1\text{ banding } 370$), setiap pembacaan sensor yang memiliki **$|Z| > 3$** (yaitu $Z > +3,0$ atau $Z < -3,0$) diklasifikasikan sebagai **Outlier**.

#### Contoh Perhitungan Numerik Z-Score
Sebuah dataset sensor kelembaban udara memiliki distribusi normal dengan rata-rata **$\mu = 60\%$** dan standar deviasi **$\sigma = 5\%$**. Tentukan apakah pembacaan sensor التالية: (a) $x_1 = 68\%$ dan (b) $x_2 = 76\%$ merupakan *outlier* dengan ambang $|Z| > 3$!
1. Untuk $x_1 = 68\%$:
   $$Z_1 = \frac{68 - 60}{5} = \frac{8}{5} = +1,60 \quad (|1,60| \le 3 \implies \textbf{Data Normal})$$
2. Untuk $x_2 = 76\%$:
   $$Z_2 = \frac{76 - 60}{5} = \frac{16}{5} = +3,20 \quad (|3,20| > 3 \implies \textbf{Terdeteksi OUTLIER!})$$

### 3.2 Kelemahan Fatal Z-Score: Data Menceng (*Skewed*) & *Masking Effect*
Mengapa Z-Score sering gagal pada data sensor lingkungan terbuka?
1. **Distorsi oleh Outlier itu Sendiri (*Masking Effect*):** Rumus Z-Score menggunakan $\mu$ dan $\sigma$. Apabila di dalam dataset terdapat beberapa *outlier* ekstrem (misal angka `999.0` akibat error sensor), nilai $\mu$ dan terutama $\sigma$ akan membengkak sangat besar. Membengkaknya penyebut $\sigma$ membuat nilai $Z$ dari *outlier* lainnya mengecil di bawah $3$, sehingga *outlier* tersebut lolos dari deteksi (*masked*)!
2. **Tidak Cocok untuk Distribusi Condong (*Right-Skewed*):** Pada data polusi debu $PM_{2.5}$ atau curah hujan yang memiliki ekor kanan panjang, asumsi kurva lonceng simetris runtuh.

---

## 4. Metode Non-Parametrik Tangguh: Interquartile Range (IQR / Tukey's Fences)

Untuk mengatasi kelemahan Z-Score, John Tukey memperkenalkan metode berbasis persentil/kuartil yang **tidak bergantung pada Mean maupun Standar Deviasi**, serta tidak menuntut data berdistribusi normal.

### 4.1 Rumus Pagar Tukey (*Tukey's Fences*)
Urutkan seluruh data dari nilai terkecil ke terbesar, lalu bagi menjadi empat bagian sama besar:
1. **Kuartil Bawah ($Q_1$ — Persentil ke-25):** Nilai batas $25\%$ data terendah.
2. **Median ($Q_2$ — Persentil ke-50):** Nilai tengah data.
3. **Kuartil Atas ($Q_3$ — Persentil ke-75):** Nilai batas $75\%$ data.
4. **Rentang Interkuartil ($IQR$):**
   $$IQR = Q_3 - Q_1$$
5. **Batas Pagar Dalam (*Inner Fences* — Standar Pengali $1,5$):**
   $$\text{Batas Bawah (Lower Fence)} = Q_1 - (1,5 \times IQR)$$
   $$\text{Batas Atas (Upper Fence)} = Q_3 + (1,5 \times IQR)$$
   Setiap nilai $x < \text{Batas Bawah}$ atau $x > \text{Batas Atas}$ divonis sebagai **Outlier**. *(Untuk deteksi outlier yang sangat ekstrem saja, pengali dapat dilonggarkan menjadi $3,0 \times IQR$ atau Outer Fence).*

```text
                                  ANATOMI VISUAL BOXPLOT & PAGAR IQR

      Outlier Kiri       Batas Bawah         Kotak IQR (50% Data Utama)         Batas Atas       Outlier Kanan
      (x < Q1-1.5*IQR)  (Q1 - 1.5*IQR)     Q1 (25%)  Median(Q2)  Q3 (75%)     (Q3 + 1.5*IQR)    (x > Q3+1.5*IQR)
                                            +-----------+-----------+
        <>    <>              |-------------|           |           |-------------|                  <>    <>
                                            +-----------+-----------+
                              <-- Whisker ->|<------- IQR --------->|<-- Whisker ->
```

#### Contoh Perhitungan Numerik IQR
Dalam analisis data sensor kelembaban tanah, diketahui nilai Kuartil Bawah **$Q_1 = 20\%$** dan Kuartil Atas **$Q_3 = 30\%$**. Berapakah rentang nilai pembacaan sensor yang dianggap valid (bukan *outlier*)?
1. Hitung $IQR$:
   $$IQR = Q_3 - Q_1 = 30\% - 20\% = 10\%$$
2. Hitung Batas Bawah:
   $$\text{Batas Bawah} = Q_1 - (1,5 \times IQR) = 20\% - (1,5 \times 10\%) = 20\% - 15\% = \mathbf{5\%}$$
3. Hitung Batas Atas:
   $$\text{Batas Atas} = Q_3 + (1,5 \times IQR) = 30\% + (1,5 \times 10\%) = 30\% + 15\% = \mathbf{45\%}$$
4. **Kesimpulan:** Seluruh pembacaan kelembaban tanah di antara **$5\%$ hingga $45\%$** adalah data valid; nilai di bawah $5\%$ atau di atas $45\%$ ditandai sebagai *outlier*.

---

## 5. Validasi Silang Spasial & Keamanan Siber Data Sensor

### 5.1 Validasi Silang Spasial & Fisika (*Cross-Sensor Validation*)
Bagaimana cara sistem analitik membedakan secara otomatis antara **sensor rusak** vs **kebakaran lahan** ketika satu sensor suhu tanah ($S_1$) melonjak dari $28^\circ\text{C}$ ke $75^\circ\text{C}$?
1. **Korelasi Spasial Tetangga (*Spatial Neighbor Check*):** Periksa pembacaan sensor suhu di titik-titik terdekat di sekitarnya ($S_2, S_3, S_4$). Kebakaran lahan fisik pasti merambatkan panas ke area sekitar; jika $S_2$ dan $S_3$ tetap tenang di $28^\circ\text{C}$, maka lonjakan pada $S_1$ adalah kerusakan terisolasi (*isolated sensor fault*).
2. **Korelasi Multivariat Antar-Modalitas Fisik (*Multi-Modal Physics Check*):** Periksa sensor jenis lain pada node yang sama (misal **Sensor Asap/Gas CO** dan **Sensor Kelembaban Udara**). Api nyata selalu diiringi kenaikan kadar asap CO dan penurunan kelembaban udara.

### 5.2 Mendeteksi Serangan Injeksi Data Palsu (*False Data Injection Attack*)
Pada infrastruktur kritis (seperti jaringan pipa air irigasi primer atau bendungan), peretas dapat membajak kredensial MQTT lalu menyuntikkan paket data tekanan pipa palsu yang konstan bernilai normal (misal: tepat `4.0000 bar` terus-menerus) untuk menyembunyikan aksi pencurian air atau kebocoran pipa. Dua metode analitik untuk membongkar serangan ini:
1. **Analisis Derau Alami (*Natural Physical Noise Check*):** Sensor analog di dunia nyata selalu memiliki fluktuasi derau mikro termal/hidrodinamika ($\sigma > 0$). Sinyal yang memiliki variansi nol mutlak ($\sigma = 0$) dalam waktu lama mengindikasikan sinyal sintetis/artifisial.
2. **Validasi Hukum Fisika Fluida (*Physics-Informed Validation*):** Bandingkan angka tekanan pipa dengan sensor **Debit Aliran (*Flow Rate*)** dan **Arus Listrik Motor Pompa ($I_{\text{pump}}$)**. Berdasarkan hukum kontinuitas fluida, jika daya pompa naik dan debit berubah tetapi tekanan terkunci datar di `4.0000 bar`, sistem langsung membunyikan alarm manipulasi data!

---

## 6. Bahan Diskusi & Berpikir Kritis (*HOTS*) di Kelas

1. **Perbandingan Z-Score vs IQR pada Polusi Udara:**
   Sebuah sensor debu $PM_{2.5}$ di dekat pabrik pengolahan pupuk mencatat nilai normal $15\ \mu\text{g/m}^3$ selama 90% waktu, tetapi sesekali melonjak ke $350\ \mu\text{g/m}^3$ saat cerobong pabrik membuang uap. Mengapa metode IQR jauh lebih andal menangkap lonjakan tersebut dibandingkan Z-Score?
2. **Strategi Penanganan Setelah Outlier Terdeteksi:**
   Jelaskan mengapa dalam kode produksi IoT, kita sebaiknya menyimpan hasil deteksi *outlier* ke dalam kolom baru `df['is_outlier'] = True/False` (*Flagging*) terlebih dahulu sebelum mengganti nilainya pada kolom `df['suhu_bersih']`, alih-alih langsung menimpa kolom data mentah aslinya!

---

## 7. Referensi & Acuan Materi Pekan Ini

1. **McKinney, W. (2022).** *Python for Data Analysis: Data Wrangling with Pandas, NumPy, and Jupyter* (3rd ed.). O'Reilly Media. (ISBN-13: 978-1098104030) — **Bab 7 (Data Cleaning and Preparation: Detecting and Filtering Outliers).**  
2. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730) — **Bab 4 (Exploring IoT Data & Outlier Detection).**  
3. **Fawcett, T., & Provost, F. (2013).** *Data Science for Business*. O'Reilly Media. (ISBN-13: 978-1449361327)  
4. **Pandas & SciPy Documentation. (2024).** `pandas.DataFrame.quantile`, `scipy.stats.zscore`, dan `seaborn.boxplot`. Tersedia: [https://pandas.pydata.org/docs/](https://pandas.pydata.org/docs/)  
5. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
