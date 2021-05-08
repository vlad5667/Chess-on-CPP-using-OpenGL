#include <gl/glut.h>
#include "Cube.h"
#include "utils.h"

namespace ChessGame {

	void Cube::draw()
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, getAmbiColor());
		glMaterialfv(GL_FRONT, GL_DIFFUSE, getDiffColor());
		glMaterialfv(GL_FRONT, GL_SPECULAR, getSpecColor());
		glMaterialf(GL_FRONT, GL_SHININESS, getShininess());
		glPushMatrix();
		glTranslatef(getXCenter(), getYCenter(), getZCenter());
		GraphUtils::parallelepiped(getXSize(), getYSize(), getZSize());
		glPopMatrix();
	}

}