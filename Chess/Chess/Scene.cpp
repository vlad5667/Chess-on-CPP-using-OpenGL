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
	std::string firstPlayerName = "", secondPlayerName = ""; // Змінні, що зберігають імена гравців
	std::string whitePlayer, blackPlayer;

	bool inputFirstPlayerNameMode = true; // Змінна, що сигналізує про режим введення імені першого гравця
	bool inputSecondPlayerNameMode = false; // Змінна, що сигналізує про режим введення імені другого гравця
	bool selectDifficultyMode = false; // Змінна, що сигналізує про режим вибору складності
	bool gameMode = false; // Змінна, що сигналізує про режим гри

	int difficulty; // Змінна, що вказує на складність гри

	bool collision = false; // Змінна, що сигналізує про колізію з іншою фігурою
	int delW = 0, delB = 0; // Змінні, що задають координату Х для побитих фігур
	bool castlingOccuredW = false; // Змінна, що сигналізує, що сталося рокірування білих
	bool castlingOccuredB = false; // Змінна, що сигналізує, що сталося рокірування чорних
	int checkW = -1, checkB = -1; // Змінні, що сигналізують, стався шах чи ні
	bool mateOccuredW = false, mateOccuredB = false; // Змінні, що сигналізують, стався мат чи ні
	int prevPieceId = 0; // Змінна, що зберігає ідентифікатор фігура, що ходила до цього (потрібно для взяття на проході)
	int promotionMode = 0; // Змінна, що сигналізує про вибір фігури для перетворення пішака

	Scene::Scene() {
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

		
		glClearColor(0, 0.5, 0.5, 0);

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

		if (gameMode) {
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
		if (gameMode) {
			// Зберігаємо поточні координати миші:
			mouseX = x;
			mouseY = y;
			if (state == GLUT_UP && !promotionMode) { // кнопка віджата
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
						bool castlingOccured = false; // Змінна, що сигналізує, сталося рокірування чи ні
						bool enPassantOccured = false; // Змінна, що сигналізує, сталося взяття на прозоді чи ні
						bool promotionOccured = false; // Змінна, що сигналізує, сталося перетворення пішака чи ні
						// Якщо фігура намагається переміститися за межі дошки, повертаємо її обратно
						std::cout << "currentZ " << currentZ << "  currentX " << currentX << "\n";
						// Якщо була обрана якась фігура та перемістилася (не за межі дошки), то виконуємо подальщі перевірки
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
							// Якщо білим оголошено шах і зараз їх хід
							else if (checkW != -1 && whiteMove) {
								// Якщо сталая колізія з іншою фігурою, то перевіряємо чи можливий удар
								if (collision && fields[mouseZCell][mouseXCell] != -1 && pieces[fields[mouseZCell][mouseXCell]] != p) {
									if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
										int k = fields[mouseZCell][mouseXCell];
										fields[mouseZCell][mouseXCell] = fields[currentZ][currentX];
										fields[currentZ][currentX] = -1;
										// Якщо після удару, король всеодно знаходиться під ударом, скасовуємо удар
										checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
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
										}
									}
									else {
										p->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
									}
								}
								// Якщо фігура намагається походити, перевіряємо коректність ходу
								else if (p->isMovePossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
									// Якщо після цього ходу король не звільнився від шаху, повертаємо фігуру назад
									if (static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields) != -1) {
										p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
									}
									// Якщо після цього ходу король звільнився від шаху, тобто фігура закрила короля
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
							// Якщо чорним оголошено шах і зараз хід чорних
							else if (checkB != -1 && !whiteMove) {
								// Якщо сталая колізія з іншою фігурою, то перевіряємо чи можливий удар
								if (collision && fields[mouseZCell][mouseXCell] != -1 && pieces[fields[mouseZCell][mouseXCell]] != p) {
									if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
										int k = fields[mouseZCell][mouseXCell];
										fields[mouseZCell][mouseXCell] = fields[currentZ][currentX];
										fields[currentZ][currentX] = -1;
										// Якщо після удару, король всеодно знаходиться під ударом, скасовуємо удар
										checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
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
										}
									}
									else {
										p->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
									}
								}
								// Якщо фігура намагається походити, перевіряємо коректність ходу
								else if (p->isMovePossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
									// Якщо після цього ходу король не звільнився від шаху, повертаємо фігуру назад
									if (static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields) != -1) {
										p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
									}
									// Якщо після цього ходу король звільнився від шаху, тобто фігура закрила короля
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
							// Перевірка на колізію з іншою фігурою
							else if (collision && fields[mouseZCell][mouseXCell] != -1 && pieces[fields[mouseZCell][mouseXCell]] != p) {
								// Якщо колір поточної фігури білий і зараз хід білих
								if (p->getColor() == 'W' && whiteMove) {
									// Якщо рокірування ще не сталося, і поточна фігура король, перевіряємо рокірування
									if (!castlingOccuredW && typeid(*p) == typeid(King)) {
										castlingOccuredW = static_cast<King*>(p)->isCastlingOccured(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX);
										castlingOccured = castlingOccuredW;
										hit = castlingOccuredW;
									}
									// Якщо колір поточної фігури білий, зараз хід білих і фігура, яку хочемо побити, чорна, то перевіряємо удар по чорній
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
								// Якщо колір поточної фігури чорний і зараз хід чорних
								else if (p->getColor() == 'B' && !whiteMove) {
									// Якщо рокірування ще не сталося, і поточна фігура король, перевіряємо рокірування
									if (!castlingOccuredB && typeid(*p) == typeid(King)) {
										castlingOccuredB = static_cast<King*>(p)->isCastlingOccured(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX);
										castlingOccured = castlingOccuredB;
										hit = castlingOccuredB;
									}
									// Якщо колір поточної фігури чорний, зараз хід чорних і фігура, яку хочемо побити, біла, то перевіряємо удар по білій
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
								// Скасування переміщення, якщо удар або рокірування не сталося
								if (!hit && (zStart != mouseZCell || xStart != mouseXCell)) {
									p->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
								}
								// Якщо сталося рокірування або удар, хід переходить до противника
								else if (castlingOccured || hit) {
									// Перевірка на шах після удару чи рокірування
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
							else { // Перевірка коректності переміщення
								if ((p->getColor() == 'W' && whiteMove) || (p->getColor() == 'B' && !whiteMove)) {
									if (p->isMovePossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell)) {
										// Перевірка на шах для обох королей
										checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
										checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
										// Якщо король робить хід, який призведе до шаху, повертаємо його назад
										if (checkB != -1 && p->getColor() == 'B') {
											p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
											checkB = -1;
										}
										else if (checkW != -1 && p->getColor() == 'W') {
											p->movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
											checkW = -1;
										}
										else { // Якщо сталося успішне переміщення, хід переходить до противника
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
				this->button = -1;  // ніяка кнопка не натиснута
				return;
			}
			this->button = button;  // зберігаємо інформацію про кнопки
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
		if (gameMode) {
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

	int tick = 0; // лічильник, значення якого змінюється кожні 25 мс

				  // Оброблювач події від таймера
	void Scene::on_timer(int value)
	{
		tick++;
		// нарахували наступну секунду
		if (tick >= 40) {
			// секунди нарощуються, якщо гру не закінчено
			if (!mateOccuredB && !mateOccuredW && gameMode) {
				if (whiteMove) {
					firstPlayerTime--;
				}
				else {
					secondPlayerTime--;
				}
			}
			tick = 0;   // скинули лічильник
		}
		on_paint();     // здійснюємо перемалювання вікна
	}
}