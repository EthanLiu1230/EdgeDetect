#ifndef MYPIXITEM_H
#define MYPIXITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QImage>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QPainterPath>

class MyImgItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
private:
    QImage qimage;

public:
    MyImgItem(QImage &img);
    MyImgItem();
    void addImg(QImage &img);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


};

#endif // MYPIXITEM_H
