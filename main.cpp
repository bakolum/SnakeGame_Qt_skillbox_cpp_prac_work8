#include <QApplication>
#include "snake.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Snake snakeGame;
    snakeGame.show();
    return app.exec();
}