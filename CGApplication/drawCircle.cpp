#include <iostream>
#include <fstream>
#include <vector>
#include "drawCircle.h"
#include "circle.h"
#include "shader.h"
#include "textureManager.h"
#include "windowSetting.h"

#include <freeglut\freeglut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
using namespace drawCircle;

circle myCircle;

void drawCircleApplication(int argc, char **argv)
{
	if (glutGet(GLUT_INIT_STATE) != 1)
	{
		glutInit(&argc, argv);
		circleInitGlutWindow();
	}
	circleInitMenus();
	//set shader program    
	shader *myShader = new shader("2dModel.vert", "2dModel.frag");
	myShaderProgram = myShader->shaderProgram;

	//set VAO
	circleInitVAO(myVAO,myVBO);

	//set transform
	GLuint transformLocation = glGetUniformLocation(myShaderProgram, "transform");

	glutDisplayFunc(circleRender2DSence);
	glutIdleFunc(circleRender2DSence);
	glutMouseFunc(circleOnMouseClick);
	glutMotionFunc(circleOnActiveMotion);
	glutReshapeFunc(circleOnReshape);
	while (GL_TRUE)
	{
		glutMainLoopEvent();
		if (transformStatus == EXIT)
			break;
		glutPostRedisplay();
	}
}
void circleRender2DSence()
{
	glClearColor(1.0f, 0.8f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(myShaderProgram);
	//myTextureManager->bindTexture(testTextureID);
	glBindVertexArray(myVAO);
	//glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));
	glDrawArrays(GL_POINTS, 0, myCircle.getPointsNum());
	glBindVertexArray(0);

	if ((!drawing) && myCircle.getPointsNum() >= 1)
	{
		GLfloat x1, y1, x2, y2, x3, y3, x4, y4, dertX, dertY;
		controlPoints = myCircle.getControlPoints();
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
void circleOnMouseClick(int button, int state, int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	switch (transformStatus)
	{
	case drawCircle::EDIT:
		if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
		{
			if (myCircle.getPointsNum() <= 1)
			{
				drawing = GL_TRUE;
				myCircle.setCenter(glm::ivec3(x, y, 0));
				myCircle.setRadius(0);
			}
			else
			{
				GLint x1, y1, x2, y2, x3, y3, x4, y4;
				controlPoints = myCircle.getControlPoints();
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
				rX = abs(x - myCircle.getCenter().x), rY = abs(y - myCircle.getCenter().y);
				myCircle.setRadius(rX < rY ? rX : rY);
				myCircle.circleUseMidpoint();
				glBindBuffer(GL_ARRAY_BUFFER, myVBO);
				glBufferData(GL_ARRAY_BUFFER, myCircle.getPointsNum()*myCircle.getPointSize(),
					myCircle.getCirclePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}
		break;
	case drawCircle::MOVE:
		if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
		{
			lastMouseX = x, lastMouseY = y;
		}
		else if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
		{
			circleGetTransformMatrix(glm::ivec2(x - lastMouseX, y - lastMouseY));
			lastMouseX = x, lastMouseY = y;
			myCircle.circleUseMidpoint();
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, myCircle.getPointsNum()*myCircle.getPointSize(),
				myCircle.getCirclePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		break;
	}
	
}
void circleOnActiveMotion(int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	switch (transformStatus)
	{
	case drawCircle::EDIT:
		if (drawing)
		{
			GLint rX, rY;
			rX = abs(x - myCircle.getCenter().x), rY = abs(y - myCircle.getCenter().y);
			myCircle.setRadius(rX < rY ? rX : rY);
			myCircle.circleUseMidpoint();
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, myCircle.getPointsNum()*myCircle.getPointSize(),
				myCircle.getCirclePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		break;
	case drawCircle::MOVE:
		circleGetTransformMatrix(glm::ivec2(x - lastMouseX, y - lastMouseY));
		lastMouseX = x, lastMouseY = y;
		myCircle.circleUseMidpoint();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, myCircle.getPointsNum()*myCircle.getPointSize(),
			myCircle.getCirclePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	}

}
void circleProcessMenuEvent(int options)
{
	switch (options)
	{
	case EDIT:
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		glutMouseWheelFunc(circleOnMouseWheelScrollInvalid);
		transformStatus = EDIT;
		break;
	case MOVE:
		glutSetCursor(GLUT_CURSOR_CYCLE);
		glutMouseWheelFunc(circleOnMouseWheelScrollInvalid);
		transformStatus = MOVE;
		break;
	case ZOOM:
		glutSetCursor(GLUT_CURSOR_SPRAY);
		glutMouseWheelFunc(circleOnMouseWheelScrollValid);
		transformStatus = ZOOM;
		break;
	case EXIT:
		transformStatus = EXIT;
		break;
	}
}
void circleOnMouseWheelScrollValid(int wheel, int direction, int x, int y)
{
	circleGetTransformMatrix(glm::ivec2(direction * 4, direction * 4));
	myCircle.circleUseMidpoint();
	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, myCircle.getPointsNum()*myCircle.getPointSize(),
		myCircle.getCirclePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void circleOnMouseWheelScrollInvalid(int wheel, int direction, int x, int y)
{

}
void circleOnReshape(int width, int height)
{

}
void circleInitGlutWindow()
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
void circleLoadImageAsTexture(unsigned int testTextureID, const char *filename)
{
	myTextureManager->loadTexture(filename, testTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void circleInitVAO(GLuint &VAO, GLuint &VBO)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//set vertex buffer and attribute point
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	myCircle = circle();
	glBufferData(GL_ARRAY_BUFFER, myCircle.getPointsNum()*myCircle.getPointSize(),
		myCircle.getCirclePixels().begin()._Ptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//unbind VBO&VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void circleInitMenus()
{
	GLint menu = glutCreateMenu(circleProcessMenuEvent);
	glutSetMenuFont(menu, GLUT_BITMAP_9_BY_15);
	glutAddMenuEntry("Edit Circle", EDIT);
	glutAddMenuEntry("Move Circle", MOVE);
	glutAddMenuEntry("Zoom Circle", ZOOM);
	glutAddMenuEntry("Exit", EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutSetCursor(GLUT_CURSOR_INHERIT);
}
void circleGetTransformMatrix(glm::ivec2 transformInfo)
{
	//当处于移动状态时，传入transformInfo代表x，y方向上的移动距离
	//当处于缩放状态时，传入transformInfo代表x，y方向上的缩放百分比
	glm::mat4 transformMat;
	glm::vec2 vecA, vecB;
	glm::ivec3 center = myCircle.getCenter();
	switch (transformStatus)
	{
	case drawCircle::MOVE:
		center.x += transformInfo.x, center.y += transformInfo.y;
		myCircle.setCenter(center);
		break;
	case drawCircle::ZOOM:
		myCircle.setRadius(myCircle.getRadius()*(transformInfo.x+100)/100.0);
		break;
	}
}


