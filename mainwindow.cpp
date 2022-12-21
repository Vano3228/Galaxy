#include "mainwindow.h"
#include "math.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QTime>
#include <star.h>
#include <fstream>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    ui->lineEdit_num->setText(QString::number(300));                      //Для ввода количества звезд
    ui->lineEdit_DistCon->setText(QString::number(1e10));                 //Для ввода радиуса слияния
    ui->lineEdit_sysR->setText(QString::number(1e12));                    //Для ввода радиуса Галактики
    connect(ui->pushButtonStart,  SIGNAL(clicked()), this, SLOT(buttonText()));
    connect(ui->pushButton, SIGNAL (released()), this, SLOT (buttonOut()));
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
    connect(ui->create_Galaxy, SIGNAL (clicked()), this, SLOT (buttonCreate()));                // Подключение кнопки создания Галактики
    connect(ui->Create_Galaxy_File, SIGNAL (clicked()), this, SLOT (buttonCreatefromFile()));   // Подключение кнопки создания ГалактикиbuttonCreatefromFile()
    timer->start(1);
}
MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::buttonText(){
    if(ui->pushButtonStart->text()==textB[0]){
        ui->pushButtonStart->setText(textB[1]);
        connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
        pause = true;
    }else{
        ui->pushButtonStart->setText(textB[0]);
        disconnect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
        pause = false;
    }
    return;
}

const int topX0 = 100, topY0 = 100, h = 800, length = 800;
galaxy *galactika = new galaxy();

void MainWindow::buttonCreate(){

        numStars = ui->lineEdit_num->text().toInt();
        ui->lineEdit->setText(QString::number(numStars));
        distConnect =ui->lineEdit_DistCon->text().toDouble();           //Присваиваем новые параметры для Вселенной
        systemRadius = ui->lineEdit_sysR->text().toDouble();
        isGalaxyCreated = true;                                         //Меняем флаг
        delete galactika;                                               //Удаляем содержимое объекта
        star::starCounter = 0;                                          //Обнуление счетчика
        galactika = new galaxy();                                       //Создаем новый объект с новыми параметрами

}

void MainWindow::buttonOut(){                               //Кнопка для вывода инфы в файл
    ofstream out("C:\\Users\\Vano\\Documents\\fileout.txt");
    out << galactika;
    out.close();
    return;
}

void MainWindow::buttonCreatefromFile(){                            //Галактика из файла
    ifstream in("C:\\Users\\Vano\\Documents\\filein.txt");
    in >> numStars >> distConnect >> systemRadius;
    in.close();
    ui->lineEdit->setText(QString::number(numStars));
    isGalaxyCreated = true;                                         //Меняем флаг
    delete galactika;                                               //Удаляем содержимое объекта
    galactika = new galaxy();                                       //Создаем новый объект с новыми параметрами
    return;
}

void MainWindow::paintEvent(QPaintEvent *e) {
  if(pause){                                                 //Проверка на паузу
      Q_UNUSED(e);
      if (!isGalaxyCreated) return;                          // Флагирование на проверку создания Галактики
      QPainter painter(this);
      QPen pen(Qt::black, 1, Qt::SolidLine);
      painter.setPen(pen);

      QBrush brush;
      brush.setStyle(Qt::SolidPattern);

      double coefX = length / 2 / 1e12; // system radius
      int centerX = length / 2;
      for(int i = 0; i < galactika->num; ++i){
          if(galactika->stars[i]){
              brush.setColor(galactika->stars[i]->col);
              if(!i) brush.setColor(Qt::yellow);
              painter.setBrush(brush);
              for(int k = 0; k < 2; ++k){
                        painter.drawEllipse(galactika->stars[i]->x[0] * coefX + centerX + topX0,
                                            galactika->stars[i]->x[1] * coefX + centerX + topY0,
                                            6 + 4 * !i, 6 + 4 * !i);
              }
          }
      }

      galactika->move();

      ui->lineEdit->setText(QString::number(star::starCounter));
      ui->lineEdit_2->setText(QString::number(galactika->stars[0]->m));
      ui->lineEdit_3->setText(QString::number(galactika->stars[0]->x[0]));


      ui->lineEdit_4->setText(QTime::currentTime().toString());                     //Текущее время



    }
  }

