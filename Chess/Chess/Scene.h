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
	const int M = 8, N = 8; // к≥льк≥сть р€дк≥в та колонок пол€

							// ќсновний клас гри, €кий представл€Ї геометр≥ю сцени,
							// контролюЇ правила гри, розташуванн€ елемент≥в,
							// а також реал≥зуЇ обробку под≥й GLUT
	class Scene {
		std::vector<Shape*> shapes; // "гнучкий" масив указ≥вник≥в на елементи гри
		Piece* pieces[4 * N];
		int button;           // кнопка миш≥ (-1 - не натиснута, 0 - л≥ва, 2 - права)
		float angleX, angleY; // поточний кут повороту сцени 
		float mouseX, mouseY; // поточн≥ координати миш≥
		float width, height;  // –озм≥ри в≥кна
		float distZ;          // в≥дстань по ос≥ Z до сцени
		int firstPlayerTime; // час на парт≥ю першого гравц€
		int secondPlayerTime; // час на парт≥ю другого гравц€
		int fields[N][M];
		int mouseXCell, mouseZCell; // координати кл≥тинки, на €ку вказуЇ миша, коли в≥джата кнопка
		int currentX, currentZ; // поточн≥ координати ф≥гури
		int xStart, zStart;	// координати ф≥гури до перем≥щенн€
		int xTo, zTo; // координати пункту призначенн€ ф≥гури
		bool whiteMove = true; // х≥д (false - чорн≥, true - б≥л≥)
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