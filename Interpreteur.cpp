#include "Interpreteur.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

Interpreteur::Interpreteur(ifstream & fichier) :
m_lecteur(fichier), m_table(), m_arbre(nullptr) {
}

void Interpreteur::analyse() {
  m_arbre = programme(); // on lance l'analyse de la première règle
}

void Interpreteur::tester(string const & symboleAttendu) const
		throw (SyntaxeException) {
  // Teste si le symbole courant est égal au symboleAttendu... Si non, lève une exception
  static char messageWhat[256];
  if (m_lecteur.getSymbole() != symboleAttendu) {
    sprintf(messageWhat,
            "Ligne %d, Colonne %d - Erreur de syntaxe - Symbole attendu : %s - Symbole trouvé : %s",
            m_lecteur.getLigne(), m_lecteur.getColonne(),
            symboleAttendu.c_str(), m_lecteur.getSymbole().getChaine().c_str());
    throw SyntaxeException(messageWhat);
  }
}

void Interpreteur::testerEtAvancer(string const & symboleAttendu)
		throw (SyntaxeException) {
  // Teste si le symbole courant est égal au symboleAttendu... Si oui, avance, Sinon, lève une exception
  tester(symboleAttendu);
  m_lecteur.avancer();
}

void Interpreteur::erreur(string const & message) const throw (SyntaxeException) {
  // Lève une exception contenant le message et le symbole courant trouvé
  // Utilisé lorsqu'il y a plusieurs symboles attendus possibles...
  static char messageWhat[256];
  sprintf(messageWhat,
          "Ligne %d, Colonne %d - Erreur de syntaxe - %s - Symbole trouvé : %s",
          m_lecteur.getLigne(), m_lecteur.getColonne(), message.c_str(), m_lecteur.getSymbole().getChaine().c_str());
  throw SyntaxeException(messageWhat);
}

Noeud* Interpreteur::programme() {
  // <programme> ::= procedure principale() <seqInst> finproc FIN_FICHIER
  testerEtAvancer("procedure");
  testerEtAvancer("principale");
  testerEtAvancer("(");
  testerEtAvancer(")");
  Noeud* sequence = seqInst();
  testerEtAvancer("finproc");
  tester("<FINDEFICHIER>");
  return sequence;
}

Noeud* Interpreteur::seqInst() {
	// <seqInst> ::= <inst> { <im_instructionsm_instructionsnst> }
  NoeudSeqInst* sequence = new NoeudSeqInst();
  do {
    sequence->ajoute(inst());
	} while (m_lecteur.getSymbole() == "<VARIABLE>"
			|| m_lecteur.getSymbole() == "si"
			|| m_lecteur.getSymbole() == "tantque"
			|| m_lecteur.getSymbole() == "repeter"
			|| m_lecteur.getSymbole() == "pour"
			|| m_lecteur.getSymbole() == "ecrire"
			|| m_lecteur.getSymbole() == "lire");
  // Tant que le symbole courant est un début possible d'instruction...
  // Il faut compléter cette condition chaque fois qu'on rajoute une nouvelle instruction
  return sequence;
}

Noeud* Interpreteur::inst() {
  // <inst> ::= <affectation>  ; | <instSi>
try {
	if (m_lecteur.getSymbole() == "<VARIABLE>") {
		Noeud *affect = affectation();
		testerEtAvancer(";");
		return affect;
	} else if (m_lecteur.getSymbole() == "si") {
		return instSi();
	} else if (m_lecteur.getSymbole() == "tantque") {
		return instTantQue();
	} else if (m_lecteur.getSymbole() == "repeter") {
		return instRepeter();
	} else if (m_lecteur.getSymbole() == "pour") {
		return instPour();
	} else if (m_lecteur.getSymbole() == "ecrire") {
		return instEcrire();
	} else if (m_lecteur.getSymbole() == "lire") {
		return instLire();
	} else {
		erreur("Instruction incorrecte");
			char const * err = "Erreur de syntaxe";
			throw SyntaxeException(err, m_lecteur.getLigne(),
					m_lecteur.getColonne());
	}
}
catch (SyntaxeException & e) {
		Symbole x = m_lecteur.getSymbole();
		Symbole y = m_lecteur.getSymbole();
		while (x.getChaine() == y.getChaine()) {
			m_lecteur.avancer();
			y = m_lecteur.getSymbole();
		}
		throw;
	}

}

Noeud* Interpreteur::affectation() {
  // <affectation> ::= <variable> = <expression>
  tester("<VARIABLE>");
  Noeud* var = m_table.chercheAjoute(m_lecteur.getSymbole()); // La variable est ajoutée à la table eton la mémorise
  m_lecteur.avancer();
  testerEtAvancer("=");
  Noeud* exp = expression();             // On mémorise l'expression trouvée
  return new NoeudAffectation(var, exp); // On renvoie un noeud affectation
}

Noeud* Interpreteur::expression() {
  // <expression> ::= <facteur> { <opBinaire> <facteur> }
  //  <opBinaire> ::= + | - | *  | / | < | > | <= | >= | == | != | et | ou
  Noeud* fact = facteur();
  while ( m_lecteur.getSymbole() == "+"  || m_lecteur.getSymbole() == "-"  ||
          m_lecteur.getSymbole() == "*"  || m_lecteur.getSymbole() == "/"  ||
          m_lecteur.getSymbole() == "<"  || m_lecteur.getSymbole() == "<=" ||
          m_lecteur.getSymbole() == ">"  || m_lecteur.getSymbole() == ">=" ||
          m_lecteur.getSymbole() == "==" || m_lecteur.getSymbole() == "!=" ||
          m_lecteur.getSymbole() == "et" || m_lecteur.getSymbole() == "ou"   ) {
    Symbole operateur = m_lecteur.getSymbole(); // On mémorise le symbole de l'opérateur
    m_lecteur.avancer();
    Noeud* factDroit = facteur(); // On mémorise l'opérande droit
    fact = new NoeudOperateurBinaire(operateur, fact, factDroit); // Et on construuit un noeud opérateur binaire
  }
  return fact; // On renvoie fact qui pointe sur la racine de l'expression
}

Noeud* Interpreteur::facteur() {
  // <facteur> ::= <entier> | <variable> | - <facteur> | non <facteur> | ( <expression> )
  Noeud* fact = nullptr;
  if (m_lecteur.getSymbole() == "<VARIABLE>" || m_lecteur.getSymbole() == "<ENTIER>") {
    fact = m_table.chercheAjoute(m_lecteur.getSymbole()); // on ajoute la variable ou l'entier à la table
    m_lecteur.avancer();
  } else if (m_lecteur.getSymbole() == "-") { // - <facteur>
    m_lecteur.avancer();
    // on représente le moins unaire (- facteur) par une soustraction binaire (0 - facteur)
    fact = new NoeudOperateurBinaire(Symbole("-"), m_table.chercheAjoute(Symbole("0")), facteur());
  } else if (m_lecteur.getSymbole() == "non") { // non <facteur>
    m_lecteur.avancer();
    // on représente le moins unaire (- facteur) par une soustractin binaire (0 - facteur)
    fact = new NoeudOperateurBinaire(Symbole("non"), facteur(), nullptr);
  } else if (m_lecteur.getSymbole() == "(") { // expression parenthésée
    m_lecteur.avancer();
    fact = expression();
    testerEtAvancer(")");
  } else
    erreur("Facteur incorrect");
  return fact;
}

Noeud* Interpreteur::instSi() {
	// <instSi> ::= si ( <expression> ) <seqInst> finsi
	testerEtAvancer("si");
	testerEtAvancer("(");
	NoeudPour* res = new NoeudPour;
	Noeud* condition = expression(); // On mémorise la condition
	res->ajoute(condition);
	testerEtAvancer(")");
	Noeud* sequence = seqInst();     // On mémorise la séquence d'instruction
	res->ajoute(sequence);
	while (m_lecteur.getSymbole() == "sinonsi") {
		testerEtAvancer("sinonsi");
		testerEtAvancer(")");
		Noeud* condition = expression(); // On mémorise la condition
		res->ajoute(condition);
		testerEtAvancer(")");
		Noeud* sequence = seqInst();    // On mémorise la séquence d'instruction
		res->ajoute(sequence);
	}
	if (m_lecteur.getSymbole() == "sinon") {
		testerEtAvancer("sinon");
		Noeud* sinon = seqInst();
		res->ajoute(sinon);
	}
	testerEtAvancer("finsi");
	return res;
}

Noeud* Interpreteur::instTantQue() {
	testerEtAvancer("tantQue");
	testerEtAvancer("(");
	NoeudPour* res = new NoeudPour;
	Noeud* expr = expression();
	res->ajoute(expr);
	testerEtAvancer(")");
	Noeud* seq = seqInst();
	res->ajoute(seq);
	testerEtAvancer("finTantQue");
	return nullptr;
}
Noeud* Interpreteur::instRepeter() {
	testerEtAvancer("repeter");
	NoeudPour* res = new NoeudPour;
	Noeud* seq = seqInst();
	res->ajoute(seq);
	testerEtAvancer("jusqua");
	testerEtAvancer("(");
	Noeud* expr = expression();
	res->ajoute(expr);
	testerEtAvancer(")");
	return res;
}
Noeud* Interpreteur::instPour() {
	testerEtAvancer("pour");
	testerEtAvancer("(");
	NoeudPour* res = new NoeudPour;
	if (m_lecteur.getSymbole() != ";") {
		Noeud* aff1 = affectation();
		res->ajoute(aff1);
	}
	testerEtAvancer(";");
	Noeud* exp = expression();
	res->ajoute(exp);
	testerEtAvancer(";");
	if (m_lecteur.getSymbole() != ")") {
		Noeud* aff2 = affectation();
		res->ajoute(aff2);
	}
	testerEtAvancer(")");
	Noeud* seq = seqInst();
	res->ajoute(seq);
	testerEtAvancer("finpour");
	return res;
}
Noeud* Interpreteur::instEcrire() {
	testerEtAvancer("ecrire");
	testerEtAvancer("(");
	Noeud* p;
	NoeudEcrire *vchaine = new NoeudEcrire;
	// on regarde si l’objet pointé par p est de type SymboleValue et si c’est une chaîne
	if ((typeid(*p) == typeid(SymboleValue)
			&& *((SymboleValue*) p) == "<CHAINE>")) {
		Noeud* chaine1 = m_table.chercheAjoute(m_lecteur.getSymbole());
		vchaine->ajoute(chaine1);
	} else {
		Noeud* exp = expression();
		vchaine->ajoute(exp);
	}
	m_lecteur.avancer();
	while (m_lecteur.getSymbole() != ")") {
		testerEtAvancer(",");
		if ((typeid(*p) == typeid(SymboleValue)
				&& *((SymboleValue*) p) == "<CHAINE>")) {
			Noeud* chaine1 = m_table.chercheAjoute(m_lecteur.getSymbole());
			vchaine->ajoute(chaine1);
		} else {
			Noeud* exp = expression();
			vchaine->ajoute(exp);
		}
	}
	return vchaine;
}
Noeud* Interpreteur::instLire() {
	testerEtAvancer("lire");
	testerEtAvancer("(");
	NoeudLire* vvar = new NoeudLire;
	Noeud* var = m_table.chercheAjoute(m_lecteur.getSymbole());
	vvar->ajoute(var);
	while (m_lecteur.getSymbole() != ")") {
		testerEtAvancer(",");
		Noeud* var = m_table.chercheAjoute(m_lecteur.getSymbole());
		vvar->ajoute(var);
	}
	return vvar;
}


