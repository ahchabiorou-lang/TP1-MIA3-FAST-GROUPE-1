// =====================================================================
// AnneeAcademique.h : une annee academique (ex : 2026-2027)
// =====================================================================
#ifndef ANNEEACADEMIQUE_H
#define ANNEEACADEMIQUE_H

#include <string>

class AnneeAcademique {
private:
    std::string libelle;

public:
    explicit AnneeAcademique(const std::string& libelle) : libelle(libelle) {}

    std::string getLibelle() const { return libelle; }
};

#endif
