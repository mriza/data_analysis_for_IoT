---
mata_kuliah: "Analisis Data untuk IoT"
semester: 5
dosen: 
  - "Mohammad Riza Nurtam"
  - "Ega Evinda Putri"
title: "Modul Praktikum Pekan 2: Instalasi dan Konfigurasi Message Broker"
---

# Modul Praktikum Pekan 2: Instalasi dan Konfigurasi Message Broker

## Tujuan Praktikum

Mahasiswa mampu melakukan instalasi dan konfigurasi sistem operasi server, message broker (Mosquitto dan RabbitMQ), serta memahami konsep pengiriman pesan menggunakan MQTT dan AMQP.

---

## 0. Kebutuhan Sistem (System Requirements)

Untuk memastikan praktikum berjalan dengan lancar tanpa hambatan kinerja, berikut adalah persyaratan sistem yang direkomendasikan beserta alasannya:

- **Prosesor (CPU):** Minimal 4 Core (disarankan Intel Core i5/Ryzen 5 ke atas).
  *Alasan:* Sistem operasi Host (Windows) akan berjalan bersamaan dengan Guest (Ubuntu Server di VirtualBox). Multicore memastikan pembagian beban kerja yang baik sehingga host tidak terhambat (*lag*) saat VM berjalan. Pastikan fitur **Virtualization (VT-x atau AMD-V)** sudah diaktifkan di BIOS/UEFI.
- **RAM:** Minimal 8 GB (Direkomendasikan 16 GB).
  *Alasan:* OS Windows umumnya memakan 4-6 GB RAM. Ubuntu Server VM akan dialokasikan minimal 2 GB RAM. Jika RAM di bawah 8 GB, sistem akan sering melakukan *swapping* memori ke harddisk yang membuat komputer menjadi sangat lambat.
- **Penyimpanan (Storage):** Minimal 20 GB ruang kosong, disarankan menggunakan SSD.
  *Alasan:* Virtual Machine sangat bergantung pada kecepatan I/O (Input/Output) disk. Menggunakan HDD konvensional akan membuat proses booting VM dan instalasi paket menjadi sangat lama.
- **Koneksi Internet:** Diperlukan untuk mengunduh ISO Ubuntu, paket update, dan instalasi aplikasi (`apt install`).

---

## 1. Instalasi VirtualBox di Windows dan Setup Jaringan

1. **Instalasi:** Berdasarkan penggunaan versi terbaru (VirtualBox 7.2.x), silakan mengacu langsung pada panduan resmi instalasi di dokumentasi VirtualBox 7.2 User Manual [1]. Pastikan Anda mengikuti langkah instalasi standar sesuai dengan sistem operasi Host (Windows).
2. **Setup Jaringan VirtualBox:**
   VirtualBox menyediakan beberapa mode jaringan. Pemahaman mode ini penting untuk komunikasi antara komputer host dan guest:

   - **NAT (Network Address Translation):** Mode default saat membuat VM baru. Pada mode ini, Guest OS (Ubuntu) dapat terkoneksi ke internet dengan "meminjam" jalur Host. **Komplikasi:** Host OS (Windows) *tidak akan bisa* mengakses atau melakukan *ping* ke IP Guest OS secara langsung karena bersembunyi di balik NAT VirtualBox. Jika Anda hanya menggunakan NAT, Anda akan gagal melakukan remote SSH atau koneksi MQTT ke VM.
   - **Host-Only Adapter:** Membuat kartu jaringan virtual khusus agar Host OS dan Guest OS saling terhubung secara privat. Keduanya bisa saling berkomunikasi dengan lancar, namun Guest tidak mendapat akses internet melalui adapter ini.
   - **Bridged Adapter:** Guest OS mem-*bypass* Host dan terhubung langsung ke router fisik (misalnya via WiFi atau kabel LAN). Guest akan mendapatkan IP langsung dari jaringan fisik yang sama dengan Host.

   *Alternatif Solusi Jaringan:*
   1. **Kombinasi NAT + Host-Only (Rekomendasi Utama):** Pasang 2 *Network Adapter* pada VM Anda. Adapter 1 menggunakan **NAT** (hanya untuk koneksi internet VM agar bisa *update/install* paket). Adapter 2 menggunakan **Host-Only Adapter** (agar Host OS bisa me-remote VM via SSH/MQTT). Ini adalah cara paling stabil meskipun Anda berpindah-pindah koneksi WiFi/tempat.
   2. **Bridged Adapter Saja (Alternatif demi kemudahan):** Gunakan **Bridged Adapter** saja jika Anda menginginkan kemudahan akses langsung, seolah-olah VM adalah komputer fisik lain di jaringan Anda. **Kelemahannya:** Jika Anda berpindah koneksi jaringan WiFi umum, IP VM akan berubah-ubah dan terkadang router publik memblokir koneksi VM.

   Untuk pengaturan jaringan yang lebih mendetail di versi 7.2, dapat merujuk ke bab Virtual Networking pada dokumentasi resmi [2].

---

## 2. Instalasi OS Ubuntu Server 24.04 LTS di VirtualBox

1. Unduh file ISO Ubuntu Server 24.04 LTS dari situs web resmi Ubuntu [3]. Untuk proses unduh yang lebih cepat, sangat disarankan menggunakan Mirror Lokal Indonesia. Daftar tautan *mirror* lokal (seperti BiznetGio, Datautama, UNAIR, atau PENS) dapat dilihat pada **Lampiran** di akhir modul ini. Panduan lengkap pengaturan Ubuntu 24.04 juga dapat dilihat pada referensi [4].
2. Buka VirtualBox -> Klik **New**.
3. Beri nama (contoh: `Ubuntu_Server_IoT`), pilih tipe **Linux**, versi **Ubuntu (64-bit)**.
4. Alokasikan RAM: **2048 MB** (2 GB) atau lebih.
5. Buat Virtual Hard Disk baru (VDI, Dynamically allocated) sebesar **20 GB**.
6. Masuk ke **Settings** VM -> **Network** -> Ubah *Attached to* ke **Bridged Adapter**.
7. Masuk ke **Settings** VM -> **Storage** -> Masukkan file ISO Ubuntu 24.04 di bagian CD/DVD.
8. Jalankan VM (Start) dan ikuti wizard instalasi:
   - Gunakan pengaturan default untuk layout keyboard.
   - Pada bagian Network, pastikan *interface* mendapatkan IP Address.
   - Jangan lupa mencentang **"Install OpenSSH server"** agar kita bisa remote via SSH dari terminal Windows.
   - Buat username dan password (contoh: user `praktikan`, password `rahasia`).
   - Tunggu proses instalasi selesai dan lakukan *Reboot*.

### 2.5 Mengakses Server Secara Remote Menggunakan SSH (Terminal Windows)
Untuk mempermudah pekerjaan (*copy-paste* perintah), sangat disarankan mengakses Ubuntu Server secara jarak jauh (*remote*) dari Host OS (Windows) menggunakan protokol SSH.
1. Pastikan Ubuntu VM Anda sedang menyala.
2. Di VM Ubuntu, ketik `ip a` dan catat IP Address jaringan Anda (misal: `192.168.1.100`).
3. Pada Windows 10/11, **klien SSH sudah terpasang secara bawaan**. Anda dapat menggunakan *Command Prompt* atau *PowerShell* langsung, sehingga **tidak perlu menginstal aplikasi tambahan seperti PuTTY** (meskipun PuTTY tetap dapat menjadi alternatif).
4. Buka **Command Prompt (CMD)** di Windows Anda.
5. Jalankan perintah SSH berikut:
   ```bash
   ssh nama_user@ip_address_ubuntu
   # Contoh: ssh praktikan@192.168.1.100
   ```
6. Ketik `yes` saat peringatan keamanan (*fingerprint*) pertama kali muncul, lalu masukkan *password* Ubuntu Anda. Anda kini dapat mengontrol Ubuntu sepenuhnya dari terminal Windows.

### 2.6 Instalasi Paket Tambahan Python
Sebagai server IoT, Ubuntu Server 24.04 sudah memiliki Python 3 bawaan. Namun, kita perlu menambahkan beberapa aplikasi pendukung agar kita bisa memanggil perintah `python` secara langsung (tanpa harus mengetik `python3`) dan agar bisa membuat *Virtual Environment* (`venv`).

1. Buka terminal (atau via SSH) dan jalankan pembaruan (*update*) repositori:
   ```bash
   sudo apt update
   ```
2. Instal paket pendukung Python:
   ```bash
   sudo apt install -y python-is-python3 python3-venv
   ```
   *Catatan:* `python-is-python3` akan membuat perintah `python` otomatis merujuk ke Python 3. `python3-venv` digunakan agar kita bisa membuat lingkungan terisolasi untuk menginstal *library* Python di praktikum selanjutnya.

---

## 3. Setup Mosquitto (MQTT Broker)

Untuk dokumentasi lengkap dari broker Mosquitto, dapat dilihat pada referensi [5].

1. Login ke Ubuntu Server.
2. Update repository:
   ```bash
   sudo apt update
   ```
3. Install Mosquitto dan client-nya:
   ```bash
   sudo apt install -y mosquitto mosquitto-clients
   ```
4. Cek apakah service sudah berjalan:
   ```bash
   sudo systemctl status mosquitto
   ```

   (Tekan `q` untuk keluar).

---

## 4. Setup RabbitMQ (AMQP & MQTT Broker)

RabbitMQ adalah broker pesan multi-protokol yang lebih kompleks dibandingkan Mosquitto, mendukung fitur *routing* tingkat lanjut dan antrean pesan (*queuing*). Dokumentasi resmi RabbitMQ tersedia pada referensi [6].

1. Install RabbitMQ Server:
   ```bash
   sudo apt update
   sudo apt install -y rabbitmq-server
   ```
2. Aktifkan plugin Management (Dashboard Web UI):
   ```bash
   sudo rabbitmq-plugins enable rabbitmq_management
   ```
3. (Opsional) Aktifkan plugin MQTT jika ingin RabbitMQ melayani protokol MQTT:
   ```bash
   sudo rabbitmq-plugins enable rabbitmq_mqtt
   ```

---

## 5. Manajemen Service Systemd (Resolusi Konflik Port)

**Masalah:** Mosquitto berjalan di port `1883` (default MQTT). Jika RabbitMQ MQTT Plugin diaktifkan (pada langkah 4.3), RabbitMQ juga akan mencoba menggunakan port `1883`, sehingga akan terjadi tabrakan (*port conflict*) dan salah satu service akan mati.

### Mengatur Service (Enable / Disable)

Kita dapat mengatur prioritas service melalui `systemctl`:

- **Menghentikan service** yang sedang berjalan:
  ```bash
  sudo systemctl stop mosquitto
  ```
- **Disable service** (service tidak akan jalan secara otomatis saat Ubuntu booting/direstart):
  ```bash
  sudo systemctl disable mosquitto
  ```
- **Enable service** (service otomatis jalan) dan menjadikannya default broker:
  ```bash
  sudo systemctl enable --now rabbitmq-server
  ```

### Memindahkan Port (Agar Bisa Berjalan Berdampingan)

Jika kita ingin Mosquitto dan RabbitMQ MQTT berjalan bersamaan, kita bisa menggeser port Mosquitto ke `1884` (port non-standar).

1. Edit konfigurasi Mosquitto:
   ```bash
   sudo nano /etc/mosquitto/conf.d/custom.conf
   ```
2. Tambahkan baris berikut agar *listen* di port 1884:
   ```text
   listener 1884
   allow_anonymous true
   ```
3. Simpan (Ctrl+O, Enter, Ctrl+X) dan restart Mosquitto:
   ```bash
   sudo systemctl restart mosquitto
   ```

Sekarang, RabbitMQ (MQTT) berjalan di port default `1883`, dan Mosquitto berjalan berdampingan di port `1884`.

---

## 6. Menguji Pengiriman Message (MQTT Box / messagebox)

1. Dapatkan IP Ubuntu VM menggunakan perintah `ip a`. (Misal: `192.168.1.100`).
2. Buka aplikasi **MQTTBox** di Windows (atau *compile* dan gunakan aplikasi *messagebox* dari `github.com/mriza/messagebox`).
3. Buat MQTT Client baru.
   - **Protocol:** `mqtt://`
   - **Host:** `192.168.1.100:1884` (Gunakan 1884 jika ngetes Mosquitto, 1883 jika ngetes RabbitMQ).
4. Di bagian **Subscriber**, masukkan Topik: `sensor/suhu` lalu klik *Subscribe*.
5. Di bagian **Publisher**, masukkan Topik: `sensor/suhu`, dan isikan payload/pesan: `{"suhu": 25.5, "status": "normal"}`
6. Klik *Publish*. Pesan akan muncul seketika di panel Subscriber.

---

## 7. Mengatur Mosquitto dan RabbitMQ Tingkat Lanjut

### Keamanan Mosquitto (Username, Password & Host)

1. Buat file kredensial dan user baru (misal user `admin`):

   ```bash
   sudo mosquitto_passwd -c /etc/mosquitto/passwd admin
   ```

   *(Sistem akan meminta Anda mengetikkan password baru untuk user tersebut)*.
2. Edit file konfigurasi agar tidak menerima user anonim:

   ```bash
   sudo nano /etc/mosquitto/conf.d/custom.conf
   ```

   Ubah isinya menjadi:
   ```text
   listener 1884 0.0.0.0
   allow_anonymous false
   password_file /etc/mosquitto/passwd
   ```
3. Restart Mosquitto: `sudo systemctl restart mosquitto`. (Kini MQTT client harus memasukkan username dan password).

### Pengaturan RabbitMQ (User, Queue, Messages, Exchange)

Konsep RabbitMQ memisahkan antara penerima pesan dan antrean:

- **Exchange:** Menerima pesan dari publisher dan memutuskan (*routing*) ke antrean mana pesan tersebut dibuang.
- **Queue:** Tempat penampungan (antrean) pesan sebelum diambil (*consume*) oleh subscriber.
- **Routing Key:** Aturan kriteria penghubung antara Exchange dan Queue.

Membuat user administrator RabbitMQ via terminal:

```bash
sudo rabbitmqctl add_user admin passwordrahasia
sudo rabbitmqctl set_user_tags admin administrator
sudo rabbitmqctl set_permissions -p / admin ".*" ".*" ".*"
```

Anda dapat melakukan manajemen Queue, Exchange, dan Messages secara GUI dengan login ke dashboard web RabbitMQ di **`http://[IP_UBUNTU]:15672`** menggunakan user `admin` dan password `passwordrahasia`.

---

## 8. Menguji Pengiriman Message Menggunakan Python

### Persiapan

Buka terminal/Command Prompt di Windows (Host), install library yang dibutuhkan:

```bash
pip install paho-mqtt pika
```

### Script Publisher MQTT (Python + Mosquitto)

Gunakan library `paho-mqtt` [7] untuk publikasi data. Simpan sebagai `mqtt_publisher.py` dan jalankan.

```python
import paho.mqtt.client as mqtt
import time

broker_address = "192.168.1.100"  # Sesuaikan IP Ubuntu VM
port = 1884

# Callback saat terhubung
def on_connect(client, userdata, flags, rc):
    print("Berhasil terhubung MQTT dengan kode: " + str(rc))

client = mqtt.Client("Python_Publisher")
client.username_pw_set("admin", "passwordrahasia") # Autentikasi Mosquitto
client.on_connect = on_connect

client.connect(broker_address, port=port)
client.loop_start()

for i in range(1, 6):
    pesan = f"Data suhu ke-{i}: 28.5 C"
    client.publish("sensor/suhu", pesan)
    print("Mengirim pesan:", pesan)
    time.sleep(2)

client.loop_stop()
client.disconnect()
```

### Script Subscriber AMQP (Python + RabbitMQ)

Gunakan library `pika` [8] untuk *consume* pesan langsung dari queue di RabbitMQ (protokol AMQP di port 5672). Simpan sebagai `amqp_subscriber.py` dan jalankan.

```python
import pika
import sys, os

def main():
    credentials = pika.PlainCredentials('admin', 'passwordrahasia')
    parameters = pika.ConnectionParameters('192.168.1.100', 5672, '/', credentials)
    connection = pika.BlockingConnection(parameters)
    channel = connection.channel()

    # Mendeklarasikan antrean (Queue)
    channel.queue_declare(queue='hello')

    def callback(ch, method, properties, body):
        print(f" [x] Menerima {body.decode()}")

    channel.basic_consume(queue='hello', on_message_callback=callback, auto_ack=True)

    print(' [*] Menunggu pesan dari RabbitMQ AMQP. Untuk keluar tekan CTRL+C')
    channel.start_consuming()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('Keluar...')
        try:
            sys.exit(0)
        except SystemExit:
            os._exit(0)
```

---

## 9. Referensi

[1] Oracle. (2024). *VirtualBox User Manual*. [Online]. Tersedia: https://docs.oracle.com/en/virtualization/virtualbox/  
[2] Canonical Ltd. (2024). *Ubuntu Server Documentation*. [Online]. Tersedia: https://ubuntu.com/server/docs  
[3] Eclipse Foundation. (2024). *Mosquitto Documentation*. [Online]. Tersedia: https://mosquitto.org/documentation/  
[4] RabbitMQ. (2024). *RabbitMQ Documentation*. [Online]. Tersedia: https://www.rabbitmq.com/documentation.html  
[5] Eclipse Foundation, "Eclipse Paho MQTT Python client library," [Online]. Tersedia: https://eclipse.dev/paho/files/paho.mqtt.python/html/index.html  
[8] Pika Project, "Pika Documentation," [Online]. Tersedia: https://pika.readthedocs.io/  

---  

## Lampiran: Daftar Mirror Lokal Indonesia untuk Ubuntu 24.04

Gunakan salah satu tautan repositori lokal berikut untuk mempercepat proses unduhan ISO Ubuntu Server 24.04 LTS:

- **BiznetGio:** `https://repo.biznetgio.com/ubuntu-releases/24.04/`
- **Datautama (Kartolo):** `https://kartolo.sby.datautama.net.id/ubuntu-cd/24.04/`
- **PENS (Politeknik Elektronika Negeri Surabaya):** `https://kebo.pens.ac.id/ubuntu-releases/24.04/`
- **UNAIR (Universitas Airlangga):** `https://repo.unair.ac.id/ubuntu-releases/24.04/`


---

# Materi Praktikum Pekan 2: Setup Lingkungan dan Message Broker (MQTT)

## 1. Tujuan Praktikum
- Mahasiswa mampu menyiapkan environment Python untuk instalasi library Data Science.
- Mahasiswa mampu menulis *script* generator data tiruan (*dummy data*) untuk keperluan simulasi.
- Mahasiswa memahami cara kerja *publish-subscribe* pada protokol MQTT menggunakan RabbitMQ dan Paho-MQTT.

## 2. Persiapan Alat & Prasyarat
- **Software:** Python 3.8+, VS Code / Jupyter Notebook, RabbitMQ Server.
- **Library Python:** `pandas`, `numpy`, `paho-mqtt`.

## 3. Dasar Teori Singkat
Dalam sistem analitik data IoT, kita seringkali membutuhkan protokol ringan untuk aliran data (*data ingestion*). **MQTT** (Message Queuing Telemetry Transport) sangat populer karena hemat *bandwidth*. MQTT bekerja menggunakan perantara bernama **Broker** (seperti RabbitMQ atau Mosquitto). Perangkat pengirim disebut **Publisher**, dan penerima data disebut **Subscriber**. Pengelompokan jalur data disebut **Topic** (contoh: `kebun/lahan1/suhu`).

## 4. Langkah Kerja (Detail & Anotasi Kode)

### Langkah 1: Persiapan Virtual Environment dan Instalasi Library
Sangat disarankan menggunakan *virtual environment* agar *library* proyek tidak bertabrakan dengan sistem operasi bawaan. Buka terminal dan jalankan:

```bash
# Membuat virtual environment bernama 'env_iot'
python -m venv env_iot

# Mengaktifkan virtual environment (di Ubuntu/Linux)
source env_iot/bin/activate
# (Jika Anda menjalankannya di Windows CMD, gunakan: env_iot\Scripts\activate)

# Menginstal library yang dibutuhkan di dalam environment
pip install pandas numpy paho-mqtt
```

### Langkah 2: Membuat Generator Sensor Dummy
Buat file `generator_sensor.py`. Tujuan skrip ini adalah menciptakan ratusan baris data sensor buatan jika kita belum memiliki sensor fisik.

```python
import pandas as pd
import numpy as np
import datetime

# freq="H" berarti kita men-generate data dengan interval 1 Jam (Hourly)
waktu = pd.date_range(start="2023-10-01", end="2023-10-07", freq="H")

# np.random.normal membuat nilai yang terdistribusi normal (seperti lonceng)
# loc=28.0 (rata-rata suhu 28 C), scale=2.0 (standar deviasi 2 C)
suhu = np.random.normal(loc=28.0, scale=2.0, size=len(waktu))
kelembaban = np.random.normal(loc=65.0, scale=5.0, size=len(waktu))

# Menggabungkan waktu, suhu, dan kelembaban menjadi tabel (DataFrame)
df = pd.DataFrame({"timestamp": waktu, "suhu": suhu, "kelembaban_tanah": kelembaban})

# Menyimpan dataframe ke dalam file CSV agar bisa dianalisis nanti
df.to_csv("data_sensor_dummy.csv", index=False)
print("Data dummy sukses di-generate!")
```

### Langkah 3: Membuat MQTT Publisher (Simulasi Perangkat Edge)
Buat file `mqtt_publish.py`. Skrip ini bertindak sebagai mikrokontroler (misal ESP32) yang mengirim suhu ke server secara berulang.

```python
import paho.mqtt.client as mqtt
import time, random

# Tentukan alamat broker (gunakan 'localhost' jika RabbitMQ berjalan di PC yang sama)
broker_address = "localhost"

# Inisialisasi client pengirim bernama 'SensorSuhu'
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, "SensorSuhu")
client.connect(broker_address, 1883) # 1883 adalah port default MQTT

while True:
    # Membangkitkan nilai suhu acak antara 25.0 hingga 35.0
    suhu_saat_ini = round(random.uniform(25.0, 35.0), 2)
    
    # Mengirim data (publish) ke topik "kebun/sensor/suhu"
    client.publish("kebun/sensor/suhu", str(suhu_saat_ini))
    
    print(f"Data dipublish: {suhu_saat_ini} C")
    time.sleep(2) # Jeda pengiriman 2 detik
```

### Langkah 4: Membuat MQTT Subscriber (Simulasi Server Cloud/Fog)
Buat file `mqtt_subscribe.py`. Skrip ini berjalan di server untuk 'mendengarkan' aliran data masuk.

```python
import paho.mqtt.client as mqtt

# Fungsi callback yang akan dipicu otomatis ketika ada pesan/data masuk
def on_message(client, userdata, message):
    # message.payload berisi data asli berbentuk byte, harus di-decode menjadi teks (utf-8)
    data_teks = str(message.payload.decode("utf-8"))
    topik = message.topic
    print(f"[TERIMA] Topik: {topik} | Payload: {data_teks} C")

# Inisialisasi client penerima bernama 'ServerPusat'
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, "ServerPusat")
client.on_message = on_message # Hubungkan fungsi event listener
client.connect("localhost", 1883)

# Server wajib 'berlangganan' pada topik yang spesifik
client.subscribe("kebun/sensor/suhu")

print("Menunggu aliran data dari sensor...")
# loop_forever membuat skrip terus berjalan tanpa henti untuk mendengarkan pesan
client.loop_forever() 
```

### Langkah 5: Eksekusi Multi-Terminal
1. Buka satu terminal/CMD, jalankan `python mqtt_subscribe.py`.
2. Buka terminal/CMD baru, jalankan `python mqtt_publish.py`.

## 5. Hasil Eksperimen yang Diharapkan
Pada terminal `mqtt_subscribe.py`, Anda akan melihat teks muncul setiap 2 detik yang menangkap persis nilai acak yang dikirimkan oleh terminal `mqtt_publish.py`.

## 6. Troubleshooting
- **Error `ConnectionRefusedError: [Errno 111] Connection refused`:** Broker RabbitMQ (atau Mosquitto) belum berjalan. Pastikan service telah di-start (di Windows cek *Services*, di Linux jalankan `sudo systemctl start rabbitmq-server`).
- **Data tidak muncul di Subscriber:** Pastikan nama topik (`kebun/sensor/suhu`) di skrip publisher sama persis (huruf besar/kecil berpengaruh) dengan di skrip subscriber.

## 7. Tugas Mandiri/Tantangan
Modifikasi skrip publisher agar mengirimkan payload berformat **JSON** (memuat data suhu, kelembaban, dan waktu secara bersamaan), lalu modifikasi subscriber agar mampu melakukan *parsing* JSON tersebut dan mencetaknya secara terstruktur!
