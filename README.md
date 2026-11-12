# TRAVAUX-DE-POO
Résolution d'exercices et travaux dirigés.

EXERCICE 1 : 
Exercice 1 : 
1-) Classe: 
Livre, Etudiant, Emprunt, Bibliotheque

2- ) Attributs et méthodes : 

Livre
* Attributs (privés) : idLivre: int, titre: string, auteur: string, anneePublication: int, etat: Etat (DISPONIBLE / EMPRUNTE), empruntCourant: Emprunt* (nul si disponible)
* Méthodes : constructeur, getters, estDisponible(), marquerEmprunte(), marquerDisponible(), afficher()
Etudiant
* Attributs (privés) : matricule: string, nom: string, prenom: string, email: string, emprunts: vector<Emprunt*>
* Méthodes : constructeur, getters, ajouterEmprunt(), nbEmpruntsEnCours(), afficher()
Emprunt
* Attributs (privés) : idEmprunt: int, livre: Livre*, etudiant: Etudiant*, dateEmprunt: time_t, dateRetour: time_t
* Méthodes : constructeur, getters, estEnCours(), cloturer(), afficher()
Bibliotheque
* Attributs (privés) : livres: vector<Livre*>, etudiants: vector<Etudiant*>, emprunts: vector<Emprunt*>
* Méthodes : ajouterLivre(), ajouterEtudiant(), chercherLivre(), chercherEtudiant(), emprunterLivre(), retournerLivre(), afficherCatalogue(), afficherHistorique()
3. Associations entre les classes

Association	Cardinalités	Type Etudiant effectue Emprunt	1 — 0..*	Association Emprunt concerne Livre	0..* — 1	Association Bibliotheque — Livre	1 — 0..*	Agrégation Bibliotheque — Etudiant	1 — 0..*	Agrégation Bibliotheque — Emprunt	1 — 0..*	Composition
Contrainte métier (à noter sur le diagramme) : pour un livre donné, au plus un emprunt en cours à la fois (dateRetour vide). C'est la traduction de « un livre ne peut être emprunté que par un seul étudiant à la fois ». Un étudiant, lui, peut avoir plusieurs emprunts en cours.


4. Diagramme de cas d'utilisation
                 ┌─────────── Système : Gestion de bibliothèque ──────────┐
                 │                                                        │
                 │   (Ajouter un livre)                                   │
                 │   (Inscrire un étudiant)                               │
 Bibliothécaire ─┼──(Enregistrer un emprunt)──<<include>>──►(Vérifier     │
                 │                                       disponibilité)   │
                 │   (Enregistrer un retour)                              │
                 │   (Consulter l'historique)                             │
                 │                                                        │
   Étudiant ─────┼──(Consulter le catalogue)                              │
                 │   (Consulter ses emprunts)                             │
                 └────────────────────────────────────────────────────────┘
* Bibliothécaire : ajouter un livre, inscrire un étudiant, enregistrer un emprunt, enregistrer un retour, consulter l'historique.
* Étudiant : consulter le catalogue, consulter ses emprunts.
* Enregistrer un emprunt inclut (<<include>>) Vérifier la disponibilité : cette vérification est obligatoire à chaque emprunt.


5. Diagramme de classes UML
┌─────────────────────────┐
│       Bibliotheque      │
├─────────────────────────┤
│ - livres                │
│ - etudiants             │
│ - emprunts              │
├─────────────────────────┤
│ + ajouterLivre()        │
│ + ajouterEtudiant()     │
│ + emprunterLivre()      │
│ + retournerLivre()      │
└──◇───────◇───────◆──────┘
 1│      1 │     1  │
  │ 0..*   │ 0..*   │ 0..*
  │        │        │
Relations de Bibliotheque : ◇ vers Livre (0..*), ◇ vers Etudiant (0..*), ◆ vers Emprunt (0..*).
┌──────────────────────┐ 1      0..* ┌──────────────────────┐
│       Etudiant       │─────────────│       Emprunt        │
├──────────────────────┤  effectue   ├──────────────────────┤
│ - matricule: string  │             │ - idEmprunt: int     │
│ - nom: string        │             │ - dateEmprunt: time_t│
│ - prenom: string     │             │ - dateRetour: time_t │
│ - email: string      │             ├──────────────────────┤
├──────────────────────┤             │ + estEnCours(): bool │
│ + ajouterEmprunt()   │             │ + cloturer(): void   │
│ + nbEmpruntsEnCours()│             │ + afficher(): void   │
│ + afficher(): void   │             └──────────┬───────────┘
└──────────────────────┘                        │ 0..*
                                                │ concerne
                                                │ 1
                                     ┌──────────┴──────────────┐
                                     │          Livre          │
                                     ├─────────────────────────┤
                                     │ - idLivre: int          │
                                     │ - titre: string         │
                                     │ - auteur: string        │
                                     │ - anneePublication: int │
                                     │ - etat: Etat            │
                                     ├─────────────────────────┤
                                     │ + estDisponible(): bool │
                                     │ + marquerEmprunte()     │
                                     │ + marquerDisponible()   │
                                     │ + afficher(): void      │
                                     └─────────────────────────┘
Note : {au plus un Emprunt en cours par Livre}.
Les getters et constructeurs ne sont pas dessinés pour alléger. Les attributs emprunts (Etudiant) et empruntCourant (Livre) viennent des associations : en UML ils sont représentés par les liens, et ils apparaissent dans le code C++.

6. Implémentation C++
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

enum class Etat { DISPONIBLE, EMPRUNTE };

class Livre;
class Etudiant;
class Emprunt;

// ======================= Livre =======================
class Livre {
private:
    int idLivre;
    string titre;
    string auteur;
    int anneePublication;
    Etat etat;
    Emprunt* empruntCourant;   // nullptr si disponible
public:
    Livre(int idLivre, const string& titre, const string& auteur,
          int anneePublication)
        : idLivre(idLivre), titre(titre), auteur(auteur),
          anneePublication(anneePublication),
          etat(Etat::DISPONIBLE), empruntCourant(nullptr) {}

    int getIdLivre() const { return idLivre; }
    string getTitre() const { return titre; }
    string getAuteur() const { return auteur; }
    int getAnneePublication() const { return anneePublication; }
    Etat getEtat() const { return etat; }
    Emprunt* getEmpruntCourant() const { return empruntCourant; }

    bool estDisponible() const { return etat == Etat::DISPONIBLE; }

    void marquerEmprunte(Emprunt* e) {
        etat = Etat::EMPRUNTE;
        empruntCourant = e;
    }
    void marquerDisponible() {
        etat = Etat::DISPONIBLE;
        empruntCourant = nullptr;
    }

    void afficher() const;   // défini après Emprunt
};

// ======================= Etudiant =======================
class Etudiant {
private:
    string matricule;
    string nom;
    string prenom;
    string email;
    vector<Emprunt*> emprunts;   // historique de l'étudiant
public:
    Etudiant(const string& matricule, const string& nom,
             const string& prenom, const string& email)
        : matricule(matricule), nom(nom), prenom(prenom), email(email) {}

    string getMatricule() const { return matricule; }
    string getNom() const { return nom; }
    string getPrenom() const { return prenom; }
    string getEmail() const { return email; }

    void ajouterEmprunt(Emprunt* e) { emprunts.push_back(e); }

    int nbEmpruntsEnCours() const;   // défini après Emprunt
    void afficher() const;           // défini après Emprunt
};

// ======================= Emprunt =======================
class Emprunt {
private:
    static int compteur;     // génère les identifiants automatiquement
    int idEmprunt;
    Livre* livre;
    Etudiant* etudiant;
    time_t dateEmprunt;
    time_t dateRetour;       // 0 tant que le livre n'est pas rendu

    static string formater(time_t t) {
        char buf[20];
        strftime(buf, sizeof(buf), "%d/%m/%Y", localtime(&t));
        return string(buf);
    }
public:
    Emprunt(Livre* livre, Etudiant* etudiant)
        : idEmprunt(++compteur), livre(livre), etudiant(etudiant),
          dateEmprunt(time(nullptr)), dateRetour(0) {}

    int getIdEmprunt() const { return idEmprunt; }
    Livre* getLivre() const { return livre; }
    Etudiant* getEtudiant() const { return etudiant; }
    time_t getDateEmprunt() const { return dateEmprunt; }
    time_t getDateRetour() const { return dateRetour; }

    bool estEnCours() const { return dateRetour == 0; }
    void cloturer() { dateRetour = time(nullptr); }

    void afficher() const {
        cout << "Emprunt n°" << idEmprunt << " : \"" << livre->getTitre()
             << "\" par " << etudiant->getMatricule()
             << ", emprunte le " << formater(dateEmprunt) << " - ";
        if (estEnCours()) cout << "en cours\n";
        else cout << "rendu le " << formater(dateRetour) << "\n";
    }
};
int Emprunt::compteur = 0;

// ============ Définitions différées (Emprunt est complet) ============
void Livre::afficher() const {
    cout << "[" << idLivre << "] " << titre << " de " << auteur
         << " (" << anneePublication << ") - ";
    if (estDisponible()) cout << "disponible\n";
    else cout << "emprunte par "
              << empruntCourant->getEtudiant()->getMatricule() << "\n";
}

int Etudiant::nbEmpruntsEnCours() const {
    int n = 0;
    for (const Emprunt* e : emprunts)
        if (e->estEnCours()) n++;
    return n;
}

void Etudiant::afficher() const {
    cout << "Etudiant " << matricule << " : " << prenom << " " << nom
         << " <" << email << ">\n";
    if (emprunts.empty()) cout << "  Aucun emprunt.\n";
    for (const Emprunt* e : emprunts) {
        cout << "  ";
        e->afficher();
    }
}

// ======================= Bibliotheque =======================
class Bibliotheque {
private:
    vector<Livre*> livres;
    vector<Etudiant*> etudiants;
    vector<Emprunt*> emprunts;
public:
    Bibliotheque() = default;
    Bibliotheque(const Bibliotheque&) = delete;             // évite la double
    Bibliotheque& operator=(const Bibliotheque&) = delete;  // libération mémoire

    ~Bibliotheque() {
        for (auto l : livres) delete l;
        for (auto e : etudiants) delete e;
        for (auto m : emprunts) delete m;
    }

    void ajouterLivre(Livre* l) { livres.push_back(l); }
    void ajouterEtudiant(Etudiant* e) { etudiants.push_back(e); }

    Livre* chercherLivre(int idLivre) const {
        for (auto l : livres)
            if (l->getIdLivre() == idLivre) return l;
        return nullptr;
    }

    Etudiant* chercherEtudiant(const string& matricule) const {
        for (auto e : etudiants)
            if (e->getMatricule() == matricule) return e;
        return nullptr;
    }

    bool emprunterLivre(const string& matricule, int idLivre) {
        Etudiant* e = chercherEtudiant(matricule);
        Livre* l = chercherLivre(idLivre);
        if (e == nullptr || l == nullptr) {
            cout << "Erreur : etudiant ou livre introuvable.\n";
            return false;
        }
        if (!l->estDisponible()) {
            cout << "Erreur : \"" << l->getTitre() << "\" est deja emprunte.\n";
            return false;
        }
        Emprunt* m = new Emprunt(l, e);
        emprunts.push_back(m);
        e->ajouterEmprunt(m);
        l->marquerEmprunte(m);
        cout << e->getPrenom() << " " << e->getNom()
             << " a emprunte \"" << l->getTitre() << "\".\n";
        return true;
    }

    bool retournerLivre(int idLivre) {
        Livre* l = chercherLivre(idLivre);
        if (l == nullptr) {
            cout << "Erreur : livre introuvable.\n";
            return false;
        }
        if (l->estDisponible()) {
            cout << "Erreur : \"" << l->getTitre() << "\" n'est pas emprunte.\n";
            return false;
        }
        Emprunt* m = l->getEmpruntCourant();
        m->cloturer();
        l->marquerDisponible();
        cout << m->getEtudiant()->getPrenom() << " " << m->getEtudiant()->getNom()
             << " a retourne \"" << l->getTitre() << "\".\n";
        return true;
    }

    void afficherCatalogue() const {
        cout << "--- Catalogue ---\n";
        for (auto l : livres) l->afficher();
    }

    void afficherHistorique() const {
        cout << "--- Historique des emprunts ---\n";
        for (auto m : emprunts) m->afficher();
    }
};

// ======================= Programme principal =======================
int main() {
    Bibliotheque bib;

    bib.ajouterLivre(new Livre(1, "Algorithmique", "Cormen", 2009));
    bib.ajouterLivre(new Livre(2, "Le langage C++", "Stroustrup", 2013));
    bib.ajouterLivre(new Livre(3, "Analyse numerique", "Burden", 2010));

    bib.ajouterEtudiant(new Etudiant("M001", "Diallo", "Aminata", "a.diallo@univ.edu"));
    bib.ajouterEtudiant(new Etudiant("M002", "Traore", "Moussa", "m.traore@univ.edu"));

    cout << "=== Emprunts ===\n";
    bib.emprunterLivre("M001", 1);   // OK
    bib.emprunterLivre("M001", 2);   // OK : plusieurs livres pour un etudiant
    bib.emprunterLivre("M002", 1);   // refuse : livre deja emprunte
    bib.emprunterLivre("M009", 3);   // refuse : etudiant inconnu
    bib.afficherCatalogue();

    cout << "\n=== Retours ===\n";
    bib.retournerLivre(1);           // OK
    bib.retournerLivre(3);           // refuse : non emprunte
    bib.emprunterLivre("M002", 1);   // OK : le livre est de nouveau disponible

    cout << "\n";
    bib.afficherCatalogue();
    cout << "\n";
    bib.afficherHistorique();
    cout << "\n";
    bib.chercherEtudiant("M001")->afficher();
    return 0;
}


7. Programme d'emprunt et de retour : principe
Emprunter (emprunterLivre) :
1. Chercher l'étudiant et le livre ; sinon, erreur.
2. Vérifier estDisponible() ; sinon, refus.
3. Créer un Emprunt, l'enregistrer dans la bibliothèque et chez l'étudiant, puis appeler marquerEmprunte() sur le livre.
Retourner (retournerLivre) :
1. Chercher le livre ; sinon, erreur.
2. Vérifier qu'il est bien emprunté ; sinon, erreur.
3. Récupérer empruntCourant, appeler cloturer() (date de retour), puis marquerDisponible().


8. Notions mobilisées
* Classes et objets : 4 classes, instanciées dynamiquement dans main.
* Attributs et méthodes : tous les attributs de l'énoncé sont présents (identifiant, titre, auteur, année, état pour le livre ; matricule, nom, prénom, email pour l'étudiant).
* Constructeurs : initialisent l'état à DISPONIBLE, empruntCourant à nullptr, et Emprunt génère son idEmprunt via un compteur statique.
* Encapsulation : attributs privés, accès par getters et méthodes métier (marquerEmprunte, cloturer).
* Associations et cardinalités : Etudiant 1—0..* Emprunt, Livre 1—0..* Emprunt, avec la contrainte « un seul emprunt en cours par livre » garantie par le test estDisponible().
* Agrégation / composition : la bibliothèque agrège livres et étudiants, et compose les emprunts. Son destructeur libère toute la mémoire, et la copie est interdite (= delete).