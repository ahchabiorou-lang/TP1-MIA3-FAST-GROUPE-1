// =====================================================================
// main.cpp : point d'entree du programme
// TP 1 : Gestion des inscriptions universitaires
// =====================================================================
#include <iostream>

#include "Menu.h"
#include "Scolarite.h"

int main() {
    Scolarite sc("inscriptions.csv");   // charge les inscriptions deja sauvegardees
    executerMenu(sc);                   // menu principal
    sc.sauvegarder();                   // sauvegarde finale
    std::cout << "Donnees sauvegardees. Au revoir.\n";
    return 0;
}
