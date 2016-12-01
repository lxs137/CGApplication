#include "windowSetting.h"

DRAWINGMODEL drawingModel=DRAWPOLYGON;

void drawLineApplication(int argc, char **argv);
//void drawCircleApplication(int argc, char **argv);
void drawEllipseApplication(int argc, char **argv);
void drawBezierApplication(int argc, char **argv);
void drawSplineApplication(int argc, char **argv);
void drawPolygonApplication(int argc, char **argv);

int main(int argc,char *argv[])
{
	while (true)
	{
		switch (drawingModel)
		{
		case DRAWLINE:
			drawLineApplication(argc, argv);
			break;
		case DRAWCIRCLE:
			//drawCircleApplication(argc, argv);
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
		}
	}
}