---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Praktikum Pekan 7: Praktik Edge Scripting (Efisiensi Memori, Circular Buffer, & Report by Exception)"
---

# Materi Praktikum Pekan 7: Praktik Edge Scripting (Efisiensi Memori, Circular Buffer, & Report by Exception)

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 7 (Suplemen Praktikum Pemrograman Edge Hemat Memori)  
**Capaian Pembelajaran (Sub-CPMK 7):** Mahasiswa mampu mengimplementasikan algoritma pemrosesan aliran data (*Data Streaming*) menggunakan pustaka standar Python murni tanpa Pandas, menerapkan struktur data **Circular Buffer (`collections.deque`)** dan **Single-State EMA** dengan kompleksitas memori konstan $O(1)$, serta menerapkan logika penghematan *bandwidth* **Report by Exception (*Delta Reporting*)**.

---

## 1. Tujuan Praktikum
1. Mahasiswa mampu menyimulasikan lingkungan *Resource-Constrained Edge Computing* menggunakan Python murni tanpa memuat pustaka raksasa seperti Pandas ke dalam RAM.
2. Mahasiswa mampu membuktikan perbedaan konsumsi memori antara pendekatan *Batch Loading* (`list` tanpa batas) dan pendekatan *Streaming Circular Buffer* (`collections.deque(maxlen=k)`) yang menjaga pemakaian RAM tetap konstan ($O(1)$ *space complexity*).
3. Mahasiswa mampu mengimplementasikan algoritma **Streaming Exponential Moving Average (EMA)** yang hanya membutuhkan penyimpanan 1 variabel angka di memori.
4. Mahasiswa mampu memprogram mekanisme **Report by Exception (*Delta Reporting*) + Heartbeat** dan menghitung persentase penghematan *bandwidth* transmisi jaringan yang dihasilkan.

---

## 2. Persiapan Alat & Prasyarat
- **Perangkat Lunak:** Terminal Linux / CMD Windows / VS Code / Jupyter Notebook.
- **Pustaka Python:** Hanya menggunakan modul bawaan (*built-in standard library*) Python: `csv`, `time`, `statistics`, `collections`, `sys`, `math`, dan `random` (sangat ringan, berjalan di papan SBC/MicroPython tanpa instalasi `pip`).
- **Dataset:** Berkas `data_sensor_dummy.csv` (akan kita bangkitkan sebanyak **50.000 baris** pada Langkah 0 untuk menguji ketahanan memori).

---

## 3. Dasar Teori Singkat
Pada server Cloud, kita biasa memanggil `pd.read_csv('file.csv')` yang langsung menyalin seluruh isi berkas dari disk ke dalam memori RAM sekaligus. Jika berkas berukuran 500 MB dijalankan pada perangkat *Edge Gateway* kecil (seperti Raspberry Pi Zero dengan RAM 512 MB), sistem operasi akan mengalami kehabisan memori (**Out-of-Memory / OOM Killer**) dan mematikan program secara paksa.

Pada pemrograman **Edge Streaming**, kita menerapkan tiga prinsip utama:
1. **Iterative Line-by-Line Reading:** Membaca berkas atau aliran soket satu baris demi satu baris menggunakan *generator/iterator*. Baris yang sudah selesai diproses langsung dibuang dari memori.
2. **Circular Buffer (`deque(maxlen=k)`):** Menyimpan hanya $k$ sampel terakhir di dalam antrean melingkar berukuran tetap untuk menghitung *Moving Average* dan *Standard Deviation* lokal.
3. **Report by Exception (*Delta Reporting*):** Hanya mengirimkan paket ke Cloud apabila nilai sensor saat ini berubah melebihi ambang $\pm \Delta$ dari kiriman terakhir, atau ketika terjadi kondisi alarm darurat.

---

## 4. Langkah Kerja Praktikum (Detail & Anotasi Kode)

### Langkah 0: Membangkitkan Dataset Uji Berukuran Besar (50.000 Baris)
Buat berkas skrip bernama `generate_big_stream.py` (atau jalankan di sel notebook) untuk menghasilkan berkas CSV berisi **50.000 baris** rekaman sensor suhu dan kelembaban:

```python
import csv
import math
import random

N_BARIS = 50000
random.seed(42)

with open('data_sensor_50k.csv', mode='w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(['id_sampel', 'suhu', 'kelembaban'])
    
    for i in range(1, N_BARIS + 1):
        # Pola suhu harian berlapis derau halus
        suhu = 28.0 + 4.0 * math.sin(i / 200.0) + random.gauss(0, 0.3)
        kelembaban = 70.0 - 1.2 * (suhu - 28.0) + random.gauss(0, 0.8)
        
        # Suntikkan lonjakan bahaya panas pada sampel ke-25.000 s.d. 25.015
        if 25000 <= i <= 25015:
            suhu += 12.0
            
        writer.writerow([i, round(suhu, 2), round(kelembaban, 2)])

print(f"Berhasil membuat 'data_sensor_50k.csv' dengan {N_BARIS} baris data!")
```

---

### Langkah 1: Membangun Skrip Streaming Edge dengan Circular Buffer (`deque`)
Buat berkas bernama `edge_streaming_deque.py`. Perhatikan bagaimana kita memproses 50.000 baris data menggunakan jendela `maxlen=10` dan membandingkan ukuran memorinya terhadap `list` biasa yang terus membesar:

```python
import csv
import statistics
import sys
from collections import deque

print("=== SIMULASI EDGE STREAMING DENGAN CIRCULAR BUFFER (DEQUE) ===")

WINDOW_SIZE = 10
# Circular Buffer: Ukuran dikunci maksimal 10 elemen!
buffer_deque = deque(maxlen=WINDOW_SIZE)

# Sebagai pembanding: List naif yang terus di-append tanpa batas
list_naif = []

peak_mem_deque = 0
jumlah_alarm = 0
baris_diproses = 0

with open('data_sensor_50k.csv', mode='r') as file:
    reader = csv.DictReader(file)
    
    for row in reader:
        try:
            baris_diproses += 1
            suhu = float(row['suhu'])
            
            # 1. Masukkan ke Circular Buffer (elemen tertua otomatis tergeser keluar jika > 10)
            buffer_deque.append(suhu)
            
            # 2. Masukkan ke List Naif (sebagai perbandingan konsumsi RAM)
            list_naif.append(suhu)
            
            # Catat puncak memori yang dipakai oleh objek deque
            mem_sekarang = sys.getsizeof(buffer_deque)
            if mem_sekarang > peak_mem_deque:
                peak_mem_deque = mem_sekarang
                
            # 3. Komputasi Analitik Lokal di Edge (Saat jendela sudah terisi 10 data)
            if len(buffer_deque) == WINDOW_SIZE:
                rata_rata = statistics.mean(buffer_deque)
                std_dev = statistics.stdev(buffer_deque)
                
                # Deteksi Anomali Lokal: Rata-rata suhu > 36.0°C
                if rata_rata > 36.0:
                    jumlah_alarm += 1
                    if jumlah_alarm <= 5:  # Cetak 5 alarm pertama agar layar tidak penuh
                        print(f"[Sampel #{baris_diproses}] ALARM EDGE: SMA_10 = {rata_rata:.2f}°C (Std: {std_dev:.2f})")
                        
        except ValueError:
            pass  # Abaikan baris jika terdapat nilai NaN/kosong

mem_list_naif = sys.getsizeof(list_naif)

print("\n=== HASIL AUDIT EFISIENSI MEMORI EDGE ===")
print(f"Total sampel diproses             : {baris_diproses:,} baris")
print(f"Total kejadian alarm terdeteksi   : {jumlah_alarm} kali")
print(f"Konsumsi RAM Circular Buffer Deque: {peak_mem_deque:,} Bytes (Tetap Konstan O(1)!)")
print(f"Konsumsi RAM List Naif (Append)   : {mem_list_naif:,} Bytes (Membengkak O(N)!)")
print(f"Rasio Penghematan Struktur Memori : {mem_list_naif / peak_mem_deque:.1f}x lipat lebih hemat!")
```

---

### Langkah 2: Algoritma Streaming EMA & *Report by Exception (Delta Reporting)*
Sekarang buat berkas `edge_delta_reporting.py`. Kita akan menggabungkan dua teknik optimasi Edge paling populer di industri:
1. **Single-State EMA** (menghaluskan sinyal hanya dengan 1 variabel float `ema_suhu` tanpa array *buffer* sama sekali!).
2. **Report by Exception (*Delta Reporting*) + Heartbeat** (hanya mengirim paket ke Cloud jika nilai EMA suhu berubah $\ge 0,5^\circ\text{C}$ dari nilai kiriman terakhir, atau setiap 3.600 sampel sebagai sinyal detak jantung *heartbeat*).

```python
import csv
import sys

print("=== SIMULASI EDGE EMA + REPORT BY EXCEPTION (DELTA REPORTING) ===")

ALPHA = 0.3                  # Bobot EMA untuk data terbaru (0 < alpha < 1)
DELTA_THRESHOLD = 0.5        # Kirim ke Cloud hanya jika suhu berubah >= 0.5°C
HEARTBEAT_INTERVAL = 3600    # Wajib lapor minimal setiap 3.600 sampel meskipun suhu stabil

ema_suhu = None              # Hanya butuh 1 variabel float di RAM!
suhu_terakhir_kirim = None
total_sampel = 0
paket_terkirim_cloud = 0

with open('data_sensor_50k.csv', mode='r') as file:
    reader = csv.DictReader(file)
    
    for row in reader:
        total_sampel += 1
        suhu_mentah = float(row['suhu'])
        
        # 1. Perhitungan Single-State EMA (Kompleksitas Ruang O(1) murni 1 variabel)
        if ema_suhu is None:
            ema_suhu = suhu_mentah
        else:
            ema_suhu = (ALPHA * suhu_mentah) + ((1.0 - ALPHA) * ema_suhu)
            
        # 2. Logika Report by Exception (Delta Reporting) + Heartbeat
        wajib_kirim = False
        alasan = ""
        
        if suhu_terakhir_kirim is None:
            wajib_kirim = True
            alasan = "INIT_BOOT"
        elif abs(ema_suhu - suhu_terakhir_kirim) >= DELTA_THRESHOLD:
            wajib_kirim = True
            alasan = f"DELTA_CHANGE ({ema_suhu - suhu_terakhir_kirim:+.2f}°C)"
        elif total_sampel % HEARTBEAT_INTERVAL == 0:
            wajib_kirim = True
            alasan = "PERIODIC_HEARTBEAT"
            
        if wajib_kirim:
            paket_terkirim_cloud += 1
            suhu_terakhir_kirim = ema_suhu
            if paket_terkirim_cloud <= 8:
                print(f"[Kirim #{paket_terkirim_cloud:03d} | Sampel {total_sampel:05d}] EMA Suhu: {ema_suhu:.2f}°C | Alasan: {alasan}")

penghematan_bw = (1.0 - (paket_terkirim_cloud / total_sampel)) * 100.0

print("\n=== HASIL AUDIT PENGHEMATAN BANDWIDTH & MEMORI ===")
print(f"Ukuran Memori Variabel Tunggal EMA : {sys.getsizeof(ema_suhu)} Bytes!")
print(f"Total sampel dibaca di Edge        : {total_sampel:,} paket")
print(f"Total paket dikirim ke Cloud       : {paket_terkirim_cloud:,} paket")
print(f"Penghematan Bandwidth & Kuota Radio: {penghematan_bw:.2f}%!")
```

---

## 5. Hasil Eksperimen yang Diharapkan
1. Pada Langkah 1, meskipun jumlah sampel mencapai 50.000 baris (bahkan jika diperbesar menjadi 10 juta baris), ukuran memori `buffer_deque` tetap terkunci di sekitar **~624 Bytes**, sementara `list_naif` membengkak hingga ratusan ribu Bytes!
2. Pada Langkah 2, variabel tunggal `ema_suhu` hanya membutuhkan **24 Bytes** di Python (atau 4 Bytes di C), dan mekanisme *Report by Exception* berhasil memangkas jumlah transmisi radio ke Cloud hingga **lebih dari 90%** tanpa kehilangan satu pun informasi perubahan suhu maupun lonjakan bahaya!

---

## 6. Tugas Mandiri Praktikum (Persiapan Praktik UTS)
1. **Deteksi Anomali Fluktuasi (Standar Deviasi Bergerak di Edge):**
   Modifikasi skrip `edge_streaming_deque.py` dengan ukuran jendela `WINDOW_SIZE = 5`. Tambahkan dua aturan deteksi anomali lokal sekaligus:
   - **Aturan 1 (Fluktuasi Ekstrem):** Jika `statistics.stdev(buffer_deque) > 2.0`, cetak peringatan `[FLUKTUASI EKSTREM]` dan simpan baris waktu beserta nilainya ke dalam berkas lokal `log_anomali_edge.txt`.
   - **Aturan 2 (Stuck-at Fault / Sensor Macet):** Jika `statistics.stdev(buffer_deque) == 0.0`, cetak peringatan `[SENSOR MACET / ZERO VARIANCE]`.
2. **Adaptive Sampling Rate Simulator:**
   Tambahkan logika pada skrip Anda di mana variabel `interval_baca` bernilai `10` (hanya memproses setiap kelipatan 10 baris) saat kondisi normal (`suhu < 33.0°C`), namun otomatis berubah menjadi `interval_baca = 1` (memproses setiap baris tanpa ada yang dilewati) begitu terdeteksi `suhu >= 33.0°C`!

---

## 7. Referensi & Acuan Materi Pekan Ini

1. **Warden, P., & Situnayake, D. (2019).** *TinyML: Machine Learning with TensorFlow Lite on Arduino and Ultra-Low-Power Microcontrollers*. O'Reilly Media. (ISBN-13: 978-1492052043)  
2. **Hanes, D., et al. (2017).** *IoT Fundamentals: Networking Technologies, Protocols, and Use Cases for the Internet of Things*. Cisco Press. (ISBN-13: 978-1587144561)  
3. **Python Software Foundation. (2024).** *Python Documentation: `collections.deque`, `csv`, and `statistics`*. Tersedia: [https://docs.python.org/3/](https://docs.python.org/3/)  
4. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
