# Exercice 1 : Gestion d'une bibliothèque universitaire

> Modélisation UML (cas d'utilisation et classes) et implémentation en C++.

## Énoncé en bref

Une bibliothèque universitaire souhaite informatiser la gestion de ses **livres**, de ses **étudiants** et des **emprunts**.

- Un **livre** a un identifiant, un titre, un auteur, une année de publication et un état (disponible ou emprunté).
- Un **étudiant** a un matricule, un nom, un prénom et une adresse électronique.
- Un étudiant peut emprunter **plusieurs** livres, mais un livre n'est emprunté que par **un seul** étudiant à la fois.

## Sommaire

1. [Identification des classes](#1-identification-des-classes)
2. [Attributs et méthodes](#2-attributs-et-méthodes)
3. [Associations entre les classes](#3-associations-entre-les-classes)
4. [Diagramme de cas d'utilisation](#4-diagramme-de-cas-dutilisation)
5. [Diagramme de classes UML](#5-diagramme-de-classes-uml)
6. [Implémentation en C++](#6-implémentation-en-c)
7. [Programme d'emprunt et de retour](#7-programme-demprunt-et-de-retour)
8. [Compilation et exécution](#8-compilation-et-exécution)
9. [Notions mobilisées](#9-notions-mobilisées)

---

## 1. Identification des classes

| Classe | Rôle |
|---|---|
| `Livre` | Un ouvrage du catalogue |
| `Etudiant` | Un usager de la bibliothèque |
| `Emprunt` | Classe d'association : un livre prêté à un étudiant, avec ses dates |
| `Bibliotheque` | Classe de gestion : regroupe les livres, les étudiants et les emprunts |
| `Etat` | Énumération : `DISPONIBLE` ou `EMPRUNTE` |

> **Pourquoi une classe `Emprunt` ?** Un emprunt possède ses propres informations (date d'emprunt, date de retour) et un même étudiant peut emprunter plusieurs fois le même livre. L'emprunt est donc une entité à part entière, ce qui permet aussi de conserver l'**historique**.

## 2. Attributs et méthodes

Tous les attributs sont **privés** (encapsulation) ; l'accès se fait par des accesseurs (*getters*) et des méthodes métier.

### Livre

| Attributs | Méthodes |
|---|---|
| `idLivre : int` | constructeur, accesseurs |
| `titre : string` | `estDisponible() : bool` |
| `auteur : string` | `marquerEmprunte(Emprunt*)` |
| `anneePublication : int` | `marquerDisponible()` |
| `etat : Etat` | `afficher()` |
| `empruntCourant : Emprunt*` (nul si disponible) | |

### Etudiant

| Attributs | Méthodes |
|---|---|
| `matricule : string` | constructeur, accesseurs |
| `nom : string` | `ajouterEmprunt(Emprunt*)` |
| `prenom : string` | `nbEmpruntsEnCours() : int` |
| `email : string` | `afficher()` |
| `emprunts : vector<Emprunt*>` (historique) | |

### Emprunt

| Attributs | Méthodes |
|---|---|
| `idEmprunt : int` (généré automatiquement) | constructeur, accesseurs |
| `livre : Livre*` | `estEnCours() : bool` |
| `etudiant : Etudiant*` | `cloturer()` (enregistre la date de retour) |
| `dateEmprunt : time_t` | `afficher()` |
| `dateRetour : time_t` (0 tant que le livre n'est pas rendu) | |

### Bibliotheque

| Attributs | Méthodes |
|---|---|
| `livres : vector<Livre*>` | `ajouterLivre()`, `ajouterEtudiant()` |
| `etudiants : vector<Etudiant*>` | `chercherLivre()`, `chercherEtudiant()` |
| `emprunts : vector<Emprunt*>` | `emprunterLivre()`, `retournerLivre()` |
| | `afficherCatalogue()`, `afficherHistorique()` |

## 3. Associations entre les classes

| Association | Cardinalités | Type |
|---|---|---|
| `Etudiant` **effectue** `Emprunt` | 1 — 0..\* | Association |
| `Livre` **concerne** `Emprunt` | 1 — 0..\* | Association |
| `Bibliotheque` — `Livre` | 1 — 0..\* | Agrégation |
| `Bibliotheque` — `Etudiant` | 1 — 0..\* | Agrégation |
| `Bibliotheque` — `Emprunt` | 1 — 0..\* | Composition |
| `Livre` — `Etat` | 1 — 1 | Dépendance |

**Contrainte métier :** pour un livre donné, il y a **au plus un emprunt en cours** à la fois (`dateRetour` vide). Elle traduit la phrase de l'énoncé « un livre ne peut être emprunté que par un seul étudiant à la fois ». Un étudiant, lui, peut avoir plusieurs emprunts en cours.

## 4. Diagramme de cas d'utilisation

```mermaid
flowchart LR
    bib["«acteur»<br/>Bibliothécaire"]
    etu["«acteur»<br/>Étudiant"]

    subgraph SYS["Système de gestion de la bibliothèque universitaire"]
        UC1(["Ajouter un livre au catalogue"])
        UC2(["Inscrire un étudiant"])
        UC3(["Enregistrer un emprunt"])
        UC4(["Enregistrer un retour"])
        UC5(["Consulter l'historique des emprunts"])
        UC6(["Consulter le catalogue des livres"])
        UC7(["Consulter ses emprunts en cours"])
        UC8(["Vérifier la disponibilité du livre"])
    end

    bib --- UC1
    bib --- UC2
    bib --- UC3
    bib --- UC4
    bib --- UC5
    etu --- UC6
    etu --- UC7
    UC3 -. "«include»" .-> UC8

    classDef acteur fill:#FFF1CC,stroke:#A15C07,color:#14213D
    class bib,etu acteur
```

**Lecture du diagramme**

- Le **bibliothécaire** gère le catalogue, les étudiants, les emprunts et les retours.
- L'**étudiant** consulte le catalogue et ses propres emprunts.
- *Enregistrer un emprunt* **inclut** (`«include»`) *Vérifier la disponibilité du livre* : cette vérification est obligatoire à chaque emprunt.

## 5. Diagramme de classes UML

```mermaid
classDiagram
    class Etat {
        <<enumeration>>
        DISPONIBLE
        EMPRUNTE
    }

    class Livre {
        -idLivre : int
        -titre : string
        -auteur : string
        -anneePublication : int
        -etat : Etat
        +estDisponible() bool
        +marquerEmprunte(e : Emprunt) void
        +marquerDisponible() void
        +afficher() void
    }

    class Etudiant {
        -matricule : string
        -nom : string
        -prenom : string
        -email : string
        +ajouterEmprunt(e : Emprunt) void
        +nbEmpruntsEnCours() int
        +afficher() void
    }

    class Emprunt {
        -idEmprunt : int
        -dateEmprunt : time_t
        -dateRetour : time_t
        +estEnCours() bool
        +cloturer() void
        +afficher() void
    }

    class Bibliotheque {
        -livres : vector~Livre~
        -etudiants : vector~Etudiant~
        -emprunts : vector~Emprunt~
        +ajouterLivre(l : Livre) void
        +ajouterEtudiant(e : Etudiant) void
        +chercherLivre(idLivre : int) Livre
        +chercherEtudiant(matricule : string) Etudiant
        +emprunterLivre(matricule : string, idLivre : int) bool
        +retournerLivre(idLivre : int) bool
    }

    Etudiant "1" -- "0..*" Emprunt : effectue
    Livre "1" -- "0..*" Emprunt : concerne
    Livre ..> Etat : utilise
    Bibliotheque "1" o-- "0..*" Livre
    Bibliotheque "1" o-- "0..*" Etudiant
    Bibliotheque "1" *-- "0..*" Emprunt

    note for Emprunt "Contrainte : un livre a au plus un emprunt en cours (dateRetour vide)"
```

**Lecture du diagramme**

- `Emprunt` est la classe d'association : elle relie un `Etudiant` et un `Livre`.
- Les cardinalités traduisent l'énoncé : un étudiant a **0..\*** emprunts, un livre apparaît dans **0..\*** emprunts au fil du temps, mais un seul est en cours.
- Losange vide : agrégation. Losange plein : composition (la bibliothèque crée et détruit les emprunts).
- En C++, les références entre classes (`Emprunt`, `Livre`, `Etudiant`) sont des **pointeurs**. Le diagramme les montre par les liens, sans le symbole `*`.

## 6. Implémentation en C++

Le programme tient dans un seul fichier, `main.cpp`. Il n'utilise que la bibliothèque standard : `iostream`, `string`, `vector`, `ctime`.

```cpp
// =====================================================================
// Exercice 1 : Gestion d'une bibliotheque universitaire
// Classes : Livre, Etudiant, Emprunt, Bibliotheque (+ enumeration Etat)
// =====================================================================
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
    Emprunt* empruntCourant;   // nullptr si le livre est disponible
public:
    Livre(int idLivre, const string& titre, const string& auteur, int anneePublication)
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

    void afficher() const;   // defini apres Emprunt
};

// ======================= Etudiant =======================
class Etudiant {
private:
    string matricule;
    string nom;
    string prenom;
    string email;
    vector<Emprunt*> emprunts;   // historique des emprunts de l'etudiant
public:
    Etudiant(const string& matricule, const string& nom,
             const string& prenom, const string& email)
        : matricule(matricule), nom(nom), prenom(prenom), email(email) {}

    string getMatricule() const { return matricule; }
    string getNom() const { return nom; }
    string getPrenom() const { return prenom; }
    string getEmail() const { return email; }

    void ajouterEmprunt(Emprunt* e) { emprunts.push_back(e); }

    int nbEmpruntsEnCours() const;   // defini apres Emprunt
    void afficher() const;           // defini apres Emprunt
};

// ======================= Emprunt =======================
class Emprunt {
private:
    static int compteur;     // genere les identifiants automatiquement
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

// ============ Definitions differees (Emprunt est maintenant complet) ============
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
    Bibliotheque(const Bibliotheque&) = delete;             // evite la double
    Bibliotheque& operator=(const Bibliotheque&) = delete;  // liberation memoire

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
    bib.retournerLivre(3);           // refuse : livre non emprunte
    bib.emprunterLivre("M002", 1);   // OK : le livre est de nouveau disponible

    cout << "\n";
    bib.afficherCatalogue();
    cout << "\n";
    bib.afficherHistorique();
    cout << "\n";
    bib.chercherEtudiant("M001")->afficher();
    return 0;
}
```

**Points importants du code**

- **Encapsulation :** tous les attributs sont privés.
- **Constructeurs :** `Livre` s'initialise `DISPONIBLE` avec `empruntCourant` à `nullptr` ; `Emprunt` génère son identifiant avec un compteur statique.
- **Déclarations anticipées :** `class Livre; class Etudiant; class Emprunt;` permettent aux classes de se référencer avant d'être complètement définies. Les méthodes qui ont besoin d'`Emprunt` sont définies après lui.
- **Gestion de la mémoire :** `Bibliotheque` crée les objets avec `new` et les libère dans son destructeur. La copie est interdite (`= delete`) pour éviter une double libération.

## 7. Programme d'emprunt et de retour

La logique se trouve dans les méthodes de `Bibliotheque`.

**Emprunter un livre** (`emprunterLivre`)

1. Chercher l'étudiant et le livre ; sinon, afficher une erreur.
2. Vérifier `estDisponible()` ; sinon, refuser l'emprunt.
3. Créer un `Emprunt`, l'enregistrer dans la bibliothèque et chez l'étudiant, puis appeler `marquerEmprunte()` sur le livre.

**Retourner un livre** (`retournerLivre`)

1. Chercher le livre ; sinon, afficher une erreur.
2. Vérifier qu'il est bien emprunté ; sinon, afficher une erreur.
3. Récupérer `empruntCourant`, appeler `cloturer()` (date de retour), puis `marquerDisponible()`.

**Exemple d'exécution** (les dates affichées dépendent du jour d'exécution) :

```text
=== Emprunts ===
Aminata Diallo a emprunte "Algorithmique".
Aminata Diallo a emprunte "Le langage C++".
Erreur : "Algorithmique" est deja emprunte.
Erreur : etudiant ou livre introuvable.
--- Catalogue ---
[1] Algorithmique de Cormen (2009) - emprunte par M001
[2] Le langage C++ de Stroustrup (2013) - emprunte par M001
[3] Analyse numerique de Burden (2010) - disponible

=== Retours ===
Aminata Diallo a retourne "Algorithmique".
Erreur : "Analyse numerique" n'est pas emprunte.
Moussa Traore a emprunte "Algorithmique".

--- Catalogue ---
[1] Algorithmique de Cormen (2009) - emprunte par M002
[2] Le langage C++ de Stroustrup (2013) - emprunte par M001
[3] Analyse numerique de Burden (2010) - disponible

--- Historique des emprunts ---
Emprunt n°1 : "Algorithmique" par M001, emprunte le 12/11/2026 - rendu le 12/11/2026
Emprunt n°2 : "Le langage C++" par M001, emprunte le 12/11/2026 - en cours
Emprunt n°3 : "Algorithmique" par M002, emprunte le 12/11/2026 - en cours

Etudiant M001 : Aminata Diallo <a.diallo@univ.edu>
  Emprunt n°1 : "Algorithmique" par M001, emprunte le 12/11/2026 - rendu le 12/11/2026
  Emprunt n°2 : "Le langage C++" par M001, emprunte le 12/11/2026 - en cours
```

Le scénario du `main` montre les cas normaux (plusieurs livres pour un étudiant, retour puis nouvel emprunt du même livre) et les cas d'erreur (livre déjà emprunté, étudiant inconnu, retour d'un livre non emprunté).

## 8. Compilation et exécution

```bash
g++ -std=c++11 main.cpp -o bibliotheque
./bibliotheque        # sous Windows : bibliotheque.exe
```

## 9. Notions mobilisées

| Notion | Où on la retrouve |
|---|---|
| Classes et objets | `Livre`, `Etudiant`, `Emprunt`, `Bibliotheque` instanciés dans `main` |
| Attributs et méthodes | Tableaux de la section 2 |
| Constructeurs | Initialisation de l'état, de `empruntCourant` et de `idEmprunt` |
| Encapsulation | Attributs privés, accès par accesseurs et méthodes métier |
| Associations entre classes | `Etudiant`—`Emprunt`—`Livre`, implémentées par des pointeurs |
| Cardinalités UML | 1 — 0..\* et la contrainte « un seul emprunt en cours par livre » |
| Agrégation et composition | `Bibliotheque` agrège livres et étudiants, et compose les emprunts |

🎓 TP 1 — Application de gestion des inscriptions universitaires
Module : Modélisation UML & Programmation orientée objet (C++) Outils : PlantUML (modélisation) · C++ (implémentation) · Fichiers CSV (persistance)
<h1 align="center">TP 1 - Application de gestion des inscriptions universitaires</h1>
<p align="center">
<img src="https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&logoColor=white" alt="C++20">
<img src="https://img.shields.io/badge/UML-PlantUML-2F6FAD" alt="PlantUML">
<img src="https://img.shields.io/badge/persistance-CSV-6c757d" alt="CSV">
<img src="https://img.shields.io/badge/tests-69%20r%C3%A9ussis-2ea44f" alt="tests">
</p>
<p align="center"><b>Modélisation UML (PlantUML) et implémentation en C++</b></p>

## Sommaire
1. Énoncé
2. Analyse du besoin
3. Modélisation UML
   - 3.1 Diagramme de cas d'utilisation
   - 3.2 Diagramme de classes
   - 3.3 Diagramme de séquence : inscription d'un étudiant
4. Implémentation C++
5. Compilation, exécution et tests
6. Bilan

---

## 1. Énoncé
Au début de chaque année académique, les étudiants doivent effectuer différentes démarches d'inscription : identification, choix de filière, choix de niveau et paiement des frais[span_16](start_span)[span_16](end_span). Un service de scolarité souhaite disposer d'une application pour faciliter le suivi de ces inscriptions[span_17](start_span)[span_17](end_span).

**Fonctionnalités attendues :**
* Enregistrer une inscription académique[span_18](start_span)[span_18](end_span)[span_19](start_span)[span_19](end_span)
* Associer un étudiant à une filière et à un niveau[span_20](start_span)[span_20](end_span)[span_21](start_span)[span_21](end_span)
* Vérifier si un étudiant est déjà inscrit[span_22](start_span)[span_22](end_span)[span_23](start_span)[span_23](end_span)
* Consulter la liste des étudiants inscrits[span_24](start_span)[span_24](end_span)[span_25](start_span)[span_25](end_span)
* Rechercher un étudiant par matricule[span_26](start_span)[span_26](end_span)[span_27](start_span)[span_27](end_span)
* Afficher les inscriptions par année académique[span_28](start_span)[span_28](end_span)[span_29](start_span)[span_29](end_span)

**Modélisation demandée :** diagramme de cas d'utilisation, diagramme de classes, diagramme de séquence de l'inscription[span_30](start_span)[span_30](end_span)[span_31](start_span)[span_31](end_span).
**Classes envisageables :** Etudiant, Inscription, Filiere, Niveau, AnneeAcademique, Scolarite, + Emprunt (consigne complémentaire)[span_32](start_span)[span_32](end_span)[span_33](start_span)[span_33](end_span).
**Extensions :** sauvegarde des données dans des fichiers et génération d'une attestation d'inscription[span_34](start_span)[span_34](end_span)[span_35](start_span)[span_35](end_span).

---

## 2. Analyse du besoin

### 2.1 Acteurs
* **Agent de scolarité (Principal) :** Saisit les étudiants et les inscriptions, consulte les listes, délivre les attestations[span_36](start_span)[span_36](end_span).
* **Chef de scolarité (Principal - spécialise l'agent) :** Peut en plus gérer les filières et annuler une inscription[span_37](start_span)[span_37](end_span).
* **Étudiant (Secondaire) :** Paie ses frais et emprunte des ouvrages ; il ne manipule pas directement l'application[span_38](start_span)[span_38](end_span).

### 2.2 Règles de gestion
* **RG1 :** Chaque étudiant est identifié par un matricule unique généré automatiquement (`ETU2026-0001`)[span_39](start_span)[span_39](end_span)[span_40](start_span)[span_40](end_span).
* **RG2 :** Une inscription associe un étudiant, une filière, un niveau et une année académique[span_41](start_span)[span_41](end_span)[span_42](start_span)[span_42](end_span).
* **RG3 :** Un étudiant ne peut avoir qu'une seule inscription active par année académique (contrôle de doublon)[span_43](start_span)[span_43](end_span).
* **RG4 :** Le niveau choisi doit être ouvert dans la filière (ex. la filière MSI n'ouvre que M1 et M2)[span_44](start_span)[span_44](end_span).
* **RG5 :** Le montant dû correspond aux frais de la filière au jour de l'inscription ; le paiement peut se faire en plusieurs versements, sans jamais dépasser le reste à payer[span_45](start_span)[span_45](end_span).
* **RG6 :** Une inscription est validée quand les frais sont intégralement payés, et annulée sur décision du chef de scolarité[span_46](start_span)[span_46](end_span).
* **RG7 :** L'attestation d'inscription n'est délivrée que pour une inscription validée[span_47](start_span)[span_47](end_span).
* **RG8 :** Seul un étudiant ayant une inscription validée pour l'année en cours peut emprunter un ouvrage[span_48](start_span)[span_48](end_span).
* **RG9 :** Un emprunt dure 14 jours, avec au maximum 3 emprunts en cours par étudiant et dans la limite des exemplaires disponibles[span_49](start_span)[span_49](end_span).

---

## 3. Modélisation UML

### 3.1 Diagramme de cas d'utilisation
<p align="center">
<img src="c827006c-879a-4bee-8232-ceb006a04dc5.jpeg" alt="Diagramme de cas d'utilisation" width="100%">
</p>
<details>
<summary><b>Copier le code PlantUML</b></summary>

```plantuml
@startuml cas_utilisation
left to right direction
skinparam packageStyle rectangle
skinparam shadowing false
skinparam ranksep 20
BackgroundColor #EAF2FB
BorderColor #2F6FAD

actor "Agent de\nscolarité" as Agent
actor "Chef de\nscolarité" as Chef
actor "Étudiant" as Etu
Agent <-- Chef

rectangle "Système de gestion des inscriptions" {
  usecase "Consulter la liste\ndes inscrits" as UC7
  usecase "Inscriptions par année académique" as UC8
  usecase "Enregistrer un étudiant" as UC1
  usecase "Enregistrer une\ninscription académique" as UC2
  usecase "Générer l'attestation\nd'inscription" as UC9
  usecase "Gérer les filières /\nannuler une inscription" as UC11
  usecase "Associer filière\net niveau" as UC3
  usecase "Vérifier si\ndéjà inscrit" as UC4
  usecase "Rechercher\npar matricule" as UC5
  usecase "Payer les frais" as UC6
  usecase "Emprunter\nun ouvrage" as UC10
}

Agent --> UC7
Agent --> UC8
Agent --> UC1
Agent --> UC2
Agent --> UC9
Chef --> UC11

UC2 ..> UC3 : <<include>>
UC2 ..> UC4 : <<include>>
UC2 ..> UC5 : <<include>>
UC9 ..> UC5 : <<include>>
UC2 <.. UC6 : <<extend>>
UC4 <.. UC10 : <<include>>
UC6 <-- Etu
UC10 <-- Etu
@enduml

</details>
3.2 Diagramme de classes
<p align="center">
<img src="d656b423-6448-4e75-ab97-e9136e499dd5.jpeg" alt="Diagramme de classes" width="100%">
</p>
<details>
<summary><b>Copier le code PlantUML</b></summary>

@startuml classes
skinparam classAttributeIconSize 0
skinparam shadowing false
skinparam BackgroundColor #EAF2FB
skinparam BorderColor #2F6FAD
hide empty members

class Scolarite <<contrôleur>> {
  + enregistrerEtudiant(...)
  + rechercherParMatricule(mat): Etudiant
  + inscrire(mat, filiere, niveau, annee): Inscription
  + enregistrerPaiement(id, montant)
  + estDejaInscrit(mat, annee): bool
  + listeInscrits(annee)
  + inscriptionsParAnnee(): Map
  + emprunter(mat, cote, annee): Emprunt
  + retournerOuvrage(idEmprunt)
}

class Etudiant {
  matricule: string
  nom: string
  prenoms: string
  dateNaissance: Date
  email: string
  telephone: string
}

class Inscription {
  id: string
  dateInscription: Date
  montantDu: Montant
  montantPaye: Montant
  + statut(): StatutInscription
  + resteAPayer(): Montant
  + enregistrerPaiement(m)
  + annuler()
}

class Filiere {
  code: string
  libelle: string
  fraisInscription: Montant
  + proposeNiveau(n): bool
}

class AnneeAcademique {
  debut: int
  + libelle(): string
  + {static} courante()
}

enum Niveau {
  L1, L2, L3, M1, M2
}

enum StatutInscription {
  EnAttentePaiement
  Validee
  Annulee
}

class Emprunt {
  id: string
  dateEmprunt: Date
  dateRetourPrevue: Date
  dateRetour: Date [0..1]
  + estEnCours(): bool
  + estEnRetard(d): bool
  + enregistrerRetour(d)
}

class Ouvrage {
  cote: string
  titre: string
  auteur: string
  nombreExemplaires: int
}

Scolarite "1" o-- "*" Etudiant
Scolarite "1" o-- "*" Filiere
Scolarite "1" *-- "*" Inscription
Scolarite "1" *-- "*" Emprunt
Etudiant "1" --> "*" Inscription : effectue >
Inscription "*" --> "1" Filiere : dans >
Inscription "*" --> "1" Niveau : pour >
Inscription "*" --> "1" AnneeAcademique : de >
Inscription ..> StatutInscription
Filiere "*" --> "1..*" Niveau : ouvre >
Etudiant "1" --> "*" Emprunt : réalise (< 3 en cours) >
Emprunt "*" --> "1" Ouvrage : porte sur >
@enduml

</details>
3.3 Diagramme de séquence : inscription d'un étudiant
<p align="center">
<img src="2a9be7a1-b56f-4549-a2f9-bca3b7438d85.jpeg" alt="Diagramme de séquence" width="100%">
</p>
<details>
<summary><b>Copier le code PlantUML</b></summary>


@startuml sequence_inscription
autonumber
skinparam shadowing false
skinparam maxMessageSize 200
skinparam ParticipantBackgroundColor #EAF2FB
skinparam ParticipantBorderColor #2F6FAD
skinparam LifeLineBorderColor #2F6FAD

actor "Agent de\nscolarité" as agent
boundary "Interface" as ui
control "Scolarite" as sc
entity "Filiere" as fil
entity "Inscription" as ins
database "Fichiers\nCSV" as db

agent -> ui : saisit matricule, année, filière, niveau, versement
activate ui
ui -> sc : inscrire(matricule, filière, niveau, année, versement)
activate sc

sc -> sc : rechercherParMatricule(matricule)
sc -> fil : proposeNiveau(niveau)
fil --> sc : vrai / faux
sc -> sc : estDejaInscrit(matricule, année)

alt étudiant inconnu, niveau non ouvert ou déjà inscrit cette année
  sc --> ui : ErreurScolarite
  ui --> agent : affiche l'erreur
else contrôles OK
  sc -> fil : fraisInscription()
  fil --> sc : montantDu
  create ins
  sc -> ins : <<create>>
  opt versement > 0
    sc -> ins : enregistrerPaiement(versement)
  end
  sc --> ui : inscription (statut)
  deactivate sc
  ui -> db : sauvegarder()
  db --> ui : données enregistrées
  ui --> agent : confirme n° d'inscription + statut (Validée / En attente)
end
deactivate ui
@enduml


</details>
4. Implémentation C++

4.1 Architecture
Puce Encapsulation : chaque classe valide ses données dès sa construction (nom obligatoire, date au format JJ/MM/AAAA vérifiée, montants positifs). Un objet invalide ne peut exister.  
Puce Gestion d'erreurs : toute violation d'une règle lève une ErreurScolarite avec un message clair. L'interface l'affiche sans planter.  
Puce C++20 moderne : std::optional pour les recherches, enum class, operator<=> pour comparer les années, std::filesystem.  

4.2 Structure du dépôt

tpi-inscriptions/
├── CMakeLists.txt
├── README.md
├── include/           # En-têtes (.hpp)[span_56](start_span)[span_56](end_span)
├── src/               # Implémentations (.cpp) + main.cpp[span_57](start_span)[span_57](end_span)
├── tests/             # Tests unitaires (69 tests)[span_58](start_span)[span_58](end_span)
├── data/              # Données de démonstration (CSV)[span_59](start_span)[span_59](end_span)
└── docs/uml/          # Sources PlantUML et images[span_60](start_span)[span_60](end_span)


5. Compilation, exécution et tests
Avec CMake :

cmake -S . -B build
cmake --build build
./build/inscriptions
./build/tests          # Lance les 69 tests unitaires[span_61](start_span)[span_61](end_span)[span_62](start_span)[span_62](end_span)

Sans CMake (g++) :

g++ -std=c++20 -Iinclude src/*.cpp -o inscriptions
./inscriptions data


6. Bilan
Puce Les 6 fonctionnalités majeures de l'énoncé sont implémentées, testées et validées (69 tests réussis).  
Puce Les diagrammes UML demandés sont cohérents avec l'architecture du code.  
Puce Les extensions de persistance CSV sécurisée et de génération d'attestation sont fonctionnelles.
