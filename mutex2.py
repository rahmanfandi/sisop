import threading

# Variabel global untuk saldo
balance = 500
lock = threading.Lock()  # Membuat lock untuk melindungi saldo


# Fungsi untuk menarik uang dari saldo
def withdraw_200():
    global balance
    lock.acquire()  # Mengunci akses
    amount = 200
    if balance >= amount:
        balance -= amount
        print(f"Withdraw {amount} successful. Current balance: {balance}")
    else:
        print(
            f"Withdraw {amount} failed. Insufficient funds. Current balance: {balance}"
        )
    lock.release()  # Melepaskan kunci


def withdraw_300():
    global balance
    lock.acquire()  # Mengunci akses
    amount = 400
    if balance >= amount:
        balance -= amount
        print(f"Withdraw {amount} successful. Current balance: {balance}")
    else:
        print(
            f"Withdraw {amount} failed. Insufficient funds. Current balance: {balance}"
        )
    lock.release()  # Melepaskan kunci


# Membuat thread untuk fungsi penarikan
thread1 = threading.Thread(target=withdraw_200)
thread2 = threading.Thread(target=withdraw_300)

# Menjalankan kedua thread secara manual
thread1.start()
thread2.start()

# Menunggu kedua thread selesai
thread1.join()
thread2.join()

# Menampilkan saldo akhir
print("Final balance:", balance)
