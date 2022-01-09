all:
	clear
	g++ client.cpp -o client -lsqlite3
	g++ server.cpp -o server -lsqlite3
rm:
	rm -f client server
cl:
	clear
	./client 127.0.0.1 2024
db:
	clear
	rm -f database
	g++ database.cpp -o database -lsqlite3


