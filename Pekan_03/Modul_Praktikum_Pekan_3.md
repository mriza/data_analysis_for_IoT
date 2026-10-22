---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Modul Praktikum Pekan 3: Pengenalan, Pemrosesan, dan Dekomposisi Data Time-Series"
---

# Modul Praktikum Pekan 3: Pengenalan, Pemrosesan, dan Dekomposisi Data Time-Series

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 3 (Suplemen Praktikum Mandiri / Terbimbing)  
**Capaian Pembelajaran (Sub-CPMK 3):** Mahasiswa mampu menyiapkan lingkungan kerja analisis interaktif (Jupyter Notebook / Google Colab), memuat dataset *time-series* berindeks waktu (`DatetimeIndex`), melakukan agregasi ulang frekuensi waktu (*resampling*), serta memvisualisasikan dekomposisi *Trend, Seasonality*, dan *Residual/Noise* menggunakan Python.

---

## 1. Tujuan Praktikum
1. Mahasiswa memahami konsep dasar dan karakteristik spesifik dari data runtun waktu (*time-series*) secara praktis.
2. Mahasiswa mengetahui sumber-sumber repositori ilmiah untuk mengunduh dataset *time-series* dan IoT.
3. Mahasiswa mampu menginstal dan mengoperasikan perangkat lunak **Jupyter Notebook** (baik lokal, Google Colab, maupun *remote headless* di Ubuntu Server) untuk membaca, mengolah, dan mendekomposisi data *time-series*.
4. Mahasiswa mampu mengekspor laporan analisis interaktif (`.ipynb`) ke format dokumen standar (PDF/Markdown) tanpa kendala *parsing error*.

---

## 2. Pengenalan Data Time-Series Secara Detail

Dalam konteks *Internet of Things* (IoT), hampir seluruh data sensor yang dikirim ke server adalah **Data Time-Series** (Data Runtun Waktu). Data *time-series* adalah serangkaian titik data yang diindeks, dicatat, atau diurutkan berdasarkan urutan waktu (*chronological order*). Waktu pencatatan bisa dalam interval milidetik, detik, menit, jam, hari, hingga tahunan.

Berbeda dengan data tabular biasa (*cross-sectional*), analisis *time-series* sangat bergantung pada urutan observasi. Karakteristik utama dari data *time-series* meliputi:
- **Trend (Kecenderungan):** Pola pergerakan data jangka panjang, apakah cenderung naik ($T_t \uparrow$), turun ($T_t \downarrow$), atau stasioner.
- **Seasonality (Musiman):** Pola berulang dalam interval waktu tertentu yang tetap (misalnya, suhu rumah kaca selalu mencapai puncak pada pukul 12.00–13.00 siang, atau beban listrik naik setiap pukul 19.00 malam).
- **Cyclic (Siklus):** Fluktuasi jangka panjang yang terjadi tetapi tidak memiliki periode waktu yang tetap.
- **Noise / Irregular (Residu):** Fluktuasi acak atau ketidakteraturan dalam data yang tidak dapat dijelaskan oleh *trend* maupun *seasonality*. Di data IoT, ini sering berupa derau sinyal elektronika sensor atau *outlier* (pencilan).

---

## 3. Dataset Time-Series Klasik dan Tempat Mengunduhnya

Untuk mempelajari analisis *time-series*, kita dapat memanfaatkan repositori dataset publik:
1. **Kaggle Datasets** (`https://www.kaggle.com/datasets`)
2. **UCI Machine Learning Repository** (`https://archive.ics.uci.edu/ml/datasets.php`)
3. **Mendeley Data / NASA POWER Agroclimatology** (Data cuaca dan radiasi surya pertanian global).

### Contoh Dataset Time-Series Klasik & IoT
1. **Dataset Air Passengers (Penumpang Pesawat Internasional 1949–1960):**
   - **Mengapa dataset ini menjadi tolok ukur klasik?** Karena dataset ini sangat jelas menampilkan komponen **Trend** yang naik secara konsisten setiap tahun dan komponen **Seasonality Multiplikatif** (puncak jumlah penumpang selalu terjadi di pertengahan tahun saat musim liburan, dan tinggi gelombangnya semakin membesar mengikuti kenaikan tren).
   - **Format:** Berkas `CSV` dengan dua kolom: `Month` (`YYYY-MM`) dan `Passengers` (Nilai numerik).
2. **Beijing PM2.5 / Smart Agriculture Weather Dataset (UCI Repo):**
   - Mencatat parameter sensor kualitas udara ($PM_{2.5}$), suhu embun (*dew point*), temperatur, tekanan udara, dan kecepatan angin setiap 1 jam.

---

## 4. Perangkat Lunak (Software) Analisis Time-Series

Untuk membaca, mengolah, dan melihat karakteristik (*Exploratory Data Analysis*) pada dataset *time-series*, industri analitik IoT mengandalkan ekosistem **Python** dengan antarmuka interaktif **Jupyter Notebook** atau layanan *cloud* **Google Colaboratory**.

### A. Library Utama yang Akan Digunakan
1. **Pandas (`pandas`):** Pustaka utama untuk memanipulasi data tabular (*DataFrame*) dan deret waktu (`DatetimeIndex`, `resample`, `rolling`).
2. **Matplotlib & Seaborn (`matplotlib`, `seaborn`):** Pustaka visualisasi grafik garis (*line chart*), sebaran (*scatter*), dan peta korelasi (*heatmap*).
3. **Statsmodels (`statsmodels`):** Pustaka pemodelan statistik tingkat lanjut, termasuk fungsi `seasonal_decompose` untuk memisahkan sinyal menjadi *Trend*, *Seasonality*, dan *Residual*.

### B. Alternatif 1: Menggunakan Google Colaboratory (Cloud)
1. Buka browser dan akses [https://colab.research.google.com/](https://colab.research.google.com/).
2. Login menggunakan akun Google, lalu klik **New Notebook**.
3. Di Google Colab, pustaka `pandas`, `matplotlib`, `seaborn`, dan `statsmodels` sudah terinstal secara bawaan. Unggah (*upload*) file dataset CSV ke panel *Files* di sebelah kiri layar sebelum menjalankan kode.

### C. Alternatif 2: Instalasi Standar Python & Jupyter di Windows (Lokal)
1. Unduh **Python 3.x** dari situs resmi [https://www.python.org/downloads/](https://www.python.org/downloads/).
2. Jalankan *installer*, **pastikan opsi "Add Python to PATH" dicentang**, lalu selesaikan instalasi.
3. Buka *Command Prompt* (CMD) dan instal Jupyter beserta pustaka analisis data:
   ```bash
   pip install jupyter pandas matplotlib seaborn statsmodels
   ```
4. Jalankan perintah berikut di CMD untuk membuka antarmuka Jupyter di *browser*:
   ```bash
   jupyter notebook
   ```

### D. Alternatif 3: Instalasi Jupyter di Ubuntu Server Headless (Akses Remote via VirtualBox)
Sebagai kelanjutan praktikum Pekan 2, kita dapat menjalankan Jupyter Server di dalam Ubuntu Server VM dan mengaksesnya dari browser Windows (Host):
1. **Akses Ubuntu Server via SSH dari CMD Windows:**
   ```bash
   ssh praktikan@192.168.1.100
   ```
2. **Pastikan paket Python dan `venv` sudah terpasang:**
   ```bash
   sudo apt update
   sudo apt install -y python3 python3-pip python3-venv python-is-python3
   ```
3. **Buat dan aktifkan *Virtual Environment* (`jupyter-env`):**
   ```bash
   python -m venv jupyter-env
   source jupyter-env/bin/activate
   ```
4. **Instal Jupyter dan pustaka pengolahan data di dalam `venv`:**
   ```bash
   pip install jupyter pandas matplotlib seaborn statsmodels
   ```
5. **Buat konfigurasi dan atur kata sandi keamanan Jupyter:**
   ```bash
   jupyter notebook --generate-config
   jupyter notebook password
   ```
6. **Jalankan Jupyter Notebook dalam mode *headless* (`0.0.0.0`):**
   ```bash
   jupyter notebook --ip=0.0.0.0 --port=8888 --no-browser
   ```
7. Buka browser di Windows (Host), akses **`http://[IP_UBUNTU]:8888`**, lalu masukkan kata sandi yang telah dibuat.

---

## 5. Mengenal Antarmuka dan Fitur Jupyter Notebook

### A. Konsep Notebook: Laporan (Markdown) dan Kode (Python) Terpadu
Keunggulan utama berkas `.ipynb` adalah kemampuannya menggabungkan narasi ilmiah dan eksekusi kode secara berdampingan:
- **Sel Code:** Menulis dan menjalankan skrip Python (`Shift + Enter`). Tabel dan grafik langsung tampil di bawah sel.
- **Sel Markdown:** Menulis dokumentasi, landasan teori, rumus matematika LaTeX (`$\mu = \frac{\sum x}{n}$`), dan analisis hasil secara terstruktur dalam satu dokumen utuh.

### B. Manajemen File dan Workspace Jupyter
- Jupyter hanya menampilkan folder dan berkas yang berada di bawah direktori tempat perintah `jupyter notebook` dijalankan.
- **Peringatan Penting:** Jangan menghapus, mengubah nama, atau menyimpan dataset di dalam folder `jupyter-env` (folder sistem *virtual environment* Python). Buatlah folder khusus seperti `praktikum_pekan_3` untuk menyimpan pekerjaan Anda.

---

## 6. Langkah Kerja Praktikum: Membaca, Resampling, dan Dekomposisi Time-Series

### Langkah 1: Menyiapkan Dataset di Lingkungan Kerja
Unduh dataset klasik `AirPassengers.csv` langsung menggunakan terminal Linux (atau *cell* Jupyter dengan tanda seru `!wget`):
```bash
wget https://raw.githubusercontent.com/jbrownlee/Datasets/master/airline-passengers.csv -O AirPassengers.csv
```

### Langkah 2: Memuat Data dan Mengatur `DatetimeIndex`
Buat notebook baru `praktikum_pekan3.ipynb` dan jalankan kode berikut:

```python
import pandas as pd
import matplotlib.pyplot as plt

# Membaca CSV, mengubah kolom 'Month' menjadi objek datetime64, dan menjadikannya Index
df = pd.read_csv('AirPassengers.csv', parse_dates=['Month'], index_col='Month')

print("--- Informasi Struktur DataFrame ---")
print(df.info())

print("\n--- 5 Baris Pertama Data ---")
display(df.head())
```
*Penjelasan Teknis:* Perhatikan pada output `df.info()` bahwa indeks tabel kini berjenis **`DatetimeIndex`** (`1949-01-01` s.d. `1960-12-01`), bukan lagi indeks angka urut `0, 1, 2...`. Hal ini memungkinkan Pandas melakukan pemotongan berbasis tahun (misal `df.loc['1955':'1958']`) dan agregasi waktu otomatis.

### Langkah 3: Visualisasi Time-Series Dasar
```python
plt.figure(figsize=(12, 5))
plt.plot(df.index, df['Passengers'], marker='o', markersize=4, linestyle='-', color='#1F77B4', label='Jumlah Penumpang Bulanan')
plt.title('Grafik Runtun Waktu Jumlah Penumpang Pesawat (1949 - 1960)', fontsize=13, fontweight='bold')
plt.xlabel('Tahun Pencatatan')
plt.ylabel('Jumlah Penumpang (Ribuan)')
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()
plt.tight_layout()
plt.show()
```

### Langkah 4: Eksperimen Agregasi Waktu (*Downsampling / Resampling*)
Sebagaimana dipelajari pada modul teori, kita dapat meringkas data bulanan menjadi ringkasan tahunan (`'YE'` atau `'Y'`) dengan menghitung nilai rata-rata (`mean`), maksimum (`max`), dan minimum (`min`):

```python
# Melakukan downsampling dari frekuensi Bulanan (Monthly) ke Tahunan (Yearly)
df_tahunan = df['Passengers'].resample('YE').agg(['mean', 'max', 'min', 'std'])

print("--- Tabel Agregasi Tahunan (Mean, Max, Min, Std) ---")
display(df_tahunan.head())

# Membandingkan kurva bulanan asli vs rata-rata dan puncak tahunan
plt.figure(figsize=(12, 5))
plt.plot(df.index, df['Passengers'], color='lightgray', label='Data Bulanan Asli')
plt.plot(df_tahunan.index, df_tahunan['mean'], color='blue', marker='s', linewidth=2, label='Rata-rata Tahunan (Mean)')
plt.plot(df_tahunan.index, df_tahunan['max'], color='red', linestyle='--', marker='^', label='Puncak Tahunan (Max)')
plt.title('Perbandingan Data Asli vs Downsampling Tahunan')
plt.xlabel('Tahun')
plt.ylabel('Jumlah Penumpang')
plt.legend()
plt.grid(True, alpha=0.5)
plt.show()
```

### Langkah 5: Dekomposisi Komponen Time-Series (`Trend`, `Seasonal`, `Residual`)
Kita gunakan pustaka `statsmodels` untuk membedah sinyal menjadi komponen murninya. Karena amplitudo gelombang membesar seiring waktu, kita gunakan model **`multiplicative`**:

```python
from statsmodels.tsa.seasonal import seasonal_decompose

# Mendekomposisi data menjadi Observed, Trend, Seasonal, dan Residual
dekomposisi = seasonal_decompose(df['Passengers'], model='multiplicative', period=12)

# Mengatur ukuran gambar dan menampilkan ke-4 grafik bertumpuk
fig = dekomposisi.plot()
fig.set_size_inches(12, 8)
plt.suptitle('Dekomposisi Multiplikatif Data Time-Series', fontsize=14, fontweight='bold', y=1.02)
plt.tight_layout()
plt.show()
```
*Analisis Hasil:*
1. **Panel 1 (Observed):** Sinyal asli sebelum diuraikan.
2. **Panel 2 (Trend):** Garis mulus yang naik secara konsisten dari tahun 1949 hingga 1960 (efek musiman telah dibersihkan).
3. **Panel 3 (Seasonal):** Pola periodik 12 bulanan yang berulang secara presisi.
4. **Panel 4 (Resid / Noise):** Fluktuasi acak yang tersisa di sekitar angka $1,0$ (untuk model multiplikatif) atau $0,0$ (untuk model aditif).

---

## 7. Panduan Menyimpan dan Ekspor Laporan (`.ipynb` ke PDF/Markdown)

### A. Ekspor ke PDF via Browser (Cara Paling Aman Tanpa Instal LaTeX)
1. Di Jupyter Notebook, klik menu **File $\rightarrow$ Print Preview** (atau langsung tekan **`Ctrl + P`**).
2. Ubah opsi *Destination/Printer* menjadi **Save as PDF**.
3. Centang opsi **Background graphics** agar warna grafik tercetak sempurna, lalu klik **Save**.

### B. Peringatan Penting: Hindari Error `500 Internal Server Error` Akibat YAML Front Matter
Saat menulis identitas nama dan NIM di dalam **Sel Markdown Jupyter Notebook**, **jangan menggunakan garis pembatas `---` (YAML Front Matter)** di baris paling atas sel. Mesin konversi *Pandoc* milik Jupyter akan salah mengartikan blok `---` sebagai metadata konfigurasi yang rusak dan memicu `500: Internal Server Error`.

Gunakan dua spasi di akhir baris untuk membuat baris baru (*line break*) tanpa tanda `---`:
```markdown
# Laporan Praktikum Pekan 3
**Nama:** [Nama Mahasiswa]  
**NIM:** [NIM Mahasiswa]  
**Kelas:** TRK-3A  
```

---

## 8. Tugas Mandiri Praktikum
1. Ubah parameter `model='multiplicative'` pada kode Langkah 5 menjadi `model='additive'`. Bandingkan grafik panel **Resid (Residual)** antara model *Additive* dan *Multiplicative*! Pada model manakah komponen residunya masih menyisakan pola gelombang di awal dan akhir tahun? Jelaskan mengapa hal tersebut membuktikan bahwa dataset ini bersifat multiplikatif!
2. Buatlah simulasi data sensor suhu rumah kaca selama **7 hari dengan interval 10 menit** yang memiliki pola **Musiman Harian (24 jam)** ditambah **Sensor Drift** (suhu rata-rata bergeser naik $+0,5^\circ\text{C}$ setiap hari) dan derau acak. Gunakan `seasonal_decompose(period=144)` (karena $24\text{ jam} \times 6\text{ sampel/jam} = 144$) untuk memisahkan kurva *Sensor Drift* (pada panel *Trend*) dari siklus suhu siang-malamnya!

---

## 9. Referensi & Acuan Materi Pekan Ini

1. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730) — **Bab 4 (Exploring IoT Data).**  
2. **McKinney, W. (2022).** *Python for Data Analysis: Data Wrangling with Pandas, NumPy, and Jupyter* (3rd ed.). O'Reilly Media. (ISBN-13: 978-1098104030) — **Bab 11 (Time Series).**  
3. **Fawcett, T., & Provost, F. (2013).** *Data Science for Business*. O'Reilly Media. (ISBN-13: 978-1449361327)  
4. **Project Jupyter. (2024).** *Project Jupyter Documentation*. Tersedia: [https://jupyter.org/documentation](https://jupyter.org/documentation)  
5. **Pandas Development Team. (2024).** *Time Series / Date Functionality*. Tersedia: [https://pandas.pydata.org/docs/user_guide/timeseries.html](https://pandas.pydata.org/docs/user_guide/timeseries.html)  
6. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
