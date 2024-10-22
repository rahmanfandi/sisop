import threading

lock1 = threading.Lock()
lock2 = threading.Lock()


def thread1():
    if lock1.acquire(timeout=2):  # Beri batas waktu 2 detik
        print("Thread 1: lock1 acquired")
        if lock2.acquire(timeout=2):
            print("Thread 1: lock2 acquired")
            lock2.release()
        lock1.release()
    else:
        print("Thread 1: could not acquire lock1")


def thread2():
    if lock2.acquire(timeout=2):  # Beri batas waktu 2 detik
        print("Thread 2: lock2 acquired")
        if lock1.acquire(timeout=2):
            print("Thread 2: lock1 acquired")
            lock1.release()
        lock2.release()
    else:
        print("Thread 2: could not acquire lock2")


t1 = threading.Thread(target=thread1)
t2 = threading.Thread(target=thread2)

t1.start()
t2.start()

t1.join()
t2.join()
