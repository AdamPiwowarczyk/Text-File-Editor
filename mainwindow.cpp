#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Numerator");
    this->setFixedSize(this->size());
    separator = '.';
    ui->comboBox->addItem("Open File");
    ui->comboBox->addItem("Number");
    ui->comboBox->addItem("Sort");
    ui->comboBox->addItem("Reorder");
    ui->comboBox->addItem("Delete First");
    ui->comboBox->addItem("Delete Last");
    ui->comboBox->addItem("Switch");
    ui->lineEditSeparator->setMaxLength(3);
    ui->comboBox->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OpenFile()
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::StandardLocation(QStandardPaths::DesktopLocation));
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), dir, tr("(*.txt)"));
    if(fileName == "")
        return;
    file = new QFile(fileName);
    QString name = "";
    for(int i = fileName.length() - 1; fileName.at(i) != '/'; i--)
        name += fileName.at(i);
    reverse(name);
    ui->labelName->setText(name);
}

void MainWindow::reverse(QString &str)
{
    QString str2("");
    for(int i = str.length() - 1; i >= 0; i--)
        str2.append(str[i]);
    str = str2;
}

bool MainWindow::openFileToRead()
{
    if(file == nullptr)
        OpenFile();
    if(file == nullptr) return false;
    if(!file->open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, "error",file->errorString());
        return false;
    }
    return true;
}

void MainWindow:: writeToFile(QList<QString>& list, QString title)
{
    if (QMessageBox::No == QMessageBox::question(this, tr(title.toStdString().c_str()), tr("Confirm"))) return;
    if(!file->open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, "error",file->errorString());
        return;
    }
    QTextStream out(file);
    for(int i = 0; i < list.length() - 1; i++)
    {
        if(isEndLine(list.at(i)))
        {
            out << '\n';
        }
        else
        {
            QString strPost = (list[i].split(QRegExp("\n|\r\n|\r")).first());
            out << strPost + "\n";
        }
    }
    QString strPost = list[list.length() - 1].split(QRegExp("\n|\r\n|\r")).first();
    QString qwe = strPost;//.toUtf8();
    out << strPost;
    file->close();
}

bool MainWindow::isEndLine(QString str)
{
    if(str == "\n" || str == "\r\n" || str == "\r")
        return true;
    return false;
}

void MainWindow::on_pushButtonStart_clicked()
{
    if(ui->comboBox->currentText() == "Number")
        number();
    if(ui->comboBox->currentText() == "Switch")
        switchSides();
    if(ui->comboBox->currentText() == "Delete Last")
        deleteLast();
    if(ui->comboBox->currentText() == "Open File")
        openFile();
    if(ui->comboBox->currentText() == "Delete First")
        deleteFirst();
    if(ui->comboBox->currentText() == "Reorder")
        reorder();
    if(ui->comboBox->currentText() == "Sort")
        sort();
}

void MainWindow::openFile()
{
    OpenFile();
}

void MainWindow::number()
{
    if(!openFileToRead()) return;
    int i = 1;
    QList<QString> list;
    QTextStream in(file);

    while(!in.atEnd())
    {
        list.append(QString::number(i) + separator + ' ' + in.readLine());
        i++;
    }
    file->close();
    writeToFile(list, "Number");
}

void MainWindow::switchSides()
{
    if(!openFileToRead()) return;
    QList<QString> list;
    QList<QString> listPost;

    while(!file->atEnd())
    {
        list.append(file->readLine());
    }
    QString tempStr("");
    QStringList row;
    QString newRow = "";
    for(auto str : list)
    {
        if(isEndLine(str))
        {
            listPost.append("\n");
            continue;
        }
        QString strPost = str.split(QRegExp("\n|\r\n|\r"),QString::SkipEmptyParts).first();
        row = strPost.split(ui->lineEditSeparator->text());
        for(int i = row.length() - 1; i >= 0; i--)
            newRow.append(row[i] + ui->lineEditSeparator->text());
        newRow.chop(ui->lineEditSeparator->text().length());
        listPost.append(newRow);
        newRow = "";
    }
    file->close();
    writeToFile(listPost, "Switch");

}

void MainWindow::deleteLast()
{
    if(!openFileToRead()) return;
    QList<QString> list;
    QString temp("");
    bool firstWord = false;
    bool space = false;
    bool write = false;
    while(!file->atEnd())
    {
        QString str = file->readLine();
        firstWord = false;
        space = false;
        write = false;
        temp = "";
        for(int i = str.length() - 1; i >= 0; i--)
        {
            if(!str[i].isSpace() && str[i] != '\t' && str[i] != '\n')
                firstWord = true;
            if(firstWord && (str[i].isSpace() || str[i] == '\t'))
                space = true;
            if(space && !str[i].isSpace() && str[i] != '\t')
                write = true;
            if(write)
                temp.append(str[i]);
        }
        reverse(temp);
        list.append(temp);
    }
    for(auto str : list)
        qDebug()<<str;

    file->close();
    writeToFile(list, "Delete Last");
}


void MainWindow::deleteFirst()
{
    if(!openFileToRead()) return;
    QList<QString> list;
    QString strPost = "";
    while(!file->atEnd())
    {
        QString str = file->readLine();
        bool write = false;
        bool firstSpacebar = false;
        bool firstWord = false;
        for(QChar c : str)
        {
            if(!write)
            {
                if(!c.isSpace() && c != '\t')
                    firstWord = true;
                if(firstWord && (c.isSpace() || c == '\t'))
                    firstSpacebar = true;
                if(firstSpacebar && !c.isSpace() & c != '\t')
                    write = true;
            }
            if(write)
                strPost.append(c);
        }
        list.append(strPost);
        qDebug()<<strPost;
        strPost = "";
    }
    file->close();
    writeToFile(list, "Delete First");
}

void MainWindow::reorder()
{
    if(!openFileToRead()) return;
    QList<QString> listPre;
    while(!file->atEnd())
        listPre.append(file->readLine());
    QList<QString> listPost;
    while(listPre.count() > 0)
    {
        int nr = QRandomGenerator::global()->bounded(listPre.count());
        listPost.append(listPre.takeAt(nr));
    }
    file->close();
    writeToFile(listPost, "Reorder");
}

void MainWindow::sort()
{
    if(!openFileToRead()) return;
    QList<QString> list;
    while(!file->atEnd())
        list.append(file->readLine());
    list.sort();
    int prev = 0;
    int next = 0;
    QString temp = "";
    for(int i = 0; i < list.length() - 1; i++)
        for(int j = i + 1; j < list.length(); j++)
            {
                prev = list.at(i).split(separator).at(0).toInt();
                next = list.at(j).split(separator).at(0).toInt();
                if(prev > next)
                {
                    temp = list[i];
                    list[i] = list[j];
                    list[j] = temp;
                }
            }

    file->close();
    writeToFile(list, "Sort");
}
