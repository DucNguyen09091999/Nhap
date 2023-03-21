CC = gcc
CFLAGS = -Wall -Wextra
DEPSRC = dll/dll.c mac-list/mac-list.c routing-table/routing-table.c sync/sync.c

.PHONY: dll mac-list routing-table sync shm_ip

default: dll mac-list routing-table sync shm_ip
	$(CC) $(CFLAGS) client.c shm_ip.c $(DEPSRC) -o client -lrt
	$(CC) $(CFLAGS) server.c shm_ip.c $(DEPSRC) -o server -lrt

client: dll mac-list routing-table sync shm_ip
	$(CC) $(CFLAGS) client.c shm_ip.c $(DEPSRC) -o client -lrt

server: dll mac-list routing-table sync shm_ip
	$(CC) $(CFLAGS) server.c shm_ip.c $(DEPSRC) -o server -lrt

dll:
	$(CC) $(CFLAGS) -c dll/dll.c -o dll/dll.o
	# $(CC) $(CFLAGS) dll/main.c dll/dll.c -o dll/main

mac-list: dll shm_ip
	$(CC) $(CFLAGS) -c mac-list/mac-list.c -o mac-list/mac-list.o

routing-table: dll
	$(CC) $(CFLAGS) -c routing-table/routing-table.c -o  routing-table/routing-table.o

sync: dll mac-list routing-table shm_ip
	$(CC) $(CFLAGS) -c sync/sync.c -o sync/sync.o

shm_ip:
	$(CC) $(CFLAGS) -c shm_ip.c -o shm_ip.o

clean:
	rm -f client server dll/dll.o mac-list/mac-list.o routing-table/routing-table.o shm_ip.o