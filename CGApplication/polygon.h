#ifndef POLYGON_H
#define POLYGON_H

#include "windowSetting.h"
#include "line.h"
#include "Weiler_Atherton.h"
#include <cmath>
#include <vector>
#include <list>
using namespace std;

struct Edge
{
	GLint yMax;
	GLfloat x;
	GLfloat m_1;// m_1=1/m
};

class polygon
{
private:
	GLint pointsNum;
	//each points' space in memory
	GLint pointSize;
	vector<GLfloat> pixels;
	vector<glm::ivec3> vertics;
	glm::vec3 lineColor;
	glm::vec3 fillColor;
	GLboolean isPolygonClose;//表示多边形是否闭合
	void findYminYmax(GLint *yMax, GLint *yMin)
	{
		GLint maxValue, minValue;
		if (vertics.size() > 0)
		{
			minValue = vertics[0].y;
			maxValue = vertics[0].y;
		}
		else
		{
			minValue = 0;
			maxValue = 0;
			return;
		}
		for (GLint i = 0; i < vertics.size(); i++)
		{
			if (maxValue < vertics[i].y)
				maxValue = vertics[i].y;
			if (minValue > vertics[i].y)
				minValue = vertics[i].y;
		}
		*yMax = maxValue;
		*yMin = minValue;
	}
	void initSortedEdgeTable(vector<list<Edge>> &SET, GLint y0)
	{
		glm::ivec3 verticBefore, verticNow, verticNext, verticNextNext;
		GLint n = vertics.size();
		for (GLint i = 0; i < n; i++)
		{
			verticBefore = vertics[(i - 1 + n) % n];
			verticNow = vertics[i];
			verticNext = vertics[(i + 1) % n];
			verticNextNext = vertics[(i + 2) % n];
			if (verticNow.y == verticNext.y)
				continue;
			else if (verticNow.y < verticNext.y)
			{
				Edge edge;
				edge.m_1 = ((GLfloat)(verticNext.x - verticNow.x)) / (verticNext.y - verticNow.y);
				edge.x = verticNow.x;
				if (verticNext.y < verticNextNext.y)
					edge.yMax = verticNext.y - 1;
				else
					edge.yMax = verticNext.y;
				pushSETEdge(SET[verticNow.y - y0], edge);
			}
			else
			{
				Edge edge;
				edge.m_1 = ((GLfloat)(verticNext.x - verticNow.x)) / (verticNext.y - verticNow.y);
				edge.x = verticNext.x;
				if (verticNow.y < verticBefore.y)
					edge.yMax = verticNow.y - 1;
				else
					edge.yMax = verticNow.y;
				pushSETEdge(SET[verticNext.y - y0], edge);
			}
		}
	}
	void pushSETEdge(list<Edge> &SETi,Edge &edge)
	{
		if (SETi.size() == 0)
			SETi.push_back(edge);
		else
		{
			for (list<Edge>::iterator ilist = SETi.begin(); ilist != SETi.end(); ilist++)
			{
				if (edge.x < ilist->x || (fabs(edge.x - ilist->x) < 1e-5 && edge.m_1 < ilist->m_1))
				{
					SETi.insert(ilist, edge);
					return;
				}
			}
			SETi.push_back(edge);
		}
	}
	void initActiveEdgeTable(vector<list<Edge>> &SET, vector<list<Edge>> &AET, GLint yMin, GLint yMax)
	{
		GLint n = yMax - yMin + 1;
		AET[0].insert(AET[0].end(), SET[0].begin(), SET[0].end());
		for (GLint i = 1; i < n; i++)
		{
			getEdgeInSET(SET, AET, i, yMin);
		}
	}
	void getEdgeInSET(vector<list<Edge>> &SET, vector<list<Edge>> &AET, GLint y, GLint yMin)
	{
		list<Edge> tempList = list<Edge>(AET[y - 1]);
		list<Edge>::iterator itempList;
		list<Edge>::iterator ilist;
		for (itempList = tempList.begin(); itempList != tempList.end(); itempList++)
		{
			itempList->x += itempList->m_1;
		}
		for (itempList = tempList.begin(); itempList != tempList.end();)
		{
			if (itempList->yMax < y + yMin)
			{
				tempList.erase(itempList++);
			}
			else
				itempList++;
		}
		if (SET[y].size() == 0)
		{
			AET[y].insert(AET[y].end(), tempList.begin(), tempList.end());
			return;
		}
		for (ilist = SET[y].begin(); ilist != SET[y].end(); ilist++)
		{
			if (ilist->yMax < y + yMin)
				continue;
			for (itempList = tempList.begin(); itempList != tempList.end(); itempList++)
			{
				if (ilist->x < itempList->x)
				{
					Edge tempEdge;
					tempEdge.x = ilist->x;
					tempEdge.m_1 = ilist->m_1;
					tempEdge.yMax = ilist->yMax;
					tempList.insert(itempList, tempEdge);
					break;
				}
			}
			if (itempList == tempList.end() && ilist->x > (--itempList)->x)
			{
				Edge tempEdge;
				tempEdge.x = ilist->x;
				tempEdge.m_1 = ilist->m_1;
				tempEdge.yMax = ilist->yMax;
				tempList.push_back(tempEdge);
			}
		}
		AET[y].insert(AET[y].end(), tempList.begin(), tempList.end());
	}
	void fillScanLine(vector<list<Edge>> &AET, GLint yMin)
	{
		GLint num=AET.size();
		for (GLint j = 0; j < num;j++)
		{
			list<Edge> &listEdge = AET[j];
			list<Edge>::iterator ilist1 = listEdge.begin();
			list<Edge>::iterator ilist2 = listEdge.begin();
			ilist2++;
			GLint n = ((GLint)listEdge.size()) / 2;
			for (GLint i = 0; i < n; i++)
			{
				for (GLint k = ilist1->x + 1; k <= ilist2->x - 1; k++)
				{
					pixels.push_back(k / (GLfloat)WIDTH_HALF);
					pixels.push_back((j+yMin) / (GLfloat)HEIGHT_HALF);
					pixels.push_back((GLfloat)0);
					pixels.push_back(fillColor.x);
					pixels.push_back(fillColor.y);
					pixels.push_back(fillColor.z);
					pointsNum++;
				}
				if (i + 1 < n)
				{
					ilist1++, ilist1++;
					ilist2++, ilist2++;
				}
			}
		}
	}
	void points2Clockwise(vector<glm::ivec3> &points)
	{
		// turn the points to clockwise
		if (points.size() < 3)
			return;
		GLint j, k, n=points.size();
		GLfloat s=0;
		for (GLint i = 0; i < n; i++)
		{
			j = (i + n - 1) % n;
			k = (i + 1) % n;
			s += points[i].y*(points[j].x - points[k].x);
		}
		if (s<0.0f)
			return;
		else
		{
			vector<glm::ivec3> pointsTemp;
			swap(points, pointsTemp);
			while (!pointsTemp.empty())
			{
				points.push_back(pointsTemp.back());
				pointsTemp.pop_back();
			}
		}
	}
	inline void clearPixels()
	{
		pointsNum = 0;
		pixels.clear();
	}
public:
	polygon()
	{
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->lineColor = glm::vec3(0.0f, 0.0f, 0.0f);
		this->fillColor = glm::vec3(0.0f, 0.0f, 0.0f);
		this->isPolygonClose = GL_FALSE;
	}
	polygon(vector<glm::ivec3> verticsPoint)
	{
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->vertics.insert(vertics.end(), verticsPoint.begin(), verticsPoint.end());
		this->lineColor = glm::vec3(0.0f, 0.0f, 0.0f);
		this->fillColor = glm::vec3(0.0f, 0.0f, 0.0f);
		this->isPolygonClose = GL_FALSE;
	}
	polygon(vector<glm::ivec3> verticsPoint, glm::ivec3 lineColor)
	{
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->vertics.insert(vertics.end(), verticsPoint.begin(), verticsPoint.end());
		this->lineColor = lineColor;
		this->fillColor = glm::vec3(0.0f, 0.0f, 0.0f);
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
	GLboolean getIsClose()
	{
		return this->isPolygonClose;
	}
	void setIsclose(GLboolean isClose)
	{
		if ((!this->isPolygonClose) && isClose)
			vertics.pop_back();
		this->isPolygonClose = isClose;
	}
	void setFillClor(glm::vec3 color)
	{
		this->fillColor=color;
	}
	vector<glm::ivec3> getVertics()
	{
		return this->vertics;
	}
	void setVertics(glm::ivec3 point,int index)
	{
		//index从1开始
		if (vertics.size() >= index)
		{
			vertics[index - 1].x = point.x;
			vertics[index - 1].y = point.y;
		}
		else
			vertics.push_back(glm::ivec3(point.x, point.y, 0));	
	}
	//isClose 代表多边形是否闭合，即是否连接最后一个顶点与第一个顶点
	void polygonUseLine()
	{
		clearPixels();
		glm::ivec3 startPoint = glm::ivec3(vertics[0].x, vertics[0].y, 0), endPoint;
		line tempLine;
		vector<GLfloat> pixelsLine;
		for (GLint i = 0; i < vertics.size(); i++)
		{
			endPoint = glm::ivec3(vertics[i].x, vertics[i].y, 0);
			tempLine = line(startPoint, endPoint, lineColor);
			tempLine.lineUseBresenham();
			pixelsLine = tempLine.getLinePixels();
			pixels.insert(pixels.end(), pixelsLine.begin(), pixelsLine.end());
			pointsNum += tempLine.getPointsNum();
			startPoint = endPoint;
		}
		if (this->isPolygonClose)
		{
			endPoint = glm::ivec3(vertics[0].x, vertics[0].y, 0);
			tempLine = line(startPoint, endPoint, lineColor);
			tempLine.lineUseBresenham();
			pixelsLine = tempLine.getLinePixels();
			pixels.insert(pixels.end(), pixelsLine.begin(), pixelsLine.end());
			pointsNum += tempLine.getPointsNum();
		}	
	}
	void fillPolygonScanLine()
	{
		if (!this->isPolygonClose)
			return;
		clearPixels();
		GLint yMax, yMin;
		findYminYmax(&yMax, &yMin);
		vector<list<Edge>> SET(yMax - yMin + 1);//Sorted Edge Table
		initSortedEdgeTable(SET, yMin);
		vector<list<Edge>> AET(yMax - yMin + 1);//Active Edge Table
		initActiveEdgeTable(SET, AET, yMin, yMax);
		fillScanLine(AET, yMin);
	}
	vector<polygon> clipWithPolygon(vector<glm::ivec3> windowVertics)
	{
		clearPixels();
		points2Clockwise(windowVertics);
		points2Clockwise(this->vertics);
		vector<vector<glm::ivec3>> clipVertics = clipPolygonUseWA(this->vertics, windowVertics);
		vector<polygon> clipPolygon;
		polygon onePolygon;
		for (GLint i=0; i<clipVertics.size(); i++)
		{
			onePolygon = polygon(clipVertics[i]);
			clipPolygon.push_back(onePolygon);
			//cout << "polygon:" << endl;
			//for (GLint j = 0; j < clipVertics[i].size(); j++)
			//	cout << "    (" << clipVertics[i][j].x << "," << clipVertics[i][j].y << ")" << endl;
		}
		return clipPolygon;
	}
};

#endif
