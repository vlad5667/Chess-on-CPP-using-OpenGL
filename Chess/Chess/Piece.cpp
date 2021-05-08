#include <gl/glut.h>
#include "Piece.h"
#include "utils.h"
#include "Scene.h"

namespace ChessGame {
	void Piece::draw() {
        // Визначення властивостей матеріалу:
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, getAmbiColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, getDiffColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, getSpecColor());
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, getShininess());
        // Запис поточної матриці в стек
        // (збереження вмісту поточної матриці для подальшого використання):
        glPushMatrix();
        glTranslatef(getXCenter(), getYCenter() + getYSize() / 2, getZCenter());
        // Циліндр повинен бути розташований у вертикальному напрямку:
        glRotatef(90, 1, 0, 0);
        GLUquadricObj* quadricObj = gluNewQuadric();
        gluQuadricDrawStyle(quadricObj, GLU_FILL);
        gluCylinder(quadricObj, radius, radius, getYSize(), 20, 20);
        // Диск повинен бути намальований зовнішньою гранню догори:
        glRotatef(180, 1, 0, 0);
        // Малюємо диск зверху:
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        gluQuadricTexture(quadricObj, GL_TRUE);
        gluDisk(quadricObj, 0, radius, 20, 20);
        glDisable(GL_TEXTURE_2D);

        // Малюємо диск знизу:
        glTranslatef(0, 0, -getYSize());
        gluDisk(quadricObj, 0, radius, 20, 20);
        gluDeleteQuadric(quadricObj);
        // Відновлення поточної матриці зі стека:
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