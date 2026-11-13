// =====================================================================
// Scolarite.cpp : implementation de la classe Scolarite
// =====================================================================
#include "Scolarite.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "Utils.h"

using namespace std;

// ---------------------------------------------------------------------
// Constructeur et destructeur
// ---------------------------------------------------------------------
Scolarite::Scolarite(const string& fichier) : fichier(fichier), prochainId(1) {
    niveaux.push_back(Niveau("L1", 60000));
    niveaux.push_back(Niveau("L2", 65000));
    niveaux.push_back(Niveau("L3", 70000));

    filieres.push_back(Filiere("MI", "Mathematiques-Informatique"));
    filieres.push_back(Filiere("PH", "Physique"));
    filieres.push_back(Filiere("CH", "Chimie"));
    filieres.push_back(Filiere("BI", "Biologie"));
    filieres.push_back(Filiere("EG", "Economie et gestion"));
    filieres.push_back(Filiere("DR", "Droit"));

    annees.push_back(AnneeAcademique("2024-2025"));
    annees.push_back(AnneeAcademique("2025-2026"));
    annees.push_back(AnneeAcademique("2026-2027"));

    charger();   // recharge les inscriptions deja sauvegardees
}

Scolarite::~Scolarite() {
    for (size_t i = 0; i < inscriptions.size(); i++) delete inscriptions[i];
    for (size_t i = 0; i < etudiants.size(); i++) delete etudiants[i];
}

// ---------------------------------------------------------------------
// Recherches internes
// ---------------------------------------------------------------------
Niveau* Scolarite::chercherNiveau(const string& code) {
    for (size_t i = 0; i < niveaux.size(); i++)
        if (niveaux[i].getCode() == code) return &niveaux[i];
    return nullptr;
}

Filiere* Scolarite::chercherFiliere(const string& code) {
    for (size_t i = 0; i < filieres.size(); i++)
        if (filieres[i].getCode() == code) return &filieres[i];
    return nullptr;
}

AnneeAcademique* Scolarite::chercherAnnee(const string& libelle) {
    for (size_t i = 0; i < annees.size(); i++)
        if (annees[i].getLibelle() == libelle) return &annees[i];
    return nullptr;
}

Inscription* Scolarite::chercherInscription(const string& matricule,
                                            const string& libelleAnnee) const {
    for (size_t i = 0; i < inscriptions.size(); i++)
        if (inscriptions[i]->getEtudiant()->getMatricule() == matricule &&
            inscriptions[i]->getAnnee()->getLibelle() == libelleAnnee)
            return inscriptions[i];
    return nullptr;
}

// ---------------------------------------------------------------------
// Fonctionnalites du TP
// ---------------------------------------------------------------------
Etudiant* Scolarite::rechercherEtudiant(const string& matricule) const {
    string m = majuscules(matricule);
    for (size_t i = 0; i < etudiants.size(); i++)
        if (etudiants[i]->getMatricule() == m) return etudiants[i];
    return nullptr;
}

bool Scolarite::estInscrit(const string& matricule, const string& libelleAnnee) const {
    return chercherInscription(majuscules(matricule), libelleAnnee) != nullptr;
}

bool Scolarite::enregistrer(const string& matricule, const string& nom, const string& prenom,
                            const string& codeFiliere, const string& codeNiveau,
                            const string& libelleAnnee, double fraisVerses, string& message) {
    string m = majuscules(nettoyer(matricule));
    Etudiant* e = rechercherEtudiant(m);
    string n = nettoyer(nom), p = nettoyer(prenom);

    // 1. Champs obligatoires
    if (m.empty() || (e == nullptr && (n.empty() || p.empty()))) {
        message = "Matricule, nom et prenom sont obligatoires.";
        return false;
    }
    // 2. Regle cle : une seule inscription par etudiant et par annee
    if (estInscrit(m, libelleAnnee)) {
        message = "Cet etudiant est deja inscrit pour " + libelleAnnee + ".";
        return false;
    }
    // 3. Filiere, niveau et annee doivent exister
    Filiere* f = chercherFiliere(codeFiliere);
    Niveau* nv = chercherNiveau(codeNiveau);
    AnneeAcademique* a = chercherAnnee(libelleAnnee);
    if (!f || !nv || !a) {
        message = "Filiere, niveau ou annee invalide.";
        return false;
    }
    // 4. Creation de l'etudiant (s'il est inconnu) puis de l'inscription
    if (e == nullptr) {
        e = new Etudiant(m, n, p);
        etudiants.push_back(e);
    }
    if (fraisVerses < 0) fraisVerses = 0;
    inscriptions.push_back(new Inscription(prochainId++, e, f, nv, a, fraisVerses, dateDuJour()));
    sauvegarder();
    message = e->getNomComplet() + " est inscrit(e) en " + nv->getCode() + ", " +
              f->getNom() + " (" + libelleAnnee + ").";
    return true;
}

void Scolarite::afficherInscrits() const {
    if (inscriptions.empty()) { cout << "  Aucun etudiant inscrit.\n"; return; }
    for (size_t i = 0; i < inscriptions.size(); i++) inscriptions[i]->afficher();
    cout << "  Total : " << inscriptions.size() << " inscription(s).\n";
}

void Scolarite::afficherHistorique(const string& matricule) const {
    string m = majuscules(matricule);
    int n = 0;
    for (size_t i = 0; i < inscriptions.size(); i++)
        if (inscriptions[i]->getEtudiant()->getMatricule() == m) {
            inscriptions[i]->afficher();
            n++;
        }
    if (n == 0) cout << "  Aucune inscription enregistree.\n";
}

void Scolarite::afficherParAnnee(const string& libelleAnnee) const {
    int total = 0, soldes = 0;
    double encaisse = 0;
    for (size_t i = 0; i < inscriptions.size(); i++) {
        const Inscription* ins = inscriptions[i];
        if (ins->getAnnee()->getLibelle() != libelleAnnee) continue;
        ins->afficher();
        total++;
        if (ins->estSoldee()) soldes++;
        encaisse += min(ins->getFraisVerses(), ins->getNiveau()->getFrais());
    }
    if (total == 0) { cout << "  Aucune inscription pour " << libelleAnnee << ".\n"; return; }
    cout << "  Effectif : " << total << " | Frais soldes : " << soldes
         << " | Frais percus : " << montant(encaisse) << "\n";
}

// ---------------------------------------------------------------------
// Extension 1 : sauvegarde dans un fichier CSV
// ---------------------------------------------------------------------
void Scolarite::sauvegarder() const {
    ofstream out(fichier.c_str());
    if (!out) { cout << "Erreur : impossible d'ecrire " << fichier << "\n"; return; }
    for (size_t i = 0; i < inscriptions.size(); i++) {
        const Inscription* ins = inscriptions[i];
        out << ins->getEtudiant()->getMatricule() << ";" << ins->getEtudiant()->getNom() << ";"
            << ins->getEtudiant()->getPrenom() << ";" << ins->getFiliere()->getCode() << ";"
            << ins->getNiveau()->getCode() << ";" << ins->getAnnee()->getLibelle() << ";"
            << fixed << setprecision(0) << ins->getFraisVerses() << ";" << ins->getDate() << "\n";
    }
}

void Scolarite::charger() {
    ifstream in(fichier.c_str());
    if (!in) return;                      // premier lancement : pas de fichier
    string ligne;
    while (getline(in, ligne)) {
        if (!ligne.empty() && ligne[ligne.size() - 1] == '\r') ligne.erase(ligne.size() - 1);
        stringstream ss(ligne);
        string mat, nom, pre, fil, niv, ann, paye, date;
        getline(ss, mat, ';'); getline(ss, nom, ';'); getline(ss, pre, ';');
        getline(ss, fil, ';'); getline(ss, niv, ';'); getline(ss, ann, ';');
        getline(ss, paye, ';'); getline(ss, date, ';');
        Filiere* f = chercherFiliere(fil);
        Niveau* nv = chercherNiveau(niv);
        AnneeAcademique* a = chercherAnnee(ann);
        if (mat.empty() || !f || !nv || !a) continue;    // ligne invalide ignoree
        Etudiant* e = rechercherEtudiant(mat);
        if (e == nullptr) { e = new Etudiant(mat, nom, pre); etudiants.push_back(e); }
        inscriptions.push_back(new Inscription(prochainId++, e, f, nv, a, atof(paye.c_str()), date));
    }
}

// ---------------------------------------------------------------------
// Extension 2 : attestation d'inscription et liste en HTML
// ---------------------------------------------------------------------
bool Scolarite::genererAttestation(const string& matricule, const string& libelleAnnee,
                                   string& nomFichier) const {
    string m = majuscules(matricule);
    const Inscription* ins = chercherInscription(m, libelleAnnee);
    if (ins == nullptr) return false;
    nomFichier = "attestation_" + m + "_" + libelleAnnee + ".html";
    ofstream out(nomFichier.c_str());
    if (!out) return false;
    out << "<!DOCTYPE html>\n<html lang=\"fr\">\n<head>\n<meta charset=\"utf-8\">\n"
        << "<title>Attestation d'inscription</title>\n"
        << "<style>body{font-family:Arial,sans-serif;max-width:600px;margin:40px auto;padding:20px;border:2px solid #333}"
        << "h1{text-align:center;font-size:22px}p{font-size:16px;line-height:1.6}.pied{margin-top:40px;text-align:right}</style>\n"
        << "</head>\n<body>\n<h1>ATTESTATION D'INSCRIPTION</h1>\n"
        << "<p style=\"text-align:center\">Ann&eacute;e acad&eacute;mique " << libelleAnnee << "</p>\n"
        << "<p>Le service de la scolarit&eacute; atteste que <b>" << ins->getEtudiant()->getNomComplet()
        << "</b>, matricule <b>" << m << "</b>, est r&eacute;guli&egrave;rement inscrit(e) en <b>"
        << ins->getNiveau()->getCode() << "</b>, fili&egrave;re <b>" << ins->getFiliere()->getNom() << "</b>.</p>\n"
        << "<p>Frais de scolarit&eacute; : ";
    if (ins->estSoldee()) out << "sold&eacute;s";
    else out << "vers&eacute;s " << montant(ins->getFraisVerses()) << " sur " << montant(ins->getNiveau()->getFrais());
    out << ".</p>\n<p>Inscription enregistr&eacute;e le " << ins->getDate() << ".</p>\n"
        << "<p class=\"pied\">Le service de la scolarit&eacute;</p>\n</body>\n</html>\n";
    return true;
}

bool Scolarite::exporterListeHTML(const string& libelleAnnee, string& nomFichier) const {
    nomFichier = "liste_inscrits.html";
    ofstream out(nomFichier.c_str());
    if (!out) return false;
    out << "<!DOCTYPE html>\n<html lang=\"fr\">\n<head>\n<meta charset=\"utf-8\">\n"
        << "<title>Liste des inscrits</title>\n"
        << "<style>body{font-family:Arial,sans-serif;margin:20px}table{border-collapse:collapse;width:100%}"
        << "th,td{border:1px solid #999;padding:6px;text-align:left}th{background:#e6f1f6}</style>\n"
        << "</head>\n<body>\n<h1>Liste des &eacute;tudiants inscrits"
        << (libelleAnnee.empty() ? "" : " - " + libelleAnnee) << "</h1>\n"
        << "<table>\n<tr><th>Matricule</th><th>Nom et pr&eacute;nom</th><th>Fili&egrave;re</th>"
        << "<th>Niveau</th><th>Ann&eacute;e</th><th>Frais</th></tr>\n";
    for (size_t i = 0; i < inscriptions.size(); i++) {
        const Inscription* ins = inscriptions[i];
        if (!libelleAnnee.empty() && ins->getAnnee()->getLibelle() != libelleAnnee) continue;
        out << "<tr><td>" << ins->getEtudiant()->getMatricule() << "</td><td>"
            << ins->getEtudiant()->getNomComplet() << "</td><td>" << ins->getFiliere()->getNom()
            << "</td><td>" << ins->getNiveau()->getCode() << "</td><td>" << ins->getAnnee()->getLibelle()
            << "</td><td>" << (ins->estSoldee() ? "sold&eacute;s" : "reste " + montant(ins->resteAPayer()))
            << "</td></tr>\n";
    }
    out << "</table>\n</body>\n</html>\n";
    return true;
}
