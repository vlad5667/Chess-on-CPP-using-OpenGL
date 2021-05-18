#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <gl/glut.h>
#include <algorithm>
#include <string>
#include <ctime>
#include <fstream>
#include "Scene.h"
#include "Cube.h"
#include "utils.h"

namespace ChessGame {
	using namespace GraphUtils;

	bool firstInit = true; // �����, �� �������� ��� ����� ����������� ���
	int prevPieceId; // �����, �� ������ ������������� ������, �� ������ �� ����� (������� ��� ������ �� ������)
	int promotionMode; // �����, �� �������� ��� ���� ������ ��� ������������ �����
	bool castlingOccurredW, castlingOccurredB; // ����, �� �����������, ������� ���������� �� �
	int checkW, checkB; // ����, �� �����������, ������ ��� �� �
	bool mateOccurredW, mateOccurredB; // ����, �� �����������, ������ ��� �� �
	bool stalemateOccurredW, stalemateOccurredB; // ����, �� �����������, ������ ��� �� �
	int delW, delB; // ����, �� ������� ���������� � ��� ������� �����
	bool editBoardMode; // �����, �� �������� ��� ����� ����������� ����� (����� �� �����)

	void Scene::checkConditions() {
		for (int i = 0; i < 16; i++) {
			if (pieces[i]->getZCenter() != i / 8 - 3 || pieces[i]->getXCenter() != i % 8 - 3) {
				pieces[i]->setFirstMove(false);
			}
		}
		for (int i = 16; i < 32; i++) {
			if (pieces[i]->getZCenter() != i / 8 + 1 || pieces[i]->getXCenter() != i % 8 - 3) {
				pieces[i]->setFirstMove(false);
			}
		}
		checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
		if (checkW != -1) {
			whiteMove = true;
		}
		checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
		if (checkB != -1) {
			whiteMove = false;
		}
		if (mateOccurredW = static_cast<King*>(pieces[28])->isMateOccurred(pieces, fields)) {
			finish = true;
		}
		if (mateOccurredB = static_cast<King*>(pieces[4])->isMateOccurred(pieces, fields)) {
			finish = true;
		}
		if (stalemateOccurredW = static_cast<King*>(pieces[28])->isStalemateOccurred(pieces, fields)) {
			finish = true;
		}
		if (stalemateOccurredB = static_cast<King*>(pieces[4])->isStalemateOccurred(pieces, fields)) {
			finish = true;
		}
		if (whiteMove) { // ���� ����� ��� ����, ������������ ����� ������ ������
			angleX = 0;
			angleY = angleY < 0 ? -angleY : angleY;
		}
		else { // ���� ����� ��� ������, ������������ ����� ������ ������
			angleX = 180;
			angleY = angleY > 0 ? -angleY : angleY;
		}
	}

	bool sortComp(recordRow A, recordRow B) {
		std::string timeA = "";
		timeA += A.time[0];
		timeA += A.time[1];
		int timeInSecondsA = atoi(timeA.c_str()) * 60;
		timeA = "";
		timeA += A.time[3];
		timeA += A.time[4];
		timeInSecondsA += atoi(timeA.c_str());
		std::string timeB = "";
		timeB += B.time[0];
		timeB += B.time[1];
		int timeInSecondsB = atoi(timeB.c_str()) * 60;
		timeB = "";
		timeB += B.time[3];
		timeB += B.time[4];
		timeInSecondsB += atoi(timeB.c_str());
		return timeInSecondsA < timeInSecondsB;
	}

	void Scene::readRecords() {
		recordsTable.clear();
		recordsEasy.clear();
		recordsNormal.clear();
		recordsHard.clear();
		std::ifstream in("records.txt");
		if (!in.is_open()) {
			writeRecords();
		}
		std::string row;
		int dif;
		while (std::getline(in, row)) {
			recordsTable.push_back(row);
			if (row[0] == 'E') {
				dif = 1;
			}
			else if (row[0] == 'N') {
				dif = 2;
			}
			else if (row[0] == 'H') {
				dif = 3;
			}
			if (isdigit(row[0])) {
				std::string firstName = "", secondName = "", winner = "", moves = "", time = "";
				int i = 4;
				while (row[i] != '-') {
					firstName += row[i];
					i++;
				}
				i++;
				while (row[i] != ' ' && row[i + 1] != '|') {
					secondName += row[i];
					i++;
				}
				i += 3;
				while (row[i] != ' ' || row[i + 1] != '|') {
					winner += row[i];
					i++;
				}
				i += 3;
				while (row[i] != ' ' && row[i + 1] != '|') {
					moves += row[i];
					i++;
				}
				i += 3;
				while (row[i] != '\0') {
					time += row[i];
					i++;
				}
				recordRow record(firstName, secondName, winner, moves, time);
				switch (dif) {
				case 1:
					recordsEasy.push_back(record);
					break;
				case 2:
					recordsNormal.push_back(record);
					break;
				case 3:
					recordsHard.push_back(record);
					break;
				}
				
			}
		}
		in.close();
	}

	void Scene::writeRecords() {
		std::ofstream out("records.txt");
		int i = 1;
		out << "------------------------------\nEASY\n";
		for (auto& record : recordsEasy) {
			if (i > 5) break;
			out << i++ << " | " << record.whitePlayerName << "-" << record.blackPlayerName << " | " << record.winner << " | " << record.moves << " | " << record.time << "\n";
		}
		i = 1;
		out << "------------------------------\nNORMAL\n";
		for (auto& record : recordsNormal) {
			if (i > 5) break;
			out << i++ << " | " << record.whitePlayerName << "-" << record.blackPlayerName << " | " << record.winner << " | " << record.moves << " | " << record.time << "\n";
		}
		i = 1;
		out << "------------------------------\nHARD\n";
		for (auto& record : recordsHard) {
			if (i > 5) break;
			out << i++ << " | " << record.whitePlayerName << "-" << record.blackPlayerName << " | " << record.winner << " | " << record.moves << " | " << record.time << "\n";
		}
		out.close();
	}

	void Scene::addRecord() {
		recordRow record(whitePlayerName, blackPlayerName, winner, std::to_string(moves), timeString);
		switch (difficulty) {
		case 1:
			recordsEasy.push_back(record);
			std::sort(recordsEasy.begin(), recordsEasy.end(), sortComp);
			break;
		case 2:
			recordsNormal.push_back(record);
			std::sort(recordsNormal.begin(), recordsNormal.end(), sortComp);
			break;
		case 3:
			recordsHard.push_back(record);
			std::sort(recordsHard.begin(), recordsHard.end(), sortComp);
			break;
		}
		writeRecords();
	}

	void Scene::clearRecords() {
		recordsEasy.clear();
		recordsNormal.clear();
		recordsHard.clear();
		writeRecords();
		readRecords();
	}

	Scene::Scene() {	
		distZ = -13;
		angleY = 80;
		readRecords();
		initialize();
	}
	Scene::~Scene() {
		// ��������� �� ������:
		for (unsigned int i = 0; i < shapes.size(); i++) {
			delete shapes[i];
		}
	}
	//������������ ������ �� �����
	void Scene::resetPieces() {
		int k = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (i < 2 || i > 5) {
					fields[i][j] = k;
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

		// ������������ ������� ��� ������ �� �������������
		timeString = "";
		winner = "";
		time = 0;
		moves = 0;
		editBoardMode = true;
		delW = 0, delB = 0;
		castlingOccurredW = false, castlingOccurredB = false;
		checkW = -1, checkB = -1;
		mateOccurredW = false, mateOccurredB = false;
		stalemateOccurredW = false, stalemateOccurredB = false;
		prevPieceId = 0;
		promotionMode = 0;
		whiteMove = true;
		finish = false;
		switch (difficulty) {
		case 2:
			whitePlayerTime = blackPlayerTime = 120 * 60;
			break;
		case 3:
			whitePlayerTime = blackPlayerTime = 60 * 60;
		}
		angleX = 0;
		angleY = 80;

		if (rand() % 2 == 0) {
			whitePlayerName = firstPlayerName;
			blackPlayerName = secondPlayerName;
		}
		else {
			whitePlayerName = secondPlayerName;
			blackPlayerName = firstPlayerName;
		}
		for (int i = 0; i < 32; i++) {
			delete pieces[i];
		}
		shapes.clear();
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
		int k = 0;
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
			return false;
		}

		// ����������:
		pieces[fields[currentZ][currentX]]->movePieceToPosition(fields, zTo, xTo, currentZ, currentX);

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

				// ����� x, y, z ��������� ����:
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
		std::string state, promotion;
		if (promotionMode > 1) {
			int id = prevPieceId;
			float x = pieces[id]->getXCenter(), z = pieces[id]->getZCenter();
			delete pieces[id];
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
			checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
			checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
			promotionMode = 0;
		}
		if (promotionMode) {
			promotion = "Select a promotion piece (Q - Queen, B - Bishop, K - Knight, R - Rook)";
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

		if (finish) {
			glClearColor(0, 0.7, 0.7, 0);
		}
		else {
			glClearColor(0, 0.5, 0.5, 0);
		}

		// ������� ������:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();

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
		glTranslatef(-0.5f, 0.0f, distZ);	// ������ � ������� ��������� ��������� �� distZ, 

		glRotatef(angleX, 0.0f, 1.0f, 0.0f);  // ���� ���������� �� �� Oy
		glRotatef(angleY, 1.0f, 0.0f, 0.0f);  // ���� ���������� �� �� Ox 
		
		glEnable(GL_DEPTH_TEST);	// �������� ����� ������� (��� �������� ��������� ������ ����������)
									// �������� ����� ��� ��������� ���������:
		glEnable(GL_LIGHTING);

		// ������ ������� ����� � 0 (�� ���� ���� �� 8), ����� ���� ������ � "����":
		glEnable(GL_LIGHT0);

		if (currentMode == gameMode) {
			// ������� �� ������:
			for (int i = 0; i < shapes.size(); i++) {
				shapes[i]->draw();
			}
		}
		// �������� ���, �� ��������:
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		// ��� ����������� ������, ����� ��������������� ������������ ��������:
		glOrtho(0, 1, 0, 1, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glColor3f(1, 1, 0); // ������ �����
		switch (currentMode) {
		case inputFirstPlayerNameMode:
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, "Input first player name", 0.43, 0.6);
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, firstPlayerName.c_str(), 0.48, 0.55);
			break;
		case inputSecondPlayerNameMode:
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, "Input second player name", 0.42, 0.6);
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, secondPlayerName.c_str(), 0.48, 0.55);
			break;
		case selectDifficultyMode:
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, "Select difficulty", 0.46, 0.6);
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, "1 - Easy", 0.47, 0.55);
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, "2 - Normal", 0.47, 0.48);
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, "3 - Hard", 0.47, 0.41);
			break;
		case recordsMode:
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, "C - clear records", 0.01, 0.95);
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, "RECORDS", 0.46, 0.92);
			for (int i = 0; i < recordsTable.size(); i++) {
				double y = 0.88 - (4 * i) / 100.0;
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, recordsTable[i].c_str(), 0.35, y);
			}
			break;
		case gameMode:
			if (editBoardMode) {
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "ESC - exit game", 0.01, 0.95);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "F1 - start game", 0.15, 0.95);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "F2 - reset positions", 0.28, 0.95);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "F3 - change difficulty", 0.43, 0.95);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "F4 - change names", 0.6, 0.95);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "F5 - records table", 0.75, 0.95);
				state = "Edit board";
			}
			else {
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "ESC - exit game", 0.01, 0.95);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "F2 - restart game", 0.15, 0.95);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "F3 - change difficulty", 0.3, 0.95);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "F4 - change names", 0.47, 0.95);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "F5 - records table", 0.63, 0.95);
			}
			if (whitePlayerTime / 60 < 10) {
				timeString = "0" + std::to_string(whitePlayerTime / 60);
			}
			else {
				timeString = std::to_string(whitePlayerTime / 60);
			}
			if (whitePlayerTime - whitePlayerTime / 60 * 60 < 10) {
				timeString += ":0" + std::to_string(whitePlayerTime - whitePlayerTime / 60 * 60);
			}
			else {
				timeString += ":" + std::to_string(whitePlayerTime - whitePlayerTime / 60 * 60);
			}
			std::string whitePlayerTimeString = "White player time: " + timeString;

			if (blackPlayerTime / 60 < 10) {
				timeString = "0" + std::to_string(blackPlayerTime / 60);
			}
			else {
				timeString = std::to_string(blackPlayerTime / 60);
			}
			if (blackPlayerTime - blackPlayerTime / 60 * 60 < 10) {
				timeString += ":0" + std::to_string(blackPlayerTime - blackPlayerTime / 60 * 60);
			}
			else {
				timeString += ":" + std::to_string(blackPlayerTime - blackPlayerTime / 60 * 60);
			}
			std::string blackPlayerTimeString = "Black player time: " + timeString;

			if (time / 60 < 10) {
				timeString = "0" + std::to_string(time / 60);
			}
			else {
				timeString = std::to_string(time / 60);
			}
			if (time - time / 60 * 60 < 10) {
				timeString += ":0" + std::to_string(time - time / 60 * 60);
			}
			else {
				timeString += ":" + std::to_string(time - time / 60 * 60);
			}
			switch (difficulty) {
			case 1:
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "Time: unlimit", 0.01, 0.74);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, ("moves:" + std::to_string(moves)).c_str(), 0.9, 0.95);
				break;
			default:
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, whitePlayerTimeString.c_str(), 0.01, 0.81);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, blackPlayerTimeString.c_str(), 0.01, 0.74);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, timeString.c_str(), 0.9, 0.95);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, ("Moves: " + std::to_string(moves)).c_str(), 0.9, 0.88);
				break;
			}
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, ("White - " + whitePlayerName).c_str(), 0.01, 0.88);
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, ("Black - " + blackPlayerName).c_str(), 0.2, 0.88);
			drawString(GLUT_BITMAP_TIMES_ROMAN_24, promotion.c_str(), 0.25, 0.81);
			if (mateOccurredB) {
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, ("Winner - " + whitePlayerName).c_str(), 0.45, 0.88);
			}
			else if (mateOccurredW) {
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, ("Winner - " + blackPlayerName).c_str(), 0.45, 0.88);
			}
			else if (stalemateOccurredB || stalemateOccurredW) {
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "Draw", 0.48, 0.88);
			}
			else {
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, state.c_str(), 0.45, 0.88);
			}
			break;
		}
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

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
		if (currentMode == gameMode) {
			// �������� ������ ���������� ����:
			mouseX = x;
			mouseY = y;
			if (state == GLUT_UP && !promotionMode && !mateOccurredB && !mateOccurredW) { // ������ ������
				findNearest(x, y, mouseXCell, mouseZCell); // ��������� ���������� ����, �� ��� �������� �������
				// ³����� ������ ���� ����
				if (button == 0) {
					downAllPieces();
					if (editBoardMode) {
						if (fields[currentZ][currentX] != -1 && (zStart != mouseZCell || xStart != mouseXCell)) {
							if (mouseZCell != -1 && mouseXCell != -1) {
								Piece* p1 = pieces[fields[currentZ][currentX]], *p2 = pieces[fields[mouseZCell][mouseXCell]];
								if (fields[mouseZCell][mouseXCell] != -1 && p2 != p1) {
									if (p1->getColor() != p2->getColor() && typeid(*p2) != typeid(King)) {
										p2->setBeaten(true);
										if (p2->getColor() == 'B') {
											p2->setZCenter(4 - (delB / 3));
											p2->setXCenter(-5 - delB++ % 3);											
										}
										else {
											p2->setZCenter(-3 + (delW / 3));
											p2->setXCenter(6 + delW++ % 3);
										}
										p1->movePieceToPosition(fields, zTo, xTo, currentZ, currentX);
									}
								}
							}
						}
					}
					else {
						// ���� ������ ���������� ������������ �� ��� �����, ��������� �� �������
						if (mouseXCell == -1 || mouseZCell == -1 || currentZ == -1 || currentX == -1) {
							if (zStart != -1 && xStart != -1 && currentZ != -1 && currentX != -1) {
								pieces[fields[currentZ][currentX]]->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
							}
							this->button = -1;
							return;
						}
						// ���� ������� ���� �� ������ �� ���������� ������� ���� �� ���������� ����������� ���� ����
						if (fields[currentZ][currentX] != -1 && (zStart != mouseZCell || xStart != mouseXCell)) {
							Piece* p = pieces[fields[currentZ][currentX]]; // �������� �� ������� ������
							bool hit = false; // �����, �� ��������, ������ ���� �� ������ ����� �� �
							bool castlingOccurred = false; // �����, �� ��������, ������� ���������� �� �
							bool enPassantOccurred = false; // �����, �� ��������, ������� ������ �� ������ �� �
							bool promotionOccurred = false; // �����, �� ��������, ������� ������������ ����� �� �
							// ���� ���� ������ ����� ������ �� ������������ (�� �� ��� �����), �� �������� �������� ��������
							if (mouseZCell != -1 && mouseXCell != -1) {
								// ���� ������� ������ - ����, ���������� �� ������� ������ �� ������
								if (typeid(*p) == typeid(Pawn)) {
									if (enPassantOccurred = static_cast<Pawn*>(p)->isEnPassantOccurred(pieces, fields, prevPieceId, mouseZCell, mouseXCell) && checkW == -1 && checkB == -1) {
										if (pieces[prevPieceId]->getColor() == 'W') {
											pieces[prevPieceId]->setZCenter(-3 + (delW / 3));
											pieces[prevPieceId]->setXCenter(6 + delW++ % 3);
											pieces[prevPieceId]->setBeaten(true);
										}
										else {
											pieces[prevPieceId]->setZCenter(4 - (delB / 3));
											pieces[prevPieceId]->setXCenter(-5 - delB++ % 3);
											pieces[prevPieceId]->setBeaten(true);
										}
										// ���������� ���� ���� ������ �� ������
										checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
										checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
										whiteMove = !whiteMove;
										moves++;
									}
								}
								// �������� �� ����� � ����� �������
								if (fields[mouseZCell][mouseXCell] != -1 && pieces[fields[mouseZCell][mouseXCell]] != p) {
									// ���� ���� ������� ������ ���� � ����� ��� ����
									if (p->getColor() == 'W' && whiteMove) {
										// ���� ���������� �� �� �������, � ������� ������ ������, ���������� ����������
										if (!castlingOccurredW && typeid(*p) == typeid(King)) {
											castlingOccurredW = static_cast<King*>(p)->isCastlingOccurred(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX);
											castlingOccurred = castlingOccurredW;
											hit = castlingOccurredW;
										}
										// ���� ���� ������� ������ ����, ����� ��� ���� � ������, ��� ������ ������, �����, �� ���������� ���� �� �����
										if (pieces[fields[mouseZCell][mouseXCell]]->getColor() == 'B' && !castlingOccurred) {
											// ���� ���� ���������, ����� ������� ��������� ��������� ������ ��� �����
											if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
												// �����'������� ���� ����
												int actualCheckW = checkW;
												// ������ ���� �� ���������� ������� �� ��� ���� ����� �����
												int k = fields[mouseZCell][mouseXCell];
												fields[mouseZCell][mouseXCell] = fields[currentZ][currentX];
												fields[currentZ][currentX] = -1;
												checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
												checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
												fields[currentZ][currentX] = fields[mouseZCell][mouseXCell];
												fields[mouseZCell][mouseXCell] = k;
												// ���� ���� ����� ����, �� ���������� �� �����, ��������� ��� ����
												if (checkW != -1) {
													// ���� �� �� ����� ��� ���� �� �����, ��������� ������������� ������, ��� ��������� ��� ����, �� ��������, ��� ���� �� �����
													if (actualCheckW != -1) {
														checkW = actualCheckW;
													}
													// ���� �� �� ����� �� ���� �� �����, ������� ������������� ������, �� ��������� ��� ���� ����� - 1, ����� ���� ����
													else {
														checkW = -1;
													}
													// ������� ������������� ������, �� ��������� ��� ������ ����� -1, ����� ���� ����, ������� �� ���� ���� ��������� ��� ����
													checkB = -1;
													hit = false;
												}
												// ���� ���� ����� ���� ����, ��������� ��� ����
												else {
													pieces[fields[mouseZCell][mouseXCell]]->setZCenter(4 - (delB / 3));
													pieces[fields[mouseZCell][mouseXCell]]->setXCenter(-5 - delB++ % 3);
													pieces[fields[mouseZCell][mouseXCell]]->setBeaten(true);
													p->movePieceToPosition(fields, mouseZCell, mouseXCell, currentZ, currentX);
												}
											}
										}
									}
									// ���� ���� ������� ������ ������ � ����� ��� ������
									else if (p->getColor() == 'B' && !whiteMove) {
										// ���� ���������� �� �� �������, � ������� ������ ������, ���������� ����������
										if (!castlingOccurredB && typeid(*p) == typeid(King)) {
											castlingOccurredB = static_cast<King*>(p)->isCastlingOccurred(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX);
											castlingOccurred = castlingOccurredB;
											hit = castlingOccurredB;
										}
										// ���� ���� ������� ������ ������, ����� ��� ������ � ������, ��� ������ ������, ���, �� ���������� ���� �� ���
										if (pieces[fields[mouseZCell][mouseXCell]]->getColor() == 'W' && !castlingOccurred) {
											// ���� ���� ���������, ����� ������� ��������� ��������� ������ ��� �����
											if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
												// �����'������� ���� ����
												int actualCheckB = checkB;
												// ������ ���� �� ���������� ������� �� ��� ���� ����� �����
												int k = fields[mouseZCell][mouseXCell];
												fields[mouseZCell][mouseXCell] = fields[currentZ][currentX];
												fields[currentZ][currentX] = -1;
												checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
												checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
												fields[currentZ][currentX] = fields[mouseZCell][mouseXCell];
												fields[mouseZCell][mouseXCell] = k;
												// ���� ���� ����� ������, ���� ���������� �� �����, ��������� ��� ����
												if (checkB != -1) {
													// ���� ���� �� ����� ��� ���� �� �����, ��������� ������������� ������, ��� ��������� ��� ������, �� ��������, ��� ���� �� �����
													if (actualCheckB != -1) {
														checkB = actualCheckB;
													}
													// ���� ���� �� ����� �� ���� �� �����, ������� ������������� ������, �� ��������� ��� ������ ����� -1, ����� ���� ����
													else {
														checkB = -1;
													}
													// ������� ������������� ������, �� ��������� ��� ���� ����� -1, ����� ���� ����, ������� �� ���� ���� ��������� ��� ����
													checkW = -1;
													hit = false;
												}
												// ���� ���� ����� ���� ���� �� ��������� ��� ����
												else {
													pieces[fields[mouseZCell][mouseXCell]]->setZCenter(-3 + (delW / 3));
													pieces[fields[mouseZCell][mouseXCell]]->setXCenter(6 + delW++ % 3);
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
									// ���� ������� ���������� ��� ����
									else if (castlingOccurred || hit) {
										moves++;
										// ���� ���� ������ �� ����� ���������� �� ������ ���, ���� ���, ��� ����� �������, �������� ����� �� ������� �������
										if (checkW != -1 && static_cast<King*>(pieces[28])->isMateOccurred(pieces, fields)) {
											finish = mateOccurredW = true;
											winner = "winner - " + blackPlayerName;
											addRecord();
										}
										// ���� ������ ������ �� ����� ���������� �� ������ ���, ���� ���, ��� ����� �������, �������� ����� �� ������� �������
										else if (checkB != -1 && static_cast<King*>(pieces[4])->isMateOccurred(pieces, fields)) {
											finish = mateOccurredB = true;
											winner = "winner - " + whitePlayerName;
											addRecord();
										}
										// ���� �� ������, �� ���� ������ �� �� �����, ����������, �� ������ ���
										else if (finish = stalemateOccurredB = static_cast<King*>(pieces[4])->isStalemateOccurred(pieces, fields)) {
											winner = "draw";
											addRecord();
										}
										else if (finish = stalemateOccurredW = static_cast<King*>(pieces[28])->isStalemateOccurred(pieces, fields)) {
											winner = "draw";
											addRecord();
										}
										// ���� ���� �� ���� �� �������, ��� ���������� �� ����������
										else {
											if (p->getFirstMove()) {
												p->setFirstMove(false);
											}
											whiteMove = !whiteMove;
											// ���� ������� ������ - ����, ���������� �� ������� ������������ �����
											if (typeid(*p) == typeid(Pawn)) {
												// ���� ������������ ����� ������� � ����� ������ �� �����, �� ������� ����� ������������ �����
												if (promotionOccurred = static_cast<Pawn*>(p)->isPromotionOccurred(mouseZCell, mouseXCell) && checkW == -1 && checkB == -1) {
													promotionMode = 1;
												}
											}
										}
									}
								}
								else { // �������� ���������� ����������
									if ((p->getColor() == 'W' && whiteMove) || (p->getColor() == 'B' && !whiteMove)) {
										// ���� ����� ��� ��������
										if (p->isMovePossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
											// �����'������� ���� ���� ����� �� ������� �������
											int actualCheckW = checkW, actualCheckB = checkB;
											// �������� �� ��� ��� ���� �������
											checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
											checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
											// ���� ������ �������� ��� ������ ������, �� �����������, ���� ��������� ����������
											if (checkB == 28 && checkW == 4) {
												if (actualCheckB != -1) {
													checkB = actualCheckB;
												}
												else {
													checkB = -1;
												}
												if (actualCheckW != -1) {
													checkW = actualCheckW;
												}
												else {
													checkW = -1;
												}
												p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
											}
											// ���� ���� ������ �� ����� ���������� �� ������ ���, ���� ���, ��� ����� ��������, �������� ����� �� ������� �������
											else if (checkW != -1 && static_cast<King*>(pieces[28])->isMateOccurred(pieces, fields)) {
												finish = mateOccurredW = true;
												moves++;
												winner = "winner - " + blackPlayerName;
												addRecord();
											}
											// ���� ������ ������ �� ����� ���������� �� ������ ���, ���� ���, ��� ����� �������, �������� ����� �� ������� �������
											else if (checkB != -1 && static_cast<King*>(pieces[4])->isMateOccurred(pieces, fields)) {
												finish = mateOccurredB = true;
												moves++;
												winner = "winner - " + whitePlayerName;
												addRecord();
											}
											// ���� ��� �� ������ �� ���� ���������� ������ ������ ����������� �� �����, �� ��������� �� ����������
											else if (checkB != -1 && p->getColor() == 'B') {
												// ���� ���� �� ����� ��� ���� �� �����, ��������� ������������� ������, ��� ��������� ��� ������, �� ��������, ��� ���� �� �����
												if (actualCheckB != -1) {
													checkB = actualCheckB;
												}
												// ���� ���� �� ����� �� ���� �� �����, ������� ������������� ������, �� ��������� ��� ������ ����� -1, ����� ���� ����
												else {
													checkB = -1;
												}
												p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
											}
											// ���� ��� �� ������ �� ���� ���������� ���� ������ ����������� �� �����, �� ��������� �� ����������
											else if (checkW != -1 && p->getColor() == 'W') {
												// ���� �� �� ����� ��� ���� �� �����, ��������� ������������� ������, ��� ��������� ��� ����, �� ��������, ��� ���� �� ����������
												if (actualCheckW != -1) {
													checkW = actualCheckW;
												}
												// ���� �� �� ����� �� ���� �� �����, ������� ������������� ������, �� ��������� ��� ���� ����� - 1, ����� ���� ����
												else {
													checkW = -1;
												}
												p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
											}
											// ���� ���������� ������� �� ����� ������ �� �� �����
											else {
												if (finish = stalemateOccurredB = static_cast<King*>(pieces[4])->isStalemateOccurred(pieces, fields)) {
													moves++;
													winner = "draw";
													addRecord();
												}
												else if (finish = stalemateOccurredW = static_cast<King*>(pieces[28])->isStalemateOccurred(pieces, fields)) {
													moves++;
													winner = "draw";
													addRecord();
												}
												else {
													if (p->getFirstMove()) {
														p->setFirstMove(false);
													}
													whiteMove = !whiteMove;
													moves++;
													// ���� ������� ������ - ����, ���������� �� ������� ������������ �����
													if (typeid(*p) == typeid(Pawn)) {
														// ���� ������������ ����� ������� � ����� ������ �� �����, �� ������� ����� ������������ �����
														if (promotionOccurred = static_cast<Pawn*>(p)->isPromotionOccurred(mouseZCell, mouseXCell) && checkW == -1 && checkB == -1) {
															promotionMode = 1;
															checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
															checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
														}
													}
												}
											}
										}
										// ���� ��� �� ��������, ��������� ����������
										else {
											p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
										}
									}
								}
							}
							prevPieceId = p->getId(); // �����'������ ������������� ������, �� ������ �� �����
							if (whiteMove) { // ���� ����� ��� ����, ������������ ����� ������ ������
								angleX = 0;
								angleY = angleY < 0 ? -angleY : angleY;
							}
							else { // ���� ����� ��� ������, ������������ ����� ������ ������
								angleX = 180;
								angleY = angleY > 0 ? -angleY : angleY;
							}
						}
					}
				}
				else if (button == 2 && editBoardMode) {
					if (mouseZCell != -1 && mouseXCell != -1) {
						if (fields[mouseZCell][mouseXCell] != -1 && fields[mouseZCell][mouseXCell] != 4 && fields[mouseZCell][mouseXCell] != 28) {
							Piece* p = pieces[fields[mouseZCell][mouseXCell]];
							fields[mouseZCell][mouseXCell] = -1;
							if (p->getColor() == 'W') {
								p->setBeaten(true);
								p->setZCenter(-3 + (delW / 3));
								p->setXCenter(6 + delW++ % 3);
							}
							else {
								p->setZCenter(4 - (delB / 3));
								p->setXCenter(-5 - delB++ % 3);
							}
						}
					}
				}
				this->button = -1;  // ���� ������ �� ���������
				return;
			}
			this->button = button;  // �������� ���������� ��� ������
			// ���� ������ ���������� ������ �� �� ������ ����� �� �� ������ ���
			if (findNearest(x, y, currentX, currentZ) && currentX != -1 && currentZ != -1 && !mateOccurredB && !mateOccurredW) {
				// ���� ��������� ��� ������ �� ������� ���� �� ������ � �� �������� ����� ������������ �����
				if (button == 0 && fields[currentZ][currentX] != -1 && !promotionMode) {
					// ϳ������ ������
					upPiece(currentX, currentZ);
					xStart = currentX;
					zStart = currentZ;
				}
			}
		}
	}
	void Scene::on_motion(int x, int y) {
		// ���� �������� ����� ����� - ������� �����
		if (currentMode == gameMode) {
			switch (button) {
				// ���� ���� ����������� �� ������� ���� �������
			case 0:
				// ���� ������ ���������� �� �� ������ �����
				if (currentX != -1 && currentZ != -1) {
					Piece* p = pieces[fields[currentZ][currentX]];
					// ���� �������� ����� ��������� �����
					if (editBoardMode) {
						// ���� ������� ���� �� ������
						if (findNearest(x, y, xTo, zTo) && fields[currentZ][currentX] != -1) {
							// ���� ���������� �������, ������������ ������ ���������� ������
							if (movePiece(currentZ, currentX, zTo, xTo)) {
								currentX = xTo;
								currentZ = zTo;
							}
						}
					}
					// ���� ����� ��� ���� �� ������� ������  -���, ��� ����� ��� ������ �� ������� ������ - �����
					else if ((whiteMove && p->getColor() == 'W') || (!whiteMove && p->getColor() == 'B')) {
						// ���� ������� ���� �� ������, �� �������� ����� ������������ ����� �� �� ������ ���
						if (findNearest(x, y, xTo, zTo) && fields[currentZ][currentX] != -1 && !promotionMode && !mateOccurredB && !mateOccurredW) {
							// ���� ���������� �������, ������������ ������ ���������� ������
							if (movePiece(currentZ, currentX, zTo, xTo)) {
								currentX = xTo;
								currentZ = zTo;
							}
						}
					}
				}
				break;
				// ���� ���� ����������� �� ������� ������ �������
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
		if (currentMode == gameMode) {
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
			case GLUT_KEY_F1: // ������ ���
				if (editBoardMode) {
					editBoardMode = false;
					checkConditions();
				}
				break;
			case GLUT_KEY_F2:   // ���� ���
				initialize();
				break;
			case GLUT_KEY_F3: // ���� ���������
				currentMode = selectDifficultyMode;
				break;
			case GLUT_KEY_F4: // ���� ��'�
				currentMode = inputFirstPlayerNameMode;
				break;
			case GLUT_KEY_F5: // ������� �������
				readRecords(); // ������� ������� � �����
				currentMode = recordsMode;
				break;
			}
		}
	}

	void Scene::on_keyboard(unsigned char key, int x, int y) {
		if (currentMode == inputFirstPlayerNameMode) { // �������� ���� ������� ������
			switch (key) {
			case 8: // ���� ��������� ������ - backspace, �� ��������� ������� ������� ������
				if (firstPlayerName.size() > 0) {
					if (firstPlayerName == whitePlayerName) {
						whitePlayerName.pop_back();
					}
					else {
						blackPlayerName.pop_back();
					}
					firstPlayerName.pop_back();
				}
				break;
			case 13: // ���� ��������� ������ - enter, �� ���������� �� �������� ���� ������� ������
				if (firstPlayerName.size() > 0) {
					currentMode = inputSecondPlayerNameMode;
				}
				break;
			case 27: // ���� ��������� ������ - escape, �� ��� ����������� �� ��� ��� �������� � ���
				if (!firstInit) {
					currentMode = gameMode;
				}
				else {
					exit(0);
				}
				break;
			default: // ���� ��������� ������ - �� ��������� �����, ���������� �� � ����� ������
				if (isalpha(key)) {
					if (firstPlayerName == whitePlayerName) {
						whitePlayerName += key;
					}
					else {
						blackPlayerName += key;
					}
					firstPlayerName += key;
				}
				break;
			}
		}
		else if (currentMode == inputSecondPlayerNameMode) { // �������� ���� ������� ������
			switch(key) {
			case 8: // ���� ��������� ������ - backspace, �� ��������� ������� ������� ������
				if (secondPlayerName.size() > 0) {
					if (secondPlayerName == whitePlayerName) {
						whitePlayerName.pop_back();
					}
					else {
						blackPlayerName.pop_back();
					}
					secondPlayerName.pop_back();
				}
				break;
			case 13: // ���� ��������� ������ - enter, �� ���������� �� ��� ��� ������ ���� ���������
				if (secondPlayerName.size() > 0) {
					if (firstInit) {
						currentMode = selectDifficultyMode;
					}
					else {
						currentMode = gameMode;
					}
				}
				break;
			case 27: // ���� ��������� ������ - escape, �� ����������� �� ����������� ���� ������� ������
				currentMode = inputFirstPlayerNameMode;
				break;
			default: // ���� ��������� ������ - �� ��������� �����, ���������� �� � ����� ������
				if (isalpha(key)) {
					if (secondPlayerName == whitePlayerName) {
						whitePlayerName += key;
					}
					else {
						blackPlayerName += key;
					}
					secondPlayerName += key;
				}
				break;
			}
		}
		else if (currentMode == selectDifficultyMode) { // ���� ���� ���������
			switch (key) {
			case '1': // ���� ��������� ������ 1
				if (difficulty != 1) { // ���� ����� ��������� �� ����� �� ��� ������, �� ���������� ��� ������
					initialize();
				}
				difficulty = 1; // ������� ����� ��������� ����� �������
				firstInit = false;
				currentMode = gameMode; // ���������� �� ���
				break;
			case '2': // ���� ��������� ������ 2
				if (difficulty != 2) { // ���� ����� ��������� �� ����� �� ��� ������, �� ���������� ��� ������
					difficulty = 2; // ������� ����� ��������� ����� �����
					initialize(); // ���������� �� ���
				}
				firstInit = false;
				currentMode = gameMode;
				break;
			case '3': // ���� ��������� ������ 3
				if (difficulty != 3) { // ���� ����� ��������� �� ����� �� ��� �����, �� ���������� ��� ������
					difficulty = 3; // ������� ����� ��������� ����� �����
					initialize();
				}
				firstInit = false;
				currentMode = gameMode; // ���������� �� ���
				break;
			case 27: // ���� ��������� ������ - escape, �� ��� ����������� �� �������� ���� ������� ������ ��� ����������� �� ���
				if (!firstInit) {
					currentMode = gameMode;
				}
				else {
					currentMode = inputSecondPlayerNameMode;
				}
				break;
			}
		}
		else if (currentMode == recordsMode) { // ����� ��������� ������� �������
			if (key == 27) { // ���� ��������� ������ - escape, �� ����������� �� ���
				currentMode = gameMode;
			}
			else if (key == 'c' || key == 'C') { // ���� ��������� ������ C
				clearRecords(); // ������� ������� �������
			}
		}
		else if (currentMode == gameMode) { // ����� ���
			if (promotionMode) { // ���� ����� ����� ������������ �����
				switch (key) {
				case 'q': // ���� ��������� ������ q
					promotionMode = 2;
					break;
				case 'b': // ���� ��������� ������ b
					promotionMode = 3;
					break;
				case 'k': // ���� ��������� ������ k
					promotionMode = 4;
					break;
				case 'r': // ���� ��������� ������ r
					promotionMode = 5;
					break;
				}
			}
			if (key == 27) { // ���� ��������� ������ - escape, �� �������� � ���
				exit(0);
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
			// ���� ����� �� �� ��������
			if (!finish && !editBoardMode && currentMode == gameMode) {
				// �������� ���, �� ������� �� ������� ����
				time++;
				if (difficulty != 1) {
					// ��� ������ ������, ���� ����� ���� ���
					if (whiteMove) {
						whitePlayerTime--;
						if (whitePlayerTime < 0) {
							mateOccurredW = true;
						}
					}
					else {
						blackPlayerTime--;
						if (blackPlayerTime < 0) {
							mateOccurredB = true;
						}
					}
				}
			}
			tick = 0;   // ������� ��������
		}
		on_paint();     // ��������� ������������� ����
	}
}