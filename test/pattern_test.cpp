#include "doctest.h"
#include "matrix.hpp"
#include "pattern.hpp"
#include <stdexcept>

TEST_CASE("Pattern - Funzionamento Base")
{
  SUBCASE("Inizializzazione corretta")
  {
    int lato = 5;
    hp::Pattern p(lato);

    CHECK(p.getSize() == 5);
    CHECK(p.getNumNeurons() == 25);

    for (unsigned i = 0; i < p.getNumNeurons(); i++) {
      CHECK(p.getNeuron(i) == 0);
    }
  }

  SUBCASE("Set e Get dei valori")
  {
    hp::Pattern p(3);
    p.setNeuron(0, 1);
    p.setNeuron(8, -1);

    CHECK(p.getNeuron(0) == 1);
    CHECK(p.getNeuron(8) == -1);
    CHECK(p.getNeuron(4) == 0);
  }
}

TEST_CASE("Gestione errori percentuale")
{
  const int size = 2;
  hp::Pattern p(size);

  SUBCASE("Rumore negativo deve lanciare errore")
  {
    CHECK_THROWS_AS(p.addNoise(-0.1f), std::invalid_argument);
  }

  SUBCASE("Rumore superiore al 100% deve lanciare errore")
  {
    CHECK_THROWS_AS(p.addNoise(1.1f), std::invalid_argument);
  }

  SUBCASE("Rumore valido non deve lanciare errore")
  {
    CHECK_NOTHROW(p.addNoise(0.5f));
    CHECK_NOTHROW(p.addNoise(0.0f));
    CHECK_NOTHROW(p.addNoise(1.0f));
  }

  SUBCASE("Rumore 100% inverte tutti i neuroni")
  {
    for (unsigned i = 0; i < p.getNumNeurons(); i++) {
      p.setNeuron(i, 1);
    }

    p.addNoise(1.0f);

    for (unsigned i = 0; i < p.getNumNeurons(); i++) {
      CHECK(p.getNeuron(i) == -1);
    }
  }

  SUBCASE("Rumore 0% non cambia i neuroni")
  {
    for (unsigned i = 0; i < p.getNumNeurons(); i++) {
      p.setNeuron(i, -1);
    }

    p.addNoise(0.0f);

    for (unsigned i = 0; i < p.getNumNeurons(); i++) {
      CHECK(p.getNeuron(i) == -1);
    }
  }
}

TEST_CASE("Funzionamento IsIdentical")
{
  hp::Pattern p1(2);
  hp::Pattern p2(2);

  SUBCASE("Pattern Vuoti sono Identici")
  {
    CHECK(p1.isIdentical(p2) == true);
  }

  SUBCASE("Controllo identicità dopo modifiche")

  {
    p1.setNeuron(0, 1);
    p1.setNeuron(1, -1);

    p2.setNeuron(0, 1);
    p2.setNeuron(1, -1);

    CHECK(p1.isIdentical(p2) == true);
  }
  SUBCASE("Pattern invertiti sono identici")
  {
    p1.setNeuron(0, 1);
    p1.setNeuron(1, -1);

    p2.setNeuron(0, -1);
    p2.setNeuron(1, 1);

    CHECK(p1.isIdentical(p2) == true);
  }

  SUBCASE("Pattern diversi non sono identici")
  {
    p1.setNeuron(0, 1);
    p1.setNeuron(1, 1);

    p2.setNeuron(0, -1);

    CHECK(p1.isIdentical(p2) == false);
  }
}
