#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include <iostream>
#include "scanner.h"
#include <QFileDialog>
#include <QString>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // //connect compile new file action
    // connect(ui->actioncompile_new_file, &QAction::triggered, this, &MainWindow::on_actioncompile_new_file_triggered);

    // Create a graphics scene and view
    scene = new QGraphicsScene(this);
    QGraphicsView* view = new QGraphicsView(scene, this);

    // Configure the view
    view->setSceneRect(0, 0, 8000, 6000);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setRenderHint(QPainter::Antialiasing);

    // Add to the main window
    setCentralWidget(view);

    // Add some graphics items to the scene
    test_nodes(scene, false);
    std::cout<<"test"<<std::endl;

    // Window Settings
    setWindowTitle("Parser Tree Viewer");
    showMaximized();
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

        auto sibling = root->addSibling(scene, "if", 1);//first sibling
        sibling->addTerminalChild(scene, "op\n(<)");
        sibling->addSibling(scene, "repeat", 3);//second sibling


        auto siblingChild = sibling->addNonTerminalChild(scene, "repeat");
        siblingChild->addSibling(scene, "repeat", 4);

        auto terminalChild = siblingChild->addTerminalChild(scene, "id\n test");

        terminalChild->addChild(scene, "id\n test");
        terminalChild->addChild(scene, "id\n test");
        terminalChild->addChild(scene, "id\n test");
    }
}

void MainWindow::traverse_parse_tree(Node* node, QGraphicsScene *scene, NonTerminalNode *nt, TerminalNode *t, int counter)
{
    //add children
    if (node->shape == "rectangle"){
        for (Node* child : node->children){
            // if (child==nullptr)
            //     continue;
            if (child->shape=="rectangle"){//non-terminal
                NonTerminalNode* childNode = nt->addNonTerminalChild(scene, QString::fromStdString(child->token_value));
                traverse_parse_tree(child, scene, childNode, nullptr, counter++);
            } else {//terminal
                TerminalNode* childNode = nt->addTerminalChild(scene, QString::fromStdString(child->token_value));
                traverse_parse_tree(child, scene, nullptr, childNode, counter++);
            }
        }
    }else {
        for (Node* child: node->children){
            if(t == nullptr)
                continue;
            TerminalNode *childNode = t->addChild(scene, QString::fromStdString(child->token_value));
            traverse_parse_tree(child, scene, nullptr, childNode, counter++);
        }
    }

    //add sibling if any
    if (node->sibling != nullptr){
        if (node->sibling->token_value == "SYNTAX_ERROR"){
            QMessageBox::warning(nullptr, "Warning", "Syntax error");
            parsingError=true;
        }

        NonTerminalNode* sibling = nt->addSibling(scene, QString::fromStdString(node->sibling->token_value), counter);
        traverse_parse_tree(node->sibling, scene, sibling, nullptr, counter++);
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
        //parser.clearTables();
        std::cout << "Selected file: " << filePath.toStdString() << std::endl;
        if (open_compilation(filePath.toStdString().c_str()) != 0)
        {
            //get_error_string()
            print_error();
        }

        Token* tokens = tokenize();
        size_t token_count = get_token_count();
        for (size_t i = 0; i < token_count; i++)
        {
            print_token(&tokens[i]);
        }
        Parser parser;
        parser.setTokensList(tokens, token_count);
        parser.run();
        parser.printParseTree(parser.parse_tree, 0);
        NonTerminalNode* root = new NonTerminalNode(120, 120, 100, 50, QString::fromStdString(parser.parse_tree->token_value));
        scene->addItem(root);
        traverse_parse_tree(parser.parse_tree, scene, root, nullptr, 1);
        if(parsingError){
            scene->clear();
            parsingError=false;
        }
        parser.clearTables();
    } else {
        std::cout<< "No file selected.";
    }
}
