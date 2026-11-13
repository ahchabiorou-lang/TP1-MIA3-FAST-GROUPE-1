// =====================================================================
// Inscription.cpp : implementation de la classe Inscription
// =====================================================================
#include "Inscription.h"

#include <iostream>

#include "Utils.h"

using namespace std;

bool Inscription::estSoldee() const {
    return fraisVerses >= niveau->getFrais();
}

double Inscription::resteAPayer() const {
    double r = niveau->getFrais() - fraisVerses;
    return r > 0 ? r : 0;
}

void Inscription::afficher() const {
    cout << "  " << etudiant->getMatricule() << " | " << etudiant->getNomComplet()
         << " | " << filiere->getNom() << " | " << niveau->getCode()
         << " | " << annee->getLibelle() << " | ";
    if (estSoldee()) cout << "frais soldes\n";
    else cout << "reste " << montant(resteAPayer()) << "\n";
}
