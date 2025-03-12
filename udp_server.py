import socket

# Konfigurasi server
UDP_IP = "0.0.0.0"  # Menerima dari semua alamat
UDP_PORT = 12345  # Port yang digunakan

# Membuat socket UDP
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Server UDP berjalan di {UDP_IP}:{UDP_PORT}...")

while True:
    data, addr = sock.recvfrom(65535)  # Menerima data dengan ukuran besar
    print(f"Diterima dari {addr}: {len(data)} byte")
