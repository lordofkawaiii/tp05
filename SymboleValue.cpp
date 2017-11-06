#include "SymboleValue.h"
#include "Exceptions.h"
#include <stdlib.h>

SymboleValue::SymboleValue(Symbole const & s) :
Symbole(s.getChaine()) {
  if (s == "<ENTIER>") {
    m_valeur = atoi(s.getChaine().c_str()); // c_str convertit une string en char*
    m_defini = true;
  } else {
    m_defini = false;
  }
}

int SymboleValue::executer() {
  if (!m_defini) throw IndefiniException(); // on lève une exception si valeur non définie
  return m_valeur;
}

ostream & operator<<(ostream & cout, SymboleValue const & symbole) {
  cout << (Symbole) symbole << "\t\t - Valeur=";
	if (symbole.m_defini)
		cout << symbole.m_valeur << " ";
  else cout << "indefinie ";
  return cout;
}

void SymboleValue::traduitEnCpp(ostream & cout, unsigned int indentation) const
{
	cout << setw(4 * indentation) << "";
	if (m_categorie == "<VARIABLE>") {
		// Si c'est une variable on ecrit sous la forme int nom_variable = valeur_variable;
		try {
			//On teste si la variable à une valeur
      int res = this->m_valeur;
      cout << "int " << m_chaine << " = " << res << ";"
					<< "\n";
		} catch (IndefiniException & e) {
			// Si elle n'a pas de valeur alors on l'ecrit sous la forme int nom_variable;
			cout << "int " << m_chaine << ";"
					<< endl;
		}
	} else if (m_categorie == "<ENTIER>") {
		cout << m_chaine << endl;
	} else if (m_categorie == "<CHAINE>") {
		cout << m_chaine << endl;
	} else if (m_categorie == "<MOTCLE>") {
		//Si le symbole est un mot clé alors on lance le traduitEnCpp du noeud correspondant au mot cle
		// Je vois juste pas comment faire
	} else if (m_categorie == "<FINDEFICHIER>") {
		//jsp quoi ecrire
	} else {
		//idem
	}
}
