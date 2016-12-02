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
	std::vector<glm::ivec3> controlPoints;
};
void circleRender2DSence();
void circleOnMouseClick(int button, int state, int x, int y);
void circleOnActiveMotion(int x, int y);
void circleOnReshape(int width, int height);
void circleInitGlutWindow();
void circleLoadImageAsTexture(unsigned int testTextureID, const char *filename);
void circleInitVAO(GLuint &VAO,GLuint &VBO);
glm::mat4 circleGetTransformMatrix();
#endif // !DRAWCIRCLE_H
