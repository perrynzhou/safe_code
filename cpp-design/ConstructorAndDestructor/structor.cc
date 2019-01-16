/*************************************************************************
  > File Name: structor.cc
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 三  1/16 11:03:07 2019
 ************************************************************************/

#include <iostream>
using namespace std;
namespace structor
{
class EntryV1
{
  std::string name_;
  int id_;

public:
  EntryV1(string &name, int id) : name_(name), id_(id) {}
};
class EntryV2
{
  std::string name_;
  int id_;

public:
  EntryV2(string &name, int id) 
  {
    name_ = name;
    id_ = id;
  }
};
string RandString(int n)
{
  std::string s;
  s.reserve(n);
  const char *src = "abcdefgpiuerwerlwe1232342353454546450";
  for (int i = 0; i < n; i++)
  {
    s[i] = (rand() + i) % n;
  }
  return s;
}
} // namespace structor
int main(int argc, char *argv[])
{
  if (argc == 3)
  {
    int n = atoi(argv[1]);
    int size = atoi(argv[2]);
    if (n == 1)
    {
      fprintf(stdout, "...initialization structor::EntryV1 v1(s, rand())...\n");
      string s = structor::RandString(18);
      for (int i = 0; i < size; i++)
      {

        structor::EntryV1 v1(s, rand());
      }
    }
    else
    {
      fprintf(stdout, "...assignment structor::EntryV2 v2(s, rand())...\n");

      string s = structor::RandString(18);
      for (int i = 0; i < size; i++)
      {
        structor::EntryV2 v2(s, rand());
      }
    }
/*
perrynzhou@CentOS7:~/Source/perrynzhou-source/safe_code/cpp-design/ConstructorAndDestructor$ time ./test_s 0  9999999
...structor::EntryV2 v2(s, rand())...
real    0m0.605s
user    0m0.604s
sys     0m0.001s
perrynzhou@CentOS7:~/Source/perrynzhou-source/safe_code/cpp-design/ConstructorAndDestructor$ time ./test_s 1  9999999 
...structor::EntryV1 v1(s, rand())...
real    0m0.338s
user    0m0.336s
sys     0m0.001s
*/
  }
}