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
            for(unsigned j=0; j<size; j++) {
                CHECK(m.getWeight(i,j)==doctest::Approx(m.getWeight(j,i)));
            }
        }
    }

}

TEST_CASE("accumulodi due pattern") {
     const unsigned int size = 4; //misura minore per max velocità
    Matrix m(size);
    Pattern p1(size);
    Pattern p2(size);
    for(unsigned i=0; i<size; i++) {
        p1.setNeuron(i, 1.0f); //[1,1,1,1]
        p2.setNeuron(i, (i<2)? -1.0f : 1.0f); //[-1,-1,1,1]
    }
    m.learnPattern(p1);
    m.learnPattern(p2);
    SUBCASE("peso si annullano") {
        CHECK(m.getWeight(0,2)==doctest::Approx(0.0f)); //0.25+(-0.25)
        CHECK(m.getWeight(1,3)==doctest::Approx(0.0f)); 
    }
    SUBCASE("peso si sommano") {
        CHECK(m.getWeight(2,3)==doctest::Approx(0.5f));
        CHECK(m.getWeight(0,1)==doctest::Approx(0.5f));

    }


}