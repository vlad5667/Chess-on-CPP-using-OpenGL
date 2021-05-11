#include "Pawn.h"
#include "King.h"
#include <cmath>
#include <iostream>

namespace ChessGame {
	bool Pawn::isMovePossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		if (getColor() == 'W') {
			if (getFirstMove() && zStart - mouseZCell == 2 && xStart - mouseXCell == 0 && fields[zStart - 1][xStart] == -1 && fields[zStart - 2][xStart] == this->getId()) {
				return true;
			}
			else if (zStart - mouseZCell == 1 && xStart - mouseXCell == 0 && fields[zStart - 1][xStart] == this->getId()) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (getFirstMove() && mouseZCell - zStart == 2 && xStart - mouseXCell == 0 && fields[zStart + 1][xStart] == -1 && fields[zStart + 2][xStart] == this->getId()) {
				return true;
			}
			else if (mouseZCell - zStart == 1 && xStart - mouseXCell == 0 && fields[zStart + 1][xStart] == this->getId()) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	bool Pawn::isHitPossible(Piece *pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		if (getColor() == 'W') {
			// Перевірка можливості побити ворожу фігуру
			if (zStart - mouseZCell == 1 && abs(mouseXCell - xStart) == 1) {
				return true;
			}
		}
		else {
			// Перевірка можливості побити ворожу фігуру
			if (mouseZCell - zStart == 1 && abs(mouseXCell - xStart) == 1) {
				return true;
			}
		}
		return false;
	}
	bool Pawn::check(Piece* pieces[32], int(&fields)[8][8]) {
		int currentZ = this->getZCenter() + 3, currentX = this->getXCenter() + 3;
		if (this->getId() != fields[currentZ][currentX]) {
			return false;
		}
		if (currentZ >= 1 && currentX >= 1 && currentZ <= 6 && currentX <= 6) {
			if (getColor() == 'W') {
				if (fields[currentZ - 1][currentX + 1] != -1) {
					Piece* p = pieces[fields[currentZ - 1][currentX + 1]];
					if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
						return true;
					}
				}
				if (fields[currentZ - 1][currentX - 1] != -1) {
					Piece* p = pieces[fields[currentZ - 1][currentX - 1]];
					if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
						return true;
					}
				}
			}
			else {
				if (fields[currentZ + 1][currentX + 1] != -1) {
					Piece* p = pieces[fields[currentZ + 1][currentX + 1]];
					if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
						return true;
					}
				}
				if (fields[currentZ + 1][currentX - 1] != -1) {
					Piece* p = pieces[fields[currentZ + 1][currentX - 1]];
					if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
						return true;
					}
				}
			}
		}
		return false;
	}
	bool Pawn::isEnPassantOccured(Piece* pieces[32], int(&fields)[8][8], int prevPieceId, int mouseZCell, int mouseXCell) {
		int currentZ = this->getZCenter() + 3, currentX = this->getXCenter() + 3;
		int prevPieceZ = pieces[prevPieceId]->getZCenter() + 3, prevPieceX = pieces[prevPieceId]->getXCenter() + 3;
		if (this->getColor() == 'W' && typeid(*pieces[prevPieceId]) == typeid(Pawn) && pieces[prevPieceId]->getColor() == 'B') {
			if (prevPieceZ == 3 && currentZ == 2 && prevPieceX == currentX) {
				if (mouseZCell - prevPieceZ == -1 && mouseXCell == prevPieceX) {
					return true;
				}
			}
		}
		else if (this->getColor() == 'B' && typeid(*pieces[prevPieceId]) == typeid(Pawn) && pieces[prevPieceId]->getColor() == 'W') {
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
	bool Pawn::capture(Piece* pieces[32], int(&fields)[8][8], int capturePieceId) {
		int currentZ = this->getZCenter() + 3, currentX = this->getXCenter() + 3;
		if (this->isBeaten()) {
			return false;
		}
		if (currentZ >= 1 && currentX >= 1 && currentZ <= 6 && currentX <= 6) {
			if (getColor() == 'W') {
				if (fields[currentZ - 1][currentX + 1] == capturePieceId) {
					fields[currentZ - 1][currentX + 1] = fields[currentZ][currentX];
					fields[currentZ][currentX] = -1;
					int checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
					int checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
					fields[currentZ][currentX] = fields[currentZ - 1][currentX + 1];
					fields[currentZ - 1][currentX + 1] = capturePieceId;
					if (getColor() == 'W') {
						if (checkW != -1) {
							return false;
						}
					}
					else {
						if (checkB != -1) {
							return false;
						}
					}
					return true;
				}
				if (fields[currentZ - 1][currentX - 1]  == capturePieceId) {
					fields[currentZ - 1][currentX - 1] = fields[currentZ][currentX];
					fields[currentZ][currentX] = -1;
					int checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
					int checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
					fields[currentZ][currentX] = fields[currentZ - 1][currentX - 1];
					fields[currentZ - 1][currentX - 1] = capturePieceId;
					if (getColor() == 'W') {
						if (checkW != -1) {
							return false;
						}
					}
					else {
						if (checkB != -1) {
							return false;
						}
					}
					return true;
				}
			}
			else {
				if (fields[currentZ + 1][currentX + 1] == capturePieceId) {
					fields[currentZ + 1][currentX + 1] = fields[currentZ][currentX];
					fields[currentZ][currentX] = -1;
					int checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
					int checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
					fields[currentZ][currentX] = fields[currentZ + 1][currentX + 1];
					fields[currentZ + 1][currentX + 1] = capturePieceId;
					if (getColor() == 'W') {
						if (checkW != -1) {
							return false;
						}
					}
					else {
						if (checkB != -1) {
							return false;
						}
					}
					return true;
				}
				if (fields[currentZ + 1][currentX - 1] == capturePieceId) {
					fields[currentZ + 1][currentX - 1] = fields[currentZ][currentX];
					fields[currentZ][currentX] = -1;
					int checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
					int checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
					fields[currentZ][currentX] = fields[currentZ + 1][currentX - 1];
					fields[currentZ + 1][currentX - 1] = capturePieceId;
					if (getColor() == 'W') {
						if (checkW != -1) {
							return false;
						}
					}
					else {
						if (checkB != -1) {
							return false;
						}
					}
					return true;
				}
			}
		}
		return false;
	}
}