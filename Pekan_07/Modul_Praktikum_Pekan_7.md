# Materi Praktikum Pekan 7: Praktik Edge Scripting (Efisiensi Memori)

## 1. Tujuan Praktikum
- Mahasiswa mampu mensimulasikan lingkungan *Edge Computing* dengan menulis skrip Python yang menghindari penggunaan pustaka berat (*pandas/numpy*).
- Mahasiswa menguasai pengolahan data sensor bergaya *Streaming* secara *real-time* iteratif untuk menjaga konsumsi RAM seminimal mungkin.

## 2. Persiapan Alat & Prasyarat
- **Software:** IDE Python sederhana (Terminal/VS Code).
- **Library:** Tidak memakai library pihak ketiga. Murni `built-in` Python (`csv`, `time`, `statistics`).

## 3. Dasar Teori Singkat
Pendekatan konvensional (*Cloud-style*): Membaca file 1 juta baris CSV -> Memuat semua (1 juta baris) ke variabel RAM -> Memproses rata-ratanya. Jika Anda melakukan ini pada papan Raspberry Pi lama, RAM akan langsung kepenuhan (*Out-of-Memory / OOM Killer*).
Pendekatan *Edge-style*: Membaca baris ke-1 -> Proses lalu hapus -> Baca baris ke-2 -> Proses lalu hapus. Memori yang dipakai konsisten rendah dan tidak terakumulasi.

## 4. Langkah Kerja (Detail & Anotasi Kode)

### Langkah 1: Membaca Data tanpa Pandas
Buat file `edge_script.py`. Script ini merepresentasikan apa yang berjalan di *Edge Gateway*.

```python
import csv
import time
import statistics

print("Mulai membaca data secara streaming (Simulasi Edge)...")

# Kita menggunakan array sementara yang ukurannya akan dibatasi (maksimal 10 item)
buffer_suhu = []

# Membuka file secara konvensional tanpa load utuh
with open('data_sensor_dummy.csv', mode='r') as file:
    reader = csv.DictReader(file)
    
    # Iterator bergerak per 1 baris. Hanya 1 baris ini yang ada di RAM pada satu waktu
    for row in reader:
        try:
            # Casting teks menjadi desimal
            suhu = float(row['suhu'])
            buffer_suhu.append(suhu)
            
            # SIMULASI: Jika array buffer sudah berisi 10 data (berarti sudah mengumpulkan data selama 10 jam)
            if len(buffer_suhu) == 10:
                # 1. Edge melakukan komputasi lokal
                rata_rata = statistics.mean(buffer_suhu)
                suhu_maks = max(buffer_suhu)
                
                # 2. Edge baru mengirimkan rangkuman keputusannya ke Cloud
                print(f"\n--- MENGIRIM PAYLOAD KE CLOUD ---")
                print(f"Rata-rata 10 jam terakhir: {rata_rata:.2f} C")
                
                # Rule base filtering: Hanya bunyikan alarm ke server JIKA suhu sangat panas
                if suhu_maks > 34.0:
                    print(f"[ALARM CLOUD] Peringatan! Terdeteksi suhu maks {suhu_maks:.2f} C")
                
                # 3. MENGOSONGKAN MEMORI ARRAY SECARA PAKSA agar RAM lega kembali
                buffer_suhu.clear() 
                
                # Jeda sejenak untuk mensimulasikan proses komputasi lambat di Edge
                time.sleep(0.5)
                
        except ValueError:
            # Melewati proses jika terjadi error parsing (misal nilai kosong / NaN)
            pass 
```

### Langkah 2: Eksekusi dan Monitor Resource
Jalankan file tersebut menggunakan perintah `python edge_script.py`. 

## 5. Hasil Eksperimen yang Diharapkan
- Anda akan melihat tampilan di konsol *printout* pengiriman paket agregat "Rata-rata 10 jam terakhir" diikuti oleh notifikasi Alarm lokal jika kondisi batas terpenuhi.
- Meskipun baris data yang dibaca ada ribuan, pemakaian RAM komputer Anda tidak akan bertambah seiring waktu berjalannya program tersebut, karena fungsi `.clear()` bekerja konsisten layaknya *Garbage Collector*.

## 6. Troubleshooting
- **Error `KeyError: 'suhu'`:** File CSV hasil praktikum sebelumnya tidak memiliki kolom bernama `suhu`. Buka file CSV dan cek nama header kolom baris paling atas, sesuaikan nama kolom di script.

## 7. Tugas Mandiri/Tantangan
Modifikasi *script* agar `buffer_suhu` hanya menampung **5** data saja. Hitung nilai *Standard Deviation (Standar Deviasi)* menggunakan pustaka bawaan `statistics.stdev()`. Buat logika: *Jika Standar Deviasi dari 5 data terakhir itu lebih besar dari 2.0 (artinya fluktuasi sangat ekstrem), Edge tidak hanya print [ALARM CLOUD], namun juga menyimpan log fluktuasi mematikan tersebut ke dalam file `anomali_edge.txt` secara lokal!*
