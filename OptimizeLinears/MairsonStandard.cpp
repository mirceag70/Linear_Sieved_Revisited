#include "..\Helper\Helper.h"

typedef uint64_t tpUnsigned;

uint64_t Mairson_SingleLink(uint64_t Nmax, 
                            tpUnsigned rlink[], bool bDELETE[], void*)
{
    tpUnsigned i;
    for (i = 1; i <= Nmax; i++) { rlink[i] = i + 1; bDELETE[i] = false; }

    tpUnsigned nsqrt = (tpUnsigned)floor(sqrt(Nmax));
    for (tpUnsigned prime = 2; prime <= nsqrt; prime = rlink[prime])
    {
        tpUnsigned factor = prime;
        for (tpUnsigned k = prime * factor; k <= Nmax; k = prime * factor)
        {
            bDELETE[k] = true;
            tpUnsigned newfactor = rlink[factor];
            if (bDELETE[newfactor])     // we need to unlink it
                rlink[factor] = rlink[newfactor];
            factor = newfactor;
        }
    }

    tpUnsigned numPrimes = 0;
    for (i = 2; i <= Nmax; i++)
        if (not bDELETE[i]) { AddPrime(i); numPrimes++; }
    return numPrimes;
}

uint64_t Mairson_Standard(uint64_t Nmax, 
                          tpUnsigned rlink[], tpUnsigned llink[], tpUnsigned DELETE[])
{
    auto crossoff = [&](tpUnsigned i)
        { rlink[llink[i]] = rlink[i]; llink[rlink[i]] = llink[i]; };

    for (tpUnsigned i = 1; i <= Nmax; i++)
        { rlink[i] = i + 1; llink[i] = i - 1; DELETE[i] = 0; }
    rlink[Nmax] = llink[1] = 0;

    tpUnsigned nsqrt = (tpUnsigned)floor(sqrt(Nmax));
    for (tpUnsigned prime = 2; prime <= nsqrt; prime = rlink[prime])
    {
        tpUnsigned factor = prime, pointer = 0;
        for (tpUnsigned k = prime * factor; k <= Nmax; k = prime * factor)
            { DELETE[++pointer] = k; factor = rlink[factor]; }
        for (tpUnsigned i = 1; i <= pointer; i++)
            crossoff(DELETE[i]);
    }

    uint64_t numPrimes = 0;
    for (tpUnsigned i = rlink[1]; i > 0; i = rlink[i])
        { AddPrime(i); numPrimes++; }
    return numPrimes;
}
