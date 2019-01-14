/*************************************************************************
  > File Name: replace_interface.cc
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 一  1/14 19:33:43 2019
 ************************************************************************/
#include "replace_interface.h"
#include <functional>
#include <cstdio>
void Penguin::run()
{
  fprintf(stdout, " \n  ->Penguin can run\n");
}
void Penguin::swim()
{
  fprintf(stdout, "\n  ->Penguin can swim\n");
}
void Sparrow::run()
{
  fprintf(stdout, "\n  ->Sparrow can run\n");
}
void Sparrow::fly()
{
  fprintf(stdout, "\n  ->Sparrow can fly\n");
}
Foo::Foo(const char *name, FlyCallback flyCb, RunCallback runCb) : name_(name), flyCb_(flyCb), runCb_(runCb)
{
}
void Foo::actionFoo()
{
  fprintf(stdout, "%s\n", name_.c_str());
  runCb_();
  flyCb_();
}
Bar::Bar(const char *name, SwimCallback swimCb, RunCallback runCb) : name_(name), swimCb_(swimCb), runCb_(runCb) {}
void Bar::actionBar()
{
  fprintf(stdout, "%s\n", name_.c_str());
  runCb_();
  swimCb_();
}
int main(void)
{
  Penguin penguin;
  Sparrow sparrow;
  Foo foo("i'am foo", std::bind(&Sparrow::run, &sparrow), std::bind(&Sparrow::fly, &sparrow));
  Bar bar("i'am bar", std::bind(&Penguin::run, &penguin), std::bind(&Penguin::swim, &penguin));
  foo.actionFoo();
  bar.actionBar();
  return 0;
}