import socket


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


# Konfigurasi server
UDP_IP = "0.0.0.0"
UDP_PORT = 12345

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Server UDP berjalan di {UDP_IP}:{UDP_PORT}...")

while True:
    data, addr = sock.recvfrom(1024)

    # Ambil checksum yang dikirim (2 byte pertama)
    received_checksum = struct.unpack("!H", data[:2])[0]
    message = data[2:]  # Sisanya adalah pesan

    # Hitung ulang checksum
    calculated_checksum = checksum(message)

    # Periksa apakah checksum cocok
    if received_checksum == calculated_checksum:
        print(f"Diterima dari {addr}: {message.decode()} (Checksum VALID)")
    else:
        print(f"Diterima dari {addr}: (Checksum ERROR!)")
