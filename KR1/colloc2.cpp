#include <iostream>

using namespace std;

struct A {
    int a;
    A (int n):a(n){}
    virtual void f() {cout << "f()_A\n";}
    virtual int g()=0;
};

struct B : public A {
    B():A(5){}
    virtual int g(){return 1;}
    virtual void f() {cout << "f()_B\n";}
};

struct T {
    int a,b;
    T(int x = 0, int y = 0) {
        a = x; b = y;
    }
};

struct P : public T {
    T *src;
    P (T* _src):T(0, 0), src(_src){}
    T* operator->() {
        if (src)
            return src;
        else
            return this;
    }
};

int main () {
    B b;
    A* pa = &b;
    pa -> f();
    cout << pa -> a << endl;
    T t;
    P p(&t);
    p->a = p->b = 3;
    cout << t.a << ' ' << t.b << endl;
    P p1(NULL);
    cout << p1 -> a << ' ' << p1 -> b << endl;
    return 0;
}
