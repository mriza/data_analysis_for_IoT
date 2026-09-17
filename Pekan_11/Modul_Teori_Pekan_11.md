# Materi Teori Pekan 11: Visualisasi Data IoT & Konsep Time-Series Database

## Prinsip UI/UX dalam Desain Dashboard Industri
Sebuah *Dashboard* (seperti Grafana) yang baik harus mematuhi prinsip visibilitas operasional:
- **Jangan menumpuk grafik:** Gunakan *Single Stat* atau *Gauge* untuk informasi *real-time* utama (suhu saat ini).
- **Gunakan warna berkonteks:** Merah untuk bahaya/kritis, hijau untuk normal.
- **Hirarki Visual:** Letakkan peringatan di bagian paling atas *dashboard*.

## EWS (Early Warning System) dan Mitigasi
Analisis data tidak berguna jika tidak menghasilkan aksi (Actionable Insight).
*Dashboard* harus diintegrasikan dengan *Alerting System* (Sistem Peringatan Dini) menggunakan mekanisme *Webhook*. Misalnya, mengirimkan pesan *Telegram* atau *WhatsApp* ke ponsel manajer kebun saat *Machine Learning* memprediksi tanaman akan layu dalam 6 jam.
