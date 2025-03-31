#pragma once
#include "Oferta.h"
#include <vector>
#include <functional>
#include <unordered_map>
#include <random>
using std::vector;


class RepoAbstract {
public:
	RepoAbstract() = default;

	RepoAbstract(const RepoAbstract& o) = delete;

	// adauga oferta
	virtual void store(const Oferta& o) = 0;

	// sterge oferta
	virtual void remove(const Oferta& o) = 0;

	// cauta oferta
	virtual const Oferta& find(string denumire, string destinatie) = 0;

	virtual void modify(const Oferta& of) = 0;

	virtual const vector<Oferta>& getAllOffers() = 0;

	virtual void sortBy(std::function<bool(const Oferta&, const Oferta&)> comparator) = 0;

	//    virtual bool exist(const Oferta &o) = 0;
};

/*
Clasa de exceptii specifice Repo
*/
class RepoException {
private:
	string errorMsg;
public:
	RepoException(string errorMsg) :errorMsg{ errorMsg } {};
	string getErrorMessage() {
		return this->errorMsg;
	}
};

class OfferRepository : public RepoAbstract {
private:
	vector<Oferta> allOffers;
public:

	OfferRepository() = default;
	OfferRepository(const OfferRepository& ot) = delete;

	void store(const Oferta& s) override;

	void modify(const Oferta& of) override;

	const vector<Oferta>& getAllOffers() override;

	const Oferta& find(string denumire, string destinatie) override;

	bool exists(const Oferta& s);

	void remove(const Oferta& s) override;

	void sortBy(std::function<bool(const Oferta&, const Oferta&)> comparator) override;

	const Oferta& getOffer(const string& denumire, const string& destinatie);

};

class OfferFileRepo : public OfferRepository {
private:
	string fileName;

	void loadFromFile();
	void writeToFile();
public:
	explicit OfferFileRepo(string fileName) : OfferRepository(), fileName{ fileName } {
		loadFromFile();
	}

	void store(const Oferta& o) override {
		OfferRepository::store(o);
		writeToFile();
	}

	void remove(const Oferta& o) override {
		OfferRepository::remove(o);
		writeToFile();
	}
};


class RepoMap : public OfferRepository {
private:
	std::unordered_map<int, Oferta> oferte;
	double probabilitate;

	void random() const throw(RepoException);

	bool exists(const Oferta& o);
public:
	RepoMap() {
		std::mt19937 gen(123);
		std::uniform_real_distribution<double> dis(0.0, 1.0);
		probabilitate = dis(gen);
	}

	void setProbabilitate(double p);
	void store(const Oferta& o) override;
	void remove(const Oferta& o) override;
	const Oferta& find(std::string denumire, std::string destinatie) override;
	vector<Oferta>& getAllOffers() override;
};

void testeRepo();

