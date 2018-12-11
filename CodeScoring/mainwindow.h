#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QtXlsx>

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

    void on_inputFolderButton_clicked();

    void on_exitButton_clicked();

    void on_answerCombo_activated(int index);

    void on_answerFolderButton_clicked();

private:
    Ui::MainWindow *ui;
    QDir codefile_folder_dir;
    QDir input_folder_dir;
    QDir output_folder_dir;

    QFileInfoList codeFileList;
    QFileInfoList inputFileList;
    QFileInfoList outputFileList;

    QFile answerCodeFile;
    QString answerCodeString;
    double timeLimit;
};

#endif // MAINWINDOW_H
