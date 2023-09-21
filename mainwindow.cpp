#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Без имении - Блокнот");
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_textEdit_textChanged() //проверка загаловка, добавление звездочки
{
    if(!save and change and !open and !open2) //проверка на пустое окно при не открытом файле
    {
        QString text2 = ui->textEdit->toPlainText().toUtf8(); //берем текст из textEdit
        if(text2==nullptr or text2==NULL or text2.isEmpty())
        {
            tmp_name.remove("*");
            setWindowTitle(tmp_name);
            change = false;
        }
    }
    else
    {
        if(!save and !change)
        {
            tmp_name = windowTitle(); //меняем имя окна

            QString window_name= "*" + tmp_name; //формируем название окна
            setWindowTitle(window_name); //меняем название окна по открытому файлу

            change = true;
        }
    }
}

void MainWindow::on_Menu_Open_triggered() //Меню Открыть файл
{    
    if(change) //есть ли звездочка изменения
    {
        int closing = QMessageBox::question(this, "Выход", "Файл не сохранен. Сохраним его, чтобы начать работать над другим?", \
                                            QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel , QMessageBox::Yes);

        switch (closing)
        { //обработка QMessageBox
        case  QMessageBox::Yes: //ДА
        {
            QString Name;

            if(!change)
            {
                Name = windowTitle(); //строка для имени окна = файла. Она без звезд после изменений файла
            }
            else
            {
                Name = tmp_name; //В случае, когда произошло изменение (со * в названии)
            }

            if (!open and !open2) //Файл не выбран
            {
                save=true;

                Name=QFileDialog::getSaveFileName(this, tr("Сохранить как"), "C:/Users/Костя/Documents", tr("Text files (*.txt)")); //путь для сохранения файла
                GLOBAL_FILE_NEMA = Name;
                QString f_nm =  Name.right(Name.size()-Name.lastIndexOf("/")-1) ; //берем название файла без пути

                if (Name.isEmpty()) //Файл не выбран
                {
                    QMessageBox::information(this, "Ошибка", "Файл не выбран");

                    if(!open2 & !open)
                    {
                        setWindowTitle(f_nm + "*Без имени - Блокнот");
                        change = true;
                        sv = false; //файл не сохранили, обратно
                    }
                    else
                    {
                        setWindowTitle(tmp_name);
                    }
                    change = true;
                    sv = false; //файл не сохранили, обратно
                }
                else
                {

                    //setWindowTitle(f_nm + " - Блокнот"); //меняем название окна
                    QFileInfo f(GLOBAL_FILE_NEMA);
                    QString ext = f.suffix();
                    if (ext == "txt")
                    {
                        setWindowTitle(f_nm + " - Блокнот");

                        QFile file; //класс файлов
                        file.setFileName(Name);


                        file.open(QIODevice::WriteOnly);

                        file.write(ui->textEdit->toPlainText().toUtf8()); //пишем в файл, используя цепочки преобразований

                        file.close(); //Закрываем файл

                        save = false;
                        change =false;
                        open2=true;
                        open = false;
                        sv = true; //файл сохранили, можно идти дальше
                    }
                    else
                    {
                        sv=false;
                        //qApp->exit();
                        QMessageBox::information(this,"Ошибка","ТАК НЕЛЬЗЯ");
                        return;


                    }
                }
            }
            else
            {
                save=true;

                setWindowTitle(Name); //меняем название окна
                Name.chop(10);

                Name = "C:/Users/Костя/Documents" + Name; //создаем путь для сохранения файла
                QFileInfo f(GLOBAL_FILE_NEMA);
                QString ext = f.suffix();
                if (ext == "txt")
                {

                    QFile file; //класс файлов
                    file.setFileName(Name);

                    file.open(QIODevice::WriteOnly);

                    file.write(ui->textEdit->toPlainText().toUtf8()); //пишем в файл, используя цепочки преобразований

                    file.close();
                }//Закрываем файл
                else
                {
                    QMessageBox::information(this,"Ошибка","ТАК НЕЛЬЗЯ");
                    return;
                }
                save = false;
                change = false;
                open2 = true;
                open = false;
                sv = true; //файл сохранили, можно идти дальше
            }
        }
            break;
        case QMessageBox::No: //НЕТ
        {
            save = false;
            change = true;
            sv = true; // файл не сохранили, но можно идти дальше


        }
            break;
        case QMessageBox::Cancel: //ОТМЕНА
        {
            sv=false;

        }
            break;
        }
    }
    if (sv)
    {
        QString FileName; //строка для имени файла
        FileName=QFileDialog::getOpenFileName(this, tr("Открыть файл"), "C:/Users/Костя/Documents", tr("Text files (*.txt)")); // windows style - экранируем слеши
        if (FileName.isEmpty()) //Файл не выбран
        {
            QMessageBox::information(this, "Ошибка", "Файл не выбран");
        }
        else
        {
            save=true;

            QString f_nm =  FileName.right(FileName.size()-FileName.lastIndexOf("/")-1) ; //берем название файла без пути
            QString window_name= f_nm + " - Блокнот"; //формируем название окна
            /////////////


            ui->textEdit->clear(); //очистка поля редактора
            QFileInfo f(FileName);
            QString ext = f.suffix();
            if (ext == "txt")
            {
                setWindowTitle(window_name); //меняем название окна по открытому файлу
                QFile file; //класс файлов
                file.setFileName(FileName);

                file.open(QIODevice::ReadOnly);

                QByteArray ba; //массив для передачи данных

                ba.clear();
                long long int size;
                size = file.size(); //получаем размер файла

                ba = file.read(size);
                ui->textEdit->append(QString(ba)); //помещаем считанные данные в поле редактора через вызов преобразования классом строки

                text_in = ui->textEdit->toPlainText();

                file.close(); //Закрываем файл
            }
            else
            {
                QMessageBox::information(this,"Ошибка","ТАК НЕЛЬЗЯ");
                return;
            }
            save=false;
            change=false;
            open = true;
        }
    }


}
void MainWindow::on_Menu_SaveAs_triggered() //Меню Сохранить как
{
    QString FileName; //строка для имени файла
    FileName=QFileDialog::getSaveFileName(this, tr("Сохранить как"), "C:/Users/Костя/Documents", tr("Text files (*.txt)")); // windows style - экранируем слеши
    QString f_nm =  FileName.right(FileName.size()-FileName.lastIndexOf("/")-1) ; //берем название файла без пути
    if (FileName.isEmpty()) //Файл не выбран
    {
        QMessageBox::information(this, "Ошибка", "Файл не выбран");
    }
    else
    {
        save=true;
        QFileInfo f(FileName);
        QString ext = f.suffix();
        if (ext == "txt")
        {
            setWindowTitle(GLOBAL_FILE_NEMA); //меняем название окна по открытому файлу
            QFile file; //класс файлов
            file.setFileName(FileName);

            file.open(QIODevice::ReadOnly);

            QByteArray ba; //массив для передачи данных

            ba.clear();
            long long int size;
            size = file.size(); //получаем размер файла

            ba = file.read(size);
            ui->textEdit->append(QString(ba)); //помещаем считанные данные в поле редактора через вызов преобразования классом строки

            text_in = ui->textEdit->toPlainText();

            file.close(); //Закрываем файл
        }
        else
        {
            QMessageBox::critical(this,"Ошибка","ВЫ ЧТО ТАК НЕНАДА");
            return;
        }
        f_nm.remove("*"); //убираем звездочку изменения из названия

        setWindowTitle(f_nm + " - Блокнот"); //меняем название окна

        QFile file; //класс файлов
        file.setFileName(FileName);

        file.open(QIODevice::WriteOnly);

        file.write(ui->textEdit->toPlainText().toUtf8()); //пишем в файл, используя цепочки преобразований

        file.close(); //Закрываем файл

        save = false;
        change=false;
        open = true;
    }
}

void MainWindow::closeEvent(QCloseEvent *event) // обработчик события закрытия формы
{
    if(!exit & change)
    {

        int closing = QMessageBox::question(this, "Выход", "Файл не сохранен. Хотите сохранить его?", \
                                            QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel , QMessageBox::Yes);
        switch (closing) {
        case  QMessageBox::Yes: //ДА
        {
            QString Name;

            if(!change)
            {
                Name = windowTitle(); //строка для имени окна = файла. Она без звезд после изменений файла
            }
            else
            {
                Name = tmp_name; //В случае, когда произошло изменение (со * в названии)
            }

            if (!open & !open2) //Файл не выбран
            {

                save=true;

                Name=QFileDialog::getSaveFileName(this, tr("Сохранить как"), "C:/Users/Костя/Documents", tr("Text files (*.txt)")); //путь сохранения файла
                QString f_nm =  Name.right(Name.size()-Name.lastIndexOf("/")-1) ; //берем название файла без пути
                if (Name.isEmpty()) //Файл не выбран
                {
                    QMessageBox::information(this, "Ошибка", "Файл не выбран");
                    event->ignore();
                }
                else
                {
                    setWindowTitle(f_nm); //название окна

                    QFile file; //класс файлов
                    file.setFileName(Name);

                    file.open(QIODevice::WriteOnly);

                    file.write(ui->textEdit->toPlainText().toUtf8()); //пишем в файл, используя цепочки преобразований

                    file.close(); //Закрываем файл
                    save = false;
                    change =false;

                    event->accept(); //одобряем закрытие
                }

            }
            else
            {
                save=true;

                setWindowTitle(Name); //меням название окна
                Name.chop (10); //убираем " - Блокнот"

                Name = "C:/Users/Костя/Documents" + Name;
                QFile file; //класс файлов
                file.setFileName(Name);

                file.open(QIODevice::WriteOnly);

                file.write(ui->textEdit->toPlainText().toUtf8()); //пишем в файл, используя цепочки преобразований

                file.close(); //Закрываем файл

                save = false;
                change = false;
                event->accept();
            }

        }
            break;
        case QMessageBox::No:
        {
            save = false;
            change = true;
            event->accept();

        }
            break;
        case QMessageBox::Cancel:
        {
            event->ignore();
        }
            break;
        }
    }
    else
        event->accept();


}


/*
 1. Доделать остальные пункты меню, как в Блокноте Windows+
 2. Сделать привязку поля текста к размерам окна +
 3. Имя файла в заголовке окна +
 4. Работа с русскими буквами +
 5. Проверка на сохранение изменений (не забываем * у имени файла)+
 6. да нет отмена(не выходим)
 */






void MainWindow::on_Menu_Create_triggered() // Меню создать
{
    if(!change)
    {
        save=true;

        ui->textEdit->clear(); //очистка поля редактора
        setWindowTitle("Без имени - Блокнот"); //заменаназвания окна

        open = false;
        open2 = false;
        sv=true;
        save=false;

    }
    else //файл не сохранен
    {
        int closing2 = QMessageBox::question(this, "Выход", "Файл не сохранен. Хотите сохранить?", \
                                             QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel , QMessageBox::Yes);
        switch (closing2) {
        case QMessageBox::Yes: //ДА
        {
            QString Name;

            if(!change)
            {
                Name = windowTitle(); //строка для имени окна = файла. Она без звезд после изменений файла
            }
            else
            {
                Name = tmp_name; //В случае, когда произошло изменение (со * в названии)
            }

            if (!open & !open2) //Файл не выбран
            {
                save=true;

                Name=QFileDialog::getSaveFileName(this, tr("Сохранить как"), "C:/Users/Костя/Documents", tr("Text files (*.txt)"));
                QString f_nm =  Name.right(Name.size()-Name.lastIndexOf("/")-1) ; //берем название файла без пути

                if (Name.isEmpty()) //Файл не выбран
                {
                    QMessageBox::information(this, "Ошибка", "Файл не выбран");
                    sv=false;

                    break;
                }
                else
                {
                    setWindowTitle(f_nm); //название окна

                    QFile file; //класс файлов
                    file.setFileName(Name);

                    file.open(QIODevice::WriteOnly);

                    file.write(ui->textEdit->toPlainText().toUtf8()); //пишем в файл, используя цепочки преобразований

                    file.close(); //Закрываем файл

                    ui->textEdit->clear(); //очистка поля редактора
                    setWindowTitle("Без имени - Блокнот"); //заменаназвания окна

                    save = false;
                    change =false;
                    open= false;
                    open2 = false;
                    sv = true;
                }

            }
            else
            {
                save=true;

                setWindowTitle(Name);
                Name.chop (10); // убраем из названия " - Блокнот"


                Name = "C:/Users/Костя/Documents" + Name; //создаем путь
                QFile file; //класс файлов
                file.setFileName(Name);

                file.open(QIODevice::WriteOnly);

                file.write(ui->textEdit->toPlainText().toUtf8()); //пишем в файл, используя цепочки преобразований

                file.close(); //Закрываем файл
                ui->textEdit->clear(); //очистка поля редактора
                setWindowTitle("Без имени - Блокнот"); //заменаназвания окна

                save = false;
                change = false;
                sv=true;
                open = false;
                open2 = false;
            }

        }
            break;
        case QMessageBox::No:
        {
            save=true;//не вставляем знак несохраненности

            ui->textEdit->clear(); //очистка поля редактора
            setWindowTitle("Без имени - Блокнот"); //заменаназвания окна

            save=false; //после стирания следим за изменением файла
            change=false;
            open=false;
            open2 = false;
            sv=true;
        }
            break;
        case QMessageBox::Cancel:
            break;
        }


    }
}


void MainWindow::on_Menu_Exit_triggered() //Меню Выйти
{
    if (!change)
    {
        exit=true;
        close();
    }
    else
    {

        int closing = QMessageBox::question(this, "Выход", "Файл не сохранен. Сохраним его?", \
                                            QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel , QMessageBox::Yes);

        switch (closing) {
        case  QMessageBox::Yes:
        {
            QString Name;

            if(!change)
            {
                Name = windowTitle(); //строка для имени окна = файла. Она без звезд после изменений файла
            }
            else
            {
                Name = tmp_name; //В случае, когда произошло изменение (со * в названии)
            }

            if (!open) //Файл не выбран
            {

                save=true;

                Name=QFileDialog::getSaveFileName(this, tr("Сохранить как"), "C:/Users/Костя/Documents", tr("Text files (*.txt)"));
                QString f_nm =  Name.right(Name.size()-Name.lastIndexOf("/")-1) ; //берем название файла без пути
                if (Name.isEmpty()) //Файл не выбран
                {
                    QMessageBox::information(this, "Ошибка", "Файл не выбран");

                }
                else
                {
                    save = true;

                    setWindowTitle(f_nm);

                    QFile file; //класс файлов
                    file.setFileName(Name);

                    file.open(QIODevice::WriteOnly);

                    file.write(ui->textEdit->toPlainText().toUtf8()); //пишем в файл, используя цепочки преобразований

                    file.close(); //Закрываем файл
                    save = false;
                    change =false;

                    exit = true;
                    close();
                }

            }
            else
            {
                save=true;

                Name.chop (10); // убираем " -  Блокнот"

                setWindowTitle(Name);
                Name = "C:/Users/Костя/Documents" + Name;
                QFile file; //класс файлов
                file.setFileName(Name);

                file.open(QIODevice::WriteOnly);

                file.write(ui->textEdit->toPlainText().toUtf8()); //пишем в файл, используя цепочки преобразований

                file.close(); //Закрываем файл

                save = false;
                change = false;

                exit=true;
                close();
            }

        }
            break;
        case QMessageBox::No:
        {
            save = false;
            change = true;

            exit=true;
            close();

        }
            break;
        case QMessageBox::Cancel:
        {
        }
            break;
        }
    }

}


void MainWindow::on_Menu_Save_triggered() //Меню Сохранить
{
    QString Name;

    if(!change)
    {
        Name = windowTitle(); //строка для имени окна = файла. Она без звезд после изменений файла
    }
    else
    {
        Name = tmp_name; //В случае, когда произошло изменение (со * в названии)
    }

    if (!open & !open2) //Файл не выбран
    {


        Name=QFileDialog::getSaveFileName(this, tr("Сохранить как"), "C:/Users/Костя/Documents", tr("Text files (*.txt)"));
        QString f_nm =  Name.right(Name.size()-Name.lastIndexOf("/")-1) ; //берем название файла без пути

        if (Name.isEmpty()) //Файл не выбран
        {
            QMessageBox::information(this, "Ошибка", "Файл не выбран");

        }
        else
        {
            save = true;

            f_nm.remove("*"); //убираем звездочку сохранения

            setWindowTitle(f_nm + " - Блокнот");
            QFile file; //класс файлов
            file.setFileName(Name);

            file.open(QIODevice::WriteOnly);

            file.write(ui->textEdit->toPlainText().toUtf8()); //пишем в файл, используя цепочки преобразований

            file.close(); //Закрываем файл
            save = false;
            change =false;
            open= true;
        }

    }
    else
    {
        save=true;
        setWindowTitle(Name);
        Name.chop (10); //убираем " - Блокнот"

        Name = "C:/Users/Костя/Documents" + Name;
        QFile file; //класс файлов
        file.setFileName(Name);

        file.open(QIODevice::WriteOnly);

        file.write(ui->textEdit->toPlainText().toUtf8()); //пишем в файл, используя цепочки преобразований

        file.close(); //Закрываем файл

        save = false;
        change = false;
        open = true;

    }
}

