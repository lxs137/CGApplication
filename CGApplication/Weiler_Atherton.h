#ifndef WEILERATHERTON_H
#define WEILERATHERTON_H
#include "windowSetting.h"
#include "line.h"
#include "polygon.h"
#define GLEW_STATIC
#include <gl\glew.h>
#include <cmath>
#include <vector>
struct InterSectPoint
{
	glm::ivec3 point;
	GLint flag;//0表示属于被剪裁多边形，1表示为交点
	GLint index;//属于被剪裁多边形线段的序号
	GLint indexWindow;//属于剪裁窗口线段的序号 
	GLint status;//0表示入，1表示出,-1表示清除
};
struct PWAArray
{
	GLfloat dist;
	GLint index;//交点储存位置
};

vector<polygon> clipPolygonUseWA(vector<glm::ivec3> polygonVertics,vector<glm::ivec3> windowVertics)
{
	vector<InterSectPoint> injectPoints;

	//求交点
	GLint numPolygon = polygonVertics.size(), numWindow = windowVertics.size();
	line linePolygon=line(), lineWindow=line();
	glm::ivec3 interPoint;
	for (GLint i = 0; i < numPolygon; i++)
	{
		linePolygon.setPoint(polygonVertics[i % numPolygon], polygonVertics[(i + 1) % numPolygon]);
		for (GLint j = 0; j < numWindow; j++)
		{
			lineWindow.setPoint(windowVertics[j % numWindow], windowVertics[(j + 1) % numWindow]);
			if (linePolygon.findLineIntersect(lineWindow, interPoint))
			{
				InterSectPoint pTemp;
				pTemp.point = glm::ivec3(interPoint.x, interPoint.y, 0);
				pTemp.flag = 1;
				pTemp.index = i;
				pTemp.indexWindow = j;
				injectPoints.push_back(pTemp);
			}
		}
	}
}

#endif // !WEILERATHERTON_H
