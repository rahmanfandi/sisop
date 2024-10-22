import threading
import time

# Fungsi tugas1
def tugas1(thread_id):
    for i in range(5):
        print(f"Tugas1 - Thread ID: {thread_id}, Iterasi: {i}")
        time.sleep(1)  # Delay 1 detik

# Fungsi tugas2
def tugas2(thread_id):
    for i in range(5):
        print(f"Tugas2 - Thread ID: {thread_id}, Iterasi: {i}")
        time.sleep(1)  # Delay 1 detik

# Membuat thread
thread1 = threading.Thread(target=tugas1, args=(1,))
thread2 = threading.Thread(target=tugas2, args=(2,))

# Memulai thread
thread1.start()
thread2.start()

# Menunggu kedua thread selesai
thread1.join()
thread2.join()

print("Semua thread selesai.")