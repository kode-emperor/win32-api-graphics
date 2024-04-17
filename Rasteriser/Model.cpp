#include "Model.h"

Model::Model()
{
	_polygons.clear();
	_vertices.clear();
	_transform.clear();

	ka_red = 0;
	ka_green = 0;
	ka_blue = 0;

	kd_red = 0;
	kd_green = 0;
	kd_blue = 0;

	ks_red = 0;
	ks_green = 0;
	ks_blue = 0;
}

Model::Model(float kaRed, float kaGreen, float kaBlue, float kdRed, float kdGreen, float kdBlue, float ksRed, float ksGreen, float ksBlue)
{
	_polygons.clear();
	_vertices.clear();
	_transform.clear();

	ka_red = kaRed;
	ka_green = kaGreen;
	ka_blue = kaBlue;

	kd_red = kdRed;
	kd_green = kdGreen;
	kd_blue = kdBlue;

	ks_red = ksBlue;
	ks_green = ksGreen;
	ks_blue = ksBlue;
}

//Copy Constructor
Model::Model(const Model& m)
{
	_polygons = m.GetPolygons();
	_vertices = m.GetVertices();
	_transform = m.GetTransform();

	ka_red = m.GetKaRed();
	ka_green = m.GetKaGreen();
	ka_blue = m.GetKaBlue();

	kd_red = m.GetKdRed();
	kd_green = m.GetKdGreen();
	kd_blue = m.GetKdBlue();

	ks_red = m.GetKsRed();
	ks_green = m.GetKsGreen();
	ks_blue = m.GetKsBlue();

	_colour = m.GetColour();
}

Model::~Model()
{

}

//Accessors and mutators
const std::vector<Polygon3D>& Model::GetPolygons() const
{
	return _polygons;
}

const std::vector<Vertex>& Model::GetVertices() const
{
	return _vertices;
}

const std::vector<Vertex>& Model::GetTransform() const
{
	return _transform;
}

float Model::GetKaRed() const
{
	return ka_red;
}

float Model::GetKaGreen() const
{
	return ka_green;
}

float Model::GetKaBlue() const
{
	return ka_blue;
}

void Model::SetKaRed(const float k)
{
	ka_red = k;
}

void Model::SetKaGreen(const float k)
{
	ka_green = k;
}

void Model::SetKaBlue(const float k)
{
	ka_blue = k;
}

float Model::GetKdRed() const
{
	return kd_red;
}

float Model::GetKdGreen() const
{
	return kd_green;
}

float Model::GetKdBlue() const
{
	return kd_blue;
}

void Model::SetKdRed(const float k)
{
	kd_red = k;
}

void Model::SetKdGreen(const float k)
{
	kd_green = k;
}

void Model::SetKdBlue(const float k)
{
	kd_blue = k;
}

float Model::GetKsRed() const
{
	return ks_red;
}

float Model::GetKsGreen() const
{
	return ks_green;
}

float Model::GetKsBlue() const
{
	return ks_blue;
}

void Model::SetKsRed(const float k)
{
	ks_red = k;
}

void Model::SetKsGreen(const float k)
{
	ks_green = k;
}

void Model::SetKsBlue(const float k)
{
	ks_blue = k;
}

void Model::SetKValues(const float kaRed, const float kaGreen, const float kaBlue, const float kdRed, const float kdGreen, const float kdBlue, const float ksRed, const float ksGreen, const float ksBlue)
{
	ka_red = kaRed;
	ka_green = kaGreen;
	ka_blue = kaBlue;

	kd_red = kdRed;
	kd_green = kdGreen;
	kd_blue = kdBlue;

	ks_red = ksRed;
	ks_green = ksGreen;
	ks_blue = ksBlue;
}

size_t Model::GetPolygonCount() const
{
	return _polygons.size();
}

size_t Model::GetVertexCount() const
{
	return _vertices.size();
}

COLORREF Model::GetColour() const
{
	return _colour;
}

void Model::AddVertex(float x, float y, float z)
{
	Vertex temp(x, y, z, 1);
	_vertices.push_back(temp);
}

void Model::AddPolygon(int i0, int i1, int i2)
{
	Polygon3D temp(i0, i1, i2);
	_polygons.push_back(temp);
}

void Model::ApplyTransformToLocalVertices(const Matrix &transform)
{
	_transform.clear();
	int verticesSize = int(_vertices.size());
	for (int i = 0; i < verticesSize; i++)
	{
		_transform.push_back(transform * _vertices[i]);
	}
}

void Model::ApplyTransformToTransformedVertices(const Matrix &transform)
{
	int transformSize = int(_transform.size());
	for (int i = 0; i < transformSize; i++)
	{
		_transform[i] = transform * _transform[i];
	}
}

void Model::Dehomogenize()
{
	int transformSize = int(_transform.size());
	for (int i = 0; i < transformSize; i++)
	{
		_transform[i].Dehomogenize();
	}
}

void Model::CalculateBackfaces(Camera camera)
{
	int polygonSize = int(_polygons.size());
	for (int i = 0; i < polygonSize; i++)
	{
		//Get the indices
		int index0 = _polygons[i].GetIndex(0);
		int index1 = _polygons[i].GetIndex(1);
		int index2 = _polygons[i].GetIndex(2);
		
		//Get the vertices
		Vertex vertex0 = _transform[index0];
		Vertex vertex1 = _transform[index1];
		Vertex vertex2 = _transform[index2];

		//Contructing vectors by subtracting vertices
		Vector3D vectorA = vertex1.MinusToVector(vertex0);
		Vector3D vectorB = vertex2.MinusToVector(vertex0);

		//Calculating the normal (cross product) between the two vectors
		Vector3D normal = vectorB.CrossProduct(vectorA);

		//Creating an eye-vector by subtracting the camera position from vertex0
		Vector3D eyeVector = vertex0.MinusToVector(camera.GetPos());

		bool cull;
		if (float dotProduct = normal.DotProduct(eyeVector) < 0)
		{
			//Flag this polygon for culling
			cull = true;
			_polygons[i].SetCull(cull);
		}
		else
		{
			//Flag the polygon to not be culled any more, so it can be displayed when it rotates round
			cull = false;
			_polygons[i].SetCull(cull);
		}

		//Save the normal into the polygon
		_polygons[i].SetNormal(normal);
	}
}

bool SortFunction(Polygon3D elem1, Polygon3D elem2)
{
	return elem1.GetAverageZ() > elem2.GetAverageZ();
}

void Model::Sort(void)
{
	int polygonSize = int(_polygons.size());
	for (int i = 0; i < polygonSize; i++)
	{
		//Get the indices
		int index0 = _polygons[i].GetIndex(0);
		int index1 = _polygons[i].GetIndex(1);
		int index2 = _polygons[i].GetIndex(2);

		//Get the vertices
		Vertex vertex0 = _transform[index0];
		Vertex vertex1 = _transform[index1];
		Vertex vertex2 = _transform[index2];

		_polygons[i].SetAverageZ((vertex0.GetZ() + vertex1.GetZ() + vertex2.GetZ()) / 3);
	}

	std::sort(_polygons.begin(), _polygons.end(), SortFunction);
}

void Model::CalculateDirectionalLighting(std::vector<DirectionalLight> directionalLights)
{
	//Creating the total and temp RGB values
	float totalRed;
	float totalGreen;
	float totalBlue;

	float tempRed;
	float tempGreen;
	float tempBlue;

	for (int i = 0; i < _polygons.size(); i++)
	{
		//Reset to polygon colour
		totalRed = GetRValue(_polygons[i].GetColour());
		totalGreen = GetGValue(_polygons[i].GetColour());
		totalBlue = GetBValue(_polygons[i].GetColour());

		for (int j = 0; j < directionalLights.size(); j++)
		{
			//Set temp colour to intensity of the light
			tempRed = float(directionalLights[j].GetRed());
			tempGreen = float(directionalLights[j].GetGreen());
			tempBlue = float(directionalLights[j].GetBlue());

			//Multiply the temp colour by the diffuse coefficients
			tempRed = tempRed * kd_red;
			tempGreen = tempGreen * kd_green;
			tempBlue = tempBlue * kd_blue;

			//Calculate the normal of the directional light vector
			float length = sqrt(directionalLights[j].GetDirection().GetX() * directionalLights[j].GetDirection().GetX()
							  + directionalLights[j].GetDirection().GetY() * directionalLights[j].GetDirection().GetY()
							  + directionalLights[j].GetDirection().GetZ() * directionalLights[j].GetDirection().GetZ());
			float normalX = directionalLights[j].GetDirection().GetX() / length;
			float normalY = directionalLights[j].GetDirection().GetY() / length;
			float normalZ = directionalLights[j].GetDirection().GetZ() / length;

			Vector3D diNormal(normalX, normalY, normalZ);

			//Calculate the dot product of the normal vector of the light and the normal of the polygon
			float dotProduct = diNormal.DotProduct(_polygons[i].GetNormal());

			//Multiply temp colour by dot product
			tempRed = tempRed * dotProduct;
			tempGreen = tempGreen * dotProduct;
			tempBlue = tempBlue * dotProduct;

			//Add temp colour to total colour
			totalRed += tempRed;
			totalGreen += tempGreen;
			totalBlue += tempBlue;
		}

		//Clamp the total colour values to 0-255
		if (totalRed < 0 || totalRed > 255)
		{
			if (totalRed < 0)
			{
				totalRed = 0;
			}
			else
			{
				totalRed = 255;
			}
		}
		if (totalGreen < 0 || totalGreen > 255)
		{
			if (totalGreen < 0)
			{
				totalGreen = 0;
			}
			else
			{
				totalGreen = 255;
			}
		}
		if (totalBlue < 0 || totalBlue > 255)
		{
			if (totalBlue < 0)
			{
				totalBlue = 0;
			}
			else
			{
				totalBlue = 255;
			}
		}

		//Build a COLORREF with the total RGB values
		COLORREF tempColour = RGB(totalRed, totalGreen, totalBlue);

		//Save the colour to the polygon
		_polygons[i].SetColour(tempColour);
	}
}

void Model::CalculateAmbientLighting(AmbientLight light)
{
	//Creating the total and temp RGB values
	float totalRed;
	float totalGreen;
	float totalBlue;

	float tempRed;
	float tempGreen;
	float tempBlue;

	int polygonSize = int(_polygons.size());
	for (int i = 0; i < polygonSize; i++)
	{
		//Reset to black
		totalRed = 0;
		totalGreen = 0;
		totalBlue = 0;

		//Set temp colour to intensity of the light
		tempRed = float(light.GetRed());
		tempGreen = float(light.GetGreen());
		tempBlue = float(light.GetBlue());

		//Multiply the temp colour by the diffuse coefficients
		tempRed = tempRed * ka_red;
		tempGreen = tempGreen * ka_green;
		tempBlue = tempBlue * ka_blue;

		//Add temp colour to total colour
		totalRed += tempRed;
		totalGreen += tempGreen;
		totalBlue += tempBlue;

		//Clamp the total colour values to 0-255
		if (totalRed < 0 || totalRed > 255)
		{
			if (totalRed < 0)
			{
				totalRed = 0;
			}
			else
			{
				totalRed = 255;
			}
		}
		if (totalGreen < 0 || totalGreen > 255)
		{
			if (totalGreen < 0)
			{
				totalGreen = 0;
			}
			else
			{
				totalGreen = 255;
			}
		}
		if (totalBlue < 0 || totalBlue > 255)
		{
			if (totalBlue < 0)
			{
				totalBlue = 0;
			}
			else
			{
				totalBlue = 255;
			}
		}

		//Build a COLORREF with the total RGB values
		COLORREF tempColour = RGB(totalRed, totalGreen, totalBlue);

		//Save the colour to the polygon
		_polygons[i].SetColour(tempColour);

		//Save the colour to the vertices
		_transform[_polygons[i].GetIndex(0)].SetColour(tempColour);
		_transform[_polygons[i].GetIndex(1)].SetColour(tempColour);
		_transform[_polygons[i].GetIndex(2)].SetColour(tempColour);
	}
}

void Model::CalculatePointLighting(std::vector<PointLight> pointLights)
{
	//Creating the total and temp RGB values
	float totalRed;
	float totalGreen;
	float totalBlue;

	float tempRed;
	float tempGreen;
	float tempBlue;

	int polygonSize = int(_polygons.size());
	for (int i = 0; i < polygonSize; i++)
	{
		//Reset to polygon colour
		totalRed = GetRValue(_polygons[i].GetColour());
		totalGreen = GetGValue(_polygons[i].GetColour());
		totalBlue = GetBValue(_polygons[i].GetColour());

		for (int j = 0; j < pointLights.size(); j++)
		{
			//Set temp colour to intensity of the light
			tempRed = float(pointLights[j].GetRed());
			tempGreen = float(pointLights[j].GetGreen());
			tempBlue = float(pointLights[j].GetBlue());

			//Multiply the temp colour by the diffuse coefficients
			tempRed = tempRed * kd_red;
			tempGreen = tempGreen * kd_green;
			tempBlue = tempBlue * kd_blue;

			//Calculate the vector and normal to the light
			Vertex tempVertex = _transform[_polygons[i].GetIndex(0)] - pointLights[j].GetPosition();
			Vector3D lightVector(tempVertex.GetX(), tempVertex.GetY(), tempVertex.GetZ());

			float length = sqrt(lightVector.GetX() * lightVector.GetX()
				+ lightVector.GetY() * lightVector.GetY()
				+ lightVector.GetZ() * lightVector.GetZ());
			float normalX = lightVector.GetX() / length;
			float normalY = lightVector.GetY() / length;
			float normalZ = lightVector.GetZ() / length;

			Vector3D pointNormal(normalX, normalY, normalZ);

			//Calculate the attenuation
			float a = pointLights[j].GetAttenA();
			float b = pointLights[j].GetAttenB();
			float c = pointLights[j].GetAttenC();
			float dSquared = length * length;
			float atten = 1 / (a + b * length + c * dSquared);

			//Calculate the dot product of the normal vector of the light and the normal of the polygon
			float dotProduct = pointNormal.DotProduct(_polygons[i].GetNormal());

			//Multiply temp colour by dot product
			tempRed = tempRed * dotProduct;
			tempGreen = tempGreen * dotProduct;
			tempBlue = tempBlue * dotProduct;

			//Multiply by the attenuation
			tempRed = tempRed * atten;
			tempGreen = tempGreen * atten;
			tempBlue = tempBlue * atten;

			//Add temp colour to total colour
			totalRed += tempRed;
			totalGreen += tempGreen;
			totalBlue += tempBlue;
		}

		//Clamp the total colour values to 0-255
		if (totalRed < 0 || totalRed > 255)
		{
			if (totalRed < 0)
			{
				totalRed = 0;
			}
			else
			{
				totalRed = 255;
			}
		}
		if (totalGreen < 0 || totalGreen > 255)
		{
			if (totalGreen < 0)
			{
				totalGreen = 0;
			}
			else
			{
				totalGreen = 255;
			}
		}
		if (totalBlue < 0 || totalBlue > 255)
		{
			if (totalBlue < 0)
			{
				totalBlue = 0;
			}
			else
			{
				totalBlue = 255;
			}
		}

		//Build a COLORREF with the total RGB values
		COLORREF tempColour = RGB(totalRed, totalGreen, totalBlue);

		//Save the colour to the polygon
		_polygons[i].SetColour(tempColour);
	}
}

void Model::CalculateVertexNormals()
{
	int verticesSize = int(_transform.size());
	int polygonSize = int(_polygons.size());

	//Reset the normal and contributing count for all vertices
	for (int i = 0; i < verticesSize; i++)
	{
		_transform[i].SetNormal(Vector3D(0, 0, 0));
		_transform[i].SetNormalCount(0);
	}

	for (int i = 0; i < polygonSize; i++)
	{
		//Get the indices
		int index0 = _polygons[i].GetIndex(0);
		int index1 = _polygons[i].GetIndex(1);
		int index2 = _polygons[i].GetIndex(2);

		//Add the polygon normal for the 3 vertices of the polygon, and increment the contributing count
		_transform[index0].SetNormal(_transform[index0].GetNormal() + _polygons[i].GetNormal());
		_transform[index0].IncrementNormalCount();

		_transform[index1].SetNormal(_transform[index1].GetNormal() + _polygons[i].GetNormal());
		_transform[index1].IncrementNormalCount();

		_transform[index2].SetNormal(_transform[index2].GetNormal() + _polygons[i].GetNormal());
		_transform[index2].IncrementNormalCount();
	}

	for (int i = 0; i < verticesSize; i++)
	{
		//Divide the normal by its contributing count
		_transform[i].SetNormal(_transform[i].GetNormal() / float(_transform[i].GetNormalCount()));
		//Normalise the vector
		float length = sqrt(_transform[i].GetNormal().GetX() * _transform[i].GetNormal().GetX()
						  + _transform[i].GetNormal().GetY() * _transform[i].GetNormal().GetY()
						  + _transform[i].GetNormal().GetZ() * _transform[i].GetNormal().GetZ());
		_transform[i].SetNormal(_transform[i].GetNormal() / length);
	}
}

void Model::CalculateDirectionalLightingVertex(std::vector<DirectionalLight> directionalLights)
{
	//Creating the total and temp RGB values
	float totalRed;
	float totalGreen;
	float totalBlue;

	float tempRed;
	float tempGreen;
	float tempBlue;

	int polygonSize = int(_polygons.size());
	int lightsSize = int(directionalLights.size());
	for (int i = 0; i < polygonSize; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			//Get the index
			int index = _polygons[i].GetIndex(k);

			//Reset to vertex colour
			totalRed = GetRValue(_transform[index].GetColour());
			totalGreen = GetGValue(_transform[index].GetColour());
			totalBlue = GetBValue(_transform[index].GetColour());

			for (int j = 0; j < lightsSize; j++)
			{
				//Set temp colour to intensity of the light
				tempRed = float(directionalLights[j].GetRed());
				tempGreen = float(directionalLights[j].GetGreen());
				tempBlue = float(directionalLights[j].GetBlue());

				//Multiply the temp colour by the diffuse coefficients
				tempRed = tempRed * kd_red;
				tempGreen = tempGreen * kd_green;
				tempBlue = tempBlue * kd_blue;

				//Calculate the normal of the directional light vector
				float length = sqrt(directionalLights[j].GetDirection().GetX() * directionalLights[j].GetDirection().GetX()
					+ directionalLights[j].GetDirection().GetY() * directionalLights[j].GetDirection().GetY()
					+ directionalLights[j].GetDirection().GetZ() * directionalLights[j].GetDirection().GetZ());
				float normalX = directionalLights[j].GetDirection().GetX() / length;
				float normalY = directionalLights[j].GetDirection().GetY() / length;
				float normalZ = directionalLights[j].GetDirection().GetZ() / length;

				Vector3D diNormal(normalX, normalY, normalZ);

				//Calculate the dot product of the normal vector of the light and the normal of the vertex
				float dotProduct = diNormal.DotProduct(_transform[index].GetNormal());

				//Multiply temp colour by dot product
				tempRed = tempRed * dotProduct;
				tempGreen = tempGreen * dotProduct;
				tempBlue = tempBlue * dotProduct;

				//Add temp colour to total colour
				totalRed += tempRed;
				totalGreen += tempGreen;
				totalBlue += tempBlue;
			}

			//Clamp the total colour values to 0-255
			if (totalRed < 0 || totalRed > 255)
			{
				if (totalRed < 0)
				{
					totalRed = 0;
				}
				else
				{
					totalRed = 255;
				}
			}
			if (totalGreen < 0 || totalGreen > 255)
			{
				if (totalGreen < 0)
				{
					totalGreen = 0;
				}
				else
				{
					totalGreen = 255;
				}
			}
			if (totalBlue < 0 || totalBlue > 255)
			{
				if (totalBlue < 0)
				{
					totalBlue = 0;
				}
				else
				{
					totalBlue = 255;
				}
			}

			//Build a COLORREF with the total RGB values
			COLORREF tempColour = RGB(totalRed, totalGreen, totalBlue);

			//Save the colour to the vertex
			_transform[index].SetColour(tempColour);
		}
	}
}

void Model::CalculatePointLightingVertex(std::vector<PointLight> pointLights)
{
	//Creating the total and temp RGB values
	float totalRed;
	float totalGreen;
	float totalBlue;

	float tempRed;
	float tempGreen;
	float tempBlue;

	int polygonSize = int(_polygons.size());
	int lightsSize = int(pointLights.size());
	for (int i = 0; i < polygonSize; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			//Get the index
			int index = _polygons[i].GetIndex(k);

			//Reset to vertex colour
			totalRed = GetRValue(_transform[index].GetColour());
			totalGreen = GetGValue(_transform[index].GetColour());
			totalBlue = GetBValue(_transform[index].GetColour());

			for (int j = 0; j < lightsSize; j++)
			{
				//Set temp colour to intensity of the light
				tempRed = float(pointLights[j].GetRed());
				tempGreen = float(pointLights[j].GetGreen());
				tempBlue = float(pointLights[j].GetBlue());

				//Multiply the temp colour by the diffuse coefficients
				tempRed = tempRed * kd_red;
				tempGreen = tempGreen * kd_green;
				tempBlue = tempBlue * kd_blue;

				//Calculate the vector and normal to the light
				Vertex tempVertex = _transform[index] - pointLights[j].GetPosition();
				Vector3D lightVector(tempVertex.GetX(), tempVertex.GetY(), tempVertex.GetZ());

				float length = sqrt(lightVector.GetX() * lightVector.GetX()
					+ lightVector.GetY() * lightVector.GetY()
					+ lightVector.GetZ() * lightVector.GetZ());
				float normalX = lightVector.GetX() / length;
				float normalY = lightVector.GetY() / length;
				float normalZ = lightVector.GetZ() / length;

				Vector3D pointNormal(normalX, normalY, normalZ);

				//Calculate the attenuation
				float a = pointLights[j].GetAttenA();
				float b = pointLights[j].GetAttenB();
				float c = pointLights[j].GetAttenC();
				float dSquared = length * length;
				float atten = 1 / (a + b * length + c * dSquared);

				//Calculate the dot product of the normal vector of the light and the normal of the vertex
				float dotProduct = pointNormal.DotProduct(_transform[index].GetNormal());

				//Multiply temp colour by dot product
				tempRed = tempRed * dotProduct;
				tempGreen = tempGreen * dotProduct;
				tempBlue = tempBlue * dotProduct;

				//Multiply by the attenuation
				tempRed = tempRed * atten;
				tempGreen = tempGreen * atten;
				tempBlue = tempBlue * atten;

				//Add temp colour to total colour
				totalRed += tempRed;
				totalGreen += tempGreen;
				totalBlue += tempBlue;
			}

			//Clamp the total colour values to 0-255
			if (totalRed < 0 || totalRed > 255)
			{
				if (totalRed < 0)
				{
					totalRed = 0;
				}
				else
				{
					totalRed = 255;
				}
			}
			if (totalGreen < 0 || totalGreen > 255)
			{
				if (totalGreen < 0)
				{
					totalGreen = 0;
				}
				else
				{
					totalGreen = 255;
				}
			}
			if (totalBlue < 0 || totalBlue > 255)
			{
				if (totalBlue < 0)
				{
					totalBlue = 0;
				}
				else
				{
					totalBlue = 255;
				}
			}

			//Build a COLORREF with the total RGB values
			COLORREF tempColour = RGB(totalRed, totalGreen, totalBlue);

			//Save the colour to the vertex
			_transform[index].SetColour(tempColour);
		}
	}
}

void Model::CalculateAmbientLightingVertex(AmbientLight light)
{
	{
		//Creating the total and temp RGB values
		float totalRed;
		float totalGreen;
		float totalBlue;

		float tempRed;
		float tempGreen;
		float tempBlue;

		int verticesSize = int(_transform.size());
		for (int i = 0; i < verticesSize; i++)
		{
			//Reset to black
			totalRed = 0;
			totalGreen = 0;
			totalBlue = 0;

			//Set temp colour to intensity of the light
			tempRed = float(light.GetRed());
			tempGreen = float(light.GetGreen());
			tempBlue = float(light.GetBlue());

			//Multiply the temp colour by the diffuse coefficients
			tempRed = tempRed * ka_red;
			tempGreen = tempGreen * ka_green;
			tempBlue = tempBlue * ka_blue;

			//Add temp colour to total colour
			totalRed += tempRed;
			totalGreen += tempGreen;
			totalBlue += tempBlue;

			//Clamp the total colour values to 0-255
			if (totalRed < 0 || totalRed > 255)
			{
				if (totalRed < 0)
				{
					totalRed = 0;
				}
				else
				{
					totalRed = 255;
				}
			}
			if (totalGreen < 0 || totalGreen > 255)
			{
				if (totalGreen < 0)
				{
					totalGreen = 0;
				}
				else
				{
					totalGreen = 255;
				}
			}
			if (totalBlue < 0 || totalBlue > 255)
			{
				if (totalBlue < 0)
				{
					totalBlue = 0;
				}
				else
				{
					totalBlue = 255;
				}
			}

			//Build a COLORREF with the total RGB values
			COLORREF tempColour = RGB(totalRed, totalGreen, totalBlue);

			//Save the colour to the vertex
			_transform[i].SetColour(tempColour);
		}
	}
}

Model& Model::operator=(const Model& rhs)
{
	//Only do the assignment if we are not assigning
	//to ourselves
	if (this != &rhs)
	{
		_polygons = rhs.GetPolygons();
		_vertices = rhs.GetVertices();
		_transform = rhs.GetTransform();

		ka_red = rhs.GetKaRed();
		ka_green = rhs.GetKaGreen();
		ka_blue = rhs.GetKaBlue();

		kd_red = rhs.GetKdRed();
		kd_green = rhs.GetKdGreen();
		kd_blue = rhs.GetKdBlue();

		ks_red = rhs.GetKsRed();
		ks_green = rhs.GetKsGreen();
		ks_blue = rhs.GetKsBlue();

		_colour = rhs.GetColour();
	}
	return *this;
}