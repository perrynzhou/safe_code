/*************************************************************************
  > File Name: lamda.cc
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 一  2/18 14:01:13 2019
 ************************************************************************/

#include <iostream>
using namespace std;
void PassByRef(int count)
{
  cout << "...start PassByRef..." << endl;
  int id = 100;
  cout << "...start id address:" << static_cast<void *>(&id) << ",id=" << id << endl;

  auto add = [&id]() mutable {
    id++;
    cout << "id address:" << static_cast<void *>(&id) << ",id value:" << id << endl;
  };
  cout << "-----start lambda----" << endl;
  for (int i = 1; i < count; i++)
  {
    add();
  }
  cout << "-----end lambda----" << endl;
  cout << "id:" << id << endl;
}
void PassByValue(int count)
{
  cout << "\n\n...start PassByValue..." << endl;
  int id = 10;
  cout << "...start id address:" << static_cast<void *>(&id) << ",id=" << id << endl;

  auto add = [id]() mutable {
    id++;
    cout << "id address:" << static_cast<void *>(&id) << ",id value:" << id << endl;
  };
  cout << "-----start lambda----" << endl;
  for (int i = 1; i < count; i++)
  {
    add();
  }
  cout << "-----end lambda----" << endl;
  cout << "id:" << id << endl;
}
int main(void)
{
 
  PassByRef(5);
  PassByValue(5);
  return 0;
}