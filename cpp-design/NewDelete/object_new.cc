/*************************************************************************
  > File Name: object_new.cc
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 三  1/23 10:32:38 2019
 ************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <new>
using namespace std;
class Object
{
  int fd_;

public:
  Object() : fd_(0)
  {
    fprintf(stdout, "Object()\n");
  }
  Object(const char *path) : fd_(open(path, O_RDWR))
  {
    fprintf(stdout, "Object(const char *path)\n");
  }
  ~Object()
  {
    if (fd_ != -1)
    {
      close(fd_);
    }
    fprintf(stdout, "~Object()\n");
  }
};
int main(int argc, char *argv[])
{
  int allocSize = 0;
  int flag = 0;
  if (argc > 2)
  {
    allocSize = atoi(argv[1]);
    flag = atoi(argv[2]);
  }
  Object *obj = nullptr;

  if (allocSize > 0 && flag == 0)
  {
    fprintf(stdout, "---normal new throw exception---\n");
    obj = new Object[allocSize];
  }
  else if (allocSize > 0 && flag == 1)
  {
    fprintf(stdout, "---new not throw exception,but return nullptr---\n");
    obj = new (nothrow) Object[allocSize];
    if (obj != nullptr)
    {
      delete[] obj;
    }
    else
    {
      fprintf(stdout, " object ptr is nil\n");
    }
  }
  else
  {
    fprintf(stdout, "---placement new,reused alloced memory---\n");
    char *ptr = new char[32];
    obj = new (ptr) Object; //placement new
    obj->~Object(); //placement delete
    delete ptr;
  }
}