#ifndef Pawn_h
#define Pawn_h

#include "Piece.h"

namespace ChessGame {
	class Pawn : public Piece {
	public:
		Pawn(char color, float xCenter, float yCenter, float zCenter,
			float xSize, float ySize, float zSize,
			float* diffColor, float* ambiColor, float* specColor, float shininess, float radius, GLuint texture)
			: Piece(color, xCenter, yCenter, zCenter,
				xSize, ySize, zSize,
				diffColor, ambiColor, specColor, shininess, radius, texture) { }

		virtual bool correctMove(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) override;
		virtual bool Pawn::isHitPossible(Piece *pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell, int currentZ, int currentX) override;
		virtual int isCheckOccured(Piece* pieces[32], int(&fields)[8][8]) override;
	};
}

#endif