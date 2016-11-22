#ifndef WEILERATHERTON_H
#define WEILERATHERTON_H
#include "windowSetting.h"
#include "line.h"
#include "polygon.h"
#define GLEW_STATIC
#include <gl\glew.h>
#include <cmath>
#include <vector>
#include <algorithm>

struct InterSectPoint
{
	glm::ivec3 point;
	GLint flag;//0表示属于多边形顶点，1表示为交点
	GLint index;//属于被剪裁多边形线段的序号
	GLint indexWindow;//属于剪裁窗口线段的序号 
	GLint status;//0表示入，1表示出,-1表示清除
};
struct PointDistance
{
	GLfloat distance;//该点到线段端点的距离
	GLint indexInInject;//该点在交点数组中的序列
};

inline GLboolean distanceCompare(PointDistance p1, PointDistance p2)
{
	return p1.distance < p2.distance;
}

GLboolean pointIfInPoly(glm::ivec3 point,vector<glm::ivec3> vertics,glm::ivec2 xyMin,glm::ivec2 xyMax)
{
	if (point.x<xyMin.x || point.y<xyMin.y || point.x>xyMax.x || point.y>xyMax.y)
		return GL_FALSE;
	GLint i, j, n = vertics.size();
	GLboolean result = GL_FALSE;
	for (i = 0; i < n; i++)
	{
		j = (i + 1) % n;
		if (((vertics[i].y>point.y) != (vertics[j].y > point.y)) &&
			(point.x < (vertics[j].x - vertics[i].x)*(point.y - vertics[i].y) / (vertics[j].y - vertics[i].y) + vertics[i].x))
			result = !result;
	}
	return result;
}

vector<polygon> clipPolygonUseWA(vector<glm::ivec3> polygonVertics,vector<glm::ivec3> windowVertics)
{
	vector<InterSectPoint> injectPoints, polygonAddInject, windowAddInject;

	//求出被裁减多边形的横纵坐标极值
	glm::vec2 polygonXYMin, polygonXYMax;
	polygonXYMin.x = polygonVertics[0].x, polygonXYMin.y = polygonVertics[0].y;
	polygonXYMax.x = polygonVertics[0].x, polygonXYMax.y = polygonVertics[0].y;
	for (GLint i = 0; i < polygonVertics.size(); i++)
	{
		if (polygonXYMax.y < polygonVertics[i].y)
			polygonXYMax.y = polygonVertics[i].y;
		if (polygonXYMax.x < polygonVertics[i].x)
			polygonXYMax.x = polygonVertics[i].x;
		if (polygonXYMin.x > polygonVertics[i].x)
			polygonXYMin.x = polygonVertics[i].x;
		if (polygonXYMin.y > polygonVertics[i].y)
			polygonXYMin.y = polygonVertics[i].y;
	}

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
				if (pointIfInPoly(windowVertics[(j + 1) % numWindow], polygonVertics, polygonXYMin, polygonXYMax))
					injectPoints.push_back({ glm::ivec3(interPoint.x, interPoint.y, 0), 1, i, j, 0 });
				else
					injectPoints.push_back({ glm::ivec3(interPoint.x, interPoint.y, 0), 1, i, j, 1 });
			}
		}
	}

	//将交点插入到窗口顶点序列中
	for (GLint i = 0; i < numWindow; i++)
	{
		windowAddInject.push_back({ windowVertics[i], 0});
		vector<PointDistance> pointsDis;
		for (GLint j = 0; j < injectPoints.size(); j++)
		{
			if (injectPoints[j].indexWindow == i)
			{
				pointsDis.push_back({ sqrt(pow(injectPoints[j].point.x - windowVertics[i].x, 2)
					+ pow(injectPoints[j].point.y - windowVertics[i].y, 2)), j });
			}
		}
		sort(pointsDis.begin(), pointsDis.end(), distanceCompare);
		for (GLint k = 0; k < pointsDis.size(); k++)
		{
			windowAddInject.push_back({ injectPoints[pointsDis[k].indexInInject] });
		}
	}

	//将交点插入到被裁减多边形顶点序列中
	for (GLint i = 0; i < numPolygon; i++)
	{
		polygonAddInject.push_back({ polygonVertics[i], 0 });
		vector<PointDistance> pointsDis;
		for (GLint j = 0; j < injectPoints.size(); j++)
		{
			if (injectPoints[j].index == i)
			{
				pointsDis.push_back({ sqrt(pow(injectPoints[j].point.x - polygonVertics[i].x, 2)
					+ pow(injectPoints[j].point.y - polygonVertics[i].y, 2)), j });
			}
		}
		sort(pointsDis.begin(), pointsDis.end(), distanceCompare);
		for (GLint k = 0; k < pointsDis.size(); k++)
		{
			polygonAddInject.push_back({ injectPoints[pointsDis[k].indexInInject] });
		}
	}

	//标记出入点
	for (GLint i = 0; i < windowAddInject.size(); i++)
	{

	}
	
}

#endif // !WEILERATHERTON_H
