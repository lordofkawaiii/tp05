#include <stdlib.h>
#include "ArbreAbstrait.h"
#include "Symbole.h"
#include "SymboleValue.h"
#include "Exceptions.h"

////////////////////////////////////////////////////////////////////////////////
// NoeudSeqInst
////////////////////////////////////////////////////////////////////////////////

NoeudSeqInst::NoeudSeqInst() : m_instructions() {
}

int NoeudSeqInst::executer() {
  for (unsigned int i = 0; i < m_instructions.size(); i++)
    m_instructions[i]->executer(); // on exécute chaque instruction de la séquence
  return 0; // La valeur renvoyée ne représente rien !
}

void NoeudSeqInst::ajoute(Noeud* instruction) {
  if (instruction!=nullptr) m_instructions.push_back(instruction);
}
////////////////////////////////////////////////////////////////////////////////
// NoeudLire
////////////////////////////////////////////////////////////////////////////////

NoeudLire::NoeudLire() :
		m_variable() {
}

int NoeudLire::executer() {
	for (unsigned int i = 0; i < m_variable.size(); i++)
		m_variable[i]->executer(); // on exécute chaque instruction de la séquence
	return 0; // La valeur renvoyée ne représente rien !
}

void NoeudLire::ajoute(Noeud* instruction) {
	if (instruction != nullptr)
		m_variable.push_back(instruction);
}
////////////////////////////////////////////////////////////////////////////////
// NoeudEcrire
////////////////////////////////////////////////////////////////////////////////

NoeudEcrire::NoeudEcrire() {
}

int NoeudEcrire::executer() {
	return 0; // La valeur renvoyée ne représente rien !
}

////////////////////////////////////////////////////////////////////////////////
// NoeudPour
////////////////////////////////////////////////////////////////////////////////

NoeudPour::NoeudPour() :
{
}

int NoeudPour::executer() {

	return 0; // La valeur renvoyée ne représente rien !
}

void NoeudPour::traduitEnCpp(ostream & cout, unsigned int indentation) {
	cout << setw(4 * indentation) << "" << "for (";
	m_affectationI->traduitEnCpp(cout, 0);
	cout << " ; ";
	m_expression->traduitEnCpp(cout, 0);
	cout << " ; ";
	m_affectationIncrementaton->traduitEnCpp(cout, 0);
	cout << ") {" << endl;
	m_sequence->traduitEnCpp(cout, indentation + 1);
}
////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////

NoeudAffectation::NoeudAffectation(Noeud* variable, Noeud* expression)
: m_variable(variable), m_expression(expression) {
}

int NoeudAffectation::executer() {
  int valeur = m_expression->executer(); // On exécute (évalue) l'expression
  ((SymboleValue*) m_variable)->setValeur(valeur); // On affecte la variable
  return 0; // La valeur renvoyée ne représente rien !
}

void NoeudAffectation::traduitEnCpp(ostream & cout, unsigned int indentation) {
	cout << setw(4 * indentation) << m_variable->executer() << " = "
			<< m_expression->executer() << endl;
}
////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////

NoeudOperateurBinaire::NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit)
: m_operateur(operateur), m_operandeGauche(operandeGauche), m_operandeDroit(operandeDroit) {
}

int NoeudOperateurBinaire::executer() {
  int og, od, valeur;
  if (m_operandeGauche != nullptr) og = m_operandeGauche->executer(); // On évalue l'opérande gauche
  if (m_operandeDroit != nullptr) od = m_operandeDroit->executer(); // On évalue l'opérande droit
  // Et on combine les deux opérandes en fonctions de l'opérateur
  if (this->m_operateur == "+") valeur = (og + od);
  else if (this->m_operateur == "-") valeur = (og - od);
  else if (this->m_operateur == "*") valeur = (og * od);
  else if (this->m_operateur == "==") valeur = (og == od);
  else if (this->m_operateur == "!=") valeur = (og != od);
  else if (this->m_operateur == "<") valeur = (og < od);
  else if (this->m_operateur == ">") valeur = (og > od);
  else if (this->m_operateur == "<=") valeur = (og <= od);
  else if (this->m_operateur == ">=") valeur = (og >= od);
  else if (this->m_operateur == "et") valeur = (og && od);
  else if (this->m_operateur == "ou") valeur = (og || od);
  else if (this->m_operateur == "non") valeur = (!og);
  else if (this->m_operateur == "/") {
    if (od == 0) throw DivParZeroException();
    valeur = og / od;
  }
  return valeur; // On retourne la valeur calculée
}

void NoeudOperateurBinaire::traduitEnCpp(ostream & cout,
		unsigned int indentation) {
	cout << setw(4 * indentation) << "" << m_operandeGauche->executer() << " "
			<< m_operateur << " " << m_operandeDroit->executer() << endl;
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstSi
////////////////////////////////////////////////////////////////////////////////

NoeudInstSi::NoeudInstSi(Noeud* condition, Noeud* sequence)
: m_condition(condition), m_sequence(sequence) {
}

int NoeudInstSi::executer() {
  if (m_condition->executer()) m_sequence->executer();
  return 0; // La valeur renvoyée ne représente rien !
}

void NoeudInstSi::traduitEnCPP(ostream & cout, unsigned int indentation) const {
	cout << setw(4 * indentation) << "" << "if (";
	// Ecrit "if (" avec un décalage de 4*indentation espaces
	m_condition->traduitEnCPP(cout, 0);
	// Traduit la condition en C++ sans décalage
	cout << ") {" << endl;
	// Ecrit ") {" et passe à la ligne
	m_sequence->traduitEnCPP(cout, indentation + 1);
	// Traduit en C++ la séquence avec indentation augmentée
	cout << setw(4 * indentation) << "" << "}" << endl;
	// Ecrit "}" avec l'indentation initiale et passe à la ligne
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstTantQue
////////////////////////////////////////////////////////////////////////////////

NoeudInstTantQue::NoeudInstTantQue(Noeud* condition, Noeud* sequence) :
		m_condition(condition), m_sequence(sequence) {
}

int NoeudInstTantQue::executer() {
	if (m_condition->executer())
		m_sequence->executer();
	return 0;
}

void NoeudInstTantQue::traduitEnCpp(ostream & cout, unsigned int indentation) {
	cout << setw(4 * indentation) << "" << "while (";
	// Ecrit "while (" avec un decalage de 4*indentation espaces
	m_condition->traduitEnCpp(cout, 0);
	// Traduit la condition en C++ sans decalage
	cout << ") {" << endl;
	// Ecrit ") {" et passe a la ligne
	m_sequence->traduitEnCpp(cout, indentation + 1);
	// Traduit en C++ la sequence avec indentation augmentée
	cout << setw(4 * indentation) << "" << "}" << endl;
	// Ecrit "}" avec l'indentation initiale et passe à la ligne
}
////////////////////////////////////////////////////////////////////////////////
// NoeudInstRepeter
////////////////////////////////////////////////////////////////////////////////

NoeudInstRepeter::NoeudInstRepeter(Noeud* limite, Noeud* sequence) :
		m_limite(limite), m_sequence(sequence) {
}

int NoeudInstRepeter::executer() {

	return 0;
}

void NoeudInstRepeter::traduitEnCpp(ostream & cout, unsigned int indentation) {
	cout << setw(4 * indentation) << "" << "do {" << endl;
	// Ecrit "do {" avec un decalage de 4*indentation espaces
	m_sequence->traduitEnCpp(cout, 0);
	// Traduit la sequence en C++ sans decalage
	cout << "} while (";
	// Ecrit } while (
	m_limite->traduitEnCpp(cout, 0);
	// Ecrit la limite en C++ sans decalage
	cout << ") ;" << endl;
	// Ecrit ) ; et passe a la ligne
}
