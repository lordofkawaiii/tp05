#include "LecteurPhraseSimple.h"

#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
LecteurPhraseSimple::LecteurPhraseSimple(string nomFich) :
	ls(nomFich) {
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::analyse() {
	programme();
	cout << "Syntaxe correcte." << endl;
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::programme() {
// <programme> ::= debut <seqInst> fin FIN_FICHIER
	
	sauterSymCour("debut");
	seqInst();
	sauterSymCour("fin");
	testerSymCour("<FINDEFICHIER>");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::seqInst() {
// <seqInst> ::= <inst> ; { <inst> ; }

	do {
		inst();
		sauterSymCour(";");
	} while (ls.getSymCour()=="<VARIABLE>");
	// tant que le symbole courant est un debut possible d'instruction...
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::inst() {
// <inst> ::= <affectation>

	affectation();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::affectation() {
// <affectation> ::= <variable> = <expression>

	sauterSymCour("<VARIABLE>");
	sauterSymCour("=");
	expression();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::expression() {
// <expression> ::= <facteur> { <opBinaire> <facteur> }

	facteur();
	while (ls.getSymCour()=="+" || ls.getSymCour()=="-" || 
		   ls.getSymCour()=="*" || ls.getSymCour()=="/") {
		opBinaire();
		facteur();
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::facteur() {
// <facteur> ::= <entier>  |  <variable>  |  - <facteur>  |  ( <expression> )

	if (ls.getSymCour()=="<VARIABLE>" || ls.getSymCour()=="<ENTIER>")
		ls.suivant();
	else if (ls.getSymCour()=="-") {
		ls.suivant();
		facteur();
	} else if (ls.getSymCour()=="(") {
		ls.suivant();
		expression();
		sauterSymCour(")");
	} else
		erreur("<facteur>");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::opBinaire()
// <opBinaire> ::= + | - | *  | /
{
	if (ls.getSymCour()=="+" || ls.getSymCour()=="-" || 
		ls.getSymCour()=="*" || ls.getSymCour()=="/")
		ls.suivant();
	else
		erreur("<opBinaire>");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::testerSymCour(string ch) {
	if (ls.getSymCour() != ch) {
		cout << endl << "-------- Erreur ligne " << ls.getLigne()
				<< " - Colonne " << ls.getColonne() << endl << "   Attendu : "
				<< ch << endl << "   Trouve  : " << ls.getSymCour() << endl
				<< endl;
		exit(0); // plus tard, on levera une exception
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::sauterSymCour(string ch) {
	testerSymCour(ch);
	ls.suivant();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::erreur(string mess) {
	cout << endl << "-------- Erreur ligne " << ls.getLigne() << " - Colonne "
			<< ls.getColonne() << endl << "   Attendu : " << mess << endl
			<< "   Trouve  : " << ls.getSymCour() << endl << endl;
	exit(0); // plus tard, on levera une exception
}
