#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include "nonterminalnode.h"
#include "terminalnode.h"
#include <iostream>
#include "scanner.h"
#include "parser.h"

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
    test_nodes(scene, true);
    std::cout<<"test"<<std::endl;
    open_compilation("test-tiny.tiny");
    // Token* tokens = tokenize();
    // size_t token_count = get_token_count();
    // Parser parser;
    // parser.setTokensList(tokens, token_count);
    // parser.run();
    // parser.printParseTree(parser.parse_tree, 0);
    // parser.clearTables();


    // Window Settings
    setWindowTitle("Parser Tree Viewer");
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test_nodes(QGraphicsScene* scene, bool test)
{
    if (test){
        // Root Node
        NonTerminalNode* root = new NonTerminalNode(120, 120, 100, 50, "read\n(x)");
        scene->addItem(root);

        auto sibling = root->addSibling(scene, "if");//first sibling
        sibling->addTerminalChild(scene, "op\n(<)");
        sibling->addSibling(scene, "repeat");//second sibling


        auto siblingChild = sibling->addNonTerminalChild(scene, "repeat");
        siblingChild->addSibling(scene, "repeat");

        auto terminalChild = siblingChild->addTerminalChild(scene, "id\n test");

        terminalChild->addChild(scene, "id\n test");
        terminalChild->addChild(scene, "id\n test");
        terminalChild->addChild(scene, "id\n test");
    }
}
