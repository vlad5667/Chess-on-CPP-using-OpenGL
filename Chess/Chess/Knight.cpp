#include "Knight.h"
#include "King.h"
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
		int currentZ = this->getZCenter() + 3, currentX = this->getXCenter() + 3;
		if (currentZ >= 2 && currentX >= 1) {
			if (fields[currentZ - 2][currentX - 1] != -1) {
				Piece* p = pieces[fields[currentZ - 2][currentX - 1]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return 1;
				}
			}
		}
		if (currentZ >= 2 && currentX <= 6) {
			if (fields[currentZ - 2][currentX + 1] != -1) {
				Piece* p = pieces[fields[currentZ - 2][currentX + 1]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return 1;
				}
			}
		}
		if (currentZ <= 5 && currentX >= 1) {
			if (fields[currentZ + 2][currentX - 1] != -1) {
				Piece* p = pieces[fields[currentZ + 2][currentX - 1]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return 1;
				}
			}
		}
		if (currentZ <= 5 && currentX <= 6) {
			if (fields[currentZ + 2][currentX + 1] != -1) {
				Piece* p = pieces[fields[currentZ + 2][currentX + 1]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return 1;
				}
			}
		}
		if (currentZ >= 1 && currentX >= 2) {
			if (fields[currentZ - 1][currentX - 2] != -1) {
				Piece* p = pieces[fields[currentZ - 1][currentX - 2]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return 1;
				}
			}
		}
		if (currentZ >= 1 && currentX <= 5) {
			if (fields[currentZ - 1][currentX + 2] != -1) {
				Piece* p = pieces[fields[currentZ - 1][currentX + 2]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return 1;
				}
			}
		}
		if (currentZ <= 6 && currentX >= 2) {
			if (fields[currentZ + 1][currentX - 2] != -1) {
				Piece* p = pieces[fields[currentZ + 1][currentX - 2]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return 1;
				}
			}
		}
		if (currentZ <= 6 && currentX <= 5) {
			if (fields[currentZ + 1][currentX + 2] != -1) {
				Piece* p = pieces[fields[currentZ + 1][currentX + 2]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return 1;
				}
			}
		}
		return 0;
	}
}