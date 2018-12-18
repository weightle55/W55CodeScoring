#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), gnu_process(nullptr),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->LoadFiles->setEnabled(false);

    ui->progressBar->setValue(0);
    isInitialized=false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_OpenFolder_clicked()
{
    rootPath=QFileDialog::getExistingDirectory();

    QDir::setCurrent(rootPath.absolutePath());

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

    if(!answer_folder_dir.exists("Answerout")){
        answer_folder_dir.mkdir("Answerout");
    }
    answer_executefile_folder=QDir(answer_folder_dir.absolutePath()+"/Answerout");

    if(!answer_executefile_folder.exists("output")){
        answer_executefile_folder.mkdir("output");
    }
    answer_output_folder=QDir(answer_executefile_folder.absolutePath()+"/output");

    if(!rootPath.exists("xlsx_output")){
        rootPath.mkdir("xlsx_output");
    }
    xlsx_output_folder=QDir(rps+"/xlsx_output");

    if(!codefile_folder_dir.exists("execute_files")){
        codefile_folder_dir.mkdir("execute_files");
    }
    temp_execute_folder=QDir(codefile_folder_dir.absolutePath()+"/execute_files");

    if(!temp_execute_folder.exists("output")){
        temp_execute_folder.mkdir("output");
    }
    scored_output_dir=QDir(temp_execute_folder.absolutePath()+"/output");

    ui->LoadFiles->setEnabled(true);

//    codefile_folder_dir=QFileDialog::getExistingDirectory();
//    ui->inputFolderButton->setEnabled(true);
}

void MainWindow::on_LoadFiles_clicked()
{
    isInitialized=true;

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
        outputFileList.clear();
        ui->answerFileList->clear();
        outputFileList = answer_folder_dir.entryInfoList(filter,QDir::Files);
        for(int i=0;i<outputFileList.size();++i){
            QFileInfo fileInfo = outputFileList.at(i);
            ui->answerFileList->addItem(QString("%1").arg(fileInfo.fileName()));
        }
    }
    else{
        filter.clear();
        filter << "*.txt";
        outputFileList.clear();
        ui->answerFileList->clear();
        outputFileList = answer_output_folder.entryInfoList(filter,QDir::Files);
        for(int i=0;i<outputFileList.size();++i){
            QFileInfo fileInfo = outputFileList.at(i);
            ui->answerFileList->addItem(QString("%1").arg(fileInfo.fileName()));
        }
    }
}


void MainWindow::on_answerCombo_activated(int index)
{

}

void MainWindow::on_ScoringButton_clicked()
{
    //check init
    if(!isInitialized){
        QString ini="Open Folder and Load it first";
        QMessageBox::warning(0,"No Initializing Error",ini);
        return;
    }

    ui->ScoringButton->setEnabled(false);

    QString cellfile=xlsx_output_folder.absolutePath()+"/"+"result.xlsx";
    outexcelfile.saveAs(cellfile);

    QProcessEnvironment env= QProcessEnvironment::systemEnvironment();

    int answerCompileOption = ui ->answerEnvironment->currentIndex();
    QString program;
    QString stdoption;
    QString outFileName;
    QStringList command;


    /**
     * Compiling Answer code file and making output txt file
     */
    if(ui->answerCombo->currentIndex()==0){
        command.clear();
        outFileName=answer_executefile_folder.absolutePath()+"/"+outputFileList.at(0).baseName();

        switch(answerCompileOption){
        case 0: program="gcc"; command << outputFileList.at(0).filePath()<<"-o"<<outFileName; break;
        case 1: program="gcc"; stdoption="-std=c99"; command << stdoption << outputFileList.at(0).filePath()<<"-o"<<outFileName; break;
        case 2: program="g++"; command << outputFileList.at(0).filePath()<<"-o"<<outFileName; break;
        case 3: program="g++"; stdoption="-std=c++11"; command << stdoption << outputFileList.at(0).filePath()<<"-o"<<outFileName; break;
        case 4: program="g++"; stdoption="-std=c++14"; command << stdoption << outputFileList.at(0).filePath()<<"-o"<<outFileName; break;
        }

        if(!gnu_process){
            gnu_process=new QProcess(this);
        }

        gnu_process->setProcessEnvironment(env);
        gnu_process->setProcessChannelMode(QProcess::MergedChannels);
        gnu_process->start(program,command);
        ui->ProcessingMessage->setText("Compiling Answer Code...");
        gnu_process->waitForFinished();
        ui->ProcessingMessage->setText("Compiling Answer Code Compeleted");
        gnu_process->terminate();

        program="./"+outFileName;
        for(int i=0;i<inputFileList.size();i++){
            ui->ProcessingMessage->setText("Processing "+inputFileList.at(i).baseName());

            command.clear();
            QString savedfileName=inputFileList.at(i).baseName()+"out"+QString::number(i) +".txt";

            gnu_process->setProcessChannelMode(QProcess::SeparateChannels);
            gnu_process->setStandardInputFile(inputFileList.at(i).filePath());
            gnu_process->setStandardOutputFile(answer_output_folder.absolutePath()+"/"+savedfileName);
            gnu_process->start(program);
            gnu_process->waitForFinished();
            gnu_process->terminate();
        }
        ui->ProcessingMessage->setText("Complete to make Answer Files");

        ui->progressBar->setValue(10);

        QStringList filter;
        filter.clear();
        filter << "*.txt";
        outputFileList.clear();
        ui->answerFileList->clear();
        outputFileList = answer_output_folder.entryInfoList(filter,QDir::Files);

    }
    //end of compiling Answer code and making output txt file


    /**
      * Code File compiling
      * */
    if(codeFileList.empty()){
        QMessageBox::warning(0,"No Initializing Error","Code Folder is empty");
        ui->progressBar->setValue(0);
        return;
    }

    if(!gnu_process){
        gnu_process=new QProcess(this);
    }
    gnu_process->setProcessEnvironment(env);
    gnu_process->setProcessChannelMode(QProcess::MergedChannels);

    int ccompileoption=ui->cEnvCombo->currentIndex();
    int cppcompileoption=ui->cppEnvCombo->currentIndex();
    QString timel=ui->timeLimitInputBox->text();
    timeLimit=timel.toDouble();

    int timeout = timeLimit*1000;

    QString cstdoption="-std=c99";
    QString cppstdoption="-std=c++11";


    switch(cppcompileoption){
    case 0: break;
    case 1: cppstdoption="-std=c++11"; break;
    case 2: cppstdoption="-std=c++14"; break;
    }

    outexcelfile.write(1,1,"ID");
    outexcelfile.saveAs(cellfile);
    for(int i=0;i<inputFileList.size();i++){
        outexcelfile.write(1,i+2,"TC_"+QString::number(i));
        outexcelfile.saveAs(cellfile);
    }

    for(int i=0;i<codeFileList.size();i++){
        QString extension=codeFileList.at(i).completeSuffix();
        QString filename=temp_execute_folder.absolutePath()+"/"+codeFileList.at(i).baseName();
        QString thisfile=codeFileList.at(i).filePath();

        outexcelfile.write(i+2,1,codeFileList.at(i).baseName());

        command.clear();
        if(extension=="c"){
            program="gcc";
            if(ccompileoption==0){
                command << thisfile <<"-o"<<filename;
            }else{
                command << cstdoption << thisfile << "-o" << filename;
            }
        }
        else if(extension=="cpp"){
            program="g++";
            if(cppcompileoption==0){
                command << thisfile << "-o" <<filename;
            }else{
                command << cppstdoption << thisfile << "-o" <<filename;
            }
        }

        gnu_process->start(program,command);
        gnu_process->waitForFinished();
        int comerr=gnu_process->exitCode();
        if(!comerr){
            outexcelfile.write(i+2,2,"Compile Error");
            outexcelfile.saveAs(cellfile);
        }

        program="./"+filename;
        gnu_process->setProcessChannelMode(QProcess::SeparateChannels);
        for(int j=0;j<inputFileList.size();j++){
            QString savedtxt=codeFileList.at(i).baseName()+"out"+QString::number(j)+".txt";
            gnu_process->setStandardInputFile(inputFileList.at(j).filePath());
            gnu_process->setStandardOutputFile(scored_output_dir.absolutePath()+"/"+savedtxt);
            gnu_process->start(program);
            bool isTimeout = gnu_process->waitForFinished(timeout);
            int excode= gnu_process->exitCode();
            gnu_process->terminate();
            if(excode==1){
                outexcelfile.write(i+2,j+2,"Compile error");
                outexcelfile.saveAs(cellfile);
                continue;
            }
            if(isTimeout==true){
                outexcelfile.write(i+2,j+2,"Time out");
                outexcelfile.saveAs(cellfile);
                continue;
            }

            QFile answer(outputFileList.at(j).filePath());
            QFile scored(scored_output_dir.absolutePath()+"/"+savedtxt);

            answer.open(QIODevice::ReadOnly,QIODevice::Text);
            scored.open(QIODevice::ReadOnly,QIODevice::Text);

            QTextStream answerStream(&answer);
            QTextStream scoredStream(&scored);

            QStringList alist;
            QStringList slist;

            while(!answerStream.atEnd()){
                alist += answerStream.readLine().split("\\s+");
            }
            while(!scoredStream.atEnd()){
                slist += scoredStream.readLine().split("\\s+");
            }

            if(alist.size()<slist.size()){
                outexcelfile.write(i+2,j+2,"too many output");
            }
            else if(slist.size()<alist.size()){
                outexcelfile.write(i+2,j+2,"wrong");
            }
            else{
                bool res=true;
                for(int k =0;k<alist.size();k++){
                    if(alist.at(k)!=slist.at(k)){ res=false; break;}
                }
                if(res){
                    outexcelfile.write(i+2,j+2,"right");
                }else{
                    outexcelfile.write(i+2,j+2,"wrong");
                }
            }
            outexcelfile.saveAs(cellfile);
        }

    }

    outexcelfile.saveAs(cellfile);

    ui->ScoringButton->setEnabled(true);
    ui->progressBar->setValue(100);
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}


