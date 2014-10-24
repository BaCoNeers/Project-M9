#ifndef MATH_C
#define MATH_C

// :: Type Definitions ::
/*
* Structure for storing X and Y associated values
*/
struct vec2f{	float x, y; };
//struct vec2i{	long x, y; };
//struct vec2s{	short x, y; };
//struct vec2c{	char x, y; };

// :: Methods ::
/*
* Perform the linear interpolation between two values
*/
float lerp(float value1, float value2, float amount){	return value1 + (value2 - value1)*amount; }
//long lerp(long value1, long value2, float amount){	return value1 + (value2 - value1)*amount; }
//long lerp(long value1, long value2, double amount){	return value1 + (value2 - value1)*amount; }

#endif
