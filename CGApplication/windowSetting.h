#ifndef WINDOWSETTING_H
#define WINDOWSETTING_H
#define GLEW_STATIC
#include <gl/glew.h>
const GLfloat PI = 3.14159f;
const GLint WIDTH = 800, HEIGHT = 600;
const GLint WIDTH_HALF = 400, HEIGHT_HALF = 300;
const GLfloat BEZIER_DERT_T = 0.01f;
const GLfloat SPLINE_DERT_T = 0.01f;
const GLint NEXT_POINT_DERT = 5;
const GLint CHANGE_POINT_DIS = 6;
enum DRAWINGMODEL
{
	DRAWLINE,
	DRAWCIRCLE,
	DRAWELLIPSE,
	DRAWBEZIER
};
#endif