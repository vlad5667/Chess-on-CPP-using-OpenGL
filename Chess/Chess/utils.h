#ifndef utils_h
#define utils_h

#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include <gl/glut.h>

namespace GraphUtils {
	extern float diffWhite[];
	extern float ambiWhite[];
	extern float specWhite[];

	extern float diffBlack[];
	extern float ambiBlack[];
	extern float specBlack[];

	extern float diffBlue[];
	extern float ambiBlue[];
	extern float specBlue[];

	extern float diffGray[];
	extern float ambiGray[];
	extern float specGray[];

	extern float diffRed[];
	extern float ambiRed[];
	extern float specRed[];

	extern float diffYellow[];
	extern float ambiYellow[];
	extern float specYellow[];

	extern float diffGreen[];
	extern float ambiGreen[];
	extern float specGreen[];

	extern float diffOrange[];
	extern float ambiOrange[];
	extern float specOrange[];

	extern float diffLightBlue[];
	extern float ambiLightBlue[];
	extern float specLightBlue[];

	extern float diffViolet[];
	extern float ambiViolet[];
	extern float specViolet[];

	void shuffle(int *a, int size);

	void parallelepiped(float length, float width, float height);

	void drawString(void *font, const char* text, float x, float y);

	int binarySearch(std::pair<int, int>* arr, int n, int x);

	bool sortComp(std::pair<int, int> A, std::pair<int, int> B);
}

#endif