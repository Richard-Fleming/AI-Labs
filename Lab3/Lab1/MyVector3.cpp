#include "MyVector3.h"
#include <string.h>
#include <string>
#define PI           3.14159265358979323846

/// <summary>
/// Default constructor set all 3 values to zero
/// </summary>
MyVector3::MyVector3() :
	x{ 0.0 },
	y{ 0.0 },
	z{ 0.0 }
{
}

/// <summary>
/// default destructor
/// nothing to release because we only have
/// primitive type data
/// </summary>
MyVector3::~MyVector3()
{
}

/// <summary>
/// 
/// </summary>
/// <returns>the vector as a string surrounded by square brackets and separated by commas</returns>
std::string MyVector3::toString()
{
	const std::string output = "[" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "]";
	return output;
}

// construct from three doubles
MyVector3::MyVector3(double t_x, double t_y, double t_z) :
	x{ t_x },
	y{ t_y },
	z{ t_z }
{
}

/// <summary>
/// create a vector 3 from an sf::vector3f
/// </summary>
/// <param name="t_sfVector">input vector</param>
MyVector3::MyVector3(sf::Vector3f t_sfVector) :
	x{ static_cast<double>(t_sfVector.x) },
	y{ static_cast<double>(t_sfVector.y) },
	z{ static_cast<double>(t_sfVector.z) }
{
}

// convert an integer sf::vector3i into a vector 3
MyVector3::MyVector3(sf::Vector3i t_sfVector) :
	x{ static_cast<double>(t_sfVector.x) },
	y{ static_cast<double>(t_sfVector.y) },
	z{ static_cast<double>(t_sfVector.z) }
{
}

// convert an integer sf::vector2i into a vector 3
MyVector3::MyVector3(sf::Vector2i t_sfVector) :
	x{ static_cast<double>(t_sfVector.x) },
	y{ static_cast<double>(t_sfVector.y) },
	z{ 0 }
{
}
// converting an unsigned sf::Vector2u into a vector 3
MyVector3::MyVector3(sf::Vector2u t_sfVector) :
	x{ static_cast<double>(t_sfVector.x) },
	y{ static_cast<double>(t_sfVector.y) },
	z{ 0 }
{
}

// converting a float sf::Vector2f into a vector 3
MyVector3::MyVector3(sf::Vector2f t_sfVector) :
	x{ static_cast<double>(t_sfVector.x) },
	y{ static_cast<double>(t_sfVector.y) },
	z{ 0 }
{
}

// operator overload used for adding vector 3s together
MyVector3 MyVector3::operator+(const MyVector3 t_right) const
{
	return MyVector3{ x + t_right.x, y + t_right.y, z + t_right.z };
}

// operator overload used for subtracting vector 3s from eachother
MyVector3 MyVector3::operator-(const MyVector3 t_right) const
{
	return MyVector3{ x - t_right.x, y - t_right.y, z - t_right.z };
}

// operator overload used for multiplying vector 3s with eachother, changing the vector magnitude
MyVector3 MyVector3::operator*(const double t_scalar) const
{
	return MyVector3{ x * t_scalar, y * t_scalar, z * t_scalar };
}

// operator overload used for dividing vector 3s from each other, affecting magnitude
MyVector3 MyVector3::operator/(const double t_divisor) const
{
	if (t_divisor != 0)	// never divide by zero
	{
		return MyVector3{ x / t_divisor, y / t_divisor, x / t_divisor };
	}
	else
	{
		return MyVector3{};
	}
}

// operator overload used for adding vector 3s with each other, changing the value of the current vector
MyVector3 MyVector3::operator+=(const MyVector3 t_right)
{
	x += t_right.x;
	y += t_right.y;
	z += t_right.z;
	return MyVector3{ x,y,z };
}

// operator overload used for subtracting vector 3s from each other, changing the value of the current vector
MyVector3 MyVector3::operator-=(const MyVector3 t_right)
{
	x -= t_right.x;
	y -= t_right.y;
	z -= t_right.z;
	return MyVector3{ x,y,z };
}

// checks if two vectors are equal to each other
bool MyVector3::operator==(const MyVector3 t_right) const
{
	if (x == t_right.x && y == t_right.y && z == t_right.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// checks if vector 3s are NOT equal to eachother
bool MyVector3::operator!=(const MyVector3 t_right) const
{
	if (x != t_right.x && y != t_right.y && z != t_right.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// flips the signs of all parts of a vector 3, giving the negative vector
MyVector3 MyVector3::operator-()
{
	return MyVector3(-x, -y, -z);
}

// flips the sign of the X coordinate, used when the ball bounces off the left and right cushions
void MyVector3::reverseX()
{
	x = -x;
}

// flips the sign of the y coordinate, used when the ball bounces off the top and bottom cushions
void MyVector3::reverseY()
{
	y = -y;
}

// calculates the total length of a vector
double MyVector3::length() const
{
	const double result = std::sqrt(x * x + y * y + z * z);
	return result;
}

// calculates the squared length of a vector
double MyVector3::lengthSquared() const
{
	const double result = std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2);
	return result;
}

// calculates the dot product of vector 3s
double MyVector3::dot(const MyVector3 t_other) const
{
	const double answer = (x * t_other.x) + (y * t_other.y) + (z * t_other.z);
	return answer;
}

// calculates the cross product of bector 3s
MyVector3 MyVector3::crossProduct(const MyVector3 t_other) const
{
	const double newX = (y * t_other.z - z * t_other.y);
	const double newY = (z * t_other.x - x * t_other.z);
	const double newZ = (x * t_other.y - y * t_other.x);
	return MyVector3(newX, newY, newZ);
}

double MyVector3::angleBetween(const MyVector3 t_other) const
{
	// formula: angle = arccos(a dot b/ |a| * |b|)
	double top = dot(t_other);
	double under = length() * t_other.length();
	double angleRadians;
	if (under != 0)	// never divide by zero
	{
		double answer = top / under;
		if (answer > 1) answer = 1;
		if (answer < 1) answer = -1;
		angleRadians = std::acos(answer);
		const double angleDegrees = angleRadians * 180.0 / PI;
		return angleDegrees;
	}
}

// calculates the unit vector along the current vector
MyVector3 MyVector3::unit() const
{
	MyVector3 unitVector{};
	double vectorLength = length();
	if (vectorLength != 0)
	{
		const double unitX = x / vectorLength;
		const double unitY = y / vectorLength;
		const double unitZ = z / vectorLength;
		unitVector = { unitX, unitY, unitZ };
	}
	return unitVector;
}

// re-size current vector to magnitude one
void MyVector3::normalise()
{
	const double originalLength = length();
	if (originalLength != 0)
	{
		x = x / originalLength;
		y = y / originalLength;
		z = z / originalLength;
	}
}

// projection of one vector onto the current vector (projection parallel to current)
MyVector3 MyVector3::projection(const MyVector3 t_onto) const
{
	double lengthSq = t_onto.lengthSquared();
	double dotProduct = dot(t_onto);
	double scale{ 1.0 };	// default for null vector involvement
	if (lengthSq != 0)
	{
		scale = dotProduct / lengthSq;
	}
	return t_onto * scale;
}

// perpendicular component of vector relative to current vector defined as original  - projection
// rejection is pependicular to the current vector
MyVector3 MyVector3::rejection(const MyVector3 t_onto) const
{
	return this-> operator-(projection(t_onto));
}


MyVector3::operator sf::Vector2u()
{
	double newX{ x };
	double newY{ y };
	if (newX < 0)
	{
		newX = -newX;
	}
	if (newY < 0)
	{
		newY = -newY;
	}
	return sf::Vector2u{ static_cast<unsigned>(newX),static_cast<unsigned>(newY) };
}
