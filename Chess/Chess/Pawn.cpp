#include "Pawn.h"
#include "King.h"

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
		int id = this->getId();
		if (id != fields[currentZ][currentX]) {
			bool pieceIsFound = false;
			for (currentZ = 0; currentZ < 8; currentZ++) {
				std::pair<int, int> row[8];
				for (int i = 0; i < 8; i++) {
					row[i] = std::make_pair(fields[currentZ][i], i);
				}
				std::sort(row, row + 8, sortComp);
				currentX = binarySearch(row, 8, id);
				if (currentX != -1) {
					pieceIsFound = true;
					break;
				}
			}
			if (!pieceIsFound) {
				return false;
			}
		}
		if (getColor() == 'W') {
			if (currentZ >= 1 && currentX <= 6) {
				if (fields[currentZ - 1][currentX + 1] != -1) {
					Piece* p = pieces[fields[currentZ - 1][currentX + 1]];
					if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
						return true;
					}
				}
			}
			if (currentZ >= 1 && currentX >= 1) {
				if (fields[currentZ - 1][currentX - 1] != -1) {
					Piece* p = pieces[fields[currentZ - 1][currentX - 1]];
					if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
						return true;
					}
				}
			}
		}
		else {
			if (currentZ <= 6 && currentX <= 6) {
				if (fields[currentZ + 1][currentX + 1] != -1) {
					Piece* p = pieces[fields[currentZ + 1][currentX + 1]];
					if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
						return true;
					}
				}
			}
			if (currentZ <= 6 && currentX >= 1) {
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
	bool Pawn::isEnPassantOccurred(Piece* pieces[32], int(&fields)[8][8], int prevPieceId, int mouseZCell, int mouseXCell) {
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
	bool Pawn::isPromotionOccurred(int mouseZCell, int mouseXCell) {
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
		int id = this->getId();
		if (id != fields[currentZ][currentX]) {
			bool pieceIsFound = false;
			for (currentZ = 0; currentZ < 8; currentZ++) {
				std::pair<int, int> row[8];
				for (int i = 0; i < 8; i++) {
					row[i] = std::make_pair(fields[currentZ][i], i);
				}
				std::sort(row, row + 8, sortComp);
				currentX = binarySearch(row, 8, id);
				if (currentX != -1) {
					pieceIsFound = true;
					break;
				}
			}
			if (!pieceIsFound) {
				return false;
			}
		}
		if (getColor() == 'W') {
			if (currentZ >= 1 && currentX <= 6) {
				if (fields[currentZ - 1][currentX + 1] == capturePieceId) {
					fields[currentZ - 1][currentX + 1] = fields[currentZ][currentX];
					fields[currentZ][currentX] = -1;
					int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
					fields[currentZ][currentX] = fields[currentZ - 1][currentX + 1];
					fields[currentZ - 1][currentX + 1] = capturePieceId;
					if (checkW != -1) {
						return false;
					}
					return true;
				}
			}
			if (currentZ >= 1 && currentX >= 1) {
				if (fields[currentZ - 1][currentX - 1] == capturePieceId) {
					fields[currentZ - 1][currentX - 1] = fields[currentZ][currentX];
					fields[currentZ][currentX] = -1;
					int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
					fields[currentZ][currentX] = fields[currentZ - 1][currentX - 1];
					fields[currentZ - 1][currentX - 1] = capturePieceId;
					if (checkW != -1) {
						return false;
					}
					return true;
				}
			}
		}
		else {
			if (currentZ <= 6 && currentX <= 6) {
				if (fields[currentZ + 1][currentX + 1] == capturePieceId) {
					fields[currentZ + 1][currentX + 1] = fields[currentZ][currentX];
					fields[currentZ][currentX] = -1;
					int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
					fields[currentZ][currentX] = fields[currentZ + 1][currentX + 1];
					fields[currentZ + 1][currentX + 1] = capturePieceId;
					if (checkB != -1) {
						return false;
					}
					return true;
				}
			}
			if (currentZ <= 6 && currentX >= 1) {
				if (fields[currentZ + 1][currentX - 1] == capturePieceId) {
					fields[currentZ + 1][currentX - 1] = fields[currentZ][currentX];
					fields[currentZ][currentX] = -1;
					int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
					fields[currentZ][currentX] = fields[currentZ + 1][currentX - 1];
					fields[currentZ + 1][currentX - 1] = capturePieceId;
					if (checkB != -1) {
						return false;
					}
					return true;
				}
			}
		}
		return false;
	}
	bool Pawn::hasMove(Piece* pieces[32], int(&fields)[8][8]) {
		int currentZ = getZCenter() + 3, currentX = getXCenter() + 3;
		if (getColor() == 'W') {
			if (getFirstMove()) {
				if (fields[currentZ - 1][currentX] == -1) {
					std::swap(fields[currentZ - 1][currentX], fields[currentZ][currentX]);
					int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
					std::swap(fields[currentZ - 1][currentX], fields[currentZ][currentX]);
					if (checkW == -1) {
						return true;
					}
					if (fields[currentZ - 2][currentX] == -1) {
						std::swap(fields[currentZ - 2][currentX], fields[currentZ][currentX]);
						int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
						std::swap(fields[currentZ - 2][currentX], fields[currentZ][currentX]);
						if (checkW == -1) {
							return true;
						}
					}
				}
			}
			else if (currentZ >= 1) {
				if (fields[currentZ - 1][currentX] == -1) {
					std::swap(fields[currentZ - 1][currentX], fields[currentZ][currentX]);
					int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
					std::swap(fields[currentZ - 1][currentX], fields[currentZ][currentX]);
					if (checkW == -1) {
						return true;
					}
				}
			}
			if (currentZ >= 1 && currentX <= 6) {
				if (fields[currentZ - 1][currentX + 1] != -1) {
					if (pieces[fields[currentZ - 1][currentX + 1]]->getColor() == 'B') {
						int k = fields[currentZ - 1][currentX + 1];
						fields[currentZ - 1][currentX + 1] = fields[currentZ][currentX];
						fields[currentZ][currentX] = -1;
						int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
						fields[currentZ][currentX] = fields[currentZ - 1][currentX + 1];
						fields[currentZ - 1][currentX + 1] = k;
						if (checkW == -1) {
							return true;
						}
					}
				}
			}
			if (currentZ >= 1 && currentX >= 1) {
				if (fields[currentZ - 1][currentX - 1] != -1) {
					if (pieces[fields[currentZ - 1][currentX - 1]]->getColor() == 'B') {
						int k = fields[currentZ - 1][currentX - 1];
						fields[currentZ - 1][currentX - 1] = fields[currentZ][currentX];
						fields[currentZ][currentX] = -1;
						int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
						fields[currentZ][currentX] = fields[currentZ - 1][currentX - 1];
						fields[currentZ - 1][currentX - 1] = k;
						if (checkW == -1) {
							return true;
						}
					}
				}
			}
		}
		else {
			if (getFirstMove()) {
				if (fields[currentZ + 1][currentX] == -1) {
					std::swap(fields[currentZ + 1][currentX], fields[currentZ][currentX]);
					int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
					std::swap(fields[currentZ + 1][currentX], fields[currentZ][currentX]);
					if (checkB == -1) {
						return true;
					}
					if (fields[currentZ + 2][currentX] == -1) {
						std::swap(fields[currentZ + 2][currentX], fields[currentZ][currentX]);
						int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
						std::swap(fields[currentZ + 2][currentX], fields[currentZ][currentX]);
						if (checkB == -1) {
							return true;
						}
					}
				}
			}
			else if (currentZ <= 6) {
				if (fields[currentZ + 1][currentX] == -1) {
					std::swap(fields[currentZ + 1][currentX], fields[currentZ][currentX]);
					int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
					std::swap(fields[currentZ + 1][currentX], fields[currentZ][currentX]);
					if (checkB == -1) {
						return true;
					}
				}
			}
			if (currentZ <= 6 && currentX <= 6) {
				if (fields[currentZ + 1][currentX + 1] != -1) {
					if (pieces[fields[currentZ + 1][currentX + 1]]->getColor() == 'W') {
						int k = fields[currentZ + 1][currentX + 1];
						fields[currentZ + 1][currentX + 1] = fields[currentZ][currentX];
						fields[currentZ][currentX] = -1;
						int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
						fields[currentZ][currentX] = fields[currentZ + 1][currentX + 1];
						fields[currentZ + 1][currentX + 1] = k;
						if (checkB == -1) {
							return true;
						}
					}
				}
			}
			if (currentZ <= 6 && currentX >= 1) {
				if (fields[currentZ + 1][currentX - 1] != -1) {
					if (pieces[fields[currentZ + 1][currentX - 1]]->getColor() == 'W') {
						int k = fields[currentZ + 1][currentX - 1];
						fields[currentZ + 1][currentX - 1] = fields[currentZ][currentX];
						fields[currentZ][currentX] = -1;
						int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
						fields[currentZ][currentX] = fields[currentZ + 1][currentX - 1];
						fields[currentZ + 1][currentX - 1] = k;
						if (checkB == -1) {
							return true;
						}
					}
				}
			}
		}
		return false;
	}
}