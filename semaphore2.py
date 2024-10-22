import threading
import time

# Membuat semaphore yang mengizinkan maksimal 2 thread untuk mengakses sumber daya
semaphore = threading.Semaphore(2)


# Fungsi untuk diakses oleh thread
def akses_sumber_daya(nama):
    print(f"{nama} menunggu untuk mengakses sumber daya...")
    with semaphore:  # Thread mengunci semaphore
        print(f"{nama} mengakses sumber daya.")
        time.sleep(2)  # Simulasi proses menggunakan sumber daya
    print(f"{nama} selesai menggunakan sumber daya.")


# Fungsi untuk diakses oleh thread
def akses_sumber_daya2(nama):
    print(f"{nama} menunggu untuk mengakses sumber daya...")
    with semaphore:  # Thread mengunci semaphore
        print(f"{nama} mengakses sumber daya.")
        time.sleep(5)  # Simulasi proses menggunakan sumber daya
    print(f"{nama} selesai menggunakan sumber daya.")


# Membuat 4 thread secara manual
t1 = threading.Thread(target=akses_sumber_daya, args=("Thread-1",))
t2 = threading.Thread(target=akses_sumber_daya2, args=("Thread-2",))
t3 = threading.Thread(target=akses_sumber_daya, args=("Thread-3",))
t4 = threading.Thread(target=akses_sumber_daya2, args=("Thread-4",))

# Memulai thread
t1.start()
t2.start()
t3.start()
t4.start()

# Menunggu semua thread selesai
t1.join()
t2.join()
t3.join()
t4.join()
