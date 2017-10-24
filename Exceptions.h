/*
 * File:   Exceptions.h
 * Author: martin
 *
 * Created on 7 décembre 2014, 19:08
 */

#ifndef EXCEPTIONS_H
#define	EXCEPTIONS_H
#include <exception>
#include <string>
using namespace std;

// Classe mère de toutes les exceptions de l'interpréteur
class InterpreteurException : public exception {
public:
	char const * what() const throw () {
        return "Exception Interpreteur";
    }
};

class FichierException : public InterpreteurException {
public:
	char const * what() const throw () {
        return "Ouverture Fichier Impossible";
    }
};

class SyntaxeException : public InterpreteurException {
public:
	SyntaxeException(char const * message = NULL) :
			m_message(message) {
	}
	char const * what() const throw () {
        return m_message;
    }
private :
	char const * m_message;
};


class IndefiniException : public InterpreteurException {
public:
	char const * what() const throw () {
        return "Valeur Indéfinie";
    }
};


class DivParZeroException : public InterpreteurException {
public:
	char const * what() const throw () {
        return "Division par 0";
    }
};

class OperationInterditeException : public InterpreteurException {
public:
	char const * what() const throw () {
        return "Operation Interdite sur un noeud";
    }
};

#endif	/* EXCEPTIONS_H */

