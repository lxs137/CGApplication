#ifndef SPLINECURVE_H
#define SPLINECURVE_H

#include "windowSetting.h"
#include "line.h"
#define GLEW_STATIC
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <cmath>
#include <vector>
using namespace std;

class spline
{
private:
	GLint pointsNum;
	//each points' space in memory
	GLint pointSize;
	vector<GLfloat> pixels;
	GLint x1, y1, x2, y2, x3, y3, x4, y4;
	glm::vec3 color;
	void pushLine(glm::ivec3 startPoint, glm::ivec3 endPoint)
	{
		line tempLine = line(startPoint, endPoint, color);
		tempLine.lineUseBresenham();
		vector<GLfloat> pixelsLine = tempLine.getLinePixels();
		pixels.insert(pixels.end(), pixelsLine.begin(), pixelsLine.end());
		pointsNum += tempLine.getPointsNum();
	}
	void pushPoint(glm::ivec3 pointPosition)
	{
		pixels.push_back(pointPosition.x / (GLfloat)WIDTH_HALF);
		pixels.push_back(pointPosition.y / (GLfloat)HEIGHT_HALF);
		pixels.push_back((GLfloat)pointPosition.z);
		pixels.push_back(color.x);
		pixels.push_back(color.y);
		pixels.push_back(color.z);
		pointsNum++;
	}
public:
	spline()
	{
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->x1 = 0, this->x2 = 0, this->x3 = 0, this->x4 = 0;
		this->y1 = 0, this->y2 = 0, this->y3 = 0, this->y4 = 0;
		this->color = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	spline(glm::ivec3 p1, glm::ivec3 p2, glm::ivec3 p3, glm::ivec3 p4, glm::vec3 splineColor)
	{
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->x1 = p1.x, this->x2 = p2.x, this->x3 = p3.x, this->x4 = p4.x;
		this->y1 = p1.y, this->y2 = p2.y, this->y3 = p3.y, this->y4 = p4.y;
		this->color = splineColor;
	}
	vector<GLfloat> getSplinePixels()
	{
		return this->pixels;
	}
	GLint getPointsNum()
	{
		return this->pointsNum;
	}
	GLint getPointSize()
	{
		return this->pointSize;
	}
	void splineUseLine()
	{
		pixels.clear();
		glm::ivec3 startPoint , endPoint;
		GLfloat b1, b2, b3, b4, x, y;
		x = x1*1.0f / 6 + x2*2.0f / 3 + x3*1.0f / 6;
		y = y1*1.0f / 6 + y2*2.0f / 3 + y3*1.0f / 6;
		startPoint = glm::ivec3((GLint)x,(GLint)y,0);
		for (GLfloat t = SPLINE_DERT_T; t <= 1.0f; t += SPLINE_DERT_T)
		{
			b1 = (1 - t)*(1 - t)*(1 - t)/6.0f;
			b2 = (3*t*t*t-6*t*t+4)/6.0f;
			b3 = (-3*t*t*t+3*t*t+3*t+1)/6.0f;
			b4 = t * t * t/6.0f;
			x = b1*x1 + b2*x2 + b3*x3 + b4*x4;
			y = b1*y1 + b2*y2 + b3*y3 + b4*y4;
			endPoint = glm::ivec3((GLint)x, (GLint)y, 0);
			pushLine(startPoint, endPoint);
			startPoint = endPoint;
		}
	}
	void showControlPoints()
	{
		pushPoint(glm::ivec3(x1, y1, 0));
		pushPoint(glm::ivec3(x2, y2, 0));
		pushPoint(glm::ivec3(x3, y3, 0));
		pushPoint(glm::ivec3(x4, y4, 0));
	}
};

#endif 
