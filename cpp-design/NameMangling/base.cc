/*************************************************************************
  > File Name: base.cc
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 一  1/21 10:23:10 2019
 ************************************************************************/

#include "base.h"
#include <iostream>
using namespace std;
int Object::add(int a)
{
  cout << "int Object::add(int)" << endl;
  return 0;
}
void Object::add(int a, int b)
{
  cout << "void Object::add(int,int)" << endl;
}
int Object::add(int a, int b, int c)
{
  cout << "int Object::add(int,int,int)" << endl;
  return 0;
}
int main(void)
{
  /*
  Object obj;
  obj.add(1);
  obj.add(1, 2);
  obj.add(1, 2, 3);
  */
  return 0;
}