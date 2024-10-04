#include <iostream>
#include <gl/glew.h> 
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <vector>
#include <math.h>
#include <windows.h>

#define RECTINITSIZE 0.05
#define RECTMOVESPEED 0.005
#define RECTREDUCTION 0.001
int rectcount = 0;

GLfloat qx[4] = { 0.5f, 0.0f, 0.0f, 0.5f };
GLfloat qy[4] = { 0.5f, 0.5f, 0.0f, 0.0f };
GLfloat sx_hv[4] = { 0.0f, -1.0f, 0.0f, 1.0f };
GLfloat sy_hv[4] = { 1.0f, 0.0f, -1.0f, 0.0f };
GLfloat sx_di[4] = { 1.0f, -1.0f, -1.0f, 1.0f };
GLfloat sy_di[4] = { 1.0f, 1.0f, -1.0f, -1.0f };

struct rect {
	GLfloat x1;
	GLfloat y1;
	GLfloat x2;
	GLfloat y2;

	GLclampf r;
	GLclampf g;
	GLclampf b;
	GLclampf a = 0;

	GLfloat sx = 0;
	GLfloat sy = 0;

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

void rect_divide(int cmd);
int rect_move_hv();
void rect_move_di();
void rect_move_oneside();
void rect_move_octa();

void timer(int value);

std::vector<struct rect> rectangles;
std::vector<struct rect> rectangles_move;
struct rect divide_rect;
int divide_rect_index = -1;
int cmd = 1;
int timer_toggle = 0;

void main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("CG_4w_opengl_6");

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
	if (!rectangles_move.empty()) rectangles_move.clear();
	int maxrectcount = rand() % 5 + 1 + 5;

	rectcount = 0;
	divide_rect_index = -1;
	cmd = 1;
	timer_toggle = 0;

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

	if (!rectangles_move.empty()) {
		for (int i = 0; i < rectangles_move.size(); i++) {
			glColor3f(rectangles_move[i].r, rectangles_move[i].g, rectangles_move[i].b);
			glRectf(rectangles_move[i].x1, rectangles_move[i].y1, rectangles_move[i].x2, rectangles_move[i].y2);

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
		if (divide_rect_index != -1) {
			std::cout << divide_rect_index << std::endl;

			rect_divide(cmd);
			if (divide_rect_index == rectcount) {
				rectangles.pop_back();
			}
			else {
				rectangles.erase(rectangles.begin() + divide_rect_index);
			}
			rectcount--;
			if (timer_toggle == 0) {
				glutTimerFunc(10, timer, 1);
				timer_toggle = 1;
			}
		}
		glutPostRedisplay();
	}
	else {
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
	case '1':
		cmd = 1;
		break;
	case '2':
		cmd = 2;
		break;
	case '3':
		cmd = 3;
		break;
	case '4':
		cmd = 4;
		break;
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


void rect_divide(int cmd) {
	switch (cmd) {
	case 1:
		rect_move_hv();
		break;

	case 2:
		rect_move_di();
		break;

	case 3:
		rect_move_oneside();
		break;

	case 4:
		rect_move_octa();
		break;
	}
}

int rect_move_hv() {
	GLfloat width = rectangles[divide_rect_index].x2 - rectangles[divide_rect_index].x1;
	GLfloat height = rectangles[divide_rect_index].y2 - rectangles[divide_rect_index].y1;
	for (int i = 0; i < 4; i++) {
		struct rect rect_new;
		rect_new.x1 = rectangles[divide_rect_index].x1 + width * qx[i];
		rect_new.x2 = rect_new.x1 + width / 2;

		rect_new.y1 = rectangles[divide_rect_index].y1 + width * qy[i];
		rect_new.y2 = rect_new.y1 + width / 2;

		rect_new.r = rectangles[divide_rect_index].r;
		rect_new.g = rectangles[divide_rect_index].g;
		rect_new.b = rectangles[divide_rect_index].b;

		rect_new.sx = RECTMOVESPEED * sx_hv[i];
		rect_new.sy = RECTMOVESPEED * sy_hv[i];
		rectangles_move.push_back(rect_new);
	}
	return rectangles_move.size() - 4;
}

void rect_move_di() {
	GLfloat width = rectangles[divide_rect_index].x2 - rectangles[divide_rect_index].x1;
	GLfloat height = rectangles[divide_rect_index].y2 - rectangles[divide_rect_index].y1;
	for (int i = 0; i < 4; i++) {
		struct rect rect_new;
		rect_new.x1 = rectangles[divide_rect_index].x1 + width * qx[i];
		rect_new.x2 = rect_new.x1 + width / 2;

		rect_new.y1 = rectangles[divide_rect_index].y1 + width * qy[i];
		rect_new.y2 = rect_new.y1 + width / 2;

		rect_new.r = rectangles[divide_rect_index].r;
		rect_new.g = rectangles[divide_rect_index].g;
		rect_new.b = rectangles[divide_rect_index].b;

		rect_new.sx = RECTMOVESPEED * sx_di[i];
		rect_new.sy = RECTMOVESPEED * sy_di[i];
		rectangles_move.push_back(rect_new);
	}
}

void rect_move_oneside() {
	GLfloat width = rectangles[divide_rect_index].x2 - rectangles[divide_rect_index].x1;
	GLfloat height = rectangles[divide_rect_index].y2 - rectangles[divide_rect_index].y1;
	int dir = rand() % 4;

	for (int i = 0; i < 4; i++) {
		struct rect rect_new;
		rect_new.x1 = rectangles[divide_rect_index].x1 + width * qx[i] + sx_di[i] * 0.005;
		rect_new.x2 = rect_new.x1 + width / 2;

		rect_new.y1 = rectangles[divide_rect_index].y1 + width * qy[i] + sy_di[i] * 0.005;
		rect_new.y2 = rect_new.y1 + width / 2;

		rect_new.r = rectangles[divide_rect_index].r;
		rect_new.g = rectangles[divide_rect_index].g;
		rect_new.b = rectangles[divide_rect_index].b;

		rect_new.sx = RECTMOVESPEED * sx_di[dir];
		rect_new.sy = RECTMOVESPEED * sy_di[dir];
		rectangles_move.push_back(rect_new);
	}
}

void rect_move_octa() {

	rect_move_di();
	int hv_index = rect_move_hv();
	GLfloat width = rectangles_move[hv_index].x2 - rectangles_move[hv_index].x1;
	GLfloat height = rectangles_move[hv_index].y2 - rectangles_move[hv_index].y1;
	
	GLfloat ox = rectangles_move[hv_index].x1;
	GLfloat oy = rectangles_move[hv_index].y1;

	for (int i = hv_index; i < hv_index + 4; i++) {
		rectangles_move[i].x1 = ox - width / 2 + sx_hv[i - hv_index] * width / 2;
		rectangles_move[i].x2 = ox + width / 2 + sx_hv[i - hv_index] * width / 2;
		rectangles_move[i].y1 = oy - height / 2 + sy_hv[i - hv_index] * height / 2;
		rectangles_move[i].y2 = oy + height / 2 + sy_hv[i - hv_index] * height / 2;
	}
}

void timer(int value) {
	for (int i = 0; i < rectangles_move.size(); i++) {
		std::cout << rectangles_move[i].sx << std::endl;
		rectangles_move[i].x1 += rectangles_move[i].sx + RECTREDUCTION;
		rectangles_move[i].x2 += rectangles_move[i].sx - RECTREDUCTION;
		rectangles_move[i].y1 += rectangles_move[i].sy + RECTREDUCTION;
		rectangles_move[i].y2 += rectangles_move[i].sy - RECTREDUCTION;

		if (rectangles_move[i].x2 - rectangles_move[i].x1 < 0.001) {
			if (i == rectangles_move.size()) {
				rectangles_move.pop_back();
			}
			else {
				rectangles_move.erase(rectangles_move.begin() + i);
			}
		}
	}

	glutPostRedisplay();
	if (!rectangles_move.empty()) {
		glutTimerFunc(10, timer, 1);
	}
	else {
		timer_toggle = 0;
	}
}