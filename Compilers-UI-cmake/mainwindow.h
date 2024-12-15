#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "parser.h"
#include "nonterminalnode.h"
#include "terminalnode.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void test_nodes(QGraphicsScene *scene, bool test=true);

    void traverse_parse_tree(Node *node, QGraphicsScene* scene, NonTerminalNode *nt, TerminalNode *t);

public slots:
    void on_actioncompile_new_file_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
