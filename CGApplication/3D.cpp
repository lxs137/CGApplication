#include <iostream>
#include <fstream>
#include <vector>
#include "shader.h"
#include "textureManager.h"
#include "windowSetting.h"
using namespace std;

#define GLEW_STATIC
#include <gl\glew.h>
#include <freeglut\glut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

GLfloat vertices[] = {
	// Positions        // Texture
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

TextureManager *myTextureManager;
GLuint myShaderProgram;
GLuint myVAO;
unsigned int testTextureID = 1;
struct uniformLocation
{
	GLuint transformLocation;
	GLuint modelLocation;
	GLuint viewLocation;
	GLuint projectionLocation;
}uniformLoc;
glm::mat4 transformMat;

void initGlutWindow();
GLuint initVAO();
glm::mat4 getTransformMatrix();
void render3DSence();

int main(int argc,char *argv[])
{
	glutInit(&argc, argv);
	initGlutWindow();

	//set shader program
	shader *myShader = new shader("3dModel.vert", "3dModel.frag");
	myShaderProgram = myShader->shaderProgram;

	//set VAO
	myVAO = initVAO();

	//set texture
	myTextureManager = new TextureManager();
	myTextureManager->loadTexture("E:\\CGApplication\\boardTexture.jpg", testTextureID);

	//set transform
	uniformLoc.transformLocation = glGetUniformLocation(myShaderProgram, "transform");
	uniformLoc.modelLocation = glGetUniformLocation(myShaderProgram, "model");
	uniformLoc.viewLocation = glGetUniformLocation(myShaderProgram, "view");
	uniformLoc.projectionLocation = glGetUniformLocation(myShaderProgram, "projection");
	transformMat = getTransformMatrix();

	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(render3DSence);
	glutMainLoop();

	//clean all resources
	glDeleteVertexArrays(1, &myVAO);
	return 0;
}

void render3DSence()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myTextureManager->bindTexture(testTextureID);
	glUseProgram(myShaderProgram);

	glm::mat4 model, view, projection;
	model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(uniformLoc.transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));
	glUniformMatrix4fv(uniformLoc.modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformLoc.viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniformLoc.projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
	glBindVertexArray(myVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glutSwapBuffers();
}
void initGlutWindow()
{
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("OpenGL Application");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		cout << "Failed to initialize GLEW" << endl;
		return;
	}
	glViewport(0, 0, WIDTH, HEIGHT);
}
GLuint initVAO()
{
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//set vertex buffer and attribute point
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//unbind VBO&VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return VAO;
}
glm::mat4 getTransformMatrix()
{
	glm::mat4 transformMat;
	//transformMat = glm::rotate(transformMat,(GLfloat)glfwGetTime()*PI/2, glm::vec3(0, 0, 1));
	transformMat = glm::scale(transformMat, glm::vec3(1.0, 1.0, 1.0));
	return transformMat;
}