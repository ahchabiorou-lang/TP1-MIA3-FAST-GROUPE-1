// =====================================================================
// Etudiant.h : un etudiant, identifie par son matricule
// =====================================================================
#ifndef ETUDIANT_H
#define ETUDIANT_H

#include <string>

class Etudiant {
private:
    std::string matricule;
    std::string nom;
    std::string prenom;

public:
    Etudiant(const std::string& matricule, const std::string& nom, const std::string& prenom)
        : matricule(matricule), nom(nom), prenom(prenom) {}

    std::string getMatricule() const { return matricule; }
    std::string getNom() const { return nom; }
    std::string getPrenom() const { return prenom; }
    std::string getNomComplet() const { return prenom + " " + nom; }
};

#endif
