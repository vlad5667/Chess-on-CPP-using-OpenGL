#include "King.h"
#include "Rook.h"
#include "Bishop.h"
#include "Queen.h"
#include "Knight.h"

namespace ChessGame {
	bool King::isMovePossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		if (abs(zStart - mouseZCell) > 1 || abs(xStart - mouseXCell) > 1) {
			return false;
		}
		else {
			return true;
		}
	}
	bool King::isHitPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		if (abs(zStart - mouseZCell) <= 1 && abs(xStart - mouseXCell) <= 1) {
			return true;
		}
		return false;
	}
	bool King::isCastlingOccurred(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell, int currentZ, int currentX) {
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
	bool King::check(Piece* pieces[32], int(&fields)[8][8]) {
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
		if (currentZ <= 6) {
			if (fields[currentZ + 1][currentX] != -1) {
				Piece* p = pieces[fields[currentZ + 1][currentX]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
			}
		}
		if (currentZ >= 1) {
			if (fields[currentZ - 1][currentX] != -1) {
				Piece* p = pieces[fields[currentZ - 1][currentX]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
			}
		}
		if (currentX >= 1) {
			if (fields[currentZ][currentX - 1] != -1) {
				Piece* p = pieces[fields[currentZ][currentX - 1]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
			}
		}
		if (currentX <= 6) {
			if (fields[currentZ][currentX + 1] != -1) {
				Piece* p = pieces[fields[currentZ][currentX + 1]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
			}
		}
		if (currentZ <= 6 && currentX <= 6) {
			if (fields[currentZ + 1][currentX + 1] != -1) {
				Piece* p = pieces[fields[currentZ + 1][currentX + 1]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
			}
		}
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
		if (currentZ <= 6 && currentX >= 1) {
			if (fields[currentZ + 1][currentX - 1] != -1) {
				Piece* p = pieces[fields[currentZ + 1][currentX - 1]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
				}
			}
		}
		return false;
	}
	int King::isCheckOccurred(Piece* pieces[32], int(&fields)[8][8]) {
		if (this->getColor() == 'W') {
			for (int k = 0; k < 16; k++) {
				if (!pieces[k]->isBeaten()) {
					if (pieces[k]->check(pieces, fields)) {
						return k;
					}
				}
			}
		}
		else {
			for (int k = 16; k < 32; k++) {
				if (!pieces[k]->isBeaten()) {
					if (pieces[k]->check(pieces, fields)) {
						return k;
					}
				}
			}
		}
		return -1;
	}
	bool King::capture(Piece* pieces[32], int(&fields)[8][8], int capturePieceId) {
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
		if (currentZ <= 6) {
			if (fields[currentZ + 1][currentX] == capturePieceId) {
				fields[currentZ + 1][currentX] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ + 1][currentX];
				fields[currentZ + 1][currentX] = capturePieceId;
				if (getColor() == 'W') {
					if (checkB != -1) {
						return false;
					}
				}
				else {
					if (checkW != -1) {
						return false;
					}
				}
				return true;
			}
		}
		if (currentZ >= 1) {
			if (fields[currentZ - 1][currentX] == capturePieceId) {
				fields[currentZ - 1][currentX] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ - 1][currentX];
				fields[currentZ - 1][currentX] = capturePieceId;
				if (getColor() == 'W') {
					if (checkB != -1) {
						return false;
					}
				}
				else {
					if (checkW != -1) {
						return false;
					}
				}
				return true;
			}
		}
		if (currentX >= 1) {
			if (fields[currentZ][currentX - 1] == capturePieceId) {
				fields[currentZ][currentX - 1] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ][currentX - 1];
				fields[currentZ][currentX - 1] = capturePieceId;
				if (getColor() == 'W') {
					if (checkB != -1) {
						return false;
					}
				}
				else {
					if (checkW != -1) {
						return false;
					}
				}
				return true;
			}
		}
		if (currentX <= 6) {
			if (fields[currentZ][currentX + 1] == capturePieceId) {
				fields[currentZ][currentX + 1] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ][currentX + 1];
				fields[currentZ][currentX + 1] = capturePieceId;
				if (getColor() == 'W') {
					if (checkB != -1) {
						return false;
					}
				}
				else {
					if (checkW != -1) {
						return false;
					}
				}
				return true;
			}
		}
		if (currentZ <= 6 && currentX <= 6) {
			if (fields[currentZ + 1][currentX + 1] == capturePieceId) {
				fields[currentZ + 1][currentX + 1] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
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
		}
		if (currentZ >= 1 && currentX <= 6) {
			if (fields[currentZ - 1][currentX + 1] == capturePieceId) {
				fields[currentZ - 1][currentX + 1] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ - 1][currentX + 1];
				fields[currentZ - 1][currentX + 1] = capturePieceId;
				if (getColor() == 'W') {
					if (checkB != -1) {
						return false;
					}
				}
				else {
					if (checkW != -1) {
						return false;
					}
				}
				return true;
			}
		}
		if (currentZ >= 1 && currentX >= 1) {
			if (fields[currentZ - 1][currentX - 1] == capturePieceId) {
				fields[currentZ - 1][currentX - 1] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ - 1][currentX - 1];
				fields[currentZ - 1][currentX - 1] = capturePieceId;
				if (getColor() == 'W') {
					if (checkB != -1) {
						return false;
					}
				}
				else {
					if (checkW != -1) {
						return false;
					}
				}
				return true;
			}
		}
		if (currentZ <= 6 && currentX >= 1) {
			if (fields[currentZ + 1][currentX - 1] == capturePieceId) {
				fields[currentZ + 1][currentX - 1] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ + 1][currentX - 1];
				fields[currentZ + 1][currentX - 1] = capturePieceId;
				if (getColor() == 'W') {
					if (checkB != -1) {
						return false;
					}
				}
				else {
					if (checkW != -1) {
						return false;
					}
				}
				return true;
			}
		}
		return false;
	}
	bool King::isSalvationPossible(Piece* pieces[32], int(&fields)[8][8], int checkPieceId) {
		int currentZ = this->getZCenter() + 3, currentX = this->getXCenter() + 3;
		int checkPieceZ = pieces[checkPieceId]->getZCenter() + 3, checkPieceX = pieces[checkPieceId]->getXCenter() + 3;
		if (typeid(*pieces[checkPieceId]) == typeid(Rook)) {
			for (int z = currentZ - 1, x = currentX; z > checkPieceZ; z--) {
				if (fields[z][x] == -1) {
					if (this->getColor() == 'W') {
						for (int k = 16; k <= 31; k++) {
							if (!pieces[k]->isBeaten() && k != 28) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
					else {
						for (int k = 0; k <= 15; k++) {
							if (!pieces[k]->isBeaten() && k != 4) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
				}
				else {
					break;
				}
			}
			for (int z = currentZ + 1, x = currentX; z < checkPieceZ; z++) {
				if (fields[z][x] == -1) {
					if (this->getColor() == 'W') {
						for (int k = 16; k <= 31; k++) {
							if (!pieces[k]->isBeaten() && k != 28) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
					else {
						for (int k = 0; k <= 15; k++) {
							if (!pieces[k]->isBeaten() && k != 4) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
				}
				else {
					break;
				}
			}
			for (int z = currentZ, x = currentX + 1; x < checkPieceX; x++) {
				if (fields[z][x] == -1) {
					if (this->getColor() == 'W') {
						for (int k = 16; k <= 31; k++) {
							if (!pieces[k]->isBeaten() && k != 28) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
					else {
						for (int k = 0; k <= 15; k++) {
							if (!pieces[k]->isBeaten() && k != 4) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
				}
				else {
					break;
				}
			}
			for (int z = currentZ, x = currentX - 1; x > checkPieceX; x--) {
				if (fields[z][x] == -1) {
					if (this->getColor() == 'W') {
						for (int k = 16; k <= 31; k++) {
							if (!pieces[k]->isBeaten() && k != 28) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
					else {
						for (int k = 0; k <= 15; k++) {
							if (!pieces[k]->isBeaten() && k != 4) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
				}
				else {
					break;
				}
			}
		}
		else if (typeid(*pieces[checkPieceId]) == typeid(Bishop)) {
			for (int z = currentZ + 1, x = currentX + 1; z < checkPieceZ; z++, x++) {
				if (fields[z][x] == -1) {
					if (this->getColor() == 'W') {
						for (int k = 16; k <= 31; k++) {
							if (!pieces[k]->isBeaten() && k != 28) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
					else {
						for (int k = 0; k <= 15; k++) {
							if (!pieces[k]->isBeaten() && k != 4) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
				}
				else {
					break;
				}
			}
			for (int z = currentZ + 1, x = currentX - 1; z < checkPieceZ; z++, x--) {
				if (fields[z][x] == -1) {
					if (this->getColor() == 'W') {
						for (int k = 16; k <= 31; k++) {
							if (!pieces[k]->isBeaten() && k != 28) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
					else {
						for (int k = 0; k <= 15; k++) {
							if (!pieces[k]->isBeaten() && k != 4) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
				}
				else {
					break;
				}
			}
			for (int z = currentZ - 1, x = currentX - 1; z > checkPieceZ; z--, x--) {
				if (fields[z][x] == -1) {
					if (this->getColor() == 'W') {
						for (int k = 16; k <= 31; k++) {
							if (!pieces[k]->isBeaten() && k != 28) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
					else {
						for (int k = 0; k <= 15; k++) {
							if (!pieces[k]->isBeaten() && k != 4) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
				}
				else {
					break;
				}
			}
			for (int z = currentZ - 1, x = currentX + 1; z > checkPieceZ; z--, x++) {
				if (fields[z][x] == -1) {
					if (this->getColor() == 'W') {
						for (int k = 16; k <= 31; k++) {
							if (!pieces[k]->isBeaten() && k != 28) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
					else {
						for (int k = 0; k <= 15; k++) {
							if (!pieces[k]->isBeaten() && k != 4) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
				}
				else {
					break;
				}
			}
		}
		else if (typeid(*pieces[checkPieceId]) == typeid(Queen)) {
			for (int z = currentZ - 1, x = currentX; z > checkPieceZ; z--) {
				if (fields[z][x] == -1) {
					if (this->getColor() == 'W') {
						for (int k = 16; k <= 31; k++) {
							if (!pieces[k]->isBeaten() && k != 28) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
					else {
						for (int k = 0; k <= 15; k++) {
							if (!pieces[k]->isBeaten() && k != 4) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
				}
				else {
					break;
				}
			}
			for (int z = currentZ + 1, x = currentX; z < checkPieceZ; z++) {
				if (fields[z][x] == -1) {
					if (this->getColor() == 'W') {
						for (int k = 16; k <= 31; k++) {
							if (!pieces[k]->isBeaten() && k != 28) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
					else {
						for (int k = 0; k <= 15; k++) {
							if (!pieces[k]->isBeaten() && k != 4) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
				}
				else {
					break;
				}
			}
			for (int z = currentZ, x = currentX + 1; x < checkPieceX; x++) {
				if (fields[z][x] == -1) {
					if (this->getColor() == 'W') {
						for (int k = 16; k <= 31; k++) {
							if (!pieces[k]->isBeaten() && k != 28) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
					else {
						for (int k = 0; k <= 15; k++) {
							if (!pieces[k]->isBeaten() && k != 4) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
				}
				else {
					break;
				}
			}
			for (int z = currentZ, x = currentX - 1; x > checkPieceX; x--) {
				if (fields[z][x] == -1) {
					if (this->getColor() == 'W') {
						for (int k = 16; k <= 31; k++) {
							if (!pieces[k]->isBeaten() && k != 28) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
					else {
						for (int k = 0; k <= 15; k++) {
							if (!pieces[k]->isBeaten() && k != 4) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
				}
				else {
					break;
				}
			}
			for (int z = currentZ + 1, x = currentX + 1; z < checkPieceZ; z++, x++) {
				if (fields[z][x] == -1) {
					if (this->getColor() == 'W') {
						for (int k = 16; k <= 31; k++) {
							if (!pieces[k]->isBeaten() && k != 28) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
					else {
						for (int k = 0; k <= 15; k++) {
							if (!pieces[k]->isBeaten() && k != 4) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
				}
				else {
					break;
				}
			}
			for (int z = currentZ + 1, x = currentX - 1; z < checkPieceZ; z++, x--) {
				if (fields[z][x] == -1) {
					if (this->getColor() == 'W') {
						for (int k = 16; k <= 31; k++) {
							if (!pieces[k]->isBeaten() && k != 28) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
					else {
						for (int k = 0; k <= 15; k++) {
							if (!pieces[k]->isBeaten() && k != 4) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
				}
				else {
					break;
				}
			}
			for (int z = currentZ - 1, x = currentX - 1; z > checkPieceZ; z--, x--) {
				if (fields[z][x] == -1) {
					if (this->getColor() == 'W') {
						for (int k = 16; k <= 31; k++) {
							if (!pieces[k]->isBeaten() && k != 28) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
					else {
						for (int k = 0; k <= 15; k++) {
							if (!pieces[k]->isBeaten() && k != 4) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
				}
				else {
					break;
				}
			}
			for (int z = currentZ - 1, x = currentX + 1; z > checkPieceZ; z--, x++) {
				if (fields[z][x] == -1) {
					if (this->getColor() == 'W') {
						for (int k = 16; k <= 31; k++) {
							if (!pieces[k]->isBeaten() && k != 28) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
					else {
						for (int k = 0; k <= 15; k++) {
							if (!pieces[k]->isBeaten() && k != 4) {
								int zStart = pieces[k]->getZCenter() + 3, xStart = pieces[k]->getXCenter() + 3;
								if (pieces[k]->isMovePossible(pieces, fields, zStart, xStart, z, x)) {
									fields[z][x] = fields[zStart][xStart];
									fields[zStart][xStart] = -1;
									int check = this->isCheckOccurred(pieces, fields);
									fields[zStart][xStart] = fields[z][x];
									fields[z][x] = -1;
									if (check != -1) {
										continue;
									}
									return true;
								}
							}
						}
					}
				}
				else {
					break;
				}
			}
		}
		return false;
	}
	bool King::hasMove(Piece* pieces[32], int(&fields)[8][8]) {
		int currentZ = this->getZCenter() + 3, currentX = this->getXCenter() + 3;
		if (currentZ <= 6) {
			int z = currentZ + 1, x = currentX;
			if (fields[z][x] == -1) {
				std::swap(fields[z][x], fields[currentZ][currentX]);
				int check = isCheckOccurred(pieces, fields);
				std::swap(fields[z][x], fields[currentZ][currentX]);
				if (check == -1) {
					return true;
				}
			}
			else if (getColor() != pieces[fields[z][x]]->getColor()) {
				int k = fields[z][x];
				fields[z][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int check = isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][x];
				fields[z][x] = k;
				if (check == -1) {
					return true;
				}
			}
		}
		if (currentZ >= 1) {
			int z = currentZ - 1, x = currentX;
			if (fields[z][x] == -1) {
				std::swap(fields[z][x], fields[currentZ][currentX]);
				int check = isCheckOccurred(pieces, fields);
				std::swap(fields[z][x], fields[currentZ][currentX]);
				if (check == -1) {
					return true;
				}
			}
			else if (getColor() != pieces[fields[z][x]]->getColor()) {
				int k = fields[z][x];
				fields[z][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int check = isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][x];
				fields[z][x] = k;
				if (check == -1) {
					return true;
				}
			}
		}
		if (currentX >= 1) {
			int z = currentZ, x = currentX - 1;
			if (fields[z][x] == -1) {
				std::swap(fields[z][x], fields[currentZ][currentX]);
				int check = isCheckOccurred(pieces, fields);
				std::swap(fields[z][x], fields[currentZ][currentX]);
				if (check == -1) {
					return true;
				}
			}
			else if (getColor() != pieces[fields[z][x]]->getColor()) {
				int k = fields[z][x];
				fields[z][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int check = isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][x];
				fields[z][x] = k;
				if (check == -1) {
					return true;
				}
			}
		}
		if (currentX <= 6) {
			int z = currentZ, x = currentX + 1;
			if (fields[z][x] == -1) {
				std::swap(fields[z][x], fields[currentZ][currentX]);
				int check = isCheckOccurred(pieces, fields);
				std::swap(fields[z][x], fields[currentZ][currentX]);
				if (check == -1) {
					return true;
				}
			}
			else if (getColor() != pieces[fields[z][x]]->getColor()) {
				int k = fields[z][x];
				fields[z][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int check = isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][x];
				fields[z][x] = k;
				if (check == -1) {
					return true;
				}
			}
		}
		if (currentZ <= 6 && currentX <= 6) {
			int z = currentZ + 1, x = currentX + 1;
			if (fields[z][x] == -1) {
				std::swap(fields[z][x], fields[currentZ][currentX]);
				int check = isCheckOccurred(pieces, fields);
				std::swap(fields[z][x], fields[currentZ][currentX]);
				if (check == -1) {
					return true;
				}
			}
			else if (getColor() != pieces[fields[z][x]]->getColor()) {
				int k = fields[z][x];
				fields[z][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int check = isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][x];
				fields[z][x] = k;
				if (check == -1) {
					return true;
				}
			}
		}
		if (currentZ >= 1 && currentX <= 6) {
			int z = currentZ - 1, x = currentX + 1;
			if (fields[z][x] == -1) {
				std::swap(fields[z][x], fields[currentZ][currentX]);
				int check = isCheckOccurred(pieces, fields);
				std::swap(fields[z][x], fields[currentZ][currentX]);
				if (check == -1) {
					return true;
				}
			}
			else if (getColor() != pieces[fields[z][x]]->getColor()) {
				int k = fields[z][x];
				fields[z][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int check = isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][x];
				fields[z][x] = k;
				if (check == -1) {
					return true;
				}
			}
		}
		if (currentZ >= 1 && currentX >= 1) {
			int z = currentZ - 1, x = currentX - 1;
			if (fields[z][x] == -1) {
				std::swap(fields[z][x], fields[currentZ][currentX]);
				int check = isCheckOccurred(pieces, fields);
				std::swap(fields[z][x], fields[currentZ][currentX]);
				if (check == -1) {
					return true;
				}
			}
			else if (getColor() != pieces[fields[z][x]]->getColor()) {
				int k = fields[z][x];
				fields[z][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int check = isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][x];
				fields[z][x] = k;
				if (check == -1) {
					return true;
				}
			}
		}
		if (currentZ <= 6 && currentX >= 1) {
			int z = currentZ + 1, x = currentX - 1;
			if (fields[z][x] == -1) {
				std::swap(fields[z][x], fields[currentZ][currentX]);
				int check = isCheckOccurred(pieces, fields);
				std::swap(fields[z][x], fields[currentZ][currentX]);
				if (check == -1) {
					return true;
				}
			}
			else if (getColor() != pieces[fields[z][x]]->getColor()) {
				int k = fields[z][x];
				fields[z][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int check = isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][x];
				fields[z][x] = k;
				if (check == -1) {
					return true;
				}
			}
		}
		return false;
	}
	bool King::isMateOccurred(Piece* pieces[32], int(&fields)[8][8]) {
		if (!hasMove(pieces, fields)) {
			int checkPieceId = isCheckOccurred(pieces, fields);
			if (!pieces[checkPieceId]->isCapturePossible(pieces, fields)) {
				if (typeid(*pieces[checkPieceId]) == typeid(Knight)) {
					return true;
				}
				if (!isSalvationPossible(pieces, fields, checkPieceId)) {
					return true;
				}
			}
		}
		return false;
	}
	bool King::isStalemateOccurred(Piece* pieces[32], int(&fields)[8][8]) {
		if (isCheckOccurred(pieces, fields) == -1) {
			if (!hasMove(pieces, fields)) {
				if (getColor() == 'W') {
					for (int k = 16; k < 32; k++) {
						if (!pieces[k]->isBeaten()) {
							if (pieces[k]->hasMove(pieces, fields)) {
								return false;
							}
						}
					}
					return true;
				}
				else {
					for (int k = 0; k < 16; k++) {
						if (!pieces[k]->isBeaten()) {
							if (pieces[k]->hasMove(pieces, fields)) {
								return false;
							}
						}
					}
					return true;
				}
			}
		}
		return false;
	}
}