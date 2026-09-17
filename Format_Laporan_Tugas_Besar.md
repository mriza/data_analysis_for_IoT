---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "STANDAR FORMAT LAPORAN TUGAS BESAR (PROYEK AKHIR)"
---

# STANDAR FORMAT LAPORAN TUGAS BESAR (PROYEK AKHIR)
**Mata Kuliah:** Analisis Data untuk IoT  
**Program Studi:** Teknologi Rekayasa Komputer (TRK)  
**Institusi:** Politeknik Pertanian Negeri Payakumbuh

---

Laporan ini dikerjakan secara berkelompok dan dikumpulkan pada pekan Ujian Akhir Semester (UAS). Minimal ketebalan laporan adalah 7 halaman (di luar lampiran).

**SAMPUL LAPORAN (COVER)**
- Logo Institusi
- Judul Proyek: "IMPLEMENTASI PIPELINE DATA IOT UNTUK [NAMA KASUS/STUDI]"
- Identitas Kelompok: Nama Kelompok, Nama Anggota & NIM
- Dosen Pengampu

**ABSTRAK**
*(Satu paragraf singkat (maks. 250 kata) yang merangkum keseluruhan proyek: apa masalah utamanya, bagaimana arsitektur teknis penyelesaiannya, dan seperti apa hasil operasional dari sistem peringatan dini / dashboard yang dibangun).*

**BAB I: PENDAHULUAN**
- **1.1 Latar Belakang:** Mengapa studi kasus ini diangkat? (Misal: Kasus gagal panen akibat suhu ekstrem di pertanian cabai, atau masalah pemantauan pH di tambak ikan).
- **1.2 Rumusan Masalah:** Apa metrik atau perilaku anomali spesifik yang ingin dipecahkan/dipantau?
- **1.3 Tujuan Proyek:** Apa hasil akhir yang diharapkan dari implementasi 5 lapis arsitektur ini?

**BAB II: ARSITEKTUR SISTEM & DASAR TEORI**
- **2.1 Tinjauan Pustaka:** Referensi singkat terkait protokol MQTT/AMQP/Valkey, jenis *Time-series Database* yang dipilih, dan fungsi pustaka Python yang dipakai.
- **2.2 Desain Arsitektur Sistem (Wajib):** Menyertakan diagram blok (bagan) aliran data *End-to-End* yang dengan jelas menggambarkan kelima komponen wajib beserta pemisahan jenis protokolnya: 
  *(Data Collector --[MQTT]--> Message Broker --[AMQP/Valkey]--> Data Pipeline Worker -> Time-Series Database -> Analytic Dashboard).*

**BAB III: METODOLOGI DAN IMPLEMENTASI**
- **3.1 Data Collector & Message Broker:** Penjelasan teknis mengenai metode perolehan data sensor (fisik atau simulasi), kecepatan interval pengiriman (*delay*), dan *Topic/Queue* MQTT, AMQP, atau Valkey yang dikonfigurasi.
- **3.2 Data Pipeline Worker (Pemroses Utama):** Penjelasan logika kode Python (Subscriber). Bagaimana cara *script* melakukan *cleaning* data (menangani nilai kosong/outlier) secara waktu-nyata (*real-time*) sebelum memindahkannya ke *database*.
- **3.3 Konfigurasi Time-Series Database:** Alasan pemilihan *database* (misal: mengapa InfluxDB atau MongoDB), skema penyimpanan, dan sintaks struktur datanya.
- **3.4 Analytic Dashboard:** Desain *query* dari *database* ke *dashboard* (misalnya Flux Query atau SQL), dan fitur *alerting* apa saja yang disetel (contoh: notifikasi peringatan suhu dikirim ke Telegram).

**BAB IV: HASIL DAN PEMBAHASAN**
- **4.1 Hasil Tangkapan Layar (Live Demo):** Menyertakan *Screenshot Dashboard* (Grafana/Metabase/Streamlit) yang menangkap kejadian tren atau anomali yang terjadi.
- **4.2 Analisis Kinerja Sistem:** Evaluasi latensi pengiriman data, kestabilan *pipeline worker* (apakah pernah putus/crash?), dan akurasi/waktu respons dari fitur *Alert*.
- **4.3 Kendala dan Pemecahan:** Hambatan teknis yang ditemui kelompok saat pengerjaan proyek dan bagaimana cara mengatasinya.

**BAB V: KESIMPULAN DAN SARAN**
- **5.1 Kesimpulan:** Menjawab keberhasilan tujuan proyek dari Bab I berdasarkan bukti tangkapan layar di Bab IV.
- **5.2 Saran:** Rekomendasi perbaikan sisi keamanan arsitektur atau peningkatan skalabilitas untuk implementasi nyata di masa depan.

**DAFTAR PUSTAKA**
*(Menggunakan format penulisan standar IEEE atau APA)*

**LAMPIRAN**
*(Wajib melampirkan tautan/link repositori kode GitHub kelompok, link video demonstrasi YouTube, dan tabel matriks kontribusi / pembagian tugas per anggota).*
