#ifndef Piece_h
#define Piece_h

#include <gl/glut.h>
#include <iostream>
#include "Shape.h"

namespace ChessGame {
	class Piece : public Shape {
	private:
		int id;
		char color;
		float radius;
		GLuint texture = 0;
		bool firstMove = true;
	public:
		Piece(int id, char color, float xCenter, float yCenter, float zCenter,
			float xSize, float ySize, float zSize,
			float* diffColor, float* ambiColor, float* specColor, float shininess, float radius, bool firstMove = true)
			: Shape(xCenter, yCenter, zCenter,
				xSize, ySize, zSize,
				specColor, diffColor, ambiColor, shininess) {
			this->id = id;
			this->color = color;
			this->radius = radius;
		}

		int getId() { return id; }
		void setColor(char color) { this->color = color; }
		char getColor() { return color; }
		float getRadius() { return radius; }
		void setRadius(float radius) { this->radius = radius; }
		void setFirstMove(bool firstMove) { this->firstMove = firstMove; }
		bool getFirstMove() { return firstMove; }

		virtual void draw() override;
		// Перевіряє хід на коректність
		virtual bool isMovePossible(Piece *pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) = 0;
		// Перевіряє, чи можливо побити побити ворожу фігуру
		virtual bool isHitPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) = 0;
		// Перевіряє, чи спричинила дана фігура шах
		virtual bool check(Piece* pieces[32], int(&fields)[8][8]) = 0;
		// Перевіряє, чи знаходиться дана фігура під загрозою захвату
		bool isCapturePossible(Piece* pieces[32], int(&fields)[8][8]);
		// Перевіряє, чи загрожує дана фігура захватом фігурі з ідентифікатором capturePieceId
		virtual bool capture(Piece* pieces[32], int(&fields)[8][8], int capturePieceId) = 0;
		// Переміщує фігуру з однієї позиції в іншу
		void movePieceToPosition(int (&fields)[8][8], int zDestination, int xDestionation, int currentZ, int currentX);
		// Завантажує вказану текстуру
		void loadTexture(const char* imagePath);
	};
}

#endif