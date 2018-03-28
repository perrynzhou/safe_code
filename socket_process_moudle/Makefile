all:
	rm -rf test*
	gcc -g -std=c99 multi_thread_per_client.c -lpthread -o test_mutil_srv
	gcc -g -std=c99 client.c -lpthread -o test_cli
	gcc -g -std=c99 process_per_socket.c -o test_srv
