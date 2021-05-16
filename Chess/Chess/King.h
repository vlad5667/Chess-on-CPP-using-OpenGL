#ifndef King_h
#define King_h

#include "Piece.h"

namespace ChessGame {
	class King : public Piece {
	public:
		King(int id, char color, float xCenter, float yCenter, float zCenter,
			float xSize, float ySize, float zSize,
			float* diffColor, float* ambiColor, float* specColor, float shininess, float radius)
			: Piece(id, color, xCenter, yCenter, zCenter,
				xSize, ySize, zSize,
				diffColor, ambiColor, specColor, shininess, radius) { }

		virtual bool isMovePossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) override;
		virtual bool isHitPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) override;
		virtual bool check(Piece* pieces[32], int(&fields)[8][8]) override;
		virtual bool capture(Piece* pieces[32], int(&fields)[8][8], int capturePieceId) override;
		virtual bool hasMove(Piece* pieces[32], int(&fields)[8][8]) override;
		// Перевіряє, чи сталося рокірування
		bool isCastlingOccurred(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell, int currentZ, int currentX);
		// Перевіряє, чи стався шах та повертає ідентифікатор фігури, що його спричинила
		int isCheckOccurred(Piece* pieces[32], int(&fields)[8][8]);
		// Перевіряє, чи можна уйти від шаху
		bool isRetreatPossible(Piece* pieces[32], int(&fields)[8][8]);
		// Перевіряє, чи можна закрити короля від шаху союзною фігурою
		bool isSalvationPossible(Piece* pieces[32], int(&fields)[8][8], int checkPieceId);
		// Перевіряє, чи стався мат
		bool isMateOccurred(Piece* pieces[32], int(&fields)[8][8]);
		// Перевіряє, чи стався пат
		bool isStalemateOccurred(Piece* pieces[32], int(&fields)[8][8]);
	};
}

#endif