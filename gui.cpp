#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QAction *openTextAction = new QAction("&Open file",this);
    QAction *openImageAction = new QAction("&Open image",this);

    ui->textEdit->addAction(openTextAction);
    ui->label->addAction(openImageAction);

    connect (openTextAction,
             &QAction::triggered,
             this,
             &MainWindow::on_pushButton_clicked);
    connect (openImageAction,
             &QAction::triggered,
             this,
             &MainWindow::on_pushButtonImage_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this,"Select a text file ");
    QFile inputFile(fileName);
    if(inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream textStream(&inputFile);
        QString fileContent=textStream.readAll();
        ui->textEdit->setText(fileContent);
    }

}

void MainWindow::on_pushButtonImage_clicked()
{

    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "C:/Users/Stef/Desktop",
                                                    tr("Images (*.png *.xpm *.jpg)"));
    QPixmap pix(fileName);
        ui->label->setPixmap(pix.scaled(300,300,Qt::KeepAspectRatio));
}
