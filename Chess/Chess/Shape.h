#ifndef Shape_h
#define Shape_h

namespace ChessGame {
	class Shape {
	private:
		float xCenter, yCenter, zCenter;
		float xSize, ySize, zSize;
		float *diffColor, *ambiColor, *specColor, shininess;
	public:
		Shape() {}
		Shape(float xCenter, float yCenter, float zCenter,
			float xSize, float ySize, float zSize,
			float *diffColor, float *ambiColor, float *specColor, float shininess);
		float  getXCenter() const { return xCenter; }
		float  getYCenter() const { return yCenter; }
		float  getZCenter() const { return zCenter; }
		void   setXCenter(float xCenter) { this->xCenter = xCenter; }
		void   setYCenter(float yCenter) { this->yCenter = yCenter; }
		void   setZCenter(float zCenter) { this->zCenter = zCenter; }
		void   setCoords(float xCenter, float yCenter, float zCenter);
		float  getXSize() const { return xSize; }
		float  getYSize() const { return ySize; }
		float  getZSize() const { return zSize; }
		void   setXSize(float xSize) { this->xSize = xSize; }
		void   setYSize(float ySize) { this->ySize = ySize; }
		void   setZSize(float zSize) { this->zSize = zSize; }
		void   setSize(float xSize, float ySize, float zSize);
		float* getDiffColor() const { return diffColor; }
		float* getAmbiColor() const { return ambiColor; }
		float* getSpecColor() const { return specColor; }
		float getShininess() const { return shininess; }
		void   setDiffColor(float* diffColor) { this->diffColor = diffColor; }
		void   setAmbiColor(float* ambiColor) { this->ambiColor = ambiColor; }
		void   setSpecColor(float* specColor) { this->specColor = specColor; }
		void setShininess(float shininnes) { this->shininess = shininess; }
		void   setColors(float* diffColor, float* ambiColor, float* specColor, float shininess);
		virtual void draw() = 0;
	};
}

#endif