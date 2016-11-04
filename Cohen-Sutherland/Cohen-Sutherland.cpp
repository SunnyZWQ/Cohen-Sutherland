// Cohen-Sutherland.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<windows.h>
#include"GL/glu.h"
#include<GL/gl.h>
#include<GL/glut.h>
using namespace std;

//������
class wcPt2D {
public:
	float x, y;
};
GLubyte encode(wcPt2D pt, wcPt2D winMin, wcPt2D winMax);
void swapPts(wcPt2D *p1, wcPt2D * p2);
void swapCodes(GLubyte * c1, GLubyte *c2);
void lineClipCohSuth(wcPt2D winMin, wcPt2D winMax, wcPt2D p1, wcPt2D p2);
//����
const GLint winLeftBitCode = 0x1;
const GLint winRightBitCode = 0x2;
const GLint winBottomBitCode = 0x4;
const GLint winTopBitCode = 0x8;
//�ж��Ƿ��ڴ����ڣ���������߲��ִ�����
inline GLint inside(GLint code);
inline GLint reject(GLint code1, GLint code2);
inline GLint accept(GLint code1, GLint code2);

//��������
//�˵�ı���
GLubyte encode(wcPt2D pt, wcPt2D winMin, wcPt2D winMax);
//������
void swapPts(wcPt2D *p1, wcPt2D *p2);
//��������
void swapCodes(GLubyte *c1, GLubyte *c2);
//���ģ��ü��㷨
void lineClipCohSuth(wcPt2D winMin, wcPt2D winMax, wcPt2D p1, wcPt2D p2);
//��ʼ������
void init(void);
//���̽���Enter��Esc
void myKeyBoard(unsigned char key, int x, int y);
//��ʾ���ں�δ�ü���ֱ��
void display();

//������
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
//�ж��Ƿ��ڴ����ڣ���������߲��ִ�����
inline GLint inside(GLint code) {
	return GLint(!code);
}
inline GLint reject(GLint code1, GLint code2) {
	return GLint(code1 & code2);
}
inline GLint accept(GLint code1, GLint code2) {
	return GLint(!(code1 | code2));
}

//�˵�ı���
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
	return (code);//�˴�����Ӧ��Ϊ��ǿ�����ص��Ǳ��롣
/*PS�����еĴ���ķ��ɶ��Խϲ��Ϊelse��if֮��ʹ�û������������뿴��if-else֮���ƥ��Ƚ�����
����if��ֻ��һ�д���������ֱ�ӻ���,����ģ����Ӧ�ü��ϣ���ʹ�ý��޸������ԣ������������������.
*/
}

//������
void swapPts(wcPt2D *p1, wcPt2D *p2) {
	wcPt2D tmp;
	tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

//��������
void swapCodes(GLubyte *c1, GLubyte *c2) {
	GLubyte tmp;
	tmp = *c1;
	*c1 = *c2;
	*c2 = tmp;
}

//�����㷨���ü��㷨
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
			else {/**/ }//����һ��ifƥ�䣬����if�Ѿ�����
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

//��ʼ������
void init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 600.0, 0.0, 450.0);
}

//���̽���Enter��Esc
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

//��ʾ���ں�δ�ü���ֱ��
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