#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTime>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->radioFlow->setChecked(true);
    seed_ = 0;
    firstMatrix_ = 0;
    useGreedy_ = false;
    solutionsToCheck_ = 10000;
    ui->buttonExecute->setDisabled(true);
    ui->lineSolutions->setText("10000");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSalir_triggered()
{
    QCoreApplication::exit();
}

void MainWindow::on_fileButton_clicked()
{
    fileName_ = QFileDialog::getOpenFileName(this, tr("Abrir archivo"), "/Users/David/Desktop/meta/Practicas", tr("Data (*.dat)"));

    QStringList list = fileName_.split("/");

    QString name = list.last();

    FileLoader file;
    file.loadFile(fileName_);
    distances_ = file.getDistancesMatrix();
    flow_ = file.getFlowMatrix();

    this->ui->labelSelectedFile->setText("Archivo elegido: " + name);
    checkAllOptions();
}

void MainWindow::checkAllOptions()
{
    if (!fileName_.isNull())
        if (seed_ > 0)
            this->ui->buttonExecute->setDisabled(false);
}

void MainWindow::on_buttonExecute_clicked()
{
    string seconds;
    QString type;
    QTime time_;
    if (this->ui->radioDistances->isChecked() && firstMatrix_ == 0)
    {
        distances_.swap(flow_);
        firstMatrix_ = 1;
    }
    else if (this->ui->radioFlow->isChecked() && firstMatrix_ == 1)
    {
        flow_.swap(distances_);
        firstMatrix_ = 0;
    }

    if (this->ui->checkBoxGreedy->isChecked())
        useGreedy_ = true;
    else
        useGreedy_ = false;

    greedy greed(distances_,flow_);
    time_.start();
    greed.calculateSolution();
    time_.elapsed();
    seconds = to_string(time_.msec());

    vector<int> solution;
    solution = greed.getSolution();

    QString solution_text;

    LocalSearch localsearch(distances_, flow_, solution, seed_, useGreedy_, solutionsToCheck_);
    //Calculo del coste greedy
    string coste = to_string(localsearch.getCost());
    solution_text += "Coste solucion Greedy: ";
    solution_text += QString::fromStdString(coste);
    solution_text += "\n";
    solution_text += "Tiempo Greedy: ";
    solution_text += QString::fromStdString(seconds);
    solution_text += " mseg.\n";

    time_.start();
    localsearch.search();
    time_.elapsed();
    seconds = to_string(time_.msec());

    //Coste de la nueva solucion
    coste = to_string(localsearch.getCost());
    solution_text += "Coste solucion BL: ";
    solution_text += QString::fromStdString(coste);
    solution_text += "\n";
    solution_text += "Tiempo BL: ";
    solution_text += QString::fromStdString(seconds);
    solution_text += " mseg.\n";

    TabuSearch tabu_search(distances_, flow_, solution, seed_, useGreedy_, solutionsToCheck_);

    time_.start();
    tabu_search.search();
    time_.elapsed();
    seconds = to_string(time_.msec());

    //Coste de la nueva solucion tabu
    coste = to_string(tabu_search.getCost());
    solution_text += "Coste solucion BT: ";
    solution_text += QString::fromStdString(coste);
    solution_text += "\n";
    solution_text += "Tiempo BT: ";
    solution_text += QString::fromStdString(seconds);
    solution_text += " mseg.\n";

    this->ui->laberResults->setText(solution_text);
}

void MainWindow::on_lineSeed_textChanged(const QString &arg1)
{
    seed_ = arg1.toInt();
    checkAllOptions();
}

void MainWindow::on_lineSolutions_textChanged(const QString &arg1)
{
    solutionsToCheck_ = arg1.toInt();
}
