#include "qpayback.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <fstream>

using namespace std;

unsigned long long Maszyna::Licznik = 0; //Liczinik egzemplarzy maszyn
Maszyna **Maszyna::wskMaszyna = nullptr;

Maszyna::Maszyna(Ui::MainWindow *& uiMw, const char NAZ[25], int CzCy, int IlOp, double WyWyd, double staw, int IlZm, int DnRok, int IlGz) : CzasCyklu(CzCy), IloscOperatorow(IlOp), WymaganaWydajnosc(WyWyd), Stawka(staw), IloscZmian(IlZm), IloscDniRok(DnRok), IloscGodzinZm(IlGz)
{
    strcpy_s(Nazwa, NAZ);
    uiMainWindow=uiMw;
} //    definicja Maszyna::Maszyna - konstruktor

        //Maszyna::Maszyna(const char NAZ[25], int CzCy, int IlOp, double WyWyd, double staw, int IlZm, int DnRok, int IlGz) : CzasCyklu(CzCy), IloscOperatorow(IlOp), WymaganaWydajnosc(WyWyd), Stawka(staw), IloscZmian(IlZm), IloscDniRok(DnRok), IloscGodzinZm(IlGz)
        //{
        //    strcpy_s(Nazwa, NAZ);
        //} //    definicja Maszyna::Maszyna - konstruktor

Maszyna::Maszyna(const Maszyna & wzorzec)
{
    CzasCyklu = wzorzec.CzasCyklu;
    IloscOperatorow = wzorzec.IloscOperatorow;
    WymaganaWydajnosc = wzorzec.WymaganaWydajnosc;
    Stawka = wzorzec.Stawka;
    IloscZmian = wzorzec.IloscZmian;
    IloscDniRok = wzorzec.IloscDniRok;
    IloscGodzinZm = wzorzec.IloscGodzinZm;
    strcpy_s(Nazwa, wzorzec.Nazwa);
    uiMainWindow = wzorzec.uiMainWindow;
}   // definicja Maszyna::Maszyna - konstruktor kopiujacy

void Maszyna::wyswietlMaszyne()
{
    Licznik++;
    numerMaszyny = Licznik;

    //obsługa listy maszyn (tymczasowa lista rozszerzajaca liste poprzednia o jeden egzemplarz)
    Maszyna **nowa = new Maszyna*[Licznik];

    //kopiowanie danych z tablicy wskaznikow wskMaszyna
    for(unsigned long long i=0; i<Licznik-1; i++)
        nowa[i] = wskMaszyna[i];

    //zapisanie wskaznika na dany egzemplarz Maszyny w tablicy wskazników nowa
    nowa[Licznik-1] = this;

    //usuniecie wskMaszyna po uprzednik skopiowaniu calej zawartosci do nowej tablicy wskaznikow nowa
    delete [] wskMaszyna;

    //przypisanie wskMaszyna nowej tablicy
    wskMaszyna = nowa;

    QString OpisM = "Maszyna: ";
    QWidget *newtab = new QWidget;

    uiMainWindow->tabWidget->insertTab(  ((uiMainWindow->tabWidget->currentIndex())),   newtab, OpisM.append(QString::number(numerMaszyny) ) );
    this->NazMasz->setText(Nazwa);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Nazwa maszyny:", NazMasz);
    formLayout->addRow("Czas cyklu całkowity [s]:", CT);
    formLayout->addRow("Ilość operatorów / maszynę:", IO);
    formLayout->addRow("Ilość zmian / dobę:", IZ);
    formLayout->addRow("Ilość dni roboczych / rok:", ID);
    formLayout->addRow("Ilość godzin / zmianę:", IG);
    formLayout->addRow("OEE:", OEE);
    QBoxLayout *BoxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    BoxLayout->addLayout(formLayout);
    newtab->setLayout(BoxLayout);

    przelicz = new QPushButton("Przelicz");
    BoxLayout->addWidget(przelicz);
    connect(this->przelicz, SIGNAL(clicked()), this, SLOT(przelicz_clicked()));


    QFormLayout *maszynaPodsum = new QFormLayout;
    maszynaPodsum->addRow("Wydajność [szt/tydz.]:",  wydajnosc);
    maszynaPodsum->addRow("Wymagana ilość maszyn:",  ilmasz);
    maszynaPodsum->addRow("Zapotrzebowanie na operatorów:", zapnaop);
    maszynaPodsum->addRow("Koszt operatorów [PLN]:", kosztop);
    BoxLayout->addLayout(maszynaPodsum);


    uiMainWindow->tabWidget->setCurrentIndex( (uiMainWindow->tabWidget->currentIndex())  );
    qDebug() << "Wstawiliśmy maszynę za zakładką nr: " <<(uiMainWindow->tabWidget->currentIndex());

    if((uiMainWindow->tabWidget->count())==Licznik)
        qDebug() << "OK - licznik (" << Licznik << ") = liczba zakładek (" << uiMainWindow->tabWidget->count() << ")" ;
    else {
        qDebug() << "NOK - licznik (" << Licznik << ") != liczba zakładek (" << uiMainWindow->tabWidget->count() << ")" ;
    }


    for (int i=0; i<(uiMainWindow->tabWidget->count()); i++ )
    {
        OpisM = "Maszyna: ";
        uiMainWindow->tabWidget->setTabText(i, (OpisM.append(QString::number(wskMaszyna[i]->numerMaszyny))));
    }

    uiMainWindow->AktualneKosztOp->setText((QString::number(Maszyna::calkowityKoszt(), 'f', 2)).append("\tPLN"));
    uiMainWindow->IloscOper->setText((QString::number(Maszyna::iloscOperatorow(), 'f', 1)));

} // definicja Maszyna::wyswietlMaszyne



void Maszyna::usunMaszyne(unsigned long long nrMaszyny)
{
    Licznik--;
    qDebug() << "Nr Maszyny: " << nrMaszyny;

    Maszyna **nowa = new Maszyna * [Licznik];

    for(unsigned long long i=0; i<nrMaszyny; i++)
        nowa[i] = wskMaszyna[i];

    for(unsigned long long i=nrMaszyny; i<Licznik; i++)
        nowa[i] = wskMaszyna[i+1];

    delete [] wskMaszyna;
    wskMaszyna = nowa;

    qDebug() << "Kasujemy Maszyne";

    //wskMaszyna[nrMaszyny]->uiMainWindow->AktualneKosztOp->setText(QString::number(calkowityKoszt()));


}


double Maszyna::calkowityKoszt()
{
    double kosztCalk = 0;
    for(unsigned long long i=0; i<Licznik; i++)
    {
        kosztCalk += wskMaszyna[i]->KosztOperatorow;
    }

    qDebug() << "Koszt całkowity : " << kosztCalk;
    return kosztCalk;
}



double Maszyna::iloscOperatorow()
{
    double iloscOperatorow = 0;
    for(unsigned long long i=0; i<Licznik; i++)
    {
        iloscOperatorow += wskMaszyna[i]->ZapotrzebowanieNaOperatorow;
    }

    qDebug() << "Ilość operatorów : " << iloscOperatorow;
    return iloscOperatorow;
}

void Maszyna::Uaktualnij()
{
        CzasCyklu           = this->CT->text().toInt();
        IloscOperatorow     = this->IO->text().toInt();
        IloscZmian          = this->IZ->text().toInt();
        IloscDniRok         = this->ID->text().toInt();
        IloscGodzinZm       = this->IG->text().toInt();
        efektywnoscOEE      = this->OEE->text().toDouble();

        qDebug() << "po zaciągnięciu z komórki CT = " << CzasCyklu;

        WydajnoscTydzien = ( (3600/CzasCyklu) * IloscGodzinZm * IloscZmian * 5 * efektywnoscOEE);
        ZapotrzebowanieNaMaszyny = WymaganaWydajnosc/WydajnoscTydzien;
        ZapotrzebowanieNaOperatorow = (WymaganaWydajnosc/WydajnoscTydzien) * IloscOperatorow;
        KosztOperatorow = (Stawka * 8 * IloscZmian * IloscDniRok * ZapotrzebowanieNaOperatorow);

        this->wydajnosc->setText(QString::number(this->WydajnoscTydzien, 'f', 0));
        this->ilmasz->setText(QString::number(this->ZapotrzebowanieNaMaszyny, 'f', 1));
        this->zapnaop->setText(QString::number(this->ZapotrzebowanieNaOperatorow, 'f', 1));
        this->kosztop->setText(QString::number(this->KosztOperatorow,'f', 2));

        uiMainWindow->AktualneKosztOp->setText((QString::number(Maszyna::calkowityKoszt(), 'f', 2)).append("\tPLN"));
        uiMainWindow->IloscOper->setText((QString::number(Maszyna::iloscOperatorow(), 'f', 1)));

}  //   definicja Maszyna::Uaktualnij


void Maszyna::przelicz_clicked()
{
    this->Uaktualnij();
}  // definicja slotu po kliknieciu przycisku przelicz


MaszynaMod::MaszynaMod(Ui::MainWindow *&uiMw, double WyWyd, int IM)
    : Maszyna(uiMw,"Podaj nazwę!", 15, 1, WyWyd), iloscMaszyn(IM)
{}


MaszynaMod::~MaszynaMod()
{
    qDebug() << "Tera bydzie jechoł destruktor z tym koksym i miażdził";
    clearMaszynaModSignal();
}


void MaszynaMod::WyswietlMaszyneModMan()
{

    ZaladunekAutomatyczny = false;
    formLayoutMod = new QFormLayout;
    formLayoutMod->addRow("ilość maszyn w gnieździe", IM);
    formLayoutMod->addRow("czas cyklu maszyny [s]", CTM);
    formLayoutMod->addRow("czas cyklu operatora [s]", CTO);
    CTlabel = new QLabel(QString::number(this->CTPieceToPiece()));  //czas cyklu wyliczony
    formLayoutMod->addRow("czas cyklu 'piece to piece' [s]      ", CTlabel);
    formLayoutMod->addRow("ilość operatorów / gniazdo", IO);
    formLayoutMod->addRow("ilość zmian / dobę", IZ);
    formLayoutMod->addRow("ilość dni roboczych / rok", ID);
    formLayoutMod->addRow("ilość godzin / zmianę", IG);

    uiMainWindow->frame->setLayout(formLayoutMod);


    przelicz = new QPushButton("Przelicz");
    boxLayoutMod = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayoutMod->addWidget(przelicz);
    uiMainWindow->frame_2->setLayout(boxLayoutMod);


    connect(this->CTO, SIGNAL(editingFinished()), this, SLOT(on_CTO_editingFinished()));
    connect(this->CTM, SIGNAL(editingFinished()), this, SLOT(on_CTM_editingFinished()));


}

void MaszynaMod::WyswietlMaszyneModAut()
{

    ZaladunekAutomatyczny = true;
    formLayoutMod = new QFormLayout;
    formLayoutMod->addRow("ilość maszyn w gnieździe", IM);
    formLayoutMod->addRow("czas cyklu maszyny [s]", CTM);
    formLayoutMod->addRow("czas cyklu operatora [s]", CTO);
    CTlabel = new QLabel(QString::number(this->CTPieceToPiece()));
    formLayoutMod->addRow("czas cyklu 'piece to piece' [s]      ", CTlabel);
    formLayoutMod->addRow("ilość operatorów / gniazdo", IO);
    formLayoutMod->addRow("ilość zmian / dobę", IZ);
    formLayoutMod->addRow("ilość dni roboczych / rok", ID);
    formLayoutMod->addRow("ilość godzin / zmianę", IG);

    uiMainWindow->frame->setLayout(formLayoutMod);


    przelicz = new QPushButton("Przelicz");
    boxLayoutMod = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayoutMod->addWidget(przelicz);
    uiMainWindow->frame_2->setLayout(boxLayoutMod);


    connect(this->CTO, SIGNAL(editingFinished()), this, SLOT(on_CTO_editingFinished()));
    connect(this->CTM, SIGNAL(editingFinished()), this, SLOT(on_CTM_editingFinished()));
}


void Maszyna::zapiszMaszyne(QString FileName)
{


    for(unsigned int  i = 0; i < Licznik ; i++ )
    {

        string SFileName = FileName.toLocal8Bit().constData();
        fstream savefile;
        savefile.open(SFileName, ios::app);   //app - append moze byc tez out

        savefile<<wskMaszyna[i]->Nazwa               <<endl;
        savefile<<wskMaszyna[i]->CzasCyklu           <<endl;
        savefile<<wskMaszyna[i]->IloscOperatorow     <<endl;
        savefile<<wskMaszyna[i]->WymaganaWydajnosc   <<endl;
        savefile<<wskMaszyna[i]->Stawka              <<endl;
        savefile<<wskMaszyna[i]->IloscZmian          <<endl;
        savefile<<wskMaszyna[i]->IloscDniRok         <<endl;
        savefile<<wskMaszyna[i]->IloscGodzinZm       <<endl;
        savefile<<"-"                                <<endl;

        savefile.close();

    }

};

//CzasCyklu(CzCy),
//IloscOperatorow(IlOp),
//WymaganaWydajnosc(WyWyd),
//Stawka(staw),
//IloscZmian(IlZm),
//IloscDniRok(DnRok),
//IloscGodzinZm(IlGz)





double Maszyna::CTPieceToPiece()
{
    CTMaszyny =         this->CTM->text().toInt();
    CTOperatora =       this->CTO->text().toInt();

    if(ZaladunekAutomatyczny)
    {
        if(CTMaszyny>CTOperatora){
            return CTMaszyny;
        }
        else {
            return CTOperatora;
        }
    }
    else {
            return CTMaszyny + CTOperatora;
    }
}




void MaszynaMod::on_CTO_editingFinished()
{
    this->CTlabel->setText(QString::number(CTPieceToPiece()));
}

void MaszynaMod::on_CTM_editingFinished()
{
    this->CTlabel->setText(QString::number(CTPieceToPiece()));
}


