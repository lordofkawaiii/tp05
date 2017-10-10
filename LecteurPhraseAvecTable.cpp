#include "LecteurPhraseAvecTable.h"

#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
LecteurPhraseAvecTable::LecteurPhraseAvecTable(string nomFich) :
	ls(nomFich), ts() {
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::analyse() {
	programme();
	cout << "Syntaxe correcte." << endl;
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::programme() {
// <programme> ::= debut <seqInst> fin <EOF>

	sauterSymCour("debut");
	seqInst();
	sauterSymCour("fin");
	testerSymCour("<FINDEFICHIER>");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::seqInst() {
// <seqIns> ::= <inst> ; { <inst> ; }

	do {
		inst();
		sauterSymCour(";");
	} while (ls.getSymCour()=="<VARIABLE>" );
	// tant que le symbole courant est un debut possible d'instruction...
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::inst() {
// <inst> ::= <affectation>

		affectation();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::affectation() {
// <affectation> ::= <variable> = <expression>

	//sauterSymCour("<VARIABLE>");
	testerSymCour("<VARIABLE>");
	ts.chercheAjoute(ls.getSymCour());
	ls.suivant();
	sauterSymCour("=");
	expression();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::expression() {
// <expression> ::= <facteur> { <opBinaire> <facteur> }

	facteur();
	while ( ls.getSymCour()=="+" || ls.getSymCour()=="-" || 
			ls.getSymCour()=="*" || ls.getSymCour()=="/") {
		opBinaire();
		facteur();
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::facteur() {
// <facteur> ::= <entier>  |  <variable>  |  - <facteur>  |  ( <expression> )

	if (ls.getSymCour()=="<VARIABLE>" || ls.getSymCour()=="<ENTIER>") {
		ts.chercheAjoute(ls.getSymCour());
		ls.suivant();
	} else if (ls.getSymCour()=="-") {
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
void LecteurPhraseAvecTable::opBinaire() {
// <opBinaire> ::= + | - | *  | /

	if (ls.getSymCour()=="+" || ls.getSymCour()=="-" || 
		ls.getSymCour()=="*" || ls.getSymCour()=="/" )
		ls.suivant();
	else
		erreur("<opBinaire>");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::testerSymCour(string ch) {
	if (ls.getSymCour() != ch) {
		cout << endl << "-------- Erreur ligne " << ls.getLigne()
				<< " - Colonne " << ls.getColonne() << endl << "   Attendu : "
				<< ch << endl << "   Trouve  : " << ls.getSymCour() << endl
				<< endl;
		exit(0); // plus tard, on levera une exception
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::sauterSymCour(string ch) {
	testerSymCour(ch);
	ls.suivant();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::erreur(string mess) {
	cout << endl << "-------- Erreur ligne " << ls.getLigne() << " - Colonne "
			<< ls.getColonne() << endl << "   Attendu : " << mess << endl
			<< "   Trouve  : " << ls.getSymCour() << endl << endl;
	exit(0); // plus tard, on levera une exception
}
