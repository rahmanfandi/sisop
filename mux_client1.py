import socket

# Konfigurasi server tujuan
UDP_IP = "127.0.0.1"  # Alamat server
UDP_PORT = 12345  # Port server

# Membuat socket UDP
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Kirim pesan
message = "Halo dari Client 1!"
sock.sendto(message.encode(), (UDP_IP, UDP_PORT))

print(f"Client 1 mengirim: {message}")
sock.close()
