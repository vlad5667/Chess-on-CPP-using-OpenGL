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

	bool firstInit = true; // зм≥нна, що сигнал≥зуЇ про першу ≥н≥ц≥ал≥зац≥ю гри
	int prevPieceId; // «м≥нна, що збер≥гаЇ ≥дентиф≥катор ф≥гури, що ходила до цього (потр≥бно дл€ вз€тт€ на проход≥)
	int promotionMode; // «м≥нна, що сигнал≥зуЇ про виб≥р ф≥гури дл€ перетворенн€ п≥шака
	bool castlingOccurredW, castlingOccurredB; // «м≥нн≥, що сигнал≥зубть, сталос€ рок≥руванн€ чи н≥
	int checkW, checkB; // «м≥нн≥, що сигнал≥зують, ставс€ шах чи н≥
	bool mateOccurredW, mateOccurredB; // «м≥нн≥, що сигнал≥зують, ставс€ мат чи н≥
	bool stalemateOccurredW, stalemateOccurredB; // «м≥нн≥, що сигнал≥зують, ставс€ пат чи н≥
	int delW, delB; // «м≥нн≥, що задають координату ’ дл€ побитих ф≥гур
	bool editBoardMode; // «м≥нна, що сигнал≥зуЇ про режим редагуванн€ дошки (ф≥гур на дошц≥)

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
		if (whiteMove) { // якщо зараз х≥д б≥лих, встановлюЇмо такий ракурс камери
			angleX = 0;
			angleY = angleY < 0 ? -angleY : angleY;
		}
		else { // якщо зараз х≥д чорних, встановлюЇмо такий ракурс камери
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
		// ¬идал€Їмо вс≥ ф≥гури:
		for (unsigned int i = 0; i < shapes.size(); i++) {
			delete shapes[i];
		}
	}
	//–озставленн€ ф≥гури по м≥сц€м
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

		// ¬становленн€ значень вс≥х зм≥нних за замовчуванн€м
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
		// ƒодаЇмо дошку дл€ шах≥в
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
		// ƒодаЇмо ф≥гури на дошку
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
	// ѕерем≥щенн€ ф≥гури з≥ вказаноњ позиц≥њ на нову
	bool Scene::movePiece(int currentZ, int currentX, int zTo, int xTo) {
		// ѕерев≥рка можливост≥ перем≥щенн€:
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

		// ѕерем≥щенн€:
		pieces[fields[currentZ][currentX]]->movePieceToPosition(fields, zTo, xTo, currentZ, currentX);

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				std::cout << fields[i][j] << " ";
			}
			std::cout << std::endl;
		}
		return true;
	}
	//ѕ≥дн€тт€ ф≥гури
	void Scene::upPiece(int x, int z) {
		pieces[fields[z][x]]->setYCenter(1);
	}
	//ќпусканн€ вс≥х ф≥гур
	void Scene::downAllPieces() {
		for (int i = 0; i < 4 * N; i++)
		{
			pieces[i]->setYCenter(0.3);
		}
	}
	// ѕошук кл≥тинки, найближчоњ до позиц≥њ курсору миш≥:
	bool Scene::findNearest(int x, int y, int& x1, int& z1) {
		int viewport[4];
		int iMin = -1, jMin = -1;
		double mvMatrix[16], projMatrix[16];
		double minDist = 2000;

		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {

				// —в≥тов≥ x, y, z поточного пол€:
				double wx = i - 3;
				double wy = 0.3;
				double wz = j - 3;

				// «аповнюЇмо масив viewport поточною областю перегл€ду:
				glGetIntegerv(GL_VIEWPORT, viewport);

				// «аповнюЇмо масиви поточними матриц€ми:
				glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
				glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

				// —в≥тов≥ x, y, z координати, що обертаютьс€:
				double dx, dy, dz;

				// ќтримуЇмо координати точки, на €ку спроектовано поточний стрижень:
				gluProject(wx, wy, wz, mvMatrix, projMatrix, viewport, &dx, &dy, &dz);
				dy = viewport[3] - dy - 1; // dy необх≥дно перерахувати
				double d = (x - dx) * (x - dx) + (y - dy) * (y - dy); // квадрат в≥дстан≥
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
	// ќброблювач под≥њ, пов'€заноњ з перемалюванн€м в≥кна
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

		// ≤н≥ц≥ал≥зуЇмо параметри матер≥ал≥в ≥ джерела св≥тла:
		float lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // кол≥р фонового осв≥тленн€ 
		float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // кол≥р дифузного осв≥тленн€ 
		float lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };// кол≥р дзеркального в≥дображенн€
		float lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f };// розташуванн€ джерела св≥тла

														   // ¬становлюЇмо параметри джерела св≥тла:
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

		// ќчищуЇмо буфери:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();

		// ¬ключаЇмо режим роботи з матрицею проекц≥й:
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// «адаЇмо ус≥чений конус видимост≥ в л≥восторонн≥й систем≥ координат, 
		// 60 - кут видимост≥ в градусах по ос≥ у,
		// width/height - кут видимост≥ уздовж ос≥ x,
		// 1 и 100 - в≥дстань в≥д спостер≥гача до площин в≥дс≥канн€ по глибин≥:
		gluPerspective(60, width / height, 1, 100);

		// ¬ключаЇмо режим роботи з видовою матрицею:
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-0.5f, 0.0f, distZ);	// камера з початку координат зсуваЇтьс€ на distZ, 

		glRotatef(angleX, 0.0f, 1.0f, 0.0f);  // пот≥м обертаЇтьс€ по ос≥ Oy
		glRotatef(angleY, 1.0f, 0.0f, 0.0f);  // пот≥м обертаЇтьс€ по ос≥ Ox 
		
		glEnable(GL_DEPTH_TEST);	// включаЇмо буфер глибини (дл€ в≥дс≥канн€ невидимих частин зображенн€)
									// ¬ключаЇмо режим дл€ установки осв≥тленн€:
		glEnable(GL_LIGHTING);

		// ƒодаЇмо джерело св≥тла є 0 (њх може бути до 8), зараз воно св≥тить з "очей":
		glEnable(GL_LIGHT0);

		if (currentMode == gameMode) {
			// ћалюЇмо ус≥ ф≥гури:
			for (int i = 0; i < shapes.size(); i++) {
				shapes[i]->draw();
			}
		}
		// ¬имикаЇмо все, що включили:
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		// ƒл€ в≥дображенн€ тексту, краще використовувати ортограф≥чну проекц≥ю:
		glOrtho(0, 1, 0, 1, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glColor3f(1, 1, 0); // жовтий текст
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
		// показуЇмо в≥кно:
		glutSwapBuffers(); // перемиканн€ буфер≥в
	}
	// ќброблювач под≥њ, пов'€заноњ з≥ зм≥ною розм≥р≥в в≥кна 
	void Scene::on_size(int width, int height) {
		this->width = width;
		if (height == 0)
			height = 1;
		this->height = height;
	}
	// ќброблювач под≥й, пов'€заних з натисканн€м кнопок миш≥
	void Scene::on_mouse(int button, int state, int x, int y) {
		if (currentMode == gameMode) {
			// «бер≥гаЇмо поточн≥ координати миш≥:
			mouseX = x;
			mouseY = y;
			if (state == GLUT_UP && !promotionMode && !mateOccurredB && !mateOccurredW) { // кнопка в≥джата
				findNearest(x, y, mouseXCell, mouseZCell); // «находимо координати пол€, на €ке натиснув гравець
				// ¬≥джата кнопка була л≥вою
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
						// якщо ф≥гура намагаЇтьс€ перем≥ститис€ за меж≥ дошки, повертаЇмо њњ обратно
						if (mouseXCell == -1 || mouseZCell == -1 || currentZ == -1 || currentX == -1) {
							if (zStart != -1 && xStart != -1 && currentZ != -1 && currentX != -1) {
								pieces[fields[currentZ][currentX]]->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
							}
							this->button = -1;
							return;
						}
						// якщо поточне поле не порожнЇ та координати початку руху не в≥дпов≥дають координатам к≥нц€ руху
						if (fields[currentZ][currentX] != -1 && (zStart != mouseZCell || xStart != mouseXCell)) {
							Piece* p = pieces[fields[currentZ][currentX]]; // ¬каз≥вник на поточну ф≥гуру
							bool hit = false; // «м≥нна, що сигнал≥зуЇ, ставс€ удар по ворож≥й ф≥гур≥ чи н≥
							bool castlingOccurred = false; // «м≥нна, що сигнал≥зуЇ, сталос€ рок≥руванн€ чи н≥
							bool enPassantOccurred = false; // «м≥нна, що сигнал≥зуЇ, сталос€ вз€тт€ на прозод≥ чи н≥
							bool promotionOccurred = false; // «м≥нна, що сигнал≥зуЇ, сталос€ перетворенн€ п≥шака чи н≥
							// якщо була обрана €кась ф≥гура та перем≥стилас€ (не за меж≥ дошки), то виконуЇмо подальщ≥ перев≥рки
							if (mouseZCell != -1 && mouseXCell != -1) {
								// якщо поточна ф≥гура - п≥шак, перев≥р€Їмо чи сталос€ вз€тт€ на проход≥
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
										// ѕерев≥р€Їмо шахи п≥сл€ вз€тт€ на проход≥
										checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
										checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
										whiteMove = !whiteMove;
										moves++;
									}
								}
								// ѕерев≥рка на кол≥з≥ю з ≥ншою ф≥гурою
								if (fields[mouseZCell][mouseXCell] != -1 && pieces[fields[mouseZCell][mouseXCell]] != p) {
									// якщо кол≥р поточноњ ф≥гури б≥лий ≥ зараз х≥д б≥лих
									if (p->getColor() == 'W' && whiteMove) {
										// якщо рок≥руванн€ ще не сталос€, ≥ поточна ф≥гура король, перев≥р€Їмо рок≥руванн€
										if (!castlingOccurredW && typeid(*p) == typeid(King)) {
											castlingOccurredW = static_cast<King*>(p)->isCastlingOccurred(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX);
											castlingOccurred = castlingOccurredW;
											hit = castlingOccurredW;
										}
										// якщо кол≥р поточноњ ф≥гури б≥лий, зараз х≥д б≥лих ≥ ф≥гура, €ку хочемо побити, чорна, то перев≥р€Їмо удар по чорн≥й
										if (pieces[fields[mouseZCell][mouseXCell]]->getColor() == 'B' && !castlingOccurred) {
											// якщо удар коректний, тобто гравець правильно перем≥стив ф≥гуру дл€ удару
											if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
												// «апам'€товуЇмо стан шаху
												int actualCheckW = checkW;
												// –обимо удар та перев≥р€Їмо королей на шах п≥сл€ цього удару
												int k = fields[mouseZCell][mouseXCell];
												fields[mouseZCell][mouseXCell] = fields[currentZ][currentX];
												fields[currentZ][currentX] = -1;
												checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
												checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
												fields[currentZ][currentX] = fields[mouseZCell][mouseXCell];
												fields[mouseZCell][mouseXCell] = k;
												// якщо п≥сл€ удару б≥лих, б≥л≥ опин€ютьс€ п≥д шахом, скасовуЇмо цей удар
												if (checkW != -1) {
													// якщо б≥л≥ до цього вже були п≥д шахом, повертаЇмо ≥дентиф≥катор ф≥гури, €ка оголосила шах б≥лим, на значенн€, €ке було до удару
													if (actualCheckW != -1) {
														checkW = actualCheckW;
													}
													// якщо б≥л≥ до цього не були п≥д шахом, ставимо ≥дентиф≥катор ф≥гури, що оголосила шах б≥лим р≥вним - 1, тобто шаху немаЇ
													else {
														checkW = -1;
													}
													// —тавимо ≥дентиф≥катор ф≥гури, що оголосила шах чорним р≥вним -1, тобто шаху немаЇ, оск≥льки не може бути одночасно два шаха
													checkB = -1;
													hit = false;
												}
												// якщо п≥сл€ удару шаху немаЇ, проводимо цей удар
												else {
													pieces[fields[mouseZCell][mouseXCell]]->setZCenter(4 - (delB / 3));
													pieces[fields[mouseZCell][mouseXCell]]->setXCenter(-5 - delB++ % 3);
													pieces[fields[mouseZCell][mouseXCell]]->setBeaten(true);
													p->movePieceToPosition(fields, mouseZCell, mouseXCell, currentZ, currentX);
												}
											}
										}
									}
									// якщо кол≥р поточноњ ф≥гури чорний ≥ зараз х≥д чорних
									else if (p->getColor() == 'B' && !whiteMove) {
										// якщо рок≥руванн€ ще не сталос€, ≥ поточна ф≥гура король, перев≥р€Їмо рок≥руванн€
										if (!castlingOccurredB && typeid(*p) == typeid(King)) {
											castlingOccurredB = static_cast<King*>(p)->isCastlingOccurred(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX);
											castlingOccurred = castlingOccurredB;
											hit = castlingOccurredB;
										}
										// якщо кол≥р поточноњ ф≥гури чорний, зараз х≥д чорних ≥ ф≥гура, €ку хочемо побити, б≥ла, то перев≥р€Їмо удар по б≥л≥й
										if (pieces[fields[mouseZCell][mouseXCell]]->getColor() == 'W' && !castlingOccurred) {
											// якщо удар коректний, тобто гравець правильно перем≥стив ф≥гуру дл€ удару
											if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
												// «апам'€товуЇмо стан шаху
												int actualCheckB = checkB;
												// –обимо удар та перев≥р€Їмо королей на шах п≥сл€ цього удару
												int k = fields[mouseZCell][mouseXCell];
												fields[mouseZCell][mouseXCell] = fields[currentZ][currentX];
												fields[currentZ][currentX] = -1;
												checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
												checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
												fields[currentZ][currentX] = fields[mouseZCell][mouseXCell];
												fields[mouseZCell][mouseXCell] = k;
												// якщо п≥сл€ удару чорних, чорн≥ опин€ютьс€ п≥д шахом, скасовуЇмо цей удар
												if (checkB != -1) {
													// якщо чорн≥ до цього вже були п≥д шахом, повертаЇмо ≥дентиф≥катор ф≥гури, €ка оголосила шах чорним, на значенн€, €ке було до удару
													if (actualCheckB != -1) {
														checkB = actualCheckB;
													}
													// якщо чорн≥ до цього не були п≥д шахом, ставимо ≥дентиф≥катор ф≥гури, що оголосила шах чорним р≥вним -1, тобто шаху немаЇ
													else {
														checkB = -1;
													}
													// —тавимо ≥дентиф≥катор ф≥гури, що оголосила шах б≥лим р≥вним -1, тобто шаху немаЇ, оск≥льки не може бути одночасно два шаха
													checkW = -1;
													hit = false;
												}
												// якщо п≥сл€ удару шаху немаЇ то проводимо цей удар
												else {
													pieces[fields[mouseZCell][mouseXCell]]->setZCenter(-3 + (delW / 3));
													pieces[fields[mouseZCell][mouseXCell]]->setXCenter(6 + delW++ % 3);
													pieces[fields[mouseZCell][mouseXCell]]->setBeaten(true);
													p->movePieceToPosition(fields, mouseZCell, mouseXCell, currentZ, currentX);
												}
											}
										}
									}
									// —касуванн€ перем≥щенн€, €кщо удар або рок≥руванн€ не сталос€
									if (!hit && (zStart != mouseZCell || xStart != mouseXCell)) {
										p->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
									}
									// якщо сталос€ рок≥руванн€ або удар
									else if (castlingOccurred || hit) {
										moves++;
										// якщо б≥лий король п≥д шахом перев≥р€Їмо чи ставс€ мат, €кщо так, тод≥ парт≥ю ок≥нчено, записуЇмо парт≥ю до таблиц≥ рекорд≥в
										if (checkW != -1 && static_cast<King*>(pieces[28])->isMateOccurred(pieces, fields)) {
											finish = mateOccurredW = true;
											winner = "winner - " + blackPlayerName;
											addRecord();
										}
										// якщо чорний король п≥д шахом перев≥р€Їмо чи ставс€ мат, €кщо так, тод≥ парт≥ю ок≥нчено, записуЇмо парт≥ю до таблиц≥ рекорд≥в
										else if (checkB != -1 && static_cast<King*>(pieces[4])->isMateOccurred(pieces, fields)) {
											finish = mateOccurredB = true;
											winner = "winner - " + whitePlayerName;
											addRecord();
										}
										// якщо ан≥ чорний, ан≥ б≥лий король не п≥д шахом, перев≥р€Їмо, чи ставс€ пат
										else if (finish = stalemateOccurredB = static_cast<King*>(pieces[4])->isStalemateOccurred(pieces, fields)) {
											winner = "draw";
											addRecord();
										}
										else if (finish = stalemateOccurredW = static_cast<King*>(pieces[28])->isStalemateOccurred(pieces, fields)) {
											winner = "draw";
											addRecord();
										}
										// якщо мата та пата не сталос€, х≥д переходить до противника
										else {
											if (p->getFirstMove()) {
												p->setFirstMove(false);
											}
											whiteMove = !whiteMove;
											// якщо поточна ф≥гура - п≥шак, перев≥р€Їмо чи сталос€ перетворенн€ п≥шака
											if (typeid(*p) == typeid(Pawn)) {
												// якщо перетворенн€ п≥шака сталос€ ≥ жоден король п≥д шахом, то ставимо режим перетворенн€ п≥шака
												if (promotionOccurred = static_cast<Pawn*>(p)->isPromotionOccurred(mouseZCell, mouseXCell) && checkW == -1 && checkB == -1) {
													promotionMode = 1;
												}
											}
										}
									}
								}
								else { // ѕерев≥рка коректност≥ перем≥щенн€
									if ((p->getColor() == 'W' && whiteMove) || (p->getColor() == 'B' && !whiteMove)) {
										// якщо такий х≥д можливий
										if (p->isMovePossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
											// «апам'€товуЇмо стан шаху б≥лого та чорного королей
											int actualCheckW = checkW, actualCheckB = checkB;
											// ѕерев≥рка на шах дл€ обох королей
											checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
											checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
											// якщо король оголосив шах ≥ншому королю, це неправильно, тому скасовуЇмо перем≥щенн€
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
											// якщо б≥лий король п≥д шахом перев≥р€Їмо чи ставс€ мат, €кщо так, тод≥ парт≥ю зак≥нчено, записуЇмо парт≥ю до таблиц≥ рекорд≥в
											else if (checkW != -1 && static_cast<King*>(pieces[28])->isMateOccurred(pieces, fields)) {
												finish = mateOccurredW = true;
												moves++;
												winner = "winner - " + blackPlayerName;
												addRecord();
											}
											// якщо чорний король п≥д шахом перев≥р€Їмо чи ставс€ мат, €кщо так, тод≥ парт≥ю ок≥нчено, записуЇмо парт≥ю до таблиц≥ рекорд≥в
											else if (checkB != -1 && static_cast<King*>(pieces[4])->isMateOccurred(pieces, fields)) {
												finish = mateOccurredB = true;
												moves++;
												winner = "winner - " + whitePlayerName;
												addRecord();
											}
											// якщо мат не ставс€ та п≥сл€ перем≥щенн€ чорний король знаходитьс€ п≥д шахом, то скасовуЇмо це перем≥щенн€
											else if (checkB != -1 && p->getColor() == 'B') {
												// якщо чорн≥ до цього вже були п≥д шахом, повертаЇмо ≥дентиф≥катор ф≥гури, €ка оголосила шах чорним, на значенн€, €ке було до удару
												if (actualCheckB != -1) {
													checkB = actualCheckB;
												}
												// якщо чорн≥ до цього не були п≥д шахом, ставимо ≥дентиф≥катор ф≥гури, що оголосила шах чорним р≥вним -1, тобто шаху немаЇ
												else {
													checkB = -1;
												}
												p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
											}
											// якщо мат не ставс€ та п≥сл€ перем≥щенн€ б≥лий король знаходитьс€ п≥д шахом, то скасовуЇмо це перем≥щенн€
											else if (checkW != -1 && p->getColor() == 'W') {
												// якщо б≥л≥ до цього вже були п≥д шахом, повертаЇмо ≥дентиф≥катор ф≥гури, €ка оголосила шах б≥лим, на значенн€, €ке було до перем≥щенн€
												if (actualCheckW != -1) {
													checkW = actualCheckW;
												}
												// якщо б≥л≥ до цього не були п≥д шахом, ставимо ≥дентиф≥катор ф≥гури, що оголосила шах б≥лим р≥вним - 1, тобто шаху немаЇ
												else {
													checkW = -1;
												}
												p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
											}
											// якщо перем≥щенн€ сталос€ та н≥€кий король не п≥д шахом
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
													// якщо поточна ф≥гура - п≥шак, перев≥р€Їмо чи сталос€ перетворенн€ п≥шака
													if (typeid(*p) == typeid(Pawn)) {
														// якщо перетворенн€ п≥шака сталос€ ≥ жоден король п≥д шахом, то ставимо режим перетворенн€ п≥шака
														if (promotionOccurred = static_cast<Pawn*>(p)->isPromotionOccurred(mouseZCell, mouseXCell) && checkW == -1 && checkB == -1) {
															promotionMode = 1;
															checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
															checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
														}
													}
												}
											}
										}
										// якщо х≥д не можливий, скасовуЇмо перем≥щенн€
										else {
											p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
										}
									}
								}
							}
							prevPieceId = p->getId(); // «апам'товуЇмо ≥дентиф≥катор ф≥гури, що ходила до цього
							if (whiteMove) { // якщо зараз х≥д б≥лих, встановлюЇмо такий ракурс камери
								angleX = 0;
								angleY = angleY < 0 ? -angleY : angleY;
							}
							else { // якщо зараз х≥д чорних, встановлюЇмо такий ракурс камери
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
				this->button = -1;  // н≥€ка кнопка не натиснута
				return;
			}
			this->button = button;  // збер≥гаЇмо ≥нформац≥ю про кнопки
			// якщо поточн≥ координати ф≥гури не за межами дошки та не ставс€ мат
			if (findNearest(x, y, currentX, currentZ) && currentX != -1 && currentZ != -1 && !mateOccurredB && !mateOccurredW) {
				// якщо натиснута л≥ва кнопка та поточне поле не порожнЇ ≥ не в≥мкнений режим перетворенн€ п≥шака
				if (button == 0 && fields[currentZ][currentX] != -1 && !promotionMode) {
					// ѕ≥дн≥маЇмо ф≥гуру
					upPiece(currentX, currentZ);
					xStart = currentX;
					zStart = currentZ;
				}
			}
		}
	}
	void Scene::on_motion(int x, int y) {
		// якщо поточний режим сцени - ≥гровий режим
		if (currentMode == gameMode) {
			switch (button) {
				// якщо миша перем≥щаЇтьс€ ≥з зажатою л≥вою кнопкою
			case 0:
				// якщо поточн≥ координати не за межами дошки
				if (currentX != -1 && currentZ != -1) {
					Piece* p = pieces[fields[currentZ][currentX]];
					// якщо в≥мкнений режим розм≥щенн€ ф≥гур
					if (editBoardMode) {
						// якщо поточне поле не порожнЇ
						if (findNearest(x, y, xTo, zTo) && fields[currentZ][currentX] != -1) {
							// якщо перем≥щенн€ можливе, перезаписуЇмо поточн≥ координати ф≥гури
							if (movePiece(currentZ, currentX, zTo, xTo)) {
								currentX = xTo;
								currentZ = zTo;
							}
						}
					}
					// якщо зараз х≥д б≥лих та поточна ф≥гура  -б≥ла, або зараз х≥д чорних та поточна ф≥гура - чорна
					else if ((whiteMove && p->getColor() == 'W') || (!whiteMove && p->getColor() == 'B')) {
						// якщо поточне поле не порожнЇ, не в≥мкнений режим перетворенн€ п≥шака та не ставс€ мат
						if (findNearest(x, y, xTo, zTo) && fields[currentZ][currentX] != -1 && !promotionMode && !mateOccurredB && !mateOccurredW) {
							// якщо перем≥щенн€ можливе, перезаписуЇмо поточн≥ координати ф≥гури
							if (movePiece(currentZ, currentX, zTo, xTo)) {
								currentX = xTo;
								currentZ = zTo;
							}
						}
					}
				}
				break;
				// якщо миша перем≥щаЇтьс€ ≥з зажатою правою кнопкою
			case 2: // права кнопка - обертанн€ сцени
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
			case GLUT_KEY_UP:   // наближенн€
				if (distZ > -8) {
					break;
				}
				distZ += 0.3;
				break;
			case GLUT_KEY_DOWN: // в≥ддаленн€
				if (distZ <= -17) {
					break;
				}
				distZ -= 0.3;
				break;
			case GLUT_KEY_F1: // почати гру
				if (editBoardMode) {
					editBoardMode = false;
					checkConditions();
				}
				break;
			case GLUT_KEY_F2:   // нова гра
				initialize();
				break;
			case GLUT_KEY_F3: // виб≥р складност≥
				currentMode = selectDifficultyMode;
				break;
			case GLUT_KEY_F4: // зм≥на ≥м'€
				currentMode = inputFirstPlayerNameMode;
				break;
			case GLUT_KEY_F5: // таблиц€ рекорд≥в
				readRecords(); // зчитуЇмо рекорди з файлу
				currentMode = recordsMode;
				break;
			}
		}
	}

	void Scene::on_keyboard(unsigned char key, int x, int y) {
		if (currentMode == inputFirstPlayerNameMode) { // введенн€ ≥мен≥ першого гравц€
			switch (key) {
			case 8: // €кщо натиснута клав≥ша - backspace, то видал€Їмо останн≥й елемент строки
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
			case 13: // €кщо натиснута клав≥ша - enter, то переходимо до введенн€ ≥мен≥ другого гравц€
				if (firstPlayerName.size() > 0) {
					currentMode = inputSecondPlayerNameMode;
				}
				break;
			case 27: // €кщо натиснута клав≥ша - escape, то або повертаЇмось до гри або виходимо з гри
				if (!firstInit) {
					currentMode = gameMode;
				}
				else {
					exit(0);
				}
				break;
			default: // €кщо натиснута клав≥ша - це латинська буква, добавл€Їмо њњ в к≥нець строки
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
		else if (currentMode == inputSecondPlayerNameMode) { // введенн€ ≥мен≥ другого гравц€
			switch(key) {
			case 8: // €кщо натиснута клав≥ша - backspace, то видал€Їмо останн≥й елемент строки
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
			case 13: // €кщо натиснута клав≥ша - enter, то переходимо до гри або вибору р≥вню складност≥
				if (secondPlayerName.size() > 0) {
					if (firstInit) {
						currentMode = selectDifficultyMode;
					}
					else {
						currentMode = gameMode;
					}
				}
				break;
			case 27: // €кщо натиснута клав≥ша - escape, то повертаЇмось до редагуванн€ ≥мен≥ першого гравц€
				currentMode = inputFirstPlayerNameMode;
				break;
			default: // €кщо натиснута клав≥ша - це латинська буква, добавл€Їмо њњ в к≥нець строки
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
		else if (currentMode == selectDifficultyMode) { // виб≥р р≥вню складност≥
			switch (key) {
			case '1': // €кщо натиснута клав≥ша 1
				if (difficulty != 1) { // €кщо р≥вень складност≥ до цього не був першим, то ≥н≥ц≥ал≥зуЇмо гру заново
					initialize();
				}
				difficulty = 1; // ставимо р≥вень складност≥ р≥вним одиниц≥
				firstInit = false;
				currentMode = gameMode; // переходимо до гри
				break;
			case '2': // €кщо натиснута клав≥ша 2
				if (difficulty != 2) { // €кщо р≥вень складност≥ до цього не був другим, то ≥н≥ц≥ал≥зуЇмо гру заново
					difficulty = 2; // ставимо р≥вень складност≥ р≥вним дв≥йц≥
					initialize(); // переходимо до гри
				}
				firstInit = false;
				currentMode = gameMode;
				break;
			case '3': // €кщо натиснута клав≥ша 3
				if (difficulty != 3) { // €кщо р≥вень складност≥ до цього не був трет≥м, то ≥н≥ц≥ал≥зуЇмо гру заново
					difficulty = 3; // ставимо р≥вень складност≥ р≥вним тр≥йц≥
					initialize();
				}
				firstInit = false;
				currentMode = gameMode; // переходимо до гри
				break;
			case 27: // €кщо натиснута клав≥ша - escape, то або повертаЇмось до введенн€ ≥мен≥ другого гравц€ або повертаЇмос€ до гри
				if (!firstInit) {
					currentMode = gameMode;
				}
				else {
					currentMode = inputSecondPlayerNameMode;
				}
				break;
			}
		}
		else if (currentMode == recordsMode) { // режим перегл€ду таблиц≥ рекорд≥в
			if (key == 27) { // €кщо натиснута клав≥ша - escape, то повертаЇмос€ до гри
				currentMode = gameMode;
			}
			else if (key == 'c' || key == 'C') { // €кщо натиснута клав≥ша C
				clearRecords(); // очищаЇмо таблицю рекорд≥в
			}
		}
		else if (currentMode == gameMode) { // режим гри
			if (promotionMode) { // €кщо стоњть режим перетворенн€ п≥шака
				switch (key) {
				case 'q': // €кщо натиснута клав≥ша q
					promotionMode = 2;
					break;
				case 'b': // €кщо натиснута клав≥ша b
					promotionMode = 3;
					break;
				case 'k': // €кщо натиснута клав≥ша k
					promotionMode = 4;
					break;
				case 'r': // €кщо натиснута клав≥ша r
					promotionMode = 5;
					break;
				}
			}
			if (key == 27) { // €кщо натиснута клав≥ша - escape, то виходимо з гри
				exit(0);
			}
		}
	}

	int tick = 0; // л≥чильник, значенн€ €кого зм≥нюЇтьс€ кожн≥ 25 мс

				  // ќброблювач под≥њ в≥д таймера
	void Scene::on_timer(int value)
	{
		tick++;
		// нарахували наступну секунду
		if (tick >= 40) {
			// €кщо парт≥ю ще не зак≥нчено
			if (!finish && !editBoardMode && currentMode == gameMode) {
				// зб≥льшуЇмо час, що пройшов в≥д початку парт≥њ
				time++;
				if (difficulty != 1) {
					// час гравц€ спливаЇ, €кщо зараз його х≥д
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
			tick = 0;   // скинули л≥чильник
		}
		on_paint();     // зд≥йснюЇмо перемалюванн€ в≥кна
	}
}