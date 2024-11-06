import threading
import time

# Jumlah filsuf
num_philosophers = 5

# Membuat sumpit sebagai array lock
chopsticks = [threading.Lock() for _ in range(num_philosophers)]


def philosopher(id):
    left_chopstick = chopsticks[id]
    right_chopstick = chopsticks[(id + 1) % num_philosophers]

    while True:
        # Filsuf berpikir
        print(f"Philosopher {id+1} is thinking.")
        time.sleep(1)

        # Mencoba mengambil sumpit kiri terlebih dahulu
        while True:
            left_chopstick.acquire()
            print(f"Philosopher {id+1} picked up left chopstick.")

            # Mengecek apakah sumpit kanan tersedia
            if not right_chopstick.locked():
                right_chopstick.acquire()
                print(f"Philosopher {id+1} picked up right chopstick.")
                break
            else:
                # Jika sumpit kanan tidak tersedia, lepaskan sumpit kiri
                print(f"Philosopher {id+1} puts down left chopstick to try again.")
                left_chopstick.release()
                time.sleep(0.5)  # Menunggu sejenak sebelum mencoba lagi

        # Filsuf makan
        print(f"Philosopher {id+1} is eating.")
        time.sleep(2)

        # Melepaskan sumpit kanan dan kiri setelah makan
        print(f"Philosopher {id+1} puts down right chopstick.")
        right_chopstick.release()

        print(f"Philosopher {id+1} puts down left chopstick.")
        left_chopstick.release()


# Membuat dan memulai thread untuk setiap filsuf
threads = [
    threading.Thread(target=philosopher, args=(i,)) for i in range(num_philosophers)
]
for t in threads:
    t.start()
