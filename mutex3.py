import threading
import time

# Sumber daya bersama (saldo akun)
saldo = 1000
mutex = threading.Lock()  # Membuat mutex


# Fungsi untuk menarik uang dari akun
def tarik_uang(jumlah):
    global saldo
    print(f"{threading.current_thread().name} mencoba menarik: {jumlah}")
    time.sleep(0.1)  # Simulasi waktu yang diperlukan untuk memproses
    mutex.acquire()  # Mengunci mutex sebelum mengakses saldo
    try:
        if saldo >= jumlah:
            # Menyimpan nilai saldo sebelum melakukan pengurangan
            temp = saldo
            # Menunggu sejenak untuk mensimulasikan proses
            time.sleep(0.1)
            saldo = temp - jumlah
            print(f"Penarikan berhasil: {jumlah}, Sisa saldo: {saldo}")
        else:
            print(f"Tidak cukup saldo untuk menarik: {jumlah}")
    finally:
        mutex.release()  # Melepaskan mutex setelah selesai


# Membuat dua thread untuk melakukan penarikan
t1 = threading.Thread(target=tarik_uang, args=(800,), name="Thread-1")
t2 = threading.Thread(target=tarik_uang, args=(500,), name="Thread-2")

# Memulai thread
t1.start()
t2.start()

# Menunggu kedua thread selesai
t1.join()
t2.join()

# Menampilkan saldo akhir
print(f"Saldo akhir: {saldo}")
