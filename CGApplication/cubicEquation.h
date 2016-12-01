#ifndef CUBICEQUATION_H
#define CUBICEQUATION_H

#include <vector>

inline GLfloat fx(GLfloat nparam[3], GLfloat x);
GLint oneresult(GLfloat nparam[3], GLfloat nrange[2], GLfloat via, GLint position);
//二分法求结果
GLfloat dichotomy(GLfloat nparam[3], GLfloat x1, GLfloat x2);
std::vector<GLfloat> findRealRoot(GLfloat a, GLfloat b, GLfloat c, GLfloat d);

#endif
