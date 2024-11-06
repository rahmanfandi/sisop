import threading
import time


class Philosopher(threading.Thread):
    def __init__(self, name, left_chopstick, right_chopstick):
        threading.Thread.__init__(self)
        self.name = name
        self.left_chopstick = left_chopstick
        self.right_chopstick = right_chopstick
        self.eating_count = 0

    def run(self):
        while True:
            # Berpikir sejenak
            print(f"{self.name} is thinking.")
            time.sleep(1)

            # Mengambil sumpit kiri
            if self.left_chopstick.acquire(timeout=1):  # Menunggu selama 1 detik
                print(f"{self.name} picked up left chopstick.")

                # Mengambil sumpit kanan
                if self.right_chopstick.acquire(timeout=1):  # Menunggu selama 1 detik
                    print(f"{self.name} picked up right chopstick.")

                    # Makan
                    print(f"{self.name} is eating.")
                    self.eating_count += 1
                    time.sleep(2)

                    # Meletakkan sumpit kanan
                    print(f"{self.name} puts down right chopstick.")
                    self.right_chopstick.release()

                # Meletakkan sumpit kiri
                print(f"{self.name} puts down left chopstick.")
                self.left_chopstick.release()

            # Cek apakah filsuf sudah makan cukup sering (untuk contoh starvation)
            if self.eating_count >= 2:
                break


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
