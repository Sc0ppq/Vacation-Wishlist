#pragma once
#include "Service.h"
#include <assert.h>
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <WishlistRepo.h>

void Offers::addOffer(string denumire, string destinatie, string tip, double pret) {
	Oferta of1{ denumire, destinatie, tip, pret };
	val.valideaza(of1);
	repo.store(of1);
	undoActions.push_back(std::make_unique<UndoAdauga>(repo, of1));
}

void Offers::removeOffer(string denumire, string destinatie)
{
	const Oferta& ofertaGasita = repo.find(denumire, destinatie);
	Oferta copieOferta = ofertaGasita;
	repo.remove(ofertaGasita);
	undoActions.push_back(std::make_unique<UndoSterge>(repo, copieOferta));
}


void Offers::modifyOffer(const string& denumire, const string& destinatie, const string& tip, double pret) {
	Oferta of1{ denumire, destinatie, tip, pret };
	val.valideaza(of1);
	Oferta ofertaOriginala = repo.find(denumire, destinatie);
	repo.modify(of1);
	undoActions.push_back(std::make_unique<UndoModifica>(repo, ofertaOriginala));
}

vector<Oferta> Offers::filterByDestination(const string& destinatie) {
	vector<Oferta> filteredOffers;
	copy_if(repo.getAllOffers().begin(), repo.getAllOffers().end(), back_inserter(filteredOffers),
		[&destinatie](const Oferta& oferta) { return oferta.getDestinatie() == destinatie; });
	return filteredOffers;
}

vector<Oferta> Offers::filterByPrice(double minPrice, double maxPrice) {
	vector<Oferta> filteredOffers;
	copy_if(repo.getAllOffers().begin(), repo.getAllOffers().end(), back_inserter(filteredOffers),
		[&minPrice, &maxPrice](const Oferta& oferta) { return oferta.getPret() >= minPrice && oferta.getPret() <= maxPrice; });
	return filteredOffers;
}


void Offers::sortBy(std::function<bool(const Oferta&, const Oferta&)> comparator) {
	repo.sortBy(comparator);
}

bool Offers::compareByDenumire(const Oferta& a, const Oferta& b) {
	return a.getDenumire() < b.getDenumire();
}

bool Offers::compareByDestinatie(const Oferta& a, const Oferta& b) {
	return a.getDestinatie() < b.getDestinatie();
}

bool Offers::compareByPretSiTip(const Oferta& a, const Oferta& b) {
	if (a.getPret() == b.getPret()) {
		return a.getTip() < b.getTip();
	}
	return a.getPret() < b.getPret();
}

//Undo

void Offers::undo() {
	if (undoActions.empty())
		throw ServiceException("Nu mai exista operatii de Undo!");
	undoActions.back()->doUndo();
	undoActions.pop_back();
}

//Map

std::map<string, int> createMap() {
	return {};
}

void Offers::addToMap(std::map<string, int>& Map, const string& destinatie) {
	auto it = Map.find(destinatie);
	if (it != Map.end())
		it->second++;
	else
		Map[destinatie] = 1;
}




// Wishlist

const vector<Oferta>& Offers::getAllWishlist() noexcept {
	return wishlist.get_Wishlist();
}
void Offers::generateWishlist(int nrOferte) {
	wishlist.generate_wishlist(nrOferte, repo.getAllOffers());
}
void Offers::exportWishlist(const std::string& name) {
	wishlist.export_wishlist(name);
}
void Offers::emptyWishlist() {
	wishlist.empty_wishlist();
}
void Offers::add_to_wishlist(const string& denumire, const string& destinatie, const vector<Oferta>& filtered)
{
	try {
		auto oferta = repo.find(denumire, destinatie);

		wishlist.add_wishlist(oferta);
	}
	catch (RepoException&) {
		if (!filtered.empty()) {

			auto to_add = filtered;
			std::shuffle(to_add.begin(), to_add.end(), std::default_random_engine(time(0)));
			wishlist.add_wishlist(to_add.back());
		}
	}
}

int Offers::getWishlistSize() {
	return wishlist.get_Wishlist().size();
}


/*
void testMap()
{
	std::map<std::string, int> MapTest = createMap();

	addToMap(MapTest, "Grecia");
	addToMap(MapTest, "Turcia");
	addToMap(MapTest, "Grecia");
	addToMap(MapTest, "Germania");

	assert(MapTest["Grecia"] == 2);
	assert(MapTest["Turcia"] == 1);
	assert(MapTest["Germania"] == 1);

	for (const auto& pair : MapTest) {
		std::cout << "Destinatia: " << pair.first << ", Numar oferte: " << pair.second << std::endl;
	}

}
*/
/*
void testAddService() {
	OfferRepository testRepo;
	OfferValidator testVal;
	WishlistRepo testWishlist;
	Offers testService{ testRepo,testVal,testWishlist };

	testService.addOffer("Vacanta", "Grecia", "hotel", 2500);
	testService.addOffer("Vacanta2", "Grecia", "hotel", 2500);
	testService.addOffer("Vacanta3", "Grecia", "hotel", 2500);
	assert(testService.getAllOffers().size() == 3);
	try {
		testService.addOffer("Vacanta", "Grecia", "hotel", 2500);
		assert(false);
	}
	catch (RepoException&) {
		assert(true);
	}

	try {
		testService.addOffer("Excursie de munte", "Elvetia", "drumetie", 2500);
		assert(true);
	}
	catch (ValidationException&) {
		assert(false);
	}

	try {
		testService.addOffer("", "Turcia", "Hotel All-Inclusive", 3000);
		assert(false);
	}
	catch (ValidationException& ve) {
		assert(ve.getErrorMessages() == "Denumirea trebuie sa aiba cel putin 2 caractere.\n");
	}


	try {
		testService.addOffer("Vacanta de relaxare", "", "Hotel All-Inclusive", 2500);
		assert(false);
	}
	catch (ValidationException&) {
		assert(true);
	}

	try {
		testService.addOffer("Vacanta de relaxare", "Turcia", "", 2500);
		assert(false);
	}
	catch (ValidationException&) {
		assert(true);
	}

	try {
		testService.addOffer("Vacanta de relaxare", "Turcia", "Hotel All-Inclusive", -2500);
		assert(false);
	}
	catch (ValidationException&) {
		assert(true);
	}

}

void testRemoveService() {
	OfferRepository testRepo;
	OfferValidator testVal;
	WishlistRepo testWishlist;
	Offers testService{ testRepo,testVal,testWishlist };


	testService.addOffer("Vacanta", "Grecia", "hotel", 2500);
	testService.addOffer("Vacanta2", "Grecia", "hotel", 2500);
	testService.addOffer("Vacanta3", "Grecia", "hotel", 2500);

	assert(testService.getAllOffers().size() == 3);

	testService.removeOffer("Vacanta", "Grecia");

	assert(testService.getAllOffers().size() == 2);
}

void testModifyService() {
	OfferRepository testRepo;
	OfferValidator testVal;
	WishlistRepo testWishlist;
	Offers testService{ testRepo,testVal,testWishlist };


	testService.addOffer("Vacanta", "Grecia", "hotel", 2500);

	testService.modifyOffer("Vacanta", "Grecia", "hotel", 3000);

	auto modifiedOffer = testService.filterByDestination("Grecia")[0];
	assert(modifiedOffer.getPret() == 3000);
}

void testFilterByDestination() {
	OfferRepository testRepo;
	OfferValidator testVal;
	WishlistRepo testWishlist;
	Offers testService{ testRepo,testVal,testWishlist };


	testService.addOffer("Vacanta1", "Grecia", "hotel", 2500);
	testService.addOffer("Vacanta2", "Spania", "hotel", 3000);
	testService.addOffer("Vacanta3", "Grecia", "hotel", 2800);

	auto filteredOffers = testService.filterByDestination("Grecia");

	assert(filteredOffers.size() == 2);
}

void testFilterByPrice() {
	OfferRepository testRepo;
	OfferValidator testVal;
	WishlistRepo testWishlist;
	Offers testService{ testRepo,testVal,testWishlist };


	testService.addOffer("Vacanta1", "Grecia", "hotel", 2500);
	testService.addOffer("Vacanta2", "Spania", "hotel", 3000);
	testService.addOffer("Vacanta3", "Grecia", "hotel", 2800);

	auto filteredOffers = testService.filterByPrice(2500, 2800);

	assert(filteredOffers.size() == 2);
}

void testSortByService() {
	OfferRepository testRepo;
	OfferValidator testVal;
	WishlistRepo testWishlist;
	Offers testService{ testRepo,testVal,testWishlist };


	testService.addOffer("Vacanta1", "Grecia", "hotel", 2500);
	testService.addOffer("Vacanta2", "Spania", "apartament", 3000);
	testService.addOffer("Vacanta3", "Grecia", "pensiune", 2800);

	testService.sortBy(Offers::compareByDenumire);
	auto sortedOffersByDenumireAsc = testService.getAllOffers();

	assert(sortedOffersByDenumireAsc[0].getDenumire() == "Vacanta1");
	assert(sortedOffersByDenumireAsc[1].getDenumire() == "Vacanta2");
	assert(sortedOffersByDenumireAsc[2].getDenumire() == "Vacanta3");

	testService.sortBy([](const Oferta& a, const Oferta& b) { return b.getDenumire() < a.getDenumire(); });
	auto sortedOffersByDenumireDesc = testService.getAllOffers();

	assert(sortedOffersByDenumireDesc[0].getDenumire() == "Vacanta3");
	assert(sortedOffersByDenumireDesc[1].getDenumire() == "Vacanta2");
	assert(sortedOffersByDenumireDesc[2].getDenumire() == "Vacanta1");

	testService.sortBy(Offers::compareByDestinatie);
	auto sortedOffersByDestinatieAsc = testService.getAllOffers();

	assert(sortedOffersByDestinatieAsc[0].getDestinatie() == "Grecia");
	assert(sortedOffersByDestinatieAsc[1].getDestinatie() == "Grecia");
	assert(sortedOffersByDestinatieAsc[2].getDestinatie() == "Spania");

	testService.sortBy([](const Oferta& a, const Oferta& b) { return b.getDestinatie() < a.getDestinatie(); });
	auto sortedOffersByDestinatieDesc = testService.getAllOffers();

	assert(sortedOffersByDestinatieDesc[0].getDestinatie() == "Spania");
	assert(sortedOffersByDestinatieDesc[1].getDestinatie() == "Grecia");
	assert(sortedOffersByDestinatieDesc[2].getDestinatie() == "Grecia");

	testService.sortBy(Offers::compareByPretSiTip);
	auto sortedOffersByPretSiTipAsc = testService.getAllOffers();

	assert(sortedOffersByPretSiTipAsc[0].getPret() == 2500);
	assert(sortedOffersByPretSiTipAsc[1].getPret() == 2800);
	assert(sortedOffersByPretSiTipAsc[2].getPret() == 3000);

	testService.sortBy([](const Oferta& a, const Oferta& b) {
		if (a.getPret() == b.getPret()) {
			return b.getTip() < a.getTip();
		}
		return b.getPret() < a.getPret();
		});
	auto sortedOffersByPretSiTipDesc = testService.getAllOffers();

	assert(sortedOffersByPretSiTipDesc[0].getPret() == 3000);
	assert(sortedOffersByPretSiTipDesc[1].getPret() == 2800);
	assert(sortedOffersByPretSiTipDesc[2].getPret() == 2500);
}

void testUndo()
{
	OfferRepository testRepo;
	OfferValidator testVal;
	WishlistRepo testWishlist;
	Offers testService{ testRepo,testVal,testWishlist };

	try {
		testService.undo();
	}
	catch (ServiceException& e) {
		assert(e.getErrorMessage() == "Nu mai exista operatii de Undo!");
	}

	testService.addOffer("Vacanta1", "Grecia", "hotel", 2500);
	testService.addOffer("Vacanta2", "Spania", "apartament", 3000);
	testService.addOffer("Vacanta3", "Grecia", "pensiune", 2800);

	assert(testService.getAllOffers().size() == 3);
	testService.undo();
	assert(testService.getAllOffers().size() == 2);

	testService.removeOffer("Vacanta2", "Spania");
	assert(testService.getAllOffers().size() == 1);
	testService.undo();
	assert(testService.getAllOffers().size() == 2);

	testService.modifyOffer("Vacanta1", "Grecia", "croaziera", 1800);
	assert(testService.getAllOffers().size() == 2);
	auto it = testRepo.find("Vacanta1", "Grecia");
	assert(it.getTip() == "croaziera");
	assert(it.getPret() == 1800);
	testService.undo();
	assert(testService.getAllOffers().size() == 2);
	auto it2 = testRepo.find("Vacanta1", "Grecia");
	assert(it2.getTip() == "hotel");
	assert(it2.getPret() == 2500);


}
void testAddToWishlistService() {
	OfferRepository testRepo;
	OfferValidator testVal;
	WishlistRepo testWishlist;
	Offers testService{ testRepo, testVal, testWishlist };

	testService.addOffer("Vacanta1", "Grecia", "hotel", 2500);
	testService.addOffer("Vacanta2", "Spania", "hotel", 3000);
	testService.addOffer("Vacanta3", "Grecia", "hotel", 2800);

	assert(testService.getWishlistSize() == 0);

	auto filtered = testService.filterByDestination("Grecia");

	testService.add_to_wishlist("Vacanta1", "Grecia", filtered);
	assert(testService.getWishlistSize() == 1);

	testService.add_to_wishlist("a", "Grecia", filtered);
	assert(testService.getWishlistSize() == 2);

	testService.exportWishlist("test2");
}


void testGenerateWishlistService() {
	OfferRepository testRepo;
	OfferValidator testVal;
	WishlistRepo testWishlist;
	Offers testService{ testRepo,testVal,testWishlist };

	testService.addOffer("Vacanta1", "Grecia", "hotel", 2500);
	testService.addOffer("Vacanta2", "Spania", "hotel", 3000);
	testService.addOffer("Vacanta3", "Grecia", "hotel", 2800);

	testService.generateWishlist(2);
	assert(testService.getWishlistSize() == 2);
}

void testEmptyWishlistService() {
	OfferRepository testRepo;
	OfferValidator testVal;
	WishlistRepo testWishlist;
	Offers testService{ testRepo,testVal,testWishlist };

	testService.addOffer("Vacanta1", "Grecia", "hotel", 2500);
	testService.addOffer("Vacanta2", "Spania", "hotel", 3000);
	testService.addOffer("Vacanta3", "Grecia", "hotel", 2800);

	testService.generateWishlist(2);
	assert(testService.getWishlistSize() == 2);

	testService.emptyWishlist();
	assert(testService.getWishlistSize() == 0);
}

void testService() {
	testUndo();
	testAddService();
	testRemoveService();
	testModifyService();
	testFilterByDestination();
	testFilterByPrice();
	testSortByService();
	testAddToWishlistService();
	testGenerateWishlistService();
	testEmptyWishlistService();
	testMap();
}
*/
