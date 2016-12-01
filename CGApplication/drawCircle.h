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
	GLfloat lastMouseX = WIDTH_HALF, lastMouseY = HEIGHT_HALF;
};
void circleRender2DSence();
void circleOnMouseClick(int button, int state, int x, int y);
void circleOnReshape(int width, int height);
void circleInitGlutWindow();
void circleLoadImageAsTexture(unsigned int testTextureID, const char *filename);
GLuint circleInitVAO();
glm::mat4 circleGetTransformMatrix();
#endif // !DRAWCIRCLE_H
