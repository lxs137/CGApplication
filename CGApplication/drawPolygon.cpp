#include <iostream>
#include <fstream>
#include <vector>
#include "drawPolygon.h"
#include "polygon.h"
#include "shader.h"
#include "textureManager.h"
#include "windowSetting.h"

#include <freeglut\freeglut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
using namespace drawPolygon;

polygon myPolygon;

void drawPolygonApplication(int argc, char **argv)
{
	if (glutGet(GLUT_INIT_STATE) != 1)
	{
		glutInit(&argc, argv);
		polygonInitGlutWindow();
	}

	polygonInitMenus();
	//set shader program	
	shader *myShader = new shader("2dModel.vert", "2dModel.frag");
	myShaderProgram = myShader->shaderProgram;

	//set VAO
	polygonInitVAO(myVAO, myVBO);

	//set transform
	GLuint transformLocation = glGetUniformLocation(myShaderProgram, "transform");

	glutDisplayFunc(polygonRender2DSence);
	glutIdleFunc(polygonRender2DSence);
	glutReshapeFunc(polygonOnReshape);
	glutMouseFunc(polygonOnMouseClick);
	glutMotionFunc(polygonOnActiveMotion);
	while (GL_TRUE)
	{
		glutMainLoopEvent();
		if (transformStatus == EXIT)
			break;
		glutPostRedisplay();
	}
}

void polygonRender2DSence()
{
	glClearColor(1.0f, 0.8f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(myShaderProgram);
	glBindVertexArray(myVAO);
	//glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));
	glDrawArrays(GL_POINTS, 0, myPolygon.getPointsNum());
	glBindVertexArray(0);
	if (vertexPoints.size() != 0)
	{
		GLfloat dertX, dertY, xTemp, yTemp;
		dertX = ((GLfloat)CHANGE_POINT_DIS) / WIDTH_HALF, dertY = ((GLfloat)CHANGE_POINT_DIS) / HEIGHT_HALF;
		glBegin(GL_QUADS);
		for (GLint i = 0; i < vertexPoints.size(); i++)
		{
			xTemp = ((GLfloat)vertexPoints[i].x) / WIDTH_HALF, yTemp = ((GLfloat)vertexPoints[i].y) / HEIGHT_HALF;
			glVertex2f(xTemp - dertX, yTemp + dertY);
			glVertex2f(xTemp + dertX, yTemp + dertY);
			glVertex2f(xTemp + dertX, yTemp - dertY);
			glVertex2f(xTemp - dertX, yTemp - dertY);
		}
		glEnd();

		glFlush();
	}
	glutSwapBuffers();
}
void polygonOnMouseClick(int button, int state, int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
	{
		if (!myPolygon.getIsClose())
		{
			drawStatus = 1;
			if (vertexPoints.size() >= drawingPointIndex)
			{
				vertexPoints[drawingPointIndex - 1].x = x;
				vertexPoints[drawingPointIndex - 1].y = y;
				myPolygon.setVertics(glm::ivec3(x, y, 0), drawingPointIndex);
			}
			else
			{
				vertexPoints.push_back(glm::ivec3(x, y, 0));
				myPolygon.setVertics(glm::ivec3(x, y, 0), drawingPointIndex);
			}
			myPolygon.polygonUseLine();
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
				myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			if (drawingPointIndex == 1)
				drawingPointIndex++;
		}
		else
		{
			drawStatus = 2;
			GLint xTemp, yTemp;
			GLint i;
			for (i = 0; i < vertexPoints.size(); i++)
			{
				xTemp = vertexPoints[i].x, yTemp = vertexPoints[i].y;
				if (x <= xTemp + CHANGE_POINT_DIS&&x >= xTemp - CHANGE_POINT_DIS
					&&y <= yTemp + CHANGE_POINT_DIS&&y >= yTemp - CHANGE_POINT_DIS)
					break;
			}
			if (i != vertexPoints.size())
				drawingPointIndex = i + 1;
			else
				drawStatus = 0;
		}
	}
	else if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
	{
		if (drawStatus != 0)
		{
			if (drawStatus == 1 && drawingPointIndex == 2)
			{
				if (vertexPoints[drawingPointIndex - 2].x == x
					&&vertexPoints[drawingPointIndex - 2].y == y)
				{
					drawingPointIndex--;
					vertexPoints.clear();
					drawStatus = 0;
					return;
				}
			}
			GLint startX = vertexPoints[0].x, startY = vertexPoints[0].y;
			if (drawStatus == 2)
			{
				myPolygon.setVertics(glm::ivec3(x, y, 0), drawingPointIndex);
				myPolygon.polygonUseLine();
				glBindBuffer(GL_ARRAY_BUFFER, myVBO);
				glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
					myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			else if (drawStatus == 1 && x <= startX + CHANGE_POINT_DIS*2&&x >= startX - CHANGE_POINT_DIS*2
				&&y <= startY + CHANGE_POINT_DIS*2&&y >= startY - CHANGE_POINT_DIS*2)
			{
				myPolygon.setIsclose(GL_TRUE);
				vertexPoints.pop_back();
				myPolygon.polygonUseLine();
				glBindBuffer(GL_ARRAY_BUFFER, myVBO);
				glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
					myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			else
			{
				if (vertexPoints.size() < drawingPointIndex)
				{
					vertexPoints.push_back(glm::ivec3(x, y, 0));
				}
				drawingPointIndex++;
				myPolygon.polygonUseLine();
				glBindBuffer(GL_ARRAY_BUFFER, myVBO);
				glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
					myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}				
			
		}
	}
}
void polygonOnActiveMotion(int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	switch (transformStatus)
	{
	case drawPolygon::EDIT:
		if (drawStatus != 0)
		{
			if (vertexPoints.size() >= drawingPointIndex)
			{
				vertexPoints[drawingPointIndex - 1].x = x;
				vertexPoints[drawingPointIndex - 1].y = y;
			}
			else
			{
				vertexPoints.push_back(glm::ivec3(x, y, 0));
			}
			myPolygon.setVertics(glm::ivec3(x, y, 0), drawingPointIndex);
			if (drawStatus == 2 || drawStatus == 1)
			{
				myPolygon.polygonUseLine();
			}
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
				myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		break;
	case drawPolygon::MOVE:
		break;
	case drawPolygon::ROTATE:
		break;
	case drawPolygon::ZOOM:
		break;
	case drawPolygon::EXIT:
		break;
	default:
		break;
	}
	
}
void polygonOnMouseWheelScrollValid(int wheel, int direction, int x, int y)
{
	polygonGetTransformMatrix(glm::ivec2(direction * 2, direction * 2));
	myPolygon.polygonUseLine();
	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
		myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void polygonOnMouseWheelScrollInvalid(int wheel, int direction, int x, int y){}
void polygonProcessMenuEvent(int options)
{
	GLint n;
	if (myPolygon.getPointsNum() == 0)//在多边形未初始化时，不能对多边形进行变换
		return;
	switch (options)
	{
	case EDIT:
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		glutMouseWheelFunc(polygonOnMouseWheelScrollInvalid);
		transformStatus = EDIT;
		polygonSetTransBasisPoint();
		break;
	case MOVE:
		glutSetCursor(GLUT_CURSOR_CYCLE);
		glutMouseWheelFunc(polygonOnMouseWheelScrollInvalid);
		transformStatus = MOVE;
		polygonSetTransBasisPoint();
		break;
	case ROTATE:
		glutSetCursor(GLUT_CURSOR_WAIT);
		glutMouseWheelFunc(polygonOnMouseWheelScrollInvalid);
		transformStatus = ROTATE;
		rotateCenter.x = 0, rotateCenter.y = 0;
		n = vertexPoints.size();
		for (int i = 0; i < n; i++)
			rotateCenter.x += vertexPoints[i].x, rotateCenter.y += vertexPoints[i].y;
		rotateCenter.x /= n, rotateCenter.y /= n;
		polygonSetTransBasisPoint();
		break;
	case ZOOM:
		glutSetCursor(GLUT_CURSOR_SPRAY);
		glutMouseWheelFunc(polygonOnMouseWheelScrollValid);
		transformStatus = ZOOM;
		rotateCenter.x = 0, rotateCenter.y = 0;
		n = vertexPoints.size();
		for (int i = 0; i < n; i++)
			rotateCenter.x += vertexPoints[i].x, rotateCenter.y += vertexPoints[i].y;
		rotateCenter.x /= n, rotateCenter.y /= n;
		polygonSetTransBasisPoint();
		break;
	case EXIT:
		transformStatus = EXIT;
		break;
	}
}
void polygonOnReshape(int width, int height)
{

}
void polygonInitGlutWindow()
{
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("OpenGL Application");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		cout << "Failed to initialize GLEW" << endl;
		return;
	}
	glViewport(0, 0, WIDTH, HEIGHT);
}
void polygonInitMenus()
{
	GLint menu = glutCreateMenu(polygonProcessMenuEvent);
	glutSetMenuFont(menu, GLUT_BITMAP_9_BY_15);
	glutAddMenuEntry("Edit Polygon", EDIT);
	glutAddMenuEntry("Move Polygon", MOVE);
	glutAddMenuEntry("Rotate Polygon", ROTATE);
	glutAddMenuEntry("Zoom Polygon", ZOOM);
	glutAddMenuEntry("Exit", EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutSetCursor(GLUT_CURSOR_INHERIT);
}
void polygonInitVAO(GLuint &VAO, GLuint &VBO)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//set vertex buffer and attribute point
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	myPolygon = polygon();
	glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
		myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//unbind VBO&VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void polygonGetTransformMatrix(glm::ivec2 transformInfo, glm::ivec2 rotateInfo)
{
	//当处于移动状态时，传入transformInfo代表x，y方向上的移动距离，rotateInfo不传入数据
	//当处于旋转状态时，传入transformInfo、rotateInfo分别代表旋转开始时的鼠标位置和当前鼠标位置
	//当处于缩放状态时，传入transformInfo代表x，y方向上的缩放百分比，rotateInfo不传入数据
	glm::mat4 transformMat;
	glm::vec2 vecA, vecB;
	switch (transformStatus)
	{
	case drawPolygon::MOVE:
		transformMat = glm::translate(transformMat, glm::vec3(transformInfo.x, transformInfo.y, 0));
		break;
	case drawPolygon::ROTATE:
		GLfloat degree;//弧度值
		vecA = glm::vec2(transformInfo.x - rotateCenter.x, transformInfo.y - rotateCenter.y);
		vecB = glm::vec2(rotateInfo.x - rotateCenter.x, rotateInfo.y - rotateCenter.y);
		degree = acos((vecA.x*vecB.x + vecA.y*vecB.y) /
			sqrt((vecA.x*vecA.x + vecA.y*vecA.y)*(vecB.x*vecB.x + vecB.y*vecB.y)));
		if (vecA.x*vecB.y - vecA.y*vecB.x < 0)
			degree = -degree;
		transformMat = glm::translate(transformMat, glm::vec3(rotateCenter.x, rotateCenter.y, 0));
		transformMat = glm::rotate(transformMat, degree, glm::vec3(0, 0, 1));
		transformMat = glm::translate(transformMat, glm::vec3(-rotateCenter.x, -rotateCenter.y, 0));
		break;
	case drawPolygon::ZOOM:
		transformMat = glm::translate(transformMat, glm::vec3(rotateCenter.x, rotateCenter.y, 0));
		transformMat = glm::scale(transformMat, glm::vec3((100 + transformInfo.x) / 100.0, (100 + transformInfo.y) / 100.0, 0));
		transformMat = glm::translate(transformMat, glm::vec3(-rotateCenter.x, -rotateCenter.y, 0));
		break;
	}
	glm::vec4 tempPoint = glm::vec4(0, 0, 0, 1);
	if (transformStatus == drawPolygon::ROTATE)
	{
		for (int i = 0; i < transBasisPoint.size(); i++)
		{
			tempPoint.x = transBasisPoint[i].x, tempPoint.y = transBasisPoint[i].y;
			tempPoint = transformMat*tempPoint;
			vertexPoints[i].x = (GLint)tempPoint.x, vertexPoints[i].y = (GLint)tempPoint.y;
			myPolygon.setVertics(vertexPoints[i], i + 1);
		}
	}
	else
	{
		for (int i = 0; i < vertexPoints.size(); i++)
		{
			tempPoint.x = vertexPoints[i].x, tempPoint.y = vertexPoints[i].y;
			tempPoint = transformMat*tempPoint;
			vertexPoints[i].x = (GLint)tempPoint.x, vertexPoints[i].y = (GLint)tempPoint.y;
			myPolygon.setVertics(vertexPoints[i], i + 1);
		}
	}
}
void polygonSetTransBasisPoint()
{
	for (int i = 0; i < transBasisPoint.size(); i++)
	{
		transBasisPoint[i].x = vertexPoints[i].x;
		transBasisPoint[i].y = vertexPoints[i].y;
	}
}


