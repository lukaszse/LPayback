#ifndef QPAYBACK_H
#define QPAYBACK_H

#include <iostream>
#include <string.h>
#include <QString>
#include <QWidget>
#include <QFormLayout>
#include <QLayout>
#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QDialog>
#include <QMainWindow>
#include <QPushButton>
#include <QObject>
#include <fstream>
#include <QFileDialog>

#include "ui_mainwindow.h"


using namespace std;

class Maszyna : public QMainWindow
{
    Q_OBJECT
    friend class MainWindow;
    friend class MaszynaMod;

    int NumerMaszyny;
    double CTMaszyny = 10;
    double CTOperatora = 10;
    double CzasCyklu; //czas cyklu piece to piece (=CTMaszyny jesli maszyna z automatycznym załadunkiem; =CTMaszyny+CTOperatora jeśli maszyna z zał. ręcznym(
    double IloscOperatorow; //rzeczywista ilość operatorów potrzebna do obsługi jednej maszyny
    double efektywnoscOEE = 0.8;
    double WymaganaWydajnosc;
    double Stawka;
    int IloscZmian;
    int IloscDniRok;
    int IloscGodzinZm;
    char Nazwa[25];
    bool ZaladunekAutomatyczny = false;
    Ui::MainWindow *uiMainWindow;

    double WydajnoscTydzien = ((3600 / CzasCyklu) * IloscGodzinZm * IloscZmian * 5 *efektywnoscOEE);
    double ZapotrzebowanieNaOperatorow = (WymaganaWydajnosc/WydajnoscTydzien) * IloscOperatorow; //rzeczywista ilość operatorów potrzebna do obsługi maszyny
    double KosztOperatorow = (Stawka * 8 * IloscZmian * IloscDniRok * ZapotrzebowanieNaOperatorow);
    double ZapotrzebowanieNaMaszyny = WymaganaWydajnosc/WydajnoscTydzien;


    QLineEdit *NazMasz = new QLineEdit(Nazwa);
    QLineEdit *CTM = new QLineEdit(QString::number(this->CTMaszyny));
    QLineEdit *CTO = new QLineEdit(QString::number(this->CTOperatora));
    QLineEdit *CT = new QLineEdit(QString::number(this->CzasCyklu));
    QLabel *CTlabel = new QLabel(QString::number(this->CzasCyklu));  //czas cyklu wyliczony
    QLineEdit *IO = new QLineEdit(QString::number(this->IloscOperatorow));
    QLineEdit *IZ = new QLineEdit(QString::number(this->IloscZmian));
    QLineEdit *ID = new QLineEdit(QString::number(this->IloscDniRok));
    QLineEdit *IG = new QLineEdit(QString::number(this->IloscGodzinZm));
    QLineEdit *OEE = new QLineEdit(QString::number(this->efektywnoscOEE));


    QLineEdit *wydajnosc = new QLineEdit( QString::number(this->WydajnoscTydzien, 'f', 0 ) );
    QLineEdit *ilmasz = new QLineEdit ( QString::number(this->ZapotrzebowanieNaMaszyny, 'f', 1));
    QLineEdit *zapnaop = new QLineEdit( QString::number(this->ZapotrzebowanieNaOperatorow, 'f', 1 ) ) ;
    QLineEdit *kosztop = new QLineEdit( QString::number(this->KosztOperatorow, 'f', 2 ) );




public:
    static unsigned long long Licznik;
    unsigned long long numerMaszyny = 0;
    static Maszyna **wskMaszyna;

    Maszyna(Ui::MainWindow *& uiMw, const char NAZ[25], int CzCy, int IlOp, double WyWyd, double staw=42.37, int IlZm=3, int DnRok=251, int IlGz=8);
    Maszyna(const char NAZ[25], int CzCy, int IlOp, double WyWyd, double staw=42.37, int IlZm=3, int DnRok=251, int IlGz=8);
    Maszyna(const Maszyna & wzorzec);

    double CTPieceToPiece();
    void Uaktualnij();
    void wyswietlMaszyne();

    static void usunMaszyne(unsigned long long nrMaszyny);
    static double calkowityKoszt();
    static double iloscOperatorow();

    void zapiszMaszyne(QString FileName);


    QPushButton *przelicz;


public slots:
    void przelicz_clicked();


};


class MaszynaMod : public Maszyna
{
    Q_OBJECT

    friend class MainWindow;
    int iloscMaszyn=0;
    bool isEmpty=true;
    Ui::MainWindow *uiMAinWindow;

    QBoxLayout *boxLayoutMod = nullptr;
    QFormLayout *formLayoutMod = nullptr;
    QFrame *frame = nullptr;

    QLineEdit *IM = new QLineEdit(QString::number(iloscMaszyn));
    QLineEdit *CT = new QLineEdit(QString::number(CzasCyklu));
    QLineEdit *IO = new QLineEdit(QString::number(IloscOperatorow));

public:

    MaszynaMod(Ui::MainWindow *&uiMw, double WyWyd, int IM);

    ~MaszynaMod();

    void WyswietlMaszyneModMan();
    void WyswietlMaszyneModAut();
    void HideAllWidgets(QLayout & layout);
    void ClearLayout(QLayout & layout);

public slots:

    void on_CTO_editingFinished();
    void on_CTM_editingFinished();

signals:
     void clearMaszynaModSignal();


};

#endif // QPAYBACK_H
