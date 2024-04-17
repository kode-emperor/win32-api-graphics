#pragma once
#include <vector>
#include "Polygon3D.h"
#include "Vertex.h"
#include "Matrix.h"
#include "Camera.h"
#include <algorithm>
#include "stdafx.h"
#include "DirectionalLight.h"
#include <math.h>
#include "AmbientLight.h"
#include "PointLight.h"

class Model
{
public:
	Model();
	Model(float kaRed, float kaGreen, float kaBlue, float kdRed, float kdGreen, float kdBlue, float ksRed, float ksGreen, float ksBlue);
	Model(const Model& m);
	~Model();

	//Accessors and mutators
	const std::vector<Polygon3D>& GetPolygons() const;
	const std::vector<Vertex>& GetVertices()const;
	const std::vector<Vertex>& GetTransform()const;

	float GetKaRed() const;
	float GetKaGreen() const;
	float GetKaBlue() const;

	void SetKaRed(const float k);
	void SetKaGreen(const float k);
	void SetKaBlue(const float k);

	float GetKdRed() const;
	float GetKdGreen() const;
	float GetKdBlue() const;

	void SetKdRed(const float k);
	void SetKdGreen(const float k);
	void SetKdBlue(const float k);

	float GetKsRed() const;
	float GetKsGreen() const;
	float GetKsBlue() const;

	void SetKsRed(const float k);
	void SetKsGreen(const float k);
	void SetKsBlue(const float k);

	void SetKValues(const float kaRed, const float kaGreen, const float kaBlue, const float kdRed, const float kdGreen, const float kdBlue, const float ksRed, const float ksGreen, const float ksBlue);

	COLORREF GetColour() const;

	size_t GetPolygonCount() const;
	size_t GetVertexCount() const;

	void AddVertex(float x, float y, float z);
	void AddPolygon(int i0, int i1, int i2);

	void ApplyTransformToLocalVertices(const Matrix &transform);
	void ApplyTransformToTransformedVertices(const Matrix &transform);

	void Dehomogenize();

	void CalculateBackfaces(Camera camera);

	void Sort(void);

	void CalculateDirectionalLighting(std::vector<DirectionalLight> directionalLights);
	void CalculateAmbientLighting(AmbientLight light);
	void CalculatePointLighting(std::vector<PointLight> pointLights);

	void CalculateVertexNormals();

	void CalculateDirectionalLightingVertex(std::vector<DirectionalLight> directionalLights);
	void CalculatePointLightingVertex(std::vector<PointLight> pointLights);
	void CalculateAmbientLightingVertex(AmbientLight light);

	Model& operator= (const Model& rhs);

private:
	std::vector<Polygon3D> _polygons;
	std::vector<Vertex> _vertices;
	std::vector<Vertex> _transform;

	float ka_red;
	float ka_green;
	float ka_blue;

	float kd_red;
	float kd_green;
	float kd_blue;

	float ks_red;
	float ks_green;
	float ks_blue;

	COLORREF _colour;
};