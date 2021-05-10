#include "Pawn.h"
#include "King.h"
#include <cmath>
#include <iostream>

namespace ChessGame {
	bool Pawn::correctMove(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		if (getColor() == 'W') {
			if (getFirstMove()) {
				if (abs(zStart - mouseZCell) > 2 || xStart - mouseXCell != 0) {
					movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
					return false;
				}
				else {
					setFirstMove(false);
					return true;
				}
			}
			else if (abs(zStart - mouseZCell) > 1 || zStart - mouseZCell < 0 || xStart - mouseXCell != 0) {
				movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
				return false;
			}
			else {
				return true;
			}
		}
		else {
			if (getFirstMove()) {
				// якщо х≥д некоректний
				if (abs(zStart - mouseZCell) > 2 || xStart - mouseXCell != 0) {
					movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
					return false;
				}
				else { // якщо х≥д коректний
					setFirstMove(false);
					return true;
				}
				// якщо це не перший х≥д, то перев≥р€Їмо коректн≥сть ходу на одну кл≥тинку вперед
			}
			else if (abs(zStart - mouseZCell) > 1 || zStart - mouseZCell > 0 || xStart - mouseXCell != 0) {
				movePieceToPosition(fields, zStart, xStart, mouseZCell, mouseXCell);
				return false;
			}
			else { // якщо х≥д коректний
				return true;
			}
		}
	}
	bool Pawn::isHitPossible(Piece *pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		if (getColor() == 'W') {
			// ѕерев≥рка можливост≥ побити ворожу ф≥гуру
			if (zStart - mouseZCell == 1 && abs(mouseXCell - xStart) == 1) {
				return true;
			}
		}
		else {
			// ѕерев≥рка можливост≥ побити ворожу ф≥гуру
			if (mouseZCell - zStart == 1 && abs(mouseXCell - xStart) == 1) {
				return true;
			}
		}
		return false;
	}
	int Pawn::check(Piece* pieces[32], int(&fields)[8][8]) {
		int currentZ = this->getZCenter() + 3, currentX = this->getXCenter() + 3;
		if (this->getId() != fields[currentZ][currentX]) {
			return 0;
		}
		if (currentZ >= 1 && currentX >= 1 && currentZ <= 6 && currentX <= 6) {
			if (getColor() == 'W') {
				if (fields[currentZ - 1][currentX + 1] != -1) {
					Piece* p = pieces[fields[currentZ - 1][currentX + 1]];
					if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
						return 1;
					}
				}
				if (fields[currentZ - 1][currentX - 1] != -1) {
					Piece* p = pieces[fields[currentZ - 1][currentX - 1]];
					if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
						return 1;
					}
				}
			}
			else {
				if (fields[currentZ + 1][currentX + 1] != -1) {
					Piece* p = pieces[fields[currentZ + 1][currentX + 1]];
					if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
						return 1;
					}
				}
				if (fields[currentZ + 1][currentX - 1] != -1) {
					Piece* p = pieces[fields[currentZ + 1][currentX - 1]];
					if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
						return 1;
					}
				}
			}
		}
		return 0;
	}
	bool Pawn::isEnPassantOccured(Piece* pieces[32], int(&fields)[8][8], int prevPieceId, int mouseZCell, int mouseXCell) {
		int currentZ = this->getZCenter() + 3, currentX = this->getXCenter() + 3;
		int prevPieceZ = pieces[prevPieceId]->getZCenter() + 3, prevPieceX = pieces[prevPieceId]->getXCenter() + 3;
		if (this->getColor() == 'W' && prevPieceId >= 8 && prevPieceId <= 15) {
			if (prevPieceZ == 3 && currentZ == 2 && prevPieceX == currentX) {
				if (mouseZCell - prevPieceZ == -1 && mouseXCell == prevPieceX) {
					return true;
				}
			}
		}
		else if (this->getColor() == 'B' && prevPieceId >= 16 && prevPieceId <= 23) {
			if (prevPieceZ == 4 && currentZ == 5 && prevPieceX == currentX) {
				if (mouseZCell - prevPieceZ == 1 && mouseXCell == prevPieceX) {
					return true;
				}
			}
		}
		return false;
	}
	bool Pawn::isPromotionOccured(int mouseZCell, int mouseXCell) {
		int currentZ = this->getZCenter() + 3, currentX = this->getXCenter() + 3;
		if (currentZ - mouseZCell <= 1 && abs(currentX - mouseXCell) <= 1 && mouseZCell == 0) {
			return true;
		}
		else if (mouseZCell - currentZ <= 1 && abs(currentX - mouseXCell) <= 1 && mouseZCell == 7) {
			return true;
		}
		return false;
	}
}