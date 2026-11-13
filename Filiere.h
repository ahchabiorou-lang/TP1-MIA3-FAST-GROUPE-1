// =====================================================================
// Filiere.h : une filiere d'etudes
// =====================================================================
#ifndef FILIERE_H
#define FILIERE_H

#include <string>

class Filiere {
private:
    std::string code;
    std::string nom;

public:
    Filiere(const std::string& code, const std::string& nom) : code(code), nom(nom) {}

    std::string getCode() const { return code; }
    std::string getNom() const { return nom; }
};

#endif
