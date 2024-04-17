#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{

}

DirectionalLight::DirectionalLight(int red, int green, int blue, Vector3D direction)
{
	_intensityRed = red;
	_intensityGreen = green;
	_intensityBlue = blue;
	_direction = direction;
}

//Copy Constructor
DirectionalLight::DirectionalLight(const DirectionalLight& d)
{
	_intensityRed = d.GetRed();
	_intensityGreen = d.GetGreen();
	_intensityBlue = d.GetBlue();
	_direction = d.GetDirection();
}

DirectionalLight::~DirectionalLight()
{

}

//Accessors
int DirectionalLight::GetRed() const
{
	return _intensityRed;
}

int DirectionalLight::GetGreen() const
{
	return _intensityGreen;
}

int DirectionalLight::GetBlue() const
{
	return _intensityBlue;
}

Vector3D DirectionalLight::GetDirection() const
{
	return _direction;
}

DirectionalLight& DirectionalLight::operator= (const DirectionalLight& rhs)
{
	//Only do the assignment if we are not assigning
	//to ourselves
	if (this != &rhs)
	{
		_intensityRed = rhs.GetRed();
		_intensityGreen = rhs.GetGreen();
		_intensityBlue = rhs.GetBlue();
		_direction = rhs.GetDirection();
	}
	return *this;
}