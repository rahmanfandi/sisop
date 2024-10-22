import threading

# Membuat dua lock
lock1 = threading.Lock()
lock2 = threading.Lock()


# Fungsi pertama mencoba mendapatkan lock1 lalu lock2
def thread1():
    lock1.acquire()
    print("Thread 1: lock1 acquired")
    # Simulasi kerja
    import time

    time.sleep(1)
    print("Thread 1: waiting for lock2")
    lock2.acquire()
    print("Thread 1: lock2 acquired")
    lock2.release()
    lock1.release()


# Fungsi kedua mencoba mendapatkan lock2 lalu lock1
def thread2():
    lock2.acquire()
    print("Thread 2: lock2 acquired")
    # Simulasi kerja
    import time

    time.sleep(1)
    print("Thread 2: waiting for lock1")
    lock1.acquire()
    print("Thread 2: lock1 acquired")
    lock1.release()
    lock2.release()


# Membuat dua thread
t1 = threading.Thread(target=thread1)
t2 = threading.Thread(target=thread2)

# Menjalankan thread
t1.start()
t2.start()

# Menunggu kedua thread selesai
t1.join()
t2.join()
