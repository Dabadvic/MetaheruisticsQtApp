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
    seed_ = 123456;
    ui->lineSeed->setText("123456");
    evaluations_ = 20000;
    ui->lineEvaluations->setText("20000");
    population_ = 50;
    ui->linePopulation->setText("50");
    firstMatrix_ = 0;
    ui->buttonExecute->setDisabled(true);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
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

    greedy greed(distances_,flow_);
    time_.start();
    greed.calculateSolution();
//    time_.elapsed();
//    seconds = to_string(time_.msec());
    seconds = to_string(time_.elapsed());

    vector<int> solution;
    solution = greed.getSolution();

    QString solution_text;

    LocalSearch localsearch(distances_, flow_, solution, seed_);
    //Calculo del coste greedy
    string coste = to_string(localsearch.getCost());
    solution_text += "Coste solucion Greedy: ";
    solution_text += QString::fromStdString(coste);
    solution_text += " - ";
    solution_text += "Tiempo: ";
    solution_text += QString::fromStdString(seconds);
    solution_text += " mseg.\n";

    this->ui->progressBar->setValue(15);

    time_.start();
    localsearch.search();
//    time_.elapsed();
//    seconds = to_string(time_.msec());
    seconds = to_string(time_.elapsed());

    //Coste de la nueva solucion
    coste = to_string(localsearch.getCost());
    solution_text += "Coste solucion BL: ";
    solution_text += QString::fromStdString(coste);
    solution_text += " - ";
    solution_text += "Tiempo: ";
    solution_text += QString::fromStdString(seconds);
    solution_text += " mseg.\n";

    this->ui->progressBar->setValue(30);

    Agg agg(distances_, flow_, 0, seed_, evaluations_, population_);

    time_.start();
    agg.search();
    seconds = to_string(time_.elapsed());

    coste = to_string(agg.getCost());
    solution_text += "Coste solucion AGG-Posicion: ";
    solution_text += QString::fromStdString(coste);
    solution_text += " - ";
    solution_text += "Tiempo: ";
    solution_text += QString::fromStdString(seconds);
    solution_text += " mseg.\n";

    this->ui->progressBar->setValue(60);

    Agg agg_ox(distances_, flow_, 1, seed_, evaluations_, population_);

    time_.start();
    agg_ox.search();
    seconds = to_string(time_.elapsed());

    coste = to_string(agg_ox.getCost());
    solution_text += "Coste solucion AGG-OX: ";
    solution_text += QString::fromStdString(coste);
    solution_text += " - ";
    solution_text += "Tiempo: ";
    solution_text += QString::fromStdString(seconds);
    solution_text += " mseg.\n";


    Age age(distances_, flow_, 0, seed_, evaluations_, population_);

    time_.start();
    age.search();
    seconds = to_string(time_.elapsed());

    coste = to_string(age.getCost());
    solution_text += "Coste solucion AGE-Posicion: ";
    solution_text += QString::fromStdString(coste);
    solution_text += " - ";
    solution_text += "Tiempo: ";
    solution_text += QString::fromStdString(seconds);
    solution_text += " mseg.\n";

    Age age_ox(distances_, flow_, 1, seed_, evaluations_, population_);

    time_.start();
    age_ox.search();
    seconds = to_string(time_.elapsed());

    coste = to_string(age_ox.getCost());
    solution_text += "Coste solucion AGE-OX: ";
    solution_text += QString::fromStdString(coste);
    solution_text += " - ";
    solution_text += "Tiempo: ";
    solution_text += QString::fromStdString(seconds);
    solution_text += " mseg.\n";

    this->ui->progressBar->setValue(100);

    this->ui->laberResults->setText(solution_text);
}

void MainWindow::on_lineSeed_textChanged(const QString &arg1)
{
    seed_ = arg1.toInt();
    checkAllOptions();
}

void MainWindow::on_lineEvaluations_textChanged(const QString &arg1)
{
    evaluations_ = arg1.toInt();
    checkAllOptions();
}

void MainWindow::on_linePopulation_textChanged(const QString &arg1)
{
    population_ = arg1.toInt();
    checkAllOptions();
}
