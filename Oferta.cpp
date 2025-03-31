#include "Oferta.h"
#include <assert.h>

string Oferta::to_string_print() const {
	return "\tDenumire: " + this->denumire + "\n\tDestinatie: " + this->destinatie + "\n\tTip: " + this->tip + "\n\tPret: " + std::to_string(this->pret);
}


string Oferta::getDenumire() const {
	return this->denumire;
}

string Oferta::getDestinatie() const {
	return this->destinatie;
}

string Oferta::getTip() const {
	return this->tip;
}

double Oferta::getPret() const {
	return this->pret;
}

void Oferta::setDenumire(string DenumireNoua) {
	this->denumire = DenumireNoua;
}

void Oferta::setDestinatie(string DestinatieNoua) {
	this->destinatie = DestinatieNoua;
}

void Oferta::setTip(string TipNou) {
	this->tip = TipNou;
}

void Oferta::setPret(double PretNou) {
	this->pret = PretNou;
}

std::ostream& operator<<(std::ostream& stream, const Oferta& of) {
	stream << "Oferta [Denumire: " << of.denumire << "; Destinatie: " << of.destinatie << "; Tip: " << of.tip << "; Pret: " << of.pret << "]" << endl;


	return stream;
}
void testGetSet() {
	Oferta oferta1{ "Vacanta la plaja", "Grecia","Sejur hotel", 2500 };
	assert(oferta1.getDenumire() == "Vacanta la plaja");
	assert(oferta1.getDestinatie() == "Grecia");
	assert(oferta1.getTip() == "Sejur hotel");
	assert(oferta1.getPret() == 2500);

	Oferta oferta2{ "Croaziera in Caraibe", "Caraibe", "Croaziera",3500 };
	assert(oferta2.getDenumire() == "Croaziera in Caraibe");
	assert(oferta2.getDestinatie() == "Caraibe");
	assert(oferta2.getTip() == "Croaziera");
	assert(oferta2.getPret() == 3500);

	oferta2.setDenumire("Aventura in Alpi");
	oferta2.setDestinatie("Elvetia");
	oferta2.setTip("Drumetie");
	oferta2.setPret(1800);

	assert(oferta2.getDenumire() == "Aventura in Alpi");
	assert(oferta2.getDestinatie() == "Elvetia");
	assert(oferta2.getTip() == "Drumetie");
	assert(oferta2.getPret() == 1800);
}

void testeDomain() {
	testGetSet();
}