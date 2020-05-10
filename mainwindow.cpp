#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qpayback.h"
#include "QDebug"
#include <QFile>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setTabsClosable(true);
    this->setFixedSize(QSize(920, 800));

    ui->WymaganaWydajnoscLineEdit->setText("10000");
    ui->StawkaLineEdit->setText("20");

    connect(maszynaMod, SIGNAL(clearMaszynaModSignal()), this, SLOT(on_maszynaMod_destroyed()));



} // definicja konstruktora MainWindow

MainWindow::~MainWindow()
{
    delete ui;
} // definicja destruktora MainWindow


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
    Maszyna::usunMaszyne(index);
    ui->AktualneKosztOp->setText((QString::number(Maszyna::calkowityKoszt(), 'f', 2)).append("\tPLN"));
    ui->IloscOper->setText((QString::number(Maszyna::iloscOperatorow(), 'f', 1)));
}

void MainWindow::on_pushButDodajMaszyne_clicked()
{
    maszyna = new Maszyna(ui, "Podaj nazwę!", 22, 1, ui->WymaganaWydajnoscLineEdit->text().toDouble(), ui->StawkaLineEdit->text().toDouble());
    maszyna->wyswietlMaszyne();
}



void MainWindow::on_radioButton_manual_clicked()
{
    if(maszynaMod)
        delete maszynaMod;

    maszynaMod = new MaszynaMod(ui, ui->WymaganaWydajnoscLineEdit->text().toDouble(), 3);
    connect(maszynaMod, SIGNAL(clearMaszynaModSignal()), this, SLOT(on_maszynaMod_destroyed()));


   if(ui->radioButton_manual->isChecked())
   {
       qDebug() << "dodajemy interfejs dla maszyny ładowanej ręcznie";
       maszynaMod->WyswietlMaszyneModMan();
   }

}

void MainWindow::on_radioButton_auto_clicked()
{
    if(maszynaMod)
        delete maszynaMod;

    maszynaMod = new MaszynaMod(ui, ui->WymaganaWydajnoscLineEdit->text().toDouble(), 3);
    connect(maszynaMod, SIGNAL(clearMaszynaModSignal()), this, SLOT(on_maszynaMod_destroyed()));

    if(ui->radioButton_auto->isChecked())
    {
        qDebug() << "dodajemy interfejs dla maszyny ładowanej automatycznie";
        maszynaMod->WyswietlMaszyneModAut();
    }
}

void MainWindow::on_pushKasuj_clicked()
{
   if(maszynaMod){
        delete maszynaMod;
   }
}

void MainWindow::kasujMaszynaMod()
{
    while ( ui->frame->layout()->count() != 0) {
    QLayoutItem *forDeletion = ui->frame->layout()->takeAt(0);
    delete forDeletion->widget();
    delete forDeletion;
    }
    delete ui->frame->layout();
    delete ui->frame_2->layout()->takeAt(0)->widget();
    delete ui->frame_2->layout();
}


void MainWindow::on_maszynaMod_destroyed()
{
    maszynaMod = nullptr;
    kasujMaszynaMod();
    qDebug() << "Z inicjatywy destruktora niszcza tu wszystko";
}

void MainWindow::on_actionZapisz_jako_triggered()
{
    QString FileName = QFileDialog::getSaveFileName(this, "Save a file", "D://");
    maszyna->zapiszMaszyne(FileName);
}
