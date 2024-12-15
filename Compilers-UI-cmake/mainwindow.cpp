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
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // //connect compile new file action
    // connect(ui->actioncompile_new_file, &QAction::triggered, this, &MainWindow::on_actioncompile_new_file_triggered);

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
    //if (open_compilation("test-tiny.tiny") != 0) 
    //{
    //    print_error();
    //}
    //
    //Token* tokens = tokenize();
    //size_t token_count = get_token_count();
    //for (size_t i = 0; i < token_count; i++)
    //{
    //    print_token(&tokens[i]);
    //}
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

void MainWindow::on_actioncompile_new_file_triggered(){
    // Open a file dialog with a filter for .tiny files
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Open Tiny File",          // Dialog title
        "",                        // Default directory
        "Tiny Files (*.tiny)"      // File filter
    );

    // Check if a file was selected
    if (!filePath.isEmpty()) {
        if (open_compilation(filePath.toStdString().c_str()) != 0) 
        {
            print_error();
        }
        
        Token* tokens = tokenize();
        size_t token_count = get_token_count();
        for (size_t i = 0; i < token_count; i++)
        {
            print_token(&tokens[i]);
        }
        //Parser parser;
        //parser.setTokensList(tokens, token_count);
        //parser.run();
        //parser.printParseTree(parser.parse_tree, 0);

        //parser.clearTables();
        std::cout << "Selected file: " << filePath.toStdString() << std::endl;
    } else {
        std::cout<< "No file selected.";
    }
}
