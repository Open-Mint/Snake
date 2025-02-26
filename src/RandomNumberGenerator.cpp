#include <RandomNumberGenerator.h>

RandomNumberGenerator::RandomNumberGenerator(int from, int to)
: gen(rd()), dist(from, to), number(dist(gen))
{
}

int RandomNumberGenerator::getNumber() const
{
    return number;
}
