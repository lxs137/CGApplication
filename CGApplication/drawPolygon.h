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
	GLint drawStatus = 0;//0代表未处于绘制状态；1代表正在初始化顶点；2表示正在修改顶点
	std::vector<glm::ivec3> controlPoints;//多边形的各个顶点
	GLint drawingPointIndex = 1;//代表多边形的各个顶点（从1开始）,表示正在初始化或者修改的顶点
}

#endif // !DRAWPOLYGON_H
