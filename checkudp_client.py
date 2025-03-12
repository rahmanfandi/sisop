import socket
import struct


def checksum(data):
    """Menghitung checksum UDP berbasis 16-bit ones' complement"""
    if len(data) % 2 == 1:
        data += b"\x00"

    sum_ = 0
    for i in range(0, len(data), 2):
        word = (data[i] << 8) + data[i + 1]
        sum_ += word
        sum_ = (sum_ & 0xFFFF) + (sum_ >> 16)

    return ~sum_ & 0xFFFF


# Konfigurasi server tujuan
UDP_IP = "127.0.0.1"
UDP_PORT = 12345

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Data yang dikirim
message = "Halo, UDP dengan Checksum!"
message_bytes = message.encode()

# Hitung checksum
chk = checksum(message_bytes)

# Gabungkan checksum (2 byte) + data
packet = struct.pack("!H", chk) + message_bytes

# Kirim paket
sock.sendto(packet, (UDP_IP, UDP_PORT))

print(f"Pesan terkirim dengan checksum: {chk}")
sock.close()
