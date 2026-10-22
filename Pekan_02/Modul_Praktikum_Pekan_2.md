---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Modul Praktikum Pekan 2: Akuisisi Data, Message Broker (Mosquitto & RabbitMQ), dan Simulasi Sensor Python"
---

# Modul Praktikum Pekan 2: Akuisisi Data, Message Broker (Mosquitto & RabbitMQ), dan Simulasi Sensor Python

**Mata Kuliah:** Analisis Data untuk IoT (2 SKS: 1 SKS Teori, 1 SKS Praktikum)  
**Program Studi:** Teknologi Rekayasa Komputer (TRK) — Politeknik Pertanian Negeri Payakumbuh  
**Pertemuan:** Pekan 2 (Praktikum — $1 \times 170$ Menit)  
**Capaian Pembelajaran (Sub-CPMK 2):** Mahasiswa mampu menyiapkan infrastruktur server IoT (Ubuntu Server pada VirtualBox), mengonfigurasi *Message Broker* (Mosquitto MQTT & RabbitMQ AMQP), memahami anatomi topik/QoS MQTT, serta membangun skrip akuisisi data (*Publisher & Subscriber*) dan simulator data sensor menggunakan Python.

---

## 1. Tujuan Pembelajaran & Praktikum
Setelah menyelesaikan modul praktikum pekan ini, mahasiswa mampu:
1. Menjelaskan perbedaan mendasar antara arsitektur komunikasi **Publish/Subscribe (MQTT/AMQP)** dan **Request/Response (HTTP)** pada sistem akuisisi data IoT.
2. Menganalisis struktur hierarki **Topic MQTT**, penggunaan karakter *wildcard* (`+` dan `#`), mekanisme **Quality of Service (QoS 0, 1, 2)**, serta perbandingan format serialisasi *payload* (Teks/JSON vs Biner).
3. Melakukan instalasi dan konfigurasi jaringan **Ubuntu Server 24.04 LTS** di atas **VirtualBox** beserta akses *remote* melalui **SSH**.
4. Menginstal, mengamankan, dan mengelola layanan *Message Broker* **Eclipse Mosquitto (MQTT)** dan **RabbitMQ (AMQP & MQTT)** menggunakan `systemctl` tanpa konflik *port*.
5. Menulis dan mengeksekusi program **Python** menggunakan pustaka `paho-mqtt`, `pika`, `pandas`, dan `numpy` untuk membangkitkan data sensor tiruan (*dummy*) serta melakukan pengiriman (*publish*) dan penangkapan (*subscribe*) telemetri secara waktu nyata (*real-time*).

---

## 2. Landasan Teori: Arsitektur Akuisisi Data (*Data Ingestion*) pada IoT

Sebelum data sensor dapat dibersihkan atau dianalisis menggunakan *Machine Learning*, data tersebut harus terlebih dahulu dikirimkan dari perangkat lapangan (*Edge*) menuju server pengumpul data. Tahap penangkapan dan penyaluran aliran data masuk ini disebut **Data Ingestion**.

### 2.1 Mengapa Protokol HTTP Kurang Ideal untuk Ribuan Sensor IoT?
Pada aplikasi web biasa, komunikasi menggunakan protokol **HTTP (*Request-Response*)** yang bersifat sinkron dan berorientasi dokumen:
- Setiap kali ingin mengirim 1 angka suhu, sensor harus membuka koneksi TCP, mengirim *header* teks HTTP yang gemuk (ratusan *byte*), dan menunggu balasan `200 OK` dari server.
- Jika server pusat sedang sibuk atau jaringan seluler di kebun terputus sesaat, pengiriman langsung gagal.
- Server tidak dapat dengan mudah menyebarkan (*broadcast*) satu data sensor ke 5 aplikasi penerima berbeda secara bersamaan tanpa di-*polling* berulang-ulang.

### 2.2 Arsitektur Publish/Subscribe (*Pub/Sub*) pada MQTT
Untuk mengatasi kelemahan HTTP, industri IoT menggunakan pola **Publish/Subscribe** dengan protokol ringan seperti **MQTT (*Message Queuing Telemetry Transport*)** yang memiliki ukuran *header* paket minimal hanya **2 Bytes**.

```text
   [ EDGE NODES / PUBLISHERS ]               [ MESSAGE BROKER ]              [ SUBSCRIBERS / CONSUMERS ]
 (Tidak tahu siapa penerimanya)         (Mosquitto / RabbitMQ :1883)       (Tidak tahu siapa pengirimnya)

+-----------------------------+                                           +------------------------------+
| Sensor Suhu Zona A (ESP32)  | -- Publish("kebun/zonaA/suhu", "28.5") -> | Skrip Python Data Cleaner    |
+-----------------------------+                    |                      | Subscribe("kebun/+/suhu")    |
                                                   v                      +------------------------------+
+-----------------------------+         +---------------------+                          ^
| Sensor Tanah Zona A (ESP32) | ------> |   ROUTER TOPIK &    | -------------------------+
+-----------------------------+         |   ANTREAN PESAN     |
                                        +---------------------+ -------------------------+
+-----------------------------+                    ^                                     v
| Sensor Suhu Zona B (ESP32)  | -------------------+                      +------------------------------+
+-----------------------------+                                           | Dashboard Pemantauan Live    |
                                                                          | Subscribe("kebun/#")         |
                                                                          +------------------------------+
```

Tiga prinsip decouple (pemisahan ketergantungan) pada MQTT:
1. **Space Decoupling:** *Publisher* (sensor) dan *Subscriber* (aplikasi analitik) tidak perlu saling mengetahui alamat IP satu sama lain; keduanya hanya perlu mengetahui alamat IP **Broker**.
2. **Time Decoupling:** *Publisher* dan *Subscriber* tidak harus aktif pada detik yang persis bersamaan (terutama jika menggunakan fitur *Persistent Session / Queue* atau *Retained Message*).
3. **Synchronization Decoupling:** Pengiriman dan penerimaan pesan berjalan secara asinkron (*event-driven callback*) tanpa memblokir eksekusi utama.

---

### 2.3 Anatomi Hierarki Topik MQTT & Karakter Wildcard (`+` vs `#`)
Di dalam MQTT, pesan tidak dikirim ke alamat tabel database, melainkan ke sebuah jalur bernama **Topic** yang dipisahkan oleh garis miring (`/`) layaknya struktur folder di Linux. **Topik bersifat *case-sensitive*** (`Suhu` berbeda dengan `suhu`).

Contoh struktur topik di rumah kaca (*Smart Greenhouse*):
- `greenhouse/zone_A/sensor/temperature`
- `greenhouse/zone_A/sensor/humidity`
- `greenhouse/zone_B/sensor/temperature`
- `greenhouse/zone_B/sensor/soil_moisture`

Seorang *Data Engineer* yang menulis skrip *Subscriber* dapat berlangganan ke banyak topik sekaligus menggunakan dua karakter **Wildcard**:

| Simbol Wildcard | Nama Teknis | Aturan Pencocokan (*Matching Rule*) | Contoh Pola *Subscribe* | Topik yang Tertangkap |
| :---: | :--- | :--- | :--- | :--- |
| **`+`** | **Single-Level Wildcard** | Mencocokkan **tepat satu level** hierarki di posisi tersebut. Dapat diletakkan di tengah atau akhir topik. | `greenhouse/+/sensor/temperature` | Mencocokkan `greenhouse/zone_A/sensor/temperature` dan `greenhouse/zone_B/sensor/temperature` (mengabaikan `humidity` dan `soil_moisture`). |
| **`#`** | **Multi-Level Wildcard** | Mencocokkan **seluruh level anak di bawahnya** (satu atau banyak level). **Wajib diletakkan di paling akhir** topik. | `greenhouse/zone_A/#` | Mencocokkan semua sensor dan sub-topik apa pun yang berada di bawah `zone_A`. |

---

### 2.4 Tingkat Layanan Pengiriman: Quality of Service (QoS 0, 1, dan 2)
Kondisi sinyal radio di lahan pertanian sering mengalami *packet loss*. MQTT menyediakan 3 level **QoS** yang menentukan kesepakatan jaminan pengiriman antara pengirim dan broker:

1. **QoS 0 — *At Most Once* ("Fire and Forget"):**
   - Pengirim hanya menembakkan paket 1 kali tanpa menunggu konfirmasi (*ACK*). Jika sinyal putus saat itu, paket hilang selamanya.
   - *Kelebihan:* Paling cepat, paling hemat *bandwidth* dan baterai.
   - *Penggunaan:* Data telemetri suhu/kelembaban rapat (misal dikirim tiap 5 detik) di mana kehilangan 1 titik data tidak berbahaya karena masih bisa diinterpolasi.
2. **QoS 1 — *At Least Once* (Minimal Satu Kali Sampai):**
   - Pengirim menyimpan pesan sementara dan terus mengirim ulang hingga menerima balasan `PUBACK` dari Broker.
   - *Konsekuensi Analitik:* Jika balasan `PUBACK` terlambat di jalan, pengirim akan mengirim ulang pesan yang sama, sehingga **berpotensi menimbulkan data duplikat (*duplicate rows*)** di sisi *Subscriber*. Skrip pembersih data (*Data Cleaner*) di server wajib memiliki logika deduplikasi (misal memeriksa duplikasi `timestamp` dan `sensor_id`).
3. **QoS 2 — *Exactly Once* (Tepat Satu Kali Sampai):**
   - Menggunakan jabat tangan 4 tahap (*4-step handshake*: `PUBLISH` $\rightarrow$ `PUBREC` $\rightarrow$ `PUBREL` $\rightarrow$ `PUBCOMP`). Dijamin sampai dan tidak pernah duplikat.
   - *Kelemahan:* Menghasilkan *overhead* lalu lintas jaringan paling berat dan latensi paling tinggi. Pada jaringan yang sudah buruk/macet, QoS 2 justru dapat memperparah kemacetan antrean paket.
   - *Penggunaan:* Hanya untuk perintah aktuator kritis (misal perintah membuka pintu air bendungan atau pemberian dosis pupuk kimia pekat) yang tidak boleh dieksekusi dua kali.

---

### 2.5 Perbandingan MQTT (Mosquitto) vs AMQP (RabbitMQ) & Format Payload
- **Mosquitto (Murni MQTT Broker):** Sangat ringan (ditulis dalam bahasa C), memakan RAM hanya beberapa Megabyte, ideal dipasang di *Edge/Fog Gateway* untuk melayani ribuan mikrokontroler.
- **RabbitMQ (Multi-Protocol Enterprise Broker - AMQP & MQTT):** Menggunakan arsitektur **Exchange $\rightarrow$ Binding (*Routing Key*) $\rightarrow$ Queue (Antrean)**. Jika skrip Python pemroses data (*Consumer*) di server tiba-tiba mati/*crash* selama 1 jam, RabbitMQ mampu **menampung dan mengantrekan seluruh pesan yang masuk di dalam *Queue*** (di RAM/Disk) sehingga tidak ada satu pun data sensor yang terbuang saat skrip hidup kembali.
- **Format Serialisasi Payload:**
  - **JSON (`{"suhu": 28.5, "rh": 70}`):** Mudah dibaca manusia (*human-readable*) dan langsung didukung oleh Python (`json.loads()`), namun ukuran byte lebih besar karena mengulang nama kunci teks.
  - **Format Biner (*Protocol Buffers / MessagePack / Raw Struct*):** Menyandikan angka langsung ke dalam urutan byte biner padat. Ukuran payload jauh lebih kecil (hemat kuota satelit/LoRaWAN) dan *parsing* CPU jauh lebih cepat, namun memerlukan skema *decoder* khusus.

---

## 3. Kebutuhan Sistem Praktikum (*System Requirements*)

Untuk memastikan praktikum berjalan dengan lancar tanpa hambatan kinerja, berikut adalah persyaratan sistem yang direkomendasikan beserta alasan teknisnya:

- **Prosesor (CPU):** Minimal 4 Core (disarankan Intel Core i5/Ryzen 5 ke atas).
  *Alasan:* Sistem operasi Host (Windows) akan berjalan bersamaan dengan Guest (Ubuntu Server di VirtualBox). Multicore memastikan pembagian beban kerja yang baik sehingga host tidak terhambat (*lag*) saat VM berjalan. Pastikan fitur **Virtualization (VT-x atau AMD-V)** sudah diaktifkan di BIOS/UEFI.
- **RAM:** Minimal 8 GB (Direkomendasikan 16 GB).
  *Alasan:* OS Windows umumnya memakan 4–6 GB RAM. Ubuntu Server VM akan dialokasikan minimal 2 GB RAM. Jika RAM di bawah 8 GB, sistem akan sering melakukan *swapping* memori ke harddisk yang membuat komputer menjadi sangat lambat.
- **Penyimpanan (Storage):** Minimal 20 GB ruang kosong, disarankan menggunakan SSD.
  *Alasan:* Virtual Machine sangat bergantung pada kecepatan I/O (*Input/Output*) disk. Menggunakan HDD konvensional akan membuat proses booting VM dan instalasi paket menjadi sangat lama.
- **Koneksi Internet:** Diperlukan untuk mengunduh ISO Ubuntu, paket update, dan instalasi aplikasi (`apt install` & `pip install`).

---

## 4. Instalasi VirtualBox, Ubuntu Server 24.04 LTS, dan Setup Jaringan

### 4.1 Instalasi VirtualBox dan Konsep Mode Jaringan Virtual
1. **Instalasi:** Berdasarkan penggunaan versi terbaru (VirtualBox 7.2.x), silakan mengacu pada panduan resmi di dokumentasi *VirtualBox User Manual* [1].
2. **Setup Jaringan VirtualBox:**
   VirtualBox menyediakan beberapa mode jaringan yang wajib dipahami agar Host (Windows) dan Guest (Ubuntu Server) dapat saling berkomunikasi:
   - **NAT (*Network Address Translation*):** Mode bawaan saat membuat VM baru. Guest OS (Ubuntu) dapat terkoneksi ke internet dengan "meminjam" jalur Host. **Komplikasi:** Host OS (Windows) *tidak akan bisa* mengakses atau melakukan *ping* ke IP Guest OS secara langsung karena bersembunyi di balik NAT VirtualBox. Jika Anda hanya menggunakan NAT tanpa *Port Forwarding*, Anda akan gagal melakukan *remote* SSH atau koneksi MQTT ke VM.
   - **Host-Only Adapter:** Membuat kartu jaringan virtual khusus agar Host OS dan Guest OS saling terhubung secara privat. Keduanya bisa saling berkomunikasi dengan lancar, namun Guest tidak mendapat akses internet melalui adapter ini.
   - **Bridged Adapter:** Guest OS mem-*bypass* Host dan terhubung langsung ke router fisik (misalnya via WiFi atau kabel LAN). Guest akan mendapatkan IP langsung dari jaringan fisik yang sama dengan Host.

   **Rekomendasi Konfigurasi Jaringan Praktikum:**
   1. **Kombinasi NAT + Host-Only (Rekomendasi Utama Paling Stabil):** Pasang 2 *Network Adapter* pada VM Anda. Adapter 1 menggunakan **NAT** (untuk koneksi internet VM agar bisa `apt update`/`install`). Adapter 2 menggunakan **Host-Only Adapter** (agar Host OS bisa me-*remote* VM via SSH dan mengirim paket MQTT secara stabil meskipun berpindah WiFi).
   2. **Bridged Adapter Saja (Alternatif):** Gunakan **Bridged Adapter** jika berada di jaringan LAN/WiFi rumah yang stabil. *Catatan:* Beberapa router hotspot publik/kampus mengaktifkan *Client Isolation* yang memblokir komunikasi antar-IP di WiFi yang sama.

### 4.2 Instalasi OS Ubuntu Server 24.04 LTS di VirtualBox
1. Unduh file ISO Ubuntu Server 24.04 LTS dari situs web resmi Ubuntu [2] atau gunakan **Mirror Lokal Indonesia** pada bagian Lampiran modul ini agar unduhan jauh lebih cepat.
2. Buka VirtualBox $\rightarrow$ Klik **New**.
3. Beri nama (contoh: `Ubuntu_Server_IoT`), pilih tipe **Linux**, versi **Ubuntu (64-bit)**.
4. Alokasikan RAM: **2048 MB** (2 GB) atau lebih.
5. Buat Virtual Hard Disk baru (VDI, *Dynamically allocated*) sebesar **20 GB**.
6. Masuk ke **Settings** VM $\rightarrow$ **Network** $\rightarrow$ Atur sesuai rekomendasi di atas (Bridged Adapter atau NAT + Host-Only).
7. Masuk ke **Settings** VM $\rightarrow$ **Storage** $\rightarrow$ Masukkan file ISO Ubuntu 24.04 di bagian CD/DVD.
8. Jalankan VM (**Start**) dan ikuti wizard instalasi:
   - Gunakan pengaturan default untuk layout keyboard.
   - Pada bagian Network, pastikan *interface* mendapatkan IP Address.
   - **Wajib mencentang `"Install OpenSSH server"`** agar server dapat dikendalikan via SSH dari terminal Windows.
   - Buat *username* dan *password* (contoh: user `praktikan`, password `rahasia`).
   - Tunggu proses instalasi selesai dan lakukan *Reboot*.

### 4.3 Mengakses Server Secara Remote Menggunakan SSH (Terminal Windows)
Untuk mempermudah pekerjaan (*copy-paste* perintah dan kode), kendalikan Ubuntu Server secara jarak jauh (*remote*) dari Host OS (Windows) menggunakan protokol SSH:
1. Pastikan Ubuntu VM sedang menyala, lalu ketik `ip a` di terminal VM untuk melihat IP Address (misal: `192.168.1.100`).
2. Buka **Command Prompt (CMD)** atau **PowerShell** bawaan Windows 10/11 (tidak perlu menginstal PuTTY).
3. Jalankan perintah SSH berikut:
   ```bash
   ssh nama_user@ip_address_ubuntu
   # Contoh: ssh praktikan@192.168.1.100
   ```
4. Ketik `yes` saat konfirmasi *fingerprint* muncul pertama kali, lalu masukkan *password* Ubuntu Anda.

### 4.4 Instalasi Paket Pendukung Python di Ubuntu Server
Ubuntu Server 24.04 sudah memiliki Python 3 bawaan, namun kita perlu memasang paket alias `python-is-python3` dan modul *Virtual Environment* (`python3-venv`):
```bash
sudo apt update
sudo apt install -y python-is-python3 python3-venv python3-pip
```
*Penjelasan Teknis:* `python-is-python3` memetakan perintah `python` agar langsung merujuk ke `python3`. Sementara `python3-venv` diperlukan karena Ubuntu 24.04 menerapkan aturan keamanan **PEP 668 (*Externally Managed Environment*)** yang melarang instalasi `pip install` langsung ke sistem OS global tanpa *virtual environment*.

---

## 5. Instalasi dan Konfigurasi Message Broker (Mosquitto & RabbitMQ)

### 5.1 Setup Eclipse Mosquitto (MQTT Broker)
1. Login ke Ubuntu Server via SSH, lalu instal paket `mosquitto` beserta aplikasi klien pengujiannya:
   ```bash
   sudo apt update
   sudo apt install -y mosquitto mosquitto-clients
   ```
2. Periksa apakah *daemon service* Mosquitto sudah aktif berjalan di latar belakang:
   ```bash
   sudo systemctl status mosquitto
   ```
   *(Pastikan berstatus `active (running)`, tekan `q` untuk keluar).*

### 5.2 Setup RabbitMQ (AMQP & Multi-Protocol Broker)
1. Instal paket `rabbitmq-server`:
   ```bash
   sudo apt update
   sudo apt install -y rabbitmq-server
   ```
2. Aktifkan *plugin* antarmuka web manajemen (**RabbitMQ Management Dashboard** di port `15672`):
   ```bash
   sudo rabbitmq-plugins enable rabbitmq_management
   ```
3. (Opsional) Aktifkan *plugin* adaptor MQTT jika ingin RabbitMQ juga menerima koneksi protokol MQTT:
   ```bash
   sudo rabbitmq-plugins enable rabbitmq_mqtt
   ```

### 5.3 Manajemen Service Systemd & Resolusi Konflik Port (`1883` vs `1884`)
**Akar Masalah:** Secara standar (*default*), protokol MQTT menggunakan port **`1883`**. Jika Mosquitto sedang berjalan di port `1883` dan Anda mengaktifkan `rabbitmq_mqtt`, maka keduanya akan berebut port `1883` (*Address already in use / Port Conflict*).

#### Cara A: Memilih Salah Satu Service dengan `systemctl`
```bash
# Menghentikan Mosquitto sementara:
sudo systemctl stop mosquitto

# Menonaktifkan Mosquitto saat booting:
sudo systemctl disable mosquitto

# Mengaktifkan RabbitMQ sebagai broker utama:
sudo systemctl enable --now rabbitmq-server
```

#### Cara B: Menggeser Port Mosquitto ke `1884` Agar Berjalan Berdampingan
Jika kita ingin **Mosquitto (port 1884)** dan **RabbitMQ MQTT (port 1883)** hidup berdampingan di server yang sama:
1. Buat file konfigurasi kustom Mosquitto:
   ```bash
   sudo nano /etc/mosquitto/conf.d/custom.conf
   ```
2. Tambahkan konfigurasi berikut agar Mosquitto mendengarkan koneksi dari seluruh antarmuka jaringan (`0.0.0.0`) pada port `1884`:
   ```text
   listener 1884 0.0.0.0
   allow_anonymous true
   ```
   *(Catatan Penting: Sejak Mosquitto versi 2.0+, jika kita tidak menulis `0.0.0.0`, Mosquitto hanya mengizinkan koneksi dari dalam `localhost` dan menolak koneksi dari laptop Windows/ESP32 luar!).*
3. Simpan (`Ctrl+O`, `Enter`, `Ctrl+X`) dan muat ulang layanan Mosquitto:
   ```bash
   sudo systemctl restart mosquitto
   ```

---

## 6. Pengamanan Broker & Manajemen Antrean Tingkat Lanjut

### 6.1 Mengaktifkan Autentikasi Username & Password pada Mosquitto
Membiarkan broker MQTT terbuka (`allow_anonymous true`) di jaringan produksi sangat berbahaya karena siapa saja dapat menyuntikkan data sensor palsu (*False Data Injection*).
1. Buat file kredensial terenkripsi untuk pengguna `admin`:
   ```bash
   sudo mosquitto_passwd -c /etc/mosquitto/passwd admin
   ```
   *(Ketikkan password, misalnya: `passwordrahasia`).*
2. Ubah konfigurasi `/etc/mosquitto/conf.d/custom.conf`:
   ```text
   listener 1884 0.0.0.0
   allow_anonymous false
   password_file /etc/mosquitto/passwd
   ```
3. Terapkan perubahan:
   ```bash
   sudo systemctl restart mosquitto
   ```

### 6.2 Konfigurasi User Administrator & Konsep Exchange-Queue pada RabbitMQ
Berbeda dengan Mosquitto yang langsung meneruskan topik, RabbitMQ (AMQP) menggunakan arsitektur:
- **Exchange:** Pintu masuk yang menerima pesan dari *Publisher* dan meneruskannya ke antrean berdasarkan aturan rute.
- **Queue (Antrean):** Penampung pesan (*buffer* di RAM/Disk) sebelum diambil (*consume*) oleh *Subscriber*.
- **Routing Key / Binding:** Aturan penghubung antara *Exchange* dan *Queue*.

Buat akun administrator baru pada RabbitMQ (karena akun bawaan `guest` hanya diizinkan login dari `localhost`):
```bash
sudo rabbitmqctl add_user admin passwordrahasia
sudo rabbitmqctl set_user_tags admin administrator
sudo rabbitmqctl set_permissions -p / admin ".*" ".*" ".*"
```
Sekarang Anda dapat membuka browser di Windows dan mengakses dashboard visual RabbitMQ di **`http://[IP_UBUNTU]:15672`** menggunakan akun `admin` / `passwordrahasia`.

---

## 7. Eksperimen Pemrograman Python: Akuisisi Data & Simulasi Sensor

### Langkah 1: Persiapan Virtual Environment Python & Instalasi Pustaka
Buka terminal (di Ubuntu Server atau CMD Windows) dan siapkan lingkungan kerja terisolasi:
```bash
# 1. Membuat virtual environment bernama 'env_iot'
python -m venv env_iot

# 2. Mengaktifkan virtual environment:
# Di Linux/Ubuntu:
source env_iot/bin/activate
# Di Windows CMD:
# env_iot\Scripts\activate

# 3. Menginstal pustaka Data Science dan klien protokol IoT
pip install pandas numpy paho-mqtt pika
```

### Langkah 2: Membangun Generator Dataset Sensor Tiruan (`generator_sensor.py`)
Dalam tahap pengembangan sistem analitik, kita seringkali harus menguji *pipeline* sebelum sensor fisik selesai dipasang di kebun. Skrip berikut menggunakan distribusi statistik **Normal (Gaussian)** dari NumPy untuk membangkitkan data mikroklimat selama 1 minggu (168 jam):

```python
import pandas as pd
import numpy as np

# Kunci seed agar angka acak konsisten saat diuji ulang
np.random.seed(42)

# Membuat deret waktu (timestamp) dengan frekuensi 1 Jam ('h') selama 7 hari
waktu = pd.date_range(start="2026-10-01 00:00:00", end="2026-10-07 23:00:00", freq="h")

# Membangkitkan data suhu udara (Rata-rata/loc = 28.0°C, Standar Deviasi/scale = 2.0°C)
suhu = np.random.normal(loc=28.0, scale=2.0, size=len(waktu))

# Membangkitkan data kelembaban tanah (Rata-rata = 65.0%, Standar Deviasi = 5.0%)
kelembaban = np.random.normal(loc=65.0, scale=5.0, size=len(waktu))

# Menggabungkan ke dalam struktur tabel Pandas DataFrame
df = pd.DataFrame({
    "timestamp": waktu,
    "suhu": np.round(suhu, 2),
    "kelembaban_tanah": np.round(kelembaban, 2)
})

# Menyimpan ke berkas CSV untuk bahan analisis modul berikutnya
df.to_csv("data_sensor_dummy.csv", index=False)
print(f"Berhasil membangkitkan {len(df)} baris data sensor ke 'data_sensor_dummy.csv'!")
print(df.head())
```

### Langkah 3: Simulasi Perangkat Edge — MQTT Publisher (`mqtt_publish.py`)
Skrip ini mensimulasikan sebuah mikrokontroler ESP32 di lahan yang membaca sensor dan mempublikasikan nilainya ke Broker Mosquitto setiap 2 detik:

```python
import paho.mqtt.client as mqtt
import time
import random

# Sesuaikan dengan IP Ubuntu VM Anda (atau "localhost" jika dijalankan di dalam VM)
BROKER_ADDRESS = "192.168.1.100"
PORT = 1884
TOPIC = "kebun/lahan1/suhu"

def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code == 0:
        print(f"[CONNECTED] Berhasil terhubung ke Broker MQTT ({BROKER_ADDRESS}:{PORT})")
    else:
        print(f"[ERROR] Gagal terhubung, kode error: {reason_code}")

# Inisialisasi client dengan API Versi 2 (standar paho-mqtt v2.x terbaru)
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, client_id="Edge_Sensor_Suhu_01")
client.username_pw_set("admin", "passwordrahasia")
client.on_connect = on_connect

client.connect(BROKER_ADDRESS, port=PORT, keepalive=60)
client.loop_start()

try:
    iterasi = 0
    while True:
        iterasi += 1
        suhu_saat_ini = round(random.uniform(25.0, 35.0), 2)
        
        # Mengirimkan pesan dengan QoS 1 (Minimal sampai satu kali)
        client.publish(TOPIC, payload=str(suhu_saat_ini), qos=1)
        print(f"[{iterasi}] Publish -> Topik: {TOPIC} | Payload: {suhu_saat_ini} °C")
        time.sleep(2)
except KeyboardInterrupt:
    print("\nMenghentikan transmisi sensor...")
    client.loop_stop()
    client.disconnect()
```

### Langkah 4: Simulasi Server Pengumpul — MQTT Subscriber (`mqtt_subscribe.py`)
Skrip ini berjalan di sisi server untuk mendengarkan topik menggunakan *wildcard* `+` sehingga dapat menangkap data suhu dari lahan mana pun (`lahan1`, `lahan2`, dst.):

```python
import paho.mqtt.client as mqtt

BROKER_ADDRESS = "192.168.1.100"
PORT = 1884
# Wildcard '+' mencocokkan semua nama lahan pada level kedua
TOPIC_SUBSCRIBE = "kebun/+/suhu"

def on_connect(client, userdata, flags, reason_code, properties):
    print(f"[SERVER READY] Terhubung ke Broker dengan kode: {reason_code}")
    # Langsung berlangganan topik saat koneksi berhasil (dan otomatis re-subscribe jika putus)
    client.subscribe(TOPIC_SUBSCRIBE, qos=1)
    print(f"[SUBSCRIBED] Mendengarkan pola topik: {TOPIC_SUBSCRIBE}")

def on_message(client, userdata, message):
    # Payload diterima dalam bentuk raw bytes, wajib di-decode ke string UTF-8
    data_teks = message.payload.decode("utf-8")
    print(f"[TERIMA DATA] Topik: {message.topic} | Nilai: {data_teks} °C (QoS: {message.qos})")

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, client_id="Server_Data_Ingestor")
client.username_pw_set("admin", "passwordrahasia")
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER_ADDRESS, port=PORT, keepalive=60)
print("Menunggu aliran data telemetri... (Tekan Ctrl+C untuk berhenti)")
client.loop_forever()
```

### Langkah 5: Konsumsi Antrean Pesan via Protokol AMQP RabbitMQ (`amqp_subscriber.py`)
Untuk menguji antrean pesan langsung pada RabbitMQ menggunakan protokol AMQP (port `5672`), jalankan skrip berikut:

```python
import pika
import sys
import os

def main():
    credentials = pika.PlainCredentials('admin', 'passwordrahasia')
    parameters = pika.ConnectionParameters('192.168.1.100', 5672, '/', credentials)
    connection = pika.BlockingConnection(parameters)
    channel = connection.channel()

    # Mendeklarasikan antrean (Queue) agar tidak error jika antrean belum dibuat
    channel.queue_declare(queue='antrean_sensor', durable=True)

    def callback(ch, method, properties, body):
        print(f" [x] Menerima dari AMQP Queue: {body.decode('utf-8')}")

    channel.basic_consume(queue='antrean_sensor', on_message_callback=callback, auto_ack=True)
    print(' [*] Menunggu pesan dari RabbitMQ AMQP. Tekan CTRL+C untuk keluar.')
    channel.start_consuming()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('\nKeluar dari program...')
        try:
            sys.exit(0)
        except SystemExit:
            os._exit(0)
```

---

## 8. Panduan Troubleshooting Masalah Umum di Laboratorium

| Gejala / Pesan Error | Penyebab Teknis | Solusi Perbaikan |
| :--- | :--- | :--- |
| `ConnectionRefusedError: [Errno 111] Connection refused` | Layanan Mosquitto/RabbitMQ belum jalan, salah port, atau Mosquitto hanya *bind* ke `127.0.0.1`. | Cek `sudo systemctl status mosquitto`. Pastikan baris `listener 1884 0.0.0.0` sudah tertulis di `/etc/mosquitto/conf.d/custom.conf`, lalu *restart* service. |
| `TypeError: Client.__init__() missing 1 required positional argument: 'callback_api_version'` | Pustaka `paho-mqtt` versi 2.0+ mewajibkan argumen `CallbackAPIVersion`. | Gunakan `mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, client_id="...")` saat inisialisasi objek klien. |
| Subscriber terhubung (kode `0`), tetapi tidak ada pesan yang muncul saat Publisher mengirim | Nama topik antara Publisher dan Subscriber tidak identik (berbeda huruf besar/kecil atau garis miring). | Periksa kembali string topik. Ingat bahwa `Kebun/lahan1/suhu` $\neq$ `kebun/lahan1/suhu`. |
| Koneksi terputus berulang-ulang setiap beberapa detik (*disconnect loop*) | Dua program (Publisher dan Subscriber) menggunakan string `client_id` yang sama persis. | Broker MQTT melarang dua klien aktif memiliki `client_id` kembar (klien lama akan ditendang). Berikan nama `client_id` yang unik untuk setiap skrip. |

---

## 9. Tugas Mandiri Praktikum
1. **Serialisasi Payload JSON Multivariat:**
   Modifikasi skrip `mqtt_publish.py` menggunakan modul bawaan `json` (`import json`) agar mengirimkan satu paket *payload* berformat JSON setiap 2 detik ke topik `kebun/zona_A/telemetri` yang memuat 4 informasi sekaligus:
   `{"timestamp": "2026-10-22 10:00:00", "node_id": "ESP32_A1", "suhu": 29.4, "kelembaban_tanah": 62.1}`
2. **Parsing & Penyimpanan Log Otomatis di Subscriber:**
   Modifikasi skrip `mqtt_subscribe.py` agar menguraikan (*parse*) string JSON yang diterima menggunakan `json.loads()`, mencetak peringatan `[PERINGATAN PANAS]` jika `suhu > 32.0`, dan menyimpan setiap baris data yang masuk secara otomatis ke dalam berkas `log_telemetri_mqtt.csv`!

---

## 10. Referensi & Acuan Materi Pekan Ini

1. **Hanes, D., Salgueiro, G., Grossetete, P., Barton, R., & Henry, J. (2017).** *IoT Fundamentals: Networking Technologies, Protocols, and Use Cases for the Internet of Things*. Cisco Press. (ISBN-13: 978-1587144561) — **Bab 6 (Application Protocols for IoT: MQTT, CoAP, AMQP, Pub/Sub Architecture & QoS).**  
2. **Minteer, A. (2017).** *Analytics for the Internet of Things (IoT)*. Packt Publishing Ltd. (ISBN-13: 978-1787120730) — **Bab 2 (IoT Analytics for the Cloud: Data Ingestion with MQTT).**  
3. **Oracle. (2024).** *VirtualBox User Manual (Bab Virtual Networking)*. Tersedia: [https://docs.oracle.com/en/virtualization/virtualbox/](https://docs.oracle.com/en/virtualization/virtualbox/)  
4. **Canonical Ltd. (2024).** *Ubuntu Server 24.04 LTS Documentation*. Tersedia: [https://ubuntu.com/server/docs](https://ubuntu.com/server/docs)  
5. **Eclipse Foundation. (2024).** *Eclipse Mosquitto & Paho MQTT Python Client Documentation*. Tersedia: [https://mosquitto.org/documentation/](https://mosquitto.org/documentation/)  
6. **Broadcom / VMware. (2024).** *RabbitMQ & Pika Python AMQP Documentation*. Tersedia: [https://www.rabbitmq.com/documentation.html](https://www.rabbitmq.com/documentation.html)  
7. Lihat kompilasi acuan selengkapnya pada dokumen [`../Referensi/Daftar_Referensi_Lengkap.md`](../Referensi/Daftar_Referensi_Lengkap.md).  

---

## Lampiran: Daftar Mirror Lokal Indonesia untuk Unduhan ISO Ubuntu Server 24.04 LTS
Gunakan salah satu tautan repositori lokal berikut untuk mempercepat proses unduhan ISO Ubuntu Server 24.04 LTS:
- **BiznetGio:** `https://repo.biznetgio.com/ubuntu-releases/24.04/`
- **Datautama (Kartolo):** `https://kartolo.sby.datautama.net.id/ubuntu-cd/24.04/`
- **PENS (Politeknik Elektronika Negeri Surabaya):** `https://kebo.pens.ac.id/ubuntu-releases/24.04/`
- **UNAIR (Universitas Airlangga):** `https://repo.unair.ac.id/ubuntu-releases/24.04/`
