#include "doctest.h"
#include "matrix.hpp"
#include "pattern.hpp"
#include <stdexcept>
TEST_CASE("Pattern - Funzionamento Base")
{
  SUBCASE("Inizializzazione corretta")
  {
    int lato = 5;
    Pattern p(lato);

    // Verifica che le dimensioni siano calcolate giuste (5*5 = 25)
    CHECK(p.getSize() == 5);
    CHECK(p.getNumNeurons() == 25);

    // Verifica che il pattern nasca 'pulito' (tutti zeri)
    for (unsigned i = 0; i < p.getNumNeurons(); i++) {
      CHECK(p.getNeuron(i) == 0);
    }
  }

  SUBCASE("Set e Get dei valori")
  {
    Pattern p(3);       // 9 neuroni
    p.setNeuron(0, 1);  // Scrivo 1 all'inizio
    p.setNeuron(8, -1); // Scrivo -1 alla fine

    CHECK(p.getNeuron(0) == 1);
    CHECK(p.getNeuron(8) == -1);
    CHECK(p.getNeuron(4) == 0); // Quelli non toccati devono restare 0
  }
}

TEST_CASE("Gestione errori percentuale")
{
  // Setup: Creiamo un pattern di prova (dimensione 4x4 = 16 neuroni)
  const int size = 4;
  Pattern p(size);

  SUBCASE("Rumore negativo deve lanciare errore")
  {
    // Proviamo a passare un valore negativo (-10%)
    // CHECK_THROWS_AS verifica che venga lanciata ESATTAMENTE un'eccezione di
    // tipo std::invalid_argument
    CHECK_THROWS_AS(p.addNoise(-0.1f), std::invalid_argument);
  }

  SUBCASE("Rumore superiore al 100% deve lanciare errore")
  {
    // Proviamo a passare un valore superiore a 1.0 (110%)
    CHECK_THROWS_AS(p.addNoise(1.1f), std::invalid_argument);
  }

  SUBCASE("Rumore valido non deve lanciare errore")
  {
    // Controllo di sanità: valori validi (es. 50%) non devono crashare
    CHECK_NOTHROW(p.addNoise(0.5f));
    CHECK_NOTHROW(p.addNoise(0.0f)); // 0% è valido
    CHECK_NOTHROW(p.addNoise(1.0f)); // 100% è valido
  }
}
TEST_CASE("andamento e logica della funzione energia")
{
  const unsigned size = 4; // 16 neuroni totali
  Matrix m(size);
  Pattern p(size);

  // 1. Creiamo un pattern stabile (tutti i neuroni a 1)
  //    Questo sarà il nostro "fondo valle" energetico.
  for (unsigned i = 0; i < size * size; ++i) {
    p.setNeuron(i, 1);
  }

  // Insegniamo il pattern alla matrice
  m.learnPattern(p);

  // Calcoliamo l'energia del pattern perfetto
  float energy_perfect = m.calcEnergy(p);

  SUBCASE("L'energia aumenta se corrompo il pattern")
  {
    // Creiamo una copia e invertiamo manualmente un neurone (es. il primo)
    Pattern p_corrupted = p;
    p_corrupted.setNeuron(0, -p.getNeuron(0)); // Flip del bit 0

    float energy_corrupted = m.calcEnergy(p_corrupted);

    // VERIFICA: L'energia del pattern corrotto (instabile) DEVE essere
    // maggiore di quella del pattern perfetto (stabile).
    CHECK(energy_corrupted > energy_perfect);
  }

  SUBCASE("L'energia diminuisce durante la recall")
  {
    Pattern p_test = p;
    // Corrompiamo il pattern
    p_test.setNeuron(0, -1);
    p_test.setNeuron(1, -1);
    p_test.setNeuron(2, -1);

    float energy_before = m.calcEnergy(p_test);

    // facciamo recall

    m.recall(p_test);

    float energy_after = m.calcEnergy(p_test);

    // VERIFICA: Dopo il recupero, l'energia deve essere scesa (o essere uguale
    // se era già al minimo)
    CHECK(energy_after <= energy_before);

    // Alla fine dovrebbe essere tornato al livello del pattern perfetto
    CHECK(energy_after == doctest::Approx(energy_perfect));
  }
}