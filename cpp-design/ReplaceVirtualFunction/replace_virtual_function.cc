/*************************************************************************
  > File Name: replace_virtual_function.cc
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: 一  1/14 16:40:05 2019
 ************************************************************************/
#include "replace_virtual_function.h"
#include <functional>
using namespace std;
void Foo::updateFooId(int id) {
  mId = id;
  fprintf(stdout, "current thread  %ld mId=%ld\n", pthread_self(), mId);
}
void Bar::updateBarId(int id) {
  mId = id;
  fprintf(stdout, "current thread  %ld mId=%ld\n", pthread_self(), mId);
}
CThread::CThread(Callback cb) : m_cb(cb) {

  pthread_create(&m_thd, nullptr,
                 [](void *arg) {
                   CThread *thread = static_cast<CThread *>(arg);
                   thread->run();
                   return (void *)0;
                 },
                 this);
}
void CThread::run() { m_cb(); }
CThread::~CThread() { pthread_join(m_thd, nullptr); }
int main(void) {
  Foo foo;
  Bar bar;
  CThread thread1(bind(&Foo::updateFooId, &foo, 105));
  CThread thread2(bind(&Bar::updateBarId, &bar, 200));
  return 0;
}