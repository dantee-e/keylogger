import socket

# Create a socket and bind it to the local IP and port
s = socket.socket()
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

ip = socket.gethostbyname(socket.gethostname()) #input your site here
port = 88
s.bind((ip, port))
s.listen(5)


print(f"Server started at {ip}:{port}")


def gravar_char(client, addr, charRecebido):
	with open(str(addr[0])+'.txt', 'a+') as file:
		print(charRecebido)
		file.write(str(charRecebido))


while True:
    # Accept client connection
	client, addr = s.accept()
	reqCompleto = client.recv(1024)
	header, body = str(reqCompleto).split('\\r\\n\\r\\n')

	gravar_char(client, addr, body.removesuffix('\''))
	
    # Send HTTP headers and content
	response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n"
	# response.encode('utf-8')
	client.sendall(response.encode('utf-8'))
	
    # Close the connection
	client.close()
