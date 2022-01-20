#ifndef DRAWWINDOW_H
#define DRAWWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLayout>
#include <QImage>
#include <QAction>
#include <QTabBar>
#include <QMenuBar>

#include "drawspace.h"

QT_BEGIN_NAMESPACE
namespace Ui { class drawWindow; }
QT_END_NAMESPACE

class drawWindow : public QMainWindow
{
    Q_OBJECT

public:
    drawWindow(QWidget *parent = nullptr);
    ~drawWindow();
    drawSpace *space;
    void timerEvent(QTimerEvent *event);

public slots:
    void add();
    //void paintEvent

private:
    Ui::drawWindow *ui;
    void tabBarDoubleClickEvent();
};
#endif // DRAWWINDOW_H
