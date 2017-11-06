#include "SymboleValue.h"

#include <stdlib.h>
#include <iomanip>
#include <string>

#include "Exceptions.h"

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

void SymboleValue::traduitEnCpp(ostream & cout,
		unsigned int indentation) const {
	cout << setw(4 * indentation) << "";
	if (m_categorie == m_categorie::VARIABLE) {
		// Si c'est une variable on ecrit sous la forme int nom_variable;
		cout << "int " << m_chaine << ";" << endl;
	} else {
		cout << getChaine() << endl;
	}
}
