#include "GUI.h"
#include "MyTable.h"
#include "MyList.h"
void GUI::initgui() {
    this->setWindowTitle("Agentie de turism");

    // Stilul butoanelor 
    QString buttonStyle = "QPushButton {"
        "background-color: #2471A3;"
        "border: 1px solid #1F618D;"
        "border-radius: 5px;"
        "color: white;"
        "padding: 8px 16px;"
        "font-size: 14px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #1F618D;"
        "border: 1px solid #2471A3;"
        "}"
        "QPushButton:pressed {"
        "background-color: #154360;"
        "border: 1px solid #212F3C;"
        "}";

    // Paleta de culori
    QPalette palette = QPalette();
    palette.setColor(QPalette::Window, "#2c3e50");
    palette.setColor(QPalette::WindowText, "#ecf0f1");
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    // Font
    QFont font("Verdana", 12);
    this->setFont(font);

    // Layout-ul principal 
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // Splitteri intre lista si optiuni
    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    mainLayout->addWidget(splitter);

    // Stanga - lista de oferte, butoanele pentru undo si refresh
    QWidget* leftWidget = new QWidget;
    QVBoxLayout* leftLayout = new QVBoxLayout;
    leftWidget->setLayout(leftLayout);
    leftLayout->addWidget(new QLabel("<font color=\"#ecf0f1\"><u><b>Lista de Oferte</b></u></font>"));
    leftLayout->addWidget(table_main);
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    undo->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(undo);
    open_wishlist->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(open_wishlist);
    refreshButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(refreshButton);
    leftLayout->addLayout(buttonLayout);
    splitter->addWidget(leftWidget);

    // Dreapta - layout separat pentru adaugare, modificare, stergere si pentru filtrare si sortare + butonul de exit
    QWidget* rightWidget = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout;
    rightWidget->setLayout(rightLayout);
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow(new QLabel("<font color=\"#ecf0f1\"><b>DENUMIRE:</b></font>"), txtDenumire);
    formLayout->addRow(new QLabel("<font color=\"#ecf0f1\"><b>DESTINATIE:</b></font>"), txtDestinatie);
    formLayout->addRow(new QLabel("<font color=\"#ecf0f1\"><b>TIP:</b></font>"), txtTip);
    formLayout->addRow(new QLabel("<font color=\"#ecf0f1\"><b>PRET:</b></font>"), txtPret);
    rightLayout->addLayout(formLayout);
    QHBoxLayout* actionLayout = new QHBoxLayout;
    add->setStyleSheet(buttonStyle);
    actionLayout->addWidget(add);
    modify->setStyleSheet(buttonStyle);
    actionLayout->addWidget(modify);
    del->setStyleSheet(buttonStyle);
    actionLayout->addWidget(del);
    rightLayout->addLayout(actionLayout);
    QVBoxLayout* filterLayout = new QVBoxLayout;
    filterLayout->addWidget(new QLabel("<font color=\"#ecf0f1\"><b>SORTARE</b></font>"));
    filterLayout->addWidget(sort_denumire);
    filterLayout->addWidget(sort_destinatie);
    filterLayout->addWidget(sort_pret_si_tip);
    rightLayout->addLayout(filterLayout);
    QHBoxLayout* filterOptionsLayout = new QHBoxLayout;
    filterOptionsLayout->addWidget(new QLabel("<font color=\"#ecf0f1\"><b>PRET MINIM:</b></font>"));
    filterOptionsLayout->addWidget(txtMinPret);
    filterOptionsLayout->addWidget(new QLabel("<font color=\"#ecf0f1\"><b>PRET MAXIM:</b></font>"));
    filterOptionsLayout->addWidget(txtMaxPret);
    filterOptionsLayout->addWidget(filter_pret);
    rightLayout->addLayout(filterOptionsLayout);
    QHBoxLayout* filterDestinationLayout = new QHBoxLayout;
    filterDestinationLayout->addWidget(new QLabel("<font color=\"#ecf0f1\"><b>FILTRU DESTINATIE:</b></font>"));
    filterDestinationLayout->addWidget(txtfilter);
    filterDestinationLayout->addWidget(filter_destinatie);
    rightLayout->addLayout(filterDestinationLayout);
    exit->setStyleSheet(buttonStyle);
    rightLayout->addWidget(exit);
    splitter->addWidget(rightWidget);
    open_row->setStyleSheet(buttonStyle);
    rightLayout->addWidget(open_row);

    // Activitate Lab 
    // Message box pt afisare
    std::vector<Oferta> oferte = service.getAllOffers();
    std::map<std::string, int> destinatiiNumarOferte;

    for (const auto& oferta : oferte) {
        std::string destinatie = oferta.getDestinatie();
        service.addToMap(destinatiiNumarOferte, destinatie);
    }

    for (const auto& pair : destinatiiNumarOferte) {
        std::string destinatie = pair.first;
        int numarOferte = pair.second;
        QPushButton* button = new QPushButton(QString::fromStdString(destinatie));
        button->setStyleSheet(buttonStyle);
        QObject::connect(button, &QPushButton::clicked, [=]() {
            QMessageBox::information(this, "Numar oferte pentru " + QString::fromStdString(destinatie), "Numar oferte: " + QString::number(numarOferte));
            });
        mainLayout->addWidget(button);
    }
}


/*
void Wishlist::reloadList(const vector<Oferta>& oferta) {
    wishlist_list->clear();
    for (const auto& o : oferta)
    {
        wishlist_list->addItem(QString::fromStdString(o.to_string_print()));
    }
}
*/

void Wishlist::reloadList(const vector<Oferta>& oferta) {
    offerListModel->updateOffers(oferta);
}


void Wishlist::innit_wishlist() {
    setWindowTitle("Wishlist");

    // Stilul butoanelor pentru Wishlist
    QString buttonStyleWishlist = "QPushButton {"
        "background-color: #16a085;"
        "border: 1px solid #138d75;"
        "border-radius: 5px;"
        "color: white;"
        "padding: 8px 16px;"
        "font-size: 14px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #138d75;"
        "border: 1px solid #16a085;"
        "}"
        "QPushButton:pressed {"
        "background-color: #0e6655;"
        "border: 1px solid #0b5345;"
        "}";

    // Paleta de culori pentru Wishlist
    QPalette paletteWishlist = QPalette();
    paletteWishlist.setColor(QPalette::Window, "#092b2b");
    paletteWishlist.setColor(QPalette::WindowText, "#ecf0f1");
    setAutoFillBackground(true);
    setPalette(paletteWishlist);
    QFont font2("Comic Sans", 12);
    setFont(font2);

    // Layout-ul orizontal pentru a aranja lista de oferte si tabelul cu wishlist
    QHBoxLayout* mainLayout = new QHBoxLayout;
    setLayout(mainLayout);

    // Layout pentru partea stanga (Wishlist)
    QVBoxLayout* leftLayout = new QVBoxLayout;
    mainLayout->addLayout(leftLayout);

    // Adaugam un titlu pentru Wishlist
    leftLayout->addWidget(new QLabel{ "WISHLIST" });

    // Buton pentru golirea Wishlist-ului
    empty_wishlist->setStyleSheet(buttonStyleWishlist);
    leftLayout->addWidget(empty_wishlist);

    // Formular pentru adaugarea de elemente in Wishlist
    QFormLayout* addFormLayout = new QFormLayout;
    addFormLayout->addRow(new QLabel{ "Denumire:" }, txtWishlistDenumire);
    addFormLayout->addRow(new QLabel{ "Destinatie:" }, txtWishlistDestinatie);
    leftLayout->addLayout(addFormLayout);

    // Buton pentru adaugarea in Wishlist
    add_wishlist->setStyleSheet(buttonStyleWishlist);
    leftLayout->addWidget(add_wishlist);

    // Buton pentru generarea Wishlist-ului
    leftLayout->addWidget(new QLabel{ "GENEREAZA" });
    QFormLayout* genLayout = new QFormLayout;
    genLayout->addRow(new QLabel{ "Numarul de oferte care sa se genereze:" }, txtWishlistNumar);
    leftLayout->addLayout(genLayout);
    generate_wishlist->setStyleSheet(buttonStyleWishlist);
    leftLayout->addWidget(generate_wishlist);

    // Buton pentru exportul Wishlist-ului
    leftLayout->addWidget(new QLabel{ "EXPORT" });
    QFormLayout* exportLayout = new QFormLayout;
    exportLayout->addRow(new QLabel{ "FILE:" }, txtWishlistFisier);
    leftLayout->addLayout(exportLayout);
    export_wishlist->setStyleSheet(buttonStyleWishlist);
    leftLayout->addWidget(export_wishlist);

    // Layout pentru partea dreapta (Tabel cu Wishlist)
    QVBoxLayout* rightLayout = new QVBoxLayout;
    mainLayout->addLayout(rightLayout);

    // Lista pentru Wishlist
    //rightLayout->addWidget(wishlist_list);
    wishlist_view = new QListView;
    offerListModel = new ListModel{ this };
    wishlist_view->setModel(offerListModel);
    rightLayout->addWidget(wishlist_view);

    // Buton pentru iesire
    exit_wishlist->setStyleSheet(buttonStyleWishlist);
    rightLayout->addWidget(exit_wishlist);
}


void GUI::loadTable(const std::vector<Oferta>& offers) {
    offerTableModel->setOffers(offers);
    table_main->setModel(offerTableModel);
}


//ui pentru adaugare, verifica daca in casuta pentru pret se adauga numar, restul exceptiilor sunt cele din repo sau validare, care sunt afisate printr-o casuta warning
void GUI::ui_add() {
    string denumire = txtDenumire->text().toStdString();
    string destinatie = txtDestinatie->text().toStdString();
    string tip = txtTip->text().toStdString();
    QString pretText = txtPret->text(); 
    bool ok; 
    double pret = pretText.toDouble(&ok); 

    if (!ok) {
        QMessageBox::warning(this, "WARNING", "Pretul trebuie sa fie un numar valid.");
        return;
    }
    try {
        service.addOffer(denumire, destinatie, tip, pret);
    }
    catch (RepoException& mes) {
        QMessageBox::warning(this, "WARNING", QString::fromStdString(mes.getErrorMessage()));
    }
    catch (ValidationException& mes) {
        QMessageBox::warning(this, "WARNING", QString::fromStdString(mes.getErrorMessages()));
    }

    loadTable(service.getAllOffers());
}

//ui pentru stergere exceptiile sunt cele din repo sau validare, care sunt afisate printr-o casuta warning
void GUI::ui_delete() {
    string denumire = txtDenumire->text().toStdString();
    string destinatie = txtDestinatie->text().toStdString();
    qDebug() << denumire << destinatie;
    try
    {
        service.removeOffer(denumire, destinatie);
    }
    catch (RepoException& mes)
    {
        QMessageBox::warning(this, "WARNING", QString::fromStdString(mes.getErrorMessage()));
    }
    catch (ValidationException& mes)
    {
        QMessageBox::warning(this, "WARNING", QString::fromStdString(mes.getErrorMessages()));
    }
    loadTable(service.getAllOffers());
}

//ui pentru modificare, verifica daca in casuta pentru pret se adauga numar, restul exceptiilor sunt cele din repo sau validare, care sunt afisate printr-o casuta warning
// Se poate modifica doar tipul sau pretul unei oferte, criteriul de cautare pentru oferte e denumirea si destinatia
void GUI::ui_modify() {
    string denumire = txtDenumire->text().toStdString();
    string destinatie = txtDestinatie->text().toStdString();
    string tip = txtTip->text().toStdString();
    QString pretText = txtPret->text();
    bool ok;
    double pret = pretText.toDouble(&ok);
    qDebug() << denumire << destinatie;

    if (!ok) {
        QMessageBox::warning(this, "WARNING", "Pretul trebuie sa fie un numar valid.");
        return;
    }
    try {
        service.modifyOffer(denumire, destinatie, tip, pret);
    }
    catch (RepoException& mes) {
        QMessageBox::warning(this, "WARNING", QString::fromStdString(mes.getErrorMessage()));
    }
    catch (ValidationException& mes) {
        QMessageBox::warning(this, "WARNING", QString::fromStdString(mes.getErrorMessages()));
    }

    loadTable(service.getAllOffers());
}


//ui pentru filtrare dupa pret, verifica daca in casuta pentru pret se adauga numar, in caz ca nu s-a adaugat un numar valid eroarea e afisata printr-o casuta warning
void GUI::ui_filter_pret() {
    QString minPretText = txtMinPret->text();
    QString maxPretText = txtMaxPret->text();
    bool minOk, maxOk;
    double minPret = minPretText.toDouble(&minOk);
    double maxPret = maxPretText.toDouble(&maxOk);

    if (!minOk || !maxOk) {
        QMessageBox::warning(this, "WARNING", "Pretul minim si maxim trebuie sa fie numere valide.");
        return;
    }

    vector<Oferta> offr = service.filterByPrice(minPret, maxPret);
    for (const auto& o : offr)
    {
        qDebug() << o.to_string_print();
    }
    loadTable(offr);
    
}

//ui pentru filtrare dupa destinatie
void GUI::ui_filter_destinatie() {
    string destinatie = txtfilter->text().toStdString();
    qDebug() << destinatie;
    vector<Oferta> offr = service.filterByDestination(destinatie);
    for (const auto& o : offr)
    {
        qDebug() << o.to_string_print();
    }
    loadTable(offr);
}

//ui pentru filtrare dupa denumire
void GUI::ui_sort_n() {
    service.sortBy([&](const Oferta& a, const Oferta& b) {
        return service.compareByDenumire(a, b);
        });
    loadTable(service.getAllOffers());
}

//ui pentru sortare dupa destinatie
void GUI::ui_sort_destinatie() {
    service.sortBy([&](const Oferta& a, const Oferta& b) {
        return service.compareByDestinatie(a, b);
        });
    loadTable(service.getAllOffers());
}

//ui pentru sortare dupa pret si tip
void GUI::ui_sort_pret_si_tip() {
    service.sortBy([&](const Oferta& a, const Oferta& b) {
        return service.compareByPretSiTip(a, b);
        });
    loadTable(service.getAllOffers());
}

//da refresh la lista de oferte, folosita dupa undo sau dupa o filtrare/sortare pentru a arata lista initiala
void GUI::ui_refresh() {
    loadTable(service.getAllOffers());
}


//ui la adaugarea in wishlist
void Wishlist::ui_addWishlist()
{
    string denumire = txtWishlistDenumire->text().toStdString();
    string destinatie = txtWishlistDestinatie->text().toStdString();
    auto gasite = wsh.filterByDestination(destinatie);
    if (gasite.empty())
    {
        QMessageBox::warning(nullptr, "1", "UNFOUND");
        return;
    }
    if (gasite.size() == 1)
    {
        try
        {
            wsh.add_to_wishlist(denumire, gasite[0].getDestinatie(), gasite);
        }
        catch (WishlistException& e)
        {
            QMessageBox::warning(nullptr, "!", QString::fromStdString(e.getErrorMessage()));
            return;
        }
        catch (RepoException& e)
        {
            QMessageBox::warning(nullptr, "!", QString::fromStdString(e.getErrorMessage()));
            return;
        }
        catch (ValidationException& e)
        {
            QMessageBox::warning(nullptr, "!", QString::fromStdString(e.getErrorMessages()));
            return;
        }
    }
    else
    {
        if (denumire != "")
        {
            try
            {
                wsh.add_to_wishlist(denumire, destinatie, gasite);
            }
            catch (WishlistException& e)
            {
                QMessageBox::warning(nullptr, "!", QString::fromStdString(e.getErrorMessage()));
                return;
            }
            catch (RepoException& e)
            {
                QMessageBox::warning(nullptr, "!", QString::fromStdString(e.getErrorMessage()));
                return;
            }
            catch (ValidationException& e)
            {
                QMessageBox::warning(nullptr, "!", QString::fromStdString(e.getErrorMessages()));
                return;
            }
        }
        else
        {
            QMessageBox::warning(nullptr, "!", "Introduceti o denumire valida!");
        }
    }
    reloadList(wsh.getAllWishlist());
    notify();
}


void Wishlist::connectSignalWishlist() {
    this->reloadList(wsh.getAllWishlist());
    QObject::connect(refreshL, &QPushButton::clicked, [&]() {
        reloadList(wsh.getAllWishlist());
        });
    QObject::connect(exit_wishlist, &QPushButton::clicked, [&]() {
        this->notify();
        close();
        });
    QObject::connect(empty_wishlist, &QPushButton::clicked, [&]() {
        wsh.emptyWishlist();
        reloadList(wsh.getAllWishlist ());
        notify();
        });
    QObject::connect(generate_wishlist, &QPushButton::clicked, [&]() {
        string n = txtWishlistNumar->text().toStdString();
        int nr;
        try
        {
            nr = stoi(n);
        }
        catch (std::invalid_argument& m)
        {
            QMessageBox::warning(nullptr, "!", "INVALID ARGUMENT");
        }
        wsh.generateWishlist(nr);

        reloadList(wsh.getAllWishlist());
        notify();
        });
    QObject::connect(add_wishlist, &QPushButton::clicked, this, &Wishlist::ui_addWishlist);
    QObject::connect(export_wishlist, &QPushButton::clicked, [&]() {
        string file = txtWishlistFisier->text().toStdString();
        wsh.exportWishlist(file);
        });

}


//functia de conectare intre butoane si actiuni
void GUI::connectSignalSlots() {
    
    QObject::connect(exit, &QPushButton::clicked, [&]() {
        this->close();
        wishlist->close();
        QMessageBox::information(nullptr, "!", "Se inchide aplicatia");
        });
    QObject::connect(add, &QPushButton::clicked, this, &GUI::ui_add);
    
    QObject::connect(del, &QPushButton::clicked, this, &GUI::ui_delete);
    
    QObject::connect(modify, &QPushButton::clicked, this, &GUI::ui_modify);
    
    QObject::connect(filter_destinatie, &QPushButton::clicked, this, &GUI::ui_filter_destinatie);

    QObject::connect(filter_pret, &QPushButton::clicked, this, &GUI::ui_filter_pret);

    QObject::connect(sort_denumire, &QRadioButton::clicked, this, &GUI::ui_sort_n);

    QObject::connect(sort_destinatie, &QRadioButton::clicked, this, &GUI::ui_sort_destinatie);

    QObject::connect(sort_pret_si_tip, &QRadioButton::clicked, this, &GUI::ui_sort_pret_si_tip);

    //Dupa undo, modificarile se pot observa dupa ce se apasa refresh!!
    //se prinde exceptie in caz ca nu se mai poate face undo si se afiseaza printr-o casuta warning
    QObject::connect(undo, &QPushButton::clicked, [&]() {
        try {
            service.undo();
        }
        catch (ServiceException& mes)
        {
            QMessageBox::warning(nullptr, "!", QString::fromStdString(mes.getErrorMessage()));
        }
        });

    QObject::connect(refreshButton, &QPushButton::clicked, this, &GUI::ui_refresh);

    QObject::connect(open_wishlist, &QPushButton::clicked, [&]() {
        wishlist->show();
        });
    
    QObject::connect(table_main->selectionModel(), &QItemSelectionModel::selectionChanged, this, [&]() {
        auto sel = table_main->selectionModel();

        txtDenumire->setText(sel->selectedRows(1).value(0).data().toString());
        txtDestinatie->setText(sel->selectedRows(3).value(0).data().toString());
        txtTip->setText(sel->selectedRows(2).value(0).data().toString());
        txtPret->setText(sel->selectedRows(4).value(0).data().toString());
        });


    QObject::connect(open_row, &QPushButton::clicked, [&]() {
        WishlistReadOnlyGUI* wishlistRO = new  WishlistReadOnlyGUI(wishlist);
        wishlistRO->show();

        });

}
