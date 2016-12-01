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
	std::vector<glm::ivec3> controlPoints;//����εĸ�������
	GLint drawingPointIndex = 1;//�������εĸ������㣨��1��ʼ��,��ʾ���ڳ�ʼ�������޸ĵĶ���
}

#endif // !DRAWPOLYGON_H
