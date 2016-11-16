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
	vector<GLfloat> pixels;
	GLint centerX, centerY,radius;
	glm::vec3 lineColor;
	glm::vec3 fillColor;
	void pushSymmetryPoint(glm::ivec3 pointPosition)
	{
		pixels.push_back((pointPosition.x +centerX)/ (GLfloat)WIDTH_HALF);
		pixels.push_back((pointPosition.y +centerY)/ (GLfloat)HEIGHT_HALF);
		pixels.push_back((GLfloat)pointPosition.z);
		pixels.push_back(lineColor.x);
		pixels.push_back(lineColor.y);
		pixels.push_back(lineColor.z);

		pixels.push_back((pointPosition.y +centerX)/ (GLfloat)WIDTH_HALF);
		pixels.push_back((pointPosition.x +centerY)/ (GLfloat)HEIGHT_HALF);
		pixels.push_back((GLfloat)pointPosition.z);
		pixels.push_back(lineColor.x);
		pixels.push_back(lineColor.y);
		pixels.push_back(lineColor.z);

		pixels.push_back((-pointPosition.x +centerX)/ (GLfloat)WIDTH_HALF);
		pixels.push_back((pointPosition.y +centerY)/ (GLfloat)HEIGHT_HALF);
		pixels.push_back((GLfloat)pointPosition.z);
		pixels.push_back(lineColor.x);
		pixels.push_back(lineColor.y);
		pixels.push_back(lineColor.z);

		pixels.push_back((-pointPosition.y +centerX)/ (GLfloat)WIDTH_HALF);
		pixels.push_back((pointPosition.x +centerY)/ (GLfloat)HEIGHT_HALF);
		pixels.push_back((GLfloat)pointPosition.z);
		pixels.push_back(lineColor.x);
		pixels.push_back(lineColor.y);
		pixels.push_back(lineColor.z);

		pixels.push_back((pointPosition.x +centerX)/ (GLfloat)WIDTH_HALF);
		pixels.push_back((-pointPosition.y +centerY) / (GLfloat)HEIGHT_HALF);
		pixels.push_back((GLfloat)pointPosition.z);
		pixels.push_back(lineColor.x);
		pixels.push_back(lineColor.y);
		pixels.push_back(lineColor.z);

		pixels.push_back((pointPosition.y +centerX) / (GLfloat)WIDTH_HALF);
		pixels.push_back((-pointPosition.x +centerY)/ (GLfloat)HEIGHT_HALF);
		pixels.push_back((GLfloat)pointPosition.z);
		pixels.push_back(lineColor.x);
		pixels.push_back(lineColor.y);
		pixels.push_back(lineColor.z);

		pixels.push_back((-pointPosition.x +centerX) / (GLfloat)WIDTH_HALF);
		pixels.push_back((-pointPosition.y +centerY)/ (GLfloat)HEIGHT_HALF);
		pixels.push_back((GLfloat)pointPosition.z);
		pixels.push_back(lineColor.x);
		pixels.push_back(lineColor.y);
		pixels.push_back(lineColor.z);

		pixels.push_back((-pointPosition.y +centerX)/ (GLfloat)WIDTH_HALF);
		pixels.push_back((-pointPosition.x +centerY)/ (GLfloat)HEIGHT_HALF);
		pixels.push_back((GLfloat)pointPosition.z);
		pixels.push_back(lineColor.x);
		pixels.push_back(lineColor.y);
		pixels.push_back(lineColor.z);
		this->pointsNum += 8;
	}
	void pushFillScanLine(GLint xLeft,GLint xRight,GLint yValue)
	{
		xLeft += centerX;
		xRight += centerX;
		yValue += centerY;
		for (GLint k = xLeft + 1; k <= xRight - 1; k++)
		{
			pixels.push_back(k / (GLfloat)WIDTH_HALF);
			pixels.push_back((yValue) / (GLfloat)HEIGHT_HALF);
			pixels.push_back((GLfloat)0);
			pixels.push_back(fillColor.x);
			pixels.push_back(fillColor.y);
			pixels.push_back(fillColor.z);
			pointsNum++;
		}
	}
	inline void clearPixels()
	{
		pointsNum = 0;
		pixels.clear();
	}
public:
	circle()
	{
		this->centerX = 0;
		this->centerY = 0;
		this->radius = 0;
		this->lineColor = glm::vec3(0.0f,0.0f,0.0f);
		this->fillColor = glm::vec3(0.0f, 0.0f, 0.0f);
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
	}
	circle(glm::ivec3 center,GLint r,glm::vec3 circleColor)
	{
		this->centerX = center.x;
		this->centerY = center.y;
		this->radius = r;
		this->lineColor = circleColor;
		this->fillColor = glm::vec3(0.0f, 0.0f, 0.0f);
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
	}
	vector<GLfloat> getCirclePixels()
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
	void circleUseMidpoint()
	{
		clearPixels();
		GLint x = 0, y = radius;
		GLfloat p0 = 5.0f / 4 - radius;
		pushSymmetryPoint(glm::ivec3(x,y,0));
		for (x = 1; x < y; x++)
		{
			if (p0 < 0)
			{
				pushSymmetryPoint(glm::ivec3(x, y, 0));
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
	void fillCircleScanLine(glm::vec3 fillColor)
	{
		this->fillColor = fillColor;
		clearPixels();
		GLint x = 0, y = radius;
		GLfloat p0 = 5.0f / 4 - radius;
		pushSymmetryPoint(glm::ivec3(x, y, 0));
		pushFillScanLine(-y, y, x);
		for (x = 1; x < y; x++)
		{
			if (p0 < 0)
			{
				pushSymmetryPoint(glm::ivec3(x, y, 0));
				p0 = p0 + x + x + 1;
			}
			else
			{
				y--;
				pushSymmetryPoint(glm::ivec3(x, y, 0));
				p0 = p0 + x + x - y - y + 1;
			}
			//Fill the Circle with ScanLine
			pushFillScanLine(-x, x, y);
			pushFillScanLine(-x, x, -y);
			pushFillScanLine(-y, y, x);
			pushFillScanLine(-y, y, -x);
		}
		pushSymmetryPoint(glm::ivec3(x, y, 0));
	}
	void clipUseRect(glm::ivec3 winP1, glm::ivec3 winP2)
	{

	}
};

#endif