/*
 * CppUnitSi.hpp
 *
 *  Created on: 2 nov. 2017
 *      Author: lordofkawaiii
 */

#ifndef TESTS_CPPUNITSI_HPP_
#define TESTS_CPPUNITSI_HPP_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Portability.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestSuite.h>

class CppUnitSi : public CPPUNIT_NS::TestFixture
{
// Utilisation de MACROS pour définir en quoi consiste la suite de tests
CPPUNIT_TEST_SUITE(CppUnitSi); // déclaration d'une suite de tests
  CPPUNIT_TEST(testSi); // ajout d'un test à la suite
  CPPUNIT_TEST_SUITE_END()
  ; // fin de la suite de tests
public:
  CppUnitSi();
  virtual ~CppUnitSi();
  void setUp(); // utilisé pour initialiser des données communes aux tests
  void tearDown(); // utilisé pour "nettoyer" les données communes
private:
  void testSi(); // pour tester le constructeur de la classe Monnaie
};


#endif /* TESTS_CPPUNITSI_HPP_ */
