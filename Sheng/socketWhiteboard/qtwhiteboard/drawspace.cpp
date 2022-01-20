#include "drawspace.h"

int clientInit (int port, char *address);
Node_s **linkedlistInit ();
void linkedlistFree (Node_s **list);

QPointF p[DOTS];
Node_s **sendList, **recvList;

drawSpace::drawSpace(QWidget *parent, bool connect)
    : QWidget{parent}
{
    int port = 2244;
    char host[] = "localhost";
    qDebug("update %d", connect);


    scribbling = false;
    setAttribute(Qt::WA_StaticContents);
    image = new QImage(800, 450, QImage::Format_RGB32);
    image->fill(qRgb(255, 255, 255));
    setAttribute(Qt::WA_DeleteOnClose);

    if (connect) {
        sendList = linkedlistInit();
        recvList = linkedlistInit();

        if ((fd = clientInit(port, host)) < 0) {
            printf("ERROR:Can't not connect to %s:%d....\nstart up offline mode\n", host, port);
            linkedlistFree(sendList);
            linkedlistFree(recvList);
        } else {
            startTimer(0);
        }
    }
}


Node_s **linkedlistInit () {
    Node_s **list;
    list = (Node_s **)malloc(sizeof(Node_s *));
    *list = (Node_s *)malloc(sizeof(Node_s));
    (*list)->next = NULL;
    return list;
}

void linkedlistFree (Node_s **list) {
    free(*list);
    free(list);
}

void drawSpace::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        p[0] = event->pos();

        if (fd > 0) {
            Pos tmp = {(int) p[0].x(),
                       (int) p[0].y()};
            dots++;
            add(sendList, tmp);
        }
        scribbling = true;
        for (int i = 1; i < DOTS; i++) {
            p[i] = QPoint(-1, -1);
        }
    }
}


void drawSpace::mouseMoveEvent(QMouseEvent *event) {
    if (scribbling) {
        QPointF plot[TIMES];
        int i = 0;
        Pos tmp;

        for (i = 0; i < DOTS-1; i++) {
            if (p[i].x() == -1 && p[i].y() == -1) {
                p[i] = event->pos();
                if (fd > 0) {
                    dots++;
                    tmp.x = p[i].x();
                    tmp.y = p[i].y();
                    add(sendList, tmp);
                }
                return;
            }
        }

        p[DOTS-1] = event->pos();
        if (fd > 0) {
            dots++;
            tmp.x = p[DOTS-1].x();
            tmp.y = p[DOTS-1].y();
            add(sendList, tmp);
        }

        bezierCurve(plot, TIMES, p, DOTS);
        plotHandwriting(p, plot, 0, color, thk);
    }
}

void drawSpace::mouseReleaseEvent(QMouseEvent *event) {
    QPointF plot[TIMES];
    Pos tmp;
    if (event->button() == Qt::LeftButton && scribbling) {
        if (fd > 0) {
            //QByteArray barr = account.toLocal8Bit();
            //char *user = barr.data();
            HWCtrl c = {color.red(),
                        color.green(),
                        color.blue(),
                        thk, "null", ++dots};

            //strcpy(c.username, user);
            tmp.x = -1;
            tmp.y = 0;
            add(sendList, tmp);
            write(fd, &c, sizeof(HWCtrl));
            socket_write(sendList, fd, dots);
            cleanList(sendList);
            dots = 0;
        }
        plotHandwriting(p, plot, 1, color, thk);
        update();
        scribbling = false;
    }
}

void drawSpace::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), *image);
}

void drawSpace::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);
    /*
    QImage *newImage = new QImage(event->size(), QImage::Format_RGB32);
    newImage->fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    delete(image);
    image = &newImage;
    update();
    QWidget::resizeEvent(event);
    */

}

void drawSpace::timerEvent(QTimerEvent *event) {
    Q_UNUSED(event);
    HWCtrl c;

    //if (( n = recv(fd, &c, sizeof(HWCtrl), MSG_DONTWAIT)) > 0) {
    if (read(fd, &c, sizeof(HWCtrl)) > 0) {
        QPointF p[DOTS], plot[TIMES];
        Node_s *cur;
        QColor color = QColor(110, 160, 20);
        int i = 0, thk = 1;

        qDebug("%d %d %d, thk %d, name %s, %d\n", c.r, c.g, c.b, c.thk, c.username, c.dotslen);
        if (socket_read(recvList, fd, c.dotslen) > 0) {
            color = QColor(c.r, c.g, c.b);
            thk = c.thk;
            for (i = 0; i < DOTS; i++) {
                p[i] = QPoint(-1, -1);
            }
            cur = (*recvList)->next;
            while (cur != NULL) {
                if (cur->point.x != -1 && cur->point.y != 0) {
                    for(i = 0; i < DOTS-1; i++) {
                        if (p[i].x() == -1 && p[i].y() == -1) {
                            p[i] = QPointF(cur->point.x, cur->point.y);
                            cur = cur->next;
                            break;
                        }
                    }
                    if (i != DOTS-1) {
                        continue;
                    }
                    p[DOTS-1] = QPointF(cur->point.x, cur->point.y);
                    bezierCurve(plot, TIMES, p, DOTS);
                    plotHandwriting(p, plot, 0, color, thk);
                }
                cur = cur->next;
            }

            plotHandwriting(p, plot, 1, color, thk);
            //IDdelete(recvList, id);
            cleanList(recvList);
        }

    }
}


int clientInit (int port, char *address) {
    int fd;
    struct sockaddr_in mAddr;
    struct hostent *host;

    if ((host = gethostbyname(address)) == NULL) {
        return -1;
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&mAddr, 0, sizeof(mAddr));
    mAddr.sin_family = AF_INET;
    mAddr.sin_port = htons(port);
    memcpy(&mAddr.sin_addr, host->h_addr_list[0], host->h_length);

    if (connect(fd, (struct sockaddr *) &mAddr, sizeof(struct sockaddr_in)) < 0) {
        return -1;
    }
    if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK) < 0) {
        close(fd);
        qDebug("error");
        return -1;
    }

    return fd;
}

void drawSpace::plotHandwriting(QPointF *p, QPointF *plot, int repair, QColor color, int thickness) {
    int i, j;
    double distance = 0.0;
    QColor handwritColor = color;
    int thk = thickness;

    QPainter painter(image);
    painter.setPen(QPen(handwritColor, thk, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    if (!repair) {
        for (i = 0; i < DOTS-1; i++) {
            distance += dist(p[i], p[i+1]);
        }

        if (distance <= 70) {
            for (i = 0; i < TIMES-1; i++) {
                painter.drawLine(plot[i], plot[i+1]);
            }
            painter.drawLine(plot[TIMES-1], p[DOTS-1]);
            p[0] = p[DOTS-1];
            i = 1;
        } else {

            for (i = 0; dist(plot[i], p[DOTS-1]) > 30 && i < TIMES-3; i++) {
                painter.drawLine(plot[i], plot[i+1]);
            }
            for (j = i; dist(plot[j], p[DOTS-1]) > 21.2 && j < TIMES-2; j++);

            p[0] = plot[i];
            p[1] = plot[j];
            p[2] = p[DOTS-1];
            i = 3;

        }
    } else {
        for (i = DOTS; i >= 3; i--) {
            if (p[i-1].x() != -1 && p[i-1].y() != -1) {
                bezierCurve(plot, TIMES, p, i);
                for (j = 0; j < TIMES-1; j++) {
                    painter.drawLine(plot[j], plot[j+1]);
                }
                painter.drawLine(plot[TIMES-1], p[i-1]);
                break;
            }
        }
        if (p[1].x() != -1 && p[1].y() != -1) {
            painter.drawLine(p[0], p[1]);
        }
        i = 0;
    }

    for (; i < DOTS; i++) {
        p[i] = QPointF(-1, -1);
    }
    update();
}
