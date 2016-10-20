#ifndef LINE_H
#define LINE_H

#include "windowSetting.h"
#define GLEW_STATIC
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <cmath>
#include <vector>
using namespace std;

class line
{
	private:
		GLint pointsNum;
		GLint pointSize;
		vector<GLfloat> vertics;
		GLint x1, y1, x2, y2;
		glm::vec3 color;
	public:
		line(glm::ivec3 p1, glm::ivec3 p2,glm::vec3 lineColor)
		{
			this->x1 = p1.x;
			this->y1 = p1.y;
			this->x2 = p2.x;
			this->y2 = p2.y;
			this->pointsNum = 0;
			this->color = lineColor;
			this->pointSize = 6 * sizeof(GLfloat);
		}
		vector<GLfloat> getLineVertics()
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
		void mergeVector(vector<glm::ivec3> pointsPosition)
		{
			vertics.clear();
			this->pointsNum = pointsPosition.size();
			for (int i = 0; i < this->pointsNum; i++)
			{
				vertics.push_back(pointsPosition[i].x / (GLfloat)WIDTH);
				vertics.push_back(pointsPosition[i].y / (GLfloat)HEIGHT);
				vertics.push_back((GLfloat)pointsPosition[i].z);
				vertics.push_back(color.x);
				vertics.push_back(color.y);
				vertics.push_back(color.z);
			}
		}
		void lineUseBresenham()
		{
			vector<glm::ivec3> pointsPosition;
			if (x1 > x2)
			{
				swap(x1, x2);
				swap(y1, y2);
			}
			GLfloat m = ((GLfloat)(y2 - y1)) / (x2 - x1);
			GLint x_next=1, y_next;
			GLint dertX = x2 - x1, dertY = y2 - y1, dert2Y = dertY + dertY,
				dert2X2Y = dertY + dertY - dertX - dertX, p0 = dertY + dertY - dertX;
			if (m>-1 && m < 1)
			{
				GLfloat y = y1;
				pointsPosition.push_back(glm::ivec3(x1, y1, 0));
				for (GLint x = x1+x_next; x != x2; x += x_next)
				{
					if (p0 <= 0)
					{
						pointsPosition.push_back(glm::ivec3(x, y, 0));
						p0 = p0 + dert2Y;
					}
					else
					{
						y += y_next;
						pointsPosition.push_back(glm::ivec3(x,y,0));
						p0 = p0 + dert2X2Y;
					}
				}
				pointsPosition.push_back(glm::ivec3(x2, y2, 0));
			}
			else
			{
				GLfloat x = x1;
 				pointsPosition.push_back(glm::ivec3(x1, y1, 0));
				for (GLint y = y1+y_next; y != y2; y += y_next)
				{
					if (p0 <= 0)
					{
						pointsPosition.push_back(glm::ivec3(x, y, 0));
						p0 = p0 + dert2Y;
					}
					else
					{
						x += x_next;
						pointsPosition.push_back(glm::ivec3(x, y, 0));
						p0 = p0 + dert2X2Y;
					}
				}
				pointsPosition.push_back(glm::ivec3(x2, y2, 0));
			}
			mergeVector(pointsPosition);
		}
		void lineUseDDA()
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
				pointsPosition.push_back(glm::ivec3(x1, y1, 0));
				for (GLint x = x1+x_dert; x != x2; x += x_dert)
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
				pointsPosition.push_back(glm::ivec3(x1, y1, 0));
				for (GLint y = y1+y_dert; y != y2; y += y_dert)
				{
					x += x_dert;
					pointsPosition.push_back(glm::ivec3(GLint(x), y, 0));
				}
				pointsPosition.push_back(glm::ivec3(x2, y2, 0));
			}
			mergeVector(pointsPosition);
		}
};





#endif