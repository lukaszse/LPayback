#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qpayback.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class Maszyna;
    friend class MaszynaMod;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Maszyna *maszyna;
    MaszynaMod *maszynaMod = nullptr;    

    void kasujMaszynaMod();
    int zsumuj();

public slots:
    void on_tabWidget_tabCloseRequested(int index);
    void on_pushButDodajMaszyne_clicked();
    void on_radioButton_manual_clicked();
    void on_radioButton_auto_clicked();
    void on_pushKasuj_clicked();
    void on_maszynaMod_destroyed();
    void on_actionZapisz_jako_triggered();



signals:
    void clearMaszynaMod();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
