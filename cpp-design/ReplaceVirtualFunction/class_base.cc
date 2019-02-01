/*************************************************************************
  > File Name: class_base.cc
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 五  2/ 1 21:01:51 2019
 ************************************************************************/
#include <iostream>
class Base {
  public: 
  Base(){std::cout<<"Base()"<<std::endl;}
  virtual void print(){}
  ~Base(){std::cout<<"~Base()"<<std::endl;}
  char m_value;
};
int main(void) {
  std::cout<<"start main"<<std::endl;
  Base base;
  std::cout<<"Base contains virtual table size:"<<sizeof(base)<<std::endl;
  return 0;
}
