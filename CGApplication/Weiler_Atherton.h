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
	GLint flag;//0��ʾ���ڱ����ö���Σ�1��ʾΪ����
	GLint index;//���ڱ����ö�����߶ε����
	GLint indexWindow;//���ڼ��ô����߶ε���� 
	GLint status;//0��ʾ�룬1��ʾ��,-1��ʾ���
};
struct PWAArray
{
	GLfloat dist;
	GLint index;//���㴢��λ��
};

vector<polygon> clipPolygonUseWA(vector<glm::ivec3> polygonVertics,vector<glm::ivec3> windowVertics)
{
	vector<InterSectPoint> injectPoints;

	//�󽻵�
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
