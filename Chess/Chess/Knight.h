#ifndef Knight_h
#define Knight_h

#include "Piece.h"

namespace ChessGame {
    class Knight : public Piece {
	public:
		Knight(int id, char color, float xCenter, float yCenter, float zCenter,
			float xSize, float ySize, float zSize,
			float* diffColor, float* ambiColor, float* specColor, float shininess, float radius)
			: Piece(id, color, xCenter, yCenter, zCenter,
				xSize, ySize, zSize,
				diffColor, ambiColor, specColor, shininess, radius) { }

		virtual bool correctMove(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) override;
		virtual bool isHitPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) override;
		virtual int check(Piece* pieces[32], int(&fields)[8][8]) override;
	};
}

#endif