// =====================================================================
// Niveau.h : un niveau d'etudes (L1, L2, L3) et ses frais de scolarite
// =====================================================================
#ifndef NIVEAU_H
#define NIVEAU_H

#include <string>

class Niveau {
private:
    std::string code;   // L1, L2, L3
    double frais;       // frais de scolarite

public:
    Niveau(const std::string& code, double frais) : code(code), frais(frais) {}

    std::string getCode() const { return code; }
    double getFrais() const { return frais; }
};

#endif
