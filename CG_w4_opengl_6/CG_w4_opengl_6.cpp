#include <iostream>
#include <gl/glew.h> 
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <vector>
#include <math.h>
#include <windows.h>

#define RECTINITSIZE 0.05

int rectcount = 0;
int erasing = 0;

struct rect {
	GLfloat x1;
	GLfloat y1;
	GLfloat x2;
	GLfloat y2;

	GLclampf r;
	GLclampf g;
	GLclampf b;
	GLclampf a = 0;


	rect() {

		GLfloat rectsize = (GLfloat)(rand() % 100) / 100 / 7 + 0.2;
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		int viewport_width = viewport[2];
		int viewport_height = viewport[3];
		x1 = (float)(rand() % viewport_width) / viewport_width * 2 - 1.0f;
		x2 = x1 + rectsize;
		y1 = (float)(rand() % viewport_height) / viewport_height * 2 - 1.0f;
		y2 = y1 + rectsize;

		r = (float)(rand() % 256) / 256;
		g = (float)(rand() % 256) / 256;
		b = (float)(rand() % 256) / 256;
	}

	rect(GLfloat ix1, GLfloat iy1, GLfloat ix2, GLfloat iy2) {
		x1 = ix1;
		x2 = ix2;
		y1 = iy1;
		y2 = iy2;

		r = (float)(rand() % 256) / 256;
		g = (float)(rand() % 256) / 256;
		b = (float)(rand() % 256) / 256;
	}
}rect;

void init();
void draw_rect(int index);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
int rect_find_top(GLfloat* input_pos);
void clamp_pos(GLfloat* input_pos);
void draw_rect(int index);


std::vector<struct rect> rectangles;
struct rect divide_rect;
int divide_rect_index = -1;

void main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("CG_4w_opengl_5");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}
	else
		glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	init();
	glutMainLoop();
}


void init() {
	if (!rectangles.empty()) rectangles.clear();
	int maxrectcount = rand() % 5 + 1 + 5;

	rectcount = 0;

	while (rectcount < maxrectcount) {
		struct rect rect_new;
		rectcount++;
		rectangles.push_back(rect_new);
	}
}


GLvoid drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (!rectangles.empty()) {
		for (int i = 0; i < rectangles.size(); i++) {
			draw_rect(i);
		}
	}
	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y)
{

	GLfloat input_pos[2] = { x, y };
	clamp_pos(input_pos);
	if (state == GLUT_DOWN) {
		divide_rect_index = rect_find_top(input_pos);
		divide_rect = rectangles[divide_rect_index];
		std::cout << divide_rect_index << std::endl;
		glutPostRedisplay();
	}
	else {
		erasing = 0;
	}

}

void clamp_pos(GLfloat* input_pos) {
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	int viewport_width = viewport[2];
	int viewport_height = viewport[3];
	input_pos[0] = (input_pos[0] / viewport_width) * 2 - 1.0f;
	input_pos[1] = -1 * ((input_pos[1] / viewport_height) * 2 - 1.0f);
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'r':
		init();
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void draw_rect(int index) {
	glColor3f(rectangles[index].r, rectangles[index].g, rectangles[index].b);
	glRectf(rectangles[index].x1, rectangles[index].y1, rectangles[index].x2, rectangles[index].y2);
}


int rect_find_top(GLfloat* input_pos) {
	if (rectangles.empty()) return -1;
	for (int i = rectangles.size() - 1; i >= 0; i--) {
		if (rectangles[i].x1 <= input_pos[0] && input_pos[0] <= rectangles[i].x2) {
			if (rectangles[i].y1 <= input_pos[1] && input_pos[1] <= rectangles[i].y2) {
				return i;

			}
		}
	}
}


