import socket

# Buat socket TCP/IP
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Hubungkan ke server
server_address = ("localhost", 1234)
client_socket.connect(server_address)

# Kirim data ke server
message = "Halo, Server!"
client_socket.sendall(message.encode())

# Terima balasan dari server
response = client_socket.recv(1024)
print(f"Balasan dari server: {response.decode()}")

# Tutup koneksi
client_socket.close()
