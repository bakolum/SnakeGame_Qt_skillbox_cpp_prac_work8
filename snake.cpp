#include "snake.h"
#include <QPainter>
#include <QTime>
#include <QRandomGenerator>
#include <QDebug>
#include <QDirIterator>

// Конструктор игры
Snake::Snake(QWidget *parent) : QWidget(parent) {
    setStyleSheet("background-color: black;");      // Задаём чёрный фон
    setFixedSize(300, 300);                         // размер окна
    setFocusPolicy(Qt::StrongFocus);                // чтобы ловить стрелки
    loadImages();
    initGame();
}



void Snake::loadImages() {
    apple.load(":/img/apple.png");  // Яблоко
    dot.load(":/img/dot.png");      // Сегмент змейки
    head.load(":/img/head.png");    // Голова змейки
}


// Начальная настройка игры
void Snake::initGame() {
    dots = 4;   // Начальная длина змейки

    const int grid = DOT_SIZE;
    const int cx = (width()  / 2 / grid) * grid;   // центр по сетке
    const int cy = (height() / 2 / grid) * grid;

    for (int i = 0; i < dots; ++i) {
        x[i] = cx - i * grid;   // голова в центре, хвост влево
        y[i] = cy;
    }

    leftDirection = false;
    rightDirection = true;
    upDirection = false;        // Начальное направление — вправо
    downDirection = false;

    inGame = true;              // Игра активна
    locateApple();              // Размещаем яблоко
    timerId = startTimer(140);  // Запускаем таймер с интервалом 140 мс
}


void Snake::paintEvent(QPaintEvent *) {
    QPainter qp(this);
    qp.setRenderHint(QPainter::Antialiasing);
    qp.fillRect(rect(), Qt::black);

    if (inGame) {
        // яблоко
        qp.drawImage(QRect(apple_x, apple_y, DOT_SIZE, DOT_SIZE), apple);

        // змея
        for (int i = 0; i < dots; ++i) {
            const QRect cell(x[i], y[i], DOT_SIZE, DOT_SIZE);
            if (i == 0)  qp.drawImage(cell, head);
            else         qp.drawImage(cell, dot);
        }
    } else {
        QFont font("Courier", 20, QFont::Bold);
        qp.setPen(Qt::white);
        qp.setFont(font);
        qp.drawText(rect(), Qt::AlignCenter, "Game Over");
    }
}

// Обработка события таймера
void Snake::timerEvent(QTimerEvent *) {
    if (inGame) {
        move();                 // Двигаем змейку
        checkCollision();       // Проверяем столкновения
        checkApple();           // Проверяем, съели ли яблоко
    }
    repaint();                  // Перерисовываем экран
}

void Snake::keyPressEvent(QKeyEvent *e) {
    int key = e->key();

    // Меняем направление движения при нажатии стрелок
    if (key == Qt::Key_Left && !rightDirection) {
        leftDirection = true;
        upDirection = downDirection = false;
    } else if (key == Qt::Key_Right && !leftDirection) {
        rightDirection = true;
        upDirection = downDirection = false;
    } else if (key == Qt::Key_Up && !downDirection) {
        upDirection = true;
        leftDirection = rightDirection = false;
    } else if (key == Qt::Key_Down && !upDirection) {
        downDirection = true;
        leftDirection = rightDirection = false;
    }
}

void Snake::move() {
    for (int i = dots; i > 0; --i) {
        x[i] = x[i - 1];
        y[i] = y[i - 1];
    }

    if (leftDirection)
        x[0] -= DOT_SIZE;
    else if (rightDirection)
        x[0] += DOT_SIZE;
    else if (upDirection)
        y[0] -= DOT_SIZE;
    else if (downDirection)
        y[0] += DOT_SIZE;
}

void Snake::checkApple() {
    if (x[0] == apple_x && y[0] == apple_y) {
        ++dots;
        locateApple();
    }
}

void Snake::locateApple() {
    const int cols = width()  / DOT_SIZE;
    const int rows = height() / DOT_SIZE;

    int r = QRandomGenerator::global()->bounded(cols);
    apple_x = r * DOT_SIZE;

    r = QRandomGenerator::global()->bounded(rows);
    apple_y = r * DOT_SIZE;
}

// Проверка столкновений
void Snake::checkCollision() {
    for (int i = 1; i < dots; ++i) {           // со 2-го сегмента до хвоста
        if (x[0] == x[i] && y[0] == y[i]) {
            inGame = false;
            break;
        }
    }

    // Столкновение с границами окна
    if (x[0] >= width() || x[0] < 0 || y[0] >= height() || y[0] < 0)
        inGame = false;

    // Если игра окончена — останавливаем таймер
    if (!inGame) {
        if (timerId) killTimer(timerId);
    }
}