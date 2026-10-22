---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Praktikum Pekan 15: Pengujian Keseluruhan Sistem, Data Storytelling, dan Presentasi Live Demo Proyek Akhir"
---

# Materi Praktikum Pekan 15: Pengujian Keseluruhan Sistem, Data Storytelling, dan Presentasi Live Demo Proyek Akhir

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 15 (Presentasi & Demonstrasi Langsung Proyek Akhir — $1 \times 170$ Menit)  
**Capaian Pembelajaran (Sub-CPMK 15):** Mahasiswa mampu melakukan pengujian penerimaan sistem secara menyeluruh (*End-to-End System & Fault-Injection Testing*), menyajikan temuan analitik menggunakan kerangka komunikasi **Data Storytelling**, serta mendemonstrasikan arsitektur *pipeline* IoT yang berjalan secara waktu nyata (*Live Demo*) beserta prosedur keselamatan cadangan (*Engineering Fallback Ethics*).

---

## 1. Tujuan Pembelajaran & Praktikum
1. Mahasiswa mampu melaksanakan **5 Tahap Pengujian Integrasi & Ketahanan Sistem (*System Acceptance & Fault-Injection Test*)** sebelum demonstrasi di depan penguji.
2. Mahasiswa mampu menyusun narasi **Data Storytelling** menggunakan kerangka **SCR (*Situation – Complication – Resolution & Business Impact*)** yang menerjemahkan metrik statistik/ML menjadi nilai ekonomi pertanian yang mudah dipahami oleh pengambil keputusan non-teknis.
3. Mahasiswa mampu menjelaskan aspek **Etika Rekayasa (*Engineering Ethics*) & Prosedur Keselamatan Cadangan (*Fail-Safe Fallback*)** pada sistem otomatis berbasis AI.
4. Mahasiswa mampu mempresentasikan dan melakukan **Live Demo** sistem Proyek Akhir secara tim dengan pembagian peran yang jelas (**Bobot Penilaian Presentasi & Demo RPS: 10%**).

---

## 2. Protokol Pengujian Keseluruhan Sistem (*Pre-Demo Acceptance Checklist*)

Sebelum kelompok maju melakukan *Live Demo*, sistem *pipeline* wajib lulus **5 Skenario Uji Ketahanan (*Fault-Injection Testing*)** berikut ini:

| No | Nama Skenario Uji | Prosedur Pengujian (*Action*) | Indikator Keberhasilan Sistem (*Expected Pass Result*) |
| :---: | :--- | :--- | :--- |
| **1** | **Uji Aliran Normal (*Baseline Flow Test*)** | Jalankan `sensor_simulator_e2e.py` (atau nyalakan ESP32) dan `main_pipeline.py`. | Data baru muncul di terminal Worker dan menggerakkan grafik garis Grafana dalam waktu $< 5\text{ detik}$ tanpa *error*. |
| **2** | **Uji Ketahanan Data Kosong (*Missing Value Injection*)** | Kirimkan payload JSON dengan nilai `"suhu": null` melalui MQTT. | `main_pipeline.py` **tidak mengalami *crash***; nilai kosong berhasil ditambal otomatis oleh *Circular Buffer* dan diberi label `IMPUTED`. |
| **3** | **Uji Tangkal Korsleting (*Hard Outlier Injection*)** | Kirimkan payload JSON dengan angka mustahil `"suhu": 999.0` atau `"-50.0"`. | Angka `999.0` ditolak/diamputasi sebelum masuk ke kolom `suhu_bersih` InfluxDB sehingga skala grafik Grafana tidak rusak. |
| **4** | **Uji Deteksi Anomali AI & Webhook (*ML & Alert Trigger Test*)** | Suntikkan kombinasi anomali multivariat (misal suhu tinggi, kelembaban anjlok, pH ekstrem). | Model *Isolation Forest* mengeluarkan vonis `-1`, indikator panel Grafana berubah menjadi **Merah**, dan notifikasi masuk ke **Telegram**. |
| **5** | **Uji Etika Keselamatan (*Fail-Safe Fallback Design*)** | Tanyakan skenario: *"Bagaimana jika server AI mati atau akurasi ML meleset 5%?"* | Kelompok mampu menunjukkan batas pengaman perangkat keras/mekanis (*fallback threshold*) yang menjaga keselamatan lahan/manusia. |

---

## 3. Metodologi *Data Storytelling* untuk Insinyur Analitik IoT

Salah satu kesalahan terbesar insinyur ketika mempresentasikan hasil analisis data di hadapan direktur perusahaan atau petani adalah **hanya membacakan bentuk grafik secara mentah** (contoh: *"Bapak/Ibu, di layar terlihat garis merah naik turun dari 28 ke 35 lalu di sebelah kanan ada kotak warna biru korelasi -0.85"*). Bagi audiens non-programmer, kalimat tersebut tidak menjawab pertanyaan **"Lalu apa artinya bagi kebun kita, dan tindakan apa yang harus kita ambil?"**

**Data Storytelling** adalah keahlian menggabungkan三 (tiga) elemen: **(1) Data & Bukti Statistik**, **(2) Visualisasi yang Fokus**, dan **(3) Narasi Kontekstual (*Domain Knowledge*)** menggunakan kerangka **SCR (*Situation – Complication – Resolution*)**:

```text
+---------------------------------------------------------------------------------------------------+
| 1. SITUATION (Konteks Masalah Lapangan):                                                          |
|    "Pada budidaya melon hidroponik di Greenhouse A, tanaman sangat rentan mengalami layu bakteri   |
|     dan busuk akar apabila suhu larutan nutrisi melampaui 31°C disertai fluktuasi pH di luar 6.0."|
+---------------------------------------------------------------------------------------------------+
                                                 |
                                                 v
+---------------------------------------------------------------------------------------------------+
| 2. COMPLICATION (Temuan Analitik & Anomali dari Data):                                            |
|    "Melalui analisis korelasi Heatmap dan model Isolation Forest selama 7 hari pengamatan, kami   |
|     menemukan bahwa setiap pukul 13.30 siang terjadi penurunan oksigen dan penyimpangan pH yang   |
|     tidak terdeteksi oleh alarm suhu tunggal biasa, karena secara terpisah angkanya tampak wajar."|
+---------------------------------------------------------------------------------------------------+
                                                 |
                                                 v
+---------------------------------------------------------------------------------------------------+
| 3. RESOLUTION & ACTIONABLE IMPACT (Solusi Sistem & Nilai Manfaat):                                |
|    "Dengan menempatkan model Isolation Forest di Pipeline Worker dan mengintegrasikannya ke       |
|     Alert Telegram Grafana (dengan debounce 2 menit), anomali terdeteksi 2 jam lebih cepat        |
|     sebelum tanaman layu, menghemat biaya kerugian panen hingga 80% dan memangkas kuota data."    |
+---------------------------------------------------------------------------------------------------+
```

### Etika Keteknikan (*Engineering Ethics*): Mengapa Sistem AI Tetap Wajib Memiliki *Fallback Manual*?
Dalam rekayasa sistem IoT kritis (seperti peringatan dini tanah longsor, kendali katup bendungan, atau ventilasi kandang tertutup), meskipun model *Machine Learning* Anda memiliki akurasi pengujian **$95\%$**, secara etika keteknikan Anda **dilarang keras menggantungkan $100\%$ keselamatan nyawa atau aset kritis hanya pada prediksi AI** karena:
1. Akurasi $95\%$ berarti masih terdapat **$5\%$ peluang *False Negative* (gagal mendeteksi bencana)**.
2. Sistem digital/server dapat mengalami kegagalan listrik, putus jaringan, atau *Concept Drift* saat terjadi cuaca ekstrem yang belum pernah ada di data latih.
3. **Solusi Etis Wajib (*Defense-in-Depth / Fallback Procedure*):** Selalu rancang lapis pengaman cadangan berbasis perangkat keras/mekanis independen (seperti saklar pelampung mekanis, termostat bimetal analog, atau sirine lokal berbasis batas tegangan fisik di Edge) yang tetap bekerja meskipun server AI pusat mati total!

---

## 4. Struktur Standar Presentasi & Skenario *Live Demo* (15 Menit / Kelompok)

Setiap kelompok diberikan waktu **15 menit** (8 menit presentasi + *live demo*, dan 7 menit tanya jawab penguji) dengan susunan berikut:

1. **Slide 1 — Judul & Latar Belakang Masalah Pertanian (1 Menit):**
   Masalah nyata apa yang diangkat dan mengapa butuh analitik IoT?
2. **Slide 2 — Diagram Arsitektur 5-Lapis End-to-End (1,5 Menit):**
   Penjelasan topologi dari *Data Collector* $\rightarrow$ *Broker* $\rightarrow$ *Pipeline Worker* $\rightarrow$ *TSDB* $\rightarrow$ *Grafana*.
3. **Slide 3 — Metodologi Pra-pemrosesan & Pemodelan Machine Learning (1,5 Menit):**
   Bagaimana cara sistem menangani *missing values*, *outlier*, *smoothing*, serta cara kerja model ML yang dipilih.
4. **Sesi Utama — LIVE DEMO SISTEM BERJALAN (4 Menit):**
   - Tampilkan jendela terminal `sensor_simulator` dan `main_pipeline.py` berdampingan dengan browser **Grafana Dashboard**.
   - Tunjukkan aliran data normal yang bergerak secara *real-time*.
   - **Demonstrasikan penyuntikan anomali (*Fault Injection*) secara langsung** dan buktikan bahwa panel Grafana berubah status serta notifikasi peringatan masuk ke **Telegram**!
5. **Tanya Jawab & Pertanggungjawaban Kode (7 Menit):**
   Dosen pengampu akan menunjuk masing-masing anggota kelompok secara acak untuk menjelaskan baris kode tertentu pada `main_pipeline.py` atau kueri Flux pada Grafana.

---

## 5. Rubrik Penilaian Presentasi & Live Demo (Bobot RPS: 10%)

Penilaian pada Pekan 15 mengacu pada [`../Instrumen_Penilaian.md`](../Instrumen_Penilaian.md):
1. **Keberhasilan Integrasi Pipeline & Live Demo Real-Time (40%):** Seluruh 5 komponen berjalan mulus secara langsung (bukan rekaman video statis) dan tahan terhadap uji injeksi anomali.
2. **Kualitas Visualisasi Dashboard & Fungsi Alerting (25%):** Kepatuhan terhadap prinsip UI/UX ruang kontrol (hirarki visual, kode warna semantik, dan ketepatan pemicu *Alert*).
3. **Ketajaman Data Storytelling & Komunikasi Teknis (20%):** Kemampuan menjelaskan makna fisis data menggunakan kerangka SCR.
4. **Penguasaan Individu Saat Tanya Jawab (15%):** Setiap anggota memahami bagian kode dan arsitektur yang dikerjakannya.

---

## 6. Referensi & Acuan Materi Pekan Ini

1. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730)  
2. **Fawcett, T., & Provost, F. (2013).** *Data Science for Business*. O'Reilly Media. (ISBN-13: 978-1449361327) — **Bab 13 & Bab 14 (Data Science and Business Strategy & Sustaining Analytical Quality).**  
3. **ACM/IEEE-CS. (2020).** *Computing Curricula 2020 (CC2020)*. (ISBN-13: 978-1450390804) — **Professionalism and Engineering Ethics in Computing.**  
4. Lihat dokumen [`../RTM_Tugas_Besar.md`](../RTM_Tugas_Besar.md), [`../Instrumen_Penilaian.md`](../Instrumen_Penilaian.md), dan [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
