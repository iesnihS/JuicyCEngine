#pragma once
template <typename T>
class Tween
{
public:
	static Tween From(T from);
	Tween& To(T to);

};

