// CMemory.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

class A {
public:
	A():_a(0x0a0a0a0a){}
	int _a;
	virtual void f(){}
};

class B : public A 
{
public:
	B() :_b(0x0b0b0b0b) {}
	int _b;
	virtual void f() {}
	virtual void g() {}
	void test(){}
};

class C : public B
{
public:
	C() : _c(0x0c0c0c0c){}
	int _c;
	virtual void h(){}
};

class D : public A
{
public:
	D() : _d(0x0d0d0d0d){}
	int _c;
	virtual void h(){}
};
//这是测试文本
//这是测试文本2
//这是测试文本3
//这是测试文本4
//这是测试文本5
//这是测试文本6
//这是测试文本7
//这是测试文本8
//这是测试文本9
//这是测试文本10
//这是测试文本11
class E : public A
{
public:
	E() : _e(1){}
	int _e;
	virtual void h(){}
};

int main()
{
	A a;
	B b;
	C c;
    return 0;
}

