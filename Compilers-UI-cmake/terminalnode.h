#ifndef TERMINALNODE_H
#define TERMINALNODE_H
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <iostream>

// Custom Terminal Node
class TerminalNode : public QGraphicsEllipseItem {
public:
    TerminalNode(qreal x, qreal y, qreal radius,  QString text);

    TerminalNode* addChild(QGraphicsScene *scene, QString text);

    int lastChildX = 0;

private:
    QGraphicsTextItem* textItem;


};

#endif // TERMINALNODE_H
