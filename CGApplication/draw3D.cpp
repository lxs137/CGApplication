#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include "shader.h"
#include "draw3D.h"
#include "windowSetting.h"

#include <freeglut\freeglut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using namespace draw3D;

void draw3DApplication(int argc, char **argv)
{
	if (glutGet(GLUT_INIT_STATE) != 1)
	{
		glutInit(&argc, argv);
		d3InitGlutWindow();
	}

	myCamera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	lastMouseX = WIDTH_HALF, lastMouseY = HEIGHT_HALF;
	for (GLint i = 0; i < 128; i++)
		normalKeys[i] = GL_FALSE;
	firstMouse = GL_FALSE;
	ifExit = GL_FALSE;

	d3InitMenus();
	//set shader program
	shader *myShader = new shader("3dModel.vert", "3dModel.frag");
	myShaderProgram = myShader->shaderProgram;

	//set VAO
	myVAO = d3InitVAO();

	//set texture
	myTextureManager = new TextureManager();
	myTextureManager->loadTexture("E:\\CGApplication\\boardTexture.jpg", testTextureID);

	//set transform
	uniformLoc.modelLocation = glGetUniformLocation(myShaderProgram, "model");
	uniformLoc.viewLocation = glGetUniformLocation(myShaderProgram, "view");
	uniformLoc.projectionLocation = glGetUniformLocation(myShaderProgram, "projection");

	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(d3Render3DSence);
	glutIdleFunc(d3Render3DSence);
	glutKeyboardFunc(d3OnNormalKeyPress);
	glutKeyboardUpFunc(d3OnNormalKeyRelease);
	glutMouseFunc(d3OnMouseClick);
	glutMotionFunc(d3OnMouseActiveMotion);
	glutMouseWheelFunc(d3OnMouseWheelScroll);
	while (true)
	{
		glutMainLoopEvent();
		if (ifExit)
			break;
		glutPostRedisplay();
	}
	
	//clean all resources
	glDeleteVertexArrays(1, &myVAO);
}

void d3Render3DSence()
{
	d3DoMovement();
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myTextureManager->bindTexture(testTextureID);
	glUseProgram(myShaderProgram);

	glm::mat4 model, view, projection;
	model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	view = myCamera.getViewMartix();
	projection = glm::perspective(myCamera.getZoom(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(uniformLoc.modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformLoc.viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniformLoc.projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
	glBindVertexArray(myVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glutSwapBuffers();
}
void d3OnNormalKeyPress(unsigned char key, int x, int y)
{
	if (key >= 0 && key <= 127)
	{
		normalKeys[key] = GL_TRUE;
	}
}
void d3OnNormalKeyRelease(unsigned char key, int x, int y)
{
	if (key >= 0 && key <= 127)
	{
		normalKeys[key] = GL_FALSE;
	}
}
void d3OnMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
	{
		//修改上一帧时的鼠标位置，防止视角跳变
		lastMouseX = x;
		lastMouseY = y;
		firstMouse = GL_TRUE;
	}
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
		firstMouse = GL_FALSE;
}
void d3OnMouseActiveMotion(int x, int y)
{
	if (firstMouse)//只有当修改了lastMouseX，lastMouseY后，才使视角变换
	{
		GLfloat xMove = x - lastMouseX;
		GLfloat yMove = y - lastMouseY;
		lastMouseX = x;
		lastMouseY = y;
		myCamera.processMouseMovement(xMove, -yMove);
	}
}
void d3OnMouseWheelScroll(int wheel, int direction, int x, int y)
{
	myCamera.processMouseScroll(direction);
}
void d3ProcessMenuEvent(int options)
{
	switch (options)
	{
	case 0:
		ifExit = GL_TRUE;
		break;
	}
}
void d3DoMovement()
{
	if (normalKeys[119] || normalKeys[87])
	{
		myCamera.processKeyBoard(FORWARD);
	}
	if (normalKeys[115] || normalKeys[83])
	{
		myCamera.processKeyBoard(BACKWARD);
	}
	if (normalKeys[100] || normalKeys[68])
	{
		myCamera.processKeyBoard(RIGHT);
	}
	if (normalKeys[97] || normalKeys[65])
	{
		myCamera.processKeyBoard(LEFT);
	}
}
void d3InitMenus()
{
	GLint menu = glutCreateMenu(d3ProcessMenuEvent);
	glutSetMenuFont(menu, GLUT_BITMAP_9_BY_15);
	glutAddMenuEntry("Exit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutSetCursor(GLUT_CURSOR_INHERIT);
}
void d3InitGlutWindow()
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
GLuint d3InitVAO()
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