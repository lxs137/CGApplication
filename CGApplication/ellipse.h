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
	vector<GLfloat> pixels;
	GLint centerX, centerY;
	GLint radiusX, radiusY;
	glm::vec3 lineColor;
	glm::vec3 fillColor;
	void pushSymmetryPoint(glm::ivec3 pointPosition)
	{
		pixels.push_back((pointPosition.x + centerX) / (GLfloat)WIDTH_HALF);
		pixels.push_back((pointPosition.y + centerY) / (GLfloat)HEIGHT_HALF);
		pixels.push_back((GLfloat)pointPosition.z);
		pixels.push_back(lineColor.x);
		pixels.push_back(lineColor.y);
		pixels.push_back(lineColor.z);

		pixels.push_back((-pointPosition.x + centerX) / (GLfloat)WIDTH_HALF);
		pixels.push_back((pointPosition.y + centerY) / (GLfloat)HEIGHT_HALF);
		pixels.push_back((GLfloat)pointPosition.z);
		pixels.push_back(lineColor.x);
		pixels.push_back(lineColor.y);
		pixels.push_back(lineColor.z);

		pixels.push_back((pointPosition.x + centerX) / (GLfloat)WIDTH_HALF);
		pixels.push_back((-pointPosition.y + centerY) / (GLfloat)HEIGHT_HALF);
		pixels.push_back((GLfloat)pointPosition.z);
		pixels.push_back(lineColor.x);
		pixels.push_back(lineColor.y);
		pixels.push_back(lineColor.z);

		pixels.push_back((-pointPosition.x + centerX) / (GLfloat)WIDTH_HALF);
		pixels.push_back((-pointPosition.y + centerY) / (GLfloat)HEIGHT_HALF);
		pixels.push_back((GLfloat)pointPosition.z);
		pixels.push_back(lineColor.x);
		pixels.push_back(lineColor.y);
		pixels.push_back(lineColor.z);

		this->pointsNum += 4;
	}
	void pushFillScanLine(GLint xLeft, GLint xRight, GLint yValue)
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
	ellipse()
	{
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->centerX = 0;
		this->centerY = 0;
		this->radiusX = 0;
		this->radiusY = 0;
		this->lineColor = glm::vec3(0.0f, 0.0f, 0.0f);
		this->fillColor = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	ellipse(glm::ivec3 center,GLint rx,GLint ry,glm::vec3 ellipseColor)
	{
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->centerX = center.x;
		this->centerY = center.y;
		this->radiusX = rx;
		this->radiusY = ry;
		this->lineColor = ellipseColor;
		this->fillColor = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	vector<GLfloat> getEllipsePixels()
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
	void ellipseUseMidpoint()
	{
		clearPixels();
		GLint x = 0, y = radiusY;
		GLfloat p1 = radiusY*radiusY - radiusX*radiusX*radiusY + radiusX*radiusX*0.25f;
		pushSymmetryPoint(glm::ivec3(0, radiusY, 0));
		do
		{
			x++;
			if (p1 <= 0)
			{
				pushSymmetryPoint(glm::ivec3(x, y, 0));
				p1 = p1 + 2 * radiusY*radiusY*x + radiusY*radiusY;
			}
			else
			{
				y = y - 1;
				pushSymmetryPoint(glm::ivec3(x, y, 0));
				p1 = p1 + 2 * radiusY*radiusY*x - 2 * radiusX*radiusX*y + radiusY*radiusY;
			}
		} while (radiusY*radiusY*x <= radiusX*radiusX*y);
		p1 = radiusY*radiusY*(x*x + x + 0.25f) + radiusX*radiusX*(y - 1)*(y - 1) - radiusX*radiusX*radiusY*radiusY;
		for (y--; y > 0; y--)
		{
			if (p1 <= 0)
			{
				x = x + 1;
				pushSymmetryPoint(glm::ivec3(x, y, 0));
				p1 = p1 + 2 * radiusY*radiusY*x - 2 * radiusX*radiusX*y + radiusX*radiusX;
			}
			else
			{
				pushSymmetryPoint(glm::ivec3(x, y, 0));
				p1 = p1 - 2 * radiusX*radiusX*y + radiusX*radiusX;
			}
		}
		pushSymmetryPoint(glm::ivec3(radiusX, 0, 0));
	}
	void fillEllipseScanLine(glm::vec3 fillColor)
	{
		clearPixels();
		this->fillColor = fillColor;
		GLint x = 0, y = radiusY;
		GLfloat p1 = radiusY*radiusY - radiusX*radiusX*radiusY + radiusX*radiusX*0.25f;
		pushSymmetryPoint(glm::ivec3(0, radiusY, 0));
		do
		{
			x++;
			if (p1 <= 0)
			{
				pushSymmetryPoint(glm::ivec3(x, y, 0));
				p1 = p1 + 2 * radiusY*radiusY*x + radiusY*radiusY;
			}
			else
			{
				y = y - 1;
				pushSymmetryPoint(glm::ivec3(x, y, 0));
				p1 = p1 + 2 * radiusY*radiusY*x - 2 * radiusX*radiusX*y + radiusY*radiusY;
			}
			pushFillScanLine(-x, x, y);
			pushFillScanLine(-x, x, -y);
		} while (radiusY*radiusY*x <= radiusX*radiusX*y);
		p1 = radiusY*radiusY*(x*x + x + 0.25f) + radiusX*radiusX*(y - 1)*(y - 1) - radiusX*radiusX*radiusY*radiusY;
		for (y--; y > 0; y--)
		{
			if (p1 <= 0)
			{
				x = x + 1;
				pushSymmetryPoint(glm::ivec3(x, y, 0));
				p1 = p1 + 2 * radiusY*radiusY*x - 2 * radiusX*radiusX*y + radiusX*radiusX;
			}
			else
			{
				pushSymmetryPoint(glm::ivec3(x, y, 0));
				p1 = p1 - 2 * radiusX*radiusX*y + radiusX*radiusX;
			}
			pushFillScanLine(-x, x, y);
			pushFillScanLine(-x, x, -y);
		}
		pushSymmetryPoint(glm::ivec3(radiusX, 0, 0));
		pushFillScanLine(-radiusX, radiusX, 0);

	}
};
#endif 
