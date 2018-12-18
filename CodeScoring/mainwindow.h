#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QtXlsx>
#include <QProcess>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_OpenFolder_clicked();

    void on_ScoringButton_clicked();

    void on_exitButton_clicked();

    void on_answerCombo_activated(int index);

    void on_LoadFiles_clicked();

private:
    Ui::MainWindow *ui;
    QDir rootPath;
    QDir codefile_folder_dir;
    QDir input_folder_dir;
    QDir answer_folder_dir;
    QDir xlsx_output_folder;
    QDir temp_execute_folder;
    QDir answer_executefile_folder;
    QDir answer_output_folder;
    QDir scored_output_dir;

    QFileInfoList codeFileList;
    QFileInfoList inputFileList;
    QFileInfoList outputFileList;

    QFile answerCodeFile;
    QString answerCodeString;

   // QString compileOption;
    QString saveFileName;

    QProcess* gnu_process;

    QXlsx::Document outexcelfile;

    bool isInitialized;
    double timeLimit;
    int ScoringOps;
};

#endif // MAINWINDOW_H
