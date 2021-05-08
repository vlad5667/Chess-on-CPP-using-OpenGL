#include "Rook.h"
#include <cmath>

namespace ChessGame {
	bool Rook::correctMove(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		if (abs(mouseZCell - zStart) != 0 && abs(mouseXCell - xStart) == 0) {
			if (mouseZCell < zStart) {
				for (int z = zStart - 1; z > mouseZCell; z--) {
					if (static_cast<Rook*>(pieces[fields[z][xStart]]) != this && fields[z][xStart] != -1) {
						goto skipRook;
					}
				}
			}
			else {
				for (int z = zStart + 1; z < mouseZCell; z++) {
					if (static_cast<Rook*>(pieces[fields[z][xStart]]) != this && fields[z][xStart] != -1) {
						goto skipRook;
					}
				}
			}
			return true;
		}
		else if (abs(mouseZCell - zStart) == 0 && abs(mouseXCell - xStart) != 0) {
			if (mouseXCell < xStart) {
				for (int x = xStart - 1; x > mouseXCell; x--) {
					if (static_cast<Rook*>(pieces[fields[zStart][x]]) != this && fields[zStart][x] != -1) {
						goto skipRook;
					}
				}
			}
			else {
				for (int x = xStart + 1; x < mouseXCell; x++) {
					if (static_cast<Rook*>(pieces[fields[zStart][x]]) != this && fields[zStart][x] != -1) {
						goto skipRook;
					}
				}
			}
			return true;
		}
		else {
		skipRook:
			movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
			return false;
		}
	}
	bool Rook::isHitPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell, int currentZ, int currentX) {
		// Перевірка можливості побити ворожу фігуру
		if (abs(mouseZCell - zStart) != 0 && abs(mouseXCell - xStart) == 0) {
			if (mouseZCell < zStart) {
				for (int z = zStart - 1; z > mouseZCell; z--) {
					if (static_cast<Rook*>(pieces[fields[z][xStart]]) != this && fields[z][xStart] != -1) {
						return false;
					}
				}
				return true;
			}
			else {
				for (int z = zStart + 1; z < mouseZCell; z++) {
					if (static_cast<Rook*>(pieces[fields[z][xStart]]) != this && fields[z][xStart] != -1) {
						return false;
					}
				}
				return true;
			}
		}
		else if (abs(mouseZCell - zStart) == 0 && abs(mouseXCell - xStart) != 0) {
			if (mouseXCell < xStart) {
				for (int x = xStart - 1; x > mouseXCell; x--) {
					if (static_cast<Rook*>(pieces[fields[zStart][x]]) != this && fields[zStart][x] != -1) {
						return false;
					}
				}
				return true;
			}
			else {
				for (int x = xStart + 1; x < mouseXCell; x++) {
					if (static_cast<Rook*>(pieces[fields[zStart][x]]) != this && fields[zStart][x] != -1) {
						return false;
					}
				}
				return true;
			}
		}
		return false;
	}
	int Rook::isCheckOccured(Piece* pieces[32], int(&fields)[8][8]) {
		return 0;
	}
}