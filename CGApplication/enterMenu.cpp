#include "windowSetting.h"
#include "entermenu.h"
#include "shader.h"
#include <iostream>
#include <freeglut\freeglut.h>
#include <glm\glm.hpp>
using namespace EnterMenu;

DRAWINGMODEL drawingModel=DRAWENTERMENU;

void drawEnterMenuApplication(int argc, char **argv);
void drawLineApplication(int argc, char **argv);
void drawCircleApplication(int argc, char **argv);
void drawEllipseApplication(int argc, char **argv);
void drawBezierApplication(int argc, char **argv);
void drawSplineApplication(int argc, char **argv);
void drawPolygonApplication(int argc, char **argv);
void draw3DApplication(int argc, char **argv);

int main(int argc,char *argv[])
{
	while (true)
	{
		switch (drawingModel)
		{
		case DRAWLINE:
			drawLineApplication(argc, argv);
			drawingModel = DRAWENTERMENU;
			break;
		case DRAWCIRCLE:
			drawCircleApplication(argc, argv);
			drawingModel = DRAWENTERMENU;
			break;
		case DRAWELLIPSE:
			drawEllipseApplication(argc, argv);
			drawingModel = DRAWENTERMENU;
			break;
		case DRAWBEZIER:
			drawBezierApplication(argc, argv);
			drawingModel = DRAWENTERMENU;
			break;
		case DRAWSPLINE:
			drawSplineApplication(argc, argv);
			drawingModel = DRAWENTERMENU;
			break;
		case DRAWPOLYGON:
			drawPolygonApplication(argc, argv);
			drawingModel = DRAWENTERMENU;
			break;
		case DRAW3D:
			draw3DApplication(argc, argv);
			init = GL_FALSE;
			drawingModel = DRAWENTERMENU;
			break;
		case DRAWENTERMENU:
			drawEnterMenuApplication(argc, argv);
			break;
		case EXITSYSTEM:
			exit(0);
			break;
		}
	}
}
void drawEnterMenuApplication(int argc, char **argv)
{
	if (glutGet(GLUT_INIT_STATE) != 1)
	{
		glutInit(&argc, argv);
	}
	if (!init)
	{
		init = GL_TRUE;			
		int windowNew=entranceInitGlutWindow();
		shader *myShader = new shader("2dModel.vert", "2dModel.frag");
		myShaderProgram = myShader->shaderProgram;

		std::vector<GLfloat> framePixels;
		myVAOs.clear();
		myVBOs.clear();
		std::vector<Button> selectedButton;
		std::vector<Button> unSelectedButton;
		for (GLint i = 0; i < 8; i++)
		{
			selectedButton.push_back(entranceInitButton(buttonRect[i], selectedColor));
			unSelectedButton.push_back(entranceInitButton(buttonRect[i], unSelectedColor));
		}
		framePixelsNum = selectedButton[0].pixelsNum * 8;
		for (GLint i = 0; i < 9; i++)
		{
			myVAOs.push_back((GLuint)(i + 1));
			myVBOs.push_back((GLuint)(i + 1));
			glGenVertexArrays(1, &myVAOs[i]);
			glGenBuffers(1, &myVBOs[i]);
			//set vertex buffer and attribute point
			glBindVertexArray(myVAOs[i]);
			glBindBuffer(GL_ARRAY_BUFFER, myVBOs[i]);

			framePixels.clear();
			for (GLint j = 0; j < 8; j++)
			{
				if (j == i)
					framePixels.insert(framePixels.end(), selectedButton[j].buttonPixels.begin(), 
					selectedButton[j].buttonPixels.end());
				else
					framePixels.insert(framePixels.end(), unSelectedButton[j].buttonPixels.begin(),
					unSelectedButton[j].buttonPixels.end());
			}
		
			glBufferData(GL_ARRAY_BUFFER, framePixelsNum * 6 * sizeof(GLfloat),
				framePixels.begin()._Ptr, GL_DYNAMIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);

			//unbind VBO&VAO
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		if (windowIdentify != 0)
			glutDestroyWindow(windowIdentify);
		windowIdentify = windowNew;
	}

	glutDisplayFunc(entranceRender2DSence);
	glutIdleFunc(entranceRender2DSence);
	glutMouseFunc(entranceOnMouseClick);
	glutPassiveMotionFunc(entranceOnMouseMove);

	while (GL_TRUE)
	{
		glutMainLoopEvent();
		if (drawingModel != DRAWENTERMENU)
			break;
		glutPostRedisplay();
	}
}
void entranceRender2DSence()
{
	glClearColor(1.0f, 0.9215f, 0.8037f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(myShaderProgram);
	glBindVertexArray(myVAOs[selectedButton]);
	glDrawArrays(GL_POINTS, 0, framePixelsNum);
	glBindVertexArray(0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(-0.75f, 0.35f);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, buttonText[0]);
	glRasterPos2f(-0.32f, 0.35f);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, buttonText[1]);
	glRasterPos2f(0.13f, 0.35f);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, buttonText[2]);
	glRasterPos2f(0.57f, 0.35f);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, buttonText[3]);
	glRasterPos2f(-0.8f, -0.32f);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, buttonText[4]);
	glRasterPos2f(-0.35f, -0.32f);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, buttonText[5]);
	glRasterPos2f(0.15f, -0.32f);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, buttonText[6]);
	glRasterPos2f(0.505f, -0.32f);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, buttonText[7]);

	glutSwapBuffers();
}
void entranceOnMouseClick(int button, int state, int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	GLint i;
	for (i = 0; i < buttonRect.size(); i++)
	{
		if (x >= buttonRect[i][3].x&&x <= buttonRect[i][1].x
			&&y >= buttonRect[i][3].y&&y <= buttonRect[i][1].y)
		{
			break;
		}
	}
	drawingModel = (DRAWINGMODEL)i;
}
void entranceOnMouseMove(int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	GLint i;
	for (i = 0; i < buttonRect.size(); i++)
	{
		if (x >= buttonRect[i][3].x&&x <= buttonRect[i][1].x
			&&y >= buttonRect[i][3].y&&y <= buttonRect[i][1].y)
		{
			break;
		}
	}
	selectedButton = (DRAWINGMODEL)i;
}
GLint entranceInitGlutWindow()
{
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	int window=glutCreateWindow("OpenGL Application");
	
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return 0;
	}
	glViewport(0, 0, WIDTH, HEIGHT);
	return window;
}
EnterMenu::Button entranceInitButton(std::vector<glm::ivec3> vertics, glm::vec3 fillColor)
{
	EnterMenu::Button myButton;
	myButton.buttonPixels.clear();
	myButton.pixelsNum = 0;
	GLint xMin = vertics[3].x, xMax = vertics[1].x, yMin = vertics[3].y, yMax = vertics[1].y;
	for (GLint i = yMin; i <= yMax; i++)
	{
		for (GLint j = xMin; j <= xMax; j++)
		{
			myButton.buttonPixels.push_back(j / (GLfloat)WIDTH_HALF);
			myButton.buttonPixels.push_back(i / (GLfloat)HEIGHT_HALF);
			myButton.buttonPixels.push_back((GLfloat)0);
			myButton.buttonPixels.push_back(fillColor.x);
			myButton.buttonPixels.push_back(fillColor.y);
			myButton.buttonPixels.push_back(fillColor.z);
			myButton.pixelsNum++;
		}
	}
	return myButton;
}