#ifndef Pawn_h
#define Pawn_h

#include "Piece.h"

namespace ChessGame {
	class Pawn : public Piece {
	public:
		Pawn(int id, char color, float xCenter, float yCenter, float zCenter,
			float xSize, float ySize, float zSize,
			float* diffColor, float* ambiColor, float* specColor, float shininess, float radius)
			: Piece(id, color, xCenter, yCenter, zCenter,
				xSize, ySize, zSize,
				diffColor, ambiColor, specColor, shininess, radius) { }

		virtual bool isMovePossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) override;
		virtual bool Pawn::isHitPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) override;
		virtual bool check(Piece* pieces[32], int(&fields)[8][8]) override;
		virtual bool capture(Piece* pieces[32], int(&fields)[8][8], int capturePieceId) override;
		// Перевіряє, чисталося взяття на проході
		bool isEnPassantOccurred(Piece* pieces[32], int(&fields)[8][8], int prevPieceId, int mouseZCell, int mouseXCell);
		// Перевіряє, чи сталося перетворення пішака
		bool isPromotionOccurred(int mouseZCell, int mouseXCell);
	};
}

#endif