#include"Utility.h"

float Utility::Lerp(float start, float end, float t)
{
	return start + t * (end - start);
}

float Utility::Rnd(float _min, float _max)
{
	return _min + (rand() / (float)RAND_MAX) * (_max - _min);
}

float Utility::SinRange(float x, float minVal, float maxVal)
{
	float result = sin(x);
	result = (result + 1) / 2;
	result = result * (maxVal - minVal) + minVal;
	return result;
}

bool Utility::CheckHit2D(float a_target1X, float a_target1Y, float a_target2X, float a_target2Y, float a_hitDist)
{
	return false;
}

float Utility::min(float a, float b)
{
	return 0.0f;
}

float Utility::max(float a, float b)
{
	return 0.0f;
}
