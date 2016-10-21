#ifndef CIRCLE_H
#define CIECLE_H

#include "windowSetting.h"
#define GLEW_STATIC
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <vector>
#include <cmath>
using namespace std;

class circle
{
private:
	GLint pointsNum;
	//each points' space in memory
	GLint pointSize;
	vector<GLfloat> vertics;
	GLint centerX, centerY,radius;
	glm::vec3 color;
	void pushSymmetryPoint(glm::ivec3 pointPosition)
	{
		vertics.push_back((pointPosition.x +centerX)/ (GLfloat)WIDTH);
		vertics.push_back((pointPosition.y +centerY)/ (GLfloat)HEIGHT);
		vertics.push_back((GLfloat)pointPosition.z);
		vertics.push_back(color.x);
		vertics.push_back(color.y);
		vertics.push_back(color.z);

		vertics.push_back((pointPosition.y +centerX)/ (GLfloat)WIDTH);
		vertics.push_back((pointPosition.x +centerY)/ (GLfloat)HEIGHT);
		vertics.push_back((GLfloat)pointPosition.z);
		vertics.push_back(color.x);
		vertics.push_back(color.y);
		vertics.push_back(color.z);

		vertics.push_back((-pointPosition.x +centerX)/ (GLfloat)WIDTH);
		vertics.push_back((pointPosition.y +centerY)/ (GLfloat)HEIGHT);
		vertics.push_back((GLfloat)pointPosition.z);
		vertics.push_back(color.x);
		vertics.push_back(color.y);
		vertics.push_back(color.z);

		vertics.push_back((-pointPosition.y +centerX)/ (GLfloat)WIDTH);
		vertics.push_back((pointPosition.x +centerY)/ (GLfloat)HEIGHT);
		vertics.push_back((GLfloat)pointPosition.z);
		vertics.push_back(color.x);
		vertics.push_back(color.y);
		vertics.push_back(color.z);

		vertics.push_back((pointPosition.x +centerX)/ (GLfloat)WIDTH);
		vertics.push_back((-pointPosition.y +centerY) / (GLfloat)HEIGHT);
		vertics.push_back((GLfloat)pointPosition.z);
		vertics.push_back(color.x);
		vertics.push_back(color.y);
		vertics.push_back(color.z);

		vertics.push_back((pointPosition.y +centerX) / (GLfloat)WIDTH);
		vertics.push_back((-pointPosition.x +centerY)/ (GLfloat)HEIGHT);
		vertics.push_back((GLfloat)pointPosition.z);
		vertics.push_back(color.x);
		vertics.push_back(color.y);
		vertics.push_back(color.z);

		vertics.push_back((-pointPosition.x +centerX) / (GLfloat)WIDTH);
		vertics.push_back((-pointPosition.y +centerY)/ (GLfloat)HEIGHT);
		vertics.push_back((GLfloat)pointPosition.z);
		vertics.push_back(color.x);
		vertics.push_back(color.y);
		vertics.push_back(color.z);

		vertics.push_back((-pointPosition.y +centerX)/ (GLfloat)WIDTH);
		vertics.push_back((-pointPosition.x +centerY)/ (GLfloat)HEIGHT);
		vertics.push_back((GLfloat)pointPosition.z);
		vertics.push_back(color.x);
		vertics.push_back(color.y);
		vertics.push_back(color.z);
		this->pointsNum += 8;
	}
public:
	circle(glm::ivec3 center,GLint r,glm::vec3 circleColor)
	{
		this->centerX = center.x;
		this->centerY = center.y;
		this->radius = r;
		this->color = circleColor;
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
	}
	vector<GLfloat> getCircleVertics()
	{
		return this->vertics;
	}
	GLint getPointsNum()
	{
		return this->pointsNum;
	}
	GLint getPointSize()
	{
		return this->pointSize;
	}
	void circleUseMidpoint()
	{
		GLint x = 0, y = radius;
		GLfloat p0 = 5.0f / 4 - radius;
		pushSymmetryPoint(glm::ivec3(x,y,0));
		for (x = 1; x < y; x++)
		{
			if (p0 < 0)
			{
				pushSymmetryPoint(glm::ivec3(x,y,0));
				p0 = p0 + x + x  + 1;
			}
			else
			{
				y--;
				pushSymmetryPoint(glm::ivec3(x, y, 0));
				p0 = p0 + x + x - y - y + 1;
			}
		}
		pushSymmetryPoint(glm::ivec3(x,y,0));
	}
};

#endif