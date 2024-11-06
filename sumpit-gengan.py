import threading
import time

num_philosophers = 5
chopsticks = [threading.Lock() for _ in range(num_philosophers)]


def philosopher(id):
    left_chopstick = chopsticks[id]
    right_chopstick = chopsticks[(id + 1) % num_philosophers]

    # Mengubah urutan untuk satu filsuf terakhir
    if id == num_philosophers - 1:
        left_chopstick, right_chopstick = right_chopstick, left_chopstick

    while True:
        print(f"Philosopher {id+1} is thinking.")
        time.sleep(1)

        left_chopstick.acquire()
        print(f"Philosopher {id+1} picked up left chopstick.")

        right_chopstick.acquire()
        print(f"Philosopher {id+1} picked up right chopstick.")

        print(f"Philosopher {id+1} is eating.")
        time.sleep(2)

        right_chopstick.release()
        left_chopstick.release()


threads = [
    threading.Thread(target=philosopher, args=(i,)) for i in range(num_philosophers)
]
for t in threads:
    t.start()
