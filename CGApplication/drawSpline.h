#ifndef DRAWSPLINE_H
#define DRAWSPLINE_H

#include <iostream>
#include <fstream>
#include <vector>
#include "textureManager.h"
#include "windowSetting.h"

#include <freeglut\glut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace drawSpline{
	TextureManager *myTextureManager;
	GLuint myShaderProgram;
	GLuint myVAO;
	GLuint myVBO;
	GLfloat lastMouseX = WIDTH_HALF, lastMouseY = HEIGHT_HALF;
	GLint drawStatus = 0;//0代表未处于绘制状态；1代表正在初始化控制点；2表示正在修改控制点
	std::vector<glm::ivec3> controlPoints;//Bezier曲线的四个控制点
	GLint drawingPointIndex = 1;//有1，2，3，4四种值，代表四个控制点,表示正在初始化或者修改的控制点
}
void splineRender2DSence();
void splineOnMouseClick(int button, int state, int x, int y);
void splineOnActiveMotion(int x, int y);
void splineOnReshape(int width, int height);
void splineInitGlutWindow();
void splineInitVAO(GLuint &VAO, GLuint &VBO);
glm::mat4 splineGetTransformMatrix();

#endif // !DRAWSPLINE_H
