// Cohen-Sutherland.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<windows.h>
#include"GL/glu.h"
#include<GL/gl.h>
#include<GL/glut.h>
using namespace std;

//数据域
class wcPt2D {
public:
	float x, y;
};
GLubyte encode(wcPt2D pt, wcPt2D winMin, wcPt2D winMax);
void swapPts(wcPt2D *p1, wcPt2D * p2);
void swapCodes(GLubyte * c1, GLubyte *c2);
void lineClipCohSuth(wcPt2D winMin, wcPt2D winMax, wcPt2D p1, wcPt2D p2);
//编码
const GLint winLeftBitCode = 0x1;
const GLint winRightBitCode = 0x2;
const GLint winBottomBitCode = 0x4;
const GLint winTopBitCode = 0x8;
//判断是否在窗口内，窗口外或者部分窗口内
inline GLint inside(GLint code);
inline GLint reject(GLint code1, GLint code2);
inline GLint accept(GLint code1, GLint code2);

//函数定义
//端点的编码
GLubyte encode(wcPt2D pt, wcPt2D winMin, wcPt2D winMax);
//交换点
void swapPts(wcPt2D *p1, wcPt2D *p2);
//交换编码
void swapCodes(GLubyte *c1, GLubyte *c2);
//核心，裁剪算法
void lineClipCohSuth(wcPt2D winMin, wcPt2D winMax, wcPt2D p1, wcPt2D p2);
//初始化函数
void init(void);
//键盘交互Enter和Esc
void myKeyBoard(unsigned char key, int x, int y);
//显示窗口和未裁剪的直线
void display();

//主函数
void main(int argc, char* argv) {
	glutInit(&argc, &argv);
	glutInitWindowPosition(50, 100);        
	glutInitWindowSize(600, 600);  
	glutCreateWindow("CLIP_LINE");
	glutKeyboardFunc(myKeyBoard);
	init();
	glutDisplayFunc(display);
	glutMainLoop();
}
//判断是否在窗口内，窗口外或者部分窗口内
inline GLint inside(GLint code) {
	return GLint(!code);
}
inline GLint reject(GLint code1, GLint code2) {
	return GLint(code1 & code2);
}
inline GLint accept(GLint code1, GLint code2) {
	return GLint(!(code1 | code2));
}

//端点的编码
GLubyte encode(wcPt2D pt, wcPt2D winMin, wcPt2D winMax) {
	GLubyte code = 0x00;
	if (pt.x < winMin.x) {
		code = code | winLeftBitCode;
	}
	if (pt.x > winMax.x) {
		code = code | winRightBitCode;
	}
	if (pt.y < winMin.y) {
		code = code | winBottomBitCode;
	}
	else
		if (pt.y > winMax.y) {
			code = code | winTopBitCode;
		}
	return (code);//此处括号应该为了强调返回的是编码。
/*PS：书中的代码的风格可读性较差，因为else－if之间使用换行缩进导致想看清if-else之间的匹配比较困难
并且if在只有一行代码的情况下直接换行,界限模糊，应该加上｛｝使得界限更加明显，尽管这样会多用两行.
*/
}

//交换点
void swapPts(wcPt2D *p1, wcPt2D *p2) {
	wcPt2D tmp;
	tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

//交换编码
void swapCodes(GLubyte *c1, GLubyte *c2) {
	GLubyte tmp;
	tmp = *c1;
	*c1 = *c2;
	*c2 = tmp;
}

//核心算法，裁剪算法
void lineClipCohSuth(wcPt2D winMin, wcPt2D winMax, wcPt2D p1, wcPt2D p2) {
	GLubyte code1, code2;
	GLint done = false;
	GLint plotLine = false;
	GLfloat m;
	while (!done) {
		code1 = encode(p1, winMin, winMax);
		code2 = encode(p2, winMin, winMax);
		if (accept(code1, code2)) {
			done = true;
			plotLine = true;
		}
		else if (reject(code1, code2)) {
			done = true;
		}
		else {
			if (inside(code1)) {
				swapPts(&p1, &p2);
				swapCodes(&code1, &code2);
			}
			else {/**/ }//和上一个if匹配，表明if已经结束
			if (p2.x != p1.x) {
				m = (p2.y - p1.y) / (p2.x - p1.x);//GLfloat m
			}
			else {/**/ }
			if (code1 & winLeftBitCode) {
				p1.y += (winMin.x - p1.x) * m;
				p1.x = winMin.x;
			}
			else if (code1 & winRightBitCode) {
				p1.y += (winMax.x - p1.x) * m;
				p1.x = winMax.x;
			}
			else if (code1 & winBottomBitCode) {
				if (p2.x != p1.x) {
					p1.x += (winMin.y - p1.y) / m;
				}
				else {/**/ }
				p1.y = winMin.y;
			}
			else if (code1 & winTopBitCode) {
				if (p2.x != p1.x) {
					p1.x += (winMax.y - p1.y) / m;
				}
				else {/**/ }
				p1.y = winMax.y;
			}
			else {/**/ }
		}//else
	}//while
	if (plotLine) {
		glBegin(GL_LINES);
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
		glEnd();
	}
	else {/**/ }
}

//初始化函数
void init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 600.0, 0.0, 450.0);
}

//键盘交互Enter和Esc
void myKeyBoard(unsigned char key, int x, int y) {
	wcPt2D winMin = { 100,100 }, winMax = { 300,300 }, p1 = { 50,50 }, p2 = { 500,500 };
	if (key == 13) {
		glColor3f(1.0, 0.0, 0.0);
		lineClipCohSuth(winMin, winMax, p1, p2);
	}
	if (key == 27) {
		exit(0);
	}
	glFlush();
}

//显示窗口和未裁剪的直线
void display()
{

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	wcPt2D winMin = { 100,100 }, winMax = { 300,300 };
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(winMin.x, winMin.y);
	glVertex2f(winMax.x, winMin.y);
	glVertex2f(winMin.x, winMin.y);
	glVertex2f(winMin.x, winMax.y);
	glVertex2f(winMax.x, winMin.y);
	glVertex2f(winMax.x, winMax.y);
	glVertex2f(winMin.x, winMax.y);
	glVertex2f(winMax.x, winMax.y);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(50, 50);
	glVertex2f(500, 500);
	glEnd();

	glFlush();
}