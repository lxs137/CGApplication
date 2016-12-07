#include <iostream>
#include <fstream>
#include <vector>
#include "drawBezier.h"
#include "bezier.h"
#include "shader.h"
#include "textureManager.h"
#include "windowSetting.h"

#include <freeglut\freeglut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
using namespace drawBezier;

bezier myBezier;

void drawBezierApplication(int argc, char **argv)
{
	if (glutGet(GLUT_INIT_STATE) != 1)
	{
		glutInit(&argc, argv);
		bezierInitGlutWindow();
	}

	//set shader program	
	shader *myShader = new shader("2dModel.vert", "2dModel.frag");
	myShaderProgram = myShader->shaderProgram;

	//set VAO
	bezierInitVAO(myVAO, myVBO);

	//set transform
	GLuint transformLocation = glGetUniformLocation(myShaderProgram, "transform");
	glm::mat4 transformMat = bezierGetTransformMatrix();

	glutDisplayFunc(bezierRender2DSence);
	glutIdleFunc(bezierRender2DSence);
	glutReshapeFunc(bezierOnReshape);
	glutMouseFunc(bezierOnMouseClick);
	glutMotionFunc(bezierOnActiveMotion);
	glutMainLoop();
}

void bezierRender2DSence()
{
	glClearColor(1.0f, 0.8f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(myShaderProgram);
	glBindVertexArray(myVAO);
	//glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));
	glDrawArrays(GL_POINTS, 0, myBezier.getPointsNum());
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
void bezierOnMouseClick(int button, int state, int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
	{
		if (myBezier.getPointsNum() <= 1)
		{
			drawStatus = 1;
			if (controlPoints.size() >= drawingPointIndex)
			{
				controlPoints[drawingPointIndex - 1].x = x;
				controlPoints[drawingPointIndex - 1].y = y;
				myBezier.setControlPoints(glm::ivec3(x, y, 0), drawingPointIndex);
			}
			else
			{
				controlPoints.push_back(glm::ivec3(x, y, 0));
				myBezier.setControlPoints(glm::ivec3(x, y, 0), drawingPointIndex);
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
			else if ((drawStatus == 1 && drawingPointIndex == 4)||drawStatus == 2)
			{
				myBezier.setControlPoints(glm::ivec3(x, y, 0), drawingPointIndex);
				myBezier.bezierUseLine();
				glBindBuffer(GL_ARRAY_BUFFER, myVBO);
				glBufferData(GL_ARRAY_BUFFER, myBezier.getPointsNum()*myBezier.getPointSize(),
					myBezier.getBezierPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}
	}
}
void bezierOnActiveMotion(int x, int y)
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
		myBezier.setControlPoints(glm::ivec3(x, y, 0), drawingPointIndex);
		if (drawStatus == 2)
		{
			myBezier.bezierUseLine();
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, myBezier.getPointsNum()*myBezier.getPointSize(),
				myBezier.getBezierPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
}
void bezierOnReshape(int width, int height)
{

}
void bezierInitGlutWindow()
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
void bezierInitVAO(GLuint &VAO, GLuint &VBO)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//set vertex buffer and attribute point
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	myBezier = bezier();
	glBufferData(GL_ARRAY_BUFFER, myBezier.getPointsNum()*myBezier.getPointSize(),
		myBezier.getBezierPixels().begin()._Ptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//unbind VBO&VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
glm::mat4 bezierGetTransformMatrix()
{
	glm::mat4 transformMat;
	//transformMat = glm::rotate(transformMat,(GLfloat)glfwGetTime()*PI/2, glm::vec3(0, 0, 1));
	transformMat = glm::scale(transformMat, glm::vec3(1.0, 1.0, 1.0));
	return transformMat;
}


