#include "Board.h"

namespace ChessGame {

	Board::Board() {
		// Додаємо дошку для шахів
		for (int i = -3; i < 5; i++) {
			for (int j = -3; j < 5; j++) {
				if (i % 2 != 0) {
					if (j % 2 == 0) {
						cells.push_back(new Cube(j, 0, i, 1, 0.5, 1, diffBlack, ambiBlack, specBlack, 0.078125));
					}
					else {
						cells.push_back(new Cube(j, 0, i, 1, 0.5, 1, diffWhite, ambiWhite, specWhite, 0.6));
					}
				}
				else {
					if (j % 2 != 0) {
						cells.push_back(new Cube(j, 0, i, 1, 0.5, 1, diffBlack, ambiBlack, specBlack, 0.078125));
					}
					else {
						cells.push_back(new Cube(j, 0, i, 1, 0.5, 1, diffWhite, ambiWhite, specWhite, 0.6));
					}
				}
			}
		}
	}

	Board::~Board() {
		for (int i = 0; i < cells.size(); i++) {
			delete cells[i];
		}
	}

	void Board::draw() {
		for (int i = 0; i < cells.size(); i++) {
			cells[i]->draw();
		}
	}
}