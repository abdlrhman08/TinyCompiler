#include "nonterminalnode.h"
#include <iostream>

NonTerminalNode::NonTerminalNode(qreal x, qreal y, qreal width, qreal height, QString text)
    : QGraphicsRectItem(x,y,width, height){
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setBrush(QBrush(Qt::white));
    setPen(QPen(Qt::blue));
    // Add text item as a child
    textItem = new QGraphicsTextItem(text, this);
    QFont font("Arial", 10, QFont::Bold);
    textItem->setFont(font);
    textItem->setDefaultTextColor(Qt::black);
    // Center the text within the ellipse
    qreal textWidth = textItem->boundingRect().width();
    qreal textHeight = textItem->boundingRect().height();
    textItem->setPos(x + ((width-textWidth)/2) ,y + (textHeight/4));
}

// a child will have coordinates relative to the parent
void NonTerminalNode::addChild(QGraphicsItem *child)
{
    child->setParentItem(this);

    // // Calculate relative position for the child
    // qreal childX = rect().width() / 2 - child->boundingRect().width() / 2;
    // qreal childY = rect().height() + 20; // Place below parent with some padding
    // child->setPos(childX, childY);

    // children_.append(child); // Store child if needed for other purposes
}

NonTerminalNode* NonTerminalNode::addSibling(QGraphicsScene* scene, QString text, int offset_weight)
{
    NonTerminalNode* sibling = new NonTerminalNode(rect().x()+400+ (offset_weight*100), rect().y(), 100, 50, text);
    scene->addItem(sibling);

    scene->addLine(rect().x()+100, rect().y()+25, sibling->rect().x(), sibling->rect().y()+25, QPen(Qt::black)); // Root to Child1
    return sibling;
}

NonTerminalNode *NonTerminalNode::addNonTerminalChild(QGraphicsScene* scene, QString text)
{
    std::cout<<std::to_string(rect().x())<<std::endl<<std::to_string(lastChild + rect().x())<<std::endl;
    NonTerminalNode* newChild = new NonTerminalNode(lastChild + rect().x(), rect().y()+200, 100, 50, text);
    scene->addItem(newChild);

    scene->addLine(rect().x()+50, rect().y()+50, newChild->rect().x()+50, newChild->rect().y(), QPen(Qt::black)); // Root to Child1
    lastChild+=400;
    return newChild;
}

TerminalNode *NonTerminalNode::addTerminalChild(QGraphicsScene *scene, QString text)
{
    TerminalNode* child = new TerminalNode(lastChild + rect().x() + 20, rect().y() + 200 , 30, text);
    scene->addItem(child);

    scene->addLine(rect().x()+50, rect().y()+50, child->rect().x()+30, child->rect().y(), QPen(Qt::black));
    lastChild +=400;

    return child;
}
