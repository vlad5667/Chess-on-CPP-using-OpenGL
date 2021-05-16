#include "Rook.h"
#include "King.h"

namespace ChessGame {
	bool Rook::isMovePossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		if (abs(mouseZCell - zStart) != 0 && mouseXCell - xStart == 0) {
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
		else if (mouseZCell - zStart == 0 && abs(mouseXCell - xStart) != 0) {
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
		// Перевірка можливості побити ворожу фігуру
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
		for (int z = currentZ - 1; z >= 0; z--) {
			if (fields[z][currentX] == capturePieceId) {
				fields[z][currentX] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][currentX];
				fields[z][currentX] = capturePieceId;
				if (getColor() == 'W') {
					if (checkW != -1) {
						break;
					}
				}
				else {
					if (checkB != -1) {
						break;
					}
				}
				return true;
			}
			else if (fields[z][currentX] != -1) {
				break;
			}
		}
		for (int z = currentZ + 1; z < 8; z++) {
			if (fields[z][currentX] == capturePieceId) {
				fields[z][currentX] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][currentX];
				fields[z][currentX] = capturePieceId;
				if (getColor() == 'W') {
					if (checkW != -1) {
						break;
					}
				}
				else {
					if (checkB != -1) {
						break;
					}
				}
				return true;
			}
			else if (fields[z][currentX] != -1) {
				break;
			}
		}
		for (int x = currentX - 1; x >= 0; x--) {
			if (fields[currentZ][x] == capturePieceId) {
				fields[currentZ][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ][x];
				fields[currentZ][x] = capturePieceId;
				if (getColor() == 'W') {
					if (checkW != -1) {
						break;
					}
				}
				else {
					if (checkB != -1) {
						break;
					}
				}
				return true;
			}
			else if (fields[currentZ][x] != -1) {
				break;
			}
		}
		for (int x = currentX + 1; x < 8; x++) {
			if (fields[currentZ][x] == capturePieceId) {
				fields[currentZ][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[currentZ][x];
				fields[currentZ][x] = capturePieceId;
				if (getColor() == 'W') {
					if (checkW != -1) {
						break;
					}
				}
				else {
					if (checkB != -1) {
						break;
					}
				}
				return true;
			}
			else if (fields[currentZ][x] != -1) {
				break;
			}
		}
		return false;
	}
	bool Rook::hasMove(Piece* pieces[32], int(&fields)[8][8]) {
		int currentZ = getZCenter() + 3, currentX = getXCenter() + 3;
		for (int z = currentZ - 1, x = currentX; z >= 0; z--) {
			if (fields[z][x] == -1) {
				std::swap(fields[z][x], fields[currentZ][currentX]);
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				std::swap(fields[z][x], fields[currentZ][currentX]);
				if (getColor() == 'W') {
					if (checkW == -1) {
						return true;
					}
				}
				else {
					if (checkB == -1) {
						return true;
					}
				}
			}
			else if (fields[z][x] != -1 && pieces[fields[z][x]]->getColor() != getColor()) {
				int k = fields[z][x];
				fields[z][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][x];
				fields[z][x] = k;
				if (getColor() == 'W') {
					if (checkW == -1) {
						return true;
					}
				}
				else {
					if (checkB == -1) {
						return true;
					}
				}
			}
		}
		for (int z = currentZ + 1, x = currentX; z < 8; z++) {
			if (fields[z][x] == -1) {
				std::swap(fields[z][x], fields[currentZ][currentX]);
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				std::swap(fields[z][x], fields[currentZ][currentX]);
				if (getColor() == 'W') {
					if (checkW == -1) {
						return true;
					}
				}
				else {
					if (checkB == -1) {
						return true;
					}
				}
			}
			else if (fields[z][x] != -1 && pieces[fields[z][x]]->getColor() != getColor()) {
				int k = fields[z][x];
				fields[z][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][x];
				fields[z][x] = k;
				if (getColor() == 'W') {
					if (checkW == -1) {
						return true;
					}
				}
				else {
					if (checkB == -1) {
						return true;
					}
				}
			}
		}
		for (int z = currentZ, x = currentX - 1; x >= 0; x--) {
			if (fields[z][x] == -1) {
				std::swap(fields[z][x], fields[currentZ][currentX]);
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				std::swap(fields[z][x], fields[currentZ][currentX]);
				if (getColor() == 'W') {
					if (checkW == -1) {
						return true;
					}
				}
				else {
					if (checkB == -1) {
						return true;
					}
				}
			}
			else if (fields[z][x] != -1 && pieces[fields[z][x]]->getColor() != getColor()) {
				int k = fields[z][x];
				fields[z][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][x];
				fields[z][x] = k;
				if (getColor() == 'W') {
					if (checkW == -1) {
						return true;
					}
				}
				else {
					if (checkB == -1) {
						return true;
					}
				}
			}
		}
		for (int z = currentZ, x = currentX + 1; x < 8; z++) {
			if (fields[z][x] == -1) {
				std::swap(fields[z][x], fields[currentZ][currentX]);
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				std::swap(fields[z][x], fields[currentZ][currentX]);
				if (getColor() == 'W') {
					if (checkW == -1) {
						return true;
					}
				}
				else {
					if (checkB == -1) {
						return true;
					}
				}
			}
			else if (fields[z][x] != -1 && pieces[fields[z][x]]->getColor() != getColor()) {
				int k = fields[z][x];
				fields[z][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][x];
				fields[z][x] = k;
				if (getColor() == 'W') {
					if (checkW == -1) {
						return true;
					}
				}
				else {
					if (checkB == -1) {
						return true;
					}
				}
			}
		}
		return false;
	}
}