#include "PointLight.h"

PointLight::PointLight()
{

}

PointLight::PointLight(int red, int green, int blue, Vertex position, float attenA, float attenB, float attenC)
{
	_intensityRed = red;
	_intensityGreen = green;
	_intensityBlue = blue;
	_position = position;
	_attenA = attenA;
	_attenB = attenB;
	_attenC = attenC;
}

//Copy Constructor
PointLight::PointLight(const PointLight& p)
{
	_intensityRed = p.GetRed();
	_intensityGreen = p.GetGreen();
	_intensityBlue = p.GetBlue();
	_position = p.GetPosition();
	_attenA = p.GetAttenA();
	_attenB = p.GetAttenB();
	_attenC = p.GetAttenC();
}

PointLight::~PointLight()
{

}

//Accessors and mutators
int PointLight::GetRed() const
{
	return _intensityRed;
}

int PointLight::GetGreen() const
{
	return _intensityGreen;
}

int PointLight::GetBlue() const
{
	return _intensityBlue;
}

Vertex PointLight::GetPosition() const
{
	return _position;
}

float PointLight::GetAttenA() const
{
	return _attenA;
}

float PointLight::GetAttenB() const
{
	return _attenB;
}

float PointLight::GetAttenC() const
{
	return _attenC;
}

PointLight& PointLight::operator= (const PointLight& rhs)
{
	//Only do the assignment if we are not assigning
	//to ourselves
	if (this != &rhs)
	{
		_intensityRed = rhs.GetRed();
		_intensityGreen = rhs.GetGreen();
		_intensityBlue = rhs.GetBlue();
		_position = rhs.GetPosition();
		_attenA = rhs.GetAttenA();
		_attenB = rhs.GetAttenB();
		_attenC = rhs.GetAttenC();
	}
	return *this;
}