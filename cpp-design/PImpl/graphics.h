/*************************************************************************
  > File Name: graphics.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 一  1/14 15:53:38 2019
 ************************************************************************/

#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include <memory>
struct Point
{
  float x;
  float y;
};
class Graphics
{

public:
  Graphics();
  ~Graphics();
  void drawLine(float x0, float y0, float x1, float y1);
  void drawLine(Point x, Point y);
  void drawRectangle(float x0, float y0, float x1, float y1);
  void drawRectangle(Point x, Point y);

private:
  class Impl;
  std::unique_ptr<Impl> impl;
};
#endif
