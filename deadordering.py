import threading

lock1 = threading.Lock()
lock2 = threading.Lock()


def thread1():
    lock1.acquire()
    print("Thread 1: lock1 acquired")
    lock2.acquire()
    print("Thread 1: lock2 acquired")
    lock2.release()
    lock1.release()


def thread2():
    lock1.acquire()  # Mengambil lock1 dulu
    print("Thread 2: lock1 acquired")
    lock2.acquire()
    print("Thread 2: lock2 acquired")
    lock2.release()
    lock1.release()


t1 = threading.Thread(target=thread1)
t2 = threading.Thread(target=thread2)

t1.start()
t2.start()

t1.join()
t2.join()
