all:
	rm -rf test*
	gcc -std=c99 -g dict.h dict.c tpcc.h tpcc.c tpcc-server.c  -lpthread -o  test_tpcc_server
	gcc -std=c99 -g tpcc.h tpcc.c tpcc-client.c -o test_tpc_client
