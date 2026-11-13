// =====================================================================
// Scolarite.h : classe de gestion de l'application
// Regroupe les etudiants, les inscriptions et les donnees de reference
// (niveaux, filieres, annees academiques)
// =====================================================================
#ifndef SCOLARITE_H
#define SCOLARITE_H

#include <string>
#include <vector>

#include "AnneeAcademique.h"
#include "Etudiant.h"
#include "Filiere.h"
#include "Inscription.h"
#include "Niveau.h"

class Scolarite {
private:
    std::vector<Niveau> niveaux;               // donnees de reference (agregation)
    std::vector<Filiere> filieres;
    std::vector<AnneeAcademique> annees;
    std::vector<Etudiant*> etudiants;          // composition : crees avec new,
    std::vector<Inscription*> inscriptions;    // liberes dans le destructeur
    std::string fichier;                       // fichier de sauvegarde (CSV)
    int prochainId;

    Niveau* chercherNiveau(const std::string& code);
    Filiere* chercherFiliere(const std::string& code);
    AnneeAcademique* chercherAnnee(const std::string& libelle);
    Inscription* chercherInscription(const std::string& matricule,
                                     const std::string& libelleAnnee) const;

public:
    explicit Scolarite(const std::string& fichier);
    ~Scolarite();
    Scolarite(const Scolarite&) = delete;               // evite la double liberation
    Scolarite& operator=(const Scolarite&) = delete;    // de la memoire

    // ----- Donnees de reference -----
    const std::vector<Niveau>& getNiveaux() const { return niveaux; }
    const std::vector<Filiere>& getFilieres() const { return filieres; }
    const std::vector<AnneeAcademique>& getAnnees() const { return annees; }

    // ----- Fonctionnalites du TP -----
    Etudiant* rechercherEtudiant(const std::string& matricule) const;
    bool estInscrit(const std::string& matricule, const std::string& libelleAnnee) const;
    bool enregistrer(const std::string& matricule, const std::string& nom,
                     const std::string& prenom, const std::string& codeFiliere,
                     const std::string& codeNiveau, const std::string& libelleAnnee,
                     double fraisVerses, std::string& message);

    void afficherInscrits() const;
    void afficherHistorique(const std::string& matricule) const;
    void afficherParAnnee(const std::string& libelleAnnee) const;

    // ----- Extensions -----
    void sauvegarder() const;     // ecrit le fichier CSV
    void charger();               // relit le fichier CSV
    bool genererAttestation(const std::string& matricule, const std::string& libelleAnnee,
                            std::string& nomFichier) const;
    bool exporterListeHTML(const std::string& libelleAnnee, std::string& nomFichier) const;
};

#endif
