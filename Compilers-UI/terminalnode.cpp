#include "terminalnode.h"

TerminalNode* TerminalNode::addChild(QGraphicsScene* scene)
{
    TerminalNode* child = new TerminalNode(rect().x(), rect().y() + 60 , 30);
    scene->addItem(child);
    return child;
}

TerminalNode *TerminalNode::addTwoChildren(QGraphicsScene *scene)
{
    return nullptr;
}
