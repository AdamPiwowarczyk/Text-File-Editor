#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QStringBuilder>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QDebug>
#include <QTextCodec>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void OpenFile();

    bool openFileToRead();

    void writeToFile(QList<QString>&, QString);

    void reverse(QString&);

    void on_pushButtonStart_clicked();

    void number();

    void switchSides();

    void deleteLast();

    void openFile();

    void deleteFirst();

    void reorder();

    void sort();

    bool isEndLine(QString);

private:
    Ui::MainWindow *ui;
    QFile *file = nullptr;
    QChar separator;
};

#endif // MAINWINDOW_H
