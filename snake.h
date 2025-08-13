#ifndef SNAKE_H
#define SNAKE_H

#include <QWidget>
#include <QKeyEvent>
#include <QTimer>

// Класс Snake — основной виджет игры "Змейка"
class Snake : public QWidget {
    Q_OBJECT

public:
    Snake(QWidget *parent = nullptr);
    // Размер окна
    QSize sizeHint() const override { return QSize(300, 300); }

protected:
    // Отрисовка игры (змейки, яблока, экрана Game Over)
    void paintEvent(QPaintEvent *) override;
    // Событие таймера: вызывается каждые N миллисекунд для обновления игры
    void timerEvent(QTimerEvent *) override;
    // Обработка нажатий клавиш (стрелок для управления змейкой)
    void keyPressEvent(QKeyEvent *) override;

private:
    static const int DOT_SIZE = 10;     // Размер одного сегмента змейки (и клетки сетки), в пикселях
    static const int ALL_DOTS = 900;    // Максимальное количество сегментов змейки (размер массива координат)
    static const int RAND_POS = 29;     // Параметр для генерации случайных координат яблока

    int timerId;
    int dots;       // Текущая длина змейки (в сегментах)

    // Координаты яблока
    int apple_x;
    int apple_y;

    // Массивы координат сегментов змейки (x и y)
    int x[ALL_DOTS];
    int y[ALL_DOTS];

    // Направления движения змейки (флаги)
    bool leftDirection;
    bool rightDirection;
    bool upDirection;
    bool downDirection;

    bool inGame;            // Флаг состояния игры (true — игра идет, false — Game Over)

    // Изображения сегмента змейки, яблока и головы
    QImage dot;
    QImage apple;
    QImage head;

    void loadImages();          // Загрузка изображений
    void initGame();            // Начальная инициализация игры
    void locateApple();         // Случайное размещение яблока на поле
    void move();                // Движение змейки в текущем направлении
    void checkApple();          // Проверка, съела ли змейка яблоко
    void checkCollision();      // Проверка столкновений с границами поля или самой собой
};

#endif // SNAKE_H