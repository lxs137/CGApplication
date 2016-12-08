#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "line.h"
#include "windowSetting.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <vector>
#include <cmath>
using namespace std;

class ellipse
{
private:
	GLint pointsNum;
	//each points' space in memory
	GLint pointSize;
	vector<GLfloat> pixels;
	GLint centerX, centerY;
	GLint radiusX, radiusY;
	GLfloat degree;//椭圆的偏转角（弧度制）
	glm::mat4 degreeMartix;//椭圆的旋转变换矩阵
	glm::vec3 lineColor;
	glm::vec3 fillColor;
	vector<glm::ivec3> controlPoints;
	GLint xMin, xMax, yMin, yMax;//裁剪窗口的XY最小最大值
	void pushSymmetryPoint(glm::ivec3 point)
	{
		vector<glm::ivec3> symmetryPoints;
		GLint x, y;
		glm::vec4 tempPoint = glm::vec4(0, 0, 0, 1);
		if (fabs(degree) < 0.02)
		{			
			x = centerX + point.x, y = centerY + point.y;
			if (x >= xMin && x <= xMax && y >= yMin && y <= yMax)
				symmetryPoints.push_back(glm::ivec3(x, y, 0));

			x = centerX + point.x, y = centerY - point.y;
			if (x >= xMin && x <= xMax && y >= yMin && y <= yMax)
				symmetryPoints.push_back(glm::ivec3(x, y, 0));

			x = centerX - point.x, y = centerY - point.y;
			if (x >= xMin && x <= xMax && y >= yMin && y <= yMax)
				symmetryPoints.push_back(glm::ivec3(x, y, 0));

			x = centerX - point.x, y = centerY + point.y;
			if (x >= xMin && x <= xMax && y >= yMin && y <= yMax)
				symmetryPoints.push_back(glm::ivec3(x, y, 0));
		}
		else
		{
			tempPoint.x = centerX + point.x, tempPoint.y = centerY + point.y;
			tempPoint = degreeMartix*tempPoint;
			x = (GLint)tempPoint.x, y = (GLint)tempPoint.y;
			if (x >= xMin && x <= xMax && y >= yMin && y <= yMax)
				symmetryPoints.push_back(glm::ivec3(x, y, 0));

			tempPoint.x = centerX + point.x, tempPoint.y = centerY - point.y;
			tempPoint = degreeMartix*tempPoint;
			x = (GLint)tempPoint.x, y = (GLint)tempPoint.y;
			if (x >= xMin && x <= xMax && y >= yMin && y <= yMax)
				symmetryPoints.push_back(glm::ivec3(x, y, 0));

			tempPoint.x = centerX - point.x, tempPoint.y = centerY - point.y;
			tempPoint = degreeMartix*tempPoint;
			x = (GLint)tempPoint.x, y = (GLint)tempPoint.y;
			if (x >= xMin && x <= xMax && y >= yMin && y <= yMax)
				symmetryPoints.push_back(glm::ivec3(x, y, 0));

			tempPoint.x = centerX - point.x, tempPoint.y = centerY + point.y;
			tempPoint = degreeMartix*tempPoint;
			x = (GLint)tempPoint.x, y = (GLint)tempPoint.y;
			if (x >= xMin && x <= xMax && y >= yMin && y <= yMax)
				symmetryPoints.push_back(glm::ivec3(x, y, 0));
		}
		for (int i = 0; i < symmetryPoints.size(); i++)
		{
			pixels.push_back(symmetryPoints[i].x / (GLfloat)WIDTH_HALF);
			pixels.push_back(symmetryPoints[i].y / (GLfloat)HEIGHT_HALF);
			pixels.push_back((GLfloat)symmetryPoints[i].z);
			pixels.push_back(lineColor.x);
			pixels.push_back(lineColor.y);
			pixels.push_back(lineColor.z);
			this->pointsNum++;
		}
	}
	void pushFillScanLine(glm::ivec3 point)
	{
		if (fabs(degree) < 0.02)
		{
			GLint xLeft, xRight, yValue;
			xLeft = centerX - point.x, xRight = centerX + point.x, yValue = centerY + point.y;
			if (yValue<yMin || yValue>yMax)
				return;
			else
			{
				xLeft = xLeft < xMin ? (xMin - 1) : xLeft;
				xRight = xRight < xMax ? xRight : (xMax + 1);
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
			xLeft = centerX - point.x, xRight = centerX + point.x, yValue = centerY - point.y;
			if (yValue<yMin || yValue>yMax)
				return;
			else
			{
				xLeft = xLeft < xMin ? (xMin - 1) : xLeft;
				xRight = xRight < xMax ? xRight : (xMax + 1);
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
		else
		{
			line lineFill;
			glm::vec4 tempPoint1, tempPoint2;
			vector<GLfloat> &fillPixels=vector<GLfloat>();

			tempPoint1 = glm::vec4(0, 0, 0, 1),tempPoint2 = glm::vec4(0, 0, 0, 1);
			tempPoint1.x = centerX - point.x, tempPoint1.y = centerY + point.y;
			tempPoint1 = degreeMartix*tempPoint1;
			tempPoint2.x = centerX + point.x, tempPoint2.y = centerY + point.y;
			tempPoint2 = degreeMartix*tempPoint2;
			lineFill=line(glm::ivec3((GLint)tempPoint1.x, (GLint)tempPoint1.y, 0),
				glm::ivec3((GLint)tempPoint2.x, (GLint)tempPoint2.y, 0),fillColor);
			lineFill.lineUseBresenham();
			fillPixels=lineFill.getLinePixels();
			this->pixels.insert(this->pixels.end(), fillPixels.begin(), fillPixels.end());
			this->pointsNum += lineFill.getPointsNum();

			tempPoint1 = glm::vec4(0, 0, 0, 1), tempPoint2 = glm::vec4(0, 0, 0, 1);
			tempPoint1.x = centerX - point.x, tempPoint1.y = centerY - point.y;
			tempPoint1 = degreeMartix*tempPoint1;
			tempPoint2.x = centerX + point.x, tempPoint2.y = centerY - point.y;
			tempPoint2 = degreeMartix*tempPoint2;
			lineFill = line(glm::ivec3((GLint)tempPoint1.x, (GLint)tempPoint1.y, 0),
				glm::ivec3((GLint)tempPoint2.x, (GLint)tempPoint2.y, 0), fillColor);
			lineFill.lineUseBresenham();
			fillPixels = lineFill.getLinePixels();
			this->pixels.insert(this->pixels.end(), fillPixels.begin(), fillPixels.end());
			this->pointsNum += lineFill.getPointsNum();
		}
		
	}
	void fillEllipseWithWindow()
	{
		GLint x = 0, y = radiusY;
		GLfloat p1 = radiusY*radiusY - radiusX*radiusX*radiusY + radiusX*radiusX*0.25f;
		pushSymmetryPoint(glm::ivec3(0, radiusY, 0));
		do
		{
			x++;
			if (p1 <= 0)
			{
				p1 = p1 + 2 * radiusY*radiusY*x + radiusY*radiusY;
			}
			else
			{
				y = y - 1;
				p1 = p1 + 2 * radiusY*radiusY*x - 2 * radiusX*radiusX*y + radiusY*radiusY;
			}
			pushSymmetryPoint(glm::ivec3(x, y, 0));
			//fill the Ellipse
			pushFillScanLine(glm::ivec3(x, y, 0));
		} while (radiusY*radiusY*x <= radiusX*radiusX*y);
		p1 = radiusY*radiusY*(x*x + x + 0.25f) + radiusX*radiusX*(y - 1)*(y - 1) - radiusX*radiusX*radiusY*radiusY;
		for (y--; y > 0; y--)
		{
			if (p1 <= 0)
			{
				x = x + 1;
				p1 = p1 + 2 * radiusY*radiusY*x - 2 * radiusX*radiusX*y + radiusX*radiusX;
			}
			else
			{				
				p1 = p1 - 2 * radiusX*radiusX*y + radiusX*radiusX;
			}
			pushSymmetryPoint(glm::ivec3(x, y, 0));
			//fill the Ellipse
			pushFillScanLine(glm::ivec3(x, y, 0));
		}
		pushSymmetryPoint(glm::ivec3(radiusX, 0, 0));
		pushFillScanLine(glm::ivec3(radiusX, 0, 0));
	}
	inline void clearPixels()
	{
		pointsNum = 0;
		pixels.clear();
	}
public:
	ellipse()
	{
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->centerX = 0;
		this->centerY = 0;
		this->radiusX = 0;
		this->radiusY = 0;
		this->degree = 0;
		this->degreeMartix=glm::mat4();
		this->lineColor = glm::vec3(0.0f, 0.0f, 0.0f);
		this->fillColor = glm::vec3(0.0f, 0.0f, 0.0f);
		this->xMin = -WIDTH, this->xMax = WIDTH, this->yMin = -HEIGHT, this->yMax = HEIGHT;
		controlPoints.clear();
		controlPoints.push_back(glm::ivec3(centerX - radiusX, centerY + radiusY, 0));
		controlPoints.push_back(glm::ivec3(centerX + radiusX, centerY + radiusY, 0));
		controlPoints.push_back(glm::ivec3(centerX + radiusX, centerY - radiusY, 0));
		controlPoints.push_back(glm::ivec3(centerX - radiusX, centerY - radiusY, 0));
	}
	ellipse(glm::ivec3 center,GLint rx,GLint ry,glm::vec3 ellipseColor,GLfloat rotateDegree = 0.0f)
	{
		this->pointsNum = 0;
		this->pointSize = 6 * sizeof(GLfloat);
		this->centerX = center.x;
		this->centerY = center.y;
		this->radiusX = rx;
		this->radiusY = ry;
		this->degree=rotateDegree;
		this->degreeMartix=glm::mat4();
		this->lineColor = ellipseColor;
		this->fillColor = glm::vec3(0.0f, 0.0f, 0.0f);
		this->xMin = -WIDTH, this->xMax = WIDTH, this->yMin = -HEIGHT, this->yMax = HEIGHT;
		controlPoints.clear();
		controlPoints.push_back(glm::ivec3(centerX - radiusX, centerY + radiusY, 0));
		controlPoints.push_back(glm::ivec3(centerX + radiusX, centerY + radiusY, 0));
		controlPoints.push_back(glm::ivec3(centerX + radiusX, centerY - radiusY, 0));
		controlPoints.push_back(glm::ivec3(centerX - radiusX, centerY - radiusY, 0));
	}
	void setCenter(glm::ivec3 center)
	{
		this->centerX = center.x;
		this->centerY = center.y;
		this->degreeMartix = glm::mat4();
		degreeMartix = glm::translate(degreeMartix, glm::vec3(centerX, centerY, 0));
		degreeMartix = glm::rotate(degreeMartix, this->degree, glm::vec3(0, 0, 1.0f));
		degreeMartix = glm::translate(degreeMartix, glm::vec3(-centerX, -centerY, 0));
	}
	void setRadius(GLint rX, GLint rY)
	{
		this->radiusX = rX;
		this->radiusY = rY;
	}
	void addRotateDegree(GLfloat rotateDegree)
	{
		this->degree+=rotateDegree;
		if (this->degree > 3.14)
			this->degree -= 3.14;
		if (this->degree < -3.14)
			this->degree += 3.14;
		this->degreeMartix=glm::mat4();
		degreeMartix = glm::translate(degreeMartix, glm::vec3(centerX, centerY, 0));
		degreeMartix = glm::rotate(degreeMartix, this->degree, glm::vec3(0, 0, 1.0f));
		degreeMartix = glm::translate(degreeMartix, glm::vec3(-centerX, -centerY, 0));
    }
	vector<GLfloat> getEllipsePixels()
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
	glm::ivec3 getRadius()
	{
		return glm::ivec3(radiusX,radiusY,0);
	}
	vector<glm::ivec3> getControlPoints()
	{
		if (fabs(degree) < 0.02)
		{
			controlPoints[0].x = centerX - radiusX, controlPoints[0].y = centerY + radiusY;
			controlPoints[1].x = centerX + radiusX, controlPoints[1].y = centerY + radiusY;
			controlPoints[2].x = centerX + radiusX, controlPoints[2].y = centerY - radiusY;
			controlPoints[3].x = centerX - radiusX, controlPoints[3].y = centerY - radiusY;
		}
		else
		{
			glm::vec4 tempPoint = glm::vec4(0, 0, 0, 1);
			tempPoint.x = centerX + radiusX, tempPoint.y = centerY + radiusY;
			tempPoint = degreeMartix*tempPoint;
			controlPoints[0].x = tempPoint.x, controlPoints[0].y = tempPoint.y;

			tempPoint.x = centerX + radiusX, tempPoint.y = centerY - radiusY;
			tempPoint = degreeMartix*tempPoint;
			controlPoints[1].x = tempPoint.x, controlPoints[1].y = tempPoint.y;

			tempPoint.x = centerX - radiusX, tempPoint.y = centerY - radiusY;
			tempPoint = degreeMartix*tempPoint;
			controlPoints[2].x = tempPoint.x, controlPoints[2].y = tempPoint.y;

			tempPoint.x = centerX - radiusX, tempPoint.y = centerY + radiusY;
			tempPoint = degreeMartix*tempPoint;
			controlPoints[3].x = tempPoint.x, controlPoints[3].y = tempPoint.y;
		}
		return this->controlPoints;
	}
	GLfloat getRotateDegree()
	{
		return this->degree;
	}
	void ellipseUseMidpoint()
	{
		clearPixels();
		xMin = -WIDTH, xMax = WIDTH, yMin = -HEIGHT, yMax = HEIGHT;
		GLint x = 0, y = radiusY;
		GLfloat p1 = radiusY*radiusY - radiusX*radiusX*radiusY + radiusX*radiusX*0.25f;
		pushSymmetryPoint(glm::ivec3(0, radiusY, 0));
		do
		{
			x++;
			if (p1 <= 0)
			{
				p1 = p1 + 2 * radiusY*radiusY*x + radiusY*radiusY;
			}
			else
			{
				y = y - 1;
				p1 = p1 + 2 * radiusY*radiusY*x - 2 * radiusX*radiusX*y + radiusY*radiusY;
			}
			pushSymmetryPoint(glm::ivec3(x, y, 0));
		} while (radiusY*radiusY*x <= radiusX*radiusX*y);
		p1 = radiusY*radiusY*(x*x + x + 0.25f) + radiusX*radiusX*(y - 1)*(y - 1) - radiusX*radiusX*radiusY*radiusY;
		for (y--; y > 0; y--)
		{
			if (p1 <= 0)
			{
				x = x + 1;
				p1 = p1 + 2 * radiusY*radiusY*x - 2 * radiusX*radiusX*y + radiusX*radiusX;
			}
			else
			{
				p1 = p1 - 2 * radiusX*radiusX*y + radiusX*radiusX;
			}
			pushSymmetryPoint(glm::ivec3(x, y, 0));
		}
		pushSymmetryPoint(glm::ivec3(radiusX, 0, 0));
	}
	void fillEllipseScanLine(glm::vec3 fillColor)
	{
		clearPixels();
		this->fillColor = fillColor;
		xMin = -WIDTH, xMax = WIDTH, yMin = -HEIGHT, yMax = HEIGHT;
		fillEllipseWithWindow();
	}
	void clipUseRect(glm::ivec3 winP1, glm::ivec3 winP2)
	{
		GLint xWMin = winP1.x<winP2.x ? winP1.x : winP2.x, xWMax = winP1.x>winP2.x ? winP1.x : winP2.x,
			yWMin = winP1.y<winP2.y ? winP1.y : winP2.y, yWMax = winP1.y>winP2.y ? winP1.y : winP2.y;
		//check RectWindow and Circle position
		if ((centerX + radiusX <= xWMin || centerY + radiusY <= yWMin)
			|| (centerX - radiusX >= xWMax || centerY - radiusY >= yWMax))
		{
			clearPixels();
			return;
		}
		else if (centerX - radiusX >= xWMin&&centerX + radiusX <= xWMax
			&&centerY - radiusY >= yWMin&&centerY + radiusY <= yWMax)
		{
			return;
		}
		clearPixels();
		xMin = xWMin, xMax = xWMax, yMin = yWMin, yMax = yWMax;
		fillEllipseWithWindow();
	}
};
#endif 
