#include "King.h"
#include "Rook.h"
#include <cmath>

namespace ChessGame {
	bool King::correctMove(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		if (abs(zStart - mouseZCell) > 1 || abs(xStart - mouseXCell) > 1) {
			movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
			return false;
		}
		else {
			return true;
		}
	}
	bool King::isHitPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell, int currentZ, int currentX) {
		if (abs(zStart - mouseZCell) == 1 || abs(xStart - mouseXCell) == 1) {
			return true;
		}
		return false;
	}
	bool King::isCastlingPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell, int currentZ, int currentX) {
		std::cout << getFirstMove() << std::endl;
		if (abs(zStart - mouseZCell) == 0 && (mouseXCell == 0 || mouseXCell == 7) && getFirstMove()) {
			if (typeid(*pieces[fields[mouseZCell][mouseXCell]]) == typeid(Rook) && pieces[fields[mouseZCell][mouseXCell]]->getFirstMove()) {
				if (mouseXCell < xStart) {
					for (int x = xStart - 1; x > mouseXCell; x--) {
						if (fields[zStart][x] != -1 && pieces[fields[zStart][x]] != this) {
							return false;
						}
					}
					static_cast<Rook*>(pieces[fields[mouseZCell][mouseXCell]])->movePieceToPosition(fields, zStart, xStart - 1, mouseZCell, mouseXCell);
					this->movePieceToPosition(fields, zStart, xStart - 2, currentZ, currentX);
					setFirstMove(false);
					return true;
				}
				else {
					for (int x = xStart + 1; x < mouseXCell; x++) {
						if (fields[zStart][x] != -1 && pieces[fields[zStart][x]] != this) {
							return false;
						}
					}
					static_cast<Rook*>(pieces[fields[mouseZCell][mouseXCell]])->movePieceToPosition(fields, zStart, xStart + 1, mouseZCell, mouseXCell);
					this->movePieceToPosition(fields, zStart, xStart + 2, currentZ, currentX);
					setFirstMove(false);
					return true;
				}
			}
		}
		return false;
	}
	int King::isCheckOccured(Piece* pieces[32], int(&fields)[8][8]) {
		if (this->getColor() == 'W') {
			for (int k = 0; k < 15; k++) {
				if (k != 4) {
					if (pieces[k]->isCheckOccured(pieces, fields)) {
						return k;
					}
				}
			}
		}
		else {
			for (int k = 16; k < 32; k++) {
				if (k != 28) {
					if (pieces[k]->isCheckOccured(pieces, fields)) {
						return k;
					}
				}
			}
		}
		return -1;
	}
	bool King::isMateOccured(Piece* pieces[32], int(&fields)[8][8]) {
		/*int currentZ = this->getZCenter() + 3, currentX = this->getXCenter() + 3;
		int noCheckCells = 0, k = fields[currentZ][currentX];
		if (fields[currentZ][currentX] == -1) {
			fields[currentZ + 1][currentX] = fields[currentZ][currentX];
			if (isCheckOccured)
		}*/
		return false;
	}
}