#ifndef Scene_h
#define Scene_h

#include "Shape.h"
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

#include <vector>

namespace ChessGame {
	const int M = 8, N = 8; // кількість рядків та колонок поля

							// Основний клас гри, який представляє геометрію сцени,
							// контролює правила гри, розташування елементів,
							// а також реалізує обробку подій GLUT
	class Scene {
		std::vector<Shape*> shapes; // "гнучкий" масив указівників на елементи гри
		Piece* pieces[4 * N];
		int button;           // кнопка миші (-1 - не натиснута, 0 - ліва, 2 - права)
		float angleX, angleY; // поточний кут повороту сцени 
		float mouseX, mouseY; // поточні координати миші
		float width, height;  // Розміри вікна
		float distZ;          // відстань по осі Z до сцени
		int time;
		int fields[N][M];
		int mouseXCell, mouseZCell; // координати клітинки, на яку вказує миша, коли віджата кнопка
		int currentX, currentZ; // поточні координати фігури
		int xStart, zStart;	// координати фігури до переміщення
		int xTo, zTo; // координати пункту призначення фігури
		bool whiteMove = true; // хід (false - чорні, true - білі)
	public:
		Scene();
		~Scene();
		void on_paint();
		void on_size(int width, int height);
		void on_mouse(int button, int state, int x, int y);
		void on_motion(int x, int y);
		void on_special(int key, int x, int y);
		void on_keyboard(unsigned char key, int x, int y);
		void on_timer(int value);
	private:
		void initialize();
		void upPiece(int x, int z);
		bool movePiece(int xFrom, int zFrom, int xTo, int zTo);
		void downAllPieces();
		void resetPieces();
		bool findNearest(int x, int y, int& x1, int& z1);
	};

}

#endif