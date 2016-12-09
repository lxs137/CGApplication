#include "windowSetting.h"

DRAWINGMODEL drawingModel=DRAWELLIPSE;

void drawLineApplication(int argc, char **argv);
void drawCircleApplication(int argc, char **argv);
void drawEllipseApplication(int argc, char **argv);
void drawBezierApplication(int argc, char **argv);
void drawSplineApplication(int argc, char **argv);
void drawPolygonApplication(int argc, char **argv);
void draw3DApplication(int argc, char **argv);

int main(int argc,char *argv[])
{
	while (true)
	{
		switch (drawingModel)
		{
		case DRAWLINE:
			drawLineApplication(argc, argv);
			drawingModel = DRAW3D;
			break;
		case DRAWCIRCLE:
			drawCircleApplication(argc, argv);
			break;
		case DRAWELLIPSE:
			drawEllipseApplication(argc, argv);
			break;
		case DRAWBEZIER:
			drawBezierApplication(argc, argv);
			break;
		case DRAWSPLINE:
			drawSplineApplication(argc, argv);
			break;
		case DRAWPOLYGON:
			drawPolygonApplication(argc, argv);
			break;
		case DRAW3D:
			draw3DApplication(argc, argv);
			break;
		}
	}
}