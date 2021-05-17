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

#include <string>
#include <vector>

namespace ChessGame {
	const int M = 8, N = 8; // ������� ����� �� ������� ����

	struct recordRow {
		std::string whitePlayerName;
		std::string blackPlayerName;
		std::string winner;
		std::string moves;
		std::string time;

		recordRow(std::string whitePlayerName, std::string blackPlayerName, std::string winner, std::string moves, std::string time) {
			this->whitePlayerName = whitePlayerName;
			this->blackPlayerName = blackPlayerName;
			this->winner = winner;
			this->moves = moves;
			this->time = time;
		}
	};

	enum Modes {
		inputFirstPlayerNameMode, // �������� ��� ����� �������� ���� ������� ������
		inputSecondPlayerNameMode, // �������� ��� ����� �������� ���� ������� ������
		selectDifficultyMode, // �������� ��� ����� ������ ���������
		recordsMode, // �������� ��� ����� ��������� ������� �������
		gameMode // �������� ��� ����� ���
	};

							// �������� ���� ���, ���� ����������� �������� �����,
							// ��������� ������� ���, ������������ ��������,
							// � ����� ������ ������� ���� GLUT
	class Scene {
		Modes currentMode; // �������� ����� �����
		std::string firstPlayerName = "", secondPlayerName = ""; // ����, �� ��������� ����� �������
		std::string whitePlayerName, blackPlayerName; // ����, �� ��������� ����� ����� � ������� ������
		std::string winner;
		std::string timeString; // �����, �� �������� ������
		std::vector<recordRow> recordsEasy; // ����� ������� ������� ���� ���������
		std::vector<recordRow> recordsNormal; // ����� ������� ����������� ���� ���������
		std::vector<recordRow> recordsHard; // ����� ������� ��������� ���� ����������
		std::vector<std::string> recordsTable; // ������� �������
		std::vector<Shape*> shapes; // "�������" ����� ��������� �� �������� ���
		Piece* pieces[4 * N]; // ����� �����
		int difficulty; // �����, �� ����� �� ��������� ��� (1 - �����, 2 - ���������, 3 - �����)
		int button; // ������ ���� (-1 - �� ���������, 0 - ���, 2 - �����)
		float angleX, angleY; // �������� ��� �������� ����� 
		float mouseX, mouseY; // ������ ���������� ����
		float width, height;  // ������ ����
		float distZ; // ������� �� �� Z �� �����
		int time; // ��������� ����
		int moves; // ������� ���� �� �����
		int whitePlayerTime; // ��� �� ����� ����� ������
		int blackPlayerTime; // ��� �� ����� ������� ������
		int fields[N][M]; // ������������� ���� � ������ ����������� ������
		int mouseXCell, mouseZCell; // ���������� �������, �� ��� ����� ����, ���� ������ ������
		int currentX, currentZ; // ������ ���������� ������
		int xStart, zStart;	// ���������� ������ �� ����������
		int xTo, zTo; // ���������� ������ ����������� ������
		bool whiteMove; // ��� (false - ����, true - ��)
		bool finish; // �����, �� ��������, �������� ����� �� �
		
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
		void readRecords();
		void writeRecords();
		void addRecord();
		void clearRecords();
		void checkConditions();
	};

}

#endif