
##### name-mangling 

- sample code

```
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

```

- complied
```
perrynzhou@debian:~/Source/perrynzhou-source/safe_code/cpp-design/NameMangling$ g++ -std=gnu++11 base.h base.cc -o test
perrynzhou@debian:~/Source/perrynzhou-source/safe_code/cpp-design/NameMangling$ nm test |grep add
0000000000000bb6 t _GLOBAL__sub_I__ZN6Object3addEi
0000000000000a20 T _ZN6Object3addEi   //int add(int)
0000000000000a5e T _ZN6Object3addEii  //void add(int,int)
0000000000000a9c T _ZN6Object3addEiii //int add(int,int,int)

通过name-mangling技术,add函数被被编译器编译为内部能级别的不同方法(_ZN6Object3addEi/_ZN6Object3addEii/_ZN6Object3addEiii)，代表三个不同的函数

重载(overload):
    1.相同作用域(同一个class的作用域)
    2.函数名称相同(参数个数，参数列表不同)
    3.virtual可有可无
重写(overwrite):
    1.不同作用域(分别位于基类和派生类)
    2.函数名称和函数参数列表完全相同
    3.基类函数必须是虚函数(virtual修饰)
```
