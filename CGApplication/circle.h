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
	GLint centerX, centerY, radius;
	glm::vec3 lineColor;
	glm::vec3 fillColor;
	void pushPoint(glm::ivec3 pointPosition,GLint xMin,GLint xMax,GLint yMin,GLint yMax)
	{
		//clip point with window(xMin-xMax,yMin-yMax)
		if (pointPosition.x >= xMin && pointPosition.x <= xMax
			&& pointPosition.y >= yMin && pointPosition.y <= yMax)
		{
			pixels.push_back(pointPosition.x / (GLfloat)WIDTH_HALF);
			pixels.push_back(pointPosition.y / (GLfloat)HEIGHT_HALF);
			pixels.push_back((GLfloat)pointPosition.z);
			pixels.push_back(lineColor.x);
			pixels.push_back(lineColor.y);
			pixels.push_back(lineColor.z);
			this->pointsNum++;
		}
	}
	void pushFillScanLine(GLint xLeft, GLint xRight, GLint yValue, GLint xMin, GLint xMax, GLint yMin, GLint yMax)
	{
		if (yValue<yMin || yValue>yMax)
			return;
		else
		{
			xLeft = xLeft < xMin ? (xMin-1) : xLeft;
			xRight = xRight < xMax ? xRight : (xMax+1);
		}
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
	void fillCircleWithWindow(GLint xWMin, GLint xWMax, GLint yWMin, GLint yWMax)
	{
		GLint x = 0, y = radius;
		GLfloat p0 = 5.0f / 4 - radius;
		pushPoint(glm::ivec3(centerX, radius + centerY, 0), xWMin, xWMax, yWMin, yWMax);
		pushPoint(glm::ivec3(centerX, -radius + centerY, 0), xWMin, xWMax, yWMin, yWMax);
		pushFillScanLine(-radius + centerX, radius + centerX, centerY, xWMin, xWMax, yWMin, yWMax);
		for (x = 1; x < y; x++)
		{
			if (p0 < 0)
			{
				p0 = p0 + x + x + 1;
			}
			else
			{
				y--;
				p0 = p0 + x + x - y - y + 1;
			}
			pushPoint(glm::ivec3(x + centerX, y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(-x + centerX, y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(x + centerX, -y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(-x + centerX, -y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(y + centerX, x + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(y + centerX, -x + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(-y + centerX, x + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(-y + centerX, -x + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			//Fill the Circle with ScanLine
			pushFillScanLine(-x + centerX, x + centerX, y + centerY, xWMin, xWMax, yWMin, yWMax);
			pushFillScanLine(-x + centerX, x + centerX, -y + centerY, xWMin, xWMax, yWMin, yWMax);
			pushFillScanLine(-y + centerX, y + centerX, x + centerY, xWMin, xWMax, yWMin, yWMax);
			pushFillScanLine(-y + centerX, y + centerX, -x + centerY, xWMin, xWMax, yWMin, yWMax);
		}
		pushPoint(glm::ivec3(x + centerX, y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
		pushPoint(glm::ivec3(x + centerX, -y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
		pushPoint(glm::ivec3(-x + centerX, y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
		pushPoint(glm::ivec3(-x + centerX, -y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
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
		pushPoint(glm::ivec3(centerX, radius + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
		pushPoint(glm::ivec3(centerX, -radius + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
		pushPoint(glm::ivec3(radius + centerX, centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
		pushPoint(glm::ivec3(-radius + centerX, centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
		for (x = 1; x < y; x++)
		{
			if (p0 < 0)
			{
				p0 = p0 + x + x  + 1;
			}
			else
			{
				y--;
				p0 = p0 + x + x - y - y + 1;
			}
			pushPoint(glm::ivec3(x + centerX, y + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
			pushPoint(glm::ivec3(-x + centerX, y + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
			pushPoint(glm::ivec3(x + centerX, -y + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
			pushPoint(glm::ivec3(-x + centerX, -y + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
			pushPoint(glm::ivec3(y + centerX, x + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
			pushPoint(glm::ivec3(y + centerX, -x + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
			pushPoint(glm::ivec3(-y + centerX, x + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
			pushPoint(glm::ivec3(-y + centerX, -x + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
		}
		pushPoint(glm::ivec3(x + centerX, y + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
		pushPoint(glm::ivec3(x + centerX, -y + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
		pushPoint(glm::ivec3(-x + centerX, y + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
		pushPoint(glm::ivec3(-x + centerX, -y + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
	}
	void fillCircleScanLine(glm::vec3 fillColor)
	{
		this->fillColor = fillColor;
		clearPixels();
		fillCircleWithWindow(-WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
	}
	void clipUseRect(glm::ivec3 winP1, glm::ivec3 winP2)
	{
		GLint xMin = winP1.x<winP2.x ? winP1.x : winP2.x, xMax = winP1.x>winP2.x ? winP1.x : winP2.x,
			yMin = winP1.y<winP2.y ? winP1.y : winP2.y, yMax = winP1.y>winP2.y ? winP1.y : winP2.y;
		//check RectWindow and Circle position
		if ((centerX + radius <= xMin || centerY + radius <= yMin)
			|| (centerX - radius >= xMax || centerY - radius >= yMax))
		{
			clearPixels();
			return;
		}
		else if (centerX - radius >= xMin&&centerX + radius <= xMax&&centerY - radius >= yMin&&centerY + radius <= yMax)
		{
			return;
		}
		clearPixels();
		fillCircleWithWindow(xMin, xMax, yMin, yMax);
	}
};

#endif