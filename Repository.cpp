#include "Repository.h"
#include <assert.h>
#include <algorithm>
#include <functional>
#include <fstream>
#include <filesystem>
bool OfferRepository::exists(const Oferta& of) {
	try {
		find(of.getDenumire(), of.getDestinatie());
		return true;
	}
	catch (RepoException&) {
		return false;
	}
}
const Oferta& OfferRepository::find(string denumire, string destinatie) {
	auto it = std::find_if(allOffers.begin(), allOffers.end(), [&](const Oferta& s) {
		return s.getDenumire() == denumire && s.getDestinatie() == destinatie;
		});

	if (it != allOffers.end()) {
		return *it;
	}

	throw RepoException("Oferta cu denumirea " + denumire + " si destinatia " + destinatie + " nu exista in lista");
}

void OfferRepository::store(const Oferta& s) {
	if (exists(s)) {
		throw RepoException("Oferta cu denumirea " + s.getDenumire() + " si destinatia " + s.getDestinatie() + " exista deja in lista");
	}
	this->allOffers.push_back(s);
}

void OfferRepository::remove(const Oferta& s) {
	if (!exists(s)) {
		throw RepoException("Oferta cu denumirea " + s.getDenumire() + " si destinatia " + s.getDestinatie() + " nu exista in lista");
	}

	auto it = std::remove(allOffers.begin(), allOffers.end(), s);
	allOffers.erase(it, allOffers.end());
}

void OfferRepository::modify(const Oferta& of) {
	if (!exists(of)) {
		throw RepoException("Oferta cu denumirea " + of.getDenumire() + " si destinatia " + of.getDestinatie() + " nu exista in lista");
	}

	for (Oferta& oferta : allOffers) {
		if (oferta.getDenumire() == of.getDenumire() && oferta.getDestinatie() == of.getDestinatie()) {
			oferta = of;
			break;
		}
	}
}


void OfferRepository::sortBy(std::function<bool(const Oferta&, const Oferta&)> comparator) {
	std::sort(allOffers.begin(), allOffers.end(), comparator);
}


const vector<Oferta>& OfferRepository::getAllOffers() {
	return this->allOffers;
}

const Oferta& OfferRepository::getOffer(const string& denumire, const string& destinatie) {
	auto it = std::find_if(allOffers.begin(), allOffers.end(), [&](const Oferta& oferta) {
		return oferta.getDenumire() == denumire && oferta.getDestinatie() == destinatie;
		});

	if (it != allOffers.end()) {
		return *it;
	}

	throw RepoException("Oferta cu denumirea " + denumire + " si destinatia " + destinatie + " nu exista in lista");
}

void OfferFileRepo::loadFromFile() {
	std::ifstream in(fileName);
	if (!in.is_open()) throw RepoException("Eroare la deschiderea fisierului!!!");
	while (!in.eof()) {
		string denumire, destinatie, tip;
		double pret;
		if (in.eof()) break;
		in >> denumire;
		if (in.eof()) break;
		in >> destinatie;
		if (in.eof()) break;
		in >> tip;
		if (in.eof()) break;
		in >> pret;
		Oferta o{ denumire, destinatie, tip, pret };
		OfferRepository::store(o);
	}
	in.close();
}

void OfferFileRepo::writeToFile() {
	std::ofstream out(fileName);
	if (!out.is_open()) throw RepoException("Eroare la deschiderea fisierului!!!");
	for (auto& o : OfferRepository::getAllOffers()) {
		out << o.getDenumire();
		out << " ";
		out << o.getDestinatie();
		out << " ";
		out << o.getTip();
		out << " ";
		out << o.getPret();
		out << " ";
		out << std::endl;
	}
	out.close();
}

void testAddRepo() {
	OfferRepository testRepo;
	Oferta oferta1{ "Vacanta la mare", "Grecia", "Sejur hotel",2500 };
	testRepo.store(oferta1);
	assert(testRepo.getAllOffers().size() == 1);

	Oferta oferta2{ "Excursie in munte", "Elvetia", "Drumetie", 1800 };
	Oferta oferta3{ "Vacanta la mare", "Grecia", "Croaziera", 3500 };
	try {
		testRepo.store(oferta3);
		assert(false);
	}
	catch (RepoException&) {
		assert(true);
	}


}
void testFindRepo() {
	OfferRepository testRepo;

	Oferta oferta1{ "Vacanta la mare", "Grecia", "Sejur hotel",2500 };
	Oferta oferta2{ "Excursie in munte", "Elvetia", "Drumetie", 1800 };
	Oferta oferta3{ "Croaziera de lux", "Caraibe", "Croaziera", 3500 };
	testRepo.store(oferta1);
	testRepo.store(oferta2);

	assert(testRepo.exists(oferta1));
	assert(!testRepo.exists(oferta3));

	auto foundOffer = testRepo.find("Vacanta la mare", "Grecia");
	assert(foundOffer.getTip() == "Sejur hotel");
	assert(foundOffer.getPret() == 2500);


	try {
		testRepo.find("Relaxare la plaja", "Turcia");
		assert(false);
	}
	catch (RepoException& ve) {
		assert(ve.getErrorMessage() == "Oferta cu denumirea Relaxare la plaja si destinatia Turcia nu exista in lista");
	}
}



void testModifyRepo()
{
	OfferRepository testRepo;

	testRepo.store(Oferta("Croaziera de lux", "Caraibe", "croaziera", 3500));

	testRepo.modify(Oferta("Croaziera de lux", "Caraibe", "croaziera", 4000));
	auto modifiedOffer = testRepo.find("Croaziera de lux", "Caraibe");
	assert(modifiedOffer.getPret() == 4000);

	try {
		testRepo.modify(Oferta("Vacanta la munte", "Elvetia", "drumetie", 2000));
		assert(false);
	}
	catch (RepoException& e) {
		assert(e.getErrorMessage() == "Oferta cu denumirea Vacanta la munte si destinatia Elvetia nu exista in lista");
	}
}



void testRemoveRepo()
{
	OfferRepository testRepo;

	testRepo.store(Oferta("Croaziera de lux", "Caraibe", "croaziera", 3500));
	testRepo.store(Oferta("Excursie la munte", "Elvetia", "drumetie", 1800));
	testRepo.store(Oferta("Distractie la plaja", "Grecia", "Sejur hotel", 2500));
	testRepo.store(Oferta("Relaxare in familie", "Turcia", "Hotel All-Inclusive", 3000));


	testRepo.remove(Oferta("Distractie la plaja", "Grecia", "Sejur hotel", 2500));
	assert(testRepo.getAllOffers().size() == 3);

	try {
		testRepo.remove(Oferta("Vacanta in Alpi", "Elvetia", "drumetie", 2000));
		assert(false);
	}
	catch (RepoException& e) {
		assert(e.getErrorMessage() == "Oferta cu denumirea Vacanta in Alpi si destinatia Elvetia nu exista in lista");
	}

}

void testSortByRepo() {
	OfferRepository testRepo;

	testRepo.store(Oferta("Excursie la munte", "Elvetia", "drumetie", 1800));
	testRepo.store(Oferta("Relaxare in familie", "Turcia", "Hotel All-Inclusive", 3000));
	testRepo.store(Oferta("Croaziera de lux", "Caraibe", "croaziera", 3500));
	testRepo.store(Oferta("Distractie la plaja", "Grecia", "Sejur hotel", 2500));

	auto comparatorByDestination = [](const Oferta& a, const Oferta& b) {
		return a.getDestinatie() < b.getDestinatie();
		};

	testRepo.sortBy(comparatorByDestination);

	const auto& sortedOffers = testRepo.getAllOffers();

	assert(sortedOffers[0].getDestinatie() == "Caraibe");
	assert(sortedOffers[1].getDestinatie() == "Elvetia");
	assert(sortedOffers[2].getDestinatie() == "Grecia");
	assert(sortedOffers[3].getDestinatie() == "Turcia");
}

void testGetOfferRepo() {
	OfferRepository testRepo;

	Oferta oferta1{ "Vacanta la mare", "Grecia", "Sejur hotel", 2500 };
	Oferta oferta2{ "Excursie in munte", "Elvetia", "Drumetie", 1800 };
	Oferta oferta3{ "Croaziera de lux", "Caraibe", "Croaziera", 3500 };
	testRepo.store(oferta1);
	testRepo.store(oferta2);
	testRepo.store(oferta3);

	try {
		auto foundOffer = testRepo.getOffer("Vacanta la mare", "Grecia");
		assert(foundOffer.getTip() == "Sejur hotel");
		assert(foundOffer.getPret() == 2500);

		testRepo.getOffer("Relaxare la plaja", "Turcia");
		assert(false);
	}
	catch (RepoException& e) {
		assert(e.getErrorMessage() == "Oferta cu denumirea Relaxare la plaja si destinatia Turcia nu exista in lista");
	}
}


void testeRepo() {
	testAddRepo();
	testFindRepo();
	testModifyRepo();
	testRemoveRepo();
	testSortByRepo();
	testGetOfferRepo();
}