#include "Knight.h"
#include <cmath>

namespace ChessGame {
	bool Knight::correctMove(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		// Якщо хід коректний
		if ((abs(zStart - mouseZCell) == 1 && abs(xStart - mouseXCell) == 2)
			|| (abs(zStart - mouseZCell) == 2 && abs(xStart - mouseXCell) == 1)) {
			return true;
		}
		else { // Якщо хід некоректний
			movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
			return false;
		}
	}
	bool Knight::isHitPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell, int currentZ, int currentX) {
		// Перевірка можливості побити ворожу фігуру
		if ((abs(zStart - mouseZCell) == 1 && abs(xStart - mouseXCell) == 2)
			|| (abs(zStart - mouseZCell) == 2 && abs(xStart - mouseXCell) == 1)) {
			return true;
		}
		return false;
	}
	int Knight::isCheckOccured(Piece* pieces[32], int(&fields)[8][8]) {
		return 0;
	}
}