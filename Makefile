all:
	gcc -Wall -g cksockets.c ckutils.c main_server.c -o run_server
	gcc -Wall -g cksockets.c ckutils.c main_client.c -o run_client