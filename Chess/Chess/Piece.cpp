#include <gl/glut.h>
#include "Piece.h"
#include "utils.h"
#include "Scene.h"

namespace ChessGame {
	void Piece::draw() {
        // ���������� ������������ ��������:
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, getAmbiColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, getDiffColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, getSpecColor());
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, getShininess());
        // ����� ������� ������� � ����
        // (���������� ����� ������� ������� ��� ���������� ������������):
        glPushMatrix();
        glTranslatef(getXCenter(), getYCenter() + getYSize() / 2, getZCenter());
        // ������ ������� ���� ������������ � ������������� ��������:
        glRotatef(90, 1, 0, 0);
        GLUquadricObj* quadricObj = gluNewQuadric();
        gluQuadricDrawStyle(quadricObj, GLU_FILL);
        gluCylinder(quadricObj, radius, radius, getYSize(), 20, 20);
        // ���� ������� ���� ������������ ��������� ������ ������:
        glRotatef(180, 1, 0, 0);
        // ������� ���� ������:
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        gluQuadricTexture(quadricObj, GL_TRUE);
        gluDisk(quadricObj, 0, radius, 20, 20);
        glDisable(GL_TEXTURE_2D);

        // ������� ���� �����:
        glTranslatef(0, 0, -getYSize());
        gluDisk(quadricObj, 0, radius, 20, 20);
        gluDeleteQuadric(quadricObj);
        // ³��������� ������� ������� � �����:
        glPopMatrix();
	}
    void Piece::movePieceToPosition(int(&fields)[8][8], int zDestination, int xDestionation, int currentZ, int currentX) {
        if (zDestination == currentZ && xDestionation == currentX) {
            return;
        }
        this->setXCenter(xDestionation - 3);
        this->setZCenter(zDestination - 3);
        fields[zDestination][xDestionation] = fields[currentZ][currentX];
        fields[currentZ][currentX] = -1;
    }
}