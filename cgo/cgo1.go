package main

/*
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct student_s {
	int age;
	char *name;
}student;

student *init(const char *name) {
	student *st = (student *)malloc(sizeof(*st));
	size_t len = strlen(name);
	st->name = (char *)malloc(len+1);
	strncpy(st->name,name,len);
	st->name[len]='\0';
	st->age = 100;
	return st;
}
void sfree(student *st) {
	free(st->name);
	free(st);
}
void init2(student **st,const char *name) {
	printf("....execute begin....");
	for(int i=0;i<4;i++) {
		st[i] = init(name);
	}
	printf("....execute ok....");
}
void sprint2(student **sst){
	for(int i=0;i<4;i++) {
		student *st= sst[i];
	fprintf(stdout,"student addr=%p\n",st);
	fprintf(stdout,"name=%s,age=%d\n",st->name,st->age);

}}
student **batch() {
	student **sst = (student **)malloc(sizeof(student *)*4);
	return sst;
}
void sprint(student *st){
	fprintf(stdout,"student addr=%p\n",st);
	fprintf(stdout,"name=%s,age=%d\n",st->name,st->age);

}
*/
import "C"
import "unsafe"
import "fmt"

func main() {
	var st *C.student
	name := C.CString("perryn")
	defer C.free(unsafe.Pointer(name))
	st = C.init(name)
	C.sprint(st)
	C.sfree(st)
	fmt.Println("--------------2-----------------")
	var st2 **C.student
	st2 = C.batch()
	//C.init2((**C.student)(unsafe.Pointer(st2)), name)
	C.init2(st2, name)
	C.sprint2(st2)
	//C.sprint2((**C.student)(unsafe.Pointer(st2)))
}
