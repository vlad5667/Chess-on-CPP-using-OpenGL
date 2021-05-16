#include "utils.h"

namespace GraphUtils {
	float diffWhite[] = { 0.4f, 0.4f, 0.4f };
	float ambiWhite[] = { 0.25f, 0.25f, 0.25f };
	float specWhite[] = { 0.774597f, 0.774597f, 0.774597f };

	float diffBlack[] = { 0.01f, 0.01f, 0.01f };
	float ambiBlack[] = { 0.02f, 0.02f, 0.02f };
	float specBlack[] = { 0.4f, 0.4f, 0.4f };

	float diffBlue[] = { 0.0f, 0.0f, 0.6f };
	float ambiBlue[] = { 0.1f, 0.1f, 0.2f };
	float specBlue[] = { 0.2f, 0.2f, 0.8f };

	float diffGray[] = { 0.6f, 0.6f, 0.6f };
	float ambiGray[] = { 0.2f, 0.2f, 0.2f };
	float specGray[] = { 0.8f, 0.8f, 0.8f };

	float diffRed[] = { 0.6f, 0.0f, 0.0f };
	float ambiRed[] = { 0.2f, 0.1f, 0.1f };
	float specRed[] = { 0.8f, 0.2f, 0.2f };

	float diffYellow[] = { 0.9f, 0.9f, 0 };
	float ambiYellow[] = { 0.2f, 0.2f, 0.1f };
	float specYellow[] = { 1.0f, 1.0f, 0.2f };

	float diffGreen[] = { 0, 0.5f, 0 };
	float ambiGreen[] = { 0.1f, 0.2f, 0.1f };
	float specGreen[] = { 0.2f, 0.7f, 0.2f };

	float diffOrange[] = { 0.9f, 0.2f, 0 };
	float ambiOrange[] = { 0.2f, 0.2f, 0.2f };
	float specOrange[] = { 0.8f, 0.8f, 0.8f };

	float diffLightBlue[] = { 0, 0.6f, 0.9f };
	float ambiLightBlue[] = { 0.2f, 0.2f, 0.2f };
	float specLightBlue[] = { 0.8f, 0.8f, 0.8f };

	float diffViolet[] = { 0.5f, 0, 0.5f };
	float ambiViolet[] = { 0.2f, 0.2f, 0.2f };
	float specViolet[] = { 0.8f, 0.8f, 0.8f };

	void shuffle(int *a, int size) {
		srand((unsigned)time(0));
		std::random_shuffle(a, a + size);
	}

	void parallelepiped(float length, float width, float height) {
		glBegin(GL_QUAD_STRIP);
		//грань 1 || YZ, x<0
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-length / 2, -width / 2, -height / 2);
		glVertex3f(-length / 2, -width / 2, height / 2);
		glVertex3f(-length / 2, width / 2, -height / 2);
		glVertex3f(-length / 2, width / 2, height / 2);

		//грань 2 || ZX, y>0
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(length / 2, width / 2, -height / 2);
		glVertex3f(length / 2, width / 2, height / 2);

		//грань 3 || YZ, x>0
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f(length / 2, -width / 2, -height / 2);
		glVertex3f(length / 2, -width / 2, height / 2);

		//грань 4 || ZX y<0
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(-length / 2, -width / 2, -height / 2);
		glVertex3f(-length / 2, -width / 2, height / 2);
		glEnd();

		glBegin(GL_QUADS);
		//грань 5 || YX, z>0
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-length / 2, -width / 2, height / 2);
		glVertex3f(-length / 2, width / 2, height / 2);
		glVertex3f(length / 2, width / 2, height / 2);
		glVertex3f(length / 2, -width / 2, height / 2);

		//грань 6  || YX, z<0
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(-length / 2, -width / 2, -height / 2);
		glVertex3f(-length / 2, width / 2, -height / 2);
		glVertex3f(length / 2, width / 2, -height / 2);
		glVertex3f(length / 2, -width / 2, -height / 2);
		glEnd();
	}

	// Відображення рядка тексту вказаним шрифтом у вказаній позиції
	void drawString(void *font, const char* text, float x, float y) {
		if (!text) // нульовий указівник
		{
			return;
		}
		// Встановлення позиції тексту:
		glRasterPos2f(x, y);
		while (*text)
		{
			// Рядок виводиться посимвольно:
			glutBitmapCharacter(font, *text);
			text++;
		}
	}

	int binarySearch(std::pair<int, int>* arr, int n, int x) {
		int l = 0, r = n;
		while (l <= r) {
			int mid = (l + r) / 2;
			if (x > arr[mid].first) {
				l = mid + 1;
			}
			else if (x < arr[mid].first) {
				r = mid - 1;
			}
			else {
				return arr[mid].second;
			}
		}
		return -1;
	}

	bool sortComp(std::pair<int, int> A, std::pair<int, int> B) {
		return A.first < B.first;
	}
}