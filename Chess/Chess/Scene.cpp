#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <gl/glut.h>
#include <string>
#include <ctime>
#include "Scene.h"
#include "Cube.h"
#include "utils.h"

namespace ChessGame {
	using namespace GraphUtils;
	std::string firstPlayerName = "", secondPlayerName = ""; // ����, �� ��������� ����� �������
	std::string whitePlayer, blackPlayer;

	bool inputFirstPlayerNameMode = true; // �����, �� �������� ��� ����� �������� ���� ������� ������
	bool inputSecondPlayerNameMode = false; // �����, �� �������� ��� ����� �������� ���� ������� ������
	bool selectDifficultyMode = false; // �����, �� �������� ��� ����� ������ ���������
	bool gameMode = false; // �����, �� �������� ��� ����� ���

	int difficulty; // �����, �� ����� �� ��������� ���

	bool collision = false; // �����, �� �������� ��� ����� � ����� �������
	int delW = 0, delB = 0; // ����, �� ������� ���������� � ��� ������� �����
	bool castlingOccuredW = false; // �����, �� ��������, �� ������� ���������� ����
	bool castlingOccuredB = false; // �����, �� ��������, �� ������� ���������� ������
	int checkW = -1, checkB = -1; // ����, �� �����������, ������ ��� �� �
	bool mateOccuredW = false, mateOccuredB = false; // ����, �� �����������, ������ ��� �� �
	int prevPieceId = 0; // �����, �� ������ ������������� ������, �� ������ �� ����� (������� ��� ������ �� ������)
	int promotionMode = 0; // �����, �� �������� ��� ���� ������ ��� ������������ �����

	Scene::Scene() {
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
					pieces[k]->setBeaten(false);
					k++;
				}
				else {
					fields[i][j] = -1;
				}
			}
		}
	}
	void Scene::initialize() {
		if (rand() % 2 == 0) {
			whitePlayer = firstPlayerName;
			blackPlayer = secondPlayerName;
		}
		else {
			whitePlayer = secondPlayerName;
			blackPlayer = firstPlayerName;
		}
		shapes.clear();
		int k = 0;
		// ������ ����� ��� �����
		for (int i = -3; i < N - 3; i++) {
			for (int j = -3; j < M - 3; j++) {
				if (i % 2 != 0) {
					if (j % 2 == 0) {
						shapes.push_back(new Cube(j, 0, i, 1, 0.5, 1, diffBlack, ambiBlack, specBlack, 0.078125));
					}
					else {
						shapes.push_back(new Cube(j, 0, i, 1, 0.5, 1, diffWhite, ambiWhite, specWhite, 0.6));
					}
				}
				else {
					if (j % 2 != 0) {
						shapes.push_back(new Cube(j, 0, i, 1, 0.5, 1, diffBlack, ambiBlack, specBlack, 0.078125));
					}
					else {
						shapes.push_back(new Cube(j, 0, i, 1, 0.5, 1, diffWhite, ambiWhite, specWhite, 0.6));
					}
				}
			}
		}
		// ������ ������ �� �����
		for (int i = -3; i < N - 3; i++) {
			for (int j = -3; j < M - 3; j++) {
				if (i <= -2 || i >= 3) {
					if (i == -2) {
						shapes.push_back(pieces[k] = new Pawn(k, 'B', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4));
						pieces[k]->loadTexture("textures/b_p.png");
					}
					else if (i == 3) {
						shapes.push_back(pieces[k] = new Pawn(k, 'W', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4));
						pieces[k]->loadTexture("textures/w_p.png");
					}
					else if (i == -3 && (j == -3 || j == 4)) {
						shapes.push_back(pieces[k] = new Rook(k, 'B', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4));
						pieces[k]->loadTexture("textures/b_r.png");
					}
					else if (i == 4 && (j == -3 || j == 4)) {
						shapes.push_back(pieces[k] = new Rook(k, 'W', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4));
						pieces[k]->loadTexture("textures/w_r.png");
					}
					else if (i == -3 && (j == -2 || j == 3)) {
						shapes.push_back(pieces[k] = new Knight(k, 'B', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4));
						pieces[k]->loadTexture("textures/b_kn.png");
					}
					else if (i == 4 && (j == -2 || j == 3)) {
						shapes.push_back(pieces[k] = new Knight(k, 'W', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4));
						pieces[k]->loadTexture("textures/w_kn.png");
					}
					else if (i == -3 && (j == -1 || j == 2)) {
						shapes.push_back(pieces[k] = new Bishop(k, 'B', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4));
						pieces[k]->loadTexture("textures/b_b.png");
					}
					else if (i == 4 && (j == -1 || j == 2)) {
						shapes.push_back(pieces[k] = new Bishop(k, 'W', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4));
						pieces[k]->loadTexture("textures/w_b.png");
					}
					else if (i == -3 && j == 0) {
						shapes.push_back(pieces[k] = new Queen(k, 'B', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4));
						pieces[k]->loadTexture("textures/b_q.png");
					}
					else if (i == 4 && j == 0) {
						shapes.push_back(pieces[k] = new Queen(k, 'W', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4));
						pieces[k]->loadTexture("textures/w_q.png");
					}
					else if (i == -3 && j == 1) {
						shapes.push_back(pieces[k] = new King(k, 'B', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4));
						pieces[k]->loadTexture("textures/b_k.png");
					}
					else if (i == 4 && j == 1) {
						shapes.push_back(pieces[k] = new King(k, 'W', j, 0.3, i, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4));
						pieces[k]->loadTexture("textures/w_k.png");
					}
					k++;
				}
			}
		}
		resetPieces();
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				std::cout << fields[i][j] << " ";
			}
			std::cout << "\n";
		}
		// ������������ ������� ��� ������ �� �������������
		collision = false;
		delW = 0, delB = 0;
		castlingOccuredW = false;
		castlingOccuredB = false;
		checkW = -1, checkB = -1;
		mateOccuredW = false;
		mateOccuredB = false;
		prevPieceId = 0;
		promotionMode = 0;
		whiteMove = true;
		switch (difficulty) {
		case 2:
			firstPlayerTime = secondPlayerTime = 120 * 60;
			break;
		case 3:
			firstPlayerTime = secondPlayerTime = 60 * 60;
		}
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
		if (currentX < 0 || currentZ < 0 || currentX >= N || currentZ >= M) {
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
	// ����� �������, ��������� �� ������� ������� ����:
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
		std::string interfaceNavigation, state, promotion;
		if (promotionMode > 1) {
			int id = prevPieceId;
			float x = pieces[id]->getXCenter(), z = pieces[id]->getZCenter();
			if (pieces[id]->getColor() == 'W') {
				GLuint texture;
				switch (promotionMode) {
				case 2:
					pieces[id] = new Queen(id, 'W', x, 0.3, z, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4);
					pieces[id]->loadTexture("textures/w_q.png");
					break;
				case 3:
					pieces[id] = new Bishop(id, 'W', x, 0.3, z, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4);
					pieces[id]->loadTexture("textures/w_b.png");
					break;
				case 4:
					pieces[id] = new Knight(id, 'W', x, 0.3, z, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4);
					pieces[id]->loadTexture("textures/w_kn.png");
					break;
				case 5:
					pieces[id] = new Rook(id, 'W', x, 0.3, z, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4);
					pieces[id]->loadTexture("textures/w_r.png");
					break;
				}
			}
			else {
				GLuint texture;
				switch (promotionMode) {
				case 2:
					pieces[id] = new Queen(id, 'B', x, 0.3, z, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4);
					pieces[id]->loadTexture("textures/b_q.png");
					break;
				case 3:
					pieces[id] = new Bishop(id, 'B', x, 0.3, z, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4);
					pieces[id]->loadTexture("textures/b_b.png");
					break;
				case 4:
					pieces[id] = new Knight(id, 'B', x, 0.3, z, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4);
					pieces[id]->loadTexture("textures/b_kn.png");
					break;
				case 5:
					pieces[id] = new Rook(id, 'B', x, 0.3, z, 0.8, 0.1, 0.8, diffWhite, ambiWhite, specWhite, 64, 0.4);
					pieces[id]->loadTexture("textures/b_r.png");
					break;
				}
			}
			pieces[id]->setFirstMove(false);
			shapes[64 + id] = pieces[id];
			promotionMode = 0;
		}
		if (promotionMode) {
			promotion = "Select the promotion piece (q - Queen, b - Bishop, k - Knight, r - Rook)";
		}
		else {
			promotion = "";
		}
		if (whiteMove) {
			state = "White to move";
		} 
		else {
			state = "Black to move";
		}
		if (checkW != -1) {
			state = "Check white";
		}
		else if (checkB != -1) {
			state = "Check black";
		}
		if (mateOccuredW) {
			state = "Mate white";
		}
		else if (mateOccuredB) {
			state = "Mate black";
		}
		interfaceNavigation = "F2 - restart game";

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
		if (inputFirstPlayerNameMode) {
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, "Input first player name", 0.45, 0.95);
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, firstPlayerName.c_str(), 0.45, 0.88);
		}
		if (inputSecondPlayerNameMode) {
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, "Input second player name", 0.45, 0.95);
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, secondPlayerName.c_str(), 0.45, 0.88);
		}
		if (selectDifficultyMode) {
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, "Select difficulty", 0.45, 0.95);
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, "1 - Easy", 0.45, 0.88);
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, "2 - Normal", 0.45, 0.81);
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, "3 - Hard", 0.45, 0.74);
		}
		if (gameMode) {
			std::string time;
			if (firstPlayerTime / 60 < 10) {
				time = "0" + std::to_string(firstPlayerTime / 60);
			}
			else {
				time = std::to_string(firstPlayerTime / 60);
			}
			if (firstPlayerTime - firstPlayerTime / 60 * 60 < 10) {
				time += ":0" + std::to_string(firstPlayerTime - firstPlayerTime / 60 * 60);
			}
			else {
				time += ":" + std::to_string(firstPlayerTime - firstPlayerTime / 60 * 60);
			}
			std::string firstPlayerTimeString = "White player time: " + time;

			if (secondPlayerTime / 60 < 10) {
				time = "0" + std::to_string(secondPlayerTime / 60);
			}
			else {
				time = std::to_string(secondPlayerTime / 60);
			}
			if (secondPlayerTime - secondPlayerTime / 60 * 60 < 10) {
				time += ":0" + std::to_string(secondPlayerTime - secondPlayerTime / 60 * 60);
			}
			else {
				time += ":" + std::to_string(secondPlayerTime - secondPlayerTime / 60 * 60);
			}
			std::string secondPlayerTimeString = "Black player time: " + time;
			switch (difficulty) {
			case 1:
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "Time: unlimit", 0.01, 0.81);
				break;
			default:
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, firstPlayerTimeString.c_str(), 0.01, 0.81);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, secondPlayerTimeString.c_str(), 0.01, 0.74);
				break;
			}
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, interfaceNavigation.c_str(), 0.01, 0.95);
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, state.c_str(), 0.01, 0.88);
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, ("White - " + whitePlayer).c_str(), 0.3, 0.88); 
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, ("Black - " + blackPlayer).c_str(), 0.55, 0.88);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, promotion.c_str(), 0.3, 0.88);
		}
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

		if (gameMode) {
			// ������� �� ������:
			for (int i = 0; i < shapes.size(); i++) {
				shapes[i]->draw();
			}
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
	void Scene::on_size(int width, int height) {
		this->width = width;
		if (height == 0)
			height = 1;
		this->height = height;
	}
	// ���������� ����, ���'������ � ����������� ������ ����
	void Scene::on_mouse(int button, int state, int x, int y) {
		if (gameMode) {
			// �������� ������ ���������� ����:
			mouseX = x;
			mouseY = y;
			if (state == GLUT_UP && !promotionMode) { // ������ ������
				// ³����� ������ ���� ����
				if (button == 0) {
					//FIXES ���� ������ �� ������ ����� ������ ������, �� �� ����� ��-�� ���� �� �����, �� �� ������������, � ����� ��������� �� �����
					//TODO FIXES ������� ����� ������ ��� ����, ������� ���������, ������� ���
					//TODO ������� ���������� ���������, �� ���� ������ ��� ������� (���� �� ����� ��������� ���� ������, ������� ������ ��������� �������, �� ��������� �� ��������)
					downAllPieces();
					findNearest(x, y, mouseXCell, mouseZCell);
					if (mouseXCell == -1 || mouseZCell == -1 || currentZ == -1 || currentX == -1) {
						if (zStart != -1 && xStart != -1 && currentZ != -1 && currentX != -1) {
							pieces[fields[currentZ][currentX]]->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
						}
						this->button = -1;
						return;
					}
					if (fields[currentZ][currentX] != -1 && (zStart != mouseZCell || xStart != mouseXCell)) {
						Piece* p = pieces[fields[currentZ][currentX]]; // �������� �� ������� ������
						bool hit = false; // �����, �� ��������, ������ ���� �� ������ ����� �� �
						bool castlingOccured = false; // �����, �� ��������, ������� ���������� �� �
						bool enPassantOccured = false; // �����, �� ��������, ������� ������ �� ������ �� �
						bool promotionOccured = false; // �����, �� ��������, ������� ������������ ����� �� �
						// ���� ������ ���������� ������������ �� ��� �����, ��������� �� �������
						std::cout << "currentZ " << currentZ << "  currentX " << currentX << "\n";
						// ���� ���� ������ ����� ������ �� ������������ (�� �� ��� �����), �� �������� �������� ��������
						if (mouseZCell != -1 && mouseXCell != -1) {
							if (typeid(*p) == typeid(Pawn)) {
								enPassantOccured = static_cast<Pawn*>(p)->isEnPassantOccured(pieces, fields, prevPieceId, mouseZCell, mouseXCell);
								promotionOccured = static_cast<Pawn*>(p)->isPromotionOccured(mouseZCell, mouseXCell);
							}
							if (promotionOccured && checkW == -1 && checkB == -1) {
								promotionMode = 1;
							}
							if (enPassantOccured && checkW == -1 && checkB == -1) {
								if (pieces[prevPieceId]->getColor() == 'W') {
									pieces[prevPieceId]->setXCenter(6 + delB++);
									pieces[prevPieceId]->setZCenter(-3);
									pieces[prevPieceId]->setBeaten(true);
								}
								else {
									pieces[prevPieceId]->setXCenter(-5 - delW++);
									pieces[prevPieceId]->setZCenter(4);
									pieces[prevPieceId]->setBeaten(true);
								}
								checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
								checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
								whiteMove = !whiteMove;
							}
							// ���� ���� ��������� ��� � ����� �� ���
							else if (checkW != -1 && whiteMove) {
								// ���� ������ ����� � ����� �������, �� ���������� �� �������� ����
								if (collision && fields[mouseZCell][mouseXCell] != -1 && pieces[fields[mouseZCell][mouseXCell]] != p) {
									if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
										int k = fields[mouseZCell][mouseXCell];
										fields[mouseZCell][mouseXCell] = fields[currentZ][currentX];
										fields[currentZ][currentX] = -1;
										// ���� ���� �����, ������ ������� ����������� �� ������, ��������� ����
										checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
										fields[currentZ][currentX] = fields[mouseZCell][mouseXCell];
										fields[mouseZCell][mouseXCell] = k;
										// ���� ��� ���� ������� ������ �� ����, ��������� ����
										if (checkW == -1) {
											pieces[fields[mouseZCell][mouseXCell]]->setXCenter(-5 - delW++);
											pieces[fields[mouseZCell][mouseXCell]]->setZCenter(4);
											pieces[fields[mouseZCell][mouseXCell]]->setBeaten(true);
											if (p->getFirstMove()) {
												p->setFirstMove(false);
											}
											p->movePieceToPosition(fields, mouseZCell, mouseXCell, currentZ, currentX);
											whiteMove = !whiteMove;
										}
									}
									else {
										p->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
									}
								}
								// ���� ������ ���������� ��������, ���������� ���������� ����
								else if (p->isMovePossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
									// ���� ���� ����� ���� ������ �� ��������� �� ����, ��������� ������ �����
									if (static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields) != -1) {
										p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
									}
									// ���� ���� ����� ���� ������ ��������� �� ����, ����� ������ ������� ������
									else {
										if (p->getFirstMove()) {
											p->setFirstMove(false);
										}
										checkW = -1;
										whiteMove = !whiteMove;
									}
								}
								else {
									p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
								}
							}
							// ���� ������ ��������� ��� � ����� ��� ������
							else if (checkB != -1 && !whiteMove) {
								// ���� ������ ����� � ����� �������, �� ���������� �� �������� ����
								if (collision && fields[mouseZCell][mouseXCell] != -1 && pieces[fields[mouseZCell][mouseXCell]] != p) {
									if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
										int k = fields[mouseZCell][mouseXCell];
										fields[mouseZCell][mouseXCell] = fields[currentZ][currentX];
										fields[currentZ][currentX] = -1;
										// ���� ���� �����, ������ ������� ����������� �� ������, ��������� ����
										checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
										fields[currentZ][currentX] = fields[mouseZCell][mouseXCell];
										fields[mouseZCell][mouseXCell] = k;
										// ���� ��� ���� ������� ������ �� ����, ��������� ����
										if (checkB == -1) {
											pieces[fields[mouseZCell][mouseXCell]]->setXCenter(6 + delB++);
											pieces[fields[mouseZCell][mouseXCell]]->setZCenter(-3);
											pieces[fields[mouseZCell][mouseXCell]]->setBeaten(true);
											p->movePieceToPosition(fields, mouseZCell, mouseXCell, currentZ, currentX);
											if (p->getFirstMove()) {
												p->setFirstMove(false);
											}
											whiteMove = !whiteMove;
										}
									}
									else {
										p->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
									}
								}
								// ���� ������ ���������� ��������, ���������� ���������� ����
								else if (p->isMovePossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
									// ���� ���� ����� ���� ������ �� ��������� �� ����, ��������� ������ �����
									if (static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields) != -1) {
										p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
									}
									// ���� ���� ����� ���� ������ ��������� �� ����, ����� ������ ������� ������
									else {
										if (p->getFirstMove()) {
											p->setFirstMove(false);
										}
										checkB = -1;
										whiteMove = !whiteMove;
									}
								}
								else {
									p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
								}
							}
							// �������� �� ����� � ����� �������
							else if (collision && fields[mouseZCell][mouseXCell] != -1 && pieces[fields[mouseZCell][mouseXCell]] != p) {
								// ���� ���� ������� ������ ���� � ����� ��� ����
								if (p->getColor() == 'W' && whiteMove) {
									// ���� ���������� �� �� �������, � ������� ������ ������, ���������� ����������
									if (!castlingOccuredW && typeid(*p) == typeid(King)) {
										castlingOccuredW = static_cast<King*>(p)->isCastlingOccured(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX);
										castlingOccured = castlingOccuredW;
										hit = castlingOccuredW;
									}
									// ���� ���� ������� ������ ����, ����� ��� ���� � ������, ��� ������ ������, �����, �� ���������� ���� �� �����
									if (pieces[fields[mouseZCell][mouseXCell]]->getColor() == 'B' && !castlingOccured) {
										if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
											int k = fields[mouseZCell][mouseXCell];
											fields[mouseZCell][mouseXCell] = fields[currentZ][currentX];
											fields[currentZ][currentX] = -1;
											checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
											checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
											fields[currentZ][currentX] = fields[mouseZCell][mouseXCell];
											fields[mouseZCell][mouseXCell] = k;
											if (checkB != -1 && p->getColor() == 'B') {
												checkB = -1;
												hit = false;
											}
											else if (checkW != -1 && p->getColor() == 'W') {
												checkW = -1;
												hit = false;
											}
											else {
												pieces[fields[mouseZCell][mouseXCell]]->setXCenter(-5 - delW++);
												pieces[fields[mouseZCell][mouseXCell]]->setZCenter(4);
												pieces[fields[mouseZCell][mouseXCell]]->setBeaten(true);
												p->movePieceToPosition(fields, mouseZCell, mouseXCell, currentZ, currentX);
											}
										}
									}
								}
								// ���� ���� ������� ������ ������ � ����� ��� ������
								else if (p->getColor() == 'B' && !whiteMove) {
									// ���� ���������� �� �� �������, � ������� ������ ������, ���������� ����������
									if (!castlingOccuredB && typeid(*p) == typeid(King)) {
										castlingOccuredB = static_cast<King*>(p)->isCastlingOccured(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX);
										castlingOccured = castlingOccuredB;
										hit = castlingOccuredB;
									}
									// ���� ���� ������� ������ ������, ����� ��� ������ � ������, ��� ������ ������, ���, �� ���������� ���� �� ���
									if (pieces[fields[mouseZCell][mouseXCell]]->getColor() == 'W' && !castlingOccured) {
										if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
											int k = fields[mouseZCell][mouseXCell];
											fields[mouseZCell][mouseXCell] = fields[currentZ][currentX];
											fields[currentZ][currentX] = -1;
											checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
											checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
											fields[currentZ][currentX] = fields[mouseZCell][mouseXCell];
											fields[mouseZCell][mouseXCell] = k;
											if (checkB != -1 && p->getColor() == 'B') {
												checkB = -1;
												hit = false;
											}
											else if (checkW != -1 && p->getColor() == 'W') {
												checkW = -1;
												hit = false;
											}
											else {
												pieces[fields[mouseZCell][mouseXCell]]->setXCenter(6 + delB++);
												pieces[fields[mouseZCell][mouseXCell]]->setZCenter(-3);
												pieces[fields[mouseZCell][mouseXCell]]->setBeaten(true);
												p->movePieceToPosition(fields, mouseZCell, mouseXCell, currentZ, currentX);
											}
										}
									}
								}
								// ���������� ����������, ���� ���� ��� ���������� �� �������
								if (!hit && (zStart != mouseZCell || xStart != mouseXCell)) {
									p->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
								}
								// ���� ������� ���������� ��� ����, ��� ���������� �� ����������
								else if (castlingOccured || hit) {
									// �������� �� ��� ���� ����� �� ����������
									checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
									checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
									if (checkW != -1 && static_cast<King*>(pieces[28])->isMateOccured(pieces, fields)) {
										mateOccuredW = true;
									}
									else if (checkB != -1 && static_cast<King*>(pieces[4])->isMateOccured(pieces, fields)) {
										mateOccuredB = true;
									}
									else {
										if (p->getFirstMove()) {
											p->setFirstMove(false);
										}
										whiteMove = !whiteMove;
									}
									std::cout << "CheckB " << checkB << " CheckW " << checkW << std::endl;
								}
								collision = false;
							}
							else { // �������� ���������� ����������
								if ((p->getColor() == 'W' && whiteMove) || (p->getColor() == 'B' && !whiteMove)) {
									if (p->isMovePossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
										// �������� �� ��� ��� ���� �������
										checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
										checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
										// ���� ������ ������ ���, ���� �������� �� ����, ��������� ���� �����
										if (checkB != -1 && p->getColor() == 'B') {
											p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
											checkB = -1;
										}
										else if (checkW != -1 && p->getColor() == 'W') {
											p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
											checkW = -1;
										}
										else { // ���� ������� ������ ����������, ��� ���������� �� ����������
											if (checkW != -1 && static_cast<King*>(pieces[28])->isMateOccured(pieces, fields)) {
												mateOccuredW = true;
											}
											else if (checkB != -1 && static_cast<King*>(pieces[4])->isMateOccured(pieces, fields)) {
												mateOccuredB = true;
											}
											else {
												if (p->getFirstMove()) {
													p->setFirstMove(false);
												}
												whiteMove = !whiteMove;
											}
										}
										std::cout << "CheckB " << checkB << " CheckW " << checkW << std::endl;
									}
									else {
										p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
									}
								}
							}
						}
						prevPieceId = p->getId();
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
				this->button = -1;  // ���� ������ �� ���������
				return;
			}
			this->button = button;  // �������� ���������� ��� ������
			if (findNearest(x, y, currentX, currentZ) && currentX != -1 && currentZ != -1 && gameMode) {
				if (button == 0 && fields[currentZ][currentX] != -1 && !promotionMode) {
					upPiece(currentX, currentZ);
					xStart = currentX;
					zStart = currentZ;
				}
			}
		}
	}
	void Scene::on_motion(int x, int y) {
		if (gameMode) {
			switch (button) {
			case 0:
				if (findNearest(x, y, xTo, zTo) && fields[currentZ][currentX] != -1 && !promotionMode) {
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
	}

	void Scene::on_special(int key, int x, int y) {
		if (gameMode) {
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
	}

	void Scene::on_keyboard(unsigned char key, int x, int y) {
		if (inputFirstPlayerNameMode) {
			switch (key) {
			case 8:
				if (firstPlayerName.size() > 0) {
					firstPlayerName.pop_back();
				}
				break;
			case 13:
				if (firstPlayerName.size() > 0) {
					inputFirstPlayerNameMode = false;
					inputSecondPlayerNameMode = true;
				}
				break;
			default:
				firstPlayerName += key;
				break;
			}
		}
		else if (inputSecondPlayerNameMode) {
			switch(key) {
			case 8:
				if (secondPlayerName.size() > 0) {
					secondPlayerName.pop_back();
				}
				break;
			case 13:
				if (secondPlayerName.size() > 0) {
					inputSecondPlayerNameMode = false;
					selectDifficultyMode = true;
					if (rand() % 2 == 0) {
						whitePlayer = firstPlayerName;
						blackPlayer = secondPlayerName;
					}
					else {
						whitePlayer = secondPlayerName;
						blackPlayer = firstPlayerName;
					}
				}
				break;
			default:
				secondPlayerName += key;
				break;
			}
		}
		else if (selectDifficultyMode) {
			switch (key) {
			case '1':
				difficulty = 1;
				selectDifficultyMode = false;
				gameMode = true;
				break;
			case '2':
				difficulty = 2;
				firstPlayerTime = 120 * 60;
				secondPlayerTime = 120 * 60;
				selectDifficultyMode = false;
				gameMode = true;
				break;
			case '3':
				difficulty = 3;
				firstPlayerTime = 60 * 60;
				secondPlayerTime = 60 * 60;
				selectDifficultyMode = false;
				gameMode = true;
				break;
			}
		}
		else if (gameMode && promotionMode) {
			switch (key) {
			case 'q':
				promotionMode = 2;
				break;
			case 'b':
				promotionMode = 3;
				break;
			case 'k' :
				promotionMode = 4;
				break;
			case 'r':
				promotionMode = 5;
				break;
			}
		}
	}

	int tick = 0; // ��������, �������� ����� ��������� ���� 25 ��

				  // ���������� ��䳿 �� �������
	void Scene::on_timer(int value)
	{
		tick++;
		// ���������� �������� �������
		if (tick >= 40) {
			// ������� �����������, ���� ��� �� ��������
			if (!mateOccuredB && !mateOccuredW && gameMode) {
				if (whiteMove) {
					firstPlayerTime--;
				}
				else {
					secondPlayerTime--;
				}
			}
			tick = 0;   // ������� ��������
		}
		on_paint();     // ��������� ������������� ����
	}
}