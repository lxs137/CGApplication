#include <iostream>
#include <fstream>
#include <vector>
#include "drawEllipse.h"
#include "ellipse.h"
#include "shader.h"
#include "textureManager.h"
#include "windowSetting.h"

#include <freeglut\freeglut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
using namespace drawEllipse;

ellipse myEllipse;

void drawEllipseApplication(int argc, char **argv)
{

	if (glutGet(GLUT_INIT_STATE) != 1)
	{
		glutInit(&argc, argv);
		ellipseInitGlutWindow();
	}

	ellipseInitMenus();
	//set shader program	
	shader *myShader = new shader("2dModel.vert", "2dModel.frag");
	myShaderProgram = myShader->shaderProgram;
	myTextureManager = new TextureManager();

	//set VAO
	ellipseInitVAO(myVAO, myVBO);

	//set transform
	textureSwitchLoc = glGetUniformLocation(myShaderProgram, "isUseTexture");

	glEnable(GL_TEXTURE_2D);
	glutDisplayFunc(ellipseRender2DSence);
	glutIdleFunc(ellipseRender2DSence);
	glutReshapeFunc(ellipseOnReshape);
	glutMouseFunc(ellipseOnMouseClick);
	glutMotionFunc(ellipseOnActiveMotion);
	while (GL_TRUE)
	{
		glutMainLoopEvent();
		if (transformStatus == EXIT)
			break;
		glutPostRedisplay();
	}
}

void ellipseRender2DSence()
{
	glClearColor(1.0f, 0.8f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (filling == 2)
	{
		glUniform1i(textureSwitchLoc, 1);
	}
	else
		glUniform1i(textureSwitchLoc, 0);
	myTextureManager->bindTexture(textureID);
	glUseProgram(myShaderProgram);
	glBindVertexArray(myVAO);
	glDrawArrays(GL_POINTS, 0, myEllipse.getPointsNum());
	glBindVertexArray(0);
	if ((!drawing) && myEllipse.getPointsNum() >= 1)
	{
		GLfloat x1, y1, x2, y2, x3, y3, x4, y4, dertX, dertY;
		controlPoints = myEllipse.getControlPoints();
		x1 = ((GLfloat)controlPoints[0].x) / WIDTH_HALF, y1 = ((GLfloat)controlPoints[0].y) / HEIGHT_HALF;
		x2 = ((GLfloat)controlPoints[1].x) / WIDTH_HALF, y2 = ((GLfloat)controlPoints[1].y) / HEIGHT_HALF;
		x3 = ((GLfloat)controlPoints[2].x) / WIDTH_HALF, y3 = ((GLfloat)controlPoints[2].y) / HEIGHT_HALF;
		x4 = ((GLfloat)controlPoints[3].x) / WIDTH_HALF, y4 = ((GLfloat)controlPoints[3].y) / HEIGHT_HALF;
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
void ellipseOnMouseClick(int button, int state, int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	switch (transformStatus)
	{
	case drawEllipse::EDIT:
		if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
		{
			if (myEllipse.getPointsNum() <= 1)
			{
				drawing = GL_TRUE;
				myEllipse.setCenter(glm::ivec3(x, y, 0));
				myEllipse.setRadius(0, 0);
			}
			else
			{
				GLint x1, y1, x2, y2, x3, y3, x4, y4;
				controlPoints = myEllipse.getControlPoints();
				x1 = controlPoints[0].x, y1 = controlPoints[0].y;
				x2 = controlPoints[1].x, y2 = controlPoints[1].y;
				x3 = controlPoints[2].x, y3 = controlPoints[2].y;
				x4 = controlPoints[3].x, y4 = controlPoints[3].y;
				if ((x <= x1 + CHANGE_POINT_DIS&&x >= x1 - CHANGE_POINT_DIS&&y <= y1 + CHANGE_POINT_DIS&&y >= y1 - CHANGE_POINT_DIS)
					|| (x <= x2 + CHANGE_POINT_DIS&&x >= x2 - CHANGE_POINT_DIS&&y <= y2 + CHANGE_POINT_DIS&&y >= y2 - CHANGE_POINT_DIS)
					|| (x <= x3 + CHANGE_POINT_DIS&&x >= x3 - CHANGE_POINT_DIS&&y <= y3 + CHANGE_POINT_DIS&&y >= y3 - CHANGE_POINT_DIS)
					|| (x <= x4 + CHANGE_POINT_DIS&&x >= x4 - CHANGE_POINT_DIS&&y <= y4 + CHANGE_POINT_DIS&&y >= y4 - CHANGE_POINT_DIS))
				{
					drawing = GL_TRUE;
				}
			}
		}
		else if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
		{
			if (drawing)
			{
				drawing = GL_FALSE;
				GLint rX, rY;
				if (fabs(myEllipse.getRotateDegree()) < 0.02)
				{
					rX = abs(x - myEllipse.getCenter().x), rY = abs(y - myEllipse.getCenter().y);
					myEllipse.setRadius(rX, rY);
				}
				else
				{
					GLfloat degree = myEllipse.getRotateDegree();
					if (fabs(degree - 1.57) < 0.01)
						degree = (degree < 1.57) ? 1.56 : 1.58;
					GLint x1 = x, y1 = y, x0 = myEllipse.getCenter().x, y0 = myEllipse.getCenter().y;
					x = (x1 + tan(degree)*tan(degree)*x0 - fabs(tan(degree))*(y0 - y1 + cos(degree))) / (1 + tan(degree)*tan(degree));
					y = fabs(tan(degree))*(x - x0) + y0;
					rX = sqrt((x0 - x)*(x0 - x) + (y0 - y)*(y0 - y));
					rY = sqrt((x1 - x)*(x1 - x) + (y1 - y)*(y1 - y));
					if (degree < 1.57)
						myEllipse.setRadius(rX, rY);
					else
						myEllipse.setRadius(rY, rX);
				}
				myEllipse.ellipseUseMidpoint();
				if(filling != 0)
					myEllipse.fillEllipseScanLine();
				glBindBuffer(GL_ARRAY_BUFFER, myVBO);
				glBufferData(GL_ARRAY_BUFFER, myEllipse.getPointsNum()*myEllipse.getPointSize(),
					myEllipse.getEllipsePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}
		break;
	case drawEllipse::MOVE:
		if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
		{
			lastMouseX = x, lastMouseY = y;
		}
		else if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
		{
			ellipseGetTransformMatrix(glm::ivec2(x - lastMouseX, y - lastMouseY));
			lastMouseX = x, lastMouseY = y;
			myEllipse.ellipseUseMidpoint();
			if(filling != 0)
				myEllipse.fillEllipseScanLine();
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, myEllipse.getPointsNum()*myEllipse.getPointSize(),
				myEllipse.getEllipsePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		break;
	case drawEllipse::ROTATE:
		if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
		{
			lastMouseX = x, lastMouseY = y;
		}
		else if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
		{
			ellipseGetTransformMatrix(glm::ivec2(lastMouseX, lastMouseY), glm::ivec2(x, y));
			lastMouseX = x, lastMouseY = y;
			myEllipse.ellipseUseMidpoint();
			if(filling != 0)
				myEllipse.fillEllipseScanLine();
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, myEllipse.getPointsNum()*myEllipse.getPointSize(),
				myEllipse.getEllipsePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		break;
	}
	
}
void ellipseOnActiveMotion(int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	switch (transformStatus)
	{
	case drawEllipse::EDIT:
		if (drawing)
		{
			GLint rX, rY;
			if (fabs(myEllipse.getRotateDegree()) < 0.02)
			{				
				rX = abs(x - myEllipse.getCenter().x), rY = abs(y - myEllipse.getCenter().y);
				myEllipse.setRadius(rX, rY);
			}
			else
			{
				GLfloat degree = myEllipse.getRotateDegree();
				if (fabs(degree - 1.57) < 0.01)
					degree = (degree < 1.57) ? 1.56 : 1.58;
				GLint x1 = x, y1 = y, x0 = myEllipse.getCenter().x, y0 = myEllipse.getCenter().y;
				x = (x1 + tan(degree)*tan(degree)*x0 - fabs(tan(degree))*(y0 - y1 + cos(degree))) / (1 + tan(degree)*tan(degree));
				y = fabs(tan(degree))*(x - x0) + y0;
				rX = sqrt((x0 - x)*(x0 - x) + (y0 - y)*(y0 - y));
				rY = sqrt((x1 - x)*(x1 - x) + (y1 - y)*(y1 - y));
				if (degree < 1.57)
					myEllipse.setRadius(rX, rY);
				else
					myEllipse.setRadius(rY, rX);
			}
			myEllipse.ellipseUseMidpoint();
			if(filling != 0)
				myEllipse.fillEllipseScanLine();
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, myEllipse.getPointsNum()*myEllipse.getPointSize(),
				myEllipse.getEllipsePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		break;
	case drawEllipse::MOVE:
		ellipseGetTransformMatrix(glm::ivec2(x - lastMouseX, y - lastMouseY));
		lastMouseX = x, lastMouseY = y;
		myEllipse.ellipseUseMidpoint();
		if(filling != 0)
			myEllipse.fillEllipseScanLine();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, myEllipse.getPointsNum()*myEllipse.getPointSize(),
			myEllipse.getEllipsePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	case drawEllipse::ROTATE:
		ellipseGetTransformMatrix(glm::ivec2(lastMouseX, lastMouseY), glm::ivec2(x, y));
		lastMouseX = x, lastMouseY = y;
		myEllipse.ellipseUseMidpoint();
		if(filling != 0)
			myEllipse.fillEllipseScanLine();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, myEllipse.getPointsNum()*myEllipse.getPointSize(),
			myEllipse.getEllipsePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	}
	
}
void ellipseOnMouseWheelScrollValid(int wheel, int direction, int x, int y)
{
	ellipseGetTransformMatrix(glm::ivec2(direction * 4, direction * 4));
	if (filling != 0)
	{
		myEllipse.fillEllipseScanLine();
	}
	else
		myEllipse.ellipseUseMidpoint();
	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, myEllipse.getPointsNum()*myEllipse.getPointSize(),
		myEllipse.getEllipsePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void ellipseOnMouseWheelScrollInvalid(int wheel, int direction, int x, int y)
{
}
void ellipseProcessMenuEvent(int options)
{
	glm::vec3 fillColor;
	switch (options)
	{
	case EDIT:
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		glutMouseWheelFunc(ellipseOnMouseWheelScrollInvalid);
		transformStatus = EDIT;
		break;
	case MOVE:
		glutSetCursor(GLUT_CURSOR_CYCLE);
		glutMouseWheelFunc(ellipseOnMouseWheelScrollInvalid);
		transformStatus = MOVE;
		break;
	case ROTATE:
		glutSetCursor(GLUT_CURSOR_WAIT);
		glutMouseWheelFunc(ellipseOnMouseWheelScrollInvalid);
		transformStatus = ROTATE;
		break;
	case ZOOM:
		glutSetCursor(GLUT_CURSOR_SPRAY);
		glutMouseWheelFunc(ellipseOnMouseWheelScrollValid);
		transformStatus = ZOOM;
		break;
	case FILLCOLOR:
		glutMouseWheelFunc(ellipseOnMouseWheelScrollValid);
		filling = (filling == 1) ? 0 : 1;
		if (filling == 1)
		{
			GLint RValue, GValue, BValue;
			cout << "输入要填充的颜色值(例如：255 0 0):" << endl;
			cin >> RValue >> GValue >> BValue;
			fillColor = glm::vec3(RValue / 255.0, GValue / 255.0, BValue / 255.0);
			myEllipse.setFillColor(fillColor);
			myEllipse.fillEllipseScanLine();
		}
		else
			myEllipse.ellipseUseMidpoint();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, myEllipse.getPointsNum()*myEllipse.getPointSize(),
			myEllipse.getEllipsePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	case FILLPICTURE:
		glutMouseWheelFunc(ellipseOnMouseWheelScrollValid);
		filling = (filling == 2) ? 0 : 2;
		if (filling == 2)
		{
			string filename;
			cout << "输入要填充图片的路径:";
			cin >> filename;
			myTextureManager->loadTexture(filename.c_str(), textureID);
			myEllipse.fillEllipseScanLine();
		}
		else
			myEllipse.ellipseUseMidpoint();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, myEllipse.getPointsNum()*myEllipse.getPointSize(),
			myEllipse.getEllipsePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	case EXIT:
		transformStatus = EXIT;
		break;
	}
}
void ellipseOnReshape(int width, int height)
{

}
void ellipseInitGlutWindow()
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
void ellipseInitMenus()
{
	GLint subMenu = glutCreateMenu(ellipseProcessMenuEvent);
	glutSetMenuFont(subMenu, GLUT_BITMAP_9_BY_15);
	glutAddMenuEntry("Fill with color", FILLCOLOR);
	glutAddMenuEntry("Fill with picture", FILLPICTURE);
	GLint menu = glutCreateMenu(ellipseProcessMenuEvent);
	glutSetMenuFont(menu, GLUT_BITMAP_9_BY_15);
	glutAddMenuEntry("Edit Ellipse", EDIT);
	glutAddMenuEntry("Move Ellipse", MOVE);
	glutAddMenuEntry("Rotate Ellipse", ROTATE);
	glutAddMenuEntry("Zoom Ellipse", ZOOM);
	glutAddSubMenu("Fill Ellipse", subMenu);
	glutAddMenuEntry("Exit", EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutSetCursor(GLUT_CURSOR_INHERIT);
}
void ellipseInitVAO(GLuint &VAO, GLuint &VBO)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//set vertex buffer and attribute point
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	myEllipse = ellipse();
	glBufferData(GL_ARRAY_BUFFER, myEllipse.getPointsNum()*myEllipse.getPointSize(),
		myEllipse.getEllipsePixels().begin()._Ptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//unbind VBO&VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void ellipseGetTransformMatrix(glm::ivec2 transformInfo, glm::ivec2 rotateInfo)
{
	//当处于移动状态时，传入transformInfo代表x，y方向上的移动距离，rotateInfo不传入数据
	//当处于旋转状态时，传入transformInfo、rotateInfo分别代表旋转开始时的鼠标位置和当前鼠标位置
	//当处于缩放状态时，传入transformInfo代表x，y方向上的缩放百分比，rotateInfo不传入数据
	glm::mat4 transformMat;
	glm::vec2 vecA, vecB;
	glm::ivec3 center = myEllipse.getCenter();
	switch (transformStatus)
	{
	case drawEllipse::MOVE:
		center.x += transformInfo.x, center.y += transformInfo.y;
		myEllipse.setCenter(center);
		break;
	case drawEllipse::ROTATE:
		GLfloat degree;//弧度值
		vecA = glm::vec2(transformInfo.x - center.x, transformInfo.y - center.y);
		vecB = glm::vec2(rotateInfo.x - center.x, rotateInfo.y - center.y);
		degree = acos((vecA.x*vecB.x + vecA.y*vecB.y) /
			sqrt((vecA.x*vecA.x + vecA.y*vecA.y)*(vecB.x*vecB.x + vecB.y*vecB.y)));
		if (vecA.x*vecB.y - vecA.y*vecB.x < 0)
			degree = -degree;
		myEllipse.addRotateDegree(degree);
		break;
	case drawEllipse::ZOOM:
		myEllipse.setRadius(myEllipse.getRadius().x*(100 + transformInfo.x) / 100.0,
			myEllipse.getRadius().y*(100 + transformInfo.y) / 100.0);
		break;
	}
}


