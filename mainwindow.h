#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool ellipse = false;
    const QString textB[2] = {"Start", "Stop"};
    QTimer *timer = new QTimer(this);

private:
    bool isGalaxyCreated = false;
    bool pause = true;
    Ui::MainWindow *ui;

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void buttonText();
    void buttonOut();
    void buttonCreate();            //Кнопка создания Галактики по параметрам
    void buttonCreatefromFile();    //Создание Галактики из файла
};

#endif // MAINWINDOW_H
