#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->LoadFiles->setEnabled(false);
    ui->ScoringButton->setEnabled(false);
    isInitialized=false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_OpenFolder_clicked()
{
    isInitialized=true;

    rootPath=QFileDialog::getExistingDirectory();

    QString rps=rootPath.absolutePath();

    //initializing project structure
    if(!rootPath.exists("SavedCodes")){
        rootPath.mkdir("SavedCodes");
    }
    codefile_folder_dir=QDir(rps+"/SavedCodes");

    if(!rootPath.exists("input_TC")){
        rootPath.mkdir("input_TC");
    }
    input_folder_dir=QDir(rps+"/input_TC");


    if(!rootPath.exists("AnsCodeOrFiles")){
        rootPath.mkdir("AnsCodeOrFiles");
    }
    answer_folder_dir=QDir(rps+"/AnsCodeOrFiles");

    ui->LoadFiles->setEnabled(true);

//    codefile_folder_dir=QFileDialog::getExistingDirectory();
//    ui->inputFolderButton->setEnabled(true);
}

void MainWindow::on_LoadFiles_clicked()
{
    //Load SavedCodesFiles
    QStringList filter;
    filter << "*.c" <<"*.cpp";
    codeFileList.clear();
    ui->InputCodeList->clear();
    codeFileList = codefile_folder_dir.entryInfoList( filter, QDir::Files );
    for(int i=0;i<codeFileList.size();++i){
        QFileInfo fileInfo = codeFileList.at(i);
        ui->InputCodeList->addItem(QString("%1").arg(fileInfo.fileName()));
    }

    //Load input_TC
    filter.clear();
    filter << "*.txt";
    inputFileList.clear();
    ui->InputFileList->clear();
    inputFileList = input_folder_dir.entryInfoList(filter,QDir::Files);
    for(int i=0;i<inputFileList.size();++i){
        QFileInfo fileInfo = inputFileList.at(i);
        ui->InputFileList->addItem(QString("%1").arg(fileInfo.fileName()));
    }

    //Load Answer Files
    if(ui->answerCombo->currentIndex()==0){
        filter.clear();
        filter << "*.cpp" <<"*.c";
    }
    else{
        filter.clear();
        filter << "*.txt";
    }

    outputFileList.clear();
    ui->answerFileList->clear();
    outputFileList = answer_folder_dir.entryInfoList(filter,QDir::Files);
    for(int i=0;i<outputFileList.size();++i){
        QFileInfo fileInfo = outputFileList.at(i);
        ui->answerFileList->addItem(QString("%1").arg(fileInfo.fileName()));
    }

    ui->ScoringButton->setEnabled(true);
}

//void MainWindow::on_inputFolderButton_clicked()
//{

//    ui->answerFolderButton->setEnabled(true);
//}


void MainWindow::on_answerCombo_activated(int index)
{

}

void MainWindow::on_ScoringButton_clicked()
{
    QString program = "g++";
    QStringList command;
    QFileInfo fileinfo = answerCodeString;
    command <<"-std=c++14 "<<answerCodeString<<" "<<"-o "<<QString("%1").arg(fileinfo.fileName().split(".",QString::SkipEmptyParts).at(0));
    ui->answerFileList->addItem(QString("%1").arg(command.at(4)));
    QProcess *myprocess = new QProcess(this);
    myprocess->execute(program,command);

    //int i=myprocess->exitCode();
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}


