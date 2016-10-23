#ifndef BEZIER_H
#define BEZIER_H

#include "windowSetting.h"
#include "line.h"
#define GLEW_STATIC
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <cmath>
#include <vector>
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
	void bezierUseLine()
	{
		pixels.clear();
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
};

#endif 
