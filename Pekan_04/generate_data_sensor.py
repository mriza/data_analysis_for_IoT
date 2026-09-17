import pandas as pd
import numpy as np

def generate_data_sensor():
    print("Mulai menghasilkan data sensor time-series dummy...")
    
    # Mengatur seed agar data acak selalu sama
    np.random.seed(42)

    # Membuat rentang waktu (1 pekan, per jam)
    waktu = pd.date_range(start="2023-10-01", end="2023-10-07 23:00:00", freq="h")

    # Membuat data suhu acak (misalnya rata-rata 28 derajat)
    suhu = np.random.normal(loc=28, scale=3, size=len(waktu))

    # Menyimpan ke dalam DataFrame
    df_sensor = pd.DataFrame({
        'timestamp': waktu,
        'suhu': suhu
    })

    # Menyimpan DataFrame ke file CSV
    output_filename = 'data_sensor_dummy.csv'
    df_sensor.to_csv(output_filename, index=False)
    print(f"Selesai! File '{output_filename}' berhasil dibuat dengan {len(df_sensor)} baris data.")

if __name__ == "__main__":
    generate_data_sensor()
