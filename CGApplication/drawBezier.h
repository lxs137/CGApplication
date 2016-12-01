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
	std::vector<glm::ivec3> controlPoints;//Bezier曲线的四个控制点
	GLint drawingPointIndex = 1;//有1，2，3，4四种值，代表四个控制点,表示正在初始化或者修改的控制点
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
