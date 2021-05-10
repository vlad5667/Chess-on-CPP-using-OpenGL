#ifndef Queen_h
#define Queen_h

#include "Piece.h"

namespace ChessGame {
    class Queen : public Piece {
	public:
		Queen(int id, char color, float xCenter, float yCenter, float zCenter,
			float xSize, float ySize, float zSize,
			float* diffColor, float* ambiColor, float* specColor, float shininess, float radius, GLuint texture)
			: Piece(id, color, xCenter, yCenter, zCenter,
				xSize, ySize, zSize,
				diffColor, ambiColor, specColor, shininess, radius, texture) { }

		virtual bool correctMove(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) override;
		virtual bool isHitPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) override;
		virtual int check(Piece* pieces[32], int(&fields)[8][8]) override;
	};
}

#endif