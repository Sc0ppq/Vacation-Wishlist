#pragma once
#include "Oferta.h"
#include "Repository.h"

class ActiuneUndo {
public:
	virtual void doUndo() = 0;
	virtual ~ActiuneUndo() {};
};

class UndoAdauga : public ActiuneUndo {
	Oferta ofertaAdaugata;
	RepoAbstract& Repo;
public:
	UndoAdauga(RepoAbstract& repo, const Oferta& o) : Repo{ repo }, ofertaAdaugata{ o } {}

	void doUndo() override {
		Repo.remove(ofertaAdaugata);
	}
};


class UndoSterge : public ActiuneUndo {
	Oferta ofertaStearsa;
	RepoAbstract& Repo;
public:
	UndoSterge(RepoAbstract& repo, const Oferta& ofertaStearsa) : Repo{ repo }, ofertaStearsa{ ofertaStearsa } {};

	void doUndo() override {
		Repo.store(ofertaStearsa);
	}
};

class UndoModifica : public ActiuneUndo {
	Oferta ofertaModificata;
	RepoAbstract& Repo;
public:
	UndoModifica(RepoAbstract& repo, const Oferta& ofertaModificata) : Repo{ repo }, ofertaModificata{ ofertaModificata } {}

	void doUndo() override {
		Repo.modify(ofertaModificata);
	}
};


