#ifndef LECTEURPHRASESIMPLE_H
#define LECTEURPHRASESIMPLE_H

#include "LecteurSymbole.h"

class LecteurPhraseSimple {
public:
    LecteurPhraseSimple(string nomFich); // Construit un lecteur de phrase pour vérifier
                                         // la syntaxe du programme dans le fichier nomFich

    void analyse();   // Si le contenu du fichier est conforme à la grammaire,
                      // cette méthode se termine normalement et affiche un message "Syntaxe correcte".
                      // Sinon, le programme sera interrompu (exit).
        	
private:
    LecteurSymbole ls;  // le lecteur de symboles utilisé pour lyser le fichier		

    // implémentation de la grammaire
    void programme();   //   <programme> ::= debut <seqInst> fin <EOF>
    void seqInst();	    //     <seq_ins> ::= <inst> ; { <inst> ; }
    void inst();	    //        <inst> ::= <affectation>
    void affectation();	// <affectation> ::= <variable> = <expression>
    void expression();	//  <expression> ::= <facteur> { <opBinaire> <facteur> }
    void facteur();     //     <facteur> ::= <entier>  |  <variable>  |  - <facteur>  |  ( <expression> )
    void opBinaire();   //  <opBinaire>  ::= + | - | *  | /

    // outils pour se simplifier l'analyse syntaxique
    void testerSymCour (string ch);  // si symbole courant != ch, erreur : on arrete le programme, sinon rien
    void sauterSymCour (string ch);  // si symbole courant == ch, on passe au symbole suivant, sinon erreur : on arrete
    void erreur (string mess);       // affiche les message d'erreur mess et arrete le programme
};

#endif /* LECTEURPHRASESIMPLE_H */
