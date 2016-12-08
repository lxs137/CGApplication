#ifndef DRAWELLIPSE_H
#define DRAWELLIPSE_H
#include <iostream>
#include <vector>
#include "textureManager.h"
#include "windowSetting.h"

#include <glm\glm.hpp>

namespace drawEllipse{
	TextureManager *myTextureManager;
	GLuint myShaderProgram;
	GLuint myVAO;
	GLuint myVBO;
	GLboolean drawing = GL_FALSE;
	GLboolean filling = GL_FALSE;
	GLfloat lastMouseX = WIDTH_HALF, lastMouseY = HEIGHT_HALF;
	std::vector<glm::ivec3> controlPoints;
	enum MenuOptions
	{
		EDIT,
		MOVE,
		ROTATE,
		ZOOM,
		FILL,
		EXIT
	};
	MenuOptions transformStatus = EDIT;
};
void ellipseRender2DSence();
void ellipseOnMouseClick(int button, int state, int x, int y);
void ellipseOnActiveMotion(int x, int y);
void ellipseOnMouseWheelScrollValid(int wheel, int direction, int x, int y);
void ellipseOnMouseWheelScrollInvalid(int wheel, int direction, int x, int y);
void ellipseProcessMenuEvent(int options);
void ellipseOnReshape(int width, int height);
void ellipseInitGlutWindow();
void ellipseInitMenus();
void ellipseLoadImageAsTexture(unsigned int testTextureID, const char *filename);
void ellipseInitVAO(GLuint &VAO,GLuint &VBO);
void ellipseGetTransformMatrix(glm::ivec2 transformInfo, glm::ivec2 rotateInfo = glm::ivec2(0, 0));
#endif // !DRAWELLIPSE_H
