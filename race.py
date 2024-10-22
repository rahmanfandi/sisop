import threading

# Variabel bersama
counter = 0


# Fungsi yang akan dijalankan oleh thread
def increment():
    global counter
    for _ in range(100000):
        counter += 1


# Membuat thread
thread1 = threading.Thread(target=increment)
thread2 = threading.Thread(target=increment)

# Menjalankan thread
thread1.start()
thread2.start()

# Menunggu kedua thread selesai
thread1.join()
thread2.join()

# Menampilkan hasil
print("Counter:", counter)  # Hasil mungkin tidak selalu 200000
