import socket

# Buat socket TCP/IP
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind ke alamat dan port
server_address = ("localhost", 1234)
server_socket.bind(server_address)

# Listen (maksimal 1 koneksi dalam antrian)
server_socket.listen(1)
print("Menunggu koneksi...")

# Terima koneksi
conn, client_address = server_socket.accept()
print(f"Koneksi dari {client_address}")

# Terima data dari client
data = conn.recv(1024)
print(f"Pesan dari client: {data.decode()}")

# Kirim balasan ke client
conn.sendall(b"Pesan diterima!")

# Tutup koneksi
conn.close()
server_socket.close()
