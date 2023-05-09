#include "..\Helper\Helper.h"

//uint16_t link[10'001];
//bool mark[10'001];
//uint16_t* link;
//
//uint8_t* next;
//uint8_t* mark;

uint64_t Mairson_Optim1(const uint64_t Nmax, uint16_t link[], bool mark[], void*)
{
    uint64_t nsqrt = (uint64_t)floor(sqrt(Nmax));
    unsigned numPrimes = 0;

    uint64_t i, p;
    for (i = 0; i <= Nmax; i++) { link[i] = 1; mark[i] = false; }

    for (p = 2; p <= nsqrt; p += link[p])
    {
        numPrimes++; AddPrime(p);

        uint64_t f = p;
        for (uint64_t n = p * f; n <= Nmax; n = p * f)
        {
            mark[n] = true;
            uint64_t fnew = f + link[f];
            if (mark[fnew])     // we need to unlink it
                link[f] += link[fnew];
            f = fnew;
        }
    }

    for (i = p; i <= Nmax; i++) 
        if (!mark[i]) { numPrimes++; AddPrime(i); }
    return numPrimes;
}

/*
// negative value is marked
uint64_t Mairson_Optim2(const uint64_t Nmax)
{
    uint64_t nsqrt = (uint64_t)floor(sqrt(Nmax));
    unsigned numPrimes = 0;

    uint64_t i, p;
    for (i = 0; i <= Nmax; i++) link[i] = 1;

    for (p = 2; p <= nsqrt; p += link[p])
    {
        numPrimes++;

        uint64_t f = p;
        uint64_t n = p * f;
        while (n <= Nmax)
        {
            link[n] = -link[n];
            int16_t deltaf = link[f];
            uint64_t fnew = (deltaf > 0) ? (f + deltaf) : (f - deltaf);
            //uint64_t fnew = f;
            //if (deltaf > 0)
            //    fnew += deltaf;
            //else
            //    fnew -= deltaf;
            deltaf = link[fnew];
            if (deltaf < 0)
            { // we need to unlink it
                link[f] -= deltaf;
            }
            f = fnew;
            n = p * f;
        }
    }

    for (i = p; i <= Nmax; i++)
    {
        if (link[i]>0)
            numPrimes++;
    }
    return numPrimes;
}

// first bit is the mark
uint64_t Mairson_Optim3(const uint64_t Nmax)
{
    uint64_t nsqrt = (uint64_t)floor(sqrt(Nmax));
    unsigned numPrimes = 0;

    uint64_t i, p;
    for (i = 0; i <= Nmax; i++) link[i] = 1;

    const uint16_t MASK_BIT_16 = 0b1000'0000'0000'0000;
    auto SetFirstBit = [](uint16_t& bits) { bits |= MASK_BIT_16; };
    auto GetFirstBit = [](uint16_t& bits) { return (bits & MASK_BIT_16); };
    auto GetLastBits = [](uint16_t& bits) { return (bits & (~MASK_BIT_16)); };

    for (p = 2; p <= nsqrt; p += link[p])
    {
        numPrimes++;

        uint64_t f = p;
        uint64_t n = p * f;
        while (n <= Nmax)
        {
            // mark composite n
            SetFirstBit(link[n]);
            // advance f
            uint64_t fnew = f + GetLastBits(link[f]);
            uint16_t lfnew = link[fnew];
            if (GetFirstBit(lfnew))     // we need to unlink it
                link[f] += GetLastBits(lfnew);;
            f = fnew;
            //advance n
            n = p * f;
        }
    }

    for (i = p; i <= Nmax; i++)
    {
        if (!GetFirstBit(link[i]))
            numPrimes++;
    }
    return numPrimes;
}
*/

// step over evens
uint64_t Mairson_Optim4(const uint64_t Nmax, uint16_t link[], bool mark[], void*)
{
    uint64_t i_max = (Nmax - 3) / 2;
    uint64_t i_p_max = ((uint64_t)floor(sqrt(Nmax)) - 3) / 2;
    unsigned numPrimes = 1; AddPrime(2); //account for 2
    
    for (uint64_t i = 0; i <= i_max; i++) { link[i] = 1; mark[i] = false; }

    uint64_t i_p, p, i_f, f, i_n, n;

    for (i_p = 0, p = 3; i_p <= i_p_max; i_p += link[i_p], p = 2 * i_p + 3)
    {
        numPrimes++; AddPrime(p);

        i_f = i_p;
        f = p;  // 2 * i_p + 3
        n = p * f; // 4 * i_p * i_p + 12 * i_p + 9

        while (n <= Nmax)
        {
            i_n = (n - 3) / 2; mark[i_n] = true;
            
            uint64_t i_fnew = i_f + link[i_f];
            if (mark[i_fnew])     // we need to unlink it
                link[i_f] += link[i_fnew];
            i_f = i_fnew;

            f = 3 + 2 * i_f;
            n = p * f; //(2*i_p+3)*(2*i_f+3)
        }
    }

    for (uint64_t i = i_p; i <= i_max; i++)
        if (!mark[i]) { numPrimes++; AddPrime(3 + 2 * i); }

    return numPrimes;
}

/*
// step over evens - only indexes but p
uint64_t Mairson_Optim5(const uint64_t Nmax)
{
    uint64_t i_max = (Nmax - 3) / 2;
    uint64_t i_p_max = ((uint64_t)floor(sqrt(Nmax)) - 3) / 2;
    unsigned numPrimes = 1; //account for 2

    for (uint64_t i = 0; i <= i_max; i++) { link[i] = 1; mark[i] = false; }

    uint64_t i_p, p, i_f;

    for (i_p = 0, p = 3; i_p <= i_p_max; i_p += link[i_p], p = 2 * i_p + 3)
    {
        numPrimes++;

        i_f = i_p;
        //f = p;  // = 2 * i_p + 3
        //n = p * f; // = 4 * i_p * i_p + 12 * i_p + 9

        uint64_t ii_n = 2 * i_p * i_p + 6 * i_p + 3; // = (n-3)/2

        while (ii_n <= i_max)
        {
            //i_n = (n - 3) / 2;
            //assert(i_n == ii_n);
            mark[ii_n] = true;

            uint64_t i_fnew = i_f + link[i_f];
            if (mark[i_fnew])     // we need to unlink it
                link[i_f] += link[i_fnew];
            //f = 3 + 2 * i_fnew;
            i_f = i_fnew;
            //n = p * f; //(2*i_p+3)*(2*i_f+3)

            //ii_n = ((2 * i_p + 3) * (2 * i_f + 3) - 3) / 2;
            ii_n = (p * (2 * i_f + 3) - 3) / 2;
        }
    }

    for (uint64_t i = i_p; i <= i_max; i++)
    {
        if (!mark[i])
        {
            p = 3 + 2 * i;
            numPrimes++;
        }
    }

    return numPrimes;
}

// step over evens - only indexes
uint64_t Mairson_Optim6(const uint64_t Nmax)
{
    uint64_t i_max = (Nmax - 3) / 2;
    uint64_t i_p_max = ((uint64_t)floor(sqrt(Nmax)) - 3) / 2;
    unsigned numPrimes = 1; //account for 2

    for (uint64_t i = 0; i <= i_max; i++) { link[i] = 1; mark[i] = false; }

    uint64_t p, i_p, i_f, i_n;

    auto Set_i_n = [&](void) { i_n = 2 * i_p * i_f + 3 * (i_p + i_f) + 3; };

    for (i_p = 0; i_p <= i_p_max; i_p += link[i_p])
    {
        numPrimes++;

        i_f = i_p;
        
        for (Set_i_n(); i_n <= i_max; Set_i_n())
        {
            mark[i_n] = true;

            uint64_t i_fnew = i_f + link[i_f];
            if (mark[i_fnew])     // we need to unlink it
                link[i_f] += link[i_fnew];
            i_f = i_fnew;
        }
    }

    for (uint64_t i = i_p; i <= i_max; i++)
    {
        if (!mark[i])
        {
            p = 3 + 2 * i;
            numPrimes++;
        }
    }

    return numPrimes;
}

// step over 2, 3
uint64_t Mairson_Optim7(const uint64_t Nmax)
{
    uint64_t i_max = (Nmax - 5) / 2;
    uint64_t i_p_max = ((uint64_t)floor(sqrt(Nmax)) - 5) / 2;
    unsigned numPrimes = 2; //account for 2 and 3

    for (uint64_t i = 0; i <= i_max; i+=3) 
    { 
        link[i+0] = 1; mark[i+0] = false;
        link[i+1] = 2; mark[i+1] = false;
        link[i+2] = 1; mark[i+2] = true;
    }

    uint64_t p, i_p, i_f, i_n;

    auto Get_idx2no = [&](uint64_t idx) { return 2 * idx + 5; };

    auto Set_i_n = [&](void) { i_n = 2 * i_p * i_f + 5 * (i_p + i_f) + 10; };

    for (i_p = 0; i_p <= i_p_max; i_p += link[i_p])
    {
        numPrimes++;

        //p = Get_idx2no(i_p);

        i_f = i_p;

        for (Set_i_n(); i_n <= i_max; Set_i_n())
        {
            //f = Get_idx2no(i_f);
            //n = Get_idx2no(i_n);

            mark[i_n] = true;

            uint64_t i_fnew = i_f + link[i_f];
            if (mark[i_fnew])     // we need to unlink it
                link[i_f] += link[i_fnew];
            i_f = i_fnew;
        }
    }

    for (uint64_t i = i_p; i <= i_max; i++)
    {
        if (!mark[i])
        {
            p = 5 + 2 * i;
            numPrimes++;
        }
    }

    return numPrimes;
}
*/

// step over 2, 3, no gaps, 
uint64_t Mairson_Optim8(const uint64_t Nmax, uint16_t link[], bool mark[], void*) 
{
    auto idx2no = [](uint64_t idx) { return 3 * idx + 5 - (idx & 1); };
    auto no2idx = [](uint64_t no) { return no / 3 - 1; };

    uint64_t i_max = no2idx(Nmax);
    uint64_t i_p_max = no2idx((uint64_t)floor(sqrt(Nmax)));
    unsigned numPrimes = 2; //account for 2 and 3
    AddPrime(2); AddPrime(3);

    uint64_t i;
    for (i = 0; i <= i_max; i++) { mark[i] = false; link[i] = 1; }

    uint64_t p, i_p, f, i_f, n, i_n;
    for (i_f = i_p = 0; i_p <= i_p_max; i_f = i_p += link[i_p])
    {
        f = p = idx2no(i_p);

        numPrimes++; AddPrime(p);

        for (n = f * p; n <= Nmax; n = f * p)
        {
            i_n = no2idx(n);
            mark[i_n] = true;

            uint64_t i_fnew = i_f + link[i_f];
            if (mark[i_fnew])     // we need to unlink it
                link[i_f] += link[i_fnew];
            i_f = i_fnew;
            f = idx2no(i_f);
        }
    }

    for (i = i_p; i <= i_max; i++)
        if (!mark[i])
            if ((p = idx2no(i)) <= Nmax)
                { numPrimes++; AddPrime(p); }
            else
                break;

    //uint64_t ldmx=0, dmx = 0;
    //for (i = 0; i < Nmax/15; i++)
    //{
    //    if ((link[i])>dmx)
    //    {
    //        dmx = link[i];
    //        ldmx = i;
    //    }
    //}
    //std::cout << "dmx " << dmx << " at " << idx2no(ldmx); nln();

    return numPrimes;
}

// step over 2, 3, no gaps, 8bit
uint64_t Mairson_Optim9(const uint64_t Nmax, uint8_t next[], bool mark[], void*)
{
    auto idx2no = [](uint64_t idx) { return 3 * idx + 5 - (idx & 1); };
    auto no2idx = [](uint64_t no) { return no / 3 - 1; };

    uint64_t i_max = no2idx(Nmax);
    uint64_t i_p_max = no2idx((uint64_t)floor(sqrt(Nmax)));
    unsigned numPrimes = 3; //account for 2 and 3 and 5
    AddPrime(2); AddPrime(3); AddPrime(5);

    uint64_t i;
    for (i = 0; i <= Nmax / 21; i++) { next[i] = 1; mark[i] = false; }
    for (; i <= i_max; i++) { mark[i] = false; }

    uint64_t p, i_p, f, i_f, n, i_n;
    //hard-code 5
    f = p = 5; n = f * p;
    for (uint8_t step = 2; n <= Nmax; step = 6 - step)
    {
        i_n = no2idx(n);
        mark[i_n] = true;
        f += step;
        n = f * p;
    }
    //start from 7
    for (i_f = i_p = 1; i_p <= i_p_max; i_f = (i_p += next[i_p]))
    {
        f = p = idx2no(i_p);
        numPrimes++; AddPrime(p);

        for (n = f * p; n <= Nmax; n = f * p)
        {
            i_n = no2idx(n);
            mark[i_n] = true;

            uint64_t i_fnew = i_f + next[i_f];
            if (mark[i_fnew])     // we need to unlink it
                next[i_f] += next[i_fnew];
            i_f = i_fnew;
            f = idx2no(i_f);
        }
    }
    
    for (; i_p <= i_max; i_p++)
        if (!mark[i_p])
            if ((p = idx2no(i_p)) <= Nmax)
                { numPrimes++; AddPrime(p); }
            else
                break;

    return numPrimes;
}

