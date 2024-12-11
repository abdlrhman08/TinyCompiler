#include "nonterminalnode.h"

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

NonTerminalNode* NonTerminalNode::addSibling(QGraphicsScene* scene)
{
    NonTerminalNode* sibling = new NonTerminalNode(rect().x()+200, rect().y(), 100, 50);
    scene->addItem(sibling);

    scene->addLine(rect().x()+100, rect().y()+25, sibling->rect().x(), sibling->rect().y()+25, QPen(Qt::black)); // Root to Child1
    return sibling;
}

NonTerminalNode *NonTerminalNode::addNonTerminalChild(QGraphicsScene* scene)
{
    NonTerminalNode* newChild = new NonTerminalNode(rect().x(), rect().y()+200, 100, 50);
    scene->addItem(newChild);

    scene->addLine(rect().x()+50, rect().y()+50, newChild->rect().x()+50, newChild->rect().y(), QPen(Qt::black)); // Root to Child1
    return newChild;
}

TerminalNode *NonTerminalNode::addTerminalChild(QGraphicsScene *scene)
{
    TerminalNode* child = new TerminalNode(rect().x() + 20, rect().y() + 100 , 30);
    scene->addItem(child);

    scene->addLine(rect().x()+50, rect().y()+50, child->rect().x()+30, child->rect().y(), QPen(Qt::black));
    return child;
}


