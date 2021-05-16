#include "Bishop.h"
#include "King.h"

namespace ChessGame {
	bool Bishop::isMovePossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
		if (abs(zStart - mouseZCell) == abs(xStart - mouseXCell)) {
			if (mouseXCell < xStart&& mouseZCell < zStart) {
				for (int x = xStart - 1, z = zStart - 1; x > mouseXCell && z > mouseZCell; x--, z--) {
					if (static_cast<Bishop*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						return false;
					}
				}
			}
			else if (mouseXCell < xStart && mouseZCell > zStart) {
				for (int x = xStart - 1, z = zStart + 1; x > mouseXCell && z < mouseZCell; x--, z++) {
					if (static_cast<Bishop*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						return false;
					}
				}
			}
			else if (mouseXCell > xStart && mouseZCell < zStart) {
				for (int x = xStart + 1, z = zStart - 1; x < mouseXCell && z > mouseZCell; x++, z--) {
					if (static_cast<Bishop*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						return false;
					}
				}
			}
			else if (mouseXCell > xStart && mouseZCell > zStart) {
				for (int x = xStart + 1, z = zStart + 1; x < mouseXCell && z < mouseZCell; x++, z++) {
					if (static_cast<Bishop*>(pieces[fields[z][x]]) != this && fields[z][x] != -1) {
						return false;
					}
				}
			}
			return true;
		}
		else { // Якщо хід некоректний
			return false;
		}
	}
	bool Bishop::isHitPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) {
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
	bool Bishop::check(Piece* pieces[32], int(&fields)[8][8]) {
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
		for (int z = currentZ + 1, x = currentX + 1; z < 8 && x < 8; z++, x++) {
			if (fields[z][x] != -1) {
				Piece* p = pieces[fields[z][x]];
				if (typeid(*p) == typeid(King) && p->getColor() != this->getColor()) {
					return true;
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
					return true;
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
					return true;
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
					return true;
				}
				else {
					break;
				}
			}
		}
		return false;
	}
	bool Bishop::capture(Piece* pieces[32], int(&fields)[8][8], int capturePieceId) {
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
		for (int z = currentZ + 1, x = currentX + 1; z < 8 && x < 8; z++, x++) {
			if (fields[z][x] == capturePieceId) {
				fields[z][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][x];
				fields[z][x] = capturePieceId;
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
			else if (fields[z][x] != -1) {
				break;
			}
		}
		for (int z = currentZ + 1, x = currentX - 1; z < 8 && x >= 0; z++, x--) {
			if (fields[z][x] == capturePieceId) {
				fields[z][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][x];
				fields[z][x] = capturePieceId;
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
			else if (fields[z][x] != -1) {
				break;
			}
		}
		for (int z = currentZ - 1, x = currentX + 1; z >= 0 && x < 8; z--, x++) {
			if (fields[z][x] == capturePieceId) {
				fields[z][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][x];
				fields[z][x] = capturePieceId;
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
			else if (fields[z][x] != -1) {
				break;
			}
		}
		for (int z = currentZ - 1, x = currentX - 1; z >= 0 && x >= 0; z--, x--) {
			if (fields[z][x] == capturePieceId) {
				fields[z][x] = fields[currentZ][currentX];
				fields[currentZ][currentX] = -1;
				int checkW = static_cast<King*>(pieces[28])->isCheckOccurred(pieces, fields);
				int checkB = static_cast<King*>(pieces[4])->isCheckOccurred(pieces, fields);
				fields[currentZ][currentX] = fields[z][x];
				fields[z][x] = capturePieceId;
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
			else if (fields[z][x] != -1) {
				break;
			}
		}
		return false;
	}
	bool Bishop::hasMove(Piece* pieces[32], int(&fields)[8][8]) {
		int currentZ = getZCenter() + 3, currentX = getXCenter() + 3;
		for (int z = currentZ - 1, x = currentX - 1; z >= 0 && x >= 0; z--, x--) {
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
		for (int z = currentZ + 1, x = currentX - 1; z < 8 && x >= 0; z++, x--) {
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
		for (int z = currentZ - 1, x = currentX + 1; z >= 0 && x < 8; z--, x++) {
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
		for (int z = currentZ + 1, x = currentX + 1; z < 8 && x < 8; z++, x++) {
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
