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
	const int M = 8, N = 8; // к≥льк≥сть р€дк≥в та колонок пол€

	struct recordRow {
		std::string firstPlayerName;
		std::string secondPlayerName;
		std::string winner;
		std::string steps;
		std::string time;

		recordRow(std::string firstPlayerName, std::string secondPlayerName, std::string winner, std::string steps, std::string time) {
			this->firstPlayerName = firstPlayerName;
			this->secondPlayerName = secondPlayerName;
			this->winner = winner;
			this->steps = steps;
			this->time = time;
		}
	};

	enum Modes {
		inputFirstPlayerNameMode, // сигнал≥зуЇ про режим введенн€ ≥мен≥ першого гравц€
		inputSecondPlayerNameMode, // сигнал≥зуЇ про режим введенн€ ≥мен≥ другого гравц€
		selectDifficultyMode, // сигнал≥зуЇ про режим вибору складност≥
		recordsMode, // сигнал≥зуЇ про режим перегл€ду таблиц≥ рекорд≥в
		placementMode, // сигнал≥зуЇ про режим розм≥щенн€ ф≥гур на дошц≥
		gameMode // сигнал≥зуЇ про режим гри
	};

							// ќсновний клас гри, €кий представл€Ї геометр≥ю сцени,
							// контролюЇ правила гри, розташуванн€ елемент≥в,
							// а також реал≥зуЇ обробку под≥й GLUT
	class Scene {
		Modes currentMode; // поточний режим сцени
		std::string firstPlayerName = "", secondPlayerName = ""; // «м≥нн≥, що збер≥гають ≥мена гравц≥в
		std::string whitePlayer, blackPlayer; // «м≥нн≥, що збер≥гають ≥мена б≥лого ≥ чорного гравц€
		std::string timeString = ""; // «м≥нна, що в≥дображаЇ таймер
		std::vector<recordRow> recordsEasy; // масив рекорд≥в легкого р≥вню складност≥
		std::vector<recordRow> recordsNormal; // масив рекорд≥в нормального р≥вню складност≥
		std::vector<recordRow> recordsHard; // масив рекорд≥в складного р≥вню складнгост≥
		std::vector<std::string> recordsTable; // таблиц€ рекорд≥в
		std::vector<Shape*> shapes; // "гнучкий" масив указ≥вник≥в на елементи гри
		Piece* pieces[4 * N]; // масив ф≥гур
		int difficulty; // зм≥нна, що вказуЇ на складн≥сть гри (1 - легко, 2 - нормально, 3 - важко)
		int button; // кнопка миш≥ (-1 - не натиснута, 0 - л≥ва, 2 - права)
		float angleX, angleY; // поточний кут повороту сцени 
		float mouseX, mouseY; // поточн≥ координати миш≥
		float width, height;  // –озм≥ри в≥кна
		float distZ; // в≥дстань по ос≥ Z до сцени
		int time; // тривал≥сть парт≥њ
		int steps; // к≥льк≥сть ход≥в за парт≥ю
		int whitePlayerTime; // час на парт≥ю б≥лого гравц€
		int blackPlayerTime; // час на парт≥ю чорного гравц€
		int fields[N][M]; // представленн€ пол€ у вигл€д≥ двовим≥рного масиву
		int mouseXCell, mouseZCell; // координати кл≥тинки, на €ку вказуЇ миша, коли в≥джата кнопка
		int currentX, currentZ; // поточн≥ координати ф≥гури
		int xStart, zStart;	// координати ф≥гури до перем≥щенн€
		int xTo, zTo; // координати пункту призначенн€ ф≥гури
		bool whiteMove; // х≥д (false - чорн≥, true - б≥л≥)
		bool mateOccurredW, mateOccurredB; // «м≥нн≥, що сигнал≥зують, ставс€ мат чи н≥
		int delW, delB; // «м≥нн≥, що задають координату ’ дл€ побитих ф≥гур
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
		void clearRecords();
	};

}

#endif