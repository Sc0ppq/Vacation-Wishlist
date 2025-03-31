#pragma once
#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;
using std::ostream;

class Oferta {
private:
	string denumire{ "" };
	string destinatie{ "" };
	string tip{ "" };
	double pret{ 0.0 };

public:
	Oferta() = delete;
	Oferta(string denumire, string destinatie, string tip, double pret) :denumire{ denumire }, destinatie{ destinatie }, tip{ tip }, pret{ pret } {};
	Oferta(const Oferta& ot) : denumire{ ot.denumire }, destinatie{ ot.destinatie }, tip{ ot.tip }, pret{ ot.pret } {}

	string getDenumire() const;
	string getDestinatie() const;
	string getTip() const;
	double getPret() const;

	void setDenumire(string DenumireNoua);
	void setDestinatie(string DestinatieNoua);
	void setTip(string TipNou);
	void setPret(double PretNou);

	bool operator==(const Oferta& other) const {
		return (other.denumire == this->denumire && other.destinatie == this->destinatie);
	}
	friend std::ostream& operator<<(std::ostream& stream, const Oferta& of);

	string to_string_print() const;

};

void testeDomain();

