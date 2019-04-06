all:
	rm -rf str_convert_svc.c str_convert_clnt.c str_convert.h
	rpcgen str_convert.x
	rm -rf test_server test_client
	gcc -std=c99 str_convert.h svc.c  str_convert_svc.c  -o test_server -lnsl
	gcc -std=c99  str_convert.h str_convert_clnt.c   cli.c   -o test_client  -lnsl
clean:
	rm -rf str_convert_svc.c str_convert_clnt.c str_convert.h
	rm -rf test_server test_client
