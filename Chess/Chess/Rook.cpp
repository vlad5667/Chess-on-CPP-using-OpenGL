#include "Rook.h"
#include "King.h"
#include <cmath>

namespace ChessGame {
	bool Rook::isMovePossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		if (abs(mouseZCell - zStart) != 0 && abs(mouseXCell - xStart) == 0) {
			if (mouseZCell < zStart) {
				for (int z = zStart - 1; z > mouseZCell; z--) {
					if (static_cast<Rook*>(pieces[fields[z][xStart]]) != this && fields[z][xStart] != -1) {
						return false;
					}
				}
			}
			else {
				for (int z = zStart + 1; z < mouseZCell; z++) {
					if (static_cast<Rook*>(pieces[fields[z][xStart]]) != this && fields[z][xStart] != -1) {
						return false;
					}
				}
			}
			return true;
		}
		else if (abs(mouseZCell - zStart) == 0 && abs(mouseXCell - xStart) != 0) {
			if (mouseXCell < xStart) {
				for (int x = xStart - 1; x > mouseXCell; x--) {
					if (static_cast<Rook*>(pieces[fields[zStart][x]]) != this && fields[zStart][x] != -1) {
						return false;
					}
				}
			}
			else {
				for (int x = xStart + 1; x < mouseXCell; x++) {
					if (static_cast<Rook*>(pieces[fields[zStart][x]]) != this && fields[zStart][x] != -1) {
						return false;
					}
				}
			}
			return true;
		}
		else {
			return false;
		}
	}
	bool Rook::isHitPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		// �������� ��������� ������ ������ ������
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
	bool Rook::check(Piece* pieces[32], int(&fields)[8][8]) {
		int currentZ = this->getZCenter() + 3, currentX = this->getXCenter() + 3;
		if (this->getId() != fields[currentZ][currentX]) {
			return false;
		}
		for (int z = currentZ - 1; z >= 0; z--) {
			if (fields[z][currentX] != -1) {
				Piece* p = pieces[fields[z][currentX]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
				else {
					break;
				}
			}
		}
		for (int z = currentZ + 1; z < 8; z++) {
			if (fields[z][currentX] != -1) {
				Piece* p = pieces[fields[z][currentX]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
				else {
					break;
				}
			}
		}
		for (int x = currentX - 1; x >= 0; x--) {
			if (fields[currentZ][x] != -1) {
				Piece* p = pieces[fields[currentZ][x]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
				else {
					break;
				}
			}
		}
		for (int x = currentX + 1; x < 8; x++) {
			if (fields[currentZ][x] != -1) {
				Piece* p = pieces[fields[currentZ][x]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
				else {
					break;
				}
			}
		}
		return false;
	}
	bool Rook::capture(Piece* pieces[32], int(&fields)[8][8], int capturePieceId) {
		int currentZ = this->getZCenter() + 3, currentX = this->getXCenter() + 3;
		for (int z = currentZ - 1; z >= 0; z--) {
			if (fields[z][currentX] == capturePieceId) {
				return true;
			}
			else {
				break;
			}
		}
		for (int z = currentZ + 1; z < 8; z++) {
			if (fields[z][currentX] == capturePieceId) {
				return true;
			}
			else {
				break;
			}
		}
		for (int x = currentX - 1; x >= 0; x--) {
			if (fields[currentZ][x] == capturePieceId) {
				return true;
			}
			else {
				break;
			}
		}
		for (int x = currentX + 1; x < 8; x++) {
			if (fields[currentZ][x] == capturePieceId) {
				return true;
			}
			else {
				break;
			}
		}
		return false;
	}
}