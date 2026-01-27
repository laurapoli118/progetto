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

TEST_CASE("accumulo di due pattern") {
     const unsigned int size = 4; //misura minore per max velocità
    Matrix m(size);
    Pattern p1(size);
    Pattern p2(size);
    for(unsigned i=0; i<size*size; i++) {
        p1.setNeuron(i, 1.0f); //[1,1,1,1,1....1] dimensione 16cd 
        p2.setNeuron(i, (i<2)? -1.0f : 1.0f); //[-1,-1,1,1,1,1,1....1]
    }
    m.learnPattern(p1);
    m.learnPattern(p2);
    SUBCASE("i pesi si annullano") {
        CHECK(m.getWeight(0,2)==doctest::Approx(0.0f)); //0.25+(-0.25)
        CHECK(m.getWeight(1,3)==doctest::Approx(0.0f)); 
    }
    SUBCASE("i pesi si sommano") {
        CHECK(m.getWeight(2,3)==doctest::Approx(0.125f));
        CHECK(m.getWeight(0,1)==doctest::Approx(0.125f));

    }
    SUBCASE("riconoscimento di un pattern non corrotto") {

        Pattern test_p1 = p1; 
        m.recall(test_p1); 
        for(unsigned i = 0; i < size * size; i++) {
            CHECK(test_p1.getNeuron(i) == doctest::Approx(p1.getNeuron(i))); 
        }

        Pattern test_p2 = p2;
        m.recall(test_p2); 
        for(unsigned i = 0; i < size * size; i++) {
            CHECK(test_p2.getNeuron(i) == doctest::Approx(p2.getNeuron(i))); 
        }
}


}
TEST_CASE("Gestione errori dimensioni") {
    Matrix m(4); // Matrice per 16 neuroni (16x16)
    Pattern p_sbagliato(3); // Pattern per 9 neuroni (3x3)
    
    // Verifichiamo che la funzione rifiuti il pattern sbagliato
    CHECK_THROWS_AS(m.learnPattern(p_sbagliato), std::runtime_error);
}
TEST_CASE("Costruttore con size zero") {
    // serve a evitare che il programma impazzisca dividenso per 0 nella regola di Hebb
    CHECK_THROWS(Matrix(0));
}



