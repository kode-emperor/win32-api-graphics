#pragma once
#include "Vertex.h"

class PointLight
{
public:
	PointLight();
	PointLight(int red, int green, int blue, Vertex position, float attenA, float attenB, float attenC);
	PointLight(const PointLight& p);
	~PointLight();

	//Accessors and mutators
	int GetRed() const;
	int GetGreen() const;
	int GetBlue() const;
	Vertex GetPosition() const;
	float GetAttenA() const;
	float GetAttenB() const;
	float GetAttenC() const;

	PointLight& operator= (const PointLight& rhs);

private:
	int _intensityRed;
	int _intensityGreen;
	int _intensityBlue;
	Vertex _position;
	float _attenA;
	float _attenB;
	float _attenC;
};

