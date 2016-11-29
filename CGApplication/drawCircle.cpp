//#include <iostream>
//#include <fstream>
//#include <vector>
//#include "drawCircle.h"
//#include "circle.h"
//#include "shader.h"
//#include "textureManager.h"
//#include "windowSetting.h"
//
//#include <freeglut\glut.h>
//#include <glm\glm.hpp>
//#include <glm\gtc\matrix_transform.hpp>
//#include <glm\gtc\type_ptr.hpp>
//using namespace drawCircle;
//
//circle myCircle;
//
//void drawCirApplication(int argc, char **argv)
//{
//	glutInit(&argc, argv);
//	circleInitGlutWindow();
//
//	//set shader program    
//	shader *myShader = new shader("2dModel.vert", "2dModel.frag");
//	myShaderProgram = myShader->shaderProgram;
//
//	//set VAO
//	myVAO = circleInitVAO();
//
//	//set transform
//	GLuint transformLocation = glGetUniformLocation(myShaderProgram, "transform");
//	glm::mat4 transformMat = circleGetTransformMatrix();
//
//	glutDisplayFunc(circleRender2DSence);
//	glutReshapeFunc(circleOnReshape);
//	glutMainLoop();
//}
//
//void circleRender2DSence()
//{
//	glClearColor(1.0f, 0.8f, 1.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glUseProgram(myShaderProgram);
//	//myTextureManager->bindTexture(testTextureID);
//	glBindVertexArray(myVAO);
//	//glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));
//	glDrawArrays(GL_POINTS, 0, myCircle.getPointsNum());
//	glBindVertexArray(0);
//
//	glutSwapBuffers();
//}
//void circleOnMouseClick(int button, int state, int x, int y)
//{
//
//}
//void circleOnReshape(int width, int height)
//{
//
//}
//void circleInitGlutWindow()
//{
//	glutInitWindowPosition(-1, -1);
//	glutInitWindowSize(WIDTH, HEIGHT);
//	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
//	glutCreateWindow("OpenGL Application");
//
//	glewExperimental = GL_TRUE;
//	if (glewInit() != GLEW_OK)
//	{
//		cout << "Failed to initialize GLEW" << endl;
//		return;
//	}
//	glViewport(0, 0, WIDTH, HEIGHT);
//}
//void circleLoadImageAsTexture(unsigned int testTextureID, const char *filename)
//{
//	myTextureManager->loadTexture(filename, testTextureID);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glBindTexture(GL_TEXTURE_2D, 0);
//}
//GLuint circleInitVAO()
//{
//	GLuint VBO, VAO;
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	//set vertex buffer and attribute point
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//	myCircle = circle(glm::ivec3(-50, -50, 0), 100, glm::vec3(1.0f, 0.0f, 0.0f));
//	myCircle.circleUseMidpoint();
//	myCircle.fillCircleScanLine(glm::vec3(0.0f, 1.0f, 0.0f));
//	myCircle.clipUseRect(glm::vec3(-100, -100, 0), glm::vec3(100, 150, 0));
//	glBufferData(GL_ARRAY_BUFFER, myCircle.getPointsNum()*myCircle.getPointSize(),
//		myCircle.getCirclePixels().begin()._Ptr, GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(1);
//	//unbind VBO&VAO
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//	return VAO;
//}
//glm::mat4 circleGetTransformMatrix()
//{
//	glm::mat4 transformMat;
//	//transformMat = glm::rotate(transformMat,(GLfloat)glfwGetTime()*PI/2, glm::vec3(0, 0, 1));
//	transformMat = glm::scale(transformMat, glm::vec3(1.0, 1.0, 1.0));
//	return transformMat;
//}
//
//
