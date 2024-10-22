import threading
import time

# Membuat semaphore dengan kapasitas 2
semaphore = threading.Semaphore(2)


# Fungsi yang akan dijalankan oleh thread
def access_resource(thread_id):
    print(f"Thread {thread_id} menunggu untuk mengakses sumber daya.")
    semaphore.acquire()  # Mengambil semaphore
    print(f"Thread {thread_id} mengakses sumber daya.")
    time.sleep(2)  # Simulasi pemrosesan
    print(f"Thread {thread_id} selesai menggunakan sumber daya.")
    semaphore.release()  # Melepaskan semaphore


# Membuat dan menjalankan beberapa thread
threads = []
for i in range(5):
    thread = threading.Thread(target=access_resource, args=(i,))
    threads.append(thread)
    thread.start()

# Menunggu semua thread selesai
for thread in threads:
    thread.join()
