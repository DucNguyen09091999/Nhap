build:
	gcc -o client client.c
	gcc -o server server.c
run-server:
	./server 5000 toB.txt
run-client:
	./client 192.168.153.128 5000 fromA.txt
clean:
	rm -rf client server toB.txt
