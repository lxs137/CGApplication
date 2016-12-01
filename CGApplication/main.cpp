//#include <iostream>
//#include <fstream>
//#include <vector>
//#include "line.h"
//#include "circle.h"
//#include "ellipse.h"
//#include "bezier.h"
//#include "splineCurve.h"
//#include "polygon.h"
//#include "shader.h"
//#include "textureManager.h"
//#include "windowSetting.h"
//using namespace std;
//
//#include <freeglut\glut.h>
//#include <glm\glm.hpp>
//#include <glm\gtc\matrix_transform.hpp>
//#include <glm\gtc\type_ptr.hpp>
//
//line myLine;
//circle myCircle;
//ellipse myEllipse;
//bezier myBezier;
//spline mySpline;
//polygon myPolygon;
//
//TextureManager *myTextureManager;
//GLuint myShaderProgram;
//GLuint myVAO;
//
//GLfloat lastMouseX = WIDTH_HALF, lastMouseY = HEIGHT_HALF;
//
//
//void initGlutWindow();
//void loadImageAsTexture(unsigned int testTextureID, const char *filename);
//GLuint initVAO();
//glm::mat4 getTransformMatrix();
////glut function
//void render2DSence();
//void onMouseClick(int button, int state, int x, int y);
//void onReshape(int width, int height);
//
//GLint64 pixelSize=0;
//
//int main(int argc, char *argv[])
//{
//	glutInit(&argc, argv);
//	initGlutWindow();
//
//	//set shader program	
//	shader *myShader = new shader("2dModel.vert", "2dModel.frag");
//	myShaderProgram = myShader->shaderProgram;
//
//	//set VAO
//	myVAO = initVAO();
//
//	//unsigned int testTextureID=1;
//	//myTextureManager = new TextureManager();
//	//loadImageAsTexture(testTextureID, "E:\\CGApplication\\img_test.png");
//
//	//set transform
//	GLuint transformLocation = glGetUniformLocation(myShaderProgram, "transform");
//	glm::mat4 transformMat = getTransformMatrix();
//
//	glutDisplayFunc(render2DSence);
//	glutIdleFunc(render2DSence);
//	glutReshapeFunc(onReshape);
//	glutMainLoop();
//}
//
//void render2DSence()
//{
//	glClearColor(1.0f, 0.8f, 1.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glUseProgram(myShaderProgram);
//	//myTextureManager->bindTexture(testTextureID);
//	glBindVertexArray(myVAO);
//	//glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));
//	glDrawArrays(GL_POINTS, 0, pixelSize);
//	glBindVertexArray(0);
//
//	GLfloat x1 = 300, y1 = -200, x2 = 100, y2 = 250, x3 = 100, y3 = -150, x4 = -100, y4 = -150,
//		x5 = -100, y5 = 200, x6 = -150, y6 = 200, x7 = -150, y7 = -200;
//	x1 /= WIDTH_HALF, x2 /= WIDTH_HALF, x3 /= WIDTH_HALF, x4 /= WIDTH_HALF, x5 /= WIDTH_HALF, x6 /= WIDTH_HALF, x7 /= WIDTH_HALF;
//	y1 /= HEIGHT_HALF, y2 /= HEIGHT_HALF, y3 /= HEIGHT_HALF, y4 /= HEIGHT_HALF, y5 /= WIDTH_HALF, y6 /= WIDTH_HALF, y7 /= WIDTH_HALF;
//	glLineStipple(2, 0x5555);
//	glEnable(GL_LINE_STIPPLE);
//	glBegin(GL_LINE_LOOP);
//	glVertex2f(x1, y1);
//	glVertex2f(x2, y2);
//	glVertex2f(x3, y3);
//	glVertex2f(x4, y4);
//	glVertex2f(x5, y5);
//	glVertex2f(x6, y6);
//	glVertex2f(x7, y7);
//	glEnd();
//	glDisable(GL_LINE_STIPPLE);
//	glFlush();
//
//	glutSwapBuffers();
//}
//void onMouseClick(int button, int state, int x, int y)
//{
//
//}
//void onReshape(int width, int height)
//{
//
//}
//void initGlutWindow()
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
//void loadImageAsTexture(unsigned int testTextureID, const char *filename)
//{
//	myTextureManager->loadTexture(filename, testTextureID);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glBindTexture(GL_TEXTURE_2D, 0);
//}
//GLuint initVAO()
//{
//	GLuint VBO, VAO;
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	//set vertex buffer and attribute point
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//	//myLine=line(glm::ivec3(-150, 250, 0), glm::ivec3(200, -200, 0), glm::vec3(1.0f, 0.0f, 0.0f));
//	//myLine.lineUseBresenham();
//	////myLine.lineUseDDA();
//	////myLine.clipUseRect(glm::ivec3(100,-200,0), glm::ivec3(0,200,0));
//	//glBufferData(GL_ARRAY_BUFFER,myLine.getPointsNum()*myLine.getPointSize(), 
//	//	myLine.getLinePixels().begin()._Ptr, GL_STATIC_DRAW);
//
//	//myCircle = circle(glm::ivec3(-50, -50, 0), 100, glm::vec3(1.0f, 0.0f, 0.0f));
//	//myCircle.circleUseMidpoint();
//	//myCircle.fillCircleScanLine(glm::vec3(0.0f, 1.0f, 0.0f));
//	//myCircle.clipUseRect(glm::vec3(-100, -100, 0), glm::vec3(100, 150, 0));
//	//glBufferData(GL_ARRAY_BUFFER, myCircle.getPointsNum()*myCircle.getPointSize(),
//	//	myCircle.getCirclePixels().begin()._Ptr, GL_STATIC_DRAW);
//
//	//myEllipse = ellipse(glm::ivec3(-50, 0, 0), 250, 150, glm::vec3(1.0f, 0.0f, 0.0f));
//	//myEllipse.ellipseUseMidpoint();
//	//myEllipse.fillEllipseScanLine(glm::vec3(0.0f, 1.0f, 0.0f));
//	//myEllipse.clipUseRect(glm::ivec3(-200, 0, 0), glm::ivec3(170, 200, 0));
//	//glBufferData(GL_ARRAY_BUFFER, myEllipse.getPointsNum()*myEllipse.getPointSize(),
//	//	myEllipse.getEllipsePixels().begin()._Ptr, GL_STATIC_DRAW);
//
//	//myBezier = bezier(glm::ivec3(-250, -200, 0), glm::ivec3(-100, 50, 0), 
//	//	glm::ivec3(150, 250, 0),glm::ivec3(300, -100, 0), glm::vec3(1.0f, 0.0f, 0.0f));
//	//myBezier.bezierUseLine();
//	//myBezier.clipUseRect(glm::ivec3(-200, 0, 0), glm::ivec3(170, 200, 0));
//	//glBufferData(GL_ARRAY_BUFFER, myBezier.getPointsNum()*myBezier.getPointSize(),
//	//	myBezier.getBezierPixels().begin()._Ptr, GL_STATIC_DRAW);
//
//	//mySpline = spline(glm::ivec3(-250, -200, 0), glm::ivec3(-150, 100, 0),
//	//		glm::ivec3(150, 250, 0),glm::ivec3(300, -100, 0), glm::vec3(1.0f, 0.0f, 0.0f));
//	//mySpline.splineUseLine();
//	////mySpline.clipUseRect(glm::ivec3(-100, 0, 0), glm::ivec3(250, 150, 0));
//	//mySpline.showControlPoints();
//	//glBufferData(GL_ARRAY_BUFFER, mySpline.getPointsNum()*mySpline.getPointSize(),
//	//	mySpline.getSplinePixels().begin()._Ptr, GL_STATIC_DRAW);
//
//	vector<glm::ivec3> verticsPoint;
//	vector<glm::ivec3> windowPoint;
//	vector<polygon> resultPolygon;
//	verticsPoint.push_back(glm::ivec3(-200, 100, 0));
//	verticsPoint.push_back(glm::ivec3(250, 150, 0));
//	verticsPoint.push_back(glm::ivec3(300, -100, 0));
//	verticsPoint.push_back(glm::ivec3(-250, -130, 0));
//	verticsPoint.push_back(glm::ivec3(-250, -80, 0));
//	verticsPoint.push_back(glm::ivec3(200, -50, 0));
//	verticsPoint.push_back(glm::ivec3(100, 100, 0));
//	verticsPoint.push_back(glm::ivec3(-200, 70, 0));
//
//	windowPoint.push_back(glm::ivec3(300, -200, 0));
//	windowPoint.push_back(glm::ivec3(100, 250, 0));
//	windowPoint.push_back(glm::ivec3(100, -150, 0));
//	windowPoint.push_back(glm::ivec3(-100, -150, 0));
//	windowPoint.push_back(glm::ivec3(-100, 200, 0));
//	windowPoint.push_back(glm::ivec3(-150, 200, 0));
//	windowPoint.push_back(glm::ivec3(-150, -200, 0));
//	myPolygon = polygon(verticsPoint, glm::vec3(0.0f, 0.0f, 0.0f));
//	myPolygon.polygonUseLine();
//	myPolygon.fillPolygonScanLine(glm::vec3(0.0f, 1.0f, 0.0f));
//	resultPolygon = myPolygon.clipWithPolygon(windowPoint);
//	vector<GLfloat> resultPixels = myPolygon.getPolygonPixels();
//	vector<GLfloat>::iterator iVector = resultPixels.end();
//	pixelSize += myPolygon.getPointsNum();
//	for (int i = 0; i < resultPolygon.size(); i++)
//	{
//		polygon onePolygon = resultPolygon[i];
//		onePolygon.fillPolygonScanLine(glm::ivec3(0.0f, 1.0f, 0.0f));
//		vector<GLfloat> onePolygonPixels = onePolygon.getPolygonPixels();	
//		resultPixels.insert(iVector, onePolygonPixels.begin(), 
//			onePolygonPixels.end());
//		iVector = resultPixels.end();
//		pixelSize += onePolygon.getPointsNum();
//	}
//	glBufferData(GL_ARRAY_BUFFER, resultPixels.size()*4,
//		resultPixels.begin()._Ptr, GL_STATIC_DRAW);
//	//glBufferData(GL_ARRAY_BUFFER, myPolygon.getPointsNum()*myPolygon.getPointSize(),
//	//	myPolygon.getPolygonPixels().begin()._Ptr, GL_STATIC_DRAW);
//
//	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(1);
//	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
//	//glEnableVertexAttribArray(2);
//
//	//unbind VBO&VAO
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//	return VAO;
//}
//glm::mat4 getTransformMatrix()
//{
//	glm::mat4 transformMat;
//	//transformMat = glm::rotate(transformMat,(GLfloat)glfwGetTime()*PI/2, glm::vec3(0, 0, 1));
//	transformMat = glm::scale(transformMat, glm::vec3(1.0, 1.0, 1.0));
//	return transformMat;
//}