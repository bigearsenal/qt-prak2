#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"
#include "qmessagebox.h"
#include "qtextstream.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->action_save,SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->action_open,SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->action_exit,SIGNAL(triggered()), this, SLOT(exit()));

    // Добавить горячие клавиши
    ui->action_save->setShortcut(QKeySequence("Ctrl+S"));
    ui->action_exit->setShortcut(QKeySequence("Ctrl+Q"));
}

void MainWindow::save() {
    // Сохранить текст к одному переменному
    QString text = ui->textEdit->toPlainText();

    // Не сохранить в файл если текст пустой
    if (text.isEmpty()) return;

    // Открыть диалог для сохранения в файл
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Сохранить тесктовый файл"), "",
            tr("Тесктовый файл (*.txt)"));

    // Проверить имя файла и сохранить
    if (fileName.isEmpty()) return;
    else {
        QFile file(fileName);
        if(!file.open(QFile::WriteOnly | QFile::Text)){
            printf("Не мог бы открыть файл");
            return;
        }
        QDataStream out(&file);
        out << text;
        file.close();
    }
}

void MainWindow::open() {
    // Открыть диалог для откытия файла
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть тесктовый файл"), tr("Тесктовый файл (*.txt)"));

    // Проверить имя файла и сохнанить
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) return;

    QTextStream in(&file);

    QString text = "";

    printf("%d", in.atEnd());
    while(!in.atEnd()) {
        printf("not end");
        QString line = in.readLine();
        text.append(line);
        text.append("\n");
        printf("%s", line.toUtf8().constData());
    }

    ui->textEdit->setText(text);

    file.close();
}

void MainWindow::exit() {
    // Сохранить текст к одному переменному
    QString text = ui->textEdit->toPlainText();

    // Не сохранить в файл если текст пустой
    if (text.isEmpty()) {
        this->close();
        return;
    }

    QMessageBox msgBox;
    msgBox.setText("Документ изменен");
    msgBox.setInformativeText("Хотите сохранить?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::Save:
          this->save();
          break;
      default:
          // should never be reached
          break;
    }
    this->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
