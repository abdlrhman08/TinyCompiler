#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

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

public slots:
    void on_actioncompile_new_file_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
