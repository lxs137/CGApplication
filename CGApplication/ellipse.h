#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "windowSetting.h"
#define GLEW_STATIC
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <vector>
#include <cmath>
using namespace std;

class ellipse
{
private:
	GLint pointsNum;
	//each points' space in memory
	GLint pointSize;
	vector<GLfloat> vertics;
	GLint centerX, centerY;
	GLint radiusX, radiusY;
	glm::vec3 color;
	void pushSymmetryPoint(glm::ivec3 pointPosition)
	{
		vertics.push_back((pointPosition.x + centerX) / (GLfloat)WIDTH);
		vertics.push_back((pointPosition.y + centerY) / (GLfloat)HEIGHT);
		vertics.push_back((GLfloat)pointPosition.z);
		vertics.push_back(color.x);
		vertics.push_back(color.y);
		vertics.push_back(color.z);

		vertics.push_back((-pointPosition.x + centerX) / (GLfloat)WIDTH);
		vertics.push_back((pointPosition.y + centerY) / (GLfloat)HEIGHT);
		vertics.push_back((GLfloat)pointPosition.z);
		vertics.push_back(color.x);
		vertics.push_back(color.y);
		vertics.push_back(color.z);

		vertics.push_back((pointPosition.x + centerX) / (GLfloat)WIDTH);
		vertics.push_back((-pointPosition.y + centerY) / (GLfloat)HEIGHT);
		vertics.push_back((GLfloat)pointPosition.z);
		vertics.push_back(color.x);
		vertics.push_back(color.y);
		vertics.push_back(color.z);

		vertics.push_back((-pointPosition.x + centerX) / (GLfloat)WIDTH);
		vertics.push_back((-pointPosition.y + centerY) / (GLfloat)HEIGHT);
		vertics.push_back((GLfloat)pointPosition.z);
		vertics.push_back(color.x);
		vertics.push_back(color.y);
		vertics.push_back(color.z);

		this->pointsNum += 4;
	}
public:
	ellipse()
	{
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->centerX = 0;
		this->centerY = 0;
		this->radiusX = 0;
		this->radiusY = 0;
		this->color = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	ellipse(glm::ivec3 center,GLint rx,GLint ry,glm::vec3 ellipseColor)
	{
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->centerX = center.x;
		this->centerY = center.y;
		this->radiusX = rx;
		this->radiusY = ry;
		this->color = ellipseColor;
	}
	void ellipseUseMidpoint()
	{
		vertics.clear();
		GLint x = 0, y = radiusY;
		GLfloat p1 = radiusY*radiusY - radiusX*radiusX*radiusY + radiusX*radiusX*0.25f;
		pushSymmetryPoint(glm::ivec3(x,y,0));
		
	}
};
#endif 
