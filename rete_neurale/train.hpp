#include <vector>
#include <pattern.hpp>

class Train
{

private:
    std::vector<std::vector<float>> weights; // da decidere come la vogliamo fare
    unsigned numNeurons;

public:
    Train(unsigned size);

    float getWeight(unsigned i, unsigned j) const;

    void setWeight(unsigned i, unsigned j, float weight);

    void learnPattern(const Pattern &pattern);
};