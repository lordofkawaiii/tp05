#ifndef ARBREABSTRAIT_H
#define ARBREABSTRAIT_H

// Contient toutes les déclarations de classes nécessaires
//  pour représenter l'arbre abstrait

#include <iostream>
#include <vector>

using namespace std;

#include "Symbole.h"
#include "Exceptions.h"

////////////////////////////////////////////////////////////////////////////////
class Noeud
{
// Classe abstraite dont dériveront toutes les classes servant à représenter l'arbre abstrait
// Remarque : la classe ne contient aucun constructeur
public:
  virtual int executer() =0; // Méthode pure (non implémentée) qui rend la classe abstraite
  virtual void ajoute(Noeud* instruction)
  {
    throw OperationInterditeException();
  }
  virtual ~Noeud()
  {
  } // Présence d'un destructeur virtuel conseillée dans les classes abstraites
  virtual void traduitEnCpp(ostream & cout, unsigned int indentation) const = 0;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudSeqInst : public Noeud
{
// Classe pour représenter un noeud "sequence d'instruction"
//  qui a autant de fils que d'instructions dans la séquence
public:
  NoeudSeqInst();   // Construit une séquence d'instruction vide
  ~NoeudSeqInst()
  {
  } // A cause du destructeur virtuel de la classe Noeud
  int executer() override;    // Exécute chaque instruction de la séquence
  void ajoute(Noeud* instruction) override; // Ajoute une instruction à la séquence
  void traduitEnCpp(ostream & cout, unsigned int indentation) const override;
private:
  vector<Noeud *> m_instructions; // pour stocker les instructions de la séquence
};
////////////////////////////////////////////////////////////////////////////////

class NoeudLire : public Noeud
{
// Classe pour représenter un noeud "lire"
//  qui a autant de fils que de variable lu
public:
  NoeudLire();   // Construit une séquence d'instruction vide
  ~NoeudLire()
  {
  } // A cause du destructeur virtuel de la classe Noeud
  int executer() override;    // Exécute chaque instruction de la séquence
  void ajoute(Noeud* variable) override; // Ajoute une instruction à la séquence
  void traduitEnCpp(ostream & cout, unsigned int indentation) const override;
private:
  vector<Noeud *> m_variable; // pour stocker les instructions de la séquence
};
////////////////////////////////////////////////////////////////////////////////

class NoeudEcrire : public Noeud
{
// Classe pour représenter un noeud "ecrire"
//  qui a autant de fils que d'expression a ecrire
public:
  NoeudEcrire();   // Construit une séquence d'instruction vide
  ~NoeudEcrire()
  {
  } // A cause du destructeur virtuel de la classe Noeud

  int executer() override;    // Exécute chaque instruction de la séquence
  void ajoute(Noeud* expression) override; // Ajoute une expression dans la chaine a ecrire
  void traduitEnCpp(ostream & cout, unsigned int indentation) const override;
private:
  vector<Noeud*> m_expression; // pour stocker les expressions ou chaine a ecrire
};

////////////////////////////////////////////////////////////////////////////////
class NoeudAffectation : public Noeud
{
// Classe pour représenter un noeud "affectation"
//  composé de 2 fils : la variable et l'expression qu'on lui affecte
public:
  NoeudAffectation(Noeud* variable, Noeud* expression); // construit une affectation
  ~NoeudAffectation()
  {
  } // A cause du destructeur virtuel de la classe Noeud
  int executer() override; // Exécute (évalue) l'expression et affecte sa valeur à la variable
  void traduitEnCpp(ostream & cout, unsigned int indentation) const override;
private:
  Noeud* m_variable;
  Noeud* m_expression;
};
////////////////////////////////////////////////////////////////////////////////

class NoeudPour : public Noeud
{
// Classe pour représenter un noeud "pour"
//  composé d'une sequence d'instruction 2 affectation et 1 expression
public:
  NoeudPour(Noeud* m_sequence, Noeud* m_expression, Noeud* m_affectationI =
                nullptr,
            Noeud* m_affectationIncrementaton = nullptr); // Construit une séquence d'instruction vide
  ~NoeudPour()
  {
  } // A cause du destructeur virtuel de la classe Noeud
  int executer() override;    // Exécute chaque instruction de la séquence
  void traduitEnCpp(ostream & cout, unsigned int indentation) const override;
private:
  Noeud* m_sequence;
  Noeud* m_expression;
  Noeud* m_affectationI;
  Noeud* m_affectationIncrementaton;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudOperateurBinaire : public Noeud
{
// Classe pour représenter un noeud "opération binaire" composé d'un opérateur
//  et de 2 fils : l'opérande gauche et l'opérande droit
public:
  NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit);
  NoeudOperateurBinaire();
  // Construit une opération binaire : operandeGauche operateur OperandeDroit
  ~NoeudOperateurBinaire()
  {
  } // A cause du destructeur virtuel de la classe Noeud
  int executer() override;            // Exécute (évalue) l'opération binaire)
  void traduitEnCpp(ostream & cout, unsigned int indentation) const override;
private:
  Symbole m_operateur;
  Noeud* m_operandeGauche;
  Noeud* m_operandeDroit;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstSi : public Noeud
{
// Classe pour représenter un noeud "instruction si"
//  et ses 2 fils : la condition du si et la séquence d'instruction associée
public:
  NoeudInstSi(Noeud* condition, Noeud* sequence);
  // Construit une "instruction si" avec sa condition et sa séquence d'instruction
  ~NoeudInstSi()
  {
  } // A cause du destructeur virtuel de la classe Noeud
  int executer() override; // Exécute l'instruction si : si condition vraie on exécute la séquence
  void ajoute(Noeud* instruction) override;
  void traduitEnCpp(ostream & cout, unsigned int indentation) const override;
private:
  vector<Noeud*> m_condition;
  vector<Noeud*> m_sequence;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstTantQue : public Noeud
{
// Classe pour représenter un noeud "instruction tant que"
//  et ses 2 fils : la condition du tant que et la séquence d'instruction associée
public:
  NoeudInstTantQue(Noeud* condition, Noeud* sequence);
  // Construit une "instruction tant que" avec sa condition et sa séquence d'instruction
  ~NoeudInstTantQue()
  {
  } // A cause du destructeur virtuel de la classe Noeud
  int executer() override; // Exécute l'instruction tant que : si condition vraie on exécute la séquence
  void traduitEnCpp(ostream & cout, unsigned int indentation) const override;
private:
  Noeud* m_condition;
  Noeud* m_sequence;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstRepeter : public Noeud
{
// Classe pour représenter un noeud "instruction repeter"
//  et ses 2 fils : la sequence d'instruction à repeter et la limite de repetition
public:
  NoeudInstRepeter(Noeud* limite, Noeud* sequence);
  // Construit une "instruction repeter" avec sa limite et sa séquence d'instruction
  ~NoeudInstRepeter()
  {
  } // A cause du destructeur virtuel de la classe Noeud
  int executer() override; // Exécute l'instruction repeter : si limite non-atteint on exécute la séquence
  void traduitEnCpp(ostream & cout, unsigned int indentation) const override;
private:
  Noeud* m_limite;
  Noeud* m_sequence;
};

#endif /* ARBREABSTRAIT_H */
