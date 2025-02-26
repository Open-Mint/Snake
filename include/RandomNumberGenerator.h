#include <random>

class RandomNumberGenerator
{
private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist;
    int number;
public:
    RandomNumberGenerator(int from, int to);
    int getNumber() const;
};
