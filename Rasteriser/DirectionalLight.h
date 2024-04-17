#pragma once
#include "Vector3D.h"

class DirectionalLight
{
public:
	DirectionalLight();
	DirectionalLight(int red, int green, int blue, Vector3D direction);
	DirectionalLight(const DirectionalLight& d);
	~DirectionalLight();

	//Accessors
	int GetRed() const;
	int GetGreen() const;
	int GetBlue() const;
	Vector3D GetDirection() const;

	DirectionalLight& operator= (const DirectionalLight& rhs);

private:
	int _intensityRed;
	int _intensityGreen;
	int _intensityBlue;
	Vector3D _direction;
};

