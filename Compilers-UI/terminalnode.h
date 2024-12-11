#ifndef TERMINALNODE_H
#define TERMINALNODE_H
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QGraphicsScene>

// Custom Terminal Node
class TerminalNode : public QGraphicsEllipseItem {
public:
    TerminalNode(qreal x, qreal y, qreal radius)
        : QGraphicsEllipseItem(x, y, radius * 2, radius * 2) {
        setBrush(QBrush(Qt::green));
    }

    TerminalNode* addChild(QGraphicsScene *scene);
};

#endif // TERMINALNODE_H
