#pragma once

class AmbientLight
{
public:
	AmbientLight();
	AmbientLight(int red, int green, int blue);
	AmbientLight(const AmbientLight& a);
	~AmbientLight();

	//Accessors and mutators
	int GetRed() const;
	void SetRed(const int red);
	int GetGreen() const;
	void SetGreen(const int green);
	int GetBlue() const;
	void SetBlue(const int blue);

private:
	int _intensityRed;
	int _intensityGreen;
	int _intensityBlue;
};

