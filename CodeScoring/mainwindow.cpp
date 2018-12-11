#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->ScoringButton->setEnabled(false);
    ui->inputFolderButton->setEnabled(false);
    ui->answerFolderButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_OpenFolder_clicked()
{
    codefile_folder_dir=QFileDialog::getExistingDirectory();
    QStringList filter;
    filter << "*.c" <<"*.cpp";
    codeFileList.clear();
    ui->InputCodeList->clear();
    codeFileList = codefile_folder_dir.entryInfoList( filter, QDir::Files );
    for(int i=0;i<codeFileList.size();++i){
        QFileInfo fileInfo = codeFileList.at(i);
        ui->InputCodeList->addItem(QString("%1").arg(fileInfo.fileName()));
    }
    ui->inputFolderButton->setEnabled(true);
}


void MainWindow::on_inputFolderButton_clicked()
{
    input_folder_dir=QFileDialog::getExistingDirectory();
    QStringList filter;
    filter << "*.txt";
    inputFileList.clear();
    ui->InputFileList->clear();
    inputFileList = input_folder_dir.entryInfoList(filter,QDir::Files);
    for(int i=0;i<inputFileList.size();++i){
        QFileInfo fileInfo = inputFileList.at(i);
        ui->InputFileList->addItem(QString("%1").arg(fileInfo.fileName()));
    }
    ui->answerFolderButton->setEnabled(true);
}

void MainWindow::on_ScoringButton_clicked()
{

}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}

void MainWindow::on_answerFolderButton_clicked()
{
    if(ui->answerCombo->currentIndex()==0){
        answerCodeString=QFileDialog::getOpenFileName();
        QFileInfo fileInfo = answerCodeString;
        ui->answerFileList->clear();
        ui->answerFileList->addItem(QString("%1").arg(fileInfo.fileName()));
    }
    else{
        output_folder_dir = QFileDialog::getExistingDirectory();
        QStringList filter;
        filter << "*.txt";
        outputFileList.clear();
        ui->answerFileList->clear();
        outputFileList = output_folder_dir.entryInfoList(filter,QDir::Files);
        for(int i=0;i<inputFileList.size();++i){
            QFileInfo fileInfo = inputFileList.at(i);
            ui->InputFileList->addItem(QString("%1").arg(fileInfo.fileName()));
        }
    }

    ui->ScoringButton->setEnabled(true);
}

void MainWindow::on_answerCombo_activated(int index)
{

}
