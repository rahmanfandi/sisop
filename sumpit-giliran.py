import threading
import time

num_philosophers = 5
chopsticks = [threading.Lock() for _ in range(num_philosophers)]
turn = 0
turn_lock = threading.Lock()


def philosopher(id):
    global turn
    left_chopstick = chopsticks[id]
    right_chopstick = chopsticks[(id + 1) % num_philosophers]

    while True:
        with turn_lock:
            if turn != id:
                continue
            print(f"Philosopher {id+1}'s turn to eat.")

        left_chopstick.acquire()
        print(f"Philosopher {id+1} picked up left chopstick.")

        right_chopstick.acquire()
        print(f"Philosopher {id+1} picked up right chopstick.")

        print(f"Philosopher {id+1} is eating.")
        time.sleep(2)

        right_chopstick.release()
        left_chopstick.release()

        with turn_lock:
            turn = (turn + 1) % num_philosophers


threads = [
    threading.Thread(target=philosopher, args=(i,)) for i in range(num_philosophers)
]
for t in threads:
    t.start()
