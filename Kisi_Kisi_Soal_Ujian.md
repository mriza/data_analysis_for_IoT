---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Kisi-kisi Soal Ujian"
---

# Kisi-kisi Soal Ujian
**Mata Kuliah:** Analisis Data untuk IoT
**Program Studi:** Teknologi Rekayasa Komputer (TRK), Politeknik Pertanian Negeri Payakumbuh
**Semester:** 5

Dokumen ini berisi panduan dan kisi-kisi soal untuk Evaluasi Tengah Semester (UTS) dan Evaluasi Akhir Semester (UAS).

---

## 1. Kisi-kisi Evaluasi Tengah Semester (UTS)
**Bentuk Ujian:** Ujian Tulis (Sinkronus) dan Studi Kasus Terjadwal
**Durasi:** 100 Menit
**Sifat:** Tutup Buku / Buka Buku (Tergantung Kebijakan Dosen)

| No | Kompetensi / Materi | Indikator Soal | Tingkat Kesulitan (Bloom) |
|---|---|---|---|
| 1 | Konsep Dasar Analitik IoT | Mahasiswa mampu menjelaskan perbedaan antara arsitektur komputasi Edge, Fog, dan Cloud dalam konteks pemrosesan data IoT. | Pemahaman (C2) |
| 2 | Karakteristik Data IoT | Mahasiswa mampu mengidentifikasi karakteristik 3V (Volume, Velocity, Variety) berdasarkan sebuah studi kasus sistem pemantauan suhu ruangan server. | Analisis (C4) |
| 3 | Akuisisi Data & MQTT | Mahasiswa mampu menuliskan dan menjelaskan fungsi dari baris perintah untuk publikasi (`pub`) dan berlangganan (`sub`) pada protokol MQTT. | Penerapan (C3) |
| 4 | Anatomi Time-Series | Diberikan sebuah grafik data suhu bulanan, mahasiswa mampu menunjukkan bagian *Trend*, *Seasonality*, dan *Noise*. | Analisis (C4) |
| 5 | Pra-pemrosesan Data (*Smoothing*) | Diberikan tabel data historis sensor yang hilang (Missing Value), mahasiswa mampu menghitung nilai pengganti menggunakan metode *Moving Average*. | Penerapan (C3) |
| 6 | Deteksi Anomali Statistik | Diberikan sekumpulan data pembacaan sensor kelembaban tanah, mahasiswa mampu mengidentifikasi nilai outlier (pencilan) menggunakan metode Z-score atau rentang interkuartil (IQR). | Evaluasi (C5) |
| 7 | Edge Analytics | Mahasiswa mampu memberikan argumentasi terkait kapan proses filtering data sebaiknya dilakukan di Edge dibandingkan di Cloud. | Evaluasi (C5) |

---

## 2. Kisi-kisi Evaluasi Akhir Semester (UAS)
*(Sesuai RPS, UAS berupa Pengumpulan Final Project. Berikut adalah rubrik/kisi-kisi penilaian Final Project)*

**Bentuk Ujian:** Penilaian Proyek Akhir dan Laporan Komprehensif
**Cakupan:** Pembuatan *End-to-End Pipeline* dari Data Dummy -> Broker -> Analisis/ML -> Dashboard.

### Aspek Penilaian Proyek Akhir:
1. **Arsitektur dan Akuisisi Data (20%)**
   - Apakah simulasi data sensor berjalan dengan frekuensi yang tepat?
   - Apakah koneksi MQTT Broker stabil dan log data terekam dengan baik?
2. **Pemrosesan Data dan Analisis (30%)**
   - Apakah data sensor telah melewati tahapan *cleaning* (penanganan *missing value / outlier*)?
   - Apakah implementasi Machine Learning (*Forecasting / Anomaly Detection*) berjalan sesuai dengan dataset yang masuk?
3. **Visualisasi Data dan Dashboard (30%)**
   - Apakah data berhasil disimpan di Time-Series Database (InfluxDB)?
   - Apakah *Dashboard* Grafana menampilkan data secara *real-time*?
   - Apakah fitur *Alerting* (notifikasi) berfungsi dan dipicu oleh kondisi anomali?
4. **Dokumentasi dan Presentasi (20%)**
   - Kualitas penulisan Laporan Akhir (Sistematika, Analisis Pembahasan).
   - Kejelasan saat presentasi dan *Live Demo* sistem.

---
*Disetujui Oleh:*
*Koordinator Mata Kuliah*
*Mohammad Riza Nurtam*
