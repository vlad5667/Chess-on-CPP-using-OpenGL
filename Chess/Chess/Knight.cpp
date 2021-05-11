#include "Knight.h"
#include "King.h"
#include <cmath>

namespace ChessGame {
	bool Knight::isMovePossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		// ���� ��� ���������
		if ((abs(zStart - mouseZCell) == 1 && abs(xStart - mouseXCell) == 2)
			|| (abs(zStart - mouseZCell) == 2 && abs(xStart - mouseXCell) == 1)) {
			return true;
		}
		else { // ���� ��� �����������
			return false;
		}
	}
	bool Knight::isHitPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		// �������� ��������� ������ ������ ������
		if ((abs(zStart - mouseZCell) == 1 && abs(xStart - mouseXCell) == 2)
			|| (abs(zStart - mouseZCell) == 2 && abs(xStart - mouseXCell) == 1)) {
			return true;
		}
		return false;
	}
	bool Knight::check(Piece* pieces[32], int(&fields)[8][8]) {
		int currentZ = this->getZCenter() + 3, currentX = this->getXCenter() + 3;
		if (this->getId() != fields[currentZ][currentX]) {
			return false;
		}
		if (currentZ >= 2 && currentX >= 1) {
			if (fields[currentZ - 2][currentX - 1] != -1) {
				Piece* p = pieces[fields[currentZ - 2][currentX - 1]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
			}
		}
		if (currentZ >= 2 && currentX <= 6) {
			if (fields[currentZ - 2][currentX + 1] != -1) {
				Piece* p = pieces[fields[currentZ - 2][currentX + 1]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
			}
		}
		if (currentZ <= 5 && currentX >= 1) {
			if (fields[currentZ + 2][currentX - 1] != -1) {
				Piece* p = pieces[fields[currentZ + 2][currentX - 1]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
			}
		}
		if (currentZ <= 5 && currentX <= 6) {
			if (fields[currentZ + 2][currentX + 1] != -1) {
				Piece* p = pieces[fields[currentZ + 2][currentX + 1]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
			}
		}
		if (currentZ >= 1 && currentX >= 2) {
			if (fields[currentZ - 1][currentX - 2] != -1) {
				Piece* p = pieces[fields[currentZ - 1][currentX - 2]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
			}
		}
		if (currentZ >= 1 && currentX <= 5) {
			if (fields[currentZ - 1][currentX + 2] != -1) {
				Piece* p = pieces[fields[currentZ - 1][currentX + 2]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
			}
		}
		if (currentZ <= 6 && currentX >= 2) {
			if (fields[currentZ + 1][currentX - 2] != -1) {
				Piece* p = pieces[fields[currentZ + 1][currentX - 2]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
			}
		}
		if (currentZ <= 6 && currentX <= 5) {
			if (fields[currentZ + 1][currentX + 2] != -1) {
				Piece* p = pieces[fields[currentZ + 1][currentX + 2]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
			}
		}
		return false;
	}
	bool Knight::capture(Piece* pieces[32], int(&fields)[8][8], int capturePieceId) {
		int currentZ = this->getZCenter() + 3, currentX = this->getXCenter() + 3;
		if (this->isBeaten()) {
			return false;
		}
		if (currentZ >= 2 && currentX >= 1) {
			if (fields[currentZ - 2][currentX - 1] == capturePieceId) {
				fields[currentZ - 2][currentX - 1] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ - 2][currentX - 1];
				fields[currentZ - 2][currentX - 1] = capturePieceId;
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
		if (currentZ >= 2 && currentX <= 6) {
			if (fields[currentZ - 2][currentX + 1] == capturePieceId) {
				fields[currentZ - 2][currentX + 1] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ - 2][currentX + 1];
				fields[currentZ - 2][currentX + 1] = capturePieceId;
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
		if (currentZ <= 5 && currentX >= 1) {
			if (fields[currentZ + 2][currentX - 1] == capturePieceId) {
				fields[currentZ + 2][currentX - 1] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ + 2][currentX - 1];
				fields[currentZ + 2][currentX - 1] = capturePieceId;
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
		if (currentZ <= 5 && currentX <= 6) {
			if (fields[currentZ + 2][currentX + 1] == capturePieceId) {
				fields[currentZ + 2][currentX + 1] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ + 2][currentX + 1];
				fields[currentZ + 2][currentX + 1] = capturePieceId;
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
		if (currentZ >= 1 && currentX >= 2) {
			if (fields[currentZ - 1][currentX - 2] == capturePieceId) {
				fields[currentZ - 1][currentX - 2] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ - 1][currentX - 2];
				fields[currentZ - 1][currentX - 2] = capturePieceId;
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
		if (currentZ >= 1 && currentX <= 5) {
			if (fields[currentZ - 1][currentX + 2] == capturePieceId) {
				fields[currentZ - 1][currentX + 2] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ - 1][currentX + 2];
				fields[currentZ - 1][currentX + 2] = capturePieceId;
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
		if (currentZ <= 6 && currentX >= 2) {
			if (fields[currentZ + 1][currentX - 2] == capturePieceId) {
				fields[currentZ + 1][currentX - 2] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ + 1][currentX - 2];
				fields[currentZ + 1][currentX - 2] = capturePieceId;
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
		if (currentZ <= 6 && currentX <= 5) {
			if (fields[currentZ + 1][currentX + 2] == capturePieceId) {
				fields[currentZ + 1][currentX + 2] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccured(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccured(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ + 1][currentX + 2];
				fields[currentZ + 1][currentX + 2] = capturePieceId;
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
		return false;
	}
}