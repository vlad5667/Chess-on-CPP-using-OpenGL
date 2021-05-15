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
	std::string firstPlayerName = "", secondPlayerName = ""; // ����, �� ��������� ����� �������
	std::string whitePlayer, blackPlayer;
	std::string timeString = "";

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
		return timeInSecondsA > timeInSecondsB;
	}

	void Scene::readRecords() {
		std::ifstream in("records.txt");
		if (!in.is_open()) {
			writeRecords();
		}
		recordsTable.clear();
		recordsEasy.clear();
		recordsNormal.clear();
		recordsHard.clear();
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
				std::string firstName = "", secondName = "", winner = "", steps = "", time = "";
				int i = 2;
				while (row[i] != '-') {
					firstName += row[i];
					i++;
				}
				i++;
				while (row[i] != ' ' && row[i + 1] != '|') {
					secondName += row[i];
					i++;
				}
				i += 12;
				while (row[i] != ' ' && row[i + 1] != '|') {
					winner += row[i];
					i++;
				}
				i += 3;
				while (row[i] != ' ' && row[i + 1] != '|') {
					steps += row[i];
					i++;
				}
				i += 3;
				while (row[i] != '\0') {
					time += row[i];
					i++;
				}
				recordRow record(firstName, secondName, winner, steps, time);
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
			out << i++ << " | " << record.firstPlayerName << "-" << record.secondPlayerName << " | winner - " << record.winner << " | " << record.steps << " | " << record.time << "\n";
		}
		i = 1;
		out << "------------------------------\nNORMAL\n";
		for (auto& record : recordsNormal) {
			if (i > 5) break;
			out << i++ << " | " << record.firstPlayerName << "-" << record.secondPlayerName << " | winner - " << record.winner << " | " << record.steps << " | " << record.time << "\n";
		}
		i = 1;
		out << "------------------------------\nHARD\n";
		for (auto& record : recordsHard) {
			if (i > 5) break;
			out << i++ << " | " << record.firstPlayerName << "-" << record.secondPlayerName << " | winner - " << record.winner << " | " << record.steps << " | " << record.time << "\n";
		}
		out.close();
	}

	void Scene::clearRecords() {
		recordsEasy.clear();
		recordsNormal.clear();
		recordsHard.clear();
		std::ofstream out("records.txt");
		out.close();
	}

	Scene::Scene() {
		readRecords();
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
		timeString = "";
		time = 0;
		steps = 0;
		collision = false;
		delW = 0, delB = 0;
		castlingOccurredW = false;
		castlingOccurredB = false;
		checkW = -1, checkB = -1;
		mateOccurredW = false;
		mateOccurredB = false;
		prevPieceId = 0;
		promotionMode = 0;
		whiteMove = true;
		switch (difficulty) {
		case 2:
			whitePlayerTime = blackPlayerTime = 120 * 60;
			break;
		case 3:
			whitePlayerTime = blackPlayerTime = 60 * 60;
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
		if (mateOccurredW) {
			state = "Mate white";
		}
		else if (mateOccurredB) {
			state = "Mate black";
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

		if (mateOccurredW || mateOccurredB) {
			glClearColor(0, 0.7, 0.7, 0);
		}
		else {
			glClearColor(0, 0.5, 0.5, 0);
		}

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
		switch (currentMode) {
			case inputFirstPlayerNameMode:
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "Input first player name", 0.43, 0.6);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, firstPlayerName.c_str(), 0.47, 0.55);
				break;
			case inputSecondPlayerNameMode:
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "Input second player name", 0.42, 0.6);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, secondPlayerName.c_str(), 0.47, 0.55);
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
			case placementMode:
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "F1 - auto place", 0.6, 0.95);
				break;
			case gameMode:
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
					drawString(GLUT_BITMAP_TIMES_ROMAN_24, ("Steps:" + std::to_string(steps)).c_str(), 0.9, 0.95);
					break;
				default:
					drawString(GLUT_BITMAP_TIMES_ROMAN_24, whitePlayerTimeString.c_str(), 0.01, 0.74);
					drawString(GLUT_BITMAP_TIMES_ROMAN_24, blackPlayerTimeString.c_str(), 0.01, 0.67);
					drawString(GLUT_BITMAP_TIMES_ROMAN_24, timeString.c_str(), 0.9, 0.95);
					drawString(GLUT_BITMAP_TIMES_ROMAN_24, ("Steps: " + std::to_string(steps)).c_str(), 0.9, 0.88);
					break;
				}
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "F2 - restart game", 0.01, 0.95);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "F3 - change difficulty", 0.2, 0.95);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "F4 - change names", 0.4, 0.95);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, "F5 - records table", 0.6, 0.95);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, ("White - " + whitePlayer).c_str(), 0.01, 0.88);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, ("Black - " + blackPlayer).c_str(), 0.2, 0.88);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, state.c_str(), 0.01, 0.81);
				drawString(GLUT_BITMAP_TIMES_ROMAN_24, promotion.c_str(), 0.25, 0.74);
				if (mateOccurredB) {
					drawString(GLUT_BITMAP_TIMES_ROMAN_24, ("Winner - " + whitePlayer).c_str(), 0.45, 0.88);
				}
				else if (mateOccurredW) {
					drawString(GLUT_BITMAP_TIMES_ROMAN_24, ("Winner - " + blackPlayer).c_str(), 0.45, 0.88);
				}
				break;
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
						bool castlingOccurred = false; // �����, �� ��������, ������� ���������� �� �
						bool enPassantOccurred = false; // �����, �� ��������, ������� ������ �� ������ �� �
						bool promotionOccurred = false; // �����, �� ��������, ������� ������������ ����� �� �
						// ���� ������ ���������� ������������ �� ��� �����, ��������� �� �������
						std::cout << "currentZ " << currentZ << "  currentX " << currentX << "\n";
						// ���� ���� ������ ����� ������ �� ������������ (�� �� ��� �����), �� �������� �������� ��������
						if (mouseZCell != -1 && mouseXCell != -1) {
							if (typeid(*p) == typeid(Pawn)) {
								enPassantOccurred = static_cast<Pawn*>(p)->isEnPassantOccurred(pieces, fields, prevPieceId, mouseZCell, mouseXCell);
								promotionOccurred = static_cast<Pawn*>(p)->isPromotionOccurred(mouseZCell, mouseXCell);
							}
							if (promotionOccurred && checkW == -1 && checkB == -1) {
								promotionMode = 1;
							}
							if (enPassantOccurred && checkW == -1 && checkB == -1) {
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
								checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
								checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
								whiteMove = !whiteMove;
								steps++;
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
										checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
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
											steps++;
										}
									}
									else {
										p->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
									}
								}
								// ���� ������ ���������� ��������, ���������� ���������� ����
								else if (p->isMovePossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
									// ���� ���� ����� ���� ������ �� ��������� �� ����, ��������� ������ �����
									if (static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields) != -1) {
										p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
									}
									// ���� ���� ����� ���� ������ ��������� �� ����, ����� ������ ������� ������
									else {
										if (p->getFirstMove()) {
											p->setFirstMove(false);
										}
										checkW = -1;
										whiteMove = !whiteMove;
										steps++;
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
										checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
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
											steps++;
										}
									}
									else {
										p->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
									}
								}
								// ���� ������ ���������� ��������, ���������� ���������� ����
								else if (p->isMovePossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
									// ���� ���� ����� ���� ������ �� ��������� �� ����, ��������� ������ �����
									if (static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields) != -1) {
										p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
									}
									// ���� ���� ����� ���� ������ ��������� �� ����, ����� ������ ������� ������
									else {
										if (p->getFirstMove()) {
											p->setFirstMove(false);
										}
										checkB = -1;
										whiteMove = !whiteMove;
										steps++;
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
									if (!castlingOccurredW && typeid(*p) == typeid(King)) {
										castlingOccurredW = static_cast<King*>(p)->isCastlingOccurred(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX);
										castlingOccurred = castlingOccurredW;
										hit = castlingOccurredW;
									}
									// ���� ���� ������� ������ ����, ����� ��� ���� � ������, ��� ������ ������, �����, �� ���������� ���� �� �����
									if (pieces[fields[mouseZCell][mouseXCell]]->getColor() == 'B' && !castlingOccurred) {
										if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
											int k = fields[mouseZCell][mouseXCell];
											fields[mouseZCell][mouseXCell] = fields[currentZ][currentX];
											fields[currentZ][currentX] = -1;
											checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
											checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
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
									if (!castlingOccurredB && typeid(*p) == typeid(King)) {
										castlingOccurredB = static_cast<King*>(p)->isCastlingOccurred(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX);
										castlingOccurred = castlingOccurredB;
										hit = castlingOccurredB;
									}
									// ���� ���� ������� ������ ������, ����� ��� ������ � ������, ��� ������ ������, ���, �� ���������� ���� �� ���
									if (pieces[fields[mouseZCell][mouseXCell]]->getColor() == 'W' && !castlingOccurred) {
										if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
											int k = fields[mouseZCell][mouseXCell];
											fields[mouseZCell][mouseXCell] = fields[currentZ][currentX];
											fields[currentZ][currentX] = -1;
											checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
											checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
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
								else if (castlingOccurred || hit) {
									// �������� �� ��� ���� ����� �� ����������
									checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
									checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
									if (checkW != -1 && static_cast<King*>(pieces[28])->isMateOccurred(pieces, fields)) {
										mateOccurredW = true;
										recordRow record(firstPlayerName, secondPlayerName, blackPlayer, std::to_string(steps), timeString);
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
									else if (checkB != -1 && static_cast<King*>(pieces[4])->isMateOccurred(pieces, fields)) {
										mateOccurredB = true;
										recordRow record(firstPlayerName, secondPlayerName, whitePlayer, std::to_string(steps), timeString);
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
									else {
										if (p->getFirstMove()) {
											p->setFirstMove(false);
										}
										whiteMove = !whiteMove;
										steps++;
									}
									std::cout << "CheckB " << checkB << " CheckW " << checkW << std::endl;
								}
								collision = false;
							}
							else { // �������� ���������� ����������
								if ((p->getColor() == 'W' && whiteMove) || (p->getColor() == 'B' && !whiteMove)) {
									if (p->isMovePossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
										// �������� �� ��� ��� ���� �������
										checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
										checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
										// ���� ������ ������ ���, ���� �������� �� ����, ��������� ���� �����
										if (checkB != -1 && p->getColor() == 'B') {
											p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
											checkB = -1;
										}
										else if (checkW != -1 && p->getColor() == 'W') {
											p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
											checkW = -1;
										}
										else {
											// ���� ������ ��������� ���, ���������� �� ���� �� ���������� �� �����, ���� �, �� �� ���
											if (checkW != -1 && static_cast<King*>(pieces[28])->isMateOccurred(pieces, fields)) {
												mateOccurredW = true;
												recordRow record(firstPlayerName, secondPlayerName, blackPlayer, std::to_string(steps), timeString);
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
											else if (checkB != -1 && static_cast<King*>(pieces[4])->isMateOccurred(pieces, fields)) {
												mateOccurredB = true;
												recordRow record(firstPlayerName, secondPlayerName, whitePlayer, std::to_string(steps), timeString);
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
											else {
												if (p->getFirstMove()) {
													p->setFirstMove(false);
												}
												whiteMove = !whiteMove;
												steps++;
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
			if (findNearest(x, y, currentX, currentZ) && currentX != -1 && currentZ != -1 && !mateOccurredB && !mateOccurredW) {
				if (button == 0 && fields[currentZ][currentX] != -1 && !promotionMode) {
					upPiece(currentX, currentZ);
					xStart = currentX;
					zStart = currentZ;
				}
			}
		}
	}
	void Scene::on_motion(int x, int y) {
		if (currentMode == gameMode) {
			switch (button) {
			case 0:
				if (findNearest(x, y, xTo, zTo) && fields[currentZ][currentX] != -1 && !promotionMode && !mateOccurredB && !mateOccurredW) {
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
			case GLUT_KEY_F2:   // ���� ���
				initialize();
				break;
			case GLUT_KEY_F3:
				currentMode = selectDifficultyMode;
				break;
			case GLUT_KEY_F4:
				currentMode = inputFirstPlayerNameMode;
				break;
			case GLUT_KEY_F5:
				readRecords();
				currentMode = recordsMode;
				break;
			}
		}
	}

	void Scene::on_keyboard(unsigned char key, int x, int y) {
		if (currentMode == inputFirstPlayerNameMode) {
			switch (key) {
			case 8:
				if (firstPlayerName.size() > 0) {
					firstPlayerName.pop_back();
				}
				break;
			case 13:
				if (firstPlayerName.size() > 0) {
					currentMode = inputSecondPlayerNameMode;
				}
				break;
			case 27:
				if (!firstInit) {
					currentMode = gameMode;
				}
				else {
					exit(0);
				}
				break;
			default:
				firstPlayerName += key;
				break;
			}
		}
		else if (currentMode == inputSecondPlayerNameMode) {
			switch(key) {
			case 8:
				if (secondPlayerName.size() > 0) {
					secondPlayerName.pop_back();
				}
				break;
			case 13:
				if (secondPlayerName.size() > 0) {
					if (firstInit) {
						currentMode = selectDifficultyMode;
					}
					else {
						firstInit = false;
						currentMode = gameMode;
					}
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
			case 27:
				if (!firstInit) {
					currentMode = inputFirstPlayerNameMode;
				}
				else {
					currentMode = inputFirstPlayerNameMode;
				}
				break;
			default:
				if (isalpha(key)) {
					secondPlayerName += key;
				}
				break;
			}
		}
		else if (currentMode == selectDifficultyMode) {
			switch (key) {
			case '1':
				if (difficulty != 1) {
					initialize();
				}
				firstInit = false;
				difficulty = 1;
				currentMode = gameMode;
				break;
			case '2':
				if (difficulty != 2) {
					difficulty = 2;
					initialize();
				}
				firstInit = false;
				currentMode = gameMode;
				break;
			case '3':
				if (difficulty != 3) {
					difficulty = 3;
					initialize();
				}
				firstInit = false;
				currentMode = gameMode;
				break;
			case 27:
				if (!firstInit) {
					currentMode = gameMode;
				}
				else {
					currentMode = inputSecondPlayerNameMode;
				}
				break;
			}
		}
		else if (currentMode == gameMode) {
			if (promotionMode) {
				switch (key) {
				case 'q':
					promotionMode = 2;
					break;
				case 'b':
					promotionMode = 3;
					break;
				case 'k':
					promotionMode = 4;
					break;
				case 'r':
					promotionMode = 5;
					break;
				}
			}
			if (key == 27) {
				exit(0);
			}
		}
		else if (currentMode == recordsMode) {
			readRecords();
			if (key == 27) {
				currentMode = gameMode;
			}
			else if (key == 'c' || key == 'C') {
				clearRecords();
				writeRecords();
				readRecords();
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
			if (!mateOccurredB && !mateOccurredW && currentMode == gameMode) {
				// ������� �����������, ���� ��� �� ��������
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