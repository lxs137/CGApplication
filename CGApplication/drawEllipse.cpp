#include <iostream>
#include <fstream>
#include <vector>
#include "drawEllipse.h"
#include "ellipse.h"
#include "shader.h"
#include "textureManager.h"
#include "windowSetting.h"

#include <freeglut\glut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
using namespace drawEllipse;

ellipse myEllipse;

void drawEllipseApplication(int argc, char **argv)
{
	glutInit(&argc, argv);
	ellipseInitGlutWindow();

	//set shader program	
	shader *myShader = new shader("2dModel.vert", "2dModel.frag");
	myShaderProgram = myShader->shaderProgram;

	//set VAO
	ellipseInitVAO(myVAO, myVBO);

	//set transform
	GLuint transformLocation = glGetUniformLocation(myShaderProgram, "transform");
	glm::mat4 transformMat = ellipseGetTransformMatrix();

	glutDisplayFunc(ellipseRender2DSence);
	glutIdleFunc(ellipseRender2DSence);
	glutReshapeFunc(ellipseOnReshape);
	glutMouseFunc(ellipseOnMouseClick);
	glutMotionFunc(ellipseOnActiveMotion);
	glutMainLoop();
}

void ellipseRender2DSence()
{
	glClearColor(1.0f, 0.8f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(myShaderProgram);
	//myTextureManager->bindTexture(testTextureID);
	glBindVertexArray(myVAO);
	//glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));
	glDrawArrays(GL_POINTS, 0, myEllipse.getPointsNum());
	glBindVertexArray(0);
	if ((!drawing) && myEllipse.getPointsNum() >= 1)
	{
		GLfloat x1, y1, x2, y2, x3, y3, x4, y4, dertX, dertY;
		GLfloat centerX = myEllipse.getCenter().x, centerY = myEllipse.getCenter().y;
		GLfloat radiusX = myEllipse.getRadius().x, radiusY = myEllipse.getRadius().y;
		x1 = (centerX - radiusX)/WIDTH_HALF, y1 = (centerY + radiusY)/HEIGHT_HALF;
		x2 = (centerX + radiusX)/WIDTH_HALF, y2 = (centerY + radiusY)/HEIGHT_HALF;
		x3 = (centerX + radiusX)/WIDTH_HALF, y3 = (centerY - radiusY)/HEIGHT_HALF;
		x4 = (centerX - radiusX)/WIDTH_HALF, y4 = (centerY - radiusY)/HEIGHT_HALF;
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

		}
	}
	else if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
	{
		if (drawing)
		{
			drawing = GL_FALSE;
			GLint rX, rY;
			rX = abs(x - myEllipse.getCenter().x), rY = abs(y - myEllipse.getCenter().y);
			myEllipse.ellipseUseMidpoint();
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, myEllipse.getPointsNum()*myEllipse.getPointSize(),
				myEllipse.getEllipsePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
}
void ellipseOnActiveMotion(int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	if (drawing)
	{
		GLint rX, rY;
		rX = abs(x - myEllipse.getCenter().x), rY = abs(y - myEllipse.getCenter().y);
		myEllipse.setRadius(rX, rY);
		myEllipse.ellipseUseMidpoint();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, myEllipse.getPointsNum()*myEllipse.getPointSize(),
			myEllipse.getEllipsePixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
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
glm::mat4 ellipseGetTransformMatrix()
{
	glm::mat4 transformMat;
	//transformMat = glm::rotate(transformMat,(GLfloat)glfwGetTime()*PI/2, glm::vec3(0, 0, 1));
	transformMat = glm::scale(transformMat, glm::vec3(1.0, 1.0, 1.0));
	return transformMat;
}


