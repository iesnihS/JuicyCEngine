#pragma once

#include <cctype>
#include <cstdlib>


class VirtualCallTest {
public:
	static volatile double accum;
	static void flushCache();
};
class A {
public:
	virtual void doSomething();
};

class B : public A {
public:
	virtual void doSomething() override;
};

class CC : public B {
public:
	virtual void doSomething() override;
};

class D : public CC {
public:
	virtual void doSomething() override;
};

class E : public D {
public:
	virtual void doSomething() override;
};

class F : public E {
public:
	virtual void doSomething() override ;
};

class G : public F {
public:
	virtual void doSomething() override;
};

class H : public G {
public:
	virtual void doSomething() override;
};

class AA {
public:
	void doSomething();
};


class AAA {
public:
	inline void doSomething() {
		VirtualCallTest::accum += Dice::randF();
		VirtualCallTest::accum += Dice::randF();
		VirtualCallTest::accum += Dice::randF();
		VirtualCallTest::accum += Dice::randF();
		VirtualCallTest::accum += Dice::randF();
		VirtualCallTest::accum += Dice::randF();
		VirtualCallTest::accum += Dice::randF();
		VirtualCallTest::accum += Dice::randF();
	}
};