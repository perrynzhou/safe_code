package main

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct T0 {
	int age;
	char *name;
}T0;
typedef struct T1 {
	T0 *tes;
	int size;
}T1;
void print_t(T1 *t) {
	for(int i=0;i<t->size;i++){
		fprintf(stdout,"tes[%d].name=%s,age=%d\n",i,t->tes[i].name,t->tes[i].age);
	}
}
void init(T0 *t,char *base_name,int count) {
	for(int i=0;i<count;i++) {
		char buffer[128] = {'\0'};
		sprintf((char *)&buffer,"%s%d",base_name,i);
		size_t len = strlen((char *)&buffer);
		t[i].name = (char *)calloc(1,len+1);
		memcpy(t[i].name,(char *)&buffer,len);
		t[i].age = 100+i;
	}

}
*/
import "C"
import (
	"unsafe"
)

/*
 在golang层面初始化C中的结构体
*/
func main() {

	tes := C.malloc(C.sizeof_T0 * 2)
	baseName := C.CString("perryn")
	defer C.free(unsafe.Pointer(baseName))
	C.init((*C.T0)(unsafe.Pointer(tes)), baseName, 2)
	t1 := &C.T1{
		size: 2,
		tes:  (*C.T0)(unsafe.Pointer(tes)),
	}
	C.print_t(t1)
	defer C.free(unsafe.Pointer(tes))
}
