---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Teori Pekan 13: Arsitektur Sistem Terpadu & Proyek Akhir"
---

# Materi Teori Pekan 13: Arsitektur Sistem Terpadu & Proyek Akhir

## Review Metodologi Proyek Enterprise
Dalam skala enterprise/industri, arsitektur IoT Analytics menggunakan prinsip *Microservices* dan skalabilitas:
1. **Perangkat Sensor (Edge):** Mengirim MQTT.
2. **Broker (Message Hub):** Apache Kafka atau HiveMQ.
3. **Database Time-Series:** InfluxDB atau TimescaleDB.
4. **Analitik Engine:** Skrip Python/Spark untuk pembersihan dan deteksi.
5. **Aplikasi Presentasi:** Grafana atau aplikasi web khusus. 

Mahasiswa merangkum seluruh prinsip ini melalui Proyek Akhir. Arsitektur yang harus dirancang mencakup:
- Pemilihan dataset yang akan digunakan atau simulasi perangkat sensor IoT yang akan dibangun.
- Menentukan algoritma pre-processing yang dibutuhkan.
- Merancang sistem Notifikasi/Alerting yang tepat.
