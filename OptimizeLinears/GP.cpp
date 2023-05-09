#include "..\Helper\Helper.h"

//pritchard 4.2
uint64_t GalePratt1(uint64_t Nmax, bool C[], void*, void*)
{
    std::vector<uint64_t> P, F, G;

    uint64_t i, p, f, pf;
    for (i = 0; i <= Nmax; i++) C[i] = false;     // assume all primes

    for (p = 2; p <= Nmax / 2; p++)
        if (!C[p])
        {
            F.push_back(p);
            std::vector<uint64_t> G;
            for (i = 0; i < F.size(); i++)
            {
                f = F[i];
                pf = p * f;
                if (pf <= Nmax)
                {
                    C[pf] = true;       // composite
                    F.push_back(pf);
                    G.push_back(f);
                }
            }
            F = G;
        }

    uint64_t numPrimes = 0;
    for (i = 2; i <= Nmax; i++)
        if (!C[i])
        {
            numPrimes++;
            AddPrime(i);
        }

    return numPrimes;
}

// 4.2 optimized
uint64_t GalePratt3(uint64_t Nmax, bool C[], uint64_t FG[], void*)
{
    uint64_t i, p, f, pf;
    for (i = 0; i <= Nmax; i++) C[i] = false;     // assume all primes

    uint64_t idx_F = 0, idx_G, maxidx = 0, maxp{};
    for (p = 2; p <= Nmax / 2; p++)
        if (!C[p])
        {
            idx_G = 0;
            FG[idx_F++] = p;
            for (i = 0; i < idx_F; i++)
            {
                f = FG[i];
                pf = p * f;
                if (pf <= Nmax)
                {
                    C[pf] = true;       // composite
                    FG[idx_F++] = pf;
                    FG[idx_G++] = f;
                }
            }
            idx_F = idx_G;
        }

    uint64_t numPrimes = 0;
    for (i = 2; i <= Nmax; i++)
        if (!C[i])
        {
            numPrimes++;
            AddPrime(i);
        }

    return numPrimes;
}

//GP bool
uint64_t GalePratt2(uint64_t Nmax, bool sv[], void*, void*)
{
    std::vector<uint64_t> s = { 1 };

    uint64_t i, k, m;
    for (i = 0; i <= Nmax; i++) sv[i] = false;     // assume all primes

    for (i = 3; i <= Nmax / 2; i += 2)
        if (!sv[i])
        {
            std::vector<uint64_t> ts;
            for (auto j : s)
            {
                k = j;
                while (k < Nmax)
                {
                    if ((k != 1) and (k != i) and (k != j))
                        sv[k] = true;
                    m = k * i;
                    if (m < Nmax)
                        ts.push_back(k);
                    k = m;
                }
            }
            s = ts;
        }

    uint64_t numPrimes = 1; AddPrime(2);
    for (i = 3; i <= Nmax; i += 2)
        if (!sv[i])
        {
            numPrimes++;
            AddPrime(i);
        }

    return numPrimes;
}

//GP 1 bit
uint64_t GalePratt4(uint64_t Nmax, uint8_t sv[], void*, void*)
{
    const uint8_t BIT_MASK[] = { 1, 2, 4, 8, 16, 32, 64, 128 };
    auto SetBit = [&](uint64_t bitidx)
    {
        bitidx /= 2;
        uint64_t idx = bitidx / 8;
        uint8_t bit = (uint8_t)(bitidx % 8);
        sv[idx] |= BIT_MASK[bit];
    };
    auto GetBit = [&](uint64_t bitidx)
    {
        bitidx /= 2;
        uint64_t idx = bitidx / 8;
        uint8_t bit = (uint8_t)(bitidx % 8);
        return (sv[idx] & BIT_MASK[bit]);
    };

    std::vector<uint64_t> s = { 1 }, ts = {};
    uint64_t numPrimes = 1; AddPrime(2);

    uint64_t i, k, m, p;
    for (i = 0; i <= Nmax / 16; i++) sv[i] = 0;     // assume all primes

    for (p = 3; p <= Nmax / 2; p += 2)
        if (!GetBit(p))
        {
            numPrimes++; AddPrime(p);

            for (auto j : s)
            {
                k = j;
                while (k < Nmax)
                {
                    if ((k != 1) and (k != p) and (k != j))
                        SetBit(k);
                    m = k * p;
                    if (m < Nmax)
                        ts.push_back(k);
                    k = m;
                }
            }
            s = ts;
            ts.resize(0);
        }

    for (; p <= Nmax; p += 2)
        if (!GetBit(p))
            { numPrimes++; AddPrime(p); }

    return numPrimes;
}

//GP pointer
uint64_t GalePratt5(uint64_t Nmax, uint8_t mark[], uint64_t v1[], uint64_t v2[])
{
    const uint8_t BIT_MASK[] = { 1, 2, 4, 8, 16, 32, 64, 128 };
    auto SetBit = [&](uint64_t bitidx)
    {
        bitidx /= 2;
        uint64_t idx = bitidx / 8;
        uint8_t bit = (uint8_t)(bitidx % 8);
        mark[idx] |= BIT_MASK[bit];
    };
    auto GetBit = [&](uint64_t bitidx)
    {
        bitidx /= 2;
        uint64_t idx = bitidx / 8;
        uint8_t bit = (uint8_t)(bitidx % 8);
        return (mark[idx] & BIT_MASK[bit]);
    };

    uint64_t* s = v1; uint64_t* ts = v2;
    s[0] = 1;
    uint64_t idx_s = 1;
    uint64_t numPrimes = 1; AddPrime(2);

    uint64_t i, j, k, m, p;
    for (i = 0; i <= Nmax/16; i++) mark[i] = 0;     // assume all primes

    for (p = 3; p <= Nmax / 2; p += 2)
        if (!GetBit(p))
        {
            numPrimes++; AddPrime(p); 

            uint64_t idx_ts = 0;
            for (j = 0; j < idx_s; j++)
            {
                k = s[j];
                while (k < Nmax)
                {
                    if ((k != 1) and (k != p) and (k != s[j]))
                        SetBit(k);
                    m = k * p;
                    if (m < Nmax)
                    {
                        ts[idx_ts++] = k;
                    }
                    k = m;
                }
            }
            idx_s = idx_ts;
            std::swap(s, ts);
        }

    for (; p <= Nmax; p += 2)
        if (!GetBit(p))
            { numPrimes++; AddPrime(p); }

    return numPrimes;
}

//uint64_t v1[10000], v2[10000];

uint64_t GalePratt6(uint64_t Nmax, uint8_t mark[], uint64_t v1[], uint64_t v2[])
{
    const uint8_t BIT_MASK[] = { 1, 2, 4, 8, 16, 32, 64, 128 };
    auto SetBit = [&](uint64_t bitidx)
    {
        //bitidx /= 2;
        uint64_t idx = bitidx / 8;
        uint8_t bit = (uint8_t)(bitidx % 8);
        mark[idx] |= BIT_MASK[bit];
    };
    auto GetBit = [&](uint64_t bitidx)
    {
        //bitidx /= 2;
        uint64_t idx = bitidx / 8;
        uint8_t bit = (uint8_t)(bitidx % 8);
        return (mark[idx] & BIT_MASK[bit]);
    };

    uint64_t* s = v1; uint64_t* ts = v2;
    uint64_t numPrimes = 1; AddPrime(2);

    uint64_t i, j, k, m, p, maxidx = 0, maxp = 0, lastp = 0;
    for (i = 0; i <= Nmax / 16; i++) mark[i] = 0;     // assume all primes

    uint64_t idx_s = 0;// 1; s[0] = 1;
    for (i = 1; true; i++)
        if (!GetBit(i))
        {
            p = 2 * i + 1;
            numPrimes++; AddPrime(p);

            uint64_t idx_ts = 1; ts[0] = p;
            for(uint64_t pp = p*p; pp <= Nmax; pp *= p)
                { SetBit(pp/2); ts[idx_ts++] = pp; }

            for (j = 0; j < idx_s; j++)
            {
                k = s[j];
                for (m = k * p; m <= Nmax; m = k * p)
                {
                    ts[idx_ts++] = k;
                    SetBit(m/2);
                    //lastp = p;
                    k = m;
                }
            }
            if (idx_ts == 1) 
                break;
            //if (idx_ts > maxidx)
            //{
            //    maxidx = idx_ts;
            //    maxp = p;
            //}
            idx_s = idx_ts;
            std::swap(s, ts);
        }

    //std::cout << maxidx << " : " << maxp << " : " << lastp;

    for (i++; i < Nmax/2; i++)
        if (!GetBit(i))
            { numPrimes++; AddPrime(2*i+1); }

    return numPrimes;
}

uint64_t GalePratt7(uint64_t Nmax, uint8_t mark[], uint32_t v1[], uint32_t v2[])
{
    const uint8_t BIT_MASK[] = { 1, 2, 4, 8, 16, 32, 64, 128 };
    auto SetBit = [&](uint32_t bitidx)
    {
        uint32_t idx = bitidx / 8;
        uint8_t bit = (uint8_t)(bitidx % 8);
        mark[idx] |= BIT_MASK[bit];
    };
    auto GetBit = [&](uint32_t bitidx)
    {
        uint32_t idx = bitidx / 8;
        uint8_t bit = (uint8_t)(bitidx % 8);
        return (mark[idx] & BIT_MASK[bit]);
    };

    uint32_t* s = v1; uint32_t* ts = v2;
    uint32_t numPrimes = 1; AddPrime(2);

    uint32_t i, j, p, idx_s = 0;
    for (i = 0; i <= Nmax / 16; i++) mark[i] = 0;     // assume all primes

    for (i = 1; true; i++)
        if (!GetBit(i))
        {
            p = 2 * i + 1;
            numPrimes++; AddPrime(p);

            uint32_t idx_ts = 1; uint64_t m = ts[0] = p;
            for (m *= p; m <= Nmax; m *= p)
                { SetBit(m / 2); ts[idx_ts++] = m; }

            for (j = 0; j < idx_s; j++)
            {
                uint64_t k = s[j];
                for (m = k * p; m <= Nmax; k = m, m = k * p)
                    { ts[idx_ts++] = k; SetBit(m / 2); }
            }
            if (idx_ts == 1) break;
            idx_s = idx_ts; std::swap(s, ts);
        }

    for (i++; i < Nmax / 2; i++)
        if (!GetBit(i))
        {
            numPrimes++; AddPrime(2 * i + 1);
        }

    return numPrimes;
}
