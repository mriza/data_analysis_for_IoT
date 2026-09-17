# Pekan 8: Evaluasi Tengah Semester (UTS) (Praktikum)

## MODUL 8: Ujian Tengah Semester (Praktik Lab)

### Tata Tertib Ujian Praktikum
- Ujian bersifat *Open Book* dan *Open Source Code*.
- Mahasiswa akan diberikan sebuah tautan *G-Drive* berisi file CSV Dataset IoT Mentah (contoh: Kualitas Udara / Air) yang mengandung cacat yang sengaja disuntikkan oleh dosen (nilai kosong dan *outlier* tak wajar).
- Waktu pengerjaan total: 170 Menit.

### Tugas yang Harus Diselesaikan:
1. **(Bobot 30%) Data Ingestion & Loading:** Menggunakan Pandas, memuat dataset tersebut dan memastikannya memiliki format waktu (datetime) sebagai indeks yang valid.
2. **(Bobot 40%) Pre-processing & Cleaning:**
   - Menghitung persentase baris data yang cacat (*missing*).
   - Melakukan metode pembersihan atau pengisian data kosong dengan *justifikasi metodologi* (memberi komentar di kodingan, mengapa memilih interpolasi dibanding *ffill*).
   - Membuat model penyaring statistik sederhana (seperti batasan IQR) untuk me-*remove outlier*.
3. **(Bobot 30%) Exploratory Data Analysis (EDA):** Menampilkan plot korelasi *Heatmap* untuk minimal 3 parameter fisik lingkungan yang ada pada dataset dan menjelaskan korelasi tertingginya dalam 1 paragraf pada *Markdown Jupyter Notebook*.

Dosen pengampu akan mengumpulkan file berformat `.ipynb` (*Jupyter Notebook*) dari setiap peserta di akhir jam praktikum.
