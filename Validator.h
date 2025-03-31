#pragma once

#include "Oferta.h"
#include <vector>
#include <string>
using std::string;
using std::vector;
class ValidationException {
	vector<string> errorMsg;
public:
	ValidationException(vector<string> errorMessages) :errorMsg{ errorMessages } {};

	string getErrorMessages() {
		string fullMsg = "";
		for (const string e : errorMsg) {
			fullMsg += e + "\n";
		}
		return fullMsg;
	}
};
/*
Clasa pentru validarea ofertelor
O Oferta este valida daca:
	are denumire format din mai mult de 2 caractere
	are destinatie format din mai mult de 2 caractere
	are tip format din mai mult de 2 caractere
	pretul nu este negativ
*/
class OfferValidator {

public:
	void valideaza(const Oferta& of) {
		vector<string> errors;
		if (of.getDenumire().length() < 2)
			errors.push_back("Denumirea trebuie sa aiba cel putin 2 caractere.");
		if (of.getDestinatie().length() < 2)
			errors.push_back("Destinatia trebuie sa aiba cel putin 2 caractere.");
		if (of.getTip().length() < 2)
			errors.push_back("Tipul trebuie sa aiba cel putin 2 caractere.");
		if (of.getPret() < 0)
			errors.push_back("Pretul trebuie sa fie un numar pozitiv.");
		if (errors.size() > 0)
			throw ValidationException(errors);
	}
};