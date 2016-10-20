#ifndef LINE_H
#define LINE_H

#include "windowSetting.h"
#define GLEW_STATIC
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <cmath>
#include <vector>
using namespace std;

vector<GLfloat> vector2Array(vector<glm::ivec3> pointsPosition, glm::vec3 lineColor);
void lineBresenham(GLint x1, GLint y1, GLint x2, GLint y2);
vector<GLfloat> lineDDA(GLint x1, GLint y1, GLint x2, GLint y2,glm::vec3 lineColor);

vector<GLfloat> lineDDA(GLint x1, GLint y1, GLint x2, GLint y2,glm::vec3 lineColor)
{
	vector<glm::ivec3> pointsPosition;
	GLfloat x_dert, y_dert;
	GLfloat m = ((GLfloat)(y2 - y1)) / (x2 - x1);
	if (m > 0)
	{
		if (m > 1 && x2 > x1) y_dert = 1, x_dert = 1 / m;
		else if (m > 1 && x2 < x1) y_dert = -1, x_dert = -1 / m;
		else if (m < 1 && x2 > x1) y_dert = m, x_dert = 1;
		else if (m < 1 && x2 < x1) y_dert = -m, x_dert = -1;
	}
	else
	{
		if (m < -1 && x2 > x1) y_dert = -1, x_dert = -1 / m;
		else if (m < -1 && x2 < x1) y_dert = 1, x_dert = 1 / m;
		else if (m > -1 && x2 > x1) y_dert = m, x_dert = 1;
		else if (m > -1 && x2 < x1) y_dert = -m, x_dert = -1;
	}
	if (m>-1 && m < 1)
	{
		GLfloat y = y1;
		x_dert = (GLint)x_dert;
		for (GLint x = x1; x < x2; x += x_dert)
		{
			y += y_dert;
			pointsPosition.push_back(glm::ivec3(x, GLint(y), 0));
		}
		pointsPosition.push_back(glm::ivec3(x2, y2, 0));
	}
	else
	{
		GLfloat x = x1;
		y_dert = (GLint)y_dert;
		for (GLint y = y1; y < y2; y += y_dert)
		{
			x += x_dert;
			pointsPosition.push_back(glm::ivec3(GLint(x), y, 0));
		}
		pointsPosition.push_back(glm::ivec3(x2, y2, 0));
	}

	return vector2Array(pointsPosition, lineColor);
}
void lineBresenham(GLint x1, GLint y1, GLint x2, GLint y2)
{
	vector<glm::ivec3> pointGrids;
	GLfloat m = (y2 - y1) / (x2 - x1);
	GLint x_next, y_next;
	if (m > 0)
	{
		if (m > 1 && x2 > x1) y_next = 1, x_next = 1;
		else if (m > 1 && x2 < x1) y_next = -1, x_next = -1;
		else if (m < 1 && x2 > x1) y_next = 1, x_next = 1;
		else if (m < 1 && x2 < x1) y_next = -1, x_next = -1;
	}
	else
	{
		if (m < -1 && x2 > x1) y_next = -1, x_next = 1;
		else if (m < -1 && x2 < x1) y_next = 1, x_next = -1;
		else if (m > -1 && x2 > x1) y_next = -1, x_next = 1;
		else if (m > -1 && x2 < x1) y_next = 1, x_next = -1;
	}
	GLint dertX = x2 - x1, dertY = y2 - y1, dert2Y = dertY + dertY,
		dert2X2Y = dertY + dertY - dertX - dertX, p0 = dertY + dertY - dertX;

}
vector<GLfloat> vector2Array(vector<glm::ivec3> pointsPosition, glm::vec3 lineColor)
{
	vector<GLfloat> vertics;
	int pointsNum = pointsPosition.size();
	for (int i = 0; i < pointsNum; i++)
	{
		vertics.push_back(pointsPosition[i].x / (GLfloat)WIDTH);
		vertics.push_back(pointsPosition[i].y / (GLfloat)HEIGHT);
		vertics.push_back((GLfloat)pointsPosition[i].z);
		vertics.push_back(lineColor.x);
		vertics.push_back(lineColor.y);
		vertics.push_back(lineColor.z);
	}
	return vertics;
}
#endif