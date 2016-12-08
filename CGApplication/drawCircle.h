#ifndef DRAWCIRCLE_H
#define DRAWCIRCLE_H

#include <iostream>
#include "textureManager.h"
#include "windowSetting.h"

#include <glm\glm.hpp>

namespace drawCircle{
	TextureManager *myTextureManager;
	GLuint myShaderProgram;
	GLuint myVAO;
	GLuint myVBO;
	GLfloat lastMouseX = WIDTH_HALF, lastMouseY = HEIGHT_HALF;
	GLboolean drawing = GL_FALSE;
	GLboolean filling = GL_FALSE;
	std::vector<glm::ivec3> controlPoints;
	glm::ivec3 rotateCenter = glm::ivec3(0, 0, 0);
	enum MenuOptions
	{
		EDIT,
		MOVE,
		ZOOM,
		FILL,
		EXIT
	};
	MenuOptions transformStatus = EDIT;
};
void circleRender2DSence();
void circleOnMouseClick(int button, int state, int x, int y);
void circleOnActiveMotion(int x, int y);
void circleOnMouseWheelScrollValid(int wheel, int direction, int x, int y);
void circleOnMouseWheelScrollInvalid(int wheel, int direction, int x, int y);
void circleOnReshape(int width, int height);
void circleProcessMenuEvent(int options);
void circleInitGlutWindow();
void circleLoadImageAsTexture(unsigned int testTextureID, const char *filename);
void circleInitVAO(GLuint &VAO,GLuint &VBO);
void circleInitMenus();
void circleGetTransformMatrix(glm::ivec2 transformInfo);
#endif // !DRAWCIRCLE_H
