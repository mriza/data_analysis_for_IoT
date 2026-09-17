---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Modul Praktikum Pekan 3: Pengenalan, Pemrosesan, dan Karakteristik Data Time-Series"
---

# Modul Praktikum Pekan 3: Pengenalan, Pemrosesan, dan Karakteristik Data Time-Series

## 1. Tujuan Praktikum
1. Mahasiswa memahami konsep dasar dan karakteristik spesifik dari data runtun waktu (*time-series*).
2. Mahasiswa mengetahui sumber-sumber repositori untuk mengunduh dataset *time-series*.
3. Mahasiswa mampu menginstal dan menggunakan perangkat lunak (Python standar, Jupyter Notebook) atau layanan *cloud* (Google Colab) untuk membaca, mengolah, dan memvisualisasikan data *time-series*.

---

## 2. Pengenalan Data Time-Series Secara Detail

Dalam konteks *Internet of Things* (IoT), hampir seluruh data sensor yang dikirim ke server adalah **Data Time-Series** (Data Runtun Waktu). 
Data *time-series* adalah serangkaian titik data yang diindeks, dicatat, atau diurutkan berdasarkan urutan waktu (*chronological order*). Waktu pencatatan bisa dalam interval detik, menit, jam, hari, hingga tahunan.

Berbeda dengan data tabular biasa (cross-sectional), analisis *time-series* sangat bergantung pada urutan observasi. Karakteristik utama dari data *time-series* meliputi:
- **Trend (Kecenderungan):** Pola pergerakan data jangka panjang, apakah cenderung naik (suhu global) atau turun.
- **Seasonality (Musiman):** Pola berulang dalam interval waktu tertentu yang tetap (misalnya, penggunaan listrik selalu naik pada jam 7 malam).
- **Cyclic (Siklus):** Fluktuasi yang terjadi tetapi tidak memiliki periode yang tetap atau teratur (lebih panjang dari musiman).
- **Noise / Irregular (Residu):** Fluktuasi acak atau ketidakteraturan dalam data yang tidak dapat dijelaskan oleh trend maupun musiman. Di data IoT, ini sering berupa *outlier* (pencilan) atau gangguan sinyal sensor.

---

## 3. Dataset Time-Series Klasik dan Tempat Mengunduhnya

Untuk belajar analisis *time-series*, kita dapat menggunakan repositori publik. Beberapa situs terbaik untuk mengunduh dataset IoT dan Time-Series adalah:
1. **Kaggle** (https://www.kaggle.com/datasets)
2. **UCI Machine Learning Repository** (https://archive.ics.uci.edu/ml/datasets.php)
3. **Mendeley Data** atau repositori terbuka lainnya.

### Contoh Dataset Time-Series Klasik
Untuk memahami struktur data, kita sering menggunakan dataset klasik dunia, seperti **Dataset Air Passengers** (Penumpang Pesawat).
Dataset ini mencatat jumlah penumpang maskapai internasional setiap bulan dari tahun 1949 hingga 1960. 
- **Mengapa dataset ini klasik?** Karena dataset ini sangat jelas menampilkan komponen **Trend** (jumlah penumpang terus naik tiap tahun) dan komponen **Seasonality** (puncak jumlah penumpang selalu terjadi di pertengahan tahun / musim liburan).
- **Format:** Biasanya berbentuk `CSV` dengan dua kolom: `Month` (Waktu) dan `Passengers` (Nilai).

Contoh lainnya untuk konteks IoT adalah **Beijing PM2.5 Data** (dari UCI Repo), yang mencatat data cuaca dan kualitas udara (time-series setiap jam) yang sangat baik untuk dianalisis pola suhu, kelembapan, dan polusinya.

---

## 4. Perangkat Lunak (Software) Analisis Time-Series

Untuk membaca, mengolah, dan melihat karakteristik (*Exploratory Data Analysis*) pada dataset *time-series*, industri data analitik IoT sangat bergantung pada ekosistem **Python** dan eksekusi interaktif menggunakan **Jupyter Notebook** atau layanan *cloud* seperti **Google Colab**.

### A. Library Utama yang Akan Digunakan
1. **Pandas:** *Library* andalan untuk membaca *file* (CSV, Excel) dan memanipulasi data tabular ke dalam bentuk *DataFrame*. Sangat *powerful* untuk memanipulasi format tanggal dan waktu (`datetime`).
2. **Matplotlib & Seaborn:** Digunakan untuk memvisualisasikan data (menggambar grafik garis/grafik sebar) guna melihat tren dan musim secara visual.
3. **Statsmodels:** Digunakan untuk pengolahan statistik tingkat lanjut, seperti mendekomposisi grafik *time-series* menjadi *Trend*, *Seasonality*, dan *Noise*.

### B. Alternatif 1: Menggunakan Google Colaboratory (Google Colab)
Bagi mahasiswa yang tidak ingin melakukan instalasi lokal, dapat menggunakan platform *cloud* Google Colab.
1. Buka browser dan akses [colab.research.google.com](https://colab.research.google.com/).
2. Login menggunakan akun Google.
3. Klik **New Notebook**.
4. Di Google Colab, *library* seperti pandas dan matplotlib sudah terinstal secara default. Anda hanya perlu mengunggah (*upload*) file dataset CSV ke *file explorer* di samping kiri layar sebelum menjalankan kode.

### C. Alternatif 2: Instalasi Standar Python & Jupyter di Windows (Lokal)
Jika ingin menjalankan secara lokal di laptop (Host OS):
1. Unduh **Python standar** versi terbaru (3.x) dari situs resmi [python.org/downloads](https://www.python.org/downloads/).
2. Jalankan *installer*, **pastikan opsi "Add Python to PATH" dicentang**, lalu selesaikan instalasi.
3. Buka *Command Prompt* (CMD) dan jalankan perintah instalasi Jupyter dan *library* yang dibutuhkan:
   ```bash
   pip install jupyter pandas matplotlib seaborn statsmodels
   ```
4. Setelah instalasi selesai, jalankan perintah berikut di CMD untuk membuka antarmuka Jupyter di *browser*:
   ```bash
   jupyter notebook
   ```

### D. Alternatif 3: Instalasi Jupyter di Ubuntu Server Headless (Akses Remote)
Sebagai kelanjutan praktikum Pekan ke-2, kita dapat menjalankan Jupyter di Ubuntu Server (Guest OS di VirtualBox) dan mengaksesnya secara *remote* dari Windows (Host).
1. **Akses Server Menggunakan SSH:** 
   Seperti yang telah dibahas pada modul pekan lalu, sangat disarankan mengakses Ubuntu secara *remote* menggunakan *Command Prompt* (CMD) bawaan Windows agar memudahkan *copy-paste* teks perintah. Anda tidak perlu menginstal PuTTY. Buka CMD dan jalankan:
   ```bash
   ssh nama_user@ip_address_ubuntu
   # Contoh: ssh praktikan@192.168.1.100
   ```
2. Update sistem dan install paket instalasi Python:
   ```bash
   sudo apt update
   sudo apt install -y python3 python3-pip python3-venv python-is-python3
   ```
   *(Penjelasan: Paket `python-is-python3` dipasang agar setiap kali kita mengetik perintah `python`, sistem Ubuntu akan otomatis mengarahkannya ke `python3`. Hal ini penting untuk mencegah eror kompatibilitas pada skrip-skrip yang seringkali hanya memanggil `python` saja).*
3. Buat dan aktifkan *virtual environment*:
   ```bash
   python -m venv jupyter-env
   source jupyter-env/bin/activate
   ```
   *(Penjelasan: Kita diwajibkan membuat *virtual environment* (*venv*) untuk proyek yang dikerjakan agar setiap library yang diinstall (seperti pandas, jupyter) terisolasi secara lokal dan tidak mengotori / merusak *package* python bawaan sistem operasi ubuntu).*
4. Install Jupyter beserta *library* pengolahan data:
   ```bash
   pip install jupyter pandas matplotlib seaborn statsmodels
   ```
5. Buat konfigurasi Jupyter dan atur *password* keamanan:
   ```bash
   jupyter notebook --generate-config
   jupyter notebook password
   ```
   *(Masukkan password saat diminta. Ini akan digunakan saat mengaksesnya di browser Windows).*
6. Jalankan Jupyter Notebook dengan mode yang memungkinkan akses dari luar (`0.0.0.0`) dan tanpa membuka *browser* karena Ubuntu berjalan *headless*:
   ```bash
   jupyter notebook --ip=0.0.0.0 --port=8888 --no-browser
   ```
7. Buka browser di Windows (Host), lalu ketikkan **`http://[IP_UBUNTU]:8888`** (Ganti `[IP_UBUNTU]` dengan IP VM Anda).
8. Masukkan *password* yang baru saja Anda buat untuk mulai menggunakan Jupyter.

---

## 5. Mengenal Antarmuka dan Fitur Jupyter Notebook

Setelah berhasil masuk ke dalam Jupyter Notebook, Anda akan dihadapkan pada beberapa fitur utama yang sangat memudahkan proses analisis data:

### A. Konsep Notebook: Laporan (Markdown) dan Kode (Python) Terpadu
Kelebihan utama file berformat `.ipynb` adalah kemampuannya untuk menggabungkan laporan naratif dan eksekusi kode dalam satu tempat secara interaktif. Terdapat dua jenis sel (*cell*) utama yang sering digunakan:
- **Sel Code:** Digunakan untuk menulis dan menjalankan skrip Python. Output (hasil dari fungsi `print()`, tabel dataset, atau gambar grafik) akan langsung muncul tepat di bawah sel tersebut setelah dieksekusi (ditekan *Run* atau `Shift + Enter`). Anda tidak perlu membuat banyak file skrip `.py` terpisah.
- **Sel Markdown:** Digunakan untuk menulis dokumentasi, penjelasan, teori, atau laporan menggunakan format teks *Markdown* (mendukung teks tebal, miring, poin-poin, hingga persamaan matematika). Dengan cara ini, Anda tidak perlu memisahkan antara kode program dan file laporan (seperti *Microsoft Word*). Semua disusun kronologis dalam satu *notebook*.

### B. Panduan Sintaks Dasar Markdown untuk Laporan
Saat menulis laporan analisis pada **Sel Markdown**, Anda dapat menggunakan kode pemformatan teks sederhana berikut ini untuk mempercantik tampilan dokumen:
- **Heading (Judul):** Gunakan simbol `#` (H1/Judul Utama), `##` (H2/Sub-judul), atau `###` (H3). Contoh penulisan: `## Hasil Eksperimen`
- **Teks Tebal (Bold) & Miring (Italic):** Gunakan *double asterisk* `**teks ini tebal**`, dan *single asterisk* `*teks ini miring*`.
- **Daftar/Poin (List):** Gunakan tanda strip `- ` untuk *bullet list* atau angka `1. ` untuk urutan nomor. Jangan lupa beri spasi setelah simbol.
- **Blok Kode (Code Snippet):** Jika Anda ingin menyalin referensi kode ke laporan tanpa mengeksekusinya, apit kode Anda dengan simbol *backtick* sebanyak tiga kali ( \`\`\` ).
- **Menyisipkan Gambar:** Jupyter mendukung fitur *drag-and-drop*! Anda cukup menarik dan menjatuhkan file gambar dari komputer ke dalam sel Markdown yang sedang di-edit.
- **Rumus Matematika (LaTeX):** Jika laporan memerlukan rumus statistik, apit rumus tersebut dengan simbol dolar. Contoh: `$\mu = \frac{\sum x}{n}$`.

### C. Manajemen File dan Sudut Pandang Workspace Jupyter
Saat pertama kali membuka Jupyter, antarmuka utamanya berupa *File Browser* (Dashboard). 
Penting dipahami bahwa **Jupyter hanya akan menampilkan folder dan file yang berada di dalam atau di bawah direktori tempat perintah `jupyter notebook` tersebut dijalankan** di terminal Linux. Jupyter terkurung secara lokal di direktori tersebut.
- Sebagai contoh, jika Anda menjalankan Jupyter dari dalam folder `/home/praktikan/`, maka Jupyter hanya bisa "melihat" isi folder tersebut ke bawah.
- Jika Anda mengikuti langkah instalasi *virtual environment* sebelumnya, Anda akan melihat folder bernama `jupyter-env` di halaman utama Jupyter. Meskipun dari Jupyter ini terlihat seperti folder biasa yang berisi banyak subfolder (`bin`, `lib`), **jangan pernah menghapus, mengubah nama, atau menyimpan laporan/dataset Anda ke dalam folder *env* ini**. Folder *env* tersebut adalah jantung yang dikelola otomatis oleh sistem instalasi Python dan harus dibiarkan apa adanya.

### D. Terminal Terintegrasi (Integrated Terminal)
Selain mengeksekusi Python melalui *notebook*, Jupyter juga memiliki fasilitas Terminal Linux bawaan. Anda dapat membuka terminal dengan cara mengklik tombol **New -> Terminal** di bagian kanan atas halaman utama (Dashboard) Jupyter.
Fasilitas ini memiliki beberapa keunggulan:
- Berfungsi sama persis layaknya terminal SSH. Anda bisa menjalankan perintah OS dasar seperti `ls`, `mkdir`, atau berpindah direktori.
- Anda bisa memanggil perintah dari sistem operasi, misalnya menginstal paket dari repositori Ubuntu (`sudo apt install nama-paket`) yang mana perintah tersebut tidak bisa dan tidak ideal jika dieksekusi dari dalam sel *notebook* atau via `pip`.
- Sangat praktis untuk mengelola file sistem Linux tanpa perlu membuka dua jendela terpisah antara *browser* (Jupyter) dan jendela *Command Prompt* SSH.

---

## 6. Langkah Praktikum: Membaca dan Melihat Karakter Dataset

Setelah instalasi selesai, ikuti langkah berikut di Jupyter Notebook untuk membaca dan memplot dataset *time-series* klasik.

1. **Menyiapkan Dataset ke dalam Lingkungan Kerja:**
   Jika Anda menggunakan Windows lokal atau Google Colab, cukup letakkan/unggah file `AirPassengers.csv` di folder kerja Anda. Namun, jika menggunakan **Ubuntu Server (VirtualBox)**, Anda harus memasukkan file tersebut ke dalam Ubuntu melalui salah satu cara berikut di terminal:
   
   - **Cara A (Download Langsung dengan `wget`):** 
     Anda bisa mengunduh file CSV langsung dari repositori internet (contoh link GitHub):
     ```bash
     wget https://raw.githubusercontent.com/jbrownlee/Datasets/master/airline-passengers.csv -O AirPassengers.csv
     ```
   - **Cara B (Menggunakan Shared Folder VirtualBox):**
     1. Pada aplikasi VirtualBox (Host), buka **Settings** VM Ubuntu Anda -> **Shared Folders**.
     2. Tambahkan folder Windows yang berisi file dataset tersebut, lalu centang opsi **Auto-mount** dan klik OK.
     3. Di terminal Ubuntu, berikan hak akses pada *user* Anda untuk membaca *shared folder* tersebut:
        ```bash
        sudo usermod -aG vboxsf $USER
        ```
        *(Catatan: Anda mungkin perlu logout atau `sudo reboot` agar hak akses grup ini aktif).*
     4. Folder tersebut akan di-*mount* secara otomatis di `/media/sf_<nama_folder>`. Salin file CSV ke direktori kerja Anda saat ini:
        ```bash
        cp /media/sf_namafolder/AirPassengers.csv .
        ```

2. Buka antarmuka Jupyter Notebook di *browser*, lalu buat *file* notebook baru (Python 3).
3. Tuliskan kode berikut untuk **membaca data**:

```python
import pandas as pd
import matplotlib.pyplot as plt

# 1. Membaca dataset CSV
# Mengubah kolom 'Month' menjadi tipe datetime dan menjadikannya Index
df = pd.read_csv('AirPassengers.csv', parse_dates=['Month'], index_col='Month')

# Menampilkan 5 baris pertama data
print("Melihat struktur data:")
display(df.head())
```

4. Menulis kode untuk **melihat karakteristik (visualisasi)**:

```python
# 2. Membuat plot time-series dasar
plt.figure(figsize=(10,5))
plt.plot(df.index, df['Passengers'], marker='o', linestyle='-', color='b')
plt.title('Grafik Jumlah Penumpang Pesawat (1949-1960)')
plt.xlabel('Tahun')
plt.ylabel('Jumlah Penumpang')
plt.grid(True)
plt.show()
```
*Analisis:* Pada plot yang dihasilkan, mahasiswa dapat langsung mengidentifikasi adanya pola yang selalu berulang (musiman) dan kecenderungan grafik yang naik (trend).

5. (Opsional) Menguraikan komponen Time-Series menggunakan `statsmodels`:
```python
from statsmodels.tsa.seasonal import seasonal_decompose

# Mendekomposisi data menjadi komponen dasar
result = seasonal_decompose(df['Passengers'], model='multiplicative')
result.plot()
plt.show()
```

---

## 7. Menyimpan dan Ekspor Laporan (Jupyter Notebook ke PDF/Markdown)

Seringkali saat mahasiswa mencoba mengekspor hasil kerja di Jupyter (`.ipynb`) ke PDF melalui menu *Download as PDF*, akan muncul pesan **error**. Hal ini terjadi karena sistem host belum memiliki modul *Pandoc* atau distribusi *LaTeX* (seperti MiKTeX/TeX Live) yang menjadi syarat wajib konversi tersebut. 

Untuk memudahkan pembuatan laporan, Anda dapat menggunakan beberapa metode alternatif berikut:

### A. Ekspor ke PDF (Cara Termudah - Tanpa Install LaTeX)
Metode ini adalah cara terbaik mengekspor *notebook* beserta hasil grafik tanpa memicu *error*:
1. Di Jupyter Notebook, klik menu **File -> Print Preview**.
2. Akan terbuka tab baru di *browser* yang menampilkan hasil kerja Anda dalam halaman statis.
3. Tekan **Ctrl + P** (atau menu *Print*) pada *browser* Anda.
4. Ubah opsi pencetak (*Destination/Printer*) menjadi **Save as PDF**.
5. Pada bagian pengaturan tambahan (*More settings*), centang **Background graphics** agar warna/grafik tercetak sempurna, dan hilangkan centang *Headers and footers*.
6. Klik **Save**.

### B. Ekspor ke File Markdown (.md)
Jika format pelaporan disyaratkan dalam bentuk *Markdown*:
1. Klik menu **File -> Download as -> Markdown (.md)**.
2. Hasil ekstraksi ini dapat langsung dibaca oleh *Markdown viewer* standar, sangat cocok dilampirkan ke dalam repositori GitHub.

### C. Peringatan: Error "500 Internal Server Error" Akibat Front Matter
Saat menulis laporan identitas di dalam sel Markdown Jupyter Notebook, **jangan menggunakan YAML Front Matter** (blok teks yang diapit oleh tanda `---` di awal dokumen, seperti yang sering dilakukan di file `.md`). 
Jupyter menggunakan *Pandoc* untuk mengonversi *notebook* ke format PDF. Jika *Pandoc* menemukan blok `---` di dalam sel, ia akan salah mengartikannya sebagai *metadata document* dan menyebabkan *parser* internal *crash* (menghasilkan *error* merah di *browser*: `500: Internal Server Error - JSONDecodeError`).

**Solusi:** Hapus tanda `---` dari sel Markdown Jupyter Anda.
**Catatan Penting Penulisan Markdown:** Saat Anda menghapus tanda `---`, Anda mungkin mendapati tulisan Nama dan NIM Anda menjadi menyambung ke samping (tidak turun ke bawah). Hal ini karena dalam Markdown, menekan tombol *Enter* satu kali **tidak akan** membuat baris baru. Agar teks turun ke bawah (*line break*), Anda **wajib menambahkan dua spasi kosong** di akhir kalimat sebelum menekan *Enter*.

*Contoh penulisan identitas yang benar:*
```text
# Laporan Praktikum Pekan 3
**Nama:** M Riza Nurtam  (tambahkan 2 spasi di akhir kalimat ini)
**NIM:** 33222005  (tambahkan 2 spasi di akhir kalimat ini)
**Dosen Pengampu:**  (tambahkan 2 spasi di akhir kalimat ini)
- Bapak M. Riza Nurtam
- Ibu Ega Evinda Putri
```

### D. Solusi Mengatasi Error Dependensi Ekspor (Untuk Ubuntu Server)
Jika Anda menggunakan *remote* Ubuntu Server (Alternatif 3) dan tetap tidak bisa mengekspor ke PDF meskipun sudah menghapus *front matter*, berarti *server* Anda kekurangan paket dependensi mesin *LaTeX*. Install paket berikut via terminal:
```bash
sudo apt update
sudo apt install -y pandoc texlive-xetex texlive-fonts-recommended texlive-plain-generic
```
Setelah instalasi selesai (mungkin memakan waktu karena ukurannya ratusan MB), Anda dapat melakukan konversi PDF dari menu *browser* atau langsung menggunakan perintah CLI:
```bash
jupyter nbconvert --to pdf nama_file_anda.ipynb
```

---



---

## 9. Referensi
[1] Fawcett, T., & Provost, F. (2013). *Data Science for Business*. O'Reilly Media. (ISBN-13: 978-1449361327)
[2] Python Software Foundation, "Python Documentation," [Online]. Tersedia: https://docs.python.org/
[3] Google, "Google Colaboratory," [Online]. Tersedia: https://colab.research.google.com/
[4] Jupyter Project, "Project Jupyter Documentation," [Online]. Tersedia: https://jupyter.org/documentation
[5] Pandas Development Team, "Time series / date functionality," [Online]. Tersedia: https://pandas.pydata.org/docs/
