// :: Type Definitions ::
/*
* Structure for storing X and Y associated values
*/
struct vec2f{	float x, y; };
//struct vec2i{	long x, y; };
//struct vec2s{	short x, y; };
//struct vec2c{	char x, y; };

// :: Functions ::
/*
* Perform the linear interpolation between two values
*/
float lerp(float value1, float value2, float amount){	return value1 + (value2 - value1)*amount; }
//long lerp(long value1, long value2, float amount){	return value1 + (value2 - value1)*amount; }
//long lerp(long value1, long value2, double amount){	return value1 + (value2 - value1)*amount; }


/*
Calculate the approximate number of ticks required to travel a meter for a given speed.
NOTE: For Drive Motors Only
*/
float ApproximateDeltaTicks_Drive(int speed)
{
	const float a = -13.552222;
	const float b = 4577.43518;
	return a * (float)speed + b;
}

/*
Calculate the approximate distance traveld through the drive wheels in meters
NOTE: For Drive Motors Only
*/
float ApproximateDistance_Drive(int ticks, int speed)
{
	// Calculate the number of ticks per meter
	float deltaticks = ApproximateDeltaTicks_Drive(speed);
	return (float)ticks / deltaticks; // Ratio
}