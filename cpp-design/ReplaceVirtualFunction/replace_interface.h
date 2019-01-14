/*************************************************************************
  > File Name: replace_interface.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 一  1/14 19:34:13 2019
 ************************************************************************/

#ifndef _REPLACE_INTERFACE_H
#define _REPLACE_INTERFACE_H
#include <functional>
#include <string>
class Penguin
{
public:
  void run();
  void swim();
};
class Sparrow
{
public:
  void run();
  void fly();
};
class Foo
{
public:
  using FlyCallback = std::function<void()>;
  using RunCallback = std::function<void()>;
  Foo(const char *name, FlyCallback flyCb, RunCallback runCb);
  ~Foo()=default;
  void actionFoo();

private:
  FlyCallback flyCb_;
  RunCallback runCb_;
  std::string name_;
};
class Bar
{
public:
  using SwimCallback = std::function<void()>;
  using RunCallback = std::function<void()>;
  Bar(const char *name, RunCallback runCb, SwimCallback flyCb);
  ~Bar()=default;
  void actionBar();

private:
  SwimCallback swimCb_;
  RunCallback runCb_;
  std::string name_;
};
#endif
