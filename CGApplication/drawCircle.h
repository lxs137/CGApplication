#ifndef DRAWCIRCLE_H
#define DRAWCIRCLE_H

#include <iostream>
#include <fstream>
#include <vector>
#include "circle.h"
#include "shader.h"
#include "textureManager.h"
#include "windowSetting.h"

#include <freeglut\glut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

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
