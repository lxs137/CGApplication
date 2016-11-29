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
	myVAO = ellipseInitVAO();

	//set transform
	GLuint transformLocation = glGetUniformLocation(myShaderProgram, "transform");
	glm::mat4 transformMat = ellipseGetTransformMatrix();

	glutDisplayFunc(ellipseRender2DSence);
	glutReshapeFunc(ellipseOnReshape);
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

	glutSwapBuffers();
}
void ellipseOnMouseClick(int button, int state, int x, int y)
{

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
GLuint ellipseInitVAO()
{
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//set vertex buffer and attribute point
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	myEllipse = ellipse(glm::ivec3(-50, 0, 0), 150, 100, glm::vec3(1.0f, 0.0f, 0.0f));
	myEllipse.ellipseUseMidpoint();
	myEllipse.fillEllipseScanLine(glm::vec3(0.0f, 1.0f, 0.0f));
	myEllipse.clipUseRect(glm::ivec3(-200, -150, 0), glm::ivec3(150, 200, 0));
	glBufferData(GL_ARRAY_BUFFER, myEllipse.getPointsNum()*myEllipse.getPointSize(),
		myEllipse.getEllipsePixels().begin()._Ptr, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//unbind VBO&VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return VAO;
}
glm::mat4 ellipseGetTransformMatrix()
{
	glm::mat4 transformMat;
	//transformMat = glm::rotate(transformMat,(GLfloat)glfwGetTime()*PI/2, glm::vec3(0, 0, 1));
	transformMat = glm::scale(transformMat, glm::vec3(1.0, 1.0, 1.0));
	return transformMat;
}


