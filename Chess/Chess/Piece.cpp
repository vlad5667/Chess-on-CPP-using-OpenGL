#include <gl/glut.h>
#include "Piece.h"
#include "utils.h"
#include "Scene.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
    void Piece::loadTexture(const char* imagePath) {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Встановлюємо параметри накладання і фільтрації текстур (для поточного зв'язаного об'єкту текстури)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Завантаження і генерація текстури
        int width, height, nrChannels;
        unsigned char* image = stbi_load(imagePath, &width, &height, &nrChannels, STBI_rgb_alpha);
        if (image != nullptr) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }
        // Звільнення пам'яті
        stbi_image_free(image);
        this->texture = texture;
    }
}