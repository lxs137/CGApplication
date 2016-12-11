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
	GLboolean cliping = GL_FALSE;
	std::vector<glm::ivec3> controlPoints;//Bezier���ߵ��ĸ����Ƶ�
	std::vector<glm::ivec3> transBasisPoint;
	glm::ivec3 rotateCenter = glm::ivec3(0, 0, 0);
	GLint drawingPointIndex = 1;//��1��2��3��4����ֵ�������ĸ����Ƶ�,��ʾ���ڳ�ʼ�������޸ĵĿ��Ƶ�
	struct ClipWindow
	{
		glm::ivec3 clipWindowCenter;
		GLint windowHeightHalf;
		GLint windowWidthHalf;
	}myClipWindow;
	enum MenuOptions
	{
		EDIT,
		MOVE,
		ROTATE,
		ZOOM,
		CLIP,
		SAVEFILE,
		OPENFILE,
		EXIT
	};
	MenuOptions transformStatus = EDIT;
}
void bezierRender2DSence();
void bezierOnMouseClick(int button, int state, int x, int y);
void bezierOnActiveMotion(int x, int y);
void bezierOnMouseWheelScrollValid(int wheel, int direction, int x, int y);
void bezierOnMouseWheelScrollInvalid(int wheel, int direction, int x, int y);
void bezierProcessMenuEvent(int options);
void bezierOnReshape(int width, int height);
void bezierInitGlutWindow();
void bezierInitMenus();
void bezierLoadImageAsTexture(unsigned int testTextureID, const char *filename);
void bezierInitVAO(GLuint &VAO, GLuint &VBO);
void bezierGetTransformMatrix(glm::ivec2 transformInfo, glm::ivec2 rotateInfo = glm::ivec2(0, 0));
void bezierSetTransBasisPoint();

#endif // !DRAWBEZIER_H
