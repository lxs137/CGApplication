//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <time.h>
//#include "shader.h"
//#include "textureManager.h"
//#include "camera.h"
//#include "windowSetting.h"
//using namespace std;
//
//#include <freeglut\freeglut.h>
//#include <glm\glm.hpp>
//#include <glm\gtc\matrix_transform.hpp>
//#include <glm\gtc\type_ptr.hpp>
//
//GLfloat vertices[] = {
//	// Positions        // Texture
//	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
//	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
//	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
//	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
//	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
//
//	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
//	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
//	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
//	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
//	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
//	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
//
//	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
//	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
//	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
//
//	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
//	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
//	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//	0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
//	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
//
//	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//	0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
//	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
//	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
//	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
//	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//
//	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
//	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
//	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
//	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
//	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
//	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
//};
//
//TextureManager *myTextureManager;
//GLuint myShaderProgram;
//GLuint myVAO;
//Camera myCamera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
//unsigned int testTextureID = 1;
//struct uniformLocation
//{
//	GLuint modelLocation;
//	GLuint viewLocation;
//	GLuint projectionLocation;
//}uniformLoc;
//GLfloat lastMouseX = WIDTH_HALF, lastMouseY = HEIGHT_HALF;
//GLboolean normalKeys[128];
//GLboolean firstMouse=GL_FALSE;//标识视角旋转时鼠标是否刚被按下
//
//void initGlutWindow();
//GLuint initVAO();
//void render3DSence();
//void onNormalKeyPress(unsigned char key, int x, int y);
//void onNormalKeyRelease(unsigned char key, int x, int y);
//void onMouseClick(int button, int state, int x, int y);
//void onMouseActiveMotion(int x, int y);
//void onMouseWheelScroll(int wheel, int direction, int x, int y);
//void doMovement();
//
//int main(int argc, char *argv[])
//{
//	glutInit(&argc, argv);
//	initGlutWindow();
//
//	//set shader program
//	shader *myShader = new shader("3dModel.vert", "3dModel.frag");
//	myShaderProgram = myShader->shaderProgram;
//
//	//set VAO
//	myVAO = initVAO();
//
//	//set texture
//	myTextureManager = new TextureManager();
//	myTextureManager->loadTexture("E:\\CGApplication\\boardTexture.jpg", testTextureID);
//
//	//set transform
//	uniformLoc.modelLocation = glGetUniformLocation(myShaderProgram, "model");
//	uniformLoc.viewLocation = glGetUniformLocation(myShaderProgram, "view");
//	uniformLoc.projectionLocation = glGetUniformLocation(myShaderProgram, "projection");
//
//	glEnable(GL_DEPTH_TEST);
//	glutDisplayFunc(render3DSence);
//	glutIdleFunc(render3DSence);
//	glutKeyboardFunc(onNormalKeyPress);
//	glutKeyboardUpFunc(onNormalKeyRelease);
//	glutMouseFunc(onMouseClick);
//	glutMotionFunc(onMouseActiveMotion);
//	glutMouseWheelFunc(onMouseWheelScroll);
//	glutMainLoop();
//	//clean all resources
//	glDeleteVertexArrays(1, &myVAO);
//	return 0;
//}
//
//void render3DSence()
//{
//	doMovement();
//	glClearColor(0.f, 0.f, 0.f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	myTextureManager->bindTexture(testTextureID);
//	glUseProgram(myShaderProgram);
//
//	glm::mat4 model, view, projection;
//	model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
//	view = myCamera.getViewMartix();
//	projection = glm::perspective(myCamera.getZoom(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
//	glUniformMatrix4fv(uniformLoc.modelLocation, 1, GL_FALSE, glm::value_ptr(model));
//	glUniformMatrix4fv(uniformLoc.viewLocation, 1, GL_FALSE, glm::value_ptr(view));
//	glUniformMatrix4fv(uniformLoc.projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
//	glBindVertexArray(myVAO);
//	glDrawArrays(GL_TRIANGLES, 0, 36);
//	glBindVertexArray(0);
//
//	glutSwapBuffers();
//}
//void onNormalKeyPress(unsigned char key, int x, int y)
//{
//	if (key >= 0 && key <= 127)
//	{
//		normalKeys[key] = GL_TRUE;
//	}
//}
//void onNormalKeyRelease(unsigned char key, int x, int y)
//{
//	if (key >= 0 && key <= 127)
//	{
//		normalKeys[key] = GL_FALSE;
//	}
//}
//void onMouseClick(int button, int state, int x, int y)
//{
//	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
//	{
//		//修改上一帧时的鼠标位置，防止视角跳变
//		lastMouseX = x;
//		lastMouseY = y;
//		firstMouse = GL_TRUE;
//	}
//	if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
//		firstMouse = GL_FALSE;
//}
//void onMouseActiveMotion(int x, int y)
//{
//	if (firstMouse)//只有当修改了lastMouseX，lastMouseY后，才使视角变换
//	{
//		GLfloat xMove = x - lastMouseX;
//		GLfloat yMove = y - lastMouseY;
//		lastMouseX = x;
//		lastMouseY = y;
//		myCamera.processMouseMovement(xMove, -yMove);
//	}	
//}
//void onMouseWheelScroll(int wheel, int direction, int x, int y)
//{
//	myCamera.processMouseScroll(direction);
//}
//void doMovement()
//{
//	if (normalKeys[119]||normalKeys[87])
//	{
//		myCamera.processKeyBoard(FORWARD);
//	}
//	if (normalKeys[115]||normalKeys[83])
//	{
//		myCamera.processKeyBoard(BACKWARD);
//	}
//	if (normalKeys[100]||normalKeys[68])
//	{
//		myCamera.processKeyBoard(RIGHT);
//	}
//	if (normalKeys[97]||normalKeys[65])
//	{
//		myCamera.processKeyBoard(LEFT);
//	}
//}
//void initGlutWindow()
//{
//	glutInitWindowPosition(-1, -1);
//	glutInitWindowSize(WIDTH, HEIGHT);
//	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
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
//GLuint initVAO()
//{
//	GLuint VBO, VAO;
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	//set vertex buffer and attribute point
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(1);
//
//	//unbind VBO&VAO
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//	return VAO;
//}