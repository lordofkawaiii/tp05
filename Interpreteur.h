#ifndef INTERPRETEUR_H
#define INTERPRETEUR_H

#include <iostream>
#include <string>

#include "Lecteur.h"
#include "TableSymboles.h"

class Interpreteur {
public:
	Interpreteur(ifstream & fichier);   // Construit un interpréteur pour interpreter
	                                    //  le programme dans  fichier

	void analyse();                     // Si le contenu du fichier est conforme à la grammaire,
	                                    //   cette méthode se termine normalement et affiche un message "Syntaxe correcte".
                                      //   la table des symboles (ts) et l'arbre abstrait (arbre) auront été construits
	                                    // Sinon, une exception sera levée

	inline TableSymboles const & getTable() const {
		return m_table;
	} // accesseur
	inline Noeud* getArbre () const { return m_arbre; }                    // accesseur
  void traduitEnCPP(ostream & cout,
			unsigned int indentation) const;
private:
    Lecteur        m_lecteur;  // Le lecteur de symboles utilisé pour analyser le fichier
    TableSymboles  m_table;    // La table des symboles valués
    Noeud*         m_arbre;    // L'arbre abstrait

    // Implémentation de la grammaire
    Noeud*  programme();   //   <programme> ::= procedure principale() <seqInst> finproc FIN_FICHIER
    Noeud*  seqInst();	   //     <seqInst> ::= <inst> { <inst> }
    Noeud*  inst();	       //        <inst> ::= <affectation> ; | <instSi>
    Noeud*  affectation(); // <affectation> ::= <variable> = <expression>
    Noeud*  expression();  //  <expression> ::= <facteur> { <opBinaire> <facteur> }
    Noeud*  facteur();     //     <facteur> ::= <entier>  |  <variable>  |  - <facteur>  | non <facteur> | ( <expression> )
                           //   <opBinaire> ::= + | - | *  | / | < | > | <= | >= | == | != | et | ou
    Noeud*  instSi();      //      <instSi> ::= si ( <expression> ) <seqInst> finsi
	Noeud* instTantQue();
	Noeud* instRepeter();
	Noeud* instPour();
	Noeud* instEcrire();
	Noeud* instLire();

    // outils pour simplifier l'analyse syntaxique
  void tester(string const & symboleAttendu) const noexcept(false); // Si symbole courant != symboleAttendu, on lève une exception
  void testerEtAvancer(string const & symboleAttendu) noexcept(false); // Si symbole courant != symboleAttendu, on lève une exception, sinon on avance
  void erreur(string const & mess) const noexcept(false);
	// Lève une exception "contenant" le message mess
};

#endif /* INTERPRETEUR_H */
