#include "..\Helper\Helper.h"

//return 2 * (no / 6) - ((((2 * (no / 6)) * 3) + 1) == no);
//return 6 * (idx / 2) + 5 + 2 * (idx & 1);

//const uint8_t delta[] = { 5, 7, 11, 13, 17, 19, 23, 25 };

//uint8_t next[10000];
//uint8_t mark[10000];

// step over 2, 3, no gaps, bit mark
uint64_t Mairson_Optim10(const uint64_t Nmax, uint8_t next[], uint8_t mark[], void*)
{
    auto SetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8;
        //uint8_t bit = (uint8_t)(bitidx - (idx * 8));
        uint8_t bit = (uint8_t)(bitidx % 8);
        mark[idx] |= BIT_MASK[bit];
    };
    auto GetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8;
        //uint8_t bit = (uint8_t)(bitidx - (idx * 8));
        uint8_t bit = (uint8_t)(bitidx % 8);
        return (mark[idx] & BIT_MASK[bit]);
        //return _bittest((long*)(mark + idx), bit);
    };

    const uint64_t i_p_max = 1 + no2idx((uint64_t)floor(sqrt(Nmax)));
    const uint64_t i_max = Nmax / 24, i_max_7 = Nmax / 33;
    unsigned numPrimes = 4; //account for 2 and 3 and 5 and 7
    AddPrime(2); AddPrime(3); AddPrime(5); AddPrime(7);

    uint64_t i, p, i_p, f, i_f, n, i_n; uint8_t step;
    for (i = 0; i <= i_max; i++) { mark[i] = false; }
    for (i = 0; i <= i_max_7; i++) { next[i] = 1; }; //next[i] = 0;

    //hard-code 5 and 7
    f = p = 5; n = f * p;
    for (step = 2; n <= Nmax; step = 6 - step)
    {
        i_n = no2idx(n); SetBit(i_n);
        f += step; n = f * p;
    }
    f = p = 7; n = f * p;
    for (i_f = 1; i_f <= i_max_7; n = f * p)
    {
        i_n = no2idx(n); SetBit(i_n);

        uint64_t i_fnew = i_f + next[i_f];
        if (GetBit(i_fnew))     // we need to unlink it
            next[i_f] += next[i_fnew];
        i_f = i_fnew;

        f = idx2no(i_f);
    }
    for (; n <= Nmax; n = f * p)
    {
        i_n = no2idx(n); SetBit(i_n);

        i_f++;

        f = idx2no(i_f);
    }
    //continue from 11
    for (i_f = i_p = 2; i_p <= i_p_max; i_f = (i_p += next[i_p]))
    {
        f = p = idx2no(i_p);
        numPrimes++; AddPrime(p);

        for (n = f * p; n <= Nmax; n = f * p)
        {
            i_n = no2idx(n); SetBit(i_n);

            uint64_t i_fnew = i_f + next[i_f];
            if (GetBit(i_fnew))     // we need to unlink it
                next[i_f] += next[i_fnew];
            i_f = i_fnew; f = idx2no(i_f);
        }
    }

    p = idx2no(i_p); i = (p - 5) / 24;
    uint64_t pbase = (i * 24) + 5;
    uint8_t b = (uint8_t)(p - pbase);
    switch (b)
    {
    case 0: b = 0; step = 2; break;
    case 2: b = 1; step = 4; break;
    case 6: b = 2; step = 2; break;
    case 8: b = 3; step = 4; break;
    case 12: b = 4; step = 2; break;
    case 14: b = 5; step = 4; break;
    case 18: b = 6; step = 2; break;
    case 20: b = 7; step = 4; break;
    default: assert(false);
    }
    uint8_t flags = mark[i];
    assert(!(flags & BIT_MASK[b]));
    for (; b < 8; b++)
    {
        if (!(flags & BIT_MASK[b])) { numPrimes++; AddPrime(p); }
        p += step; step = 6 - step;
    }
    for (i++; i < i_max-1; i++)
    {
        flags = mark[i];
        if (!(flags & BIT_MASK[0])) { numPrimes++; AddPrime(p); }
        if (!(flags & BIT_MASK[1])) { numPrimes++; AddPrime(p + 2); }
        if (!(flags & BIT_MASK[2])) { numPrimes++; AddPrime(p + 6); }
        if (!(flags & BIT_MASK[3])) { numPrimes++; AddPrime(p + 8); }
        if (!(flags & BIT_MASK[4])) { numPrimes++; AddPrime(p + 12); }
        if (!(flags & BIT_MASK[5])) { numPrimes++; AddPrime(p + 14); }
        if (!(flags & BIT_MASK[6])) { numPrimes++; AddPrime(p + 18); }
        if (!(flags & BIT_MASK[7])) { numPrimes++; AddPrime(p + 20); }
        p += 24;
    }
    while (p <= Nmax)
    {
        flags = mark[i++];
        for (b = 0; b < 8; b++)
        {
            if (p > Nmax) break;
            if (!(flags & BIT_MASK[b])) { numPrimes++; AddPrime(p); }
            p += step; step = 6 - step;
        }
    }
    /*
    const uint64_t i_maxp = no2idx(Nmax);
    for (; i_p <= i_maxp; i_p++)
        if (!GetBit(i_p))
        {
            p = idx2no(i_p);
            if (p <= Nmax)
                { numPrimes++; AddPrime(p); }
            else
                break;
        }
    */
    return numPrimes;
}








//-------------------------------
// BAD CODE
//-------------------------------


bool mark_work = true;
std::chrono::microseconds zeros(0);
uint16_t idx_client = 0;
uint16_t idx_server = 0;
uint64_t mark_buffer[_UI16_MAX];
//uint8_t idx_client = 0;
//uint8_t idx_server = 0;
//uint64_t mark_buffer[_UI8_MAX];

uint64_t loops_client, loops_server;

void mark_bits(uint8_t vmark[])
{
    loops_client = loops_server = 0;

    auto SetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8;
        //uint8_t bit = (uint8_t)(bitidx - (idx * 8));
        uint8_t bit = (uint8_t)(bitidx % 8);
        vmark[idx] |= BIT_MASK[bit];
    };

    while (mark_work)
    {
        std::this_thread::sleep_for(zeros);
        while (idx_client != idx_server)
        {
            SetBit(no2idx(mark_buffer[idx_server++]));
            if (idx_server == 0) //std::cout << "+";
            {
                loops_server++;
            }
        }
    }
}

inline void send_to_marking_thread(uint64_t no)
{
    mark_buffer[idx_client++] = no;
    if (idx_client == 0) //std::cout << "=";
    {
        loops_client++;
        while (loops_client > loops_server)
            std::this_thread::sleep_for(zeros);
    }
}

// thread
uint64_t Mairson_Optim011(const uint64_t Nmax, uint8_t next[], uint8_t mark[], void*)
{
    auto SetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8;
        //uint8_t bit = (uint8_t)(bitidx - (idx * 8));
        uint8_t bit = (uint8_t)(bitidx % 8);
        mark[idx] |= BIT_MASK[bit];
    };
    auto GetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8;
        //uint8_t bit = (uint8_t)(bitidx - (idx * 8));
        uint8_t bit = (uint8_t)(bitidx % 8);
        return (mark[idx] & BIT_MASK[bit]);
        //return _bittest((long*)(mark + idx), bit);
    };

    uint64_t i_p_max = 1 + no2idx((uint64_t)floor(sqrt(Nmax)));
    unsigned numPrimes = 3; //account for 2 and 3 and 5
    AddPrime(2); AddPrime(3); AddPrime(5);

    uint64_t i;
    const uint64_t i_max = Nmax / 24;
    for (i = 0; i <= i_max; i++) { next[i] = 1; mark[i] = false; }
    for (; i <= (Nmax / 21); i++) { next[i] = 1; }

    mark_work = true;
    std::thread tmark(&mark_bits, mark);

    uint64_t p, i_p, f, i_f, n, i_n;
    //hard-code 5
    f = p = 5; n = f * p;
    for (uint8_t step = 2; n <= Nmax; step = 6 - step)
    {
        i_n = no2idx(n);
        SetBit(i_n);
        f += step;
        n = f * p;
    }
    //start from 7
    for (i_f = i_p = 1; i_p <= i_p_max; i_f = (i_p += next[i_p]))
    {
        f = p = idx2no(i_p);
        numPrimes++;
        AddPrime(p);

        for (n = f * p; n <= Nmax; n = f * p)
        {
            //i_n = no2idx(n);
            //SetBit(i_n);
            send_to_marking_thread(n);

            uint64_t i_fnew = i_f + next[i_f];
            if (GetBit(i_fnew))     // we need to unlink it
                next[i_f] += next[i_fnew];
            i_f = i_fnew;
            f = idx2no(i_f);
        }
        while (idx_client != idx_server) 
            std::this_thread::sleep_for(zeros);
    }

    mark_work = false;

    p = idx2no(i_p);
    i = (p - 5) / 24;
    uint64_t pbase = (i * 24) + 5;
    uint8_t b = (uint8_t)(p - pbase);
    uint8_t step;
    switch (b)
    {
    case 0: b = 0; step = 2; break;
    case 2: b = 1; step = 4; break;
    case 6: b = 2; step = 2; break;
    case 8: b = 3; step = 4; break;
    case 12: b = 4; step = 2; break;
    case 14: b = 5; step = 4; break;
    case 18: b = 6; step = 2; break;
    case 20: b = 7; step = 4; break;
    default: assert(false);
    }
    uint8_t flags = mark[i];
    assert(!(flags & BIT_MASK[b]));
    for (; b < 8; b++)
    {
        if (!(flags & BIT_MASK[b])) { numPrimes++; AddPrime(p); }
        p += step; step = 6 - step;
    }
    for (i++; i < i_max-1; i++)
    {
        flags = mark[i];
        if (!(flags & BIT_MASK[0])) { numPrimes++; AddPrime(p); }
        if (!(flags & BIT_MASK[1])) { numPrimes++; AddPrime(p + 2); }
        if (!(flags & BIT_MASK[2])) { numPrimes++; AddPrime(p + 6); }
        if (!(flags & BIT_MASK[3])) { numPrimes++; AddPrime(p + 8); }
        if (!(flags & BIT_MASK[4])) { numPrimes++; AddPrime(p + 12); }
        if (!(flags & BIT_MASK[5])) { numPrimes++; AddPrime(p + 14); }
        if (!(flags & BIT_MASK[6])) { numPrimes++; AddPrime(p + 18); }
        if (!(flags & BIT_MASK[7])) { numPrimes++; AddPrime(p + 20); }
        p += 24;
    }
    while (p <= Nmax)
    {
        flags = mark[i++];
        for (b = 0; b < 8; b++)
        {
            if (p > Nmax) break;
            if (!(flags & BIT_MASK[b])) { numPrimes++; AddPrime(p); }
            p += step; step = 6 - step;
        }
    }
    /*
    const uint64_t i_max = no2idx(Nmax);
    for (; i_p <= i_max; i_p++)
    {
        if (!GetBit(i_p))
        {
            p = idx2no(i_p);
            if (p <= Nmax)
            {
                numPrimes++;
                AddPrime(p);
            }
            else
                break;
        }
    }
    */
    
    tmark.join();

    return numPrimes;
}

