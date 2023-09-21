#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog> //Библиотека с диалоговыми окнами для работы с файлами
#include <QMessageBox>
#include <QCloseEvent> //Библиотека с функциями обработки сигнала закрытия формы

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Menu_Open_triggered();

    void on_Menu_SaveAs_triggered();

    void closeEvent(QCloseEvent *event); //Будем изменять обработчик события закрытия формы

    void on_textEdit_textChanged();

    void on_Menu_Create_triggered();

    void on_Menu_Exit_triggered();

    void on_Menu_Save_triggered();

private:
    Ui::MainWindow *ui;
    bool save = false;
    QString tmp_name;
    bool change = false;
    bool exit = false;
    QString text_in;
    bool open = false;
    bool sv = true;
    bool open2=false;
    QString GLOBAL_FILE_NEMA;
};
#endif // MAINWINDOW_H
