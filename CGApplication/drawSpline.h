#ifndef DRAWSPLINE_H
#define DRAWSPLINE_H

#include <iostream>
#include <vector>
#include "textureManager.h"
#include "windowSetting.h"

#include <glm\glm.hpp>

namespace drawSpline{
	TextureManager *myTextureManager;
	GLuint myShaderProgram;
	GLuint myVAO;
	GLuint myVBO;
	GLfloat lastMouseX = WIDTH_HALF, lastMouseY = HEIGHT_HALF;
	GLint drawStatus = 0;//0����δ���ڻ���״̬��1�������ڳ�ʼ�����Ƶ㣻2��ʾ�����޸Ŀ��Ƶ�
	std::vector<glm::ivec3> controlPoints;//Bezier���ߵ��ĸ����Ƶ�
	GLint drawingPointIndex = 1;//��1��2��3��4����ֵ�������ĸ����Ƶ�,��ʾ���ڳ�ʼ�������޸ĵĿ��Ƶ�
}
void splineRender2DSence();
void splineOnMouseClick(int button, int state, int x, int y);
void splineOnActiveMotion(int x, int y);
void splineOnReshape(int width, int height);
void splineInitGlutWindow();
void splineInitVAO(GLuint &VAO, GLuint &VBO);
glm::mat4 splineGetTransformMatrix();

#endif // !DRAWSPLINE_H