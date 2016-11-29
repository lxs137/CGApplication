#include <iostream>
#include <fstream>
#include <vector>
#include "drawLine.h"
#include "line.h"
#include "shader.h"
#include "textureManager.h"
#include "windowSetting.h"

#include <freeglut\glut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
using namespace drawLine;

line myLine;

void drawLineApplication(int argc, char **argv)
{
	glutInit(&argc, argv);
	lineInitGlutWindow();

	//set shader program	
	shader *myShader = new shader("2dModel.vert", "2dModel.frag");
	myShaderProgram = myShader->shaderProgram;

	//set VAO
	lineInitVAO(myVAO,myVBO);

	//set transform
	GLuint transformLocation = glGetUniformLocation(myShaderProgram, "transform");
	glm::mat4 transformMat = lineGetTransformMatrix();

	glutDisplayFunc(lineRender2DSence);
	glutIdleFunc(lineRender2DSence);
	glutReshapeFunc(lineOnReshape);
	glutMouseFunc(lineOnMouseClick);
	glutMotionFunc(lineOnActiveMotion);
	glutMainLoop();
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
}
void lineOnActiveMotion(int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	if (drawing)
	{
		myLine.setDrawingPoint(glm::ivec3(x, y, 0));
		myLine.lineUseBresenham();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, myLine.getPointsNum()*myLine.getPointSize(),
			myLine.getLinePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
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
glm::mat4 lineGetTransformMatrix()
{
	glm::mat4 transformMat;
	//transformMat = glm::rotate(transformMat,(GLfloat)glfwGetTime()*PI/2, glm::vec3(0, 0, 1));
	transformMat = glm::scale(transformMat, glm::vec3(1.0, 1.0, 1.0));
	return transformMat;
}


