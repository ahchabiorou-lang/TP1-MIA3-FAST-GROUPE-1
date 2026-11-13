// =====================================================================
// Utils.h : fonctions utilitaires (texte, date, montants)
// =====================================================================
#ifndef UTILS_H
#define UTILS_H

#include <string>

std::string majuscules(std::string s);   // met un texte en majuscules
std::string nettoyer(std::string s);     // enleve ';' et espaces inutiles
std::string dateDuJour();                // date au format AAAA-MM-JJ
std::string montant(double m);           // ex : "70000 FCFA"

#endif
