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
    NonTerminalNode(qreal x, qreal y, qreal width, qreal height, QString text);

    void addChild(QGraphicsItem* child);
    NonTerminalNode* addSibling(QGraphicsScene* scence, QString text, int offset_weight);
    NonTerminalNode* addNonTerminalChild(QGraphicsScene* scence, QString text);

    TerminalNode* addTerminalChild(QGraphicsScene* scene, QString text);

    int lastChild = 0;

private:
    QGraphicsTextItem* textItem;
};

#endif // NONTERMINALNODE_H
