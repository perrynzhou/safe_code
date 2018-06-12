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
void stinit(student *st,char *name,int age) {
	st->name = strdup(name);
	st->age = age;
}
student **create_student(const char *name) {
	student **sst = (student **)calloc(sizeof(student *),4);
	for(int i=0;i<4;i++) {
		student *sts =(student *)calloc(sizeof(student),4);
		for(int j=0;j<4;j++) {
			char buf[64] = {'\0'};
			sprintf((char *)&buf,"%s%d",name,j);
			stinit(&sts[j],(char *)&buf,100+i);
		}
		sst[i] = sts;
	}
	return sst;
}
void student_print(student **st) {
	for(int i=0;i<4;i++) {
		sprint(st[i]);
	}
}
*/
import "C"
import (
	"fmt"
	"unsafe"
)

func main() {
	name := C.CString("perryn")
	stes := C.create_student(name)

	result1 := (*[1 << 30]*C.student)(unsafe.Pointer(stes))[:4:400000]
	fmt.Println(len(result1))

	for index, value := range result1 {
		fmt.Println("----------", index, "------------")
		result2 := (*[1 << 30]C.student)(unsafe.Pointer(value))[:4:300000]
		for _, value2 := range result2 {
			fmt.Println("name:", C.GoString(value2.name), ",age:", C.int(value2.age))
		}
	}
	//C.student_print(stes)
}
