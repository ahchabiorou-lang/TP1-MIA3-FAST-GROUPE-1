// =====================================================================
// Menu.cpp : saisies au clavier et menu principal
// Chaque option du menu correspond a un cas d'utilisation
// =====================================================================
#include "Menu.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "Utils.h"

using namespace std;

// ---------------------------------------------------------------------
// Saisies au clavier
// ---------------------------------------------------------------------
static string lireTexte(const string& invite) {
    string s;
    cout << invite;
    getline(cin, s);
    return nettoyer(s);
}

static int lireEntier(const string& invite) {
    string s = lireTexte(invite);
    return atoi(s.c_str());
}

static double lireReel(const string& invite) {
    string s = lireTexte(invite);
    return atof(s.c_str());
}

// Les choix se font par numero dans une liste (association filiere / niveau / annee)
static string choisirFiliere(const Scolarite& sc) {
    const vector<Filiere>& v = sc.getFilieres();
    for (size_t i = 0; i < v.size(); i++) cout << "  " << i + 1 << ". " << v[i].getNom() << "\n";
    int c = lireEntier("Filiere (numero) : ");
    return (c >= 1 && c <= (int)v.size()) ? v[c - 1].getCode() : "";
}

static string choisirNiveau(const Scolarite& sc) {
    const vector<Niveau>& v = sc.getNiveaux();
    for (size_t i = 0; i < v.size(); i++)
        cout << "  " << i + 1 << ". " << v[i].getCode() << " (frais : " << montant(v[i].getFrais()) << ")\n";
    int c = lireEntier("Niveau (numero) : ");
    return (c >= 1 && c <= (int)v.size()) ? v[c - 1].getCode() : "";
}

static string choisirAnnee(const Scolarite& sc) {
    const vector<AnneeAcademique>& v = sc.getAnnees();
    for (size_t i = 0; i < v.size(); i++) cout << "  " << i + 1 << ". " << v[i].getLibelle() << "\n";
    int c = lireEntier("Annee academique (numero) : ");
    return (c >= 1 && c <= (int)v.size()) ? v[c - 1].getLibelle() : "";
}

static void afficherMenu() {
    cout << "\n===== SERVICE DE SCOLARITE =====\n"
         << "1. Enregistrer une inscription\n"
         << "2. Verifier si un etudiant est deja inscrit\n"
         << "3. Consulter la liste des etudiants inscrits\n"
         << "4. Rechercher un etudiant par matricule\n"
         << "5. Afficher les inscriptions par annee academique\n"
         << "6. Generer une attestation d'inscription (HTML)\n"
         << "7. Exporter la liste des inscrits (HTML)\n"
         << "0. Quitter\n";
}

// ---------------------------------------------------------------------
// Menu principal
// ---------------------------------------------------------------------
void executerMenu(Scolarite& sc) {
    int choix;
    do {
        afficherMenu();
        choix = lireEntier("Votre choix : ");

        if (choix == 1) {
            string mat = lireTexte("Matricule : ");
            string nom, prenom;
            Etudiant* e = sc.rechercherEtudiant(mat);
            if (e != nullptr) {
                cout << "Etudiant connu : " << e->getNomComplet() << "\n";
            } else {
                nom = lireTexte("Nom : ");
                prenom = lireTexte("Prenom : ");
            }
            string fil = choisirFiliere(sc);
            string niv = choisirNiveau(sc);
            string ann = choisirAnnee(sc);
            double paye = lireReel("Frais verses (FCFA) : ");
            string message;
            bool ok = sc.enregistrer(mat, nom, prenom, fil, niv, ann, paye, message);
            cout << (ok ? "[OK] " : "[ERREUR] ") << message << "\n";
        }
        else if (choix == 2) {
            string mat = lireTexte("Matricule : ");
            string ann = choisirAnnee(sc);
            if (ann.empty()) cout << "Annee invalide.\n";
            else if (sc.estInscrit(mat, ann)) cout << "Deja inscrit pour " << ann << ".\n";
            else cout << "Pas encore inscrit pour " << ann << ".\n";
        }
        else if (choix == 3) {
            cout << "--- Etudiants inscrits ---\n";
            sc.afficherInscrits();
        }
        else if (choix == 4) {
            string mat = lireTexte("Matricule : ");
            Etudiant* e = sc.rechercherEtudiant(mat);
            if (e == nullptr) cout << "Aucun etudiant avec ce matricule.\n";
            else {
                cout << "Etudiant : " << e->getNomComplet() << " (" << e->getMatricule() << ")\n";
                sc.afficherHistorique(mat);
            }
        }
        else if (choix == 5) {
            string ann = choisirAnnee(sc);
            if (ann.empty()) cout << "Annee invalide.\n";
            else { cout << "--- Inscriptions " << ann << " ---\n"; sc.afficherParAnnee(ann); }
        }
        else if (choix == 6) {
            string mat = lireTexte("Matricule : ");
            string ann = choisirAnnee(sc);
            string fichier;
            if (sc.genererAttestation(mat, ann, fichier))
                cout << "Attestation creee : " << fichier << " (ouvrez-la dans un navigateur)\n";
            else cout << "Aucune inscription trouvee pour ce matricule et cette annee.\n";
        }
        else if (choix == 7) {
            string ann = choisirAnnee(sc);          // annee invalide = toutes les annees
            string fichier;
            if (sc.exporterListeHTML(ann, fichier))
                cout << "Liste creee : " << fichier << " (ouvrez-la dans un navigateur)\n";
            else cout << "Erreur lors de la creation du fichier.\n";
        }
        else if (choix != 0) {
            cout << "Choix invalide.\n";
        }
    } while (choix != 0);
}
