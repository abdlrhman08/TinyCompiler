#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include "nonterminalnode.h"
#include "terminalnode.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create a graphics scene and view
    QGraphicsScene* scene = new QGraphicsScene(this);
    QGraphicsView* view = new QGraphicsView(scene, this);

    // Configure the view
    view->setSceneRect(0, 0, 8000, 6000);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setRenderHint(QPainter::Antialiasing);

    // Add to the main window
    setCentralWidget(view);

    // Add some graphics items to the scene
    // Root Node
    NonTerminalNode* root = new NonTerminalNode(120, 120, 100, 50);
    scene->addItem(root);

    auto sibling = root->addSibling(scene);//first sibling
    sibling->addSibling(scene);//second sibling


    auto siblingChild = sibling->addNonTerminalChild(scene);
    siblingChild->addSibling(scene);

    // auto sibling = new NonTerminalNode(500, 50, 100, 50);
    // scene->addItem(sibling);

    // // Connections (Lines)
    // scene->addLine(400, 100, 375, 135, QPen(Qt::black)); // Root to Child1
    // scene->addLine(400, 100, 415, 135, QPen(Qt::black)); // Root to Child2
    // scene->addLine(450, 75, 500, 75, QPen(Qt::black));   // Root to Sibling

    // Window Settings
    setWindowTitle("Parser Tree Viewer");
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}
