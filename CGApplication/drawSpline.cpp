#include <iostream>
#include <fstream>
#include <vector>
#include "drawSpline.h"
#include "splineCurve.h"
#include "shader.h"
#include "textureManager.h"
#include "windowSetting.h"

#include <freeglut\freeglut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
using namespace drawSpline;

spline mySpline;

void drawSplineApplication(int argc, char **argv)
{
	
	if (glutGet(GLUT_INIT_STATE) != 1)
	{
		glutInit(&argc, argv);
		splineInitGlutWindow();
	}

	splineInitMenus();
	transBasisPoint.push_back(glm::ivec3(0, 0, 0));
	transBasisPoint.push_back(glm::ivec3(0, 0, 0));
	transBasisPoint.push_back(glm::ivec3(0, 0, 0));
	transBasisPoint.push_back(glm::ivec3(0, 0, 0));
	//set shader program	
	shader *myShader = new shader("2dModel.vert", "2dModel.frag");
	myShaderProgram = myShader->shaderProgram;

	//set VAO
	splineInitVAO(myVAO, myVBO);

	//set transform
	GLuint transformLocation = glGetUniformLocation(myShaderProgram, "transform");

	glutDisplayFunc(splineRender2DSence);
	glutIdleFunc(splineRender2DSence);
	glutReshapeFunc(splineOnReshape);
	glutMouseFunc(splineOnMouseClick);
	glutMotionFunc(splineOnActiveMotion);
	while (GL_TRUE)
	{
		glutMainLoopEvent();
		if (transformStatus == EXIT)
			break;
		glutPostRedisplay();
	}
}

void splineRender2DSence()
{
	glClearColor(1.0f, 0.8f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(myShaderProgram);
	glBindVertexArray(myVAO);
	//glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));
	glDrawArrays(GL_POINTS, 0, mySpline.getPointsNum());
	glBindVertexArray(0);
	if (controlPoints.size() != 0)
	{
		GLfloat dertX, dertY, xTemp, yTemp;
		dertX = ((GLfloat)CHANGE_POINT_DIS) / WIDTH_HALF, dertY = ((GLfloat)CHANGE_POINT_DIS) / HEIGHT_HALF;
		glBegin(GL_QUADS);
		for (GLint i = 0; i < controlPoints.size(); i++)
		{
			xTemp = ((GLfloat)controlPoints[i].x) / WIDTH_HALF, yTemp = ((GLfloat)controlPoints[i].y) / HEIGHT_HALF;
			glVertex2f(xTemp - dertX, yTemp + dertY);
			glVertex2f(xTemp + dertX, yTemp + dertY);
			glVertex2f(xTemp + dertX, yTemp - dertY);
			glVertex2f(xTemp - dertX, yTemp - dertY);
		}
		glEnd();
		glLineStipple(2, 0x5555);
		glEnable(GL_LINE_STIPPLE);
		glBegin(GL_LINE_STRIP);
		for (GLint i = 0; i < controlPoints.size(); i++)
		{
			xTemp = ((GLfloat)controlPoints[i].x) / WIDTH_HALF, yTemp = ((GLfloat)controlPoints[i].y) / HEIGHT_HALF;
			glVertex2f(xTemp, yTemp);
		}
		glEnd();
		glDisable(GL_LINE_STIPPLE);

		glFlush();
	}
	if (transformStatus == CLIP && (cliping || (!cliping && myClipWindow.windowWidthHalf != -1)))
	{
		GLfloat x1, y1, x2, y2, x3, y3, x4, y4, dertX, dertY;
		dertX = ((GLfloat)CHANGE_POINT_DIS) / WIDTH_HALF, dertY = ((GLfloat)CHANGE_POINT_DIS) / HEIGHT_HALF;
		GLint centerX, centerY, heightHalf, widthHalf;
		centerX = myClipWindow.clipWindowCenter.x, centerY = myClipWindow.clipWindowCenter.y;
		heightHalf = myClipWindow.windowHeightHalf, widthHalf = myClipWindow.windowWidthHalf;
		x1 = ((GLfloat)(centerX - widthHalf)) / WIDTH_HALF, y1 = ((GLfloat)(centerY + heightHalf)) / HEIGHT_HALF;
		x2 = ((GLfloat)(centerX + widthHalf)) / WIDTH_HALF, y2 = ((GLfloat)(centerY + heightHalf)) / HEIGHT_HALF;
		x3 = ((GLfloat)(centerX + widthHalf)) / WIDTH_HALF, y3 = ((GLfloat)(centerY - heightHalf)) / HEIGHT_HALF;
		x4 = ((GLfloat)(centerX - widthHalf)) / WIDTH_HALF, y4 = ((GLfloat)(centerY - heightHalf)) / HEIGHT_HALF;
		glBegin(GL_QUADS);
		glVertex2f(x1 - dertX, y1 + dertY);
		glVertex2f(x1 + dertX, y1 + dertY);
		glVertex2f(x1 + dertX, y1 - dertY);
		glVertex2f(x1 - dertX, y1 - dertY);

		glVertex2f(x2 - dertX, y2 + dertY);
		glVertex2f(x2 + dertX, y2 + dertY);
		glVertex2f(x2 + dertX, y2 - dertY);
		glVertex2f(x2 - dertX, y2 - dertY);

		glVertex2f(x3 - dertX, y3 + dertY);
		glVertex2f(x3 + dertX, y3 + dertY);
		glVertex2f(x3 + dertX, y3 - dertY);
		glVertex2f(x3 - dertX, y3 - dertY);

		glVertex2f(x4 - dertX, y4 + dertY);
		glVertex2f(x4 + dertX, y4 + dertY);
		glVertex2f(x4 + dertX, y4 - dertY);
		glVertex2f(x4 - dertX, y4 - dertY);
		glEnd();
		glLineStipple(2, 0x5555);
		glEnable(GL_LINE_STIPPLE);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glVertex2f(x3, y3);
		glVertex2f(x4, y4);
		glEnd();
		glDisable(GL_LINE_STIPPLE);
		glFlush();
	}
	glutSwapBuffers();
}
void splineOnMouseClick(int button, int state, int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	switch (transformStatus)
	{
	case drawSpline::EDIT:
		if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
		{
			if (mySpline.getPointsNum() <= 1)
			{
				drawStatus = 1;
				if (controlPoints.size() >= drawingPointIndex)
				{
					controlPoints[drawingPointIndex - 1].x = x;
					controlPoints[drawingPointIndex - 1].y = y;
					mySpline.setControlPoints(glm::ivec3(x, y, 0), drawingPointIndex);
				}
				else
				{
					controlPoints.push_back(glm::ivec3(x, y, 0));
					mySpline.setControlPoints(glm::ivec3(x, y, 0), drawingPointIndex);
				}
				if (drawingPointIndex == 1)
					drawingPointIndex++;
			}
			else
			{
				drawStatus = 2;
				GLint xTemp, yTemp;
				GLint i;
				for (i = 0; i < 4; i++)
				{
					xTemp = controlPoints[i].x, yTemp = controlPoints[i].y;
					if (x <= xTemp + CHANGE_POINT_DIS&&x >= xTemp - CHANGE_POINT_DIS
						&&y <= yTemp + CHANGE_POINT_DIS&&y >= yTemp - CHANGE_POINT_DIS)
						break;
				}
				if (i != 4)
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
					if (controlPoints[drawingPointIndex - 2].x == x
						&&controlPoints[drawingPointIndex - 2].y == y)
					{
						drawingPointIndex--;
						controlPoints.clear();
						drawStatus = 0;
						return;
					}
				}
				if (drawStatus == 1 && drawingPointIndex <= 3)
				{
					if (controlPoints.size() < drawingPointIndex)
					{
						controlPoints.push_back(glm::ivec3(x, y, 0));
					}
					drawingPointIndex++;
				}
				else if ((drawStatus == 1 && drawingPointIndex == 4) || drawStatus == 2)
				{
					mySpline.setControlPoints(glm::ivec3(x, y, 0), drawingPointIndex);
					mySpline.splineUseLine();
					glBindBuffer(GL_ARRAY_BUFFER, myVBO);
					glBufferData(GL_ARRAY_BUFFER, mySpline.getPointsNum()*mySpline.getPointSize(),
						mySpline.getSplinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
			}
		}
		break;
	case drawSpline::MOVE:
		if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
		{
			lastMouseX = x, lastMouseY = y;
		}
		else if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
		{
			splineGetTransformMatrix(glm::ivec2(x - lastMouseX, y - lastMouseY));
			lastMouseX = x, lastMouseY = y;
			mySpline.splineUseLine();
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, mySpline.getPointsNum()*mySpline.getPointSize(),
				mySpline.getSplinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		break;
	case drawSpline::ROTATE:
		if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
		{
			lastMouseX = x, lastMouseY = y;
		}
		else if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
		{
			splineGetTransformMatrix(glm::ivec2(lastMouseX, lastMouseY), glm::ivec2(x, y));
			lastMouseX = x, lastMouseY = y;
			mySpline.splineUseLine();
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, mySpline.getPointsNum()*mySpline.getPointSize(),
				mySpline.getSplinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			splineSetTransBasisPoint();
		}
		break;
	case drawSpline::CLIP:
		if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
		{
			if (myClipWindow.windowWidthHalf == -1)
			{
				cliping = GL_TRUE;
				myClipWindow.clipWindowCenter.x = x, myClipWindow.clipWindowCenter.y = y;
				myClipWindow.windowWidthHalf = 0, myClipWindow.windowHeightHalf = 0;
			}
			else
			{
				GLint x1, y1, x2, y2, x3, y3, x4, y4;
				GLint centerX, centerY, heightHalf, widthHalf;
				centerX = myClipWindow.clipWindowCenter.x, centerY = myClipWindow.clipWindowCenter.y;
				heightHalf = myClipWindow.windowHeightHalf, widthHalf = myClipWindow.windowWidthHalf;
				x1 = centerX - widthHalf, y1 = centerY + heightHalf;
				x2 = centerX + widthHalf, y2 = centerY + heightHalf;
				x3 = centerX + widthHalf, y3 = centerY - heightHalf;
				x4 = centerX - widthHalf, y4 = centerY - heightHalf;
				if ((x <= x1 + CHANGE_POINT_DIS&&x >= x1 - CHANGE_POINT_DIS&&y <= y1 + CHANGE_POINT_DIS&&y >= y1 - CHANGE_POINT_DIS)
					|| (x <= x2 + CHANGE_POINT_DIS&&x >= x2 - CHANGE_POINT_DIS&&y <= y2 + CHANGE_POINT_DIS&&y >= y2 - CHANGE_POINT_DIS)
					|| (x <= x3 + CHANGE_POINT_DIS&&x >= x3 - CHANGE_POINT_DIS&&y <= y3 + CHANGE_POINT_DIS&&y >= y3 - CHANGE_POINT_DIS)
					|| (x <= x4 + CHANGE_POINT_DIS&&x >= x4 - CHANGE_POINT_DIS&&y <= y4 + CHANGE_POINT_DIS&&y >= y4 - CHANGE_POINT_DIS))
				{
					cliping = GL_TRUE;
				}
			}
		}
		else if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
		{
			if (cliping)
			{
				cliping = GL_FALSE;
				glm::ivec3 windowP1, windowP2;
				myClipWindow.windowWidthHalf = abs(x - myClipWindow.clipWindowCenter.x);
				myClipWindow.windowHeightHalf = abs(y - myClipWindow.clipWindowCenter.y);
				windowP1 = glm::ivec3(myClipWindow.clipWindowCenter.x - myClipWindow.windowWidthHalf,
					myClipWindow.clipWindowCenter.y - myClipWindow.windowHeightHalf, 0);
				windowP2 = glm::ivec3(myClipWindow.clipWindowCenter.x + myClipWindow.windowWidthHalf,
					myClipWindow.clipWindowCenter.y + myClipWindow.windowHeightHalf, 0);
				mySpline.clipUseRect(windowP1, windowP2);
				glBindBuffer(GL_ARRAY_BUFFER, myVBO);
				glBufferData(GL_ARRAY_BUFFER, mySpline.getPointsNum()*mySpline.getPointSize(),
					mySpline.getSplinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}
		break;
	}
	
}
void splineOnActiveMotion(int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	switch (transformStatus)
	{
	case drawSpline::EDIT:
		if (drawStatus != 0)
		{
			if (controlPoints.size() >= drawingPointIndex)
			{
				controlPoints[drawingPointIndex - 1].x = x;
				controlPoints[drawingPointIndex - 1].y = y;
			}
			else
			{
				controlPoints.push_back(glm::ivec3(x, y, 0));
			}
			mySpline.setControlPoints(glm::ivec3(x, y, 0), drawingPointIndex);
			if (drawStatus == 2)
			{
				mySpline.splineUseLine();
				glBindBuffer(GL_ARRAY_BUFFER, myVBO);
				glBufferData(GL_ARRAY_BUFFER, mySpline.getPointsNum()*mySpline.getPointSize(),
					mySpline.getSplinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}
		break;
	case drawSpline::MOVE:
		splineGetTransformMatrix(glm::ivec2(x - lastMouseX, y - lastMouseY));
		lastMouseX = x, lastMouseY = y;
		mySpline.splineUseLine();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, mySpline.getPointsNum()*mySpline.getPointSize(),
			mySpline.getSplinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	case drawSpline::ROTATE:
		splineGetTransformMatrix(glm::ivec2(lastMouseX, lastMouseY), glm::ivec2(x, y));
		mySpline.splineUseLine();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, mySpline.getPointsNum()*mySpline.getPointSize(),
			mySpline.getSplinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	case drawSpline::CLIP:
		if (cliping)
		{
			myClipWindow.windowWidthHalf = abs(x - myClipWindow.clipWindowCenter.x);
			myClipWindow.windowHeightHalf = abs(y - myClipWindow.clipWindowCenter.y);
		}
		break;
	}
	
}
void splineOnMouseWheelScrollValid(int wheel, int direction, int x, int y)
{
	splineGetTransformMatrix(glm::ivec2(direction * 2, direction * 2));
	mySpline.splineUseLine();
	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, mySpline.getPointsNum()*mySpline.getPointSize(),
		mySpline.getSplinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void splineOnMouseWheelScrollInvalid(int wheel, int direction, int x, int y){}
void splineProcessMenuEvent(int options)
{
	GLint n;
	std::string filePath;
	if (mySpline.getPointsNum() == 0 && options != OPENFILE)//在曲线未初始化时，不能对曲线进行变换
		return;
	switch (options)
	{
	case EDIT:
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		glutMouseWheelFunc(splineOnMouseWheelScrollInvalid);
		transformStatus = EDIT;
		splineSetTransBasisPoint();
		break;
	case MOVE:
		glutSetCursor(GLUT_CURSOR_CYCLE);
		glutMouseWheelFunc(splineOnMouseWheelScrollInvalid);
		transformStatus = MOVE;
		splineSetTransBasisPoint();
		break;
	case ROTATE:
		glutSetCursor(GLUT_CURSOR_WAIT);
		glutMouseWheelFunc(splineOnMouseWheelScrollInvalid);
		transformStatus = ROTATE;
		rotateCenter.x = 0, rotateCenter.y = 0;
		n = controlPoints.size();
		for (int i = 0; i < n; i++)
			rotateCenter.x += controlPoints[i].x, rotateCenter.y += controlPoints[i].y;
		rotateCenter.x /= n, rotateCenter.y /= n;
		splineSetTransBasisPoint();
		break;
	case ZOOM:
		glutSetCursor(GLUT_CURSOR_SPRAY);
		glutMouseWheelFunc(splineOnMouseWheelScrollValid);
		transformStatus = ZOOM;
		rotateCenter.x = 0, rotateCenter.y = 0;
		n = controlPoints.size();
		for (int i = 0; i < n; i++)
			rotateCenter.x += controlPoints[i].x, rotateCenter.y += controlPoints[i].y;
		rotateCenter.x /= n, rotateCenter.y /= n;
		splineSetTransBasisPoint();
		break;
	case SAVEFILE:
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		glutMouseWheelFunc(splineOnMouseWheelScrollInvalid);
		transformStatus = SAVEFILE;
		cout << "请输入保存文件的路径:" << endl;
		cin >> filePath;
		mySpline.saveSplineIntoFile(filePath);
		break;
	case OPENFILE:
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		glutMouseWheelFunc(splineOnMouseWheelScrollInvalid);
		transformStatus = OPENFILE;
		cout << "请输入打开文件的路径:" << endl;
		cin >> filePath;
		mySpline.loadSplineFromFile(filePath);
		mySpline.splineUseLine();
		controlPoints=mySpline.getControlPoints();
		splineSetTransBasisPoint();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, mySpline.getPointsNum()*mySpline.getPointSize(),
			mySpline.getSplinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	case CLIP:
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		glutMouseWheelFunc(splineOnMouseWheelScrollInvalid);
		transformStatus = CLIP;
		myClipWindow.windowHeightHalf = -1;
		myClipWindow.windowWidthHalf = -1;
		myClipWindow.clipWindowCenter = glm::ivec3(0, 0, 0);
		break;
	case EXIT:
		transformStatus = EXIT;
		break;
	}
}
void splineOnReshape(int width, int height)
{

}
void splineInitGlutWindow()
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
void splineInitMenus()
{
	GLint subMenuFile = glutCreateMenu(splineProcessMenuEvent);
	glutSetMenuFont(subMenuFile, GLUT_BITMAP_9_BY_15);
	glutAddMenuEntry("Save File", SAVEFILE);
	glutAddMenuEntry("Open File", OPENFILE);
	GLint menu = glutCreateMenu(splineProcessMenuEvent);
	glutSetMenuFont(menu, GLUT_BITMAP_9_BY_15);
	glutAddMenuEntry("Edit Spline", EDIT);
	glutAddMenuEntry("Move Spline", MOVE);
	glutAddMenuEntry("Rotate Spline", ROTATE);
	glutAddMenuEntry("Zoom Spline", ZOOM);
	glutAddMenuEntry("Clip Spline", CLIP);
	glutAddSubMenu("File", subMenuFile);
	glutAddMenuEntry("Exit", EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutSetCursor(GLUT_CURSOR_INHERIT);
}
void splineInitVAO(GLuint &VAO, GLuint &VBO)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//set vertex buffer and attribute point
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	mySpline = spline();
	glBufferData(GL_ARRAY_BUFFER, mySpline.getPointsNum()*mySpline.getPointSize(),
		mySpline.getSplinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//unbind VBO&VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void splineGetTransformMatrix(glm::ivec2 transformInfo, glm::ivec2 rotateInfo)
{
	//当处于移动状态时，传入transformInfo代表x，y方向上的移动距离，rotateInfo不传入数据
	//当处于旋转状态时，传入transformInfo、rotateInfo分别代表旋转开始时的鼠标位置和当前鼠标位置
	//当处于缩放状态时，传入transformInfo代表x，y方向上的缩放百分比，rotateInfo不传入数据
	glm::mat4 transformMat;
	glm::vec2 vecA, vecB;
	switch (transformStatus)
	{
	case drawSpline::MOVE:
		transformMat = glm::translate(transformMat, glm::vec3(transformInfo.x, transformInfo.y, 0));
		break;
	case drawSpline::ROTATE:
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
	case drawSpline::ZOOM:
		transformMat = glm::translate(transformMat, glm::vec3(rotateCenter.x, rotateCenter.y, 0));
		transformMat = glm::scale(transformMat, glm::vec3((100 + transformInfo.x) / 100.0, (100 + transformInfo.y) / 100.0, 0));
		transformMat = glm::translate(transformMat, glm::vec3(-rotateCenter.x, -rotateCenter.y, 0));
		break;
	}
	glm::vec4 tempPoint = glm::vec4(0, 0, 0, 1);
	if (transformStatus == drawSpline::ROTATE)
	{
		for (int i = 0; i < transBasisPoint.size(); i++)
		{
			tempPoint.x = transBasisPoint[i].x, tempPoint.y = transBasisPoint[i].y;
			tempPoint = transformMat*tempPoint;
			controlPoints[i].x = (GLint)tempPoint.x, controlPoints[i].y = (GLint)tempPoint.y;
			mySpline.setControlPoints(controlPoints[i], i + 1);
		}
	}
	else
	{
		for (int i = 0; i < controlPoints.size(); i++)
		{
			tempPoint.x = controlPoints[i].x, tempPoint.y = controlPoints[i].y;
			tempPoint = transformMat*tempPoint;
			controlPoints[i].x = (GLint)tempPoint.x, controlPoints[i].y = (GLint)tempPoint.y;
			mySpline.setControlPoints(controlPoints[i], i + 1);
		}
	}
}
void splineSetTransBasisPoint()
{
	for (int i = 0; i < transBasisPoint.size(); i++)
	{
		transBasisPoint[i].x = controlPoints[i].x;
		transBasisPoint[i].y = controlPoints[i].y;
	}
}


