#include "Rasteriser.h"
//Link to the winmm library, so I can play music
#pragma comment(lib, "winmm.lib")

Rasteriser app;

float RotAngle = float(M_PI_2);
float scale = 1;
float translateX = 0;
float translateY = 0;
float translateZ = 0;
const int timeFactor = 7000;

bool Rasteriser::Initialise()
{
	if (!MD2Loader::LoadModel("sonic.md2", _model,
		&Model::AddPolygon,
		&Model::AddVertex))
	{
		return false;
	}

	//Sets the ka, kd, and ks values
	_model.SetKValues(0.2f, 0.2f, 0.2f,
					  0.5f, 0.5f, 0.5f,
					  1.0f, 1.0f, 1.0f);

	//Set the camera values
	Camera temp(0, 0, 0, Vertex(0, 7, -11, 1));
	_camera = temp;

	//Set the ambient light
	_ambientLight.SetRed(0);
	_ambientLight.SetGreen(255);
	_ambientLight.SetBlue(0);

	//Set the directional light(s)
	Vector3D lightDirection(1, -1, 1);
	DirectionalLight light1(33, 73, 222, lightDirection);
	_directionalLights.push_back(light1);

	//Set the point light(s)
	Vertex pointLight1Position(0, -25, 0, 1);
	PointLight pointLight1(255, 255, 0, pointLight1Position, 1, 0.5f, 0);
	_pointLights.push_back(pointLight1);

	PlaySound(TEXT("spacequeensintro.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);

	return true;
}

void Rasteriser::DrawWireFrame(Bitmap &bitmap)
{
	std::vector<Polygon3D> tempPolygon(_model.GetPolygons());
	int polygonSize = int(_model.GetPolygonCount());
	std::vector<Vertex> tempVertex(_model.GetTransform());

	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HGDIOBJ oldPen = SelectObject(bitmap.GetDC(), hPen);
	for (int i = 0; i < polygonSize; i++)
	{
		if (!tempPolygon[i].GetCull())
		{
			//Get the indices
			int tempIndex0 = tempPolygon[i].GetIndex(0);
			Vertex tempVertex0(tempVertex[tempIndex0]);
			int tempIndex1 = tempPolygon[i].GetIndex(1);
			Vertex tempVertex1(tempVertex[tempIndex1]);
			int tempIndex2 = tempPolygon[i].GetIndex(2);
			Vertex tempVertex2(tempVertex[tempIndex2]);

			//Draw the triangle
			MoveToEx(bitmap.GetDC(), tempVertex0.GetIntX(), tempVertex0.GetIntY(), NULL);
			LineTo(bitmap.GetDC(), tempVertex1.GetIntX(), tempVertex1.GetIntY());
			LineTo(bitmap.GetDC(), tempVertex2.GetIntX(), tempVertex2.GetIntY());
			LineTo(bitmap.GetDC(), tempVertex0.GetIntX(), tempVertex0.GetIntY());
		}
	}
	DeleteObject(hPen);
}

void Rasteriser::DrawSolidFlat(Bitmap &bitmap)
{
	std::vector<Vertex> tempVertex(_model.GetTransform());
	std::vector<Polygon3D> tempPolygon(_model.GetPolygons());
	int polygonSize = int(_model.GetPolygonCount());
	
	for (int i = 0; i < polygonSize; i++)
	{
		if (!tempPolygon[i].GetCull())
		{
			HPEN hPen = CreatePen(PS_SOLID, 1, tempPolygon[i].GetColour());
			HGDIOBJ oldPen = SelectObject(bitmap.GetDC(), hPen);

			HBRUSH hBrush = CreateSolidBrush(tempPolygon[i].GetColour());
			HGDIOBJ oldBrush = SelectObject(bitmap.GetDC(), hBrush);
			//Get the indices
			int tempIndex0 = tempPolygon[i].GetIndex(0);
			Vertex tempVertex0(tempVertex[tempIndex0]);
			int tempIndex1 = tempPolygon[i].GetIndex(1);
			Vertex tempVertex1(tempVertex[tempIndex1]);
			int tempIndex2 = tempPolygon[i].GetIndex(2);
			Vertex tempVertex2(tempVertex[tempIndex2]);

			POINT temp0{ long(tempVertex0.GetX()), long(tempVertex0.GetY()) };
			POINT temp1{ long(tempVertex1.GetX()), long(tempVertex1.GetY()) };
			POINT temp2{ long(tempVertex2.GetX()), long(tempVertex2.GetY()) };

			POINT tempArray[] = { temp0, temp1, temp2 };

			Polygon(bitmap.GetDC(), tempArray, 3);

			DeleteObject(hPen);
			DeleteObject(hBrush);
		}
	}
}

void Rasteriser::MyDrawSolidFlat(Bitmap &bitmap)
{
	std::vector<Vertex> tempVertex(_model.GetTransform());
	std::vector<Polygon3D> tempPolygon(_model.GetPolygons());
	int polygonSize = int(_model.GetPolygonCount());

	for (int i = 0; i < polygonSize; i++)
	{
		if (!tempPolygon[i].GetCull())
		{
			//Get the indices
			int tempIndex0 = tempPolygon[i].GetIndex(0);
			Vertex tempVertex0(tempVertex[tempIndex0]);
			int tempIndex1 = tempPolygon[i].GetIndex(1);
			Vertex tempVertex1(tempVertex[tempIndex1]);
			int tempIndex2 = tempPolygon[i].GetIndex(2);
			Vertex tempVertex2(tempVertex[tempIndex2]);

			FillPolygonFlat(tempVertex0, tempVertex1, tempVertex2, tempPolygon[i].GetColour(), bitmap);
		}
	}
}

void Rasteriser::FillPolygonFlat(Vertex vertex0, Vertex vertex1, Vertex vertex2, COLORREF polygonColour, Bitmap &bitmap)
{
	Vertex v0;
	Vertex v1;
	Vertex v2;
	//Very basic, but not necessarily good or effecient way of sorting the 3 vertices. However, it does work.
	if (vertex0.GetY() < vertex1.GetY() && vertex0.GetY() < vertex2.GetY())
	{
		v0 = vertex0;
		if (vertex1.GetY() < vertex2.GetY())
		{
			v1 = vertex1;
			v2 = vertex2;
		}
		else
		{
			v1 = vertex2;
			v2 = vertex1;
		}
	}
	if (vertex1.GetY() < vertex0.GetY() && vertex1.GetY() < vertex2.GetY())
	{
		v0 = vertex1;
		if (vertex0.GetY() < vertex2.GetY())
		{
			v1 = vertex0;
			v2 = vertex2;
		}
		else
		{
			v1 = vertex2;
			v2 = vertex0;
		}
	}
	if (vertex2.GetY() < vertex0.GetY() && vertex2.GetY() < vertex1.GetY())
	{
		v0 = vertex2;
		if (vertex0.GetY() < vertex1.GetY())
		{
			v1 = vertex0;
			v2 = vertex1;
		}
		else
		{
			v1 = vertex1;
			v2 = vertex0;
		}
	}

	//Convert the vertex values to "integers" (still stored as floats) to avoid drawing erroneous horizontal lines
	v0.SetX(float(v0.GetIntX()));
	v0.SetY(float(v0.GetIntY()));

	v1.SetX(float(v1.GetIntX()));
	v1.SetY(float(v1.GetIntY()));

	v2.SetX(float(v2.GetIntX()));
	v2.SetY(float(v2.GetIntY()));

	//Check for trivial case of bottom-flat triangle
	if (v1.GetY() == v2.GetY())
	{
		FillBottomFlatTriangle(v0, v1, v2, polygonColour, bitmap);
	}
	//Check for trivial case of top-flat triangle
	else if (v0.GetY() == v1.GetY())
	{
		FillTopFlatTriangle(v0, v1, v2, polygonColour, bitmap);
	}
	else
	{
		//General case - split the triangle in a topflat and bottom-flat one
		Vertex temp((v0.GetX() + ((v1.GetY() - v0.GetY()) / (v2.GetY() - v0.GetY())) * (v2.GetX() - v0.GetX())), v1.GetY(), 0, 1);

		//Convert vertex values to "integers", as with v0, v1, and v2
		temp.SetX(float(temp.GetIntX()));
		temp.SetY(float(temp.GetIntY()));

		FillBottomFlatTriangle(v0, v1, temp, polygonColour, bitmap);
		FillTopFlatTriangle(v1, temp, v2, polygonColour, bitmap);
	}
}

void Rasteriser::FillBottomFlatTriangle(Vertex v0, Vertex v1, Vertex v2, COLORREF colour, Bitmap &bitmap)
{
	//Used for interpolation in the Y direction
	float slope0 = (v1.GetX() - v0.GetX()) / (v1.GetY() - v0.GetY());
	float slope1 = (v2.GetX() - v0.GetX()) / (v2.GetY() - v0.GetY());

	float x0 = v0.GetX();
	float x1 = v0.GetX();

	for (int scanlineY = v0.GetIntY(); scanlineY <= v1.GetIntY(); scanlineY++)
	{
		//Swaps x0 and x1 if x0 is larger than x1
		float xSmall;
		float xBig;
		if (x0 < x1)
		{
			xSmall = x0;
			xBig = x1;
		}
		else
		{
			xSmall = x1;
			xBig = x0;
		}

		//Draw the horizontal line
		for (float i = xSmall; i <= xBig; i++)
		{
			SetPixel(bitmap.GetDC(), int(i), scanlineY, colour);
		}

		x0 += slope0;
		x1 += slope1;
	}
}

void Rasteriser::FillTopFlatTriangle(Vertex v0, Vertex v1, Vertex v2, COLORREF colour, Bitmap &bitmap)
{
	//Used for interpolation in the Y direction
	float slope0 = (v2.GetX() - v0.GetX()) / (v2.GetY() - v0.GetY());
	float slope1 = (v2.GetX() - v1.GetX()) / (v2.GetY() - v1.GetY());

	float x0 = v2.GetX();
	float x1 = v2.GetX();

	for (int scanlineY = v2.GetIntY(); scanlineY > v0.GetIntY(); scanlineY--)
	{
		//Swaps x0 and x1 if x0 is larger than x1
		float xSmall;
		float xBig;
		if (x0 < x1)
		{
			xSmall = x0;
			xBig = x1;
		}
		else
		{
			xSmall = x1;
			xBig = x0;
		}

		//Draw the horizontal line
		for (float i = xSmall; i <= xBig; i++)
		{
			SetPixel(bitmap.GetDC(), int(i), scanlineY, colour);
		}

		x0 -= slope0;
		x1 -= slope1;
	}
}

void Rasteriser::DrawGouraud(Bitmap &bitmap)
{
	std::vector<Vertex> tempVertex(_model.GetTransform());
	std::vector<Polygon3D> tempPolygon(_model.GetPolygons());
	int polygonSize = int(_model.GetPolygonCount());

	for (int i = 0; i < polygonSize; i++)
	{
		if (!tempPolygon[i].GetCull())
		{
			//Get the indices
			int tempIndex0 = tempPolygon[i].GetIndex(0);
			Vertex tempVertex0(tempVertex[tempIndex0]);
			int tempIndex1 = tempPolygon[i].GetIndex(1);
			Vertex tempVertex1(tempVertex[tempIndex1]);
			int tempIndex2 = tempPolygon[i].GetIndex(2);
			Vertex tempVertex2(tempVertex[tempIndex2]);

			FillPolygonGouraud(tempVertex0, tempVertex1, tempVertex2, bitmap);
		}
	}
}

void Rasteriser::FillPolygonGouraud(Vertex vertex0, Vertex vertex1, Vertex vertex2, Bitmap &bitmap)
{
	Vertex v0;
	Vertex v1;
	Vertex v2;
	//Very basic, but not necessarily good or effecient way of sorting the 3 vertices. However, it does work.
	if (vertex0.GetY() < vertex1.GetY() && vertex0.GetY() < vertex2.GetY())
	{
		v0 = vertex0;
		if (vertex1.GetY() < vertex2.GetY())
		{
			v1 = vertex1;
			v2 = vertex2;
		}
		else
		{
			v1 = vertex2;
			v2 = vertex1;
		}
	}
	if (vertex1.GetY() < vertex0.GetY() && vertex1.GetY() < vertex2.GetY())
	{
		v0 = vertex1;
		if (vertex0.GetY() < vertex2.GetY())
		{
			v1 = vertex0;
			v2 = vertex2;
		}
		else
		{
			v1 = vertex2;
			v2 = vertex0;
		}
	}
	if (vertex2.GetY() < vertex0.GetY() && vertex2.GetY() < vertex1.GetY())
	{
		v0 = vertex2;
		if (vertex0.GetY() < vertex1.GetY())
		{
			v1 = vertex0;
			v2 = vertex1;
		}
		else
		{
			v1 = vertex1;
			v2 = vertex0;
		}
	}

	//Convert the vertex values to "integers" (still stored as floats) to avoid drawing erroneous horizontal lines
	v0.SetX(float(v0.GetIntX()));
	v0.SetY(float(v0.GetIntY()));

	v1.SetX(float(v1.GetIntX()));
	v1.SetY(float(v1.GetIntY()));

	v2.SetX(float(v2.GetIntX()));
	v2.SetY(float(v2.GetIntY()));

	//Check for trivial case of bottom-flat triangle
	if (v1.GetY() == v2.GetY())
	{
		FillBottomFlatGouraudTriangle(v0, v1, v2, bitmap);
	}
	//Check for trivial case of top-flat triangle
	else if (v0.GetY() == v1.GetY())
	{
		FillTopFlatGouraudTriangle(v0, v1, v2, bitmap);
	}
	else
	{
		//General case - split the triangle in a topflat and bottom-flat one
		Vertex temp((v0.GetX() + ((v1.GetY() - v0.GetY()) / (v2.GetY() - v0.GetY())) * (v2.GetX() - v0.GetX())), v1.GetY(), 0, 1);
		float tempR = float(GetRValue(v0.GetColour()) + ((v1.GetY() - v0.GetY()) / (v2.GetY() - v0.GetY())) * (GetRValue(v2.GetColour()) - GetRValue(v0.GetColour())));
		float tempG = float(GetGValue(v0.GetColour()) + ((v1.GetY() - v0.GetY()) / (v2.GetY() - v0.GetY())) * (GetGValue(v2.GetColour()) - GetGValue(v0.GetColour())));
		float tempB = float(GetBValue(v0.GetColour()) + ((v1.GetY() - v0.GetY()) / (v2.GetY() - v0.GetY())) * (GetBValue(v2.GetColour()) - GetBValue(v0.GetColour())));
		temp.SetColour(RGB(tempR, tempG, tempB));

		//Convert vertex values to "integers", as with v0, v1, and v2
		temp.SetX(float(temp.GetIntX()));
		temp.SetY(float(temp.GetIntY()));

		FillBottomFlatGouraudTriangle(v0, v1, temp, bitmap);
		FillTopFlatGouraudTriangle(v1, temp, v2, bitmap);
	}
}

void Rasteriser::FillBottomFlatGouraudTriangle(Vertex v0, Vertex v1, Vertex v2, Bitmap &bitmap)
{
	//Used for interpolation in the Y direction
	float slope0 = (v1.GetX() - v0.GetX()) / (v1.GetY() - v0.GetY());
	float slope1 = (v2.GetX() - v0.GetX()) / (v2.GetY() - v0.GetY());

	float x0 = v0.GetX();
	float x1 = v0.GetX();

	//Used for interpolation of colour in the Y direction
	float v1v0Diff = v1.GetY() - v0.GetY();
	float slope0R = float((GetRValue(v1.GetColour()) - GetRValue(v0.GetColour())) / v1v0Diff);
	float slope0G = float((GetGValue(v1.GetColour()) - GetGValue(v0.GetColour())) / v1v0Diff);
	float slope0B = float((GetBValue(v1.GetColour()) - GetBValue(v0.GetColour())) / v1v0Diff);

	float v2v0Diff = v2.GetY() - v0.GetY();
	float slope1R = float((GetRValue(v2.GetColour()) - GetRValue(v0.GetColour())) / v2v0Diff);
	float slope1G = float((GetGValue(v2.GetColour()) - GetGValue(v0.GetColour())) / v2v0Diff);
	float slope1B = float((GetBValue(v2.GetColour()) - GetBValue(v0.GetColour())) / v2v0Diff);

	float x0R = GetRValue(v0.GetColour());
	float x0G = GetGValue(v0.GetColour());
	float x0B = GetBValue(v0.GetColour());

	float x1R = GetRValue(v0.GetColour());
	float x1G = GetGValue(v0.GetColour());
	float x1B = GetBValue(v0.GetColour());

	//Swaps slope0 and slope1 if slope0 is larger than slope1
	if (slope1 < slope0)
	{
		float slopeTemp = slope0;
		slope0 = slope1;
		slope1 = slopeTemp;

		float colourTemp = slope0R;
		slope0R = slope1R;
		slope1R = colourTemp;

		colourTemp = slope0G;
		slope0G = slope1G;
		slope1G = colourTemp;

		colourTemp = slope0B;
		slope0B = slope1B;
		slope1B = colourTemp;
	}

	for (int scanlineY = v0.GetIntY(); scanlineY <= v1.GetIntY(); scanlineY++)
	{
		//Draw the horizontal line
		for (float i = x0; i <= x1; i++)
		{
			float t = (i - x0) / (x1 - x0);

			int red = int((1 - t) * x0R + t * x1R);
			int green = int((1 - t) * x0G + t * x1G);
			int blue = int((1 - t) * x0B + t * x1B);

			SetPixel(bitmap.GetDC(), int(i), scanlineY, RGB(red, green, blue));
			
		}

		x0 += slope0;
		x1 += slope1;

		x0R += slope0R;
		x0G += slope0G;
		x0B += slope0B;
		
		x1R += slope1R;
		x1G += slope1G;
		x1B += slope1B;
	}
}

void Rasteriser::FillTopFlatGouraudTriangle(Vertex v0, Vertex v1, Vertex v2, Bitmap &bitmap)
{
	//Used for interpolation in the Y direction
	float slope0 = (v2.GetX() - v0.GetX()) / (v2.GetY() - v0.GetY());
	float slope1 = (v2.GetX() - v1.GetX()) / (v2.GetY() - v1.GetY());

	float x0 = v2.GetX();
	float x1 = v2.GetX();

	//Used for interpolation of colour in the Y direction
	float v2v0Diff = v2.GetY() - v0.GetY();
	float slope0R = float((GetRValue(v2.GetColour()) - GetRValue(v0.GetColour())) / v2v0Diff);
	float slope0G = float((GetGValue(v2.GetColour()) - GetGValue(v0.GetColour())) / v2v0Diff);
	float slope0B = float((GetBValue(v2.GetColour()) - GetBValue(v0.GetColour())) / v2v0Diff);

	float v2v1Diff = v2.GetY() - v1.GetY();
	float slope1R = float((GetRValue(v2.GetColour()) - GetRValue(v1.GetColour())) / v2v1Diff);
	float slope1G = float((GetGValue(v2.GetColour()) - GetGValue(v1.GetColour())) / v2v1Diff);
	float slope1B = float((GetBValue(v2.GetColour()) - GetBValue(v1.GetColour())) / v2v1Diff);

	float x0R = GetRValue(v2.GetColour());
	float x0G = GetGValue(v2.GetColour());
	float x0B = GetBValue(v2.GetColour());

	float x1R = GetRValue(v2.GetColour());
	float x1G = GetGValue(v2.GetColour());
	float x1B = GetBValue(v2.GetColour());

	//Swaps slope0 and slope1 if slope0 is larger than slope1
	if (slope1 < slope0)
	{
		float slopeTemp = slope0;
		slope0 = slope1;
		slope1 = slopeTemp;

		float colourTemp = slope0R;
		slope0R = slope1R;
		slope1R = colourTemp;

		colourTemp = slope0G;
		slope0G = slope1G;
		slope1G = colourTemp;

		colourTemp = slope0B;
		slope0B = slope1B;
		slope1B = colourTemp;
	}

	for (int scanlineY = v2.GetIntY(); scanlineY > v0.GetIntY(); scanlineY--)
	{
		//Draw the horizontal line
		for (float i = x1; i <= x0; i++)
		{
			float t = (i - x1) / (x0 - x1);

			int red = int((1 - t) * x1R + t * x0R);
			int green = int((1 - t) * x1G + t * x0G);
			int blue = int((1 - t) * x1B + t * x0B);

			SetPixel(bitmap.GetDC(), int(i), scanlineY, RGB(red, green, blue));
		}

		x0 -= slope0;
		x1 -= slope1;

		x0R -= slope0R;
		x0G -= slope0G;
		x0B -= slope0B;
		
		x1R -= slope1R;
		x1G -= slope1G;
		x1B -= slope1B;
	}
}

void Rasteriser::DrawString(Bitmap &bitmap, LPCTSTR text, int xPos, int yPos, int fontSize)
{
	HDC hdc = bitmap.GetDC();
	HFONT hFont, hOldFont;

	//Retrieve a handle to the variable stock font.  
	hFont = hFont = CreateFont(fontSize, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Myfont"));

	//Select the variable stock font into the specified device context. 
	if (hOldFont = (HFONT)SelectObject(hdc, hFont))
	{
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, TRANSPARENT);

		//Display the text string.  
		TextOut(hdc, xPos, yPos, text, lstrlen(text));

		//Restore the original font.        
		SelectObject(hdc, hOldFont);
	}
	DeleteObject(hFont);
}

void Rasteriser::DisplayAllStrings(Bitmap &bitmap)
{
	//Seperated the displaying of the text to this method to avoid any overlap when displaying
	if (clock() > 1 * timeFactor && clock() <= 2 * timeFactor)
	{
		DrawString(bitmap, L"Culled Backfaces", 10, 55, 35);
	}
	if (clock() > 2 * timeFactor && clock() <= 3 * timeFactor)
	{
		DrawString(bitmap, L"Ambient Lighting", 10, 55, 35);
	}
	if (clock() > 3 * timeFactor && clock() <= 4 * timeFactor)
	{
		DrawString(bitmap, L"Directional Lighting, direction (1, -1, 1)", 10, 55, 35);
	}
	if (clock() > 4 * timeFactor && clock() <= 5 * timeFactor)
	{
		DrawString(bitmap, L"Point Lighting, position (0, -25, 0)", 10, 55, 35);
	}
	if (clock() <= 2 * timeFactor)
	{
		DrawString(bitmap, L"Wireframe", 10, 10, 45);
	}
	if (clock() > 2 * timeFactor && clock() <= 5 * timeFactor)
	{
		DrawString(bitmap, L"Faceted Shading, with GDI polygon()", 10, 10, 45);
	}
	if (clock() > 5 * timeFactor && clock() <= 6 * timeFactor)
	{
		DrawString(bitmap, L"Faceted Shading, without GDI polygon()", 10, 10, 45);
	}
	if (clock() > 6 * timeFactor && clock() <= 7 * timeFactor)
	{
		DrawString(bitmap, L"My attempt at Gouraud Shading", 10, 10, 45);
	}
	if (clock() > 7 * timeFactor && clock() <= 8 * timeFactor)
	{
		DrawString(bitmap, L"We will be going back to faceted with GDI to show transformations", 10, 10, 30);
	}
	if (clock() > 8 * timeFactor && clock() <= 9 * timeFactor)
	{
		DrawString(bitmap, L"Scaling in all dimensions", 10, 10, 45);
	}
	if (clock() > 9 * timeFactor && clock() <= 10 * timeFactor)
	{
		DrawString(bitmap, L"Rotation in all axes, by the same angle", 10, 10, 45);
		DrawString(bitmap, L"Scaled to avoid distortion when the model is too close to the camera", 10, 55, 30);
	}
	if (clock() > 10 * timeFactor && clock() <= 11 * timeFactor)
	{
		DrawString(bitmap, L"Translation in all axes", 10, 10, 45);
	}
}

void Rasteriser::GeneratePerspectiveMatrix(float d, float aspectRatio)
{
	Matrix perspective{ d / aspectRatio, 0, 0, 0,
						0, d, 0, 0,
						0, 0, d, 0,
						0, 0, 1, 0 };
	_perspectiveTransform = perspective;
}

void Rasteriser::GenerateViewMatrix(float d, int width, int height)
{
	Matrix view{ float(width) / 2, 0, 0, float(width) / 2,
				 0, float(-height) / 2, 0, float(height) / 2,
				 0, 0, d / 2, d / 2,
				 0, 0, 0, 1 };
	_screenTransform = view;
}

void Rasteriser::Update(Bitmap &bitmap)
{
	//Rotation while showing all rasterisation methods
	if (clock() <= 9 * timeFactor)
	{
		RotAngle += 0.02f;
		_modelTransform = Matrix::YRotationMatrix(RotAngle);
	}

	//Showcasing scaling
	if (clock() > 8 * timeFactor && clock() <= 8.5 * timeFactor)
	{
		RotAngle += 0.02f;
		scale -= 0.008f;
		_modelTransform = Matrix::ScalingMatrix(scale, scale, scale) * Matrix::YRotationMatrix(RotAngle);
	}
	if (clock() > 8.5 * timeFactor && clock() <= 9 * timeFactor)
	{
		RotAngle += 0.02f;
		scale += 0.008f;
		_modelTransform = Matrix::ScalingMatrix(scale, scale, scale) * Matrix::YRotationMatrix(RotAngle);
	}

	//Showcasing rotation in all axes, not just Y
	if (clock() > 9 * timeFactor && clock() <= 10 * timeFactor)
	{
		RotAngle += 0.02f;
		_modelTransform = Matrix::ScalingMatrix(0.7f, 0.7f, 0.7f) * Matrix::YRotationMatrix(RotAngle) * Matrix::XRotationMatrix(RotAngle) * Matrix::ZRotationMatrix(RotAngle);
	}

	//Showcasing translation, travelling in 4 lines, and ending up back at the centre
	if (clock() > 10 * timeFactor && clock() <= 10.25 * timeFactor)
	{
		translateX -= 0.1f;
		translateY += 0.1f;
		_modelTransform = Matrix::YRotationMatrix(float(M_PI)) * Matrix::TranslationMatrix(translateX, translateY, translateZ);
	}
	if (clock() > 10.25 * timeFactor && clock() <= 10.5 * timeFactor)
	{
		translateX += 0.2f;
		_modelTransform = Matrix::YRotationMatrix(float(M_PI)) * Matrix::TranslationMatrix(translateX, translateY, translateZ);
	}
	if (clock() > 10.5 * timeFactor && clock() <= 10.75 * timeFactor)
	{
		translateX -= 0.2f;
		translateY -= 0.2f;
		translateZ -= 0.1f;
		_modelTransform = Matrix::YRotationMatrix(float(M_PI)) * Matrix::TranslationMatrix(translateX, translateY, translateZ);
	}
	if (clock() > 10.75 * timeFactor && clock() <= 11 * timeFactor)
	{
		translateX += 0.1f;
		translateY += 0.1f;
		translateZ += 0.1f;
		_modelTransform = Matrix::YRotationMatrix(float(M_PI)) * Matrix::TranslationMatrix(translateX, translateY, translateZ);
	}

	//Resets the rotation angle to face the screen
	if (clock() == 11 * timeFactor)
	{
		RotAngle = M_PI;
	}

	//Continue rotating when the demo reel ends
	if (clock() > 11 * timeFactor)
	{
		RotAngle += 0.02f;
		_modelTransform = Matrix::YRotationMatrix(RotAngle);
	}

	float aspectRatio = float(float(bitmap.GetWidth()) / float(bitmap.GetHeight()));
	GeneratePerspectiveMatrix(1, aspectRatio);
	GenerateViewMatrix(1, bitmap.GetWidth(), bitmap.GetHeight());
}

void Rasteriser::Render(Bitmap &bitmap)
{
	//Clear the window to black
	bitmap.Clear(RGB(0, 0, 0));

	_model.ApplyTransformToLocalVertices(_modelTransform);

	//clock() is used to cycle through rasterisation methods
	if (clock() > 1 * timeFactor)
	{
		_model.CalculateBackfaces(_camera);
	}

	if ((clock() > 2 * timeFactor && clock() < 6 * timeFactor) || clock() > 8 * timeFactor)
	{
		_model.CalculateAmbientLighting(_ambientLight);
	}

	if (clock() > 5 * timeFactor)
	{
		_model.CalculateVertexNormals();
	}

	if ((clock() > 3 * timeFactor && clock() < 6 * timeFactor) || clock() > 8 * timeFactor)
	{
		_model.CalculateDirectionalLighting(_directionalLights);
	}

	if ((clock() > 4 * timeFactor && clock() < 6 * timeFactor) || clock() > 8 * timeFactor)
	{
		_model.CalculatePointLighting(_pointLights);
	}

	if (clock() >= 6 * timeFactor && clock() <= 8 * timeFactor)
	{
		_model.CalculateAmbientLightingVertex(_ambientLight);
		_model.CalculateDirectionalLightingVertex(_directionalLights);
		_model.CalculatePointLightingVertex(_pointLights);
	}

	_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
	_model.Sort();
	_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
	_model.Dehomogenize();
	_model.ApplyTransformToTransformedVertices(_screenTransform);

	if (clock() <= 2 * timeFactor)
	{
		DrawWireFrame(bitmap);
	}

	if ((clock() > 2 * timeFactor && clock() <= 6 * timeFactor) || clock() > 8 * timeFactor)
	{
		DrawSolidFlat(bitmap);
	}

	if (clock() > 5 * timeFactor && clock() <= 6 * timeFactor)
	{
		MyDrawSolidFlat(bitmap);
	}

	if (clock() > 6 * timeFactor && clock() <= 8 * timeFactor)
	{
		DrawGouraud(bitmap);
	}

	//Called last to ensure the string is drawn on top of the model
	DisplayAllStrings(bitmap);
}