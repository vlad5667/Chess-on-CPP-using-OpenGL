#ifndef King_h
#define King_h

#include "Piece.h"

namespace ChessGame {
	class King : public Piece {
	public:
		King(char color, float xCenter, float yCenter, float zCenter,
			float xSize, float ySize, float zSize,
			float* diffColor, float* ambiColor, float* specColor, float shininess, float radius, GLuint texture)
			: Piece(color, xCenter, yCenter, zCenter,
				xSize, ySize, zSize,
				diffColor, ambiColor, specColor, shininess, radius, texture) { }

		virtual bool correctMove(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) override;
		virtual bool isHitPossible(Piece *pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell, int currentZ, int currentX) override;
		virtual int isCheckOccured(Piece* pieces[32], int(&fields)[8][8]) override;
		bool isCastlingPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell, int currentZ, int currentX);
	};
}

#endif