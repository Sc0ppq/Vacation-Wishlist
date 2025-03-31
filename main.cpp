#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFormLayout>
#include <QListWidget>
#include <vector>
#include <string>
#include "GUI.h"
#include "Validator.h"
using namespace std;
int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    WishlistRepo wishlist;
    string fileName = "/Users/Miha/source/repos/lab10/oferte.txt";
    OfferFileRepo repo{fileName};
    OfferValidator val;
    Offers serv{ repo,val,wishlist };
    GUI gui(serv);
    gui.show();
    return QApplication::exec();
}
