socket:
	g++ socket.cpp -o socket -lws2_32
	./socket

client:
	g++ client.cpp -o client -lws2_32
	./client