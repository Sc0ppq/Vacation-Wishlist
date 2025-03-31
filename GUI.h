#include "Observer.h"
#include "MyTable.h"
#include "MyList.h"
#include <QPaintEvent>
#include <QPainter>
#include <QMessageBox>
#include <QWidget>
#include <QVBoxLayout>
#include<QLayoutItem>
#include <QHBoxLayout>
using std::pair;
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QLabel>
#include <QDebug>
#include <QListWidget>
#include <QTableWidget>
#include "Service.h"
#include <QRadioButton>
#include <QHeaderView>
#include <QSplitter>
#include <QWidget>
class WishlistReadOnlyGUI;

class Wishlist : public QWidget, public Observable, public Observer {
    friend class Offers;
    friend class APPGUI;
    friend class WishlistReadOnlyGUI;
private:
    Offers& wsh;
    //buttons
    QPushButton* refreshL = new QPushButton{ "&REFRESH LIST" };
    QPushButton* add_wishlist = new QPushButton{ "&ADD TO WISHLIST" };
    QPushButton* empty_wishlist = new QPushButton{ "&EMPTY WISHLIST" };
    QPushButton* generate_wishlist = new QPushButton{ "&GENERATE WISHLIST" };
    QPushButton* export_wishlist = new QPushButton{ "&EXPORT WISHLIST" };
    QPushButton* exit_wishlist = new QPushButton{ "&EXIT" };
    //text fields
    QLineEdit* txtWishlistDenumire = new QLineEdit;
    QLineEdit* txtWishlistDestinatie = new QLineEdit;
    QLineEdit* txtWishlistNumar = new QLineEdit;
    QLineEdit* txtWishlistFisier = new QLineEdit;
    //list
    //QListWidget* wishlist_list = new QListWidget;
    QListView* wishlist_view;
    ListModel* offerListModel;
    void ui_addWishlist();
public:
    explicit Wishlist(Offers& srv) : wsh(srv) {
        //addObserver(this);
    };
    void innit_wishlist();
    void connectSignalWishlist();
    void reloadList(const vector<Oferta>& oferta);
    void update() override {
        reloadList(wsh.getAllWishlist());

    }

};

class WishlistReadOnlyGUI : public QWidget, public Observer {
    friend class Contract;
    Wishlist* wish;
    QListWidget* lista_obj;
public:
    explicit WishlistReadOnlyGUI(Wishlist* w) : wish(w) {
        wish->addObserver(this);
    }
    void innitWishlist()
    {
        lista_obj = new QListWidget();
        QHBoxLayout* main = new QHBoxLayout;
        main->addWidget(lista_obj);
        this->setLayout(main);
    }
    void update() override {
        this->repaint();
    }
    void reload()
    {
        lista_obj->clear();
        for (const auto& o : wish->wsh.getAllWishlist())
        {
            lista_obj->addItem(QString::fromStdString(o.to_string_print()));
        }
    }

    void paintEvent(QPaintEvent* e) override {
        QPainter p(this);
        
        QImage image("C:/Users/Miha/Downloads/pexels-pixabay-53594.jpg");
        if (image.isNull()) {
            qDebug() << "Imaginea nu poate fi incarcata!";
        }
        else {
            qDebug() << "Imaginea a fost incarcata cu succes!";
        }

        p.drawImage(0, 0, QImage("C:/Users/Miha/Downloads/pexels-pixabay-53594.jpg"));
        srand(time(0));
        int x = -20;
        for (const auto& elem : wish->wsh.getAllWishlist())
        {
            x += 40;
            int forma = rand() % 4;
            int inaltime = rand() % 130;
            int start_y = rand() % 60;
            QColor color;

            int color_num = rand() % 5;
            switch (color_num)
            {
            case 0:
                color = Qt::red;
                break;
            case 1:
                color = Qt::green;
                break;
            case 2:
                color = Qt::black;
                break;
            case 3:
                color = Qt::blue;
                break;
            case 4:
                color = Qt::lightGray;
                break;
            default:
                break;
            }

            switch (forma)
            {
            case 0: // dreptunghi
                p.drawRect(x, start_y, 20, inaltime);
                break;
            case 1: // elipsa
                p.drawEllipse(x, start_y, 20, inaltime);
                break;
            case 2: // dreptunghi colorat
                p.fillRect(x, start_y, 20, inaltime, color);
                break;
            default: // dreptunghi colorat
                p.fillRect(x, start_y, 20, inaltime, color);
                break;
            }
        }
    }

    ~WishlistReadOnlyGUI()
    {
        wish->removeObserver(this);
    }

};


class GUI :public QWidget {
public:
    GUI(Offers& ser) :service{ ser } {
        initgui();
        //wishlistgui();
        loadTable(ser.getAllOffers());
        this->wishlist = new Wishlist(service);
        wishlist->innit_wishlist();
        wishlist->connectSignalWishlist();
        //loadTable(service.getAll());
        connectSignalSlots();
    }

private:
    Offers& service;
    QTableView* table_main = new QTableView; // Change from QTableWidget to QTableView
    OfferTableModel* offerTableModel = new OfferTableModel(this); // Add the model

    // BUTOANE

    //specifice undo
    QPushButton* undo = new QPushButton{ "&UNDO" };
    QPushButton* refreshButton = new QPushButton("&REFRESH");

    //functiile principale
    QPushButton* add = new QPushButton{ "&ADAUGA" };
    QPushButton* del = new QPushButton{ "&STERGE" };
    QPushButton* modify = new QPushButton{ "&MODIFICA" };

    //sortari
    QRadioButton* sort_denumire = new QRadioButton{ "&Denumire" };
    QRadioButton* sort_destinatie = new QRadioButton{ "&Destinatie" };
    QRadioButton* sort_pret_si_tip = new QRadioButton{ "&Pret si Tip" };

    //filtrari
    QLineEdit* txtfilter = new QLineEdit;
    QLineEdit* txtMinPret = new QLineEdit;
    QLineEdit* txtMaxPret = new QLineEdit;
    QPushButton* filter_pret = new QPushButton{ "&PRET" };
    QPushButton* filter_destinatie = new QPushButton{ "&DESTINATIE" };

    //iesire din aplicatie
    QPushButton* exit = new QPushButton{ "&EXIT" };



    //Casutele text pentru o Oferta
    QLineEdit* txtDenumire = new QLineEdit;
    QLineEdit* txtPret = new QLineEdit;
    QLineEdit* txtTip = new QLineEdit;
    QLineEdit* txtDestinatie = new QLineEdit;

    //wishlist
    Wishlist* wishlist;
    QPushButton* open_wishlist = new QPushButton{ "&OPEN WISHLIST" };
    QPushButton* open_row = new QPushButton{ "Open read only wishlist" };
    
   

    

    //void loadList(const vector<Oferta>& oferta);
    void loadTable(const vector<Oferta>& oferta);
    //void loadTableWishlist(const vector<Oferta>& oferta);
    void connectSignalSlots();

    void initgui();
    //void wishlistgui();

    void ui_add();
    void ui_delete();
    void ui_modify();
    void ui_filter_pret();
    void ui_filter_destinatie();
    void ui_sort_n();
    void ui_sort_destinatie();
    void ui_sort_pret_si_tip();
    void ui_refresh();
    //void ui_addWishlist();




};