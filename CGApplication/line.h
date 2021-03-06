#ifndef LINE_H
#define LINE_H

#include "windowSetting.h"
#include <iostream>
#include <fstream>
#include <string>
#include <glm\glm.hpp>
#include <cmath>
#include <vector>
using namespace std;

class line
{
	private:
		GLint pointsNum;
		//each points' space in memory
		GLint pointSize;
		vector<GLfloat> pixels;
		GLint x1, y1, x2, y2;
		glm::vec3 color;
		GLint drawingPointIndex;//有0，1两个值，代表两个端点
		void pushPoint(glm::ivec3 pointPosition)
		{
			pixels.push_back(pointPosition.x  / (GLfloat)WIDTH_HALF);
			pixels.push_back(pointPosition.y  / (GLfloat)HEIGHT_HALF);
			pixels.push_back((GLfloat)pointPosition.z);
			pixels.push_back(color.x);
			pixels.push_back(color.y);
			pixels.push_back(color.z);
			pointsNum++;
		}
		void lineSlope1()
		{
			GLint x_1 = x1, x_2 = x2, y_1 = y1, y_2 = y2;
			if ((x_2 - x_1)*(y_2 - y_1) > 0)
			{
				int dx = abs(x_2 - x_1);
				x_1 = (x_1 < x_2) ? x_1 : x_2;
				y_1 = (y_1 < y_2) ? y_1 : y_2;
				for (int i = 0; i <= dx; i++)
					pushPoint(glm::ivec3(x_1 + i, y_1 + i, 0));
			}
			else
			{
				int dx = abs(y_2 - y_1);
				x_1 = (x_1 < x_2) ? x_1 : x_2;
				y_1 = (y_1 < y_2) ? y_2 : y_1;
				for (int i = 0; i <= dx; i++)
					pushPoint(glm::ivec3(x_1 + i, y_1 - i, 0));
			}
			return;
		}
		void lineVertical()
		{
			GLint y_1 = y1, y_2 = y2;
			if (y_1>y_2)
				swap(y_1, y_2);
			for (GLint y = y_1; y <= y_2; y++)
			{
				pushPoint(glm::ivec3(x1,y,0));
			}
		}
		GLboolean clipTestLiangBarsky(GLfloat p, GLfloat q, GLfloat &u1, GLfloat &u2)
		{
			//Be used in Liang-Barsky Clip function
			GLfloat r = 0.0f;
			if (p < 0.0f)
			{
				r=q / p;
				if (r > u1)
				{
					u1 = r;
					if (u1 > u2)
						return false;
				}		
			}
			else if (p > 0.0f)
			{
				r = q / p;
				if (r < u2)
				{
					u2 = r;
					if (u1 > u2)
						return false;
				}
			}
			else if (q < 0.0)
					return false;
			return true;
		}
		inline void clearPixels()
		{
			pointsNum = 0;
			pixels.clear();
		}
	public:
		line()
		{
			this->x1 = 0;
			this->y1 = 0;
			this->x2 = 0;
			this->y2 = 0;
			this->pointsNum = 0;
			this->color = glm::vec3(0.0f,0.0f,0.0f);
			this->pointSize = 6 * sizeof(GLfloat);
			this->drawingPointIndex = 1;
		}
		line(glm::ivec3 p1, glm::ivec3 p2,glm::vec3 lineColor)
		{
			if (p1.x < p2.x)
			{
				this->x1 = p1.x;
				this->y1 = p1.y;
				this->x2 = p2.x;
				this->y2 = p2.y;
			}
			else
			{
				this->x1 = p2.x;
				this->y1 = p2.y;
				this->x2 = p1.x;
				this->y2 = p1.y;
			}
			this->pointsNum = 0;
			this->color = lineColor;
			this->pointSize = 6 * sizeof(GLfloat);
			this->drawingPointIndex = 1;
		}
		void setDrawingPoint(glm::ivec3 point)
		{
			if (drawingPointIndex == 0)
			{
				this->x1 = point.x;
				this->y1 = point.y;
			}
			else if (drawingPointIndex == 1)
			{
				this->x2 = point.x;
				this->y2 = point.y;
			}
		}
		void setPoint(glm::ivec3 p1, glm::ivec3 p2)
		{
			this->x1 = p1.x;
			this->x2 = p2.x;
			this->y1 = p1.y;
			this->y2 = p2.y;
		}
		GLboolean checkChangePoint(glm::ivec3 changePoint)
		{
			if (abs(changePoint.x - x1) <= CHANGE_POINT_DIS&&abs(changePoint.y - y1) <= CHANGE_POINT_DIS)
			{
				x1 = changePoint.x;
				y1 = changePoint.y;
				drawingPointIndex = 0;
				return GL_TRUE;
			}
			else if (abs(changePoint.x - x2) <= CHANGE_POINT_DIS&&abs(changePoint.y - y2) <= CHANGE_POINT_DIS)
			{
				x2 = changePoint.x;
				y2 = changePoint.y;
				drawingPointIndex = 1;
				return GL_TRUE;
			}
			else
				return GL_FALSE;
		}
		glm::ivec3 getPoint(GLint index)
		{
			if (index == 0)
				return glm::ivec3(x1, y1, 0);
			else if (index == 1)
				return glm::ivec3(x2, y2, 0);
			else
				return glm::ivec3(x1, y1, 0);
		}
		vector<GLfloat> getLinePixels()
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
		glm::ivec3 findNextPoint(glm::ivec3 nowPoint,GLint next_dert)
		{
			if (x1 == x2&&y2 > y1)
				return glm::ivec3(nowPoint.x, nowPoint.y + next_dert, 0);
			else if (x1 == x2&&y2 <= y1)
				return glm::ivec3(nowPoint.x, nowPoint.y - next_dert, 0);
			GLfloat k=((GLfloat)(y2-y1))/(x2-x1);
			GLint dertX=(GLint)sqrt((next_dert*next_dert)/(1+k*k));
			if (x2 - x1 >= 0)
				dertX = abs(dertX);
			else
				dertX = -abs(dertX);
			GLint dertY = (GLint)(k*dertX);
			return glm::ivec3(nowPoint.x+dertX,nowPoint.y+dertY,0);
		}
		void lineUseBresenham()
		{
			clearPixels();
			GLint x_1=this->x1,x_2=this->x2,y_1=this->y1,y_2=this->y2;
			if (abs(x_2 - x_1) == abs(y_2 - y_1))
			{
				lineSlope1();
				return;
			}
			if (x_1 > x_2)
			{
				swap(x_1, x_2);
				swap(y_1, y_2);
			}
			else if (x_1 == x_2)
			{
				lineVertical();
				return;
			}
				
			GLfloat m = ((GLfloat)(y_2 - y_1)) / (x_2 - x_1);
			if (m > 0 && m < 1)
			{
				GLint dertX = x_2 - x_1, dertY = y_2 - y_1, dert2Y = dertY + dertY,
					dert2x_2Y = dertY + dertY - dertX - dertX, p0 = dertY + dertY - dertX;
				GLint x_next = 1, y_next = (y_2>y_1) ? 1 : -1;
				GLint y = y_1;
				pushPoint(glm::ivec3(x_1, y_1, 0));
				for (GLint x = x_1 + x_next; x != x_2; x += x_next)
				{
					if (p0 <= 0)
					{
						pushPoint(glm::ivec3(x, y, 0));
						p0 = p0 + dert2Y;
					}
					else
					{
						y += y_next;
						pushPoint(glm::ivec3(x, y, 0));
						p0 = p0 + dert2x_2Y;
					}
				}
				pushPoint(glm::ivec3(x_2, y_2, 0));
			}
			else if (m > -1 && m <= 0)
			{
				y_1 = -y_1, y_2 = -y_2;
				GLint dertX = x_2 - x_1, dertY = y_2 - y_1, dert2Y = dertY + dertY,
					dert2x_2Y = dertY + dertY - dertX - dertX, p0 = dertY + dertY - dertX;
				GLint x_next = 1, y_next = (y_2 > y_1) ? 1 : -1;
				GLint y = y_1;
				pushPoint(glm::ivec3(x_1, -y_1, 0));
				for (GLint x = x_1 + x_next; x != x_2; x += x_next)
				{
					if (p0 <= 0)
					{
						pushPoint(glm::ivec3(x, -y, 0));
						p0 = p0 + dert2Y;
					}
					else
					{
						y += y_next;
						pushPoint(glm::ivec3(x, -y, 0));
						p0 = p0 + dert2x_2Y;
					}
				}
				pushPoint(glm::ivec3(x_2, -y_2, 0));
			}
			else if (m > 1)
			{
				swap(x_1, y_1);
				swap(x_2, y_2);
				if (x_1 > x_2)
				{
					swap(x_1, x_2);
					swap(y_1, y_2);
				}
				GLint dertX = x_2 - x_1, dertY = y_2 - y_1, dert2Y = dertY + dertY,
					dert2x_2Y = dertY + dertY - dertX - dertX, p0 = dertY + dertY - dertX;
				GLint x_next = 1, y_next = (y_2 > y_1) ? 1 : -1;
				GLint y = y_1;
				pushPoint(glm::ivec3(y_1, x_1, 0));
				for (GLint x = x_1 + x_next; x != x_2; x += x_next)
				{
					if (p0 <= 0)
					{
						pushPoint(glm::ivec3(y, x, 0));
						p0 = p0 + dert2Y;
					}
					else
					{
						y += y_next;
						pushPoint(glm::ivec3(y, x, 0));
						p0 = p0 + dert2x_2Y;
					}
				}
				pushPoint(glm::ivec3(y_2, x_2, 0));
			}
			else
			{
				swap(x_1, y_1);
				swap(x_2, y_2);
				y_1 = -y_1, y_2 = -y_2;
				if (x_1 > x_2)
				{
					swap(x_1, x_2);
					swap(y_1, y_2);
				}
				GLint dertX = x_2 - x_1, dertY = y_2 - y_1, dert2Y = dertY + dertY,
					dert2x_2Y = dertY + dertY - dertX - dertX, p0 = dertY + dertY - dertX;
				GLint x_next = 1, y_next = (y_2 > y_1) ? 1 : -1;
				GLint y = y_1;
				pushPoint(glm::ivec3(-y_1, x_1, 0));
				for (GLint x = x_1 + x_next; x != x_2; x += x_next)
				{
					if (p0 <= 0)
					{
						pushPoint(glm::ivec3(-y, x, 0));
						p0 = p0 + dert2Y;
					}
					else
					{
						y += y_next;
						pushPoint(glm::ivec3(-y, x, 0));
						p0 = p0 + dert2x_2Y;
					}
				}
				pushPoint(glm::ivec3(-y_2, x_2, 0));
			}
		}
		void lineUseDDA()
		{
			clearPixels();
			if (abs(x2 - x1) == abs(y2 - y1))
			{
				lineSlope1();
				return;
			}
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
				GLfloat y = (GLfloat)y1;
				x_dert = (GLint)x_dert;
				pushPoint(glm::ivec3(x1, y1, 0));
				for (GLint x = x1+x_dert; x != x2; x += x_dert)
				{
					y += y_dert;
					pushPoint(glm::ivec3(x, GLint(y), 0));
				}
				pushPoint(glm::ivec3(x2, y2, 0));
			}
			else
			{
				GLfloat x = x1;
				y_dert = (GLint)y_dert;
				pushPoint(glm::ivec3(x1, y1, 0));
				for (GLint y = y1+y_dert; y != y2; y += y_dert)
				{
					x += x_dert;
					pushPoint(glm::ivec3(GLint(x), y, 0));
				}
				pushPoint(glm::ivec3(x2, y2, 0));
			}
		}
		void clipUseRect(glm::ivec3 winP1,glm::ivec3 winP2)
		{
			//Clip use Liang-Barsky
			GLfloat u1 = 0.0f, u2 = 1.0f;
			GLfloat xMin = winP1.x<winP2.x ? winP1.x : winP2.x, xMax = winP1.x>winP2.x ? winP1.x : winP2.x,
				yMin = winP1.y<winP2.y ? winP1.y : winP2.y, yMax = winP1.y>winP2.y ? winP1.y : winP2.y;
			GLfloat dx = this->x2 - this->x1, dy = this->y2 - this->y1;
			if (clipTestLiangBarsky(-dx, (GLfloat)this->x1 - xMin, u1, u2))
				if (clipTestLiangBarsky(dx, xMax - (GLfloat)this->x1, u1, u2))
					if (clipTestLiangBarsky(-dy, (GLfloat)this->y1 - yMin, u1, u2))
						if (clipTestLiangBarsky(dy, yMax - (GLfloat)this->y1, u1, u2))
						{
							if (u2 < 1.0)
							{
								this->x2 = this->x1 + (GLint)(u2*dx);
								this->y2 = this->y1 + (GLint)(u2*dy);
							}
							if (u1 > 0.0)
							{
								this->x1 = this->x1 + (GLint)(u1*dx);
								this->y1 = this->y1 + (GLint)(u1*dy);
							}	
							lineUseBresenham();
						}		
		}
		GLboolean findLineIntersect(line line2, glm::ivec3 &point)
		{
			//求线段的交点
			glm::ivec3 pA = glm::ivec3(x1, y1, 0), pB = glm::ivec3(x2, y2, 0),
				pC = line2.getPoint(0), pD = line2.getPoint(1);
			GLfloat denominator = (pB.y - pA.y)*(pD.x - pC.x) - (pA.x - pB.x)*(pC.y - pD.y);
			if (denominator == 0)
				return false;
			GLint intersectX, intersectY;
			intersectX = ((pB.x - pA.x)*(pD.x - pC.x)*(pC.y - pA.y) + (pB.y - pA.y)*(pD.x - pC.x)*pA.x
				- (pD.y - pC.y)*(pB.x - pA.x)*pC.x) / denominator;
			intersectY = -((pB.y - pA.y)*(pD.y - pC.y)*(pC.x - pA.x) + (pB.x - pA.x)*(pD.y - pC.y)*pA.y
				- (pD.x - pC.x)*(pB.y - pA.y)*pC.y) / denominator;

			//判断交点是否在两条线段上
			if ((intersectX - pA.x)*(intersectX - pB.x) <= 0 && (intersectY - pA.y)*(intersectY - pB.y) <= 0
				&& (intersectX - pC.x)*(intersectX - pD.x) <= 0 && (intersectY - pC.y)*(intersectY - pD.y) <= 0)
			{
				point.x = (GLint)intersectX;
				point.y = (GLint)intersectY;
				return true;
			}
			else
				return false;
		}
		void loadLineFromFile(string filePath)
		{
			ifstream ifFile(filePath);
			if (ifFile.is_open())
			{
				string str;
				ifFile >> str;
				if (str.find("Line") != string::npos)
				{
					ifFile>>str>>str;
					str = str.substr(1, str.size() - 2);
					string::size_type n;
					color.r = stof(str.substr(0, n = str.find(",")));
					str = str.substr(n + 1);
					color.g = stof(str.substr(0, n = str.find(",")));
					str = str.substr(n + 1);
					color.b = stof(str.substr(0));

					ifFile >> str >> str;
					str = str.substr(1, str.size() - 2);
					x1 = stoi(str.substr(0, n = str.find(",")));
					str = str.substr(n + 1);
					y1 = stoi(str.substr(0));
					ifFile >> str;
					str = str.substr(1, str.size() - 2);
					x2 = stoi(str.substr(0, n = str.find(",")));
					str = str.substr(n + 1);
					y2 = stoi(str.substr(0));
					cout<<"文件打开成功，读入直线数据"<<endl;
				}
				else
					cout << "文件中没有保存直线的数据"<<endl;
			}
			else
				cout << "文件打开出错"<<endl;
			
		}
		void saveLineIntoFile(string filePath)
		{
			ofstream ofFile(filePath);
			if (ofFile.is_open())
			{
				ofFile << "Line:" << "\n";
				ofFile << "lineColor:" << " (" << color.r << "," << color.g << "," << color.b << ")\n";
				ofFile << "point:" << " (" << x1 << "," << y1 << ")" << " (" << x2 << "," << y2 << ")\n";
				cout<<"文件保存成功"<<endl;
			}
			else
				cout << "文件保存出错"<<endl;
			ofFile.close();
		}
};

#endif