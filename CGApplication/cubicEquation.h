#ifndef CUBICEQUATION_H
#define CUBICEQUATION_H

#define GLEW_STATIC
#include <gl\glew.h>
#include <vector>
#include <cmath>

inline GLfloat fx(GLfloat nparam[3], GLfloat x)
{
	return pow(x, 3) + nparam[0] * pow(x, 2) + nparam[1] * x + nparam[2];
}
GLint oneresult(GLfloat nparam[3], GLfloat nrange[2], GLfloat via, GLint position)
{
	GLfloat i = via;
	nrange[0] = via;
	if (position == 0)
	{
		while (fx(nparam, i)*fx(nparam, via) > 0)
		{
			if (i > -10)
				i = i - 0.1;
			else
				i = i - 1;
			if (i<-100)
				return 1;
		}
		nrange[1] = i;
	}
	else
	{
		while (fx(nparam, i)*fx(nparam, via)>0)
		{
			if (i<10)
				i = i + 0.1;
			else
				i = i + 1;
			if (i>100)
				return 1;
		}
		nrange[1] = i;
	}
	return 0;
}
GLfloat dichotomy(GLfloat nparam[3], GLfloat x1, GLfloat x2)
{
	GLfloat x0;
	GLfloat x3;
	while (x0 = (x1 + x2) / 2, (x3 = fabs(fx(nparam, x0))) > 1e-5)
	{
		if (fx(nparam, x0)*fx(nparam, x2) > 0)
			x2 = x0;
		else
			x1 = x0;
	}
	return x0;
}

std::vector<GLfloat> findRealRoot(GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
	GLfloat param[3];
	GLfloat dparam[3];
	GLfloat range[2];
	std::vector<GLfloat> result;
	GLfloat x1, x2;
	if (a == 0)
		return ;
	param[0] = b / a;
	param[1] = c / a;
	param[2] = d / a;
	dparam[0] = 3;
	dparam[1] = 2 * param[0];
	dparam[2] = param[1];
	if (dparam[1] * dparam[1] - 4 * dparam[2] * dparam[0] < 0)  
	{
		//单调递增的,只有一个实数解
		if (1 == oneresult(param, range, 0, 1) && 1 == oneresult(param, range, 0, 0))
		{
			return ;
		}
		else
		{
			if (1 == oneresult(param, range, 0, 1))
				oneresult(param, range, 0, 0);
			result.push_back(dichotomy(param, range[0], range[1]));
		}
	}
	else
	{
		x1 = (-dparam[1] + sqrt(dparam[1] * dparam[1] - 4 * dparam[2] * dparam[0])) / (2 * dparam[0]);
		x2 = (-dparam[1] - sqrt(dparam[1] * dparam[1] - 4 * dparam[2] * dparam[0])) / (2 * dparam[0]);
		if (fx(param, x2)*fx(param, x1) <= 0)  
		{
			//极大值和极小值处函数值符号相反,则肯定有三个实数解
			oneresult(param, range, x2, 0);
			result.push_back(dichotomy(param, range[0], range[1]));
			oneresult(param, range, x1, 1);
			result.push_back(dichotomy(param, range[0], range[1]));
			result.push_back(dichotomy(param, x2, x1));
		}
		else  
		{
			//极大值和极小值处函数值符号相同,则肯定只有有一个实数解,两个虚数解
			oneresult(param, range, x1, 1);
			result.push_back(dichotomy(param, range[0], range[1]));
		}
	}
	return result;
}

#endif
