
#include <iostream>
#include "Dice.hpp"
#include "VirtualCallTest.hpp"

volatile double	VirtualCallTest::accum = 0;
static int				cacheSize = 16 * 1024 * 1024;

void VirtualCallTest::flushCache() {
	char* mem = (char*)malloc(cacheSize);
	for (volatile int i = 0; i < cacheSize; ++i)
		accum  += mem[i] = Dice::roll(Dice::roll(0,8), Dice::roll(150,250));
	//std::cout << mem[0] << std::endl;
	free(mem);
}

void AA::doSomething() {
	VirtualCallTest::accum += Dice::randF();
	VirtualCallTest::accum += Dice::randF();
	VirtualCallTest::accum += Dice::randF();
	VirtualCallTest::accum += Dice::randF();
	VirtualCallTest::accum += Dice::randF();
	VirtualCallTest::accum += Dice::randF();
	VirtualCallTest::accum += Dice::randF();
	VirtualCallTest::accum += Dice::randF();
}

void A::doSomething() {
	VirtualCallTest::accum += Dice::randF();
}

void B::doSomething()
{
	A::doSomething();
	VirtualCallTest::accum += Dice::randF();
}

void CC::doSomething()
{
	B::doSomething();
	VirtualCallTest::accum += Dice::randF();
}

void D::doSomething()
{
	CC::doSomething();
	VirtualCallTest::accum += Dice::randF();
}

void E::doSomething()
{
	D::doSomething();
	VirtualCallTest::accum += Dice::randF();
}


void F::doSomething()
{
	E::doSomething();
	VirtualCallTest::accum += Dice::randF();
}

void G::doSomething()
{
	F::doSomething();
	VirtualCallTest::accum += Dice::randF();
}


void H::doSomething()
{
	G::doSomething();
	VirtualCallTest::accum += Dice::randF();
}

