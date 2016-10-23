#ifndef POLYGON_H
#define POLYGON_H

#include "windowSetting.h"
#include "line.h"
#define GLEW_STATIC
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <cmath>
#include <vector>
using namespace std;

class polygon
{
private:
	GLint pointsNum;
	//each points' space in memory
	GLint pointSize;
	vector<GLfloat> pixels;
	vector<glm::ivec3> vertics;
	glm::vec3 color;
	void pushLine(glm::ivec3 startPoint, glm::ivec3 endPoint)
	{
		line tempLine = line(startPoint, endPoint, color);
		tempLine.lineUseBresenham();
		vector<GLfloat> pixelsLine = tempLine.getLinePixels();
		pixels.insert(pixels.end(), pixelsLine.begin(), pixelsLine.end());
		pointsNum += tempLine.getPointsNum();
	}
public:
	polygon()
	{
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->color = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	polygon(vector<glm::ivec3> verticsPoint, glm::ivec3 lineColor)
	{
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->vertics.insert(vertics.end(), verticsPoint.begin(), verticsPoint.end());
		this->color = lineColor;
	}
	vector<GLfloat> getPolygonPixels()
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
	void polygonUseLine()
	{
		glm::ivec3 startPoint=glm::ivec3(vertics[0].x,vertics[0].y,0), endPoint;
		for (GLint i = 0; i < vertics.size(); i++)
		{
			endPoint = glm::ivec3(vertics[i].x, vertics[i].y, 0);
			pushLine(startPoint, endPoint);
			startPoint = endPoint;
		}
		endPoint = glm::ivec3(vertics[0].x, vertics[0].y, 0);
		pushLine(startPoint, endPoint);
	}
};

#endif
