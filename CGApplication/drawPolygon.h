#ifndef DRAWPOLYGON_H
#define DRAWPOLYGON_H

#include <iostream>
#include <vector>
#include "textureManager.h"
#include "windowSetting.h"

#include <glm\glm.hpp>

namespace drawPolygon
{
	TextureManager *myTextureManager;
	GLuint myShaderProgram;
	GLuint myVAO;
	GLuint myVBO;
	GLfloat lastMouseX = WIDTH_HALF, lastMouseY = HEIGHT_HALF;
	GLint drawStatus = 0;//0����δ���ڻ���״̬��1�������ڳ�ʼ�����㣻2��ʾ�����޸Ķ���
	std::vector<glm::ivec3> vertexPoints;//����εĸ�������
	GLint drawingPointIndex = 1;//����εĸ������㣨��1��ʼ��,��ʾ���ڳ�ʼ�������޸ĵĶ���
}
void polygonRender2DSence();
void polygonOnMouseClick(int button, int state, int x, int y);
void polygonOnActiveMotion(int x, int y);
void polygonOnReshape(int width, int height);
void polygonInitGlutWindow();
void polygonInitVAO(GLuint &VAO, GLuint &VBO);
glm::mat4 polygonGetTransformMatrix();

#endif // !DRAWPOLYGON_H
