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
	GLint filling = 0;//0����δ��䣬1��������ɫ��䣬2������ͼ�����
	std::vector<glm::ivec3> vertexPoints;//����εĸ�������
	std::vector<glm::ivec3> transBasisPoint;
	glm::ivec3 rotateCenter = glm::ivec3(0, 0, 0);
	GLint drawingPointIndex = 1;//����εĸ������㣨��1��ʼ��,��ʾ���ڳ�ʼ�������޸ĵĶ���
	unsigned int textureID = 2;
	enum MenuOptions
	{
		EDIT,
		MOVE,
		ROTATE,
		ZOOM,
		FILLCOLOR,
		FILLPICTURE,
		EXIT
	};
	MenuOptions transformStatus = EDIT;
}
void polygonRender2DSence();
void polygonOnMouseClick(int button, int state, int x, int y);
void polygonOnActiveMotion(int x, int y);
void polygonOnMouseWheelScrollValid(int wheel, int direction, int x, int y);
void polygonOnMouseWheelScrollInvalid(int wheel, int direction, int x, int y);
void polygonProcessMenuEvent(int options);
void polygonOnReshape(int width, int height);
void polygonInitGlutWindow();
void polygonInitMenus();
void polygonInitVAO(GLuint &VAO, GLuint &VBO);
void polygonGetTransformMatrix(glm::ivec2 transformInfo, glm::ivec2 rotateInfo = glm::ivec2(0, 0));
void polygonSetTransBasisPoint();

#endif // !DRAWPOLYGON_H
