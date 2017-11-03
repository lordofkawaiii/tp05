/*
 * CppUnitSi.cpp
 *
 *  Created on: 2 nov. 2017
 *      Author: lordofkawaiii
 */

#include "CppUnitSi.hpp"
#include "../Interpreteur.h"

#include <cppunit/extensions/AutoRegisterSuite.h>


CPPUNIT_TEST_SUITE_REGISTRATION(CppUnitSi);

CppUnitSi::CppUnitSi()
{
}

CppUnitSi::~CppUnitSi()
{
}

void CppUnitSi::setUp()
{
}

void CppUnitSi::tearDown()
{
}

void CppUnitSi::testSi()
{
  //CPPUNIT_ASSERT_NO_THROW_MESSAGE("testvdsg", Interpreteur("testSi.txt"));
}
