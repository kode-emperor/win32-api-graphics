#pragma once
#include "Vector3D.h"
#include "stdafx.h"

class Vertex
{
public:
	Vertex();
	Vertex(float x, float y, float z, float w);

	//Copy constructor
	Vertex(const Vertex& v);

	//Destructor
	~Vertex();

	//Accessors and mutators
	float GetX() const;
	int GetIntX() const;
	void SetX(const float x);
	float GetY() const;
	int GetIntY() const;
	void SetY(const float y);
	float GetZ() const;
	void SetZ(const float z);
	float GetW() const;
	void SetW(const float w);
	Vector3D GetNormal() const;
	void SetNormal(const Vector3D normal);
	COLORREF GetColour() const;
	void SetColour(const COLORREF colour);
	int GetNormalCount() const;
	void SetNormalCount(const int normalCount);
	void IncrementNormalCount();

	void Dehomogenize();

	Vector3D MinusToVector(const Vertex &other) const;

	//Assignment operator
	Vertex& operator= (const Vertex& rhs);

	bool operator== (const Vertex& rhs) const;

	const Vertex operator+ (const Vertex& rhs) const;

	const Vertex operator- (const Vertex& rhs) const;


private:
	float _x;
	float _y;
	float _z;
	float _w;
	Vector3D _normal;
	COLORREF _colour;
	int _normalCount;
};

