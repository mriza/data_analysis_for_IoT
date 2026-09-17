# Materi Kuliah Pekan 7: Edge Analytics dan Komputasi Resource-Constrained

## 1. Tujuan Pembelajaran Khusus
- Mahasiswa mampu membandingkan performa dan peran analitik di *Edge* versus *Cloud*.
- Mahasiswa mengerti pendekatan logika pemrograman berbiaya memori rendah (*Memory-efficient programming*).
- Mahasiswa mengetahui evolusi teknologi terbaru *Tiny Machine Learning (TinyML)*.

## 2. Pendahuluan & Konteks Industri
Selama ini kita diajarkan memproses Big Data di komputer canggih atau Cloud Server. Namun kenyataannya di industri *Smart Farming*, mengirim ribuan *image frame* setiap detik dari kamera lapangan ke Cloud memakan biaya internet bulanan yang mahal dan memboroskan baterai *solar panel*. Paradigma modern memutar balik proses ini: Jangan kirim datanya ke server AI, tapi bawalah model AI-nya ke sensor! Inilah inti dari **Edge Analytics**.

## 3. Materi Inti
### 3.1 Apa itu Edge Analytics?
Edge Analytics adalah proses melakukan analisis data deskriptif hingga model prediktif ringan secara lokal di mikrokontroler (ESP32/Arduino) atau mikrokomputer (Raspberry Pi/Nvidia Jetson) langsung pada perangkat *IoT Node*, sesaat setelah data ditangkap oleh sensor fisik.

### 3.2 Tantangan Komputasi (Resource-Constrained)
Mikrokontroler tidak memiliki ruang sebesar PC. ESP32 hanya memiliki SRAM sekitar 520 KB. 
- *Anda tidak bisa me-load Pandas ke dalam RAM ESP32.*
- *Anda tidak bisa menyimpan histori data setahun ke dalam ESP32.*
Oleh sebab itu, pemrograman berbasis *Batching* atau *Streaming* menjadi krusial. Alih-alih menyimpan semuanya di *array*, data diproses (misal dijumlahkan nilainya) pada saat masuk, lalu nilainya segera dihapus dari memori sesaat setelah rata-ratanya dihitung. 

### 3.3 Tiny Machine Learning (TinyML)
Sub-bidang ML terobosan baru. Model klasifikasi (seperti TensorFlow Keras) yang tadinya berukuran 50 MB, "dikompres" dengan teknik **Quantization** (mengubah tipe bobot *float32* presisi tinggi menjadi *int8* berpresisi rendah) dan **Pruning** (memotong simpul saraf tak penting) hingga model menyusut menjadi 30 KB.
Model kecil ini kemudian di-flash ke dalam *chip* seharga 50 ribu rupiah. 

## 4. Studi Kasus (Real-world Example)
**Detektor Batuk Babi (Swine Flu) di Peternakan**
Sebuah peternakan modern meletakkan mikrofon pintar (*Edge Node*) di kandang babi. Jika suara dikirim via internet setiap waktu, biaya dan latensi akan bengkak. Model audio klasifikasi "Suara Babi Sehat vs Babi Batuk" ditanam ke *chip* menggunakan TinyML TensorFlow Lite. Sensor terus mendengar secara *offline* 24/7. Hanya ketika model lokal mendeteksi probabilitas batuk > 90%, modul akan mengaktifkan WiFi sejenak dan mengirimkan notifikasi "*Alert: Batuk ditemukan di Kandang 5*" ke *Cloud*. Baterai pun awet berbulan-bulan.

## 5. Bahan Diskusi Kelas
1. Jika komputasi *Edge* sangat canggih dan menghemat internet, mengapa kita masih repot-repot membutuhkan arsitektur *Cloud Computing*? 
2. Menurut Anda, data jenis apa di sektor pertanian yang HARUS dikirim ke *Cloud*, dan data apa yang CUKUP dieksekusi di *Edge*?

## 6. Referensi Spesifik
- Warden, P., & Situnayake, D. (2019). *TinyML: Machine Learning with TensorFlow Lite on Arduino and Ultra-Low-Power Microcontrollers*. O'Reilly Media.
- Lihat `Daftar_Referensi_Lengkap.md` di direktori utama untuk referensi selengkapnya.
