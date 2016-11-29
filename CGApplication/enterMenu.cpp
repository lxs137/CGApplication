#include "windowSetting.h"

DRAWINGMODEL drawingModel=DRAWELLIPSE;

void drawLineApplication(int argc, char **argv);
//void drawCircleApplication(int argc, char **argv);
void drawEllipseApplication(int argc, char **argv);

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
		}
	}
}