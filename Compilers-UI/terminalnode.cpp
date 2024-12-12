#include "terminalnode.h"

TerminalNode::TerminalNode(qreal x, qreal y, qreal radius, QString text)
    : QGraphicsEllipseItem(x, y, radius * 2, radius * 2) {
    std::cout<<"inside constructor"<<std::endl;
    setBrush(QBrush(Qt::white));
    setPen(QPen(Qt::blue));
    std::cout<< "Brush set to:" << brush().color().value()<<std::endl;
    // Add text item as a child
    textItem = new QGraphicsTextItem(text, this);
    QFont font("Arial", 10, QFont::Bold);
    textItem->setFont(font);
    textItem->setDefaultTextColor(Qt::black);
    // Center the text within the ellipse
    qreal textWidth = textItem->boundingRect().width();
    qreal textHeight = textItem->boundingRect().height();
    textItem->setPos(x + (textWidth/2) ,y + (textHeight/2));
}



TerminalNode* TerminalNode::addChild(QGraphicsScene* scene, QString text)
{
    TerminalNode* child = new TerminalNode(lastChildX + rect().x(), rect().y() + 200 , 30, text);
    scene->addItem(child);

    scene->addLine(rect().x()+30, rect().y()+60, child->rect().x()+30, child->rect().y(), QPen(Qt::black)); // Root to Child1

    lastChildX += 70;
    return child;
}
