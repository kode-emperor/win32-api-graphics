#pragma once
#define _USE_MATH_DEFINES
#include <vector>
#include <time.h>
#include <math.h>
#include "Framework.h"
#include "Vertex.h"
#include "Matrix.h"
#include "Model.h"
#include "Polygon3d.h"
#include "MD2Loader.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "Vector3D.h"
#include "AmbientLight.h"
#include "PointLight.h"

class Rasteriser : public Framework
{
public:
	bool Initialise();
	void Update(Bitmap &bitmap);
	void Render(Bitmap &bitmap);

	void DrawWireFrame(Bitmap &bitmap);
	void DrawSolidFlat(Bitmap &bitmap);

	void MyDrawSolidFlat(Bitmap &bitmap);
	void FillPolygonFlat(Vertex vertex0, Vertex vertex1, Vertex vertex2, COLORREF polygonColour, Bitmap &bitmap);
	void FillBottomFlatTriangle(Vertex v0, Vertex v1, Vertex v2, COLORREF colour, Bitmap &bitmap);
	void FillTopFlatTriangle(Vertex v0, Vertex v1, Vertex v2, COLORREF colour, Bitmap &bitmap);

	void DrawGouraud(Bitmap &bitmap);
	void FillPolygonGouraud(Vertex vertex0, Vertex vertex1, Vertex vertex2, Bitmap &bitmap);
	void FillBottomFlatGouraudTriangle(Vertex v0, Vertex v1, Vertex v2, Bitmap &bitmap);
	void FillTopFlatGouraudTriangle(Vertex v0, Vertex v1, Vertex v2, Bitmap &bitmap);

	void DrawString(Bitmap &bitmap, LPCTSTR text, int xPos, int yPos, int fontSize);
	void DisplayAllStrings(Bitmap &bitmap);

	void GeneratePerspectiveMatrix(float d, float aspectRatio);
	void GenerateViewMatrix(float d, int width, int height);

private:
	Model _model;
	Camera _camera;
	Matrix _modelTransform;
	Matrix _perspectiveTransform;
	Matrix _screenTransform;
	std::vector<DirectionalLight> _directionalLights;
	AmbientLight _ambientLight;
	std::vector<PointLight> _pointLights;
};

