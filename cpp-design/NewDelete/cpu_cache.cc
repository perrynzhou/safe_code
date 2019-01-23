/*************************************************************************
  > File Name: cache.cc
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 三  1/23 11:12:51 2019
 ************************************************************************/

#include <stdlib.h>
using namespace std;
class Cache1
{
  //two cpu cache->memory 1
  int id_;
  int count_;
  char buf[4096];
};
class Cache2
{
  // one cpu cache->memory 2
  int id_;
  char buf[4096];
  int count_;
};
int main(int argc, char *argv[])
{
  int num = atoi(argv[1]);
  int flag = atoi(argv[2]);
  if (flag == 1)
  {
    for (int i = 0; i < num; i++)
    {
      Cache1 c1;
    }
  }
  else if(flag ==2)
  {
    for (int i = 0; i < num; i++)
    {
      Cache2 c2;
    }
  }
  return 0;
}
