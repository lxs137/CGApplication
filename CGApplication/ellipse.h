#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "windowSetting.h"
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
	void pushFillScanLine(GLint xLeft, GLint xRight, GLint yValue,GLint xMin,GLint xMax,GLint yMin,GLint yMax)
	{
		if (yValue<yMin || yValue>yMax)
			return;
		else
		{
			xLeft = xLeft < xMin ? (xMin - 1) : xLeft;
			xRight = xRight < xMax ? xRight : (xMax + 1);
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
	void fillEllipseWithWindow(GLint xWMin, GLint xWMax, GLint yWMin, GLint yWMax)
	{
		GLint x = 0, y = radiusY;
		GLfloat p1 = radiusY*radiusY - radiusX*radiusX*radiusY + radiusX*radiusX*0.25f;
		pushPoint(glm::ivec3(centerX, centerY+radiusY, 0), xWMin, xWMax, yWMin, yWMax);
		pushPoint(glm::ivec3(centerX, centerY-radiusY, 0), xWMin, xWMax, yWMin, yWMax);
		do
		{
			x++;
			if (p1 <= 0)
			{
				p1 = p1 + 2 * radiusY*radiusY*x + radiusY*radiusY;
			}
			else
			{
				y = y - 1;
				p1 = p1 + 2 * radiusY*radiusY*x - 2 * radiusX*radiusX*y + radiusY*radiusY;
			}
			pushPoint(glm::ivec3(centerX + x, centerY + y, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(centerX - x, centerY + y, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(centerX + x, centerY - y, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(centerX - x, centerY - y, 0), xWMin, xWMax, yWMin, yWMax);
			//fill the Ellipse
			pushFillScanLine(centerX - x, centerX + x, centerY + y, xWMin, xWMax, yWMin, yWMax);
			pushFillScanLine(centerX - x, centerX + x, centerY - y, xWMin, xWMax, yWMin, yWMax);
		} while (radiusY*radiusY*x <= radiusX*radiusX*y);
		p1 = radiusY*radiusY*(x*x + x + 0.25f) + radiusX*radiusX*(y - 1)*(y - 1) - radiusX*radiusX*radiusY*radiusY;
		for (y--; y > 0; y--)
		{
			if (p1 <= 0)
			{
				x = x + 1;
				p1 = p1 + 2 * radiusY*radiusY*x - 2 * radiusX*radiusX*y + radiusX*radiusX;
			}
			else
			{				
				p1 = p1 - 2 * radiusX*radiusX*y + radiusX*radiusX;
			}
			pushPoint(glm::ivec3(centerX + x, centerY + y, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(centerX - x, centerY + y, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(centerX + x, centerY - y, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(centerX - x, centerY - y, 0), xWMin, xWMax, yWMin, yWMax);
			//fill the Ellipse
			pushFillScanLine(centerX - x, centerX + x, centerY + y, xWMin, xWMax, yWMin, yWMax);
			pushFillScanLine(centerX - x, centerX + x, centerY - y, xWMin, xWMax, yWMin, yWMax);
		}
		pushPoint(glm::ivec3(centerX + radiusX, centerY, 0), xWMin, xWMax, yWMin, yWMax);
		pushPoint(glm::ivec3(centerX - radiusX, centerY, 0), xWMin, xWMax, yWMin, yWMax);
		pushFillScanLine(centerX - radiusX, centerX + radiusX, centerY, xWMin, xWMax, yWMin, yWMax);
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
	void setCenter(glm::ivec3 center)
	{
		this->centerX = center.x;
		this->centerY = center.y;
	}
	void setRadius(GLint rX, GLint rY)
	{
		this->radiusX = rX;
		this->radiusY = rY;
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
	glm::ivec3 getCenter()
	{
		return glm::ivec3(centerX, centerY, 0);
	}
	glm::ivec3 getRadius()
	{
		return glm::ivec3(radiusX,radiusY,0);
	}
	void ellipseUseMidpoint()
	{
		clearPixels();
		GLint x = 0, y = radiusY;
		GLfloat p1 = radiusY*radiusY - radiusX*radiusX*radiusY + radiusX*radiusX*0.25f;
		pushPoint(glm::ivec3(centerX, centerY + radiusY, 0), -WIDTH, WIDTH, -HEIGHT, HEIGHT);
		pushPoint(glm::ivec3(centerX, centerY - radiusY, 0), -WIDTH, WIDTH, -HEIGHT, HEIGHT);
		do
		{
			x++;
			if (p1 <= 0)
			{
				p1 = p1 + 2 * radiusY*radiusY*x + radiusY*radiusY;
			}
			else
			{
				y = y - 1;
				p1 = p1 + 2 * radiusY*radiusY*x - 2 * radiusX*radiusX*y + radiusY*radiusY;
			}
			pushPoint(glm::ivec3(centerX + x, centerY + y, 0), -WIDTH, WIDTH, -HEIGHT, HEIGHT);
			pushPoint(glm::ivec3(centerX - x, centerY + y, 0), -WIDTH, WIDTH, -HEIGHT, HEIGHT);
			pushPoint(glm::ivec3(centerX + x, centerY - y, 0), -WIDTH, WIDTH, -HEIGHT, HEIGHT);
			pushPoint(glm::ivec3(centerX - x, centerY - y, 0), -WIDTH, WIDTH, -HEIGHT, HEIGHT);
		} while (radiusY*radiusY*x <= radiusX*radiusX*y);
		p1 = radiusY*radiusY*(x*x + x + 0.25f) + radiusX*radiusX*(y - 1)*(y - 1) - radiusX*radiusX*radiusY*radiusY;
		for (y--; y > 0; y--)
		{
			if (p1 <= 0)
			{
				x = x + 1;
				p1 = p1 + 2 * radiusY*radiusY*x - 2 * radiusX*radiusX*y + radiusX*radiusX;
			}
			else
			{
				p1 = p1 - 2 * radiusX*radiusX*y + radiusX*radiusX;
			}
			pushPoint(glm::ivec3(centerX + x, centerY + y, 0), -WIDTH, WIDTH, -HEIGHT, HEIGHT);
			pushPoint(glm::ivec3(centerX - x, centerY + y, 0), -WIDTH, WIDTH, -HEIGHT, HEIGHT);
			pushPoint(glm::ivec3(centerX + x, centerY - y, 0), -WIDTH, WIDTH, -HEIGHT, HEIGHT);
			pushPoint(glm::ivec3(centerX - x, centerY - y, 0), -WIDTH, WIDTH, -HEIGHT, HEIGHT);
		}
		pushPoint(glm::ivec3(centerX + radiusX, centerY, 0), -WIDTH, WIDTH, -HEIGHT, HEIGHT);
		pushPoint(glm::ivec3(centerX - radiusX, centerY, 0), -WIDTH, WIDTH, -HEIGHT, HEIGHT);
	}
	void fillEllipseScanLine(glm::vec3 fillColor)
	{
		clearPixels();
		this->fillColor = fillColor;
		/*GLint x = 0, y = radiusY;
		GLfloat p1 = radiusY*radiusY - radiusX*radiusX*radiusY + radiusX*radiusX*0.25f;
		pushPoint(glm::ivec3(0, radiusY, 0));
		do
		{
		x++;
		if (p1 <= 0)
		{
		pushPoint(glm::ivec3(x, y, 0));
		p1 = p1 + 2 * radiusY*radiusY*x + radiusY*radiusY;
		}
		else
		{
		y = y - 1;
		pushPoint(glm::ivec3(x, y, 0));
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
		pushPoint(glm::ivec3(x, y, 0));
		p1 = p1 + 2 * radiusY*radiusY*x - 2 * radiusX*radiusX*y + radiusX*radiusX;
		}
		else
		{
		pushPoint(glm::ivec3(x, y, 0));
		p1 = p1 - 2 * radiusX*radiusX*y + radiusX*radiusX;
		}
		pushFillScanLine(-x, x, y);
		pushFillScanLine(-x, x, -y);
		}
		pushPoint(glm::ivec3(radiusX, 0, 0));
		pushFillScanLine(-radiusX, radiusX, 0);*/
		fillEllipseWithWindow(-WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
	}
	void clipUseRect(glm::ivec3 winP1, glm::ivec3 winP2)
	{
		GLint xMin = winP1.x<winP2.x ? winP1.x : winP2.x, xMax = winP1.x>winP2.x ? winP1.x : winP2.x,
			yMin = winP1.y<winP2.y ? winP1.y : winP2.y, yMax = winP1.y>winP2.y ? winP1.y : winP2.y;
		//check RectWindow and Circle position
		if ((centerX + radiusX <= xMin || centerY + radiusY <= yMin)
			|| (centerX - radiusX >= xMax || centerY - radiusY >= yMax))
		{
			clearPixels();
			return;
		}
		else if (centerX - radiusX >= xMin&&centerX + radiusX <= xMax&&centerY - radiusY >= yMin&&centerY + radiusY <= yMax)
		{
			return;
		}
		clearPixels();
		fillEllipseWithWindow(xMin, xMax, yMin, yMax);
	}
};
#endif 
