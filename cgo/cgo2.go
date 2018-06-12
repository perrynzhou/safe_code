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
import (
	"fmt"
	"sync"
)

type Task struct {
	st *C.student
}

func TaskInit(name string) *Task {
	t := &Task{}
	cname := C.CString(name)
	t.st = C.init(cname)
	return t
}
func (t *Task) Print() {
	C.sprint(t.st)
}
func main() {
	task := TaskInit("perryn")
	ch := make(chan *Task)
	wg := &sync.WaitGroup{}
	wg.Add(1)
	defer wg.Wait()
	go func() {
		defer wg.Done()
		ch <- task
		fmt.Println("....done......")
	}()
	t := <-ch
	t.Print()
}
