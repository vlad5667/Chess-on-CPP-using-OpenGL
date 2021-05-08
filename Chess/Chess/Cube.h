#ifndef Cube_h
#define Cube_h

#include "Shape.h"

namespace ChessGame {
	// Клас, який відповідає за малювання дошки (поля гри)
	class Cube : public Shape {
	public:
		Cube(float xCenter, float yCenter, float zCenter,
			float xSize, float ySize, float zSize,
			float* diffColor, float* ambiColor, float* specColor, float shininess)
			: Shape(xCenter, yCenter, zCenter,
				xSize, ySize, zSize,
				specColor, diffColor, ambiColor, shininess) { }
		virtual void draw();
	};

}
#endif