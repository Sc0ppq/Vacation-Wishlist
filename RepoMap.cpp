
#include <algorithm>
#include "Repository.h"


void RepoMap::random() const throw(RepoException) {
    double random = ((double)rand() / (RAND_MAX));
    if (random > probabilitate) {
        random *= 100;
        cout << "Probabilitatea: " << random << endl;
        throw (RepoException("Nu se poate executa aceasta functie!!"));
    }
}

bool RepoMap::exists(const Oferta& o) {
    random();
    for (auto& of : oferte) {
        if (of.second.getDenumire() == o.getDenumire() && of.second.getDestinatie() == o.getDestinatie()) {
            return true;
        }
    }
    return false;
}

void RepoMap::setProbabilitate(double p) {
    probabilitate = p;
}


void RepoMap::store(const Oferta& o) {
    random();
    if (exists(o)) {
        throw RepoException("Oferta exista deja in lista.");
    }
    int i = int(oferte.size());
    oferte.insert(std::make_pair(i, o));
}

void RepoMap::remove(const Oferta& o) {
    random();
    auto it = std::find_if(oferte.begin(), oferte.end(), [&](const std::pair<int, Oferta>& p) {
        return p.second.getDenumire() == o.getDenumire() && p.second.getDestinatie() == o.getDestinatie();
        });
    if (it != oferte.end()) {
        oferte.erase(it);
    }
    else {
        throw RepoException("Oferta nu exista!");
    }
}

const Oferta& RepoMap::find(std::string denumire, std::string destinatie) {
    random();
    auto it = find_if(oferte.begin(), oferte.end(), [=](const std::pair<int, Oferta>& o) {
        return o.second.getDenumire() == denumire && o.second.getDestinatie() == destinatie;
        });
    if (it != oferte.end()) {
        return (*it).second;
    }
    else {
        throw RepoException("Oferta nu exista.");
    }
}

vector<Oferta> OFERTE;

vector<Oferta>& RepoMap::getAllOffers() {
    random();
    OFERTE.clear();
    for (auto& o : oferte) {
        OFERTE.push_back(o.second);
    }
    return OFERTE;
}
