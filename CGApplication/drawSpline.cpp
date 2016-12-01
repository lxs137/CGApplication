#include <iostream>
#include <fstream>
#include <vector>
#include "drawSpline.h"
#include "splineCurve.h"
#include "shader.h"
#include "textureManager.h"
#include "windowSetting.h"

#include <freeglut\glut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
using namespace drawSpline;

spline mySpline;

void drawSplineApplication(int argc, char **argv)
{
	glutInit(&argc, argv);
	splineInitGlutWindow();

	//set shader program	
	shader *myShader = new shader("2dModel.vert", "2dModel.frag");
	myShaderProgram = myShader->shaderProgram;

	//set VAO
	splineInitVAO(myVAO, myVBO);

	//set transform
	GLuint transformLocation = glGetUniformLocation(myShaderProgram, "transform");
	glm::mat4 transformMat = splineGetTransformMatrix();

	glutDisplayFunc(splineRender2DSence);
	glutIdleFunc(splineRender2DSence);
	glutReshapeFunc(splineOnReshape);
	glutMouseFunc(splineOnMouseClick);
	glutMotionFunc(splineOnActiveMotion);
	glutMainLoop();
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
	glutSwapBuffers();
}
void splineOnMouseClick(int button, int state, int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
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
}
void splineOnActiveMotion(int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
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
glm::mat4 splineGetTransformMatrix()
{
	glm::mat4 transformMat;
	//transformMat = glm::rotate(transformMat,(GLfloat)glfwGetTime()*PI/2, glm::vec3(0, 0, 1));
	transformMat = glm::scale(transformMat, glm::vec3(1.0, 1.0, 1.0));
	return transformMat;
}


