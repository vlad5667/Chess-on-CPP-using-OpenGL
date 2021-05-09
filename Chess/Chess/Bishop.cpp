#include "Bishop.h"
#include "King.h"
#include <cmath>

namespace ChessGame {
	bool Bishop::correctMove(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		// Якщо хід коректний
		if (abs(zStart - mouseZCell) == abs(xStart - mouseXCell)) {
			if (mouseXCell < xStart&& mouseZCell < zStart) {
				for (int x = xStart - 1, z = zStart - 1; x > mouseXCell && z > mouseZCell; x--, z--) {
					if (static_cast<Bishop*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						goto skipBishop;
					}
				}
			}
			else if (mouseXCell < xStart && mouseZCell > zStart) {
				for (int x = xStart - 1, z = zStart + 1; x > mouseXCell && z < mouseZCell; x--, z++) {
					if (static_cast<Bishop*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						goto skipBishop;
					}
				}
			}
			else if (mouseXCell > xStart && mouseZCell < zStart) {
				for (int x = xStart + 1, z = zStart - 1; x < mouseXCell && z > mouseZCell; x++, z--) {
					if (static_cast<Bishop*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						goto skipBishop;
					}
				}
			}
			else if (mouseXCell > xStart && mouseZCell > zStart) {
				for (int x = xStart + 1, z = zStart + 1; x < mouseXCell && z < mouseZCell; x++, z++) {
					if (static_cast<Bishop*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						goto skipBishop;
					}
				}
			}
			return true;
		}
		else { // Якщо хід некоректний
		skipBishop:
			movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
			return false;
		}
	}
	bool Bishop::isHitPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell, int currentZ, int currentX) {
		if (abs(zStart - mouseZCell) == abs(xStart - mouseXCell)) {
			if (mouseXCell < xStart && mouseZCell < zStart) {
				for (int x = xStart - 1, z = zStart - 1; x > mouseXCell && z > mouseZCell; x--, z--) {
					if (static_cast<Bishop*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						return false;
					}
				}
				return true;
			}
			else if (mouseXCell < xStart && mouseZCell > zStart) {
				for (int x = xStart - 1, z = zStart + 1; x > mouseXCell && z < mouseZCell; x--, z++) {
					if (static_cast<Bishop*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						return false;
					}
				}
				return true;
			}
			else if (mouseXCell > xStart && mouseZCell < zStart) {
				for (int x = xStart + 1, z = zStart - 1; x < mouseXCell && z > mouseZCell; x++, z--) {
					if (static_cast<Bishop*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						return false;
					}
				}
				return true;
			}
			else if (mouseXCell > xStart && mouseZCell > zStart) {
				for (int x = xStart + 1, z = zStart + 1; x < mouseXCell && z < mouseZCell; x++, z++) {
					if (static_cast<Bishop*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						return false;
					}
				}
				return true;
			}
		}
		return false;
	}
	int Bishop::isCheckOccured(Piece* pieces[32], int(&fields)[8][8]) {
		int currentZ = this->getZCenter() + 3, currentX = this->getXCenter() + 3;
		if (this->getId() != fields[currentZ][currentX]) {
			return 0;
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
