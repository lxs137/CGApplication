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

	//set shader program	
	shader *myShader = new shader("2dModel.vert", "2dModel.frag");
	myShaderProgram = myShader->shaderProgram;

	//set VAO
	polygonInitVAO(myVAO, myVBO);

	//set transform
	GLuint transformLocation = glGetUniformLocation(myShaderProgram, "transform");
	glm::mat4 transformMat = polygonGetTransformMatrix();

	glutDisplayFunc(polygonRender2DSence);
	glutIdleFunc(polygonRender2DSence);
	glutReshapeFunc(polygonOnReshape);
	glutMouseFunc(polygonOnMouseClick);
	glutMotionFunc(polygonOnActiveMotion);
	glutMainLoop();
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
		if (drawStatus == 2)
		{
			myPolygon.polygonUseLine();
		}
		else if (drawStatus == 1)
		{
			myPolygon.polygonUseLine();
		}
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
			myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
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
glm::mat4 polygonGetTransformMatrix()
{
	glm::mat4 transformMat;
	//transformMat = glm::rotate(transformMat,(GLfloat)glfwGetTime()*PI/2, glm::vec3(0, 0, 1));
	transformMat = glm::scale(transformMat, glm::vec3(1.0, 1.0, 1.0));
	return transformMat;
}


