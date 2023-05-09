#include "..\Helper\Helper.h"

uint32_t vRootPrimes[78'500], noRootPrime = 0;
void AddRootPrime(uint32_t prime)
{
    if (prime == 2) noRootPrime = 0;
    vRootPrimes[noRootPrime++] = prime;
}

constexpr uint32_t LIMIT_ROOT_PRIMES = 1'000'000;

uint8_t mark[LIMIT_ROOT_PRIMES / 16 + 1];
uint32_t v1[1600], v2[1600];

void InitializeRootPrimes(const uint32_t Nmax = LIMIT_ROOT_PRIMES)
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
    AddRootPrime(2);

    uint32_t i, j, p, idx_s = 0;
    for (i = 0; i <= Nmax / 16; i++) mark[i] = 0; 

    for (i = 1; true; i++)
        if (!GetBit(i))
        {
            AddRootPrime(p = 2 * i + 1);

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
            AddRootPrime(2 * i + 1);
}

//no link, SoE
uint64_t GriesMisra_OptimSoE(const uint64_t Nmax, uint8_t mark[], void*, void*)
{
    assert(Nmax <= 1'000'000'000'000);
    InitializeRootPrimes();

    auto idx2no = [](uint64_t idx) { return 3 * idx + 5 - (idx & 1); };
    auto no2idx = [](uint64_t no) { return no / 3 - 1; };
    auto SetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; uint8_t bit = (uint8_t)(bitidx % 8);
        mark[idx] |= BIT_MASK[bit];
    };
    auto GetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; uint8_t bit = (uint8_t)(bitidx % 8);
        return (mark[idx] & BIT_MASK[bit]);
    };
    unsigned numPrimes = 2; AddPrime(2); AddPrime(3);

    const uint64_t i_max = Nmax / 24;
    for (uint64_t i = 0; i <= i_max; i++) { mark[i] = false; };

    const uint64_t nsqrt = (uint64_t)floor(sqrt(Nmax));
    uint64_t p, q, pq, i_q, i_p = 2;
    for (p = vRootPrimes[i_p++]; p <= nsqrt; p = vRootPrimes[i_p++])
    {
        numPrimes++; AddPrime(p);

        q = p; i_q = i_p;
        for (pq = p * q; pq <= Nmax; pq = p * q)
        {
            for (uint64_t n = pq; n <= Nmax; n *= p)
                SetBit(no2idx(n));

            q = vRootPrimes[i_q++];
        }
    }

    const uint64_t i_p_max = no2idx(Nmax);
    for (i_p = no2idx(p); i_p <= i_p_max; i_p++)
        if (!GetBit(i_p))
        {
            p = idx2no(i_p);
            if (p <= Nmax)
            {
                numPrimes++; AddPrime(p);
            }
            else
                break;
        }
    return numPrimes;
}

