/*************************************************************************
  > File Name: base.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 一  1/21 10:23:01 2019
 ************************************************************************/

#ifndef _BASE_H
#define _BASE_H
class Object
{
public:
  Object() {}
  ~Object() {}
  //overloading add function within class
  int add(int a);
  void add(int a, int b);
  int add(int a, int b, int c);
};

#endif
