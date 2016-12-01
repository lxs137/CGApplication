#ifndef BEZIER_H
#define BEZIER_H

#include "windowSetting.h"
#include "line.h"
#include "cubicEquation.h"
#include <glm\glm.hpp>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

class bezier
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
		vector<GLfloat> verticesLine = tempLine.getLinePixels();
		pixels.insert(pixels.end(), verticesLine.begin(),verticesLine.end());
		pointsNum += tempLine.getPointsNum();
	}
	void findIntersection(vector<GLint> x, vector<GLint> y, GLint x0, GLint yMin, GLint yMax,vector<GLfloat> &pointsT)
	{
		vector<GLfloat> realRoots;
		vector<GLfloat>::iterator iVector;
		GLfloat a, b, c, d;
		//find intersection of points at xWMin
		a = -x[0] + 3 * x[1] - 3 * x[2] + x[3], b = 3 * x[0] - 6 * x[1] + 3 * x[2],
			c = -3 * x[0] + 3 * x[1], d = x[0] - x0;
		realRoots = findRealRoot(a, b, c, d);
		for (iVector = realRoots.begin(); iVector != realRoots.end(); iVector++)
		{
			if (*iVector >= 0 && *iVector <= 1)
			{
				GLint yValue = (GLint)((-y[0] + 3 * y[1] - 3 * y[2] + y[3])*pow(*iVector, 3) +
					(3 * y[0] - 6 * y[1] + 3 * y[2])*pow(*iVector, 2) + (-3 * y[0] + 3 * y[1])*(*iVector) + y[0]);
				if (yValue >= yMin&&yValue <= yMax)
				{
					pointsT.push_back(*iVector);
				}
			}
		}
	}
	inline void clearPixels()
	{
		pointsNum = 0;
		pixels.clear();
	}
public:
	bezier()
	{
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->x1 = 0, this->x2 = 0, this->x3 = 0, this->x4 = 0;
		this->y1 = 0, this->y2 = 0, this->y3 = 0, this->y4 = 0;
		this->color = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	bezier(glm::ivec3 p1, glm::ivec3 p2, glm::ivec3 p3, glm::ivec3 p4, glm::vec3 bezierColor)
	{
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->x1 = p1.x, this->x2 = p2.x, this->x3 = p3.x, this->x4 = p4.x;
		this->y1 = p1.y, this->y2 = p2.y, this->y3 = p3.y, this->y4 = p4.y;
		this->color = bezierColor;
	}
	vector<GLfloat> getBezierPixels()
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
	vector<glm::ivec3> getControlPoints()
	{
		return{ glm::ivec3(x1, y1, 0), glm::ivec3(x2, y2, 0), 
			glm::ivec3(x3, y3, 0), glm::ivec3(x4, y4, 0) };
	}
	void setControlPoints(glm::ivec3 point,GLint index)
	{
		//index´Ó1¿ªÊ¼
		switch (index)
		{
		case 1:
			x1 = point.x, y1 = point.y;
			break;
		case 2:
			x2 = point.x, y2 = point.y;
			break;
		case 3:
			x3 = point.x, y3 = point.y;
			break;
		case 4:
			x4 = point.x, y4 = point.y;
			break;
		}
	}
	void bezierUseLine()
	{
		clearPixels();
		glm::ivec3 startPoint=glm::ivec3(x1,y1,0),endPoint;
		GLfloat a1, a2, a3, a4, x, y;
		for (GLfloat t = BEZIER_DERT_T; t <= 1.0f; t += BEZIER_DERT_T)
		{
			a1 = (1 - t)*(1 - t)*(1 - t);
			a2 = (1 - t)*(1 - t) * 3 * t;
			a3 = 3 * t * t *(1 - t);
			a4 = t * t * t;
			x=a1*x1+a2*x2+a3*x3+a4*x4;
			y=a1*y1+a2*y2+a3*y3+a4*y4;
			endPoint = glm::ivec3((GLint)x, (GLint)y, 0);
			pushLine(startPoint, endPoint);
			startPoint = endPoint;
		}
	}
	void clipUseRect(glm::ivec3 winP1, glm::ivec3 winP2)
	{
		//find controlPoint xPMin,xPMax,yPMin,yPMax
		vector<GLint> x = { x1, x2, x3, x4 };
		vector<GLint> y = { y1, y2, y3, y4 };
		GLint xPMin = x[0], xPMax = x[0], yPMin = y[0], yPMax = y[0];
		for (int i = 0; i < 4; i++)
		{
			xPMin = (x[i] < xPMin) ? x[i] : xPMin;
			yPMin = (y[i] < yPMin) ? y[i] : yPMin;
			xPMax = (x[i] > xPMax) ? x[i] : xPMax;
			yPMax = (y[i] > yPMax) ? y[i] : yPMax;
		}
		//find rectWindow xWMin,xWMax,yWMin,yWMax
		GLint xWMin = winP1.x<winP2.x ? winP1.x : winP2.x, xWMax = winP1.x>winP2.x ? winP1.x : winP2.x,
			yWMin = winP1.y<winP2.y ? winP1.y : winP2.y, yWMax = winP1.y>winP2.y ? winP1.y : winP2.y;
		if ((xPMax <= xWMin || yPMax <= yWMin)
			|| (xPMin >= xWMax || yPMin >= yWMax))
		{
			clearPixels();
			return;
		}
		else if (xPMin >= xWMin&&xPMax <= xWMax&&yPMin >= yWMin&&yPMax <= yWMax)
		{
			return;
		}
		clearPixels();
		vector<GLfloat> intersectPointsT;
		//find intersection of points at xWMin
		findIntersection(x, y, xWMin, yWMin, yWMax, intersectPointsT);
		//find intersection of points at xWMin
		findIntersection(x, y, xWMax, yWMin, yWMax, intersectPointsT);
		//find intersection of points at xWMin
		findIntersection(y, x, yWMin, xWMin, xWMax, intersectPointsT);
		//find intersection of points at xWMin
		findIntersection(y, x, yWMax, xWMin, xWMax, intersectPointsT);
		sort(intersectPointsT.begin(), intersectPointsT.end());

		if (x1 >= xWMin&&x1 <= xWMax&&y1 >= yWMin&&y1 <= yWMax)
		{
			intersectPointsT.insert(intersectPointsT.begin(), 0.0f);
		}
		if (x4 >= xWMin&&x4 <= xWMax&&y4 >= yWMin&&y4 <= yWMax)
		{
			intersectPointsT.push_back(1.0f);
		}
		for (int i = 0; (2 * i + 1) < intersectPointsT.size(); i++)
		{
			GLfloat tStart = intersectPointsT[2 * i], tEnd = intersectPointsT[2 * i + 1];
			glm::ivec3 startPoint, endPoint;
			GLfloat a1, a2, a3, a4, x, y;
			a1 = (1 - tStart)*(1 - tStart)*(1 - tStart);
			a2 = (1 - tStart)*(1 - tStart) * 3 * tStart;
			a3 = 3 * tStart * tStart *(1 - tStart);
			a4 = tStart * tStart * tStart;
			x = a1*x1 + a2*x2 + a3*x3 + a4*x4;
			y = a1*y1 + a2*y2 + a3*y3 + a4*y4;
			startPoint = glm::ivec3((GLint)x, (GLint)y, 0);
			for (GLfloat t = tStart + BEZIER_DERT_T; t <= tEnd; t += BEZIER_DERT_T)
			{
				a1 = (1 - t)*(1 - t)*(1 - t);
				a2 = (1 - t)*(1 - t) * 3 * t;
				a3 = 3 * t * t *(1 - t);
				a4 = t * t * t;
				x = a1*x1 + a2*x2 + a3*x3 + a4*x4;
				y = a1*y1 + a2*y2 + a3*y3 + a4*y4;
				endPoint = glm::ivec3((GLint)x, (GLint)y, 0);
				pushLine(startPoint, endPoint);
				startPoint = endPoint;
			}
		}
	}

};

#endif 
