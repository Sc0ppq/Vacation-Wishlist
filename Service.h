#pragma once
#include "Repository.h"
#include "Validator.h"
#include "WishlistRepo.h"
#include "Undo.h"
#include <functional>
#include <map>



class Offers {
private:
	//OfferRepository& repo;
	//OfferFileRepo& repo;
	RepoAbstract& repo;
	OfferValidator& val;
	WishlistRepo& wishlist;

	vector<std::unique_ptr<ActiuneUndo>> undoActions;
public:

	static bool compareByDenumire(const Oferta& a, const Oferta& b);
	static bool compareByDestinatie(const Oferta& a, const Oferta& b);
	static bool compareByPretSiTip(const Oferta& a, const Oferta& b);

	Offers(RepoAbstract& offerRepo, OfferValidator& val, WishlistRepo& wishlist) :repo{ offerRepo }, val{ val }, wishlist{ wishlist } {};

	Offers(const Offers& ot) = delete;

	void addOffer(string denumire, string destinatie, string tip, double pret);
	const vector<Oferta>& getAllOffers() {
		return this->repo.getAllOffers();
	}
	void removeOffer(string denumire, string destinatie);
	void modifyOffer(const string& denumire, const string& destinatie, const string& tip, double pret);
	vector<Oferta> filterByDestination(const string& destinatie);
	vector<Oferta> filterByPrice(double minPrice, double maxPrice);
	void sortBy(std::function<bool(const Oferta&, const Oferta&)> comparator);
	void addToMap(std::map<string, int>& Map, const string& destinatie);

	const vector<Oferta>& getAllWishlist() noexcept;
	int getWishlistSize();
	void add_to_wishlist(const string& denumire, const string& destinatie, const vector<Oferta>& filtered);
	void generateWishlist(int nrOferte);
	void exportWishlist(const string& name);
	void emptyWishlist();

	void undo();
};

/*
Clasa de exceptii specifice Service
*/
class ServiceException {
private:
	string errorMsg;
public:
	ServiceException(string errorMsg) :errorMsg{ errorMsg } {};
	string getErrorMessage() {
		return this->errorMsg;
	}
};

void testService();

