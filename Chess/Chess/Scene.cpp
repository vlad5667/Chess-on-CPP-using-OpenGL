#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <gl/glut.h>
#include "Scene.h"
#include "Cube.h"
#include "utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace ChessGame {	
	using namespace GraphUtils;

	GLuint loadTexture(const char *imagePath) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// ������������� ��������� ��������� � ���������� ������� (��� �������� ���������� ������� ��������)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// �������� � ��������� ��������
		int width, height, nrChannels;
		unsigned char* image = stbi_load(imagePath, &width, &height, &nrChannels, STBI_rgb_alpha);
		if (image != nullptr) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(image);
		return texture;
	}

	// collison �������� ��� ����� � ����� �������, outOfBoard �������� ��� ����� �� ��� �����
	bool collision = false;
	int delW = 0, delB = 0;
	bool castlingOccuredW = false;
	bool castlingOccuredB = false;
	int checkW = -1, checkB = -1;
	Scene::Scene() {
		GLuint texture = 0;
		int k = 0;
		// ������ ����� ��� �����
		for (int i = -3; i < N - 3; i++) {
			for (int j = -3; j < M - 3; j++) {
				if (i % 2 != 0) {
					if (j % 2 == 0) {
						shapes.push_back(new Cube(j, 0, i, 1, 0.5, 1, diffBlack, ambiBlack, specBlack, 0.078125));
					} else {
						shapes.push_back(new Cube(j, 0, i, 1, 0.5, 1, diffWhite, ambiWhite, specWhite, 0.6));
					}
				} else {
					if (j % 2 != 0) {
						shapes.push_back(new Cube(j, 0, i, 1, 0.5, 1, diffBlack, ambiBlack, specBlack, 0.078125));
					} else {
						shapes.push_back(new Cube(j, 0, i, 1, 0.5, 1, diffWhite, ambiWhite, specWhite, 0.6));
					}
				}
				//���������� ������ ������ �� �����
				if (i <= -2 || i >= 3) {
					if (i == -2) {
						texture = loadTexture("textures/b_p.png");
						shapes.push_back(pieces[k] = new Pawn('B', j, 0.3, i, 0.8, 0.1, 0.8, diffBlack, ambiBlack, specBlack, 64, 0.4, texture));
					}
					else if (i == 3) {
						texture = loadTexture("textures/w_p.png");
						shapes.push_back(pieces[k] = new Pawn('W', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4, texture));
					}
					else if (i == -3 && (j == -3 || j == 4)) {
						texture = loadTexture("textures/b_r.png");
						shapes.push_back(pieces[k] = new Rook('B', j, 0.3, i, 0.8, 0.1, 0.8, diffBlack, ambiBlack, specBlack, 64, 0.4, texture));
					}
					else if (i == 4 && (j == -3 || j == 4)) {
						texture = loadTexture("textures/w_r.png");
						shapes.push_back(pieces[k] = new Rook('W', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4, texture));
					}
					else if (i == -3 && (j == -2 || j == 3)) {
						texture = loadTexture("textures/b_kn.png");
						shapes.push_back(pieces[k] = new Knight('B', j, 0.3, i, 0.8, 0.1, 0.8, diffBlack, ambiBlack, specBlack, 64, 0.4, texture));
					}
					else if (i == 4 && (j == -2 || j == 3)) {
						texture = loadTexture("textures/w_kn.png");
						shapes.push_back(pieces[k] = new Knight('W', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4, texture));
					}
					else if (i == -3 && (j == -1 || j == 2)) {
						texture = loadTexture("textures/b_b.png");
						shapes.push_back(pieces[k] = new Bishop('B', j, 0.3, i, 0.8, 0.1, 0.8, diffBlack, ambiBlack, specBlack, 64, 0.4, texture));
					}
					else if (i == 4 && (j == -1 || j == 2)) {
						texture = loadTexture("textures/w_b.png");
						shapes.push_back(pieces[k] = new Bishop('W', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4, texture));
					}
					else if (i == -3 && j == 0) {
						texture = loadTexture("textures/b_q.png");
						shapes.push_back(pieces[k] = new Queen('B', j, 0.3, i, 0.8, 0.1, 0.8, diffBlack, ambiBlack, specBlack, 64, 0.4, texture));
					}
					else if (i == 4 && j == 0) {
						texture = loadTexture("textures/w_q.png");
						shapes.push_back(pieces[k] = new Queen('W', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4, texture));
					}
					else if (i == -3 && j == 1) {
						texture = loadTexture("textures/b_k.png");
						shapes.push_back(pieces[k] = new King('B', j, 0.3, i, 0.8, 0.1, 0.8, diffBlack, ambiBlack, specBlack, 64, 0.4, texture));
					}
					else if (i == 4 && j == 1) {
						texture = loadTexture("textures/w_k.png");
						shapes.push_back(pieces[k] = new King('W', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4, texture));
					}
					k++;
				}
			}
		}
		initialize();
	}
	Scene::~Scene() {
		// ��������� �� ������:
		for (unsigned int i = 0; i < shapes.size(); i++) {
			delete shapes[i];
		}
	}

	//������������ ����� �� �����
	void Scene::resetPieces() {
		int k = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (i < 2 || i > 5) {
					fields[i][j] = k;
					pieces[k]->setCoords(j - 3, 0.3, i - 3);
					pieces[k]->setFirstMove(true);
					k++;
				} else {
					fields[i][j] = -1;
				}
			}
		}
	}
	void Scene::initialize() {
		resetPieces();
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				std::cout << fields[i][j] << " ";
			}
			std::cout << "\n";
		}
		collision = false;
		delW = 0, delB = 0;
		castlingOccuredW = false;
		castlingOccuredB = false;
		checkW = -1, checkB = -1;
		whiteMove = true;
		distZ = -13;
		angleX = 0;
		angleY = 60;
	}
	// ���������� ������ � ������� ������� �� ����
	bool Scene::movePiece(int currentZ, int currentX, int zTo, int xTo) {
		// �������� ��������� ����������:
		if (xTo < 0 || zTo < 0 || xTo >= N || zTo >= M) {
			return false;
		}
		if (currentX == xTo && currentZ == zTo) {
			return false;
		}
		if (fields[zTo][xTo] != -1 && (currentX != xTo || currentZ != zTo)) {
			collision = true;
			return false;
		}

		// ����������:
		if ((whiteMove && pieces[fields[currentZ][currentX]]->getColor() == 'W') || (!whiteMove && pieces[fields[currentZ][currentX]]->getColor() == 'B')) {
			pieces[fields[currentZ][currentX]]->movePieceToPosition(fields, zTo, xTo, currentZ, currentX);
		} else {
			return false;
		}
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				std::cout << fields[i][j] << " ";
			}
			std::cout << std::endl;
		}
		return true;
	}
	//ϳ������ ������
	void Scene::upPiece(int x, int z) {
		pieces[fields[z][x]]->setYCenter(1);
	}
	//��������� ��� �����
	void Scene::downAllPieces() {
		for (int i = 0; i < 4 * N; i++)
		{
			pieces[i]->setYCenter(0.3);
		}
	}
	// ����� �������, ����������� �� ������� ������� ����:
	bool Scene::findNearest(int x, int y, int& x1, int& z1) {
		int viewport[4];
		int iMin = -1, jMin = -1;
		double mvMatrix[16], projMatrix[16];
		double minDist = 2000;

		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {

				// ����� x, y, z ��������� �������:
				double wx = i - 3;
				double wy = 0.3;
				double wz = j - 3;

				// ���������� ����� viewport �������� ������� ���������:
				glGetIntegerv(GL_VIEWPORT, viewport);

				// ���������� ������ ��������� ���������:
				glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
				glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

				// ����� x, y, z ����������, �� �����������:
				double dx, dy, dz;

				// �������� ���������� �����, �� ��� ������������ �������� ��������:
				gluProject(wx, wy, wz, mvMatrix, projMatrix, viewport, &dx, &dy, &dz);
				dy = viewport[3] - dy - 1; // dy ��������� ������������
				double d = (x - dx) * (x - dx) + (y - dy) * (y - dy); // ������� ������
				if (d < minDist) { // ������� ������� ��������
					minDist = d;
					iMin = i;
					jMin = j;
				}
			}
		}
		if (minDist < 1000) { // ������� ���������� ��������
			z1 = jMin;
			x1 = iMin;
			return true;
		}
		else {
			z1 = -1;
			x1 = -1;
			return false;
		}
	}
	// ���������� ��䳿, ���'����� � �������������� ����
	void Scene::on_paint() {
		char text[128];
		if (whiteMove) {
			strcpy_s(text, 128, "White to move");
		} else {
			strcpy_s(text, 128, "Black to move");
		}
		//sprintf(text, "F2 - Restart game   Esc - Exit              Time: %d sec.", time);
		glViewport(0, 0, width, height);

		// ���������� ��������� �������� � ������� �����:
		float lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // ���� �������� ��������� 
		float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // ���� ��������� ��������� 
		float lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };// ���� ������������ �����������
		float lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f };// ������������ ������� �����

														   // ������������ ��������� ������� �����:
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

		
		glClearColor(0, 0.5, 0.5, 0);

		// ������� ������:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// ��� ����������� ������, ����� ��������������� ������������ ��������:
		glOrtho(0, 1, 0, 1, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(1, 1, 0); // ������ �����
		drawString(GLUT_BITMAP_TIMES_ROMAN_24, text, 0.01, 0.95);
		glPopMatrix();

		// �������� ����� ������ � �������� ��������:
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// ������ ������� ����� �������� � ����������� ������ ���������, 
		// 60 - ��� �������� � �������� �� �� �,
		// width/height - ��� �������� ������ �� x,
		// 1 � 100 - ������� �� ����������� �� ������ �������� �� ������:
		gluPerspective(60, width / height, 1, 100);

		// �������� ����� ������ � ������� ��������:
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-0.4, 0, distZ);	// ������ � ������� ��������� ��������� �� distZ, 

		glRotatef(angleX, 0.0f, 1.0f, 0.0f);  // ���� ���������� �� �� Oy
		glRotatef(angleY, 1.0f, 0.0f, 0.0f);  // ���� ���������� �� �� Ox 
		
		glEnable(GL_DEPTH_TEST);	// �������� ����� ������� (��� �������� ��������� ������ ����������)
									// �������� ����� ��� ��������� ���������:
		glEnable(GL_LIGHTING);

		// ������ ������� ����� � 0 (�� ���� ���� �� 8), ����� ���� ������ � "����":
		glEnable(GL_LIGHT0);

		// ������� �� ������:
		for (int i = 0; i < shapes.size(); i++) {
			shapes[i]->draw();
		}
		// �������� ���, �� ��������:
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glFlush();
		// �������� ����:
		glutSwapBuffers(); // ����������� ������
	}
	// ���������� ��䳿, ���'����� � ����� ������ ���� 
	void Scene::on_size(int width, int height)
	{
		this->width = width;
		if (height == 0)
			height = 1;
		this->height = height;
	}
	// ���������� ����, ���'������ � ����������� ������ ����
	void Scene::on_mouse(int button, int state, int x, int y)
	{
		// �������� ������ ���������� ����:
		mouseX = x;
		mouseY = y;
		if (state == GLUT_UP) { // ������ ������
			// ³����� ������ ���� ����
			if (button == 0) {
				//std::cout << zStart << " " << currentZ << " " << xStart << " " << currentX << " " << mouseZCell << " " << mouseXCell  << "\n";
				downAllPieces();
				findNearest(x, y, mouseXCell, mouseZCell);
				Piece* p = pieces[fields[currentZ][currentX]]; // �������� �� ������� ������
				bool hit = false; // �����, �� ��������, ������ ���� �� ������ ����� �� �
				bool castlingOccured = false; // �����, �� ��������, ������� ���������� �� �
				// ���� ������ ���������� ������������ �� ��� �����, ��������� �� �������
				if (mouseXCell == -1 || mouseZCell == -1) {
					std::cout << "mouseXCell " << mouseXCell << " mouseZCell " << mouseZCell << "\n";
					std::cout << "xStart " << xStart << " zStart " << zStart << " currentX " << currentX << " currentZ " << currentZ << "\n";
					if (zStart != -1 && xStart != -1 && currentZ != -1 && currentX != -1 && pieces[fields[currentZ][currentX]] == p) {
						p->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
					}
					this->button = -1;
					return;
					// ���� ���� ������ ����� ������ �� ������������ (�� �� ��� �����), �� �������� �������� ��������
				} else if (mouseZCell != -1 && mouseXCell != -1) {
					if (fields[currentZ][currentX] != -1 && (zStart != mouseZCell || xStart != mouseXCell)) {
						if (checkW != -1 && whiteMove) {
							if (collision && fields[mouseZCell][mouseXCell] != -1 && pieces[fields[mouseZCell][mouseXCell]] != p) {
								if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX)) {
									if (pieces[checkW] == pieces[fields[mouseZCell][mouseXCell]]) {
										pieces[fields[mouseZCell][mouseXCell]]->setXCenter(-5 - delW++);
										pieces[fields[mouseZCell][mouseXCell]]->setZCenter(4);
										p->movePieceToPosition(fields, mouseZCell, mouseXCell, currentZ, currentX);
										checkW = -1;
										whiteMove = !whiteMove;
									}
									else {
										p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
									}
								}
							}
							else if (p->correctMove(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
								if (pieces[28]->isCheckOccured(pieces, fields)) {
									p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
								}
								else {
									checkW = -1;
									whiteMove = !whiteMove;
								}
							}
						}
						else if (checkB != -1 && !whiteMove) {
							if (collision && fields[mouseZCell][mouseXCell] != -1 && pieces[fields[mouseZCell][mouseXCell]] != p) {
								if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX)) {
									if (pieces[checkB] == pieces[fields[mouseZCell][mouseXCell]]) {
										pieces[fields[mouseZCell][mouseXCell]]->setXCenter(6 + delB++);
										pieces[fields[mouseZCell][mouseXCell]]->setZCenter(-3);
										p->movePieceToPosition(fields, mouseZCell, mouseXCell, currentZ, currentX);
										checkB = -1;
										whiteMove = !whiteMove;
									}
									else {
										p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
									}
								}
							}
							else if (p->correctMove(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
								if (pieces[4]->isCheckOccured(pieces, fields)) {
									p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
								}
								else {
									checkB = -1;
									whiteMove = !whiteMove;
								}
							}
						}
						// �������� �� ����� � ����� �������
						else if (collision && fields[mouseZCell][mouseXCell] != -1 && pieces[fields[mouseZCell][mouseXCell]] != p) {
							if (p->getColor() == 'W' && whiteMove) {
								if (!castlingOccuredW && typeid(*p) == typeid(King)) {
										castlingOccuredW = static_cast<King*>(p)->isCastlingPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX);
										castlingOccured = castlingOccuredW;
										hit = castlingOccuredW;
								}
								// ���� ���� ������� ������ ����, ����� ��� ���� � ������, ��� ������ ������, �����, �� ���������� ���� �� �����
								if (pieces[fields[mouseZCell][mouseXCell]]->getColor() == 'B') {
									if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX)) {
										pieces[fields[mouseZCell][mouseXCell]]->setXCenter(-5 - delW++);
										pieces[fields[mouseZCell][mouseXCell]]->setZCenter(4);
										p->movePieceToPosition(fields, mouseZCell, mouseXCell, currentZ, currentX);
									}
								}
							}
							else if (p->getColor() == 'B' && !whiteMove) {
								if (!castlingOccuredB && typeid(*p) == typeid(King)) {
									castlingOccuredB = static_cast<King*>(p)->isCastlingPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX);
									castlingOccured = castlingOccuredB;
									hit = castlingOccuredB;
								}
								// ���� ���� ������� ������ ������, ����� ��� ������ � ������, ��� ������ ������, ���, �� ���������� ���� �� ���
								if (pieces[fields[mouseZCell][mouseXCell]]->getColor() == 'W') {
									if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX)) {
										pieces[fields[mouseZCell][mouseXCell]]->setXCenter(6 + delB++);
										pieces[fields[mouseZCell][mouseXCell]]->setZCenter(-3);
										p->movePieceToPosition(fields, mouseZCell, mouseXCell, currentZ, currentX);
									}
								}
							}
							// ���������� ����������, ���� ���� ��� ���������� �� �������
							if (!hit && (zStart != mouseZCell || xStart != mouseXCell)) {
								p->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
							}
							else if (castlingOccured || hit) {
								whiteMove = !whiteMove;
							}
							collision = false;
						}
						else { // �������� ���������� ����������
							if ((p->getColor() == 'W' && whiteMove) || (p->getColor() == 'B' && !whiteMove)) {
								if (p->correctMove(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
									whiteMove = !whiteMove;
									checkB = pieces[4]->isCheckOccured(pieces, fields);
									checkW = pieces[28]->isCheckOccured(pieces, fields);
									std::cout << "CheckB " << checkB << " CheckW " << checkW << std::endl;
								}
							}
						}
						if (whiteMove) {
							angleX = 0;
							angleY = angleY < 0 ? -angleY : angleY;
						} 
						else {
							angleX = 180;
							angleY = angleY > 0 ? -angleY : angleY;
						}
					}
				}
			}
			this->button = -1;  // ���� ������ �� ���������
			return;
		}
		this->button = button;  // �������� ���������� ��� ������
		if (findNearest(x, y, currentX, currentZ)) {
			if (button == 0 && fields[currentZ][currentX] != -1) {
				upPiece(currentX, currentZ);
				xStart = currentX;
				zStart = currentZ;
			}
		}
	}
	void Scene::on_motion(int x, int y) {
		switch (button) {
		case 0:
			if (findNearest(x, y, xTo, zTo) && fields[currentZ][currentX] != -1) {
				if (movePiece(currentZ, currentX, zTo, xTo)) {
					currentX = xTo;
					currentZ = zTo;
				}
			}
			break;
		case 2: // ����� ������ - ��������� �����
			angleX += x - mouseX;
			angleY += y - mouseY;
			mouseX = x;
			mouseY = y;
			break;
		}
	}

	void Scene::on_special(int key, int x, int y) {
		switch (key) {
		case GLUT_KEY_UP:   // ����������
			if (distZ > -8) {
				break;
			}
			distZ += 0.3;
			break;
		case GLUT_KEY_DOWN: // ���������
			if (distZ <= -17) {
				break;
			}
			distZ -= 0.3;
			break;
		case GLUT_KEY_F2:   // ���� ���
			initialize();
			break;
		}
	}

	int tick = 0; // ��������, �������� ����� ��������� ���� 25 ��

				  // ���������� ��䳿 �� �������
	void Scene::on_timer(int value)
	{
		tick++;
		if (tick >= 40) // ���������� �������� �������
		{
			//if (!finish)// ������� �����������, ���� ��� �� ��������
			//{
			//	time++;
			//}
			tick = 0;   // ������� ��������
		}
		on_paint();     // ��������� ������������� ����
	}
}