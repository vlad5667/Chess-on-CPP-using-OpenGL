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
	std::string firstPlayerName = "", secondPlayerName = ""; // Змінні, що зберігають імена гравців
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
		// Видаляємо всі фігури:
		for (unsigned int i = 0; i < shapes.size(); i++) {
			delete shapes[i];
		}
	}
	//Розставлення фігур по місцям
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
		// Додаємо дошку для шахів
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
		// Додаємо фігури на дошку
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
		// Встановлення значень всіх змінних за замовчуванням
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
	// Переміщення фігури зі вказаної позиції на нову
	bool Scene::movePiece(int currentZ, int currentX, int zTo, int xTo) {
		// Перевірка можливості переміщення:
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

		// Переміщення:
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
	//Підняття фігури
	void Scene::upPiece(int x, int z) {
		pieces[fields[z][x]]->setYCenter(1);
	}
	//Опускання всіх фігур
	void Scene::downAllPieces() {
		for (int i = 0; i < 4 * N; i++)
		{
			pieces[i]->setYCenter(0.3);
		}
	}
	// Пошук клітинки, найближчої до позиції курсору миші:
	bool Scene::findNearest(int x, int y, int& x1, int& z1) {
		int viewport[4];
		int iMin = -1, jMin = -1;
		double mvMatrix[16], projMatrix[16];
		double minDist = 2000;

		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {

				// Світові x, y, z поточного стрижня:
				double wx = i - 3;
				double wy = 0.3;
				double wz = j - 3;

				// Заповнюємо масив viewport поточною областю перегляду:
				glGetIntegerv(GL_VIEWPORT, viewport);

				// Заповнюємо масиви поточними матрицями:
				glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
				glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

				// Світові x, y, z координати, що обертаються:
				double dx, dy, dz;

				// Отримуємо координати точки, на яку спроектовано поточний стрижень:
				gluProject(wx, wy, wz, mvMatrix, projMatrix, viewport, &dx, &dy, &dz);
				dy = viewport[3] - dy - 1; // dy необхідно перерахувати
				double d = (x - dx) * (x - dx) + (y - dy) * (y - dy); // квадрат відстані
				if (d < minDist) { // знайшли ближчий стрижень
					minDist = d;
					iMin = i;
					jMin = j;
				}
			}
		}
		if (minDist < 1000) { // знайшли найближчий стрижень
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
	// Оброблювач події, пов'язаної з перемалюванням вікна
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

		// Ініціалізуємо параметри матеріалів і джерела світла:
		float lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // колір фонового освітлення 
		float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // колір дифузного освітлення 
		float lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };// колір дзеркального відображення
		float lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f };// розташування джерела світла

														   // Встановлюємо параметри джерела світла:
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

		// Очищуємо буфери:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Для відображення тексту, краще використовувати ортографічну проекцію:
		glOrtho(0, 1, 0, 1, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(1, 1, 0); // жовтий текст
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

		// Включаємо режим роботи з матрицею проекцій:
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Задаємо усічений конус видимості в лівосторонній системі координат, 
		// 60 - кут видимості в градусах по осі у,
		// width/height - кут видимості уздовж осі x,
		// 1 и 100 - відстань від спостерігача до площин відсікання по глибині:
		gluPerspective(60, width / height, 1, 100);

		// Включаємо режим роботи з видовою матрицею:
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-0.4, 0, distZ);	// камера з початку координат зсувається на distZ, 

		glRotatef(angleX, 0.0f, 1.0f, 0.0f);  // потім обертається по осі Oy
		glRotatef(angleY, 1.0f, 0.0f, 0.0f);  // потім обертається по осі Ox 
		
		glEnable(GL_DEPTH_TEST);	// включаємо буфер глибини (для відсікання невидимих частин зображення)
									// Включаємо режим для установки освітлення:
		glEnable(GL_LIGHTING);

		// Додаємо джерело світла № 0 (їх може бути до 8), зараз воно світить з "очей":
		glEnable(GL_LIGHT0);

		if (currentMode == gameMode) {
			// Малюємо усі фігури:
			for (int i = 0; i < shapes.size(); i++) {
				shapes[i]->draw();
			}
		}
		// Вимикаємо все, що включили:
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glFlush();
		// показуємо вікно:
		glutSwapBuffers(); // перемикання буферів
	}
	// Оброблювач події, пов'язаної зі зміною розмірів вікна 
	void Scene::on_size(int width, int height) {
		this->width = width;
		if (height == 0)
			height = 1;
		this->height = height;
	}
	// Оброблювач подій, пов'язаних з натисканням кнопок миші
	void Scene::on_mouse(int button, int state, int x, int y) {
		if (currentMode == gameMode) {
			// Зберігаємо поточні координати миші:
			mouseX = x;
			mouseY = y;
			if (state == GLUT_UP && !promotionMode && !mateOccurredB && !mateOccurredW) { // кнопка віджата
				// Віджата кнопка була лівою
				if (button == 0) {
					//FIXES если король по числам может побить фигуру, но по факту из-за шаха не может, то он перемещается, а нужно оставлять на месте
					//TODO FIXES сделать чтобы король мог бить, сделать интерфейс, сделать Пат
					//TODO сделать нормальную рокировку, то есть учесть все условия (если на линии рокировки есть клетка, которая бьётся вражеской фигурой, то рокировка не возможна)
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
						Piece* p = pieces[fields[currentZ][currentX]]; // Вказівник на поточну фігуру
						bool hit = false; // Змінна, що сигналізує, стався удар по ворожій фігурі чи ні
						bool castlingOccurred = false; // Змінна, що сигналізує, сталося рокірування чи ні
						bool enPassantOccurred = false; // Змінна, що сигналізує, сталося взяття на прозоді чи ні
						bool promotionOccurred = false; // Змінна, що сигналізує, сталося перетворення пішака чи ні
						// Якщо фігура намагається переміститися за межі дошки, повертаємо її обратно
						std::cout << "currentZ " << currentZ << "  currentX " << currentX << "\n";
						// Якщо була обрана якась фігура та перемістилася (не за межі дошки), то виконуємо подальщі перевірки
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
							// Якщо білим оголошено шах і зараз їх хід
							else if (checkW != -1 && whiteMove) {
								// Якщо сталая колізія з іншою фігурою, то перевіряємо чи можливий удар
								if (collision && fields[mouseZCell][mouseXCell] != -1 && pieces[fields[mouseZCell][mouseXCell]] != p) {
									if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
										int k = fields[mouseZCell][mouseXCell];
										fields[mouseZCell][mouseXCell] = fields[currentZ][currentX];
										fields[currentZ][currentX] = -1;
										// Якщо після удару, король всеодно знаходиться під ударом, скасовуємо удар
										checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
										fields[currentZ][currentX] = fields[mouseZCell][mouseXCell];
										fields[mouseZCell][mouseXCell] = k;
										// Якщо цей удар звільняє короля від шаху, проводимо його
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
								// Якщо фігура намагається походити, перевіряємо коректність ходу
								else if (p->isMovePossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
									// Якщо після цього ходу король не звільнився від шаху, повертаємо фігуру назад
									if (static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields) != -1) {
										p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
									}
									// Якщо після цього ходу король звільнився від шаху, тобто фігура закрила короля
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
							// Якщо чорним оголошено шах і зараз хід чорних
							else if (checkB != -1 && !whiteMove) {
								// Якщо сталая колізія з іншою фігурою, то перевіряємо чи можливий удар
								if (collision && fields[mouseZCell][mouseXCell] != -1 && pieces[fields[mouseZCell][mouseXCell]] != p) {
									if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
										int k = fields[mouseZCell][mouseXCell];
										fields[mouseZCell][mouseXCell] = fields[currentZ][currentX];
										fields[currentZ][currentX] = -1;
										// Якщо після удару, король всеодно знаходиться під ударом, скасовуємо удар
										checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
										fields[currentZ][currentX] = fields[mouseZCell][mouseXCell];
										fields[mouseZCell][mouseXCell] = k;
										// Якщо цей удар звільняє короля від шаху, проводимо його
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
								// Якщо фігура намагається походити, перевіряємо коректність ходу
								else if (p->isMovePossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
									// Якщо після цього ходу король не звільнився від шаху, повертаємо фігуру назад
									if (static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields) != -1) {
										p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
									}
									// Якщо після цього ходу король звільнився від шаху, тобто фігура закрила короля
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
							// Перевірка на колізію з іншою фігурою
							else if (collision && fields[mouseZCell][mouseXCell] != -1 && pieces[fields[mouseZCell][mouseXCell]] != p) {
								// Якщо колір поточної фігури білий і зараз хід білих
								if (p->getColor() == 'W' && whiteMove) {
									// Якщо рокірування ще не сталося, і поточна фігура король, перевіряємо рокірування
									if (!castlingOccurredW && typeid(*p) == typeid(King)) {
										castlingOccurredW = static_cast<King*>(p)->isCastlingOccurred(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX);
										castlingOccurred = castlingOccurredW;
										hit = castlingOccurredW;
									}
									// Якщо колір поточної фігури білий, зараз хід білих і фігура, яку хочемо побити, чорна, то перевіряємо удар по чорній
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
								// Якщо колір поточної фігури чорний і зараз хід чорних
								else if (p->getColor() == 'B' && !whiteMove) {
									// Якщо рокірування ще не сталося, і поточна фігура король, перевіряємо рокірування
									if (!castlingOccurredB && typeid(*p) == typeid(King)) {
										castlingOccurredB = static_cast<King*>(p)->isCastlingOccurred(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX);
										castlingOccurred = castlingOccurredB;
										hit = castlingOccurredB;
									}
									// Якщо колір поточної фігури чорний, зараз хід чорних і фігура, яку хочемо побити, біла, то перевіряємо удар по білій
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
								// Скасування переміщення, якщо удар або рокірування не сталося
								if (!hit && (zStart != mouseZCell || xStart != mouseXCell)) {
									p->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
								}
								// Якщо сталося рокірування або удар, хід переходить до противника
								else if (castlingOccurred || hit) {
									// Перевірка на шах після удару чи рокірування
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
							else { // Перевірка коректності переміщення
								if ((p->getColor() == 'W' && whiteMove) || (p->getColor() == 'B' && !whiteMove)) {
									if (p->isMovePossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
										// Перевірка на шах для обох королей
										checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
										checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
										// Якщо король робить хід, який призведе до шаху, повертаємо його назад
										if (checkB != -1 && p->getColor() == 'B') {
											p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
											checkB = -1;
										}
										else if (checkW != -1 && p->getColor() == 'W') {
											p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
											checkW = -1;
										}
										else {
											// Якщо королю оголошено шах, перевіряємо чи може він звільнитися від нього, якщо ні, то це мат
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
				this->button = -1;  // ніяка кнопка не натиснута
				return;
			}
			this->button = button;  // зберігаємо інформацію про кнопки
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
			case 2: // права кнопка - обертання сцени
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
			case GLUT_KEY_UP:   // наближення
				if (distZ > -8) {
					break;
				}
				distZ += 0.3;
				break;
			case GLUT_KEY_DOWN: // віддалення
				if (distZ <= -17) {
					break;
				}
				distZ -= 0.3;
				break;
			case GLUT_KEY_F2:   // нова гра
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

	int tick = 0; // лічильник, значення якого змінюється кожні 25 мс

				  // Оброблювач події від таймера
	void Scene::on_timer(int value)
	{
		tick++;
		// нарахували наступну секунду
		if (tick >= 40) {
			if (!mateOccurredB && !mateOccurredW && currentMode == gameMode) {
				// секунди нарощуються, якщо гру не закінчено
				time++;
				if (difficulty != 1) {
					// час гравця спливає, якщо зараз його хід
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
			tick = 0;   // скинули лічильник
		}
		on_paint();     // здійснюємо перемалювання вікна
	}
}