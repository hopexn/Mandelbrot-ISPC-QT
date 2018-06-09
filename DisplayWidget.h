#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>

#include <iostream>
#include <string>
#include <boost/timer.hpp>
#include <cmath>

using namespace std;
using namespace boost;

#include "ispc/Mandelbrot.h"

class DisplayWidget : public QWidget {
Q_OBJECT
public:
    DisplayWidget(int _width, int _height) {
        width = _width;
        height = _height;
        pixels = new int[width * height];
        maxIterations = 256;
        x0 = -2;
        x1 = 1;
        y0 = -1;
        y1 = 1;
        qimage = new QImage(width, height, QImage::Format_ARGB32);
    }

    ~DisplayWidget() {
        delete pixels;
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        timer t;
        t.restart();
        ispc::mandelbrot(x0, y0, x1, y1, width, height, maxIterations, pixels);
        cout << "Time cost: " << t.elapsed() << "s" << endl;
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                int index = j * width + i;
                int gray = (pixels[index] & 0x1) ? 240 : 20;
                qimage->setPixel(i, j, qRgb(gray, gray, gray));
            }
        }

        painter.drawImage(QRect(0, 0, width, height), *qimage);
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (event->buttons()) {
            float dx = float(event->x() - lastPosition.x()) / width;
            float dy = float(event->y() - lastPosition.y()) / height;
            float s = sqrtf((x1 - x0) * (y1 - y0));

            x0 -= dx * s;
            x1 -= dx * s;
            y0 -= dy * s;
            y1 -= dy * s;
        }
        lastPosition = event->pos();
        update();
    }

    void wheelEvent(QWheelEvent *event) override {
        //缩放因子，调整滚轮缩放变化的速度
        float scale_factor = 0.88f;
        if (event->delta() < 0) {
            scale_factor = 1 / scale_factor;
        }

        //放大的焦点设置为鼠标所在的位置
        float focus_x = x0 + ((float) event->x()) / width * (x1 - x0);
        float focus_y = y0 + ((float) event->y()) / height * (y1 - y0);

        x0 = focus_x + (x0 - focus_x) * scale_factor;
        x1 = focus_x + (x1 - focus_x) * scale_factor;
        y0 = focus_y + (y0 - focus_y) * scale_factor;
        y1 = focus_y + (y1 - focus_y) * scale_factor;

        update();
    }

    void mousePressEvent(QMouseEvent *event) override {
        lastPosition = event->pos();
    }

private:
    int width, height, maxIterations;
    int *pixels;
    float x0, y0, x1, y1;

    QImage *qimage;
    QPoint lastPosition;
};
