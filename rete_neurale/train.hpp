#include <vector>


class Train
{

private:

std::vector<float> weights;  // da decidere come la vogliamo fare
unsigned numNeurons; 

public:

float getWeight(unsigned i, unsigned j) const;

void setWeight(unsigned i, unsigned j);




};