#ifndef CIRCLE_H
#define CIECLE_H

#include "windowSetting.h"
#include <glm\glm.hpp>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
using namespace std;

class circle
{
private:
	GLint pointsNum;
	//each points' space in memory
	GLint pointSize;
	vector<GLfloat> pixels;
	GLint centerX, centerY, radius;
	glm::vec3 lineColor;
	glm::vec3 fillColor;
	vector<glm::ivec3> controlPoints;
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
	void pushFillScanLine(GLint xLeft, GLint xRight, GLint yValue, GLint xMin, GLint xMax, GLint yMin, GLint yMax)
	{
		if (yValue<yMin || yValue>yMax)
			return;
		else
		{
			xLeft = xLeft < xMin ? (xMin-1) : xLeft;
			xRight = xRight < xMax ? xRight : (xMax+1);
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
	void fillCircleWithWindow(GLint xWMin, GLint xWMax, GLint yWMin, GLint yWMax)
	{
		GLint x = 0, y = radius;
		GLfloat p0 = 5.0f / 4 - radius;
		pushPoint(glm::ivec3(centerX, radius + centerY, 0), xWMin, xWMax, yWMin, yWMax);
		pushPoint(glm::ivec3(centerX, -radius + centerY, 0), xWMin, xWMax, yWMin, yWMax);
		pushFillScanLine(-radius + centerX, radius + centerX, centerY, xWMin, xWMax, yWMin, yWMax);
		for (x = 1; x < y; x++)
		{
			if (p0 < 0)
			{
				p0 = p0 + x + x + 1;
			}
			else
			{
				y--;
				p0 = p0 + x + x - y - y + 1;
			}
			pushPoint(glm::ivec3(x + centerX, y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(-x + centerX, y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(x + centerX, -y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(-x + centerX, -y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(y + centerX, x + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(y + centerX, -x + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(-y + centerX, x + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(-y + centerX, -x + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			//Fill the Circle with ScanLine
			pushFillScanLine(-x + centerX, x + centerX, y + centerY, xWMin, xWMax, yWMin, yWMax);
			pushFillScanLine(-x + centerX, x + centerX, -y + centerY, xWMin, xWMax, yWMin, yWMax);
			pushFillScanLine(-y + centerX, y + centerX, x + centerY, xWMin, xWMax, yWMin, yWMax);
			pushFillScanLine(-y + centerX, y + centerX, -x + centerY, xWMin, xWMax, yWMin, yWMax);
		}
		pushPoint(glm::ivec3(x + centerX, y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
		pushPoint(glm::ivec3(x + centerX, -y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
		pushPoint(glm::ivec3(-x + centerX, y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
		pushPoint(glm::ivec3(-x + centerX, -y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
	}
	void midpointCircleWithWindow(GLint xWMin, GLint xWMax, GLint yWMin, GLint yWMax)
	{
		GLint x = 0, y = radius;
		GLfloat p0 = 5.0f / 4 - radius;
		pushPoint(glm::ivec3(centerX, radius + centerY, 0), xWMin, xWMax, yWMin, yWMax);
		pushPoint(glm::ivec3(centerX, -radius + centerY, 0), xWMin, xWMax, yWMin, yWMax);
		for (x = 1; x < y; x++)
		{
			if (p0 < 0)
			{
				p0 = p0 + x + x + 1;
			}
			else
			{
				y--;
				p0 = p0 + x + x - y - y + 1;
			}
			pushPoint(glm::ivec3(x + centerX, y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(-x + centerX, y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(x + centerX, -y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(-x + centerX, -y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(y + centerX, x + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(y + centerX, -x + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(-y + centerX, x + centerY, 0), xWMin, xWMax, yWMin, yWMax);
			pushPoint(glm::ivec3(-y + centerX, -x + centerY, 0), xWMin, xWMax, yWMin, yWMax);
		}
		pushPoint(glm::ivec3(x + centerX, y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
		pushPoint(glm::ivec3(x + centerX, -y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
		pushPoint(glm::ivec3(-x + centerX, y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
		pushPoint(glm::ivec3(-x + centerX, -y + centerY, 0), xWMin, xWMax, yWMin, yWMax);
	}
	inline void clearPixels()
	{
		pointsNum = 0;
		pixels.clear();
	}
public:
	circle()
	{
		this->centerX = 0;
		this->centerY = 0;
		this->radius = 0;
		this->lineColor = glm::vec3(0.0f,0.0f,0.0f);
		this->fillColor = glm::vec3(0.0f, 0.0f, 0.0f);
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->controlPoints.clear();
		this->controlPoints.push_back(glm::ivec3(centerX - radius, centerY + radius, 0));
		this->controlPoints.push_back(glm::ivec3(centerX + radius, centerY + radius, 0));
		this->controlPoints.push_back(glm::ivec3(centerX + radius, centerY - radius, 0));
		this->controlPoints.push_back(glm::ivec3(centerX - radius, centerY - radius, 0));
	}
	circle(glm::ivec3 center,GLint r,glm::vec3 circleColor)
	{
		this->centerX = center.x;
		this->centerY = center.y;
		this->radius = r;
		this->lineColor = circleColor;
		this->fillColor = glm::vec3(0.0f, 0.0f, 0.0f);
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->controlPoints.clear();
		this->controlPoints.push_back(glm::ivec3(centerX - radius, centerY + radius, 0));
		this->controlPoints.push_back(glm::ivec3(centerX + radius, centerY + radius, 0));
		this->controlPoints.push_back(glm::ivec3(centerX + radius, centerY - radius, 0));
		this->controlPoints.push_back(glm::ivec3(centerX - radius, centerY - radius, 0));
	}
	void setCenter(glm::ivec3 centerPoint)
	{
		this->centerX = centerPoint.x;
		this->centerY = centerPoint.y;
	}
	void setRadius(GLint r)
	{
		this->radius = r;
	}
	void setFillColor(glm::vec3 color)
	{
		this->fillColor = color;
	}
	vector<GLfloat> getCirclePixels()
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
	GLint getRadius()
	{
		return this->radius;
	}
	vector<glm::ivec3> getControlPoints()
	{
		controlPoints[0].x = centerX - radius, controlPoints[0].y = centerY + radius;
		controlPoints[1].x = centerX + radius, controlPoints[1].y = centerY + radius;
		controlPoints[2].x = centerX + radius, controlPoints[2].y = centerY - radius;
		controlPoints[3].x = centerX - radius, controlPoints[3].y = centerY - radius;
		return this->controlPoints;
	}
	void circleUseMidpoint()
	{
		clearPixels();
		GLint x = 0, y = radius;
		GLfloat p0 = 5.0f / 4 - radius;
		pushPoint(glm::ivec3(centerX, radius + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
		pushPoint(glm::ivec3(centerX, -radius + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
		pushPoint(glm::ivec3(radius + centerX, centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
		pushPoint(glm::ivec3(-radius + centerX, centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
		for (x = 1; x < y; x++)
		{
			if (p0 < 0)
			{
				p0 = p0 + x + x  + 1;
			}
			else
			{
				y--;
				p0 = p0 + x + x - y - y + 1;
			}
			pushPoint(glm::ivec3(x + centerX, y + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
			pushPoint(glm::ivec3(-x + centerX, y + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
			pushPoint(glm::ivec3(x + centerX, -y + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
			pushPoint(glm::ivec3(-x + centerX, -y + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
			pushPoint(glm::ivec3(y + centerX, x + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
			pushPoint(glm::ivec3(y + centerX, -x + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
			pushPoint(glm::ivec3(-y + centerX, x + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
			pushPoint(glm::ivec3(-y + centerX, -x + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
		}
		pushPoint(glm::ivec3(x + centerX, y + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
		pushPoint(glm::ivec3(x + centerX, -y + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
		pushPoint(glm::ivec3(-x + centerX, y + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
		pushPoint(glm::ivec3(-x + centerX, -y + centerY, 0), -WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
	}
	void fillCircleScanLine()
	{
		clearPixels();
		fillCircleWithWindow(-WIDTH_HALF, WIDTH_HALF, -HEIGHT_HALF, HEIGHT_HALF);
	}
	void clipUseRect(glm::ivec3 winP1, glm::ivec3 winP2,GLboolean filling=GL_FALSE)
	{
		GLint xMin = winP1.x<winP2.x ? winP1.x : winP2.x, xMax = winP1.x>winP2.x ? winP1.x : winP2.x,
			yMin = winP1.y<winP2.y ? winP1.y : winP2.y, yMax = winP1.y>winP2.y ? winP1.y : winP2.y;
		//check RectWindow and Circle position
		if ((centerX + radius <= xMin || centerY + radius <= yMin)
			|| (centerX - radius >= xMax || centerY - radius >= yMax))
		{
			clearPixels();
			return;
		}
		else if (centerX - radius >= xMin&&centerX + radius <= xMax&&centerY - radius >= yMin&&centerY + radius <= yMax)
		{
			return;
		}
		clearPixels();
		if (filling)
			fillCircleWithWindow(xMin, xMax, yMin, yMax);
		else
			midpointCircleWithWindow(xMin, xMax, yMin, yMax);
	}
	GLint loadCircleFromFile(string filePath,string & texturePath)
	{
		GLint filling;
		ifstream ifFile(filePath);
		if (ifFile.is_open())
		{
			string str;
			ifFile >> str;
			if (str.find("Circle") != string::npos)
			{
				ifFile >> str >> str;
				str = str.substr(1, str.size() - 2);
				string::size_type n;
				lineColor.r = stof(str.substr(0, n = str.find(",")));
				str = str.substr(n + 1);
				lineColor.g = stof(str.substr(0, n = str.find(",")));
				str = str.substr(n + 1);
				lineColor.b = stof(str.substr(0));

				ifFile >> str;
				if (str == "fill:")
				{
					ifFile >> str;
					filling = 0;
				}
				else if (str.find("fillColor") != string::npos)
				{
					ifFile >> str;
					str = str.substr(1, str.size() - 2);
					fillColor.r = stof(str.substr(0, n = str.find(",")));
					str = str.substr(n + 1);
					fillColor.g = stof(str.substr(0, n = str.find(",")));
					str = str.substr(n + 1);
					fillColor.b = stof(str.substr(0));
					filling = 1;
				}
				else if (str.find("fillTexture") != string::npos)
				{
					string fileTempStr;
					ifFile >> fileTempStr;
					str = fileTempStr.substr(1);
					while (str.find("\"") == string::npos)
					{
						ifFile >> fileTempStr;
						str += (" " + fileTempStr);
					}
					str = str.substr(0, str.size() - 1);
					filling = 2;
					texturePath = str;
				}

				ifFile >> str >> str;
				str = str.substr(1, str.size() - 2);
				centerX = stoi(str.substr(0, n = str.find(",")));
				str = str.substr(n + 1);
				centerY = stoi(str.substr(0));

				ifFile >> str >> str;
				radius = stoi(str);

				cout << "文件打开成功，读入圆形数据" << endl;
			}
			else
				cout << "文件中没有保存圆形的数据" << endl;
		}
		else
			cout << "文件打开出错" << endl;
		ifFile.close();
		return filling;
	}
	void saveCircleIntoFile(string filePath,GLboolean filled= GL_FALSE,string texturePath = "")
	{
		ofstream ofFile(filePath);
		if (ofFile.is_open())
		{
			ofFile << "Circle:" << "\n";
			ofFile << "lineColor:" << " (" << lineColor.r << "," << lineColor.g << "," << lineColor.b << ")\n";
			if (filled)
			{
				if (texturePath == "")
					ofFile << "fillColor:" << " (" << fillColor.r << "," << fillColor.g << "," << fillColor.b << ")\n";
				else
					ofFile << "fillTexture:" << " \"" << texturePath << "\"\n";
			}
			else
				ofFile << "fill: none\n";
			ofFile << "center:" << " (" << centerX << "," << centerY << ")\n";
			ofFile << "radius:" << " " << radius << "\n";
			cout << "文件保存成功" << endl;
		}
		else
			cout << "文件保存出错" << endl;
		ofFile.close();
	}
};

#endif