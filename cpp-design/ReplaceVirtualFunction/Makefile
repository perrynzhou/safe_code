all:
	rm -rf test*
	g++ -std=gnu++11 replace_virtual_function.h replace_virtual_function.cc -lpthread -o test
	g++ -std=gnu++11 replace_interface.h replace_interface.cc -o test2
	g++ -std=gnu++11  class_base.cc -o test_base