#ifndef Board_h
#define Board_h

#include "Shape.h"
#include "Cube.h"
#include "utils.h"
#include <vector>

namespace ChessGame {
	using namespace GraphUtils;
	class Board : public Shape {
		std::vector<Cube*> cells;
	public:
		Board();
		~Board();

		virtual void draw() override;
	};
}

#endif