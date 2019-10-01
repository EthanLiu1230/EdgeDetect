#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    int scaleFactor = 100;
    explicit MyGraphicsView(QWidget *parent = 0);
    void wheelEvent(QWheelEvent *event);
signals:


public slots:
};

#endif // MYGRAPHICSVIEW_H
