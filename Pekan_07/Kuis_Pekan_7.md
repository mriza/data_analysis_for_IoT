# Kuis Pekan 7: Edge Analytics dan Komputasi Resource-Constrained

Jawablah pertanyaan-pertanyaan berikut dengan jelas, ringkas, dan berdasarkan penalaran analitik Anda!

1. Paradigma tradisional IoT sering mengandalkan *Cloud Analytics* (memproses semuanya di server terpusat). Analisislah mengapa mengirimkan aliran video mentah 24 jam nonstop dari kamera pemantau tanaman ke *Cloud* adalah keputusan yang sangat buruk secara ekonomis dan teknis, serta bagaimana *Edge Analytics* membalikkan paradigma tersebut!

2. Anda diminta membangun sistem pemantauan tingkat keasaman tanah menggunakan mikrokontroler ESP32 di tengah sawah. Jika memori *SRAM* ESP32 hanya berkisar 520 KB, mengapa Anda sama sekali tidak bisa (dan tidak boleh) meniru gaya bahasa pemrograman Python (seperti menggunakan `import pandas` dan mengumpulkan seluruh rekaman data sebulan ke dalam sebuah array) di lingkungan tersebut?

3. Sebagai solusi atas keterbatasan memori di perangkat *Edge*, *Data Engineer* menggunakan teknik pemrosesan data beraliran (*streaming/online processing*) alih-alih pemrosesan tumpukan (*batch/array*). Jelaskan bagaimana teknik *streaming* memungkinkan ESP32 menghitung nilai rata-rata dari 10.000 titik data suhu tanpa harus pernah menyimpan ke-10.000 angka tersebut di memori secara bersamaan!

4. Algoritma *Tiny Machine Learning (TinyML)* memungkinkan model deteksi *Deep Learning* yang aslinya berukuran puluhan Megabyte ditekan menjadi hanya 30 Kilobyte. Salah satu teknik utamanya adalah **Quantization**. Secara konseptual, bagaimana proses penurunan presisi tipe data (dari *Float-32 bit* menjadi *Integer-8 bit*) pada bobot algoritma dapat mengecilkan ukuran *file* tanpa merusak logika kecerdasan utamanya secara total?

5. Selain *Quantization*, teknik lain dalam *TinyML* adalah **Pruning** (pemangkasan). Jika Anda mengibaratkan *Neural Network* sebagai sebuah jaringan jalan raya yang sangat padat, analisis logika di balik proses *Pruning* ini dan mengapa "menghapus jalur yang jarang dilewati" akan mempercepat komputasi *Edge Device*!

6. Pada studi kasus peternakan, mikrofon cerdas (*Edge Node*) dikonfigurasi untuk mendeteksi suara batuk babi akibat flu (*Swine Flu*). Mengapa arsitektur di mana perangkat terus-menerus "*offline*" dari WiFi dan hanya menyalakan radio pengirim saat batuk terdeteksi, menghasilkan penghematan konsumsi baterai yang jauh lebih masif dibandingkan arsitektur terhubung-terus (*always-on*)?

7. Jika kemampuan *Edge Analytics* saat ini sudah sangat luar biasa (bisa menjalankan AI, merangkum data, mengontrol aktuator secara instan), kemukakan argumen analitis Anda mengapa infrastruktur *Cloud Computing* berbiaya mahal tetap mutlak dibutuhkan dan tidak mungkin digantikan sepenuhnya oleh *Edge*!

8. Tinjau dua jenis data dari sebuah kebun tomat pintar: (A) Data suhu tanah dan (B) Foto rupa daun beresolusi tinggi. Jika sistem Anda terbatas oleh kuota internet SIM Card seluler, rancanglah skenario pembagian peran di mana Anda mendelegasikan pemrosesan Data A dan Data B ke lapisan arsitektur yang paling tepat (*Edge* atau *Cloud*)!

9. Dalam pengembangan model ML biasa, *Data Scientist* hanya fokus mengejar angka akurasi setinggi mungkin (misal: 99%). Namun saat model tersebut ingin di-deploy ke perangkat *Resource-Constrained* seperti mikrokontroler murah, mengapa metrik akurasi tinggi tersebut terpaksa "dikorbankan" hingga turun menjadi sekitar 85-90%, dan metrik fisik/perangkat keras apa yang kini menjadi tolok ukur kesuksesan yang sama pentingnya?

10. Anda merancang sistem alarm kebakaran hutan berbasis detektor asap IoT. Asumsikan algoritma deteksi asap tersebut di-*deploy* murni di *Cloud* (Sensor $\rightarrow$ Internet $\rightarrow$ Cloud $\rightarrow$ Aktuator Sirine). Analisislah skenario maut (terkait latensi dan redundansi *network*) yang dapat terjadi pada desain ini, dan buktikan mengapa meletakkan *rules engine* alarm di tingkat *Edge* adalah pertaruhan hidup-mati bagi warga sekitar!
