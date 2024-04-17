#include "AmbientLight.h"

AmbientLight::AmbientLight()
{

}

AmbientLight::AmbientLight(int red, int green, int blue)
{
	_intensityRed = red;
	_intensityGreen = green;
	_intensityBlue = blue;
}

//Copy Constructor
AmbientLight::AmbientLight(const AmbientLight& a)
{
	_intensityRed = a.GetRed();
	_intensityGreen = a.GetGreen();
	_intensityBlue = a.GetBlue();
}

AmbientLight::~AmbientLight()
{

}

//Accessors and mutators
int AmbientLight::GetRed() const
{
	return _intensityRed;
}

void AmbientLight::SetRed(const int red)
{
	_intensityRed = red;
}

int AmbientLight::GetGreen() const
{
	return _intensityGreen;
}

void AmbientLight::SetGreen(const int green)
{
	_intensityGreen = green;
}

int AmbientLight::GetBlue() const
{
	return _intensityBlue;
}
void AmbientLight::SetBlue(const int blue)
{
	_intensityBlue = blue;
}
