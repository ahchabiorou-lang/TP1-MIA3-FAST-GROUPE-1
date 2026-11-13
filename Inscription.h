// =====================================================================
// Inscription.h : relie un etudiant, une filiere, un niveau et une annee
// =====================================================================
#ifndef INSCRIPTION_H
#define INSCRIPTION_H

#include <string>

#include "AnneeAcademique.h"
#include "Etudiant.h"
#include "Filiere.h"
#include "Niveau.h"

class Inscription {
private:
    int id;
    Etudiant* etudiant;
    Filiere* filiere;
    Niveau* niveau;
    AnneeAcademique* annee;
    double fraisVerses;
    std::string date;

public:
    Inscription(int id, Etudiant* e, Filiere* f, Niveau* n, AnneeAcademique* a,
                double fraisVerses, const std::string& date)
        : id(id), etudiant(e), filiere(f), niveau(n), annee(a),
          fraisVerses(fraisVerses), date(date) {}

    int getId() const { return id; }
    Etudiant* getEtudiant() const { return etudiant; }
    Filiere* getFiliere() const { return filiere; }
    Niveau* getNiveau() const { return niveau; }
    AnneeAcademique* getAnnee() const { return annee; }
    double getFraisVerses() const { return fraisVerses; }
    std::string getDate() const { return date; }

    bool estSoldee() const;        // les frais verses couvrent-ils les frais du niveau ?
    double resteAPayer() const;    // frais restant a payer
    void afficher() const;         // affichage sur une ligne
};

#endif
