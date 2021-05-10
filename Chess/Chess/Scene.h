#ifndef Scene_h
#define Scene_h

#include "Shape.h"
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

#include <vector>

namespace ChessGame {
	const int M = 8, N = 8; // ������� ����� �� ������� ����

							// �������� ���� ���, ���� ����������� �������� �����,
							// ��������� ������� ���, ������������ ��������,
							// � ����� ������ ������� ���� GLUT
	class Scene {
		std::vector<Shape*> shapes; // "�������" ����� ��������� �� �������� ���
		Piece* pieces[4 * N];
		int button;           // ������ ���� (-1 - �� ���������, 0 - ���, 2 - �����)
		float angleX, angleY; // �������� ��� �������� ����� 
		float mouseX, mouseY; // ������ ���������� ����
		float width, height;  // ������ ����
		float distZ;          // ������� �� �� Z �� �����
		int time;
		int fields[N][M];
		int mouseXCell, mouseZCell; // ���������� �������, �� ��� ����� ����, ���� ������ ������
		int currentX, currentZ; // ������ ���������� ������
		int xStart, zStart;	// ���������� ������ �� ����������
		int xTo, zTo; // ���������� ������ ����������� ������
		bool whiteMove = true; // ��� (false - ����, true - ��)
	public:
		Scene();
		~Scene();
		void on_paint();
		void on_size(int width, int height);
		void on_mouse(int button, int state, int x, int y);
		void on_motion(int x, int y);
		void on_special(int key, int x, int y);
		void on_keyboard(unsigned char key, int x, int y);
		void on_timer(int value);
	private:
		void initialize();
		void upPiece(int x, int z);
		bool movePiece(int xFrom, int zFrom, int xTo, int zTo);
		void downAllPieces();
		void resetPieces();
		bool findNearest(int x, int y, int& x1, int& z1);
	};

}

#endif