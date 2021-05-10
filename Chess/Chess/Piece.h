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
		GLuint texture;
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
		virtual bool correctMove(Piece *pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) = 0;
		virtual bool isHitPossible(Piece* pieces[32], int(&fields)[8][8], int zStart, int xStart, int mouseZCell, int mouseXCell) = 0;
		virtual int check(Piece* pieces[32], int(&fields)[8][8]) = 0;
		void movePieceToPosition(int (&fields)[8][8], int zDestination, int xDestionation, int currentZ, int currentX);
		void loadTexture(const char* imagePath);
	};
}

#endif