#ifndef MATH_C
#define MATH_C // Prevent code from doubling up

// :: Type Definitions ::
/*
* Structure for storing X and Y floating co-ordinates
*/
typedef struct // 2D Vector Structure
{
	float x, y;
} Vector2;

// :: Methods ::
/*
* Perform the linear interpolation between two values
*/
float lerp(float value1, float value2, float amount)
{
	return value1 + (value2 - value1)*amount;
}

#endif
