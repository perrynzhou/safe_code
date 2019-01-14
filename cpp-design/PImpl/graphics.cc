/*************************************************************************
  > File Name: graphics.cc
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 一  1/14 15:53:46 2019
 ************************************************************************/

#include "graphics.h"
#include <iostream>
using namespace std;
class Graphics::Impl
{
public:
  Impl()
  {
    cout << "Grahpics::Impl()" << endl;
  }
  ~Impl()
  {
    cout << "~Graphics::Impl()" << endl;
  }
  void drawLine(float x0, float y0, float x1, float y1)
  {
    cout << "call Graphics::Impl drawLine(float x0, float y0, float x1, float y1) " << endl;
  }
  void drawLine(Point x, Point y)
  {
    cout << "call Graphics::Impl drawLine(Point x, Point y) " << endl;
  }
  void drawRectangle(float x0, float y0, float x1, float y1)
  {
    cout << "call Graphics::Impl drawRectangle(float x0, float y0, float x1, float y1)" << endl;
  }
  void drawRectangle(Point x, Point y)
  {
    cout << "call Graphics::Impl drawRectangle(Point x, Point y)" << endl;
  }
};
Graphics::Graphics() : impl(new Impl)
{
  cout << "Graphics::Graphics()" << endl;
}
Graphics::~Graphics()
{
  cout << "Graphics::~Graphics()" << endl;
}
void Graphics::drawLine(float x0, float y0, float x1, float y1)
{
  cout << "Graphics::drawLine(float x0, float y0, float x1, float y1)" << endl;
  impl->drawLine(x0, y0, x1, y1);
}
void Graphics::drawLine(Point x, Point y)
{
  impl->drawLine(x, y);
}
void Graphics::drawRectangle(float x0, float y0, float x1, float y1)
{
  impl->drawRectangle(x0, y0, x1, y1);
}
void Graphics::drawRectangle(Point x, Point y)
{
  impl->drawRectangle(x, y);
}
int main(void)
{
  Graphics graphics;
  graphics.drawLine(1.0, 2.0, 3.0, 4.0);
  return 0;
}