#pragma once
#define _USE_MATH_DEFINES  
#include <math.h>
enum class EaseType
{
	Linear,
	InSine,
	OutSine,
	InOutSine,
	InQuad,
	OutQuad,
	InOutQuad,
	InCubic,
	OutCubic,
	InOutCubic,
	InQuart,
	OutQuart,
	InOutQuart,
	InQuint,
	OutQuint,
	InOutQuint,
	InExpo,
	OutExpo,
	InOutExpo,
	InCirc,
	OutCirc,
	InOutCirc,
	InBack,
	OutBack,
	InOutBack,
	InElastic,
	OutElastic,
	InOutElastic,
	InBounce,
	OutBounce,
	InOutBounce
};

template <typename T>
class Tween
{
public:
	
	static Tween From(T from)
	{
		Tween t = Tween();
		t._from = from;
		return t;
	}
	Tween& To(T to)
	{
		_to = to;
		return *this;
	}
	Tween& For(double t)
	{
		_for = t;
		return *this;
	}
	Tween& Ease(EaseType e)
	{
		_e = e;
		return *this;
	}
    T Update(double dt)
    {
        currentT += dt;
        if (currentT >= _for)
            return _to;

        double t = currentT / _for;

        switch (_e)
        {
        case EaseType::Linear:
            return _from + (_to - _from) * t;

        case EaseType::InSine:
            return _from + (_to - _from) * (1 - cos((t * M_PI) / 2));

        case EaseType::OutSine:
            return _from + (_to - _from) * sin((t * M_PI) / 2);

        case EaseType::InOutSine:
            return _from + (_to - _from) * -(cos(M_PI * t) - 1) / 2;

        case EaseType::InQuad:
            return _from + (_to - _from) * (t * t);

        case EaseType::OutQuad:
            return _from + (_to - _from) * (1 - (1 - t) * (1 - t));

        case EaseType::InOutQuad:
            return _from + (_to - _from) * (t < 0.5 ? 2 * t * t : 1 - pow(-2 * t + 2, 2) / 2);

        case EaseType::InCubic:
            return _from + (_to - _from) * (t * t * t);

        case EaseType::OutCubic:
            return _from + (_to - _from) * (1 - pow(1 - t, 3));

        case EaseType::InOutCubic:
            return _from + (_to - _from) * (t < 0.5 ? 4 * t * t * t : 1 - pow(-2 * t + 2, 3) / 2);

        case EaseType::InQuart:
            return _from + (_to - _from) * (t * t * t * t);

        case EaseType::OutQuart:
            return _from + (_to - _from) * (1 - pow(1 - t, 4));

        case EaseType::InOutQuart:
            return _from + (_to - _from) * (t < 0.5 ? 8 * t * t * t * t : 1 - pow(-2 * t + 2, 4) / 2);

        case EaseType::InQuint:
            return _from + (_to - _from) * (t * t * t * t * t);

        case EaseType::OutQuint:
            return _from + (_to - _from) * (1 - pow(1 - t, 5));

        case EaseType::InOutQuint:
            return _from + (_to - _from) * (t < 0.5 ? 16 * t * t * t * t * t : 1 - pow(-2 * t + 2, 5) / 2);

        case EaseType::InCirc:
            return _from + (_to - _from) * (1 - sqrt(1 - pow(t, 2)));

        case EaseType::OutCirc:
            return _from + (_to - _from) * sqrt(1 - pow(t - 1, 2));

        case EaseType::InOutCirc:
            return _from + (_to - _from) * (t < 0.5 ? (1 - sqrt(1 - pow(2 * t, 2))) / 2 : (sqrt(1 - pow(-2 * t + 2, 2)) + 1) / 2);

        case EaseType::InElastic:
            return _from + (_to - _from) * (-pow(2, 10 * t - 10) * sin((t * 10 - 10.75) * (2 * M_PI) / 3));

        case EaseType::OutElastic:
            return _from + (_to - _from) * (pow(2, -10 * t) * sin((t * 10 - 0.75) * (2 * M_PI) / 3) + 1);

        case EaseType::InOutElastic:
            return _from + (_to - _from) * (t < 0.5
                ? -(pow(2, 20 * t - 10) * sin((20 * t - 11.125) * (2 * M_PI) / 4.5)) / 2
                : (pow(2, -20 * t + 10) * sin((20 * t - 11.125) * (2 * M_PI) / 4.5)) / 2 + 1);

        case EaseType::InBack:
            return _from + (_to - _from) * (2.70158 * t * t * t - 1.70158 * t * t);

        case EaseType::OutBack:
            return _from + (_to - _from) * (1 + 2.70158 * pow(t - 1, 3) + 1.70158 * pow(t - 1, 2));

        case EaseType::InOutBack:
            return _from + (_to - _from) * (t < 0.5
                ? (pow(2 * t, 2) * ((1.70158 * 1.525 + 1) * 2 * t - 1.70158 * 1.525)) / 2
                : (pow(2 * t - 2, 2) * ((1.70158 * 1.525 + 1) * (t * 2 - 2) + 1.70158 * 1.525) + 2) / 2);

        case EaseType::InBounce:
            return _from + (_to - _from) * (1 - OutBounce(1 - t));

        case EaseType::OutBounce:
            if (t < 1 / 2.75)
                return _from + (_to - _from) * (7.5625 * t * t);
            else if (t < 2 / 2.75)
                return _from + (_to - _from) * (7.5625 * (t -= 1.5 / 2.75) * t + 0.75);
            else if (t < 2.5 / 2.75)
                return _from + (_to - _from) * (7.5625 * (t -= 2.25 / 2.75) * t + 0.9375);
            else
                return _from + (_to - _from) * (7.5625 * (t -= 2.625 / 2.75) * t + 0.984375);

        case EaseType::InOutBounce:
            return _from + (_to - _from) * (t < 0.5
                ? (1 - OutBounce(1 - 2 * t)) / 2
                : (1 + OutBounce(2 * t - 1)) / 2);

        default:
            return _from;
        }
    }
	void Erase();
private :
	T _from;
	T _to;
	double _for;
	double currentT = 0; //current Time
	EaseType _e = EaseType::Linear;
    double OutBounce(double t)
    {
        if (t < 1 / 2.75)
            return 7.5625 * t * t;
        else if (t < 2 / 2.75)
            return 7.5625 * (t -= 1.5 / 2.75) * t + 0.75;
        else if (t < 2.5 / 2.75)
            return 7.5625 * (t -= 2.25 / 2.75) * t + 0.9375;
        else
            return 7.5625 * (t -= 2.625 / 2.75) * t + 0.984375;
    }
};

