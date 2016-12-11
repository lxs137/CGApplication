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
	GLint drawStatus = 0;//0代表未处于绘制状态；1代表正在初始化控制点；2表示正在修改控制点
	GLboolean cliping = GL_FALSE;
	std::vector<glm::ivec3> controlPoints;//Bezier曲线的四个控制点
	std::vector<glm::ivec3> transBasisPoint;
	glm::ivec3 rotateCenter = glm::ivec3(0, 0, 0);
	GLint drawingPointIndex = 1;//有1，2，3，4四种值，代表四个控制点,表示正在初始化或者修改的控制点
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
