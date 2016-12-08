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
	GLint drawStatus = 0;//0代表未处于绘制状态；1代表正在初始化控制点；2表示正在修改控制点
	std::vector<glm::ivec3> controlPoints;//Bezier曲线的四个控制点
	std::vector<glm::ivec3> transBasisPoint;
	glm::ivec3 rotateCenter = glm::ivec3(0, 0, 0);
	GLint drawingPointIndex = 1;//有1，2，3，4四种值，代表四个控制点,表示正在初始化或者修改的控制点
	enum MenuOptions
	{
		EDIT,
		MOVE,
		ROTATE,
		ZOOM,
		EXIT
	};
	MenuOptions transformStatus = EDIT;
}
void splineRender2DSence();
void splineOnMouseClick(int button, int state, int x, int y);
void splineOnActiveMotion(int x, int y);
void splineOnMouseWheelScrollValid(int wheel, int direction, int x, int y);
void splineOnMouseWheelScrollInvalid(int wheel, int direction, int x, int y);
void splineProcessMenuEvent(int options);
void splineOnReshape(int width, int height);
void splineInitGlutWindow();
void splineInitMenus();
void splineInitVAO(GLuint &VAO, GLuint &VBO);
void splineGetTransformMatrix(glm::ivec2 transformInfo, glm::ivec2 rotateInfo = glm::ivec2(0, 0));
void splineSetTransBasisPoint();

#endif // !DRAWSPLINE_H
