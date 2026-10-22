---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Materi Kuliah Pekan 7: Edge Analytics, Komputasi Resource-Constrained, TinyML, dan Review Persiapan UTS"
---

# Materi Kuliah Pekan 7: Edge Analytics, Komputasi Resource-Constrained, TinyML, dan Review Persiapan UTS

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 7 (Kuliah Teori Terakhir Sebelum UTS — $2 \times 50$ Menit)  
**Capaian Pembelajaran (Sub-CPMK 7):** Mahasiswa mampu menganalisis arsitektur komputasi pada perangkat bersumber daya terbatas (*Resource-Constrained Edge*), merancang algoritma *streaming* hemat memori (*Circular Buffer* & *Single-State EMA*), memahami prinsip kompresi model **TinyML** (*Quantization & Pruning*), serta mengintegrasikan seluruh konsep Pekan 1–7 sebagai persiapan **Evaluasi Tengah Semester (UTS) pada Pekan 8**.

---

## 1. Tujuan Pembelajaran Khusus
Setelah mengikuti perkuliahan pekan ini, mahasiswa diharapkan mampu:
1. Membandingkan empat kendala utama komputasi IoT lapangan (**Memori SRAM, Daya Baterai, Bandwidth Transmisi, dan Latensi**) antara arsitektur *Edge*, *Fog*, dan *Cloud*.
2. Menjelaskan mengapa struktur data dinamis tanpa batas (`list.append()` / `malloc()`) menyebabkan kegagalan fatal *Heap Exhaustion / Out-of-Memory (OOM)* pada sistem tertanam, serta merancang solusinya menggunakan **Circular Buffer (*Ring Buffer / Deque*)** ber-kompleksitas ruang $O(1)$.
3. Menerapkan strategi penghematan *bandwidth* dan energi di Edge: **Report by Exception (*Delta Reporting*)**, **Event-Driven Adaptive Sampling**, dan pemadatan *payload* biner pada jaringan **LoRaWAN**.
4. Menjelaskan alur pengembangan **Tiny Machine Learning (TinyML)** beserta implikasi matematis dari teknik kompresi **Post-Training Quantization (FP32 ke INT8)** dan **Pruning**.
5. Mensintesiskan seluruh materi Pekan 1 hingga Pekan 7 untuk menghadapi **Evaluasi Tengah Semester (UTS)** pada Pekan 8.

---

## 2. Mengapa Komputasi Harus Dibawa ke Ujung Jaringan (*Edge Analytics*)?

Pada arsitektur komputasi awan konvensional (*Cloud-Centric*), node sensor di lapangan hanya bertugas membaca ADC lalu mengirimkan seluruh data mentah ke server pusat melalui internet. Namun, pada implementasi industri nyata seperti *Smart Farming* di pedalaman, pemantauan akustik/getaran mesin pabrik, atau kendaraan otonom, paradigma *Cloud-Centric* menghadapi **empat tembok penghalang fisik**:

1. **Biaya & Keterbatasan Bandwidth (*Bandwidth Bottleneck*):**
   Sebuah sensor getaran (*accelerometer*) atau mikrofon pemantau kesehatan mesin menggiling sampel dengan frekuensi $1\text{ kHz} - 16\text{ kHz}$ (menghasilkan ratusan Megabyte hingga Gigabyte data per jam). Mengirimkan seluruh aliran mentah ini melalui modem seluler 4G atau satelit akan menguras biaya kuota yang sangat mahal dan menyumbat pita frekuensi radio.
2. **Konsumsi Energi Transmisi Radio (*Power Wall*):**
   Secara fisika elektronika, memancarkan gelombang radio (WiFi/4G/LoRa) mengonsumsi energi listrik **ratusan hingga ribuan kali lebih besar** dibandingkan mengeksekusi instruksi matematika di dalam CPU mikrokontroler. Memproses 1.000 angka di CPU ESP32 hanya memakan mikrojoule, sedangkan menyalakan pemancar WiFi untuk mengirim 1.000 angka tersebut langsung menguras baterai.
3. **Latensi Waktu Nyata (*Real-Time Latency*):**
   Perjalanan paket data dari kebun $\rightarrow$ BTS Seluler $\rightarrow$ Server Cloud $\rightarrow$ kembali ke Aktuator membutuhkan waktu ratusan milidetik hingga beberapa detik. Untuk sistem pengaman tekanan pipa atau pengereman traktor, keterlambatan ini berakibat fatal.
4. **Kemandirian Saat Koneksi Terputus (*Autonomy & Reliability*) & Privasi:**
   Jika sinyal internet di lereng bukit putus saat badai, sistem berbasis *Edge Analytics* tetap mampu mengambil keputusan mandiri secara lokal tanpa lumpuh.

> **Prinsip Emas Edge Analytics:** *"Jangan kirim jutaan data mentah ke tempat kode analitik berada (Cloud), tetapi bawalah kode analitik dan model AI yang ringan ke tempat data dilahirkan (Sensor Edge)!"*

---

## 3. Materi Inti: Rekayasa Algoritma pada Perangkat *Resource-Constrained*

### 3.1 Keterbatasan Arsitektur Perangkat Keras Mikrokontroler (MCU)
Perangkat *Edge Node* seperti **Arduino Uno (ATmega328P)** atau **ESP32** bukanlah komputer PC:
- **Kapasitas SRAM Sangat Kecil:** Arduino Uno hanya memiliki **2 KB SRAM**, sedangkan ESP32 memiliki sekitar **520 KB SRAM**. Sebagai perbandingan, memuat pustaka `pandas` di Python membutuhkan lebih dari **50 MB (50.000 KB) RAM**! Artinya, mustahil menjalankan Pandas atau memuat berkas CSV utuh di dalam mikrokontroler.
- **Batas Usia Tulis Memori Flash (*Flash Wear-Out*):** Memori penyimpanan non-volatil (Flash/EEPROM/SD Card) pada mikrokontroler memiliki batas siklus tulis fisik sekitar $10.000 - 100.000$ kali per sektor. Jika program menulis log data ke Flash setiap 1 milidetik tanpa penyangga RAM, chip memori Flash akan rusak permanen (*wear-out*) dalam hitungan jam!

---

### 3.2 Algoritma Streaming & Struktur Data *Circular Buffer* ($O(1)$ Memory)
Bagaimana cara menghitung *Moving Average* 10 sampel terakhir secara terus-menerus selama 1 tahun penuh di dalam RAM yang hanya berukuran beberapa Kilobyte?

**Kesalahan Fatal Pemula (*Unbounded Array Append*):**
Jika programmer menambahkan setiap pembacaan sensor baru ke dalam array dinamis tanpa batas (`list.append(suhu)` di Python atau `realloc()` di C), ukuran array akan terus membengkak setiap detik hingga menghabiskan seluruh SRAM (*Heap Exhaustion*), memicu *Stack-Heap Collision*, dan membuat mikrokontroler mengalami *crash / reboot*!

**Solusi Rekayasa — Circular Buffer (*Ring Buffer / Double-Ended Queue*):**
Kita mengalokasikan satu blok array berukuran tetap sebesar $k$ elemen (misal $k = 10$). Ketika sampel ke-11 masuk, kita **tidak menambah ukuran array**, melainkan menimpa (*overwrite*) elemen paling tua (indeks ke-0) menggunakan penunjuk indeks berputar berbasis operasi **Modulo (`%`)**:

```text
           MEKANISME CIRCULAR BUFFER (RING BUFFER) UKURAN TETAP k = 5
           Konsumsi Memori RAM Selalu Konstan O(1) — Tidak Pernah Membengkak!

   Kondisi Penuh (5 Sampel Pertama):        Saat Sampel Baru (29.4°C) Masuk:
   Indeks:   [0]    [1]    [2]    [3]    [4]        Indeks:   [0]    [1]    [2]    [3]    [4]
           +------+------+------+------+------+             +------+------+------+------+------+
   Nilai : | 27.0 | 27.5 | 28.0 | 28.2 | 29.0 |     Nilai : | 29.4 | 27.5 | 28.0 | 28.2 | 29.0 |
           +------+------+------+------+------+             +------+------+------+------+------+
              ^                                                       ^
              |                                                       |
         head = 0 (Data Tertua siap ditimpa)                     head = (0 + 1) % 5 = 1
```

#### Implementasi Circular Buffer pada Firmware C/C++ (ESP32 / Arduino)
```c
#define WINDOW_SIZE 10
float buffer[WINDOW_SIZE]; // Alokasi statis 10 x 4 byte = 40 Bytes di RAM!
int head = 0;
int count = 0;

void add_sample(float val) {
    buffer[head] = val;                 // Timpa posisi head saat ini (Kompleksitas waktu O(1))
    head = (head + 1) % WINDOW_SIZE;    // Geser head melingkar kembali ke 0 jika mencapai 10
    if (count < WINDOW_SIZE) count++;
}

float get_moving_average() {
    float sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += buffer[i];
    }
    return sum / count;
}
```
Pada Python (di *Edge Gateway* seperti Raspberry Pi), struktur data ini telah disediakan secara optimal dalam bahasa C melalui **`collections.deque(maxlen=10)`**.

#### Alternatif Lebih Hemat Lagi: Streaming Exponential Moving Average (EMA)
Jika kita bahkan tidak ingin menyimpan 10 angka di dalam *buffer*, kita dapat menggunakan rumus rekursif **EMA** yang hanya membutuhkan **1 variabel memori tunggal (`ema_sebelumnya`)**:
$$\text{EMA}_{\text{baru}} = (\alpha \cdot x_{\text{sekarang}}) + ((1 - \alpha) \cdot \text{EMA}_{\text{sebelumnya}})$$
Dengan $\alpha \in (0, 1)$, konsumsi memori penyangga menyusut menjadi hanya **1 variabel float (4 Bytes di C)** untuk durasi operasi tak terbatas!

---

### 3.3 Optimasi Bandwidth & Energi di Edge: *Report by Exception* & *Adaptive Sampling*

Selain menghemat RAM, *Edge Analytics* bertugas menekan jumlah paket radio yang dipancarkan ke udara:

1. **Report by Exception (*Delta Reporting*) + Heartbeat:**
   Pada pemantauan level tangki air/solar atau kelembaban tanah yang berubah sangat lambat, node sensor membaca nilai setiap menit secara lokal, namun **hanya menyalakan pemancar radio jika selisih perubahan nilai melebihi ambang delta ($\Delta x > \epsilon$)** dari nilai yang terakhir dikirim:
   $$|x_{\text{sekarang}} - x_{\text{terakhir\_kirim}}| \ge \Delta_{\text{ambang}}\ (\text{misal } \pm 0,5\%)$$
   Agar server pusat tidak mengira sensor mati saat kondisi lingkungan tenang seharian, ditambahkan pengiriman sinyal detak jantung (**Heartbeat**) pendek setiap 6 jam sekali.
2. **Event-Driven Adaptive Sampling Rate:**
   Frekuensi *sampling* diatur secara dinamis berdasarkan laju perubahan fenomena fisik ($\frac{dx}{dt}$ atau standar deviasi bergerak $\sigma$):
   - **Mode Tenang (Normal):** Saat tinggi muka air sungai stabil ($\sigma_{10\text{min}} < 1\text{ cm}$), sensor tidur (*deep sleep*) dan hanya bangun setiap **10 menit** sekali (hemat baterai hingga 95%).
   - **Mode Siaga (Kritis):** Begitu terdeteksi laju kenaikan air $> 5\text{ cm/menit}$, node otomatis menaikkan frekuensi pembacaan dan pengiriman menjadi setiap **15 detik** sekali untuk memberikan resolusi temporal tinggi bagi sistem peringatan banjir.
3. **Kepatuhan Batas Udara LoRaWAN (*Duty-Cycle & Payload Compaction*):**
   Pada jaringan radio jarak jauh berdaya rendah **LoRaWAN**, regulasi frekuensi membatasi waktu pancar udara (*duty cycle*, misal maksimal $1\%$ waktu per hari) dan ukuran *payload* sangat kecil ($51 - 222\text{ Bytes}$). Oleh karena itu, data di Edge wajib diringkas (*downsampled*) dan dikemas dalam **format biner padat** (bukan teks JSON panjang) sebelum dipancarkan.

---

### 3.4 Tiny Machine Learning (TinyML): Menanamkan AI ke dalam Mikrokontroler

Bagaimana jika pola anomali yang ingin dideteksi di Edge terlalu rumit untuk rumus `IF-ELSE` atau rata-rata biasa (misalnya mengenali suara batuk ternak di kandang atau mendeteksi pola getaran retakan bantalan turbin)? Solusinya adalah **TinyML** (Warden & Situnayake, 2019).

```text
                         ARSITEKTUR PIPELINE PENGEMBANGAN TINYML

+-----------------------------------+     +-----------------------------------+     +-----------------------------------+
| 1. CLOUD TRAINING (Server / GPU)  |     | 2. MODEL OPTIMIZATION & COMPRESS  |     | 3. EDGE DEPLOYMENT (MCU / ESP32)  |
| - Melatih model Neural Network    | --> | - Post-Training Quantization:     | --> | - Konversi ke array byte C/C++    |
|   dengan ribuan sampel historis   |     |   Bobot Float32 (4B) -> Int8 (1B) |     |   (TensorFlow Lite Micro)         |
| - Ukuran model awal: ~24 MB (FP32)|     | - Pruning: Potong sinapsis ~ 0    |     | - Ukuran akhir: ~20 KB - 300 KB   |
| - Akurasi awal: 96.5%             |     | - Ukuran menyusut >= 75%!         |     | - Inferensi offline < 20 ms!      |
+-----------------------------------+     +-----------------------------------+     +-----------------------------------+
```

#### Dua Teknik Kompresi Utama pada TinyML:
1. **Quantization (Kuantisasi Presisi Angka — FP32 ke INT8):**
   Secara standar, jaringan saraf tiruan menyimpan jutaan angka bobot (*weights*) dalam format desimal **32-bit Floating Point (`FP32`, 4 Bytes per bobot)**. Proses kuantisasi memetakan rentang angka desimal kontinu tersebut ke dalam bilangan bulat **8-bit Integer (`INT8`, 1 Byte per bobot, rentang $-128$ s.d. $+127$)**:
   $$q = \text{round}\left(\frac{r}{S}\right) + Z_{\text{offset}}$$
   - **Keuntungan:** Ukuran model langsung **menyusut sebesar 75% (menjadi $\frac{1}{4}$ ukuran aslinya)**, konsumsi RAM inferensi turun drastis, dan operasi perkalian bilangan bulat (`INT8`) dieksekusi jauh lebih cepat serta hemat daya oleh CPU mikrokontroler dibandingkan operasi *floating-point*.
   - **Trade-Off (Risiko Teknis):** Karena memampatkan jutaan kemungkinan angka desimal ke dalam 256 tangga nilai diskrit, muncul sedikit galat pembulatan (**Quantization Noise / Accuracy Drop**, biasanya akurasi turun tipis sekitar $0,5\% - 2\%$).
2. **Pruning (Pemangkasan Sinapsis):**
   Menghapus sambungan bobot neuron yang nilainya mendekati nol (tidak memberikan kontribusi signifikan terhadap output prediksi), sehingga struktur jaringan menjadi ramping dan jarang (*sparse*).

---

## 4. Rangkuman Integratif Pekan 1 – 7 (Persiapan Menghadapi UTS Pekan 8)

Sebagai penutup paruh pertama semester, pastikan Anda menguasai peta konsep terpadu berikut sebelum memasuki **Pekan 8 (Evaluasi Tengah Semester / UTS)**:

| Pekan | Topik Utama | Konsep Kunci yang Wajib Dikuasai untuk UTS |
| :---: | :--- | :--- |
| **1** | Pengantar & Arsitektur IoT | Karakteristik 5V, perhitungan beban baris/byte sensor vs batas Excel (1.048.576 baris), penyebab fisik data kotor, pembagian tugas **Edge vs Fog vs Cloud**. |
| **2** | Akuisisi Data & MQTT/AMQP | Arsitektur Pub/Sub vs HTTP, topik MQTT & *wildcard* (`+` satu level vs `#` multi-level), **QoS 0, 1 (risiko duplikat), 2 (overhead 4-step handshake)**, JSON vs Biner. |
| **3** | Anatomi Data Time-Series | Dekomposisi **Trend, Seasonality (termasuk Multi-Seasonality), Noise**, model Aditif vs Multiplikatif, **Downsampling (wajib sertakan Mean, Max, Min, Std)**, standar waktu **UTC & NTP**, diagnosa **Sensor Drift vs Stuck-at Fault ($\sigma^2=0$)**. |
| **4** | Statistik Deskriptif & EDA | `DatetimeIndex`, `resample()`, distribusi Normal vs *Right-Skewed*, **Korelasi Pearson ($r$)**, *Heatmap*, serta perbedaan kritis **Korelasi vs Kausalitas (*Confounding Variable*)**. |
| **5** | Missing Values & Smoothing | Klasifikasi **MCAR, MAR, MNAR**, bahaya *Global Mean Imputation* tanpa *Fail-Safe* (*Cascade Anomaly*), perbandingan `ffill` vs `Linear` vs `Polynomial Interpolation`, serta **SMA vs EMA vs Median Filter** dan efek **Phase Lag**. |
| **6** | Deteksi Anomali Statistik | Taksonomi *Outlier* (Point, Contextual, Collective, Multivariate), rumus & perhitungan **Z-Score ($|Z|>3$)** beserta kelemahan *Masking Effect*, rumus **IQR ($Q_1 - 1,5\text{IQR}$ s.d. $Q_3 + 1,5\text{IQR}$)**, *Cross-Sensor Validation*, & deteksi *False Data Injection*. |
| **7** | Edge Analytics & TinyML | Kendala SRAM & Flash wear-out pada MCU, struktur data **Circular Buffer ($O(1)$ RAM)**, *Report by Exception*, *Adaptive Sampling*, serta kompresi **TinyML (Quantization FP32 $\rightarrow$ INT8 & Pruning)**. |

---

## 5. Bahan Diskusi Kelas & Latihan Soal Pra-UTS

1. **Perhitungan Kuantisasi Memori TinyML:**
   Sebuah model *Convolutional Neural Network* 1D pendeteksi getaran pompa irigasi memiliki $1.500.000$ parameter bobot dalam format `float32` (4 byte per parameter). Berapa Megabyte (MB) ukuran model tersebut sebelum dikompresi, dan menjadi berapa MB ukurannya setelah dikonversi menggunakan *Post-Training Quantization* `int8` (1 byte per parameter)?
2. **Desain Arsitektur Hibrida Kandang Ayam Closed-House:**
   Jika Anda memiliki 50 kandang ayam dengan koneksi internet pedesaan yang lambat (512 Kbps) namun membutuhkan alarm kematian kipas blower dalam waktu $< 2\text{ detik}$ sekaligus prediksi pertumbuhan bobot ayam bulanan, bagian mana yang harus Anda letakkan di *Edge/Fog* dan bagian mana yang diletakkan di *Cloud*?

---

## 6. Referensi & Acuan Materi Pekan Ini

1. **Warden, P., & Situnayake, D. (2019).** *TinyML: Machine Learning with TensorFlow Lite on Arduino and Ultra-Low-Power Microcontrollers*. O'Reilly Media. (ISBN-13: 978-1492052043) — **Bab 1 (Introduction to TinyML), Bab 4 (The "Hello World" of TinyML), & Bab 12 (Quantization and Model Optimization).**  
2. **Hanes, D., Salgueiro, G., Grossetete, P., Barton, R., & Henry, J. (2017).** *IoT Fundamentals: Networking Technologies, Protocols, and Use Cases for the Internet of Things*. Cisco Press. (ISBN-13: 978-1587144561) — **Bab 7 (Edge and Fog Analytics).**  
3. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730)  
4. **Python Software Foundation. (2024).** *Python Standard Library: `collections.deque` — High-performance container datatypes*. Tersedia: [https://docs.python.org/3/library/collections.html](https://docs.python.org/3/library/collections.html)  
5. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  
