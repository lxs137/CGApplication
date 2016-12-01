#ifndef DRAWBEZIER_H
#define DRAWBEZIER_H

#include <iostream>
#include <vector>
#include "textureManager.h"
#include "windowSetting.h"

#include <glm\glm.hpp>

namespace drawBezier{
	TextureManager *myTextureManager;
	GLuint myShaderProgram;
	GLuint myVAO;
	GLuint myVBO;
	GLfloat lastMouseX = WIDTH_HALF, lastMouseY = HEIGHT_HALF;
	GLint drawStatus = 0;//0����δ���ڻ���״̬��1�������ڳ�ʼ�����Ƶ㣻2��ʾ�����޸Ŀ��Ƶ�
	std::vector<glm::ivec3> controlPoints;//Bezier���ߵ��ĸ����Ƶ�
	GLint drawingPointIndex = 1;//��1��2��3��4����ֵ�������ĸ����Ƶ�,��ʾ���ڳ�ʼ�������޸ĵĿ��Ƶ�
}
void bezierRender2DSence();
void bezierOnMouseClick(int button, int state, int x, int y);
void bezierOnActiveMotion(int x, int y);
void bezierOnReshape(int width, int height);
void bezierInitGlutWindow();
void bezierLoadImageAsTexture(unsigned int testTextureID, const char *filename);
void bezierInitVAO(GLuint &VAO, GLuint &VBO);
glm::mat4 bezierGetTransformMatrix();

#endif // !DRAWBEZIER_H
