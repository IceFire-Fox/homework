#include "drawwindow.h"
#include "ui_drawwindow.h"

#include "bezier.h"

QLabel *showTmp;
QTabWidget *MainView;

drawWindow::drawWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::drawWindow)
{
    ui->setupUi(this);
    space = new drawSpace;

    QAction *CreatePage = new QAction("New a page", this);
    CreatePage->setShortcut(QKeySequence::AddTab);
    CreatePage->setStatusTip("create a tab");
    connect(CreatePage, SIGNAL(triggered()), this, SLOT(add()));

    QMenuBar *menubar = new QMenuBar;
    QMenu *CreateMenu = menubar->addMenu("Create");

    CreateMenu->addAction(CreatePage);
    this->setMenuBar(menubar);

    QHBoxLayout *hlay = new QHBoxLayout;

    showTmp = new QLabel();
    showTmp->setAttribute(Qt::WA_DeleteOnClose);

    MainView = new QTabWidget();
    MainView->setTabShape(QTabWidget::Rounded);
    MainView->addTab(space, "MainView");
    MainView->addTab(showTmp, "showTest");

    MainView->setTabsClosable(true);
    connect(MainView->tabBar(), &QTabBar::tabCloseRequested, MainView->tabBar(), &QTabBar::removeTab);
    connect(MainView->tabBar(), &QTabBar::tabBarDoubleClicked, this, &drawWindow::tabBarDoubleClickEvent);
    //MainView->tabBar()->tabBarDoubleClicked(MainView->tabBar()->currentIndex());
    //MainView->tabBar()->tabBarDoubleClicked(1);

    MainView->setFixedSize(QSize(800, 450));
    MainView->setAttribute(Qt::WA_DeleteOnClose);
    MainView->setUsesScrollButtons(true);

    hlay->addWidget(MainView);

    free(centralWidget()->layout());
    this->centralWidget()->setLayout(hlay);

    startTimer(0);
}

drawWindow::~drawWindow()
{
    delete ui;
}

void drawWindow::timerEvent(QTimerEvent *event) {
    Q_UNUSED(event);
    showTmp->setPixmap(QPixmap::fromImage(*(space->image)));
}

void drawWindow::add() {
    static int a = 0;
    drawSpace *newSpace = new drawSpace(this, 0);
    newSpace->setAttribute(Qt::WA_DeleteOnClose);

    MainView->addTab(newSpace, QString::number(a++));
    MainView->setCurrentIndex(MainView->count()-1);
    MainView->setTabText(MainView->tabBar()->currentIndex(), "owo"+QString::number(a));
    //emit MainView->tabBarDoubleClicked(MainView->tabBar()->currentIndex());
}

void drawWindow::tabBarDoubleClickEvent() {
    qDebug("test");
    QPixmap map(MainView->currentWidget()->size());
    MainView->currentWidget()->render(&map);
    map.save("mytest"+QString::number(MainView->currentIndex())+".jpg");
    //MainView->currentWidget()
}
