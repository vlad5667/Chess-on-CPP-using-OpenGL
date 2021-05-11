#include "Pawn.h"
#include "King.h"
#include <cmath>
#include <iostream>

namespace ChessGame {
	bool Pawn::isMovePossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		if (getColor() == 'W') {
			if (getFirstMove()) {
				if (abs(zStart - mouseZCell) > 2 || xStart - mouseXCell != 0) {
					return false;
				}
				else {
					setFirstMove(false);
					return true;
				}
			}
			else if (abs(zStart - mouseZCell) > 1 || zStart - mouseZCell < 0 || xStart - mouseXCell != 0) {
				return false;
			}
			else {
				return true;
			}
		}
		else {
			if (getFirstMove()) {
				// ���� ��� �����������
				if (abs(zStart - mouseZCell) > 2 || xStart - mouseXCell != 0) {
					return false;
				}
				else { // ���� ��� ���������
					setFirstMove(false);
					return true;
				}
				// ���� �� �� ������ ���, �� ���������� ���������� ���� �� ���� ������� ������
			}
			else if (abs(zStart - mouseZCell) > 1 || zStart - mouseZCell > 0 || xStart - mouseXCell != 0) {
				return false;
			}
			else { // ���� ��� ���������
				return true;
			}
		}
	}
	bool Pawn::isHitPossible(Piece *pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		if (getColor() == 'W') {
			// �������� ��������� ������ ������ ������
			if (zStart - mouseZCell == 1 && abs(mouseXCell - xStart) == 1) {
				return true;
			}
		}
		else {
			// �������� ��������� ������ ������ ������
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
		if (currentZ >= 1 && currentX >= 1 && currentZ <= 6 && currentX <= 6) {
			if (getColor() == 'W') {
				if (fields[currentZ - 1][currentX + 1] == capturePieceId) {
					return true;
				}
				if (fields[currentZ - 1][currentX - 1]  == capturePieceId) {
					return true;
				}
			}
			else {
				if (fields[currentZ + 1][currentX + 1] == capturePieceId) {
					return true;
				}
				if (fields[currentZ + 1][currentX - 1] == capturePieceId) {
					return true;
				}
			}
		}
		return false;
	}
}