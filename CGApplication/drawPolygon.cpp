#include <iostream>
#include <fstream>
#include <vector>
#include <string>
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

	//init
	myTextureManager = new TextureManager();
	lastMouseX = WIDTH_HALF, lastMouseY = HEIGHT_HALF;
	drawStatus = 0;
	filling = 0;
	vertexPoints.clear();
	transBasisPoint.clear();
	rotateCenter = glm::ivec3(0, 0, 0);
	drawingPointIndex = 1;
	myClipWindow.vertex.clear();
	myClipWindow.clipStatus = 0;
	myClipWindow.clipPointIndex = 1;
	myClipWindow.isClose = GL_FALSE;
	transformStatus = EDIT;

	polygonInitMenus();
	//set shader program	
	shader *myShader = new shader("2dModel.vert", "2dModel.frag");
	myShaderProgram = myShader->shaderProgram;

	//set VAO
	polygonInitVAO(myVAO, myVBO);

	//set textureSwitch Loaction
	textureSwitchLoc = glGetUniformLocation(myShaderProgram, "isUseTexture");

	glEnable(GL_TEXTURE_2D);
	glutDisplayFunc(polygonRender2DSence);
	glutIdleFunc(polygonRender2DSence);
	glutReshapeFunc(polygonOnReshape);
	glutMouseFunc(polygonOnMouseClick);
	glutMotionFunc(polygonOnActiveMotion);
	while (GL_TRUE)
	{
		glutMainLoopEvent();
		if (transformStatus == EXIT)
			return;
		glutPostRedisplay();
	}
}

void polygonRender2DSence()
{
	glClearColor(1.0f, 0.9215f, 0.8037f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	if (filling == 2)
	{
		glUniform1i(textureSwitchLoc, 1);
	}
	else
		glUniform1i(textureSwitchLoc, 0);
	myTextureManager->bindTexture(textureID);
	glUseProgram(myShaderProgram);
	glBindVertexArray(myVAO);
	glDrawArrays(GL_POINTS, 0, myPolygon.getPointsNum());
	glBindVertexArray(0);
	if (vertexPoints.size() != 0 && transformStatus != CLIP)
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
	if (transformStatus == CLIP && !myClipWindow.vertex.empty())
	{
		GLfloat dertX, dertY, xTemp, yTemp;
		dertX = ((GLfloat)CHANGE_POINT_DIS) / WIDTH_HALF, dertY = ((GLfloat)CHANGE_POINT_DIS) / HEIGHT_HALF;
		glBegin(GL_QUADS);
		for (GLint i = 0; i < myClipWindow.vertex.size(); i++)
		{
			xTemp = ((GLfloat)myClipWindow.vertex[i].x) / WIDTH_HALF, yTemp = ((GLfloat)myClipWindow.vertex[i].y) / HEIGHT_HALF;
			glVertex2f(xTemp - dertX, yTemp + dertY);
			glVertex2f(xTemp + dertX, yTemp + dertY);
			glVertex2f(xTemp + dertX, yTemp - dertY);
			glVertex2f(xTemp - dertX, yTemp - dertY);
		}
		glEnd();

		glLineStipple(3, 0x5555);
		glLineWidth(4.0);
		glEnable(GL_LINE_STIPPLE);
		if (myClipWindow.isClose)
			glBegin(GL_LINE_LOOP);
		else
			glBegin(GL_LINE_STRIP);
		glColor3f(1.0f, 1.0f, 1.0f);
		for (GLint i = 0; i < myClipWindow.vertex.size(); i++)
		{
			xTemp = ((GLfloat)myClipWindow.vertex[i].x) / WIDTH_HALF, yTemp = ((GLfloat)myClipWindow.vertex[i].y) / HEIGHT_HALF;
			glVertex2f(xTemp, yTemp);
		}
		glEnd();
		glDisable(GL_LINE_STIPPLE);
		glLineWidth(1.0);
	}
	glutSwapBuffers();
}
void polygonOnMouseClick(int button, int state, int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	switch (transformStatus)
	{
	case drawPolygon::EDIT:
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
				if (filling != 0)
					myPolygon.fillPolygonScanLine();
				else
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
					if (filling != 0)
						myPolygon.fillPolygonScanLine();
					else
						myPolygon.polygonUseLine();
					glBindBuffer(GL_ARRAY_BUFFER, myVBO);
					glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
						myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
				else if (drawStatus == 1 && x <= startX + CHANGE_POINT_DIS * 2 && x >= startX - CHANGE_POINT_DIS * 2
					&& y <= startY + CHANGE_POINT_DIS * 2 && y >= startY - CHANGE_POINT_DIS * 2)
				{
					myPolygon.setIsclose(GL_TRUE);
					vertexPoints.pop_back();
					if (filling != 0)
						myPolygon.fillPolygonScanLine();
					else
						myPolygon.polygonUseLine();
					glBindBuffer(GL_ARRAY_BUFFER, myVBO);
					glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
						myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					for (int i = 0; i < vertexPoints.size(); i++)
						transBasisPoint.push_back(vertexPoints[i]);
				}
				else
				{
					if (vertexPoints.size() < drawingPointIndex)
					{
						vertexPoints.push_back(glm::ivec3(x, y, 0));
					}
					drawingPointIndex++;
					if (filling != 0)
						myPolygon.fillPolygonScanLine();
					else
						myPolygon.polygonUseLine();
					glBindBuffer(GL_ARRAY_BUFFER, myVBO);
					glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
						myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
			}
		}
		break;
	case drawPolygon::MOVE:
		if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
		{
			lastMouseX = x, lastMouseY = y;
		}
		else if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
		{
			polygonGetTransformMatrix(glm::ivec2(x - lastMouseX, y - lastMouseY));
			lastMouseX = x, lastMouseY = y;
			if (filling != 0)
				myPolygon.fillPolygonScanLine();
			else
				myPolygon.polygonUseLine();
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
				myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		break;
	case drawPolygon::ROTATE:
		if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
		{
			lastMouseX = x, lastMouseY = y;
		}
		else if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
		{
			polygonGetTransformMatrix(glm::ivec2(lastMouseX, lastMouseY), glm::ivec2(x, y));
			lastMouseX = x, lastMouseY = y;
			if (filling != 0)
				myPolygon.fillPolygonScanLine();
			else
				myPolygon.polygonUseLine();
			polygonSetTransBasisPoint();
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
				myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		break;
	case drawPolygon::CLIP:
		if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
		{
			if (!myClipWindow.isClose)
			{
				myClipWindow.clipStatus = 1;
				if (myClipWindow.vertex.size() >= myClipWindow.clipPointIndex)
				{
					myClipWindow.vertex[myClipWindow.clipPointIndex - 1].x = x;
					myClipWindow.vertex[myClipWindow.clipPointIndex - 1].y = y;
				}
				else
				{
					myClipWindow.vertex.push_back(glm::ivec3(x, y, 0));
				}
				if (myClipWindow.clipPointIndex == 1)
					myClipWindow.clipPointIndex++;
			}
			else
			{
				myClipWindow.clipStatus = 2;
				GLint xTemp, yTemp;
				GLint i;
				for (i = 0; i < myClipWindow.vertex.size(); i++)
				{
					xTemp = myClipWindow.vertex[i].x, yTemp = myClipWindow.vertex[i].y;
					if (x <= xTemp + CHANGE_POINT_DIS&&x >= xTemp - CHANGE_POINT_DIS
						&&y <= yTemp + CHANGE_POINT_DIS&&y >= yTemp - CHANGE_POINT_DIS)
						break;
				}
				if (i != myClipWindow.vertex.size())
					myClipWindow.clipPointIndex= i + 1;
				else
					myClipWindow.clipStatus = 0;
			}
		}
		else if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
		{
			if (myClipWindow.clipStatus != 0)
			{
				if (myClipWindow.clipStatus == 1 && myClipWindow.clipPointIndex == 2)
				{
					if (myClipWindow.vertex[myClipWindow.clipPointIndex - 2].x == x
						&&myClipWindow.vertex[myClipWindow.clipPointIndex - 2].y == y)
					{
						myClipWindow.clipPointIndex--;
						myClipWindow.vertex.clear();
						myClipWindow.clipStatus = 0;
						return;
					}
				}
				GLint startX = myClipWindow.vertex[0].x, startY = myClipWindow.vertex[0].y;
				if (myClipWindow.clipStatus == 2)
				{
					myClipWindow.vertex[myClipWindow.clipPointIndex - 1] = glm::ivec3(x, y, 0);
					std::vector<polygon> resultPolygon = myPolygon.clipWithPolygon(myClipWindow.vertex,
						(filling == 0) ? GL_FALSE : GL_TRUE);
					glBindBuffer(GL_ARRAY_BUFFER, myVBO);
					glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
						myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
				else if (myClipWindow.clipStatus == 1 && x <= startX + CHANGE_POINT_DIS * 2 && x >= startX - CHANGE_POINT_DIS * 2
					&& y <= startY + CHANGE_POINT_DIS * 2 && y >= startY - CHANGE_POINT_DIS * 2)
				{
					myClipWindow.isClose = GL_TRUE;
					myClipWindow.vertex.pop_back();
					std::vector<polygon> resultPolygon = myPolygon.clipWithPolygon(myClipWindow.vertex,
						(filling == 0) ? GL_FALSE : GL_TRUE);
					glBindBuffer(GL_ARRAY_BUFFER, myVBO);
					glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
						myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
				else
				{
					if (myClipWindow.vertex.size() < myClipWindow.clipPointIndex)
					{
						myClipWindow.vertex.push_back(glm::ivec3(x, y, 0));
					}
					myClipWindow.clipPointIndex++;
				}
			}
		}
		break;
	}
}
void polygonOnActiveMotion(int x, int y)
{
	x -= WIDTH_HALF;
	y = HEIGHT_HALF - y;
	switch (transformStatus)
	{
	case drawPolygon::EDIT:
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
			if (drawStatus == 2 || drawStatus == 1)
			{
				if (filling != 0)
					myPolygon.fillPolygonScanLine();
				else
					myPolygon.polygonUseLine();
			}
			glBindBuffer(GL_ARRAY_BUFFER, myVBO);
			glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
				myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		break;
	case drawPolygon::MOVE:
		polygonGetTransformMatrix(glm::ivec2(x - lastMouseX, y - lastMouseY));
		lastMouseX = x, lastMouseY = y;
		if (filling != 0)
			myPolygon.fillPolygonScanLine();
		else
			myPolygon.polygonUseLine();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
			myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	case drawPolygon::ROTATE:
		polygonGetTransformMatrix(glm::ivec2(lastMouseX, lastMouseY), glm::ivec2(x, y));
		if (filling != 0)
			myPolygon.fillPolygonScanLine();
		else
			myPolygon.polygonUseLine();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
			myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	case drawPolygon::CLIP:
		if (myClipWindow.clipStatus != 0)
		{
			if (myClipWindow.vertex.size() >= myClipWindow.clipPointIndex)
			{
				myClipWindow.vertex[myClipWindow.clipPointIndex - 1].x = x;
				myClipWindow.vertex[myClipWindow.clipPointIndex - 1].y = y;
			}
			else
			{
				myClipWindow.vertex.push_back(glm::ivec3(x, y, 0));
			}
		}
		break;
	}
	
}
void polygonOnMouseWheelScrollValid(int wheel, int direction, int x, int y)
{
	polygonGetTransformMatrix(glm::ivec2(direction * 2, direction * 2));
	if (filling != 0)
		myPolygon.fillPolygonScanLine();
	else
		myPolygon.polygonUseLine();
	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
		myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void polygonOnMouseWheelScrollInvalid(int wheel, int direction, int x, int y){}
void polygonProcessMenuEvent(int options)
{
	GLint n;
	glm::vec3 fillColor;
	std::string filePath, texturePath;
	GLboolean saveAsBmp = GL_FALSE;
	if (options != CLIP && myClipWindow.vertex.size() != 0)
	{
		saveAsBmp = GL_TRUE;
		if (filling == 0)
			myPolygon.polygonUseLine();
		else if (filling == 1 || filling == 2)
			myPolygon.fillPolygonScanLine();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
			myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (myPolygon.getPointsNum() == 0 && options != OPENFILE && options != EXIT)//在多边形未初始化时，不能对多边形进行变换
		return;
	switch (options)
	{
	case EDIT:
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		glutMouseWheelFunc(polygonOnMouseWheelScrollInvalid);
		transformStatus = EDIT;
		polygonSetTransBasisPoint();
		break;
	case MOVE:
		glutSetCursor(GLUT_CURSOR_CYCLE);
		glutMouseWheelFunc(polygonOnMouseWheelScrollInvalid);
		transformStatus = MOVE;
		polygonSetTransBasisPoint();
		break;
	case ROTATE:
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		glutMouseWheelFunc(polygonOnMouseWheelScrollInvalid);
		transformStatus = ROTATE;
		rotateCenter.x = 0, rotateCenter.y = 0;
		n = vertexPoints.size();
		for (int i = 0; i < n; i++)
			rotateCenter.x += vertexPoints[i].x, rotateCenter.y += vertexPoints[i].y;
		rotateCenter.x /= n, rotateCenter.y /= n;
		polygonSetTransBasisPoint();
		break;
	case ZOOM:
		glutSetCursor(GLUT_CURSOR_SPRAY);
		glutMouseWheelFunc(polygonOnMouseWheelScrollValid);
		transformStatus = ZOOM;
		rotateCenter.x = 0, rotateCenter.y = 0;
		n = vertexPoints.size();
		for (int i = 0; i < n; i++)
			rotateCenter.x += vertexPoints[i].x, rotateCenter.y += vertexPoints[i].y;
		rotateCenter.x /= n, rotateCenter.y /= n;
		polygonSetTransBasisPoint();
		break;
	case FILLCOLOR:
		glutMouseWheelFunc(polygonOnMouseWheelScrollValid);
		filling = (filling == 1) ? 0 : 1;
		if (filling == 1)
		{
			GLint RValue, GValue, BValue;
			cout << "输入要填充的颜色值(例如：255 0 0):" << endl;
			cin >> RValue >> GValue >> BValue;
			fillColor = glm::vec3(RValue / 255.0, GValue / 255.0, BValue / 255.0);
			myPolygon.setFillColor(fillColor);
			myPolygon.fillPolygonScanLine();
		}			
		else
			myPolygon.polygonUseLine();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
			myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	case FILLPICTURE:
		glutMouseWheelFunc(polygonOnMouseWheelScrollValid);
		filling = (filling == 2) ? 0 : 2;
		if (filling == 2)
		{
			string textureName;
			cout << "输入要填充图片的路径:";
			cin >> textureName;
			myTextureManager->loadTexture(textureName.c_str(), textureID);
			myPolygon.fillPolygonScanLine();
		}		
		else
			myPolygon.polygonUseLine();
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
			myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	case SAVEFILE:
		if (!myPolygon.getIsClose())
		{
			cout << "多边形未闭合" << endl;
			return;
		}
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		glutMouseWheelFunc(polygonOnMouseWheelScrollInvalid);
		transformStatus = SAVEFILE;
		cout << "请输入保存文件的路径:" << endl;
		cin >> filePath;
		if (filling == 0)
			myPolygon.savePolygonIntoFile(filePath);
		else if (filling == 1)
			myPolygon.savePolygonIntoFile(filePath, GL_TRUE);
		else
			myPolygon.savePolygonIntoFile(filePath, GL_TRUE, myTextureManager->getTexturePath(textureID));
		if (saveAsBmp)
		{
			cout << "裁剪后为不规则图形,已保存为图片" << endl;
			myTextureManager->saveScreenshot(filePath.append(".bmp").c_str());
		}
		break;
	case OPENFILE:
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		glutMouseWheelFunc(polygonOnMouseWheelScrollInvalid);
		transformStatus = OPENFILE;
		cout << "请输入打开文件的路径:" << endl;
		cin >> filePath;
		filling = myPolygon.loadPolygonFromFile(filePath, texturePath);
		if (filling == 0)
			myPolygon.polygonUseLine();
		else if (filling == 1)
			myPolygon.fillPolygonScanLine();
		else if (filling == 2)
		{
			myTextureManager->changeTexturePath(textureID, texturePath);
			myPolygon.fillPolygonScanLine(GL_TRUE);
		}
		vertexPoints = myPolygon.getVertics();
		polygonSetTransBasisPoint(GL_TRUE);
		if (myPolygon.getPointsNum() == 0)
			transformStatus = EDIT;
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
			myPolygon.getPolygonPixels().begin()._Ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	case CLIP:
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		glutMouseWheelFunc(polygonOnMouseWheelScrollInvalid);
		transformStatus = CLIP;
		myClipWindow.vertex.clear();
		myClipWindow.clipStatus = 0;
		myClipWindow.clipPointIndex = 1;
		myClipWindow.isClose = GL_FALSE;
		break;
	case EXIT:
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		transformStatus = EXIT;
		break;
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
void polygonInitMenus()
{
	GLint subMenuFile = glutCreateMenu(polygonProcessMenuEvent);
	glutSetMenuFont(subMenuFile, GLUT_BITMAP_9_BY_15);
	glutAddMenuEntry("Open file", OPENFILE);
	glutAddMenuEntry("Save file", SAVEFILE);
	GLint subMenuFill = glutCreateMenu(polygonProcessMenuEvent);
	glutSetMenuFont(subMenuFill, GLUT_BITMAP_9_BY_15);
	glutAddMenuEntry("Fill with color", FILLCOLOR);
	glutAddMenuEntry("Fill with picture", FILLPICTURE);
	GLint menu = glutCreateMenu(polygonProcessMenuEvent);
	glutSetMenuFont(menu, GLUT_BITMAP_9_BY_15);
	glutAddMenuEntry("Edit Polygon", EDIT);
	glutAddMenuEntry("Move Polygon", MOVE);
	glutAddMenuEntry("Rotate Polygon", ROTATE);
	glutAddMenuEntry("Zoom Polygon", ZOOM);
	glutAddSubMenu("Fill Polygon", subMenuFill);
	glutAddMenuEntry("Clip Polygon", CLIP);
	glutAddSubMenu("File", subMenuFile);
	glutAddMenuEntry("Exit", EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutSetCursor(GLUT_CURSOR_INHERIT);
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
void polygonGetTransformMatrix(glm::ivec2 transformInfo, glm::ivec2 rotateInfo)
{
	//当处于移动状态时，传入transformInfo代表x，y方向上的移动距离，rotateInfo不传入数据
	//当处于旋转状态时，传入transformInfo、rotateInfo分别代表旋转开始时的鼠标位置和当前鼠标位置
	//当处于缩放状态时，传入transformInfo代表x，y方向上的缩放百分比，rotateInfo不传入数据
	glm::mat4 transformMat;
	glm::vec2 vecA, vecB;
	switch (transformStatus)
	{
	case drawPolygon::MOVE:
		transformMat = glm::translate(transformMat, glm::vec3(transformInfo.x, transformInfo.y, 0));
		break;
	case drawPolygon::ROTATE:
		GLfloat degree;//弧度值
		vecA = glm::vec2(transformInfo.x - rotateCenter.x, transformInfo.y - rotateCenter.y);
		vecB = glm::vec2(rotateInfo.x - rotateCenter.x, rotateInfo.y - rotateCenter.y);
		degree = acos((vecA.x*vecB.x + vecA.y*vecB.y) /
			sqrt((vecA.x*vecA.x + vecA.y*vecA.y)*(vecB.x*vecB.x + vecB.y*vecB.y)));
		if (vecA.x*vecB.y - vecA.y*vecB.x < 0)
			degree = -degree;
		transformMat = glm::translate(transformMat, glm::vec3(rotateCenter.x, rotateCenter.y, 0));
		transformMat = glm::rotate(transformMat, degree, glm::vec3(0, 0, 1));
		transformMat = glm::translate(transformMat, glm::vec3(-rotateCenter.x, -rotateCenter.y, 0));
		break;
	case drawPolygon::ZOOM:
		transformMat = glm::translate(transformMat, glm::vec3(rotateCenter.x, rotateCenter.y, 0));
		transformMat = glm::scale(transformMat, glm::vec3((100 + transformInfo.x) / 100.0, (100 + transformInfo.y) / 100.0, 0));
		transformMat = glm::translate(transformMat, glm::vec3(-rotateCenter.x, -rotateCenter.y, 0));
		break;
	}
	glm::vec4 tempPoint = glm::vec4(0, 0, 0, 1);
	if (transformStatus == drawPolygon::ROTATE)
	{
		for (int i = 0; i < transBasisPoint.size(); i++)
		{
			tempPoint.x = transBasisPoint[i].x, tempPoint.y = transBasisPoint[i].y;
			tempPoint = transformMat*tempPoint;
			vertexPoints[i].x = (GLint)tempPoint.x, vertexPoints[i].y = (GLint)tempPoint.y;
			myPolygon.setVertics(vertexPoints[i], i + 1);
		}
	}
	else
	{
		for (int i = 0; i < vertexPoints.size(); i++)
		{
			tempPoint.x = vertexPoints[i].x, tempPoint.y = vertexPoints[i].y;
			tempPoint = transformMat*tempPoint;
			vertexPoints[i].x = (GLint)tempPoint.x, vertexPoints[i].y = (GLint)tempPoint.y;
			myPolygon.setVertics(vertexPoints[i], i + 1);
		}
	}
}
void polygonSetTransBasisPoint(GLboolean initTansBasis)
{
	if (initTansBasis)
	{
		transBasisPoint.clear();
		for (int i = 0; i < vertexPoints.size(); i++)
		{
			transBasisPoint.push_back(glm::ivec3(vertexPoints[i].x, vertexPoints[i].y, 0));
		}
	}
	else
	{
		for (int i = 0; i < vertexPoints.size(); i++)
		{
			transBasisPoint[i].x = vertexPoints[i].x;
			transBasisPoint[i].y = vertexPoints[i].y;
		}
	}

}


