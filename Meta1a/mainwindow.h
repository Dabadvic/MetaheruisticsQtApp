#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QStringList>
#include <vector>
#include <string>

#include "fileloader.h"
#include "greedy.h"
#include "localsearch.h"
#include "tabusearch.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void checkAllOptions();
private slots:
    void on_actionSalir_triggered();

    void on_fileButton_clicked();

    void on_buttonExecute_clicked();

    void on_lineSeed_textChanged(const QString &arg1);

    void on_lineSolutions_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QString fileName_;
    int firstMatrix_; //0 para flujos, 1 para distancias
    bool useGreedy_;
    int seed_;
    int solutionsToCheck_;
    vector< vector<int> > distances_,flow_;
};

#endif // MAINWINDOW_H
