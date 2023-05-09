#include "..\Helper\Helper.h"


uint64_t GriesMisra_Standard(const uint64_t Nmax, 
                             uint64_t rlink[], uint64_t llink[], void*)
{
    auto crossoff = [&](uint64_t i)
        { rlink[llink[i]] = rlink[i]; llink[rlink[i]] = llink[i]; };

    for (uint64_t i = 1; i <= Nmax; i++)
        { rlink[i] = i + 1; llink[i] = i - 1; }
    rlink[Nmax] = llink[1] = 0;

    uint64_t nsqrt = (uint64_t)ceil(sqrt(Nmax));
    for (uint64_t p = 2; p <= nsqrt; p = rlink[p])
    {
        uint64_t q = p;
        for (uint64_t pq = p * q; pq <= Nmax; pq = p * q)
        {
            uint64_t x = pq;
            while (x <= Nmax)
            {
                crossoff(x);
                x *= p;
            }
            q = rlink[q];
        }
    }

    unsigned numPrimes = 0;
    for (uint64_t i = rlink[1]; i > 0; i = rlink[i])
        { AddPrime(i); numPrimes++; }
    return numPrimes; 
}

// incremental, 8bit
uint64_t GriesMisra_Optim1(const uint64_t Nmax, uint8_t next[], bool mark[], void*)
{
    uint64_t i;
    for (i = 1; i < Nmax; i++) { next[i] = 1; mark[i] = false; };
    next[Nmax] = 0;

    uint64_t nsqrt = (uint64_t)floor(sqrt(Nmax));
    uint64_t p = 2, maxq = 0;
    while (p <= nsqrt)
    {
        uint64_t q = p;
        uint64_t pq = p * q;
        while (pq <= Nmax)
        {
            for (uint64_t x = pq; x <= Nmax; x *= p) { mark[x] = true; }
            uint64_t qnew = q + next[q];
            while (mark[qnew])     // we need to unlink it
            {
                next[q] += next[qnew];
                qnew += next[qnew];
                if (qnew > maxq) maxq = qnew;
            }
            q = qnew;
            pq = p * q;
        }
        p += next[p];
    }

    std::cout << maxq;

    unsigned numPrimes = 0;
    for (i = 2; next[i]; i += next[i])
    {
        if (!mark[i])
        {
            AddPrime(i);
            numPrimes++;
        }
    }
    return numPrimes;
}

// incremental, 8bit, 6k
uint64_t GriesMisra_Optim2(const uint64_t Nmax, uint8_t next[], bool mark[], void*)
{
    auto idx2no = [](uint64_t idx) { return 3 * idx + 5 - (idx & 1); };
    auto no2idx = [](uint64_t no) { return no / 3 - 1; };

    uint64_t i_max = no2idx(Nmax);
    uint64_t i_p_max = no2idx((uint64_t)floor(sqrt(Nmax)));
    unsigned numPrimes = 2; //account for 2 and 3
    AddPrime(2); AddPrime(3);

    uint64_t i;
    for (i = 0; i < i_max; i++) { mark[i] = false; };
    for (i = 0; i < Nmax / 15; i++) { next[i] = 1; };

    uint64_t nsqrt = (uint64_t)floor(sqrt(Nmax));
    uint64_t p, q, i_q, pq, i_p = 0;
    //uint64_t maxq = 0, px, qx;
    for (p = 5; p <= nsqrt; p = idx2no(i_p))
    {
        numPrimes++; AddPrime(p);

        q = p; pq = p * q; i_q = i_p;
        while (pq <= Nmax)
        {
            for (uint64_t n = pq; n <= Nmax; n *= p) 
                mark[no2idx(n)] = true;

            uint64_t i_qnew = i_q + next[i_q];
            while (mark[i_qnew])     // we need to unlink it
            {
                next[i_q] += next[i_qnew];
                i_qnew += next[i_qnew];
                //if (i_qnew > maxq)
                //{
                //    maxq = i_qnew;
                //    px = p; qx = q;
                //}
            }
            i_q = i_qnew;
            q = idx2no(i_q);
            pq = p * q;
        }
        i_p += next[i_p];
        
    }

    //std::cout << maxq << " : " << px << " : " << qx << " | ";

    for (; i_p <= i_max; i_p++)
        if (!mark[i_p])
        {
            p = idx2no(i_p);
            if (p <= Nmax)
                { numPrimes++; AddPrime(p); }
            else
                break;
        }
    return numPrimes;
}

const uint64_t BIT_MASK64[] = { 1, 2, 4, 8, 16, 32, 64, 128, 
    1ull << 8,  1ull << 9,  1ull << 10, 1ull << 11, 1ull << 12, 1ull << 13, 1ull << 14, 1ull << 15,
    1ull << 16, 1ull << 17, 1ull << 18, 1ull << 19, 1ull << 20, 1ull << 21, 1ull << 22, 1ull << 23,
    1ull << 24, 1ull << 25, 1ull << 26, 1ull << 27, 1ull << 28, 1ull << 29, 1ull << 30, 1ull << 31,
    1ull << 32, 1ull << 33, 1ull << 34, 1ull << 35, 1ull << 36, 1ull << 37, 1ull << 38, 1ull << 39,
    1ull << 40, 1ull << 41, 1ull << 42, 1ull << 43, 1ull << 44, 1ull << 45, 1ull << 46, 1ull << 47,
    1ull << 48, 1ull << 49, 1ull << 50, 1ull << 51, 1ull << 52, 1ull << 53, 1ull << 54, 1ull << 55,
    1ull << 56, 1ull << 57, 1ull << 58, 1ull << 59, 1ull << 60, 1ull << 61, 1ull << 62, 1ull << 63 };

//1bit
uint64_t GriesMisra_Optim3(const uint64_t Nmax, uint8_t next[], uint8_t mark[], void*)
{
    auto idx2no = [](uint64_t idx) { return 3 * idx + 5 - (idx & 1); };
    auto no2idx = [](uint64_t no) { return no / 3 - 1; };
    auto SetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; 
        uint8_t bit = (uint8_t)(bitidx % 8);
        mark[idx] |= BIT_MASK[bit];
    };
    auto GetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8;
        uint8_t bit = (uint8_t)(bitidx % 8);
        return (mark[idx] & BIT_MASK[bit]);
    };
    uint64_t i_max = Nmax / 24;
    unsigned numPrimes = 2; //account for 2 and 3
    AddPrime(2); AddPrime(3);

    uint64_t i;
    for (i = 0; i <= i_max; i++) { mark[i] = false; };
    for (i = 0; i <= Nmax / 15; i++) { next[i] = 1; };

    uint64_t nsqrt = (uint64_t)floor(sqrt(Nmax));
    uint64_t p, q, i_q, pq, i_p = 0;
    //uint64_t maxq = 0, px, qx;
    for (p = 5; p <= nsqrt; p = idx2no(i_p))
    {
        numPrimes++; AddPrime(p);

        q = p; pq = p * q; i_q = i_p;
        while (pq <= Nmax)
        {
            for (uint64_t n = pq; n <= Nmax; n *= p)
                SetBit(no2idx(n));

            uint64_t i_qnew = i_q + next[i_q];
            while (GetBit(i_qnew))     // we need to unlink it
            {
                next[i_q] += next[i_qnew];
                i_qnew += next[i_qnew];
                //if (i_qnew > maxq)
                //{
                //    maxq = i_qnew;
                //    px = p; qx = q;
                //}
            }
            i_q = i_qnew;
            q = idx2no(i_q);
            pq = p * q;
        }
        i_p += next[i_p];

    }

    //std::cout << maxq << " : " << px << " : " << qx << " | ";

    uint64_t i_p_max = no2idx(Nmax);
    for (; i_p <= i_p_max; i_p++)
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

//template<typename UNSIGNEDTYPE>
//void AdvanceBit(void* pflags, uint64_t& idx, bool initialize)
//{
//    UNSIGNEDTYPE* vflags = pflags;
//
//    uint64_t ix_flags = idx / sizeof(UNSIGNEDTYPE);
//    uint8_t ix_bit = (uint8_t)(idx % sizeof(UNSIGNEDTYPE));
//
//    uint64_t ix_flags = idx / sizeof(UNSIGNEDTYPE);
//    uint8_t ix_bit = (uint8_t)(idx % sizeof(UNSIGNEDTYPE));
//
//    do
//    {
//        if (qbit == (bits - 1))
//        {
//            qbit = 0; q64flags++;
//        }
//        else
//            qbit++;
//        i_q++;
//    }     
//    while (mark[q64flags] & BIT_MASK64[qbit]);
//}

//no link, basic
uint64_t GriesMisra_Optim4(const uint64_t Nmax, uint8_t mark[], void*, void*)
{
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
    uint64_t p, q, pq, i_q, i_p = 0;
    for (p = 5; p <= nsqrt; p = idx2no(i_p))
    {
        numPrimes++; AddPrime(p);

        q = p; i_q = i_p;
        for (pq = p * q; pq <= Nmax; pq = p * q)
        {
            for (uint64_t n = pq; n <= Nmax; n *= p)
                SetBit(no2idx(n));

            for( i_q++; GetBit(i_q); i_q++); // advance to the next uncancelled
            q = idx2no(i_q);
        }
        for( i_p++; GetBit(i_p); i_p++); // advance to the next uncancelled
    }

    const uint64_t i_p_max = no2idx(Nmax);
    for (; i_p <= i_p_max; i_p++)
        if (!GetBit(i_p))
        {
            p = idx2no(i_p);
            if (p <= Nmax)
                { numPrimes++; AddPrime(p); }
            else
                break;
        }
    return numPrimes;
}



//no link
uint64_t GriesMisra_Optim5(const uint64_t Nmax, uint64_t mark[], void*, void*)
{
    constexpr uint8_t sz_bits = sizeof(*mark);
        
    auto idx2no = [](uint64_t idx) { return 3 * idx + 5 - (idx & 1); };
    auto no2idx = [](uint64_t no) { return no / 3 - 1; };
    auto SetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / sz_bits; uint8_t bit = (uint8_t)(bitidx % sz_bits);
        mark[idx] |= (1ull << bit); 
        //mark[idx] |= BIT_MASK[bit];
    };
    auto GetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / sz_bits; uint8_t bit = (uint8_t)(bitidx % sz_bits);
        return (mark[idx] & (1ull << bit));
        //return (mark[idx] & BIT_MASK[bit]);
    };
    const uint64_t i_max = Nmax / (3 * sz_bits);
    unsigned numPrimes = 2; //account for 2 and 3
    AddPrime(2); AddPrime(3);

    uint64_t i;
    for (i = 0; i <= i_max; i++) { mark[i] = false; };

    uint64_t nsqrt = (uint64_t)floor(sqrt(Nmax));
    uint64_t p, q, pq, i_q, i_p = 0;
    for (p = 5; p <= nsqrt; p = idx2no(i_p))
    {
        numPrimes++; AddPrime(p);

        //constexpr auto bits = 8;

        q = p; i_q = i_p;
        for (pq = p * q; pq <= Nmax; pq = p * q)
        {
            for (uint64_t n = pq; n <= Nmax; n *= p)
                SetBit(no2idx(n));

            //for (i_q++; mark[i_q / 8] & BIT_MASK[(uint8_t)(i_q % 8)]; i_q++);
            
            for (i_q++; GetBit(i_q); i_q++); // advance to the next zero
            q = idx2no(i_q);
        }
        for (i_p++; GetBit(i_p); i_p++); // advance to the next zero
    }

    uint64_t i_p_max = no2idx(Nmax);
    for (; i_p <= i_p_max; i_p++)
        if (!GetBit(i_p))
        {
            p = idx2no(i_p);
            //numPrimes++; AddPrime(p); 
            if (p <= Nmax)
                { numPrimes++; AddPrime(p); }
            else
                break;
        }
    return numPrimes;
}


uint64_t Eratosthenes_6k(const uint64_t Nmax, uint8_t vPrimes[], void*, void*)
{
    auto idx2no = [](uint64_t idx) { return 3 * idx + 5 - (idx & 1); };
    auto no2idx = [](uint64_t no) { return no / 3 - 1; };
    auto SetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; uint8_t bit = (uint8_t)(bitidx % 8);
        vPrimes[idx] |= BIT_MASK[bit];
    };
    auto GetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; uint8_t bit = (uint8_t)(bitidx % 8);
        return (vPrimes[idx] & BIT_MASK[bit]);
    };

    const uint64_t i_max = Nmax / (3 * 8);
    const uint64_t Nsqrt = (uint64_t)floor(sqrt(Nmax));
    unsigned numPrimes = 2; AddPrime(2); AddPrime(3);

    uint64_t i, j;
    for (i = 0; i <= i_max; i++) vPrimes[i] = 0;

    uint8_t stepi = 2;
    for (i = 5; i <= Nsqrt; i += stepi, stepi = 6 - stepi)
        if (!GetBit(no2idx(i)))
        {
            numPrimes++; AddPrime(i);

            uint64_t stepj2 = 2 * i, stepj6 = 6 * i;
            uint64_t stepj = ((i - (i / 3) * 3) == 1) ? (stepj6 - stepj2) : stepj2;

            for (j = i * i; j <= Nmax; j += stepj, stepj = stepj6 - stepj)
                SetBit(no2idx(j));
        }

    for ( /*continue counting primes*/; i <= Nmax; i += stepi, stepi = 6 - stepi)
        if (!GetBit(no2idx(i)))
        {
            numPrimes++; AddPrime(i);
        }

    return numPrimes;
}
