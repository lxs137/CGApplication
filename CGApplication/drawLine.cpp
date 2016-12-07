#include <iostream>
#include <fstream>
#include <vector>
#include "drawLine.h"
#include "line.h"
#include "shader.h"
#include "textureManager.h"
#include "windowSetting.h"

#include <freeglut\freeglut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
using namespace drawLine;

line myLine;

void drawLineApplication(int argc, char **argv)
{
	if (glutGet(GLUT_INIT_STATE) != 1)
	{
		glutInit(&argc, argv);
		lineInitGlutWindow();
	}

	lineInitMenus();
	transBasisPoint.push_back(glm::ivec3(0, 0, 0));
	transBasisPoint.push_back(glm::ivec3(0, 0, 0));

	//set shader program	
	shader *myShader = new shader("2dModel.vert", "2dModel.frag");
	myShaderProgram = myShader->shaderProgram;

	//set VAO
	lineInitVAO(myVAO,myVBO);

	//set transform
	GLuint transformLocation = glGetUniformLocation(myShaderProgram, "transform");

	glutDisplayFunc(lineRender2DSence);
	glutIdleFunc(lineRender2DSence);
	glutReshapeFunc(lineOnReshape);
	glutMouseFunc(lineOnMouseClick);
	glutMotionFunc(lineOnActiveMotion);
	while (GL_TRUE)
	{
		glutMainLoopEvent();
		if (transformStatus == EXIT)
			break;
		glutPostRedisplay();
	}
}

void lineRender2DSence()
{
	glClearColor(1.0f, 0.8f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(myShaderProgram);
	glBindVertexArray(myVAO);
	//glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));
	glDrawArrays(GL_POINTS, 0, myLine.getPointsNum());
	glBindVertexArray(0);
	if ((!drawing)&&myLine.getPointsNum() >= 1)
	{
		GLfloat x1, y1, x2, y2, dertX, dertY;
		x1 = ((GLfloat)myLine.getPoint(0).x)/WIDTH_HALF, y1 = ((GLfloat)myLine.getPoint(0).y)/HEIGHT_HALF;
		x2 = ((GLfloat)myLine.getPoint(1).x)/WIDTH_HALF, y2 = ((GLfloat)myLine.getPoint(1).y)/HEIGHT_HALF;
		dertX = ((GLfloat)CHANGE_POINT_DIS) / WIDTH_HALF, dertY = ((GLfloat)CHANGE_POINT_DIS) / HEIGHT_HALF;
		glBegin(GL_QUADS);
		glVertex2f(x1 - dertX, y1 + dertY);
		glVertex2f(x1 + dertX, y1 + dertY);
		glVertex2f(x1 + dertX, y1 - dertY);
		glVertex2f(x1 - dertX, y1 - dertY);
		glVertex2f(x2 - dertX, y2 + dertY);
		glVertex2f(x2 + dertX, y2 + dertY);
		glVertex2f(x2 + dertX, y2 - dertY);
		glVertex2f(x2 - dertX, y2 - dertY);
		glEnd();
		glFlush();
	}
	glutSwapBuffers();
}
void lineOnMouseClick(int button, int state, int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	switch (transformStatus)
	{
	case drawLine::EDIT:
		if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
		{
			if (myLine.getPointsNum() <= 1)
			{
				drawing = GL_TRUE;
				myLine.setPoint(glm::ivec3(x, y, 0), glm::ivec3(x, y, 0));
			}
			else
				drawing = myLine.checkChangePoint(glm::ivec3(x, y, 0));
		}
		else if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
		{
			if (drawing)
			{
				drawing = GL_FALSE;
				if (myLine.getPoint(0) == glm::ivec3(x, y, 0))
					return;
				myLine.setDrawingPoint(glm::ivec3(x, y, 0));
				myLine.lineUseBresenham();
				glBindBuffer(GL_ARRAY_BUFFER, myVBO);
				glBufferData(GL_ARRAY_BUFFER, myLine.getPointsNum()*myLine.getPointSize(),
					myLine.getLinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}
		break;
	case drawLine::MOVE:
		if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
		{
			lastMouseX = x, lastMouseY = y;
		}
		else if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
		{
			lineGetTransformMatrix(glm::ivec2(x - lastMouseX, y - lastMouseY));
			lineSetTransBasisPoint();
			myLine.lineUseBresenham();
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, myLine.getPointsNum()*myLine.getPointSize(),
				myLine.getLinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		break;
	case drawLine::ROTATE:
		if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
		{
			lastMouseX = x, lastMouseY = y;
		}
		else if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
		{
			lineGetTransformMatrix(glm::ivec2(lastMouseX, lastMouseY), glm::ivec2(x, y));
			lineSetTransBasisPoint();
			myLine.lineUseBresenham();
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, myLine.getPointsNum()*myLine.getPointSize(),
				myLine.getLinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		break;
	}
	
}
void lineOnActiveMotion(int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	switch (transformStatus)
	{
	case drawLine::EDIT:
		if (drawing)
		{
			myLine.setDrawingPoint(glm::ivec3(x, y, 0));
			myLine.lineUseBresenham();
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, myLine.getPointsNum()*myLine.getPointSize(),
				myLine.getLinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		break;
	case drawLine::MOVE:
		lineGetTransformMatrix(glm::ivec2(x - lastMouseX, y - lastMouseY));
		myLine.lineUseBresenham();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, myLine.getPointsNum()*myLine.getPointSize(),
			myLine.getLinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	case drawLine::ROTATE:
		lineGetTransformMatrix(glm::ivec2(lastMouseX, lastMouseY), glm::ivec2(x, y));
		myLine.lineUseBresenham();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, myLine.getPointsNum()*myLine.getPointSize(),
			myLine.getLinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	}
	
}
void lineOnMouseWheelScrollValid(int wheel, int direction, int x, int y)
{
	lineGetTransformMatrix(glm::ivec2(direction*2,direction*2));
	lineSetTransBasisPoint();
	rotateCenter.x = (transBasisPoint[0].x + transBasisPoint[1].x) / 2;
	rotateCenter.y = (transBasisPoint[0].y + transBasisPoint[1].y) / 2;
	myLine.lineUseBresenham();
	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, myLine.getPointsNum()*myLine.getPointSize(),
		myLine.getLinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void lineOnMouseWheelScrollInvalid(int wheel, int direction, int x, int y)
{
}
void lineProcessMenuEvent(int options)
{
	switch (options)
	{
	case EDIT:
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		glutMouseWheelFunc(lineOnMouseWheelScrollInvalid);
		transformStatus = EDIT;
		break;
	case MOVE:
		glutSetCursor(GLUT_CURSOR_CYCLE);	
		glutMouseWheelFunc(lineOnMouseWheelScrollInvalid);
		transformStatus = MOVE;
		lineSetTransBasisPoint();
		break;
	case ROTATE:
		glutSetCursor(GLUT_CURSOR_WAIT);
		glutMouseWheelFunc(lineOnMouseWheelScrollInvalid);
		transformStatus = ROTATE;
		rotateCenter.x = (myLine.getPoint(0).x + myLine.getPoint(1).x) / 2;
		rotateCenter.y = (myLine.getPoint(0).y + myLine.getPoint(1).y) / 2;
		lineSetTransBasisPoint();
		break;
	case ZOOM:
		glutSetCursor(GLUT_CURSOR_SPRAY);
		glutMouseWheelFunc(lineOnMouseWheelScrollValid);
		transformStatus = ZOOM;
		rotateCenter.x = (myLine.getPoint(0).x + myLine.getPoint(1).x) / 2;
		rotateCenter.y = (myLine.getPoint(0).y + myLine.getPoint(1).y) / 2;
		lineSetTransBasisPoint();
		break;
	case EXIT:
		transformStatus = EXIT;
		break;
	}
}
void lineOnReshape(int width, int height)
{

}
void lineInitGlutWindow()
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
void lineInitMenus()
{
	GLint menu = glutCreateMenu(lineProcessMenuEvent);
	glutSetMenuFont(menu, GLUT_BITMAP_9_BY_15);
	glutAddMenuEntry("Edit Line", EDIT);
	glutAddMenuEntry("Move Line", MOVE);
	glutAddMenuEntry("Rotate Line", ROTATE);
	glutAddMenuEntry("Zoom Line", ZOOM);
	glutAddMenuEntry("Exit", EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutSetCursor(GLUT_CURSOR_INHERIT);
}
void lineInitVAO(GLuint &VAO,GLuint &VBO)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//set vertex buffer and attribute point
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	myLine = line();
	glBufferData(GL_ARRAY_BUFFER, myLine.getPointsNum()*myLine.getPointSize(),
		myLine.getLinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//unbind VBO&VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void lineGetTransformMatrix(glm::ivec2 transformInfo, glm::ivec2 rotateInfo)
{
	//当处于移动状态时，传入transformInfo代表x，y方向上的移动距离，rotateInfo不传入数据
	//当处于旋转状态时，传入transformInfo、rotateInfo分别代表旋转开始时的鼠标位置和当前鼠标位置
	//当处于缩放状态时，传入transformInfo代表x，y方向上的缩放百分比，rotateInfo不传入数据
	glm::mat4 transformMat;
	glm::vec2 vecA, vecB;
	switch (transformStatus)
	{
	case drawLine::MOVE:
		transformMat = glm::translate(transformMat, glm::vec3(transformInfo.x, transformInfo.y, 0));
		break;
	case drawLine::ROTATE:
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
	case drawLine::ZOOM:		
		transformMat = glm::translate(transformMat, glm::vec3(rotateCenter.x, rotateCenter.y, 0));
		transformMat = glm::scale(transformMat, glm::vec3((100 + transformInfo.x) / 100.0, (100 + transformInfo.y) / 100.0, 0));
		transformMat = glm::translate(transformMat, glm::vec3(-rotateCenter.x, -rotateCenter.y, 0));
		break;
	}
	glm::ivec3 point1, point2;
	point1 = transBasisPoint[0], point2 = transBasisPoint[1];
	glm::vec4 tempPoint = glm::vec4(point1.x, point1.y, 0, 1);
	tempPoint = transformMat*tempPoint;
	point1.x = (GLint)tempPoint.x, point1.y = (GLint)tempPoint.y;
	tempPoint = glm::vec4(point2.x, point2.y, 0, 1);
	tempPoint = transformMat*tempPoint;
	point2.x = (GLint)tempPoint.x, point2.y = (GLint)tempPoint.y;
	//cout << "point1:" << point1.x << "," << point1.y << "   ";
	//cout << "point2:" << point2.x << "," << point2.y << "   ";
	//cout << "centerX:" << (point1.x + point2.x) / 2 << "  " << (point1.y + point2.y) / 2 << endl;
	myLine.setPoint(point1, point2);
}
void lineSetTransBasisPoint()
{
	transBasisPoint[0] = myLine.getPoint(0);
	transBasisPoint[1] = myLine.getPoint(1);
}


