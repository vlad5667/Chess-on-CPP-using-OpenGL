#include <gl/glut.h>
#include "Scene.h"

using ChessGame::Scene;

Scene *scene;

void on_paint() {
	scene->on_paint();
}

void on_size(int width, int height) {
	// ��������� �������� ������� ����� Scene:
	scene->on_size(width, height);
}

void on_mouse(int button, int state, int x, int y) {
	// ��������� �������� ������� ����� Scene:
	scene->on_mouse(button, state, x, y);
}

void on_motion(int x, int y) {
	// ��������� �������� ������� ����� Scene:
	scene->on_motion(x, y);
}

void on_special(int key, int x, int y) {
	// ��������� �������� ������� ����� Scene:
	scene->on_special(key, x, y);
}

void on_keyboard(unsigned char key, int x, int y) {
	// ������� ���� �� ���������:
	scene->on_keyboard(key, x, y);
}

void on_timer(int value) {
	// ������� ��䳿 �� �������
	scene->on_timer(value);
	glutTimerFunc(25, on_timer, 0); // �a 25 �� ����������� �� �������
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("Chess");
	glutDisplayFunc(on_paint);
	glutReshapeFunc(on_size);      // �������� ������� ������� ���� ������ ����
	glutMotionFunc(on_motion);     // �������� �������, ��� ������� �� ���������� ���� � ���������� �������
	glutMouseFunc(on_mouse);       // �������� �������, ��� ������� �� ���������� �� ������ ����
	glutKeyboardFunc(on_keyboard); // �������� �������, ��� ������� �� ���������� �����
	glutSpecialFunc(on_special);   // �������� �������, ��� ������� �� ���������� ����������� �����
	glutTimerFunc(25, on_timer, 0);
	scene = new Scene();
	glutMainLoop();
	delete scene;
	return(0);
}