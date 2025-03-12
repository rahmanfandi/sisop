import socket

# Konfigurasi server tujuan
UDP_IP = "127.0.0.1"  # Alamat server
UDP_PORT = 12345  # Port yang digunakan

# Membuat socket UDP
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Buat data besar lebih dari MTU (misalnya 5000 byte)
message = "X" * 5000  # Pesan dengan karakter "X" sebanyak 5000 byte

# Kirim data ke server
sock.sendto(message.encode(), (UDP_IP, UDP_PORT))

print(f"Pesan {len(message)} byte terkirim ke {UDP_IP}:{UDP_PORT}")
sock.close()
