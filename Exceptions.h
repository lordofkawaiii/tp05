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
	SyntaxeException(char const * message = NULL, unsigned int line = 0,
			unsigned int column = 0) :
			m_message(message) {
		m_line = line;
		m_column = column;
	}
	char const * what() const throw () {
        return m_message;
    }
	unsigned int getLine() {
		return m_line;
	}
	unsigned int getColumn() {
		return m_column;
	}
private :
	char const * m_message;
	unsigned int m_line;
	unsigned int m_column;
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

