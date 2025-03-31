#include "WishlistRepo.h"
#include <assert.h>


void WishlistRepo::add_wishlist(const Oferta& of) {
    const string& denumire = of.getDenumire();
    const string& destinatie = of.getDestinatie();
    if (search_oferta_wishlist(denumire, destinatie) != -1)
        throw WishlistException("Oferta este deja in Wishlist.");
    else
    {
    this->Wishlist.push_back(of);
    }

}

int WishlistRepo::search_oferta_wishlist(const std::string& denumire, const std::string& destinatie) {
    auto it = find_if(this->Wishlist.begin(), this->Wishlist.end(), [denumire, destinatie](const Oferta& of) {
        return of.getDenumire() == denumire && of.getDestinatie() == destinatie;
        });

    if (it != this->Wishlist.end()) {
        long long poz = std::distance(this->Wishlist.begin(), it);
        return poz;
    }

    return -1;
}


void WishlistRepo::empty_wishlist()  {
    this->Wishlist.clear();
}
void WishlistRepo::generate_wishlist(int nr_oferte, const vector<Oferta>& memory) {
    empty_wishlist();
    auto to_wishlist = memory;
    shuffle(to_wishlist.begin(), to_wishlist.end(), std::default_random_engine(time(0)));
    while (nr_oferte && !to_wishlist.empty())
    {
        add_wishlist(to_wishlist.back());
        to_wishlist.pop_back();
        nr_oferte--;
    }
}
const vector<Oferta>& WishlistRepo::get_Wishlist() {
    return this->Wishlist;
}

void WishlistRepo::export_wishlist(const std::string& name_file) {
    string filename = name_file;
    auto pos = filename.find_last_of(".");
    if (pos < filename.size())
    {
        auto file_extension = name_file.substr(pos);
        if (file_extension != ".html")
            filename = filename + ".html";
    }
    else
    {
        filename += ".html";
    }

    std::ofstream fout(filename);
    fout << "<html>\n"
        "<body>\n"
        "<head>\n"
        "    <h3 style=\" font-family: 'Georgia',serif; color:indigo; \">Wishlist</h3>\n"
        "    </head>\n"
        "<style>\n"
        "    p {\n"
        "        color: #D7BDE2 ;\n"
        "        background-color: #A9CCE3 ;\n"
        "        border: 1px solid black; }\n"
        "    table,th,td{\n"
        "        border: 1px solid black;\n"
        "    }\n"
        "    th{\n"
        "        color: darkslateblue;\n"
        "    }\n"
        "    body{\n"
        "        background-color: #CACFD2;\n"
        "    }\n"
        "</style>\n";
    fout << "<table>\n"
        "\n"
        "        <th>Name</th>\n"
        "        <th>Destination</th>\n"
        "        <th>Type</th>\n"
        "        <th>Price</th>";
    for (const Oferta& oferta : get_Wishlist())
    {
        fout << "<tr>\n";
        fout << "<th>" << oferta.getDenumire() << "</th>\n"
            << "<th>" << oferta.getDestinatie() << "</th>\n"
            << "<th>" << oferta.getTip() << "</th>\n"
            << "<th>" << oferta.getPret() << "</th>\n"
            << "</tr>\n";
    }
    fout << "</table>\n"
        "\n"
        "\n"
        "</body>\n"
        "</html>";
}

void testAddToWishlist() {
    WishlistRepo wishlistRepo;
    Oferta oferta1("Denumire1", "Destinatie1", "Tip1", 100);
    Oferta oferta2("Denumire2", "Destinatie2", "Tip2", 200);

    wishlistRepo.add_wishlist(oferta1);
    wishlistRepo.add_wishlist(oferta2);

    const auto& wishlist = wishlistRepo.get_Wishlist();
    assert(wishlist.size() == 2);
    assert(wishlist[0].getDenumire() == "Denumire1");
    assert(wishlist[0].getDestinatie() == "Destinatie1");
    assert(wishlist[1].getDenumire() == "Denumire2");
    assert(wishlist[1].getDestinatie() == "Destinatie2");

    try
    {
        wishlistRepo.add_wishlist(oferta1);
    }
    catch (WishlistException& we)
    {
        assert(we.getErrorMessage() == "Oferta este deja in Wishlist.");
    }

}

void testSearchOfertaWishlist() {
    WishlistRepo wishlistRepo;
    Oferta oferta1("Denumire1", "Destinatie1", "Tip1", 100);
    Oferta oferta2("Denumire2", "Destinatie2", "Tip2", 200);

    wishlistRepo.add_wishlist(oferta1);
    wishlistRepo.add_wishlist(oferta2);

    int pozitie1 = wishlistRepo.search_oferta_wishlist("Denumire1", "Destinatie1");
    assert(pozitie1 == 0);

    int pozitie2 = wishlistRepo.search_oferta_wishlist("Denumire3", "Destinatie3");
    assert(pozitie2 == -1);
}

void testEmptyWishlist() {
    WishlistRepo wishlistRepo;
    Oferta oferta1("Denumire1", "Destinatie1", "Tip1", 100);
    Oferta oferta2("Denumire2", "Destinatie2", "Tip2", 200);

    wishlistRepo.add_wishlist(oferta1);
    wishlistRepo.add_wishlist(oferta2);

    wishlistRepo.empty_wishlist();

    const auto& wishlist = wishlistRepo.get_Wishlist();
    assert(wishlist.empty() == true);
}

void testGenerateWishlist() {
    WishlistRepo wishlistRepo;
    vector<Oferta> memory = {
        Oferta("Denumire1", "Destinatie1", "Tip1", 100),
        Oferta("Denumire2", "Destinatie2", "Tip2", 200),
        Oferta("Denumire3", "Destinatie3", "Tip3", 300)
    };

    wishlistRepo.generate_wishlist(2, memory);

    const auto& wishlist = wishlistRepo.get_Wishlist();
    assert(wishlist.size() == 2);
    wishlistRepo.export_wishlist("test");
}


void testWishlist()
{
    testAddToWishlist();
    testSearchOfertaWishlist();
    testEmptyWishlist();
    testGenerateWishlist();
}