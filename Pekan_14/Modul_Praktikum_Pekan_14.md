# Materi Praktikum Pekan 14: Integrasi End-to-End (PjBL)

## MODUL 14: Integrasi End-to-End (PjBL)
**Tujuan:** Menggabungkan seluruh tahapan (*Ingestion, Cleaning/ML, Storage, Dashboard*) menjadi satu sistem utuh yang berjalan otomatis 24/7.

### 14.1 Arsitektur Final Proyek Akhir
Mahasiswa tidak lagi mengerjakan script terpisah, melainkan membuat file *orchestrator* utama `main_pipeline.py`.

**Alur Logika Program:**
1. **MQTT Subscribe:** Menunggu paket data sensor (atau *dummy* data) masuk.
2. **Data Cleaning & ML (On-the-fly):**
   - Periksa apakah nilainya kosong (NaN).
   - Masukkan ke model ML (*Isolation Forest*) yang sudah dilatih (di-*load* menggunakan modul `pickle` Python).
   - Jika anomali = -1 (berbahaya) atau 1 (normal).
3. **Data Storage:** Simpan nilai suhu, kelembaban, dan "status anomali" tersebut ke InfluxDB.
4. **Visualisasi (Grafana):**
   - Grafana membaca "status anomali". Jika nilainya -1, Grafana memicu *Webhook* otomatis ke Telegram Manajer Kebun.

**Progress Review:** Dosen/Asisten mengecek perkembangan implementasi kode pipeline secara keseluruhan pada tiap kelompok mahasiswa.
