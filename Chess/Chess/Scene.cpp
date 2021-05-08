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

		// Устанавливаем параметры наложения и фильтрации текстур (для текущего связанного объекта текстуры)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Загрузка и генерация текстуры
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

	// collison сигналізує про колізію з іншою фігурою, outOfBoard сигналізує про вихід за межі дошки
	bool collision = false;
	int delW = 0, delB = 0;
	bool castlingOccuredW = false;
	bool castlingOccuredB = false;
	int checkW = -1, checkB = -1;
	Scene::Scene() {
		GLuint texture = 0;
		int k = 0;
		// Додаємо дошку для шахів
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
				//Паралельно додаємо фігури на дошку
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
	// Переміщення фігури зі вказаної позиції на нову
	bool Scene::movePiece(int currentZ, int currentX, int zTo, int xTo) {
		// Перевірка можливості переміщення:
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
	// Пошук стрижня, найближчого до позиції курсору миші:
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
		char text[128];
		if (whiteMove) {
			strcpy_s(text, 128, "White to move");
		} else {
			strcpy_s(text, 128, "Black to move");
		}
		//sprintf(text, "F2 - Restart game   Esc - Exit              Time: %d sec.", time);
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
		drawString(GLUT_BITMAP_TIMES_ROMAN_24, text, 0.01, 0.95);
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

		// Малюємо усі фігури:
		for (int i = 0; i < shapes.size(); i++) {
			shapes[i]->draw();
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
	void Scene::on_size(int width, int height)
	{
		this->width = width;
		if (height == 0)
			height = 1;
		this->height = height;
	}
	// Оброблювач подій, пов'язаних з натисканням кнопок миші
	void Scene::on_mouse(int button, int state, int x, int y)
	{
		// Зберігаємо поточні координати миші:
		mouseX = x;
		mouseY = y;
		if (state == GLUT_UP) { // кнопка віджата
			// Віджата кнопка була лівою
			if (button == 0) {
				//std::cout << zStart << " " << currentZ << " " << xStart << " " << currentX << " " << mouseZCell << " " << mouseXCell  << "\n";
				downAllPieces();
				findNearest(x, y, mouseXCell, mouseZCell);
				Piece* p = pieces[fields[currentZ][currentX]]; // Вказівник на поточну фігуру
				bool hit = false; // Змінна, що сигналізує, стався удар по ворожій фігурі чи ні
				bool castlingOccured = false; // Змінна, що сигналізує, сталося рокірування чи ні
				// Якщо фігура намагається переміститися за межі дошки, повертаємо її обратно
				if (mouseXCell == -1 || mouseZCell == -1) {
					std::cout << "mouseXCell " << mouseXCell << " mouseZCell " << mouseZCell << "\n";
					std::cout << "xStart " << xStart << " zStart " << zStart << " currentX " << currentX << " currentZ " << currentZ << "\n";
					if (zStart != -1 && xStart != -1 && currentZ != -1 && currentX != -1 && pieces[fields[currentZ][currentX]] == p) {
						p->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
					}
					this->button = -1;
					return;
					// Якщо була обрана якась фігура та перемістилася (не за межі дошки), то виконуємо подальщі перевірки
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
						// Перевірка на колізію з іншою фігурою
						else if (collision && fields[mouseZCell][mouseXCell] != -1 && pieces[fields[mouseZCell][mouseXCell]] != p) {
							if (p->getColor() == 'W' && whiteMove) {
								if (!castlingOccuredW && typeid(*p) == typeid(King)) {
										castlingOccuredW = static_cast<King*>(p)->isCastlingPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX);
										castlingOccured = castlingOccuredW;
										hit = castlingOccuredW;
								}
								// Якщо колір поточної фігури білий, зараз хід білих і фігура, яку хочемо побити, чорна, то перевіряємо удар по чорній
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
								// Якщо колір поточної фігури чорний, зараз хід чорних і фігура, яку хочемо побити, біла, то перевіряємо удар по білій
								if (pieces[fields[mouseZCell][mouseXCell]]->getColor() == 'W') {
									if (hit = p->isHitPossible(pieces, fields, zStart, xStart, mouseZCell, mouseXCell, currentZ, currentX)) {
										pieces[fields[mouseZCell][mouseXCell]]->setXCenter(6 + delB++);
										pieces[fields[mouseZCell][mouseXCell]]->setZCenter(-3);
										p->movePieceToPosition(fields, mouseZCell, mouseXCell, currentZ, currentX);
									}
								}
							}
							// Скасування переміщення, якщо удар або рокірування не сталося
							if (!hit && (zStart != mouseZCell || xStart != mouseXCell)) {
								p->movePieceToPosition(fields, zStart, xStart, currentZ, currentX);
							}
							else if (castlingOccured || hit) {
								whiteMove = !whiteMove;
							}
							collision = false;
						}
						else { // Перевірка коректності переміщення
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
			this->button = -1;  // ніяка кнопка не натиснута
			return;
		}
		this->button = button;  // зберігаємо інформацію про кнопки
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
		case 2: // права кнопка - обертання сцени
			angleX += x - mouseX;
			angleY += y - mouseY;
			mouseX = x;
			mouseY = y;
			break;
		}
	}

	void Scene::on_special(int key, int x, int y) {
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

	int tick = 0; // лічильник, значення якого змінюється кожні 25 мс

				  // Оброблювач події від таймера
	void Scene::on_timer(int value)
	{
		tick++;
		if (tick >= 40) // нарахували наступну секунду
		{
			//if (!finish)// секунди нарощуються, якщо гру не закінчено
			//{
			//	time++;
			//}
			tick = 0;   // скинули лічильник
		}
		on_paint();     // здійснюємо перемалювання вікна
	}
}