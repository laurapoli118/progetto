#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "matrix.hpp"
#include "pattern.hpp"

TEST_CASE("Proprietà matrice dei pesi") 
{
    const unsigned int size = 4; //misura minore per max velocità
    Matrix m(size);
    Pattern p(size);
    for(unsigned i=0; i<size; i++)
    {
        p.setNeuron(i, 1.0f);
    }
    m.learnPattern(p);

    SUBCASE("diagonale nulla")
    {
        for(unsigned i=0; i<size; i++) {
            CHECK(m.getWeight(i,i)==doctest::Approx(0.0f));// per confrontare i float
        }
    }

    SUBCASE("simmetria"){
        for(unsigned i=0; i<size; i++){
            CHECK(m.getWeight(i,j)==doctest::Approx(j,i));
        }
    }

}