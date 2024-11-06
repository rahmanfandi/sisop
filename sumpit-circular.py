import threading
import time


class Philosopher(threading.Thread):
    def __init__(self, name, left_chopstick, right_chopstick):
        threading.Thread.__init__(self)
        self.name = name
        self.left_chopstick = left_chopstick
        self.right_chopstick = right_chopstick

    def run(self):
        while True:
            # Berpikir sejenak
            print(f"{self.name} is thinking.")
            time.sleep(1)

            # Mencoba mengambil sumpit kiri
            print(f"{self.name} tries to pick up left chopstick.")
            self.left_chopstick.acquire()
            print(f"{self.name} picked up left chopstick.")

            # Mencoba mengambil sumpit kanan
            print(f"{self.name} tries to pick up right chopstick.")
            self.right_chopstick.acquire()
            print(f"{self.name} picked up right chopstick.")

            # Makan sejenak
            print(f"{self.name} is eating.")
            time.sleep(2)

            # Meletakkan sumpit kanan
            print(f"{self.name} puts down right chopstick.")
            self.right_chopstick.release()

            # Meletakkan sumpit kiri
            print(f"{self.name} puts down left chopstick.")
            self.left_chopstick.release()


# Jumlah filsuf
num_philosophers = 5

# Membuat sumpit (lock) untuk masing-masing filsuf
chopsticks = [threading.Lock() for _ in range(num_philosophers)]

# Membuat filsuf dan memberikan sumpit ke setiap filsuf
philosophers = [
    Philosopher(
        f"Philosopher {i+1}", chopsticks[i], chopsticks[(i + 1) % num_philosophers]
    )
    for i in range(num_philosophers)
]

# Memulai thread untuk setiap filsuf
for philosopher in philosophers:
    philosopher.start()
