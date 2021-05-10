#include "Queen.h"
#include "King.h"
#include <cmath>

namespace ChessGame {
	bool Queen::correctMove(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		if (abs(mouseZCell - zStart) != 0 && abs(mouseXCell - xStart) == 0) {
			if (mouseZCell < zStart) {
				for (int z = zStart - 1; z > mouseZCell; z--) {
					if (static_cast<Queen*>(pieces[fields[z][xStart]]) != this && fields[z][xStart] != -1) {
						goto skipQueen;
					}
				}
			}
			else {
				for (int z = zStart + 1; z < mouseZCell; z++) {
					if (static_cast<Queen*>(pieces[fields[z][xStart]]) != this && fields[z][xStart] != -1) {
						goto skipQueen;
					}
				}
			}
			return true;
		}
		else if (abs(mouseZCell - zStart) == 0 && abs(mouseXCell - xStart) != 0) {
			if (mouseXCell < xStart) {
				for (int x = xStart - 1; x > mouseXCell; x--) {
					if (static_cast<Queen*>(pieces[fields[zStart][x]]) != this && fields[zStart][x] != -1) {
						goto skipQueen;
					}
				}
			}
			else {
				for (int x = xStart + 1; x < mouseXCell; x++) {
					if (static_cast<Queen*>(pieces[fields[zStart][x]]) != this && fields[zStart][x] != -1) {
						goto skipQueen;
					}
				}
			}
			return true;
		}
		else if (abs(zStart - mouseZCell) == abs(xStart - mouseXCell)) {
			if (mouseXCell < xStart&& mouseZCell < zStart) {
				for (int x = xStart - 1, z = zStart - 1; x > mouseXCell && z > mouseZCell; x--, z--) {
					if (static_cast<Queen*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						goto skipQueen;
					}
				}
			}
			else if (mouseXCell < xStart && mouseZCell > zStart) {
				for (int x = xStart - 1, z = zStart + 1; x > mouseXCell && z < mouseZCell; x--, z++) {
					if (static_cast<Queen*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						goto skipQueen;
					}
				}
			}
			else if (mouseXCell > xStart && mouseZCell < zStart) {
				for (int x = xStart + 1, z = zStart - 1; x < mouseXCell && z > mouseZCell; x++, z--) {
					if (static_cast<Queen*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						goto skipQueen;
					}
				}
			}
			else if (mouseXCell > xStart && mouseZCell > zStart) {
				for (int x = xStart + 1, z = zStart + 1; x < mouseXCell && z < mouseZCell; x++, z++) {
					if (static_cast<Queen*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						goto skipQueen;
					}
				}
			}
			return true;
		}
		else {
		skipQueen:
			movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
			return false;
		}
	}
	bool Queen::isHitPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		// Перевірка можливості побити ворожу фігуру
		if (abs(mouseZCell - zStart) != 0 && abs(mouseXCell - xStart) == 0) {
			if (mouseZCell < zStart) {
				for (int z = zStart - 1; z > mouseZCell; z--) {
					if (static_cast<Queen*>(pieces[fields[z][xStart]]) != this && fields[z][xStart] != -1) {
						return false;
					}
				}
				return true;
			}
			else {
				for (int z = zStart + 1; z < mouseZCell; z++) {
					if (static_cast<Queen*>(pieces[fields[z][xStart]]) != this && fields[z][xStart] != -1) {
						return false;
					}
				}
				return true;
			}
		}
		else if (abs(mouseZCell - zStart) == 0 && abs(mouseXCell - xStart) != 0) {
			if (mouseXCell < xStart) {
				for (int x = xStart - 1; x > mouseXCell; x--) {
					if (static_cast<Queen*>(pieces[fields[zStart][x]]) != this && fields[zStart][x] != -1) {
						return false;
					}
				}
				return true;
			}
			else {
				for (int x = xStart + 1; x < mouseXCell; x++) {
					if (static_cast<Queen*>(pieces[fields[zStart][x]]) != this && fields[zStart][x] != -1) {
						return false;
					}
				}
				return true;
			}
		}
		else if (abs(zStart - mouseZCell) == abs(xStart - mouseXCell)) {
			if (mouseXCell < xStart&& mouseZCell < zStart) {
				for (int x = xStart - 1, z = zStart - 1; x > mouseXCell && z > mouseZCell; x--, z--) {
					if (static_cast<Queen*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						return false;
					}
				}
				return true;
			}
			else if (mouseXCell < xStart && mouseZCell > zStart) {
				for (int x = xStart - 1, z = zStart + 1; x > mouseXCell && z < mouseZCell; x--, z++) {
					if (static_cast<Queen*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						return false;
					}
				}
				return true;
			}
			else if (mouseXCell > xStart && mouseZCell < zStart) {
				for (int x = xStart + 1, z = zStart - 1; x < mouseXCell && z > mouseZCell; x++, z--) {
					if (static_cast<Queen*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						return false;
					}
				}
				return true;
			}
			else if (mouseXCell > xStart && mouseZCell > zStart) {
				for (int x = xStart + 1, z = zStart + 1; x < mouseXCell && z < mouseZCell; x++, z++) {
					if (static_cast<Queen*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						return false;
					}
				}
				return true;
			}
		}
		return false;
	}
	int Queen::check(Piece* pieces[32], int(&fields)[8][8]) {
		int currentZ = this->getZCenter() + 3, currentX = this->getXCenter() + 3;
		if (this->getId() != fields[currentZ][currentX]) {
			return 0;
		}
		for (int z = currentZ - 1; z >= 0; z--) {
			if (fields[z][currentX] != -1) {
				Piece* p = pieces[fields[z][currentX]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return 1;
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
					return 1;
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
					return 1;
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
					return 1;
				}
				else {
					break;
				}
			}
		}
		for (int z = currentZ + 1, x = currentX + 1; z < 8 && x < 8; z++, x++) {
			if (fields[z][x] != -1) {
				Piece* p = pieces[fields[z][x]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return 1;
				}
				else {
					break;
				}
			}
		}
		for (int z = currentZ + 1, x = currentX - 1; z < 8 && x >= 0; z++, x--) {
			if (fields[z][x] != -1) {
				Piece* p = pieces[fields[z][x]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return 1;
				}
				else {
					break;
				}
			}
		}
		for (int z = currentZ - 1, x = currentX + 1; z >= 0 && x < 8; z--, x++) {
			if (fields[z][x] != -1) {
				Piece* p = pieces[fields[z][x]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return 1;
				}
				else {
					break;
				}
			}
		}
		for (int z = currentZ - 1, x = currentX - 1; z >= 0 && x >= 0; z--, x--) {
			if (fields[z][x] != -1) {
				Piece* p = pieces[fields[z][x]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return 1;
				}
				else {
					break;
				}
			}
		}
		return 0;
	}
}