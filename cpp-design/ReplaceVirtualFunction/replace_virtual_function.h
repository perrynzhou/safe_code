/*************************************************************************
  > File Name: replace_virtual_function.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 一  1/14 16:39:59 2019
 ************************************************************************/

#ifndef _REPLACE_VIRTUAL_FUNCTION_H
#define _REPLACE_VIRTUAL_FUNCTION_H
#include <pthread.h>
#include <functional>
class Foo
{
public:
  void updateFooId(int id);
  int getId() const;

private:
  int mId;
};
class Bar
{
public:
  void updateBarId(int id);
private:
  int mId;
};
class CThread
{
public:
  using Callback = std::function<void()>;
  CThread(Callback cb);
  ~CThread();

private:
  pthread_t m_thd;
  int id;
  Callback m_cb;
  void run();
};
#endif
