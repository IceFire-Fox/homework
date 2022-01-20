#ifndef DRAWSPACE_H
#define DRAWSPACE_H

#include <QtGui>
#include <QWidget>

#include <time.h>

extern "C" {
#include "linkedlist.h"
}
#include "bezier.h"

class drawSpace : public QWidget
{
    Q_OBJECT
public:
    explicit drawSpace(QWidget *parent = nullptr, bool connect = true);
    bool press_board;
    bool scribbling;
    QImage *image;
    QColor color = QColor(133, 170, 40);
    int fd = -1, dots = 0, thk = 3;

    void plotHandwriting (QPointF *p, QPointF *plot, int repair, QColor color, int thickness);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void timerEvent(QTimerEvent *event);

signals:

};

#endif // DRAWSPACE_H
