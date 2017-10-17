#ifndef SYMBOLE_H
#define SYMBOLE_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Symbole représente un élément du langage (mot-clé, entier, identificateur de variable, ...)
class Symbole {
public:
	Symbole(string const & s = ""); // Construit le symbole à partir de la chaine (string) ch lue par le lecteur de symbole dans un fichier
	bool operator ==(string const & ch) const; // Pour tester l'égalité entre le symbole et une chaîne
	inline bool operator !=(string const & ch) const {
		return !(*this == ch);
	} // Pour tester la différence...
	inline string const & getChaine() const {
		return m_chaine;
	} // Accesseur
	friend ostream & operator <<(ostream & cout, Symbole const & symbole); // Fonction amie pour pouvoir afficher un symbole sur cout
	enum m_categorie {
		MOTCLE, VARIABLE, ENTIER, CHAINE, INDEFINI, FINDEFICHIER
	};
	m_categorie m_categorie; // Categorie du symbole (voir type énuméré ci-dessus)
	string m_chaine;             // Chaîne du symbole

private:
	bool isMotCle(string const & s) const; // Renvoie vrai si s est un mot clé du langage
	static char const * FICHIER_MOTS_CLES; // Le fichier des mots clés du langage
};

#endif /* SYMBOLE_H */
