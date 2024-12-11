#ifndef NONTERMINALNODE_H
#define NONTERMINALNODE_H
#include "nonterminalnode.h"
#include "terminalnode.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>


class NonTerminalNode : public QGraphicsRectItem
{
public:
    NonTerminalNode(qreal x, qreal y, qreal width, qreal height)
        : QGraphicsRectItem(x,y,width, height){
        setBrush(QBrush(Qt::blue));
    }

    void addChild(QGraphicsItem* child);
    NonTerminalNode* addSibling(QGraphicsScene* scence);
    NonTerminalNode* addNonTerminalChild(QGraphicsScene* scence);

    TerminalNode* addTerminalChild(QGraphicsScene* scene);
};

#endif // NONTERMINALNODE_H
