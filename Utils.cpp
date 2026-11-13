// =====================================================================
// Utils.cpp : implementation des fonctions utilitaires
// =====================================================================
#include "Utils.h"

#include <algorithm>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

string majuscules(string s) {
    for (size_t i = 0; i < s.size(); i++) s[i] = toupper((unsigned char)s[i]);
    return s;
}

string nettoyer(string s) {
    replace(s.begin(), s.end(), ';', ' ');
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

string dateDuJour() {
    time_t t = time(nullptr);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&t));
    return string(buf);
}

string montant(double m) {
    ostringstream os;
    os << fixed << setprecision(0) << m << " FCFA";
    return os.str();
}
