#include "..\Helper\Helper.h"

uint8_t s[10'000'005], flags[10'000'005];

uint64_t SoP(const uint64_t N, uint64_t s[], void*, void*)
{
    for (uint64_t i = 0; i <= N; i++) s[i] = 0;

    uint64_t p, maxS = 1, length = 2;

    auto next = [&](uint64_t w) { return s[w]; };
    auto prev = [&](uint64_t w) { return s[w - 1]; };
    auto Append = [&](uint64_t w)
        { s[maxS] = w; s[w - 1] = maxS; maxS = w; };
    auto Delete = [&](uint64_t pf)
    {
        uint64_t temp1 = s[pf - 1], temp2 = s[pf];
        s[temp1] = temp2; s[temp2 - 1] = temp1;
    };
    auto ExtendTo = [&](uint64_t n)
    {
        uint64_t w = 1, x = length + 1;

        while (x <= n)
        {
            Append(x);
            w = next(w);
            x = length + w;
        }
        length = n;
        if (length == N)
            Append(N + 2);
    };
    auto DeleteMultiples = [&](uint64_t p)
    {
        uint64_t f = p;

        while (p * f <= length)
            f = next(f);
        while (f > 1)
        {
            f = prev(f);
            Delete(p * f);
        }
    };

    for (p = 3; p * p <= N; p = next(1))
    {
        if (length < N)
            ExtendTo(std::min(p * length, N));
        DeleteMultiples(p);
    }
    if (length < N)
        ExtendTo(N);

    //get the primes
    uint64_t  numPrimes = 1;  AddPrime(2);
    
    for (p = 3; p <= N; p = next(p))
    {
        numPrimes++; AddPrime(p);
    }
    return numPrimes;
}

uint64_t na, nd;

// start from 5, whiles into fors
uint64_t SoP1(const uint64_t N, uint64_t s[], void*, void*)
{
    na = nd = 0;

    for (uint64_t i = 0; i <= N; i++) s[i] = 0;

    uint64_t maxS = 1, length = 6;

    auto next = [&](uint64_t w) { return s[w]; };
    auto prev = [&](uint64_t w) { return s[w - 1]; };
    auto Append = [&](uint64_t w)
    { s[maxS] = w; s[w - 1] = maxS; maxS = w; na++; };
    auto Delete = [&](uint64_t pf)
    {
        uint64_t temp1 = s[pf - 1], temp2 = s[pf];
        s[temp1] = temp2; s[temp2 - 1] = temp1; nd++;
    };
    auto ExtendTo = [&](uint64_t n, uint64_t p = 0)
    {
        uint64_t x, w = 1;

        for (x = length + w; x <= n; x = length + w)
        { Append(x); w = next(w); }
        length = n;
        if (length == N)
            Append(N + 2);
    };
    auto DeleteMultiples = [&](uint64_t p)
    {
        uint64_t f = p;

        while (p * f <= length)
            f = next(f);
        while (f > 1)
        {
            f = prev(f);
            Delete(p * f);
        }
    };

    Append(3); Append(5); Delete(3);
    uint64_t  p, numPrimes = 2;  AddPrime(2); AddPrime(3);

    for (p = 5; p * p <= N; p = next(1))
    {
        numPrimes++; AddPrime(p);
        if (length < N)
            ExtendTo(std::min(p * length, N), p);
        DeleteMultiples(p);
    }

    std::cout << na << " : " << nd;

    for (; p <= N; p = next(p))
        { numPrimes++; AddPrime(p); }
    return numPrimes;
}

// delay delete
uint64_t SoP2(const uint64_t N, uint64_t s[], uint8_t mark[], void*)
{
    na = nd = 0;

    auto SetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; uint8_t bit = (uint8_t)(bitidx % 8);
        mark[idx] |= BIT_MASK[bit];
    };    
    auto ResetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; uint8_t bit = (uint8_t)(bitidx % 8);
        mark[idx] &= BIT_RESET_MASK[bit];
    };
    auto GetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; uint8_t bit = (uint8_t)(bitidx % 8);
        return (mark[idx] & BIT_MASK[bit]);
    };
    for (uint64_t i = 0; i <= N/8; i++) s[i] = 0;   // none is prime

    uint64_t maxS = 1, length = 6;

    auto Delete = [&](uint64_t pf) { ResetBit(pf); nd++; };
    auto next = [&](uint64_t w)
    {
        uint64_t next_w = s[w];
        while(!GetBit(next_w)) next_w = s[next_w];
        return (s[w] = next_w);
    };
    auto DeleteMultiples = [&](uint64_t p)
    {
        for(uint64_t f = 1; p * f <= length; f = s[f])
            Delete(p * f);
    };

    auto Append = [&](uint64_t w)
    { 
        na++; SetBit(w);    // prime candidate
        s[maxS] = w; maxS = w; 
    };

    auto ExtendTo = [&](uint64_t n, uint64_t p = 0)
    {
        uint64_t x, w = 1;
        //uint64_t base = length;
        //for (int i = 1; i < p; i++)
        //{
        //    for (uint64_t w = 1; w < length; w = next(w))
        //    {
        //        x = base + w;
        //        //if(x % p)
        //            Append(x);
        //    }
        //    base += length;
        //}
        for (x = length + w; x <= n; x = length + w)
        { 
            Append(x);
            w = next(w);
        }
        length = n;
        if (length == N)
            Append(N + 2);
    };

    Append(3); Append(5); Delete(3);
    uint64_t  p, numPrimes = 2;  AddPrime(2); AddPrime(3);

    for (p = 5; p * length <= N; p = next(1))
    {
        ExtendTo(p * length, p);
        numPrimes++; AddPrime(p);
        DeleteMultiples(p);
    }
    if (length < N)
        ExtendTo(N, p);
    for (; p * p <= N; p = next(1))
    {
        numPrimes++; AddPrime(p);
        DeleteMultiples(p);
    }

    std::cout << na << " : " << nd;

    for (; p <= N; p = next(p))
    {
        numPrimes++; AddPrime(p);
    }
    return numPrimes;
}

// start from p, not from 1
uint64_t SoP3(const uint64_t N, uint64_t s[], uint8_t mark[], void*)
{
    na = nd = 0;

    auto SetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; uint8_t bit = (uint8_t)(bitidx % 8);
        mark[idx] |= BIT_MASK[bit];
    };
    auto ResetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; uint8_t bit = (uint8_t)(bitidx % 8);
        mark[idx] &= BIT_RESET_MASK[bit];
    };
    auto GetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; uint8_t bit = (uint8_t)(bitidx % 8);
        return (mark[idx] & BIT_MASK[bit]);
    };
    for (uint64_t i = 0; i <= N / 8; i++) s[i] = 0;   // none is prime

    uint64_t maxS = 1, length = 6;

    auto Delete = [&](uint64_t pf) { ResetBit(pf); nd++; };
    auto next = [&](uint64_t w)
    {
        uint64_t next_w = s[w];
        while (!GetBit(next_w)) next_w = s[next_w];
        return (s[w] = next_w);
    };
    auto DeleteMultiples = [&](uint64_t p)
    {
        for (uint64_t f = p; p * f <= length; f = s[f])
            Delete(p * f);
    };

    auto Append = [&](uint64_t w)
    {
        na++; SetBit(w);    // prime candidate
        s[maxS] = w; maxS = w;
    };

    auto ExtendTo = [&](uint64_t n, uint64_t p)
    {
        Append(length + 1);
        uint64_t x, w = p;
        //uint64_t base = length;
        //for (int i = 1; i < p; i++)
        //{
        //    for (uint64_t w = 1; w < length; w = next(w))
        //    {
        //        x = base + w;
        //        //if(x % p)
        //            Append(x);
        //    }
        //    base += length;
        //}
        for (x = length + w; x <= n; x = length + w)
        {
            Append(x);
            w = next(w);
        }
        length = n;
        if (length == N)
            Append(N + 2);
    };

    Append(3); Append(5); Delete(3);
    uint64_t  p, numPrimes = 2;  AddPrime(2); AddPrime(3);

    for (p = 5; p * length <= N; p = next(p))
    {
        ExtendTo(p * length, p);
        numPrimes++; AddPrime(p);
        DeleteMultiples(p);
    }
    if (length < N)
        ExtendTo(N, p);
    std::cout << na << " : " << nd<<" - ";
    for (; p * p <= N; p = next(p))
    {
        numPrimes++; AddPrime(p);
        DeleteMultiples(p);
    }

    std::cout  << nd;

    for (; p <= N; p = next(p))
    {
        numPrimes++; AddPrime(p);
    }
    return numPrimes;
}

// gaps instead absolutes
uint64_t SoP4(const uint64_t N, uint8_t s[], uint8_t flags[], void*)
{
    na = nd = 0;

    uint64_t i_maxS = 0, length = 6;
    for (uint64_t i = 0; i <= N / 24; i++) s[i] = 0;   // none is prime

    auto SetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; uint8_t bit = (uint8_t)(bitidx % 8);
        flags[idx] |= BIT_MASK[bit];
    };
    auto ResetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; uint8_t bit = (uint8_t)(bitidx % 8);
        flags[idx] &= BIT_RESET_MASK[bit];
    };
    auto GetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; uint8_t bit = (uint8_t)(bitidx % 8);
        return (flags[idx] & BIT_MASK[bit]);
    };

    auto Delete = [&](uint64_t pf) { ResetBit(no2idx(pf)); nd++;  };
    auto next = [&](uint64_t w)
    {
        uint64_t i_w = no2idx(w);
        uint64_t next_i_w = i_w + s[i_w];
        while (!GetBit(next_i_w)) next_i_w += s[next_i_w];
        s[i_w] = next_i_w - i_w;
        return (idx2no(next_i_w));
    };
    auto DeleteMultiples = [&](uint64_t p)
    {
        uint64_t i_f = no2idx(p);
        for (uint64_t f = p; p * f <= length; f = idx2no(i_f))
        {
            Delete(p * f);
            i_f += s[i_f];
        }
    };
    auto Append = [&](uint64_t w)
    {
        uint64_t i_w = no2idx(w);
        na++; SetBit(i_w);    // prime candidate
        s[i_maxS] = i_w - i_maxS; i_maxS = i_w;
    };
    auto ExtendTo = [&](uint64_t n, uint64_t p)
    {
        Append(length + 1);
        uint64_t x, w = p;
        //uint64_t base = length;
        //for (int i = 1; i < p; i++)
        //{
        //    for (uint64_t w = 1; w < length; w = next(w))
        //    {
        //        x = base + w;
        //        //if(x % p)
        //            Append(x);
        //    }
        //    base += length;
        //}
        for (x = length + w; x <= n; x = length + w)
        {
            Append(x);
            w = next(w);
        }
        length = n;
        if (length == N)
            Append(N + 2);
    };

    uint64_t  p, numPrimes = 2;  
    Append(5); AddPrime(2); AddPrime(3);

    for (p = 5; p * length <= N; p = next(p))
    {
        ExtendTo(p * length, p);
        numPrimes++; AddPrime(p);
        DeleteMultiples(p);
    }
    if (length < N)
        ExtendTo(N, p);
    for (; p * p <= N; p = next(p))
    {
        numPrimes++; AddPrime(p);
        DeleteMultiples(p);
    }

    std::cout << na << " : " << nd;

    for (; p <= N; p = next(p))
    {
        numPrimes++; AddPrime(p);
    }
    return numPrimes;
}

// better delete
uint64_t SoP5(const uint64_t N, uint8_t s[], uint8_t flags[], void*)
{
    cTimer tmr;
    tmr.Start();

    uint64_t max_i1 = 0, max_i2 = 0, max_q = 0;
    na = nd = 0;

    auto SetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; uint8_t bit = (uint8_t)(bitidx % 8);
        flags[idx] |= BIT_MASK[bit];
    };
    auto ResetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; uint8_t bit = (uint8_t)(bitidx % 8);
        flags[idx] &= BIT_RESET_MASK[bit];
    };
    auto GetBit = [&](uint64_t bitidx)
    {
        uint64_t idx = bitidx / 8; uint8_t bit = (uint8_t)(bitidx % 8);
        return (flags[idx] & BIT_MASK[bit]);
    };

    uint64_t i, i_maxS = 0, length = 6;
    const uint64_t i_max = N / 24;
    for (i = 0; i <= i_max; i++) flags[i] = 0;   // none is prime

    auto Delete = [&](uint64_t pf) { ResetBit(no2idx(pf)); /*nd++;*/  };
    auto next = [&](uint64_t w)
    {
        uint64_t i_w = no2idx(w);
        uint64_t next_i_w = i_w + s[i_w];
        if (!GetBit(next_i_w))
        {
            next_i_w += s[next_i_w];
            s[i_w] = next_i_w - i_w;
        }

        if (i_w > max_i1) max_i1 = i_w;
        return (idx2no(next_i_w));
    };
    auto next_simple = [&](uint64_t w)
    {
        uint64_t i_w = no2idx(w);
        uint64_t next_i_w = i_w + s[i_w];
        return (idx2no(next_i_w));
    };
    auto DeleteMultiples = [&](uint64_t p)
    {
        uint64_t i_f = no2idx(p);
        for (uint64_t f = p; p * f <= length; f = idx2no(i_f))
        {
            Delete(p * f);
            i_f += s[i_f];
            if (i_f > max_i2) max_i2 = i_f;
        }
    };
    auto Append = [&](uint64_t w)
    {
        uint64_t i_w = no2idx(w);
        /*na++;*/ SetBit(i_w);    // prime candidate
        s[i_maxS] = i_w - i_maxS; i_maxS = i_w;
    };
    auto Append_simple = [&](uint64_t w)
    {
        uint64_t i_w = no2idx(w);
        /*na++;*/ SetBit(i_w);    // prime candidate
    };    
    auto ExtendToNextP = [&](uint64_t p)
    {
        uint64_t base = length;
        Append(base + 1);
        for (uint64_t w = p; w < length; w = next(w))
            Append(base + w);
        base += length;
        for (int i = 2; i < p; i++)
        {
            Append(base + 1);
            for (uint64_t w = p; w < length; w = next_simple(w))
                Append(base + w);
            base += length;
        }
        length = base;
    };
    auto ExtendToN = [&](uint64_t p)
    {
        uint64_t base = length;
        Append(base + 1);
        for (uint64_t w = p; w < length; w = next(w))
            if (base + w <= N)
                Append_simple(base + w);
            else
                break;
        base += length;        
        for (int i = 2; i < p and base < N; i++)
        {
            Append_simple(base + 1);
            for (uint64_t w = p; w < length; w = next_simple(w))
                if (base + w <= N)
                    Append_simple(base + w);
                else
                    break;
            base += length;
        }

        length = N;
    };

    tmr.LapTime(true, "zeroing"); nln();

    uint64_t  p, q, numPrimes = 2;
    Append(5); AddPrime(2); AddPrime(3);

    for (p = 5; p * length <= N; p = next(p))
    {
        ExtendToNextP(p);
        DeleteMultiples(p);
        numPrimes++; AddPrime(p);
    }
    tmr.LapTime(true, "wheeling");
    std::cout << p << " : " << max_i1 << " - " << max_i2; nln();
    if (length < N)
    {
        ExtendToN(p);
        DeleteMultiples(p);
        numPrimes++; AddPrime(p);
        p = next(p);
    }
    tmr.LapTime(true, "extending"); 
    std::cout << p << " : " << max_i1 << " - " << max_i2; nln();
    uint64_t nsqrt = (uint64_t)floor(sqrt(N));
    for (; p <= nsqrt; p = next(p))
    {
        numPrimes++; AddPrime(p); q = p; 
        for (uint64_t pq = p * q; pq <= N; pq = p * q)
        {
            for (uint64_t n = pq; n <= N; n *= p)
                Delete(n);
            q = next(q);
            if (q > max_q) max_q = q;
        }
    }
    tmr.LapTime(true, "sieving");
    std::cout << na << " : " << nd << " -- " << max_i1 << " - " << max_i2 << " q " << q; nln();

    //for (; p <= N; p = next(p))
    //{
    //    numPrimes++; AddPrime(p);
    //}

    //uint64_t i_p_max = no2idx(N);
    //for (uint64_t i_p = no2idx(p); i_p <= i_p_max; i_p++)
    //    if (GetBit(i_p))
    //    {
    //        p = idx2no(i_p);
    //        if (p <= N)
    //        {
    //            numPrimes++; AddPrime(p);
    //        }
    //        else
    //            break;
    //    }

    i = (p - 5) / 24;
    uint64_t pbase = (i * 24) + 5;
    uint8_t step, b = (uint8_t)(p - pbase);
    switch (b)
    {
    case 0: b = 0; step = 2; break; case 12: b = 4; step = 2; break;
    case 2: b = 1; step = 4; break; case 14: b = 5; step = 4; break;
    case 6: b = 2; step = 2; break; case 18: b = 6; step = 2; break;
    case 8: b = 3; step = 4; break; case 20: b = 7; step = 4; break;
    default: assert(false);
    }
    uint8_t flgs = flags[i];
    assert(flgs & BIT_MASK[b]);
    for (; b < 8; b++)
    {
        if (flgs & BIT_MASK[b]) { numPrimes++; AddPrime(p); }
        p += step; step = 6 - step;
    }
    for (i++; i < i_max - 1; i++)
    {
        flgs = flags[i];
        if (flgs & BIT_MASK[0]) { numPrimes++; AddPrime(p); }
        if (flgs & BIT_MASK[1]) { numPrimes++; AddPrime(p + 2); }
        if (flgs & BIT_MASK[2]) { numPrimes++; AddPrime(p + 6); }
        if (flgs & BIT_MASK[3]) { numPrimes++; AddPrime(p + 8); }
        if (flgs & BIT_MASK[4]) { numPrimes++; AddPrime(p + 12); }
        if (flgs & BIT_MASK[5]) { numPrimes++; AddPrime(p + 14); }
        if (flgs & BIT_MASK[6]) { numPrimes++; AddPrime(p + 18); }
        if (flgs & BIT_MASK[7]) { numPrimes++; AddPrime(p + 20); }
        p += 24;
    }
    while (p <= N)
    {
        flgs = flags[i++];
        for (b = 0; b < 8; b++)
        {
            if (p > N) break;
            if (flgs & BIT_MASK[b]) { numPrimes++; AddPrime(p); }
            p += step; step = 6 - step;
        }
    }

    tmr.LapTime(true, "counting"); std::cout << i_maxS; nln();

    tmr.Stop();
    return numPrimes;
}



//uint64_t SoPx2(const uint64_t N, uint64_t s1[], void*, void*)
//
//{
//    na = nd = 0;
//    for (uint64_t i = 0; i <= N; i++) s[i] = 0;
//
//    uint64_t maxS = 1, length = 6;
//
//    auto next = [&](uint64_t w) { return s[w]; };
//    auto prev = [&](uint64_t w) { return s[w - 1]; };
//    auto Append = [&](uint64_t w)
//    { s[maxS] = w; s[w - 1] = maxS; maxS = w; na++; };
//    auto Delete = [&](uint64_t pf)
//    {
//        if (s[pf] == 0) return;
//        if (pf < maxS)
//        {
//            uint64_t prevp = s[pf - 1], nextp = s[pf];
//            s[prevp] = nextp; s[nextp - 1] = prevp; 
//        }
//        else
//        {
//            maxS = s[pf - 1];
//            s[maxS] = 0;
//        }
//        s[pf] = 0;
//        nd++;
//    };
//    auto ExtendTo = [&](uint64_t n, uint64_t p = 0)
//    {
//        uint64_t x, w = 1;
//        uint64_t base = length;
//        for (int i = 1; i < p; i++)
//        {
//            for (uint64_t w = 1; w < length; w = next(w))
//            {
//                x = base + w;
//                //if((p == 0) or (x % p))
//                    Append(x);
//            }
//            base += length;
//        }
//
//        //for (x = length + w; x <= n; x = length + w)
//        //{
//        //    //if((p == 0) or (x % p))
//        //    Append(x);
//        //    w = next(w);
//        //}
//        length = n;
//        if (length == N)
//            Append(N + 2);
//    };
//    auto DeleteMultiples = [&](uint64_t p)
//    {
//        uint64_t f = p;
//
//        for (f = p; p * next(f) <= length; f = next(f));
//        for (; f > 1; f = prev(f))
//        {
//            Delete(p * f);
//        }
//    };
//
//    Append(3); Append(5); Delete(3);
//    uint64_t  p, numPrimes = 2;  AddPrime(2); AddPrime(3);
//
//    const uint64_t nsqrt = (uint64_t)floor(sqrt(N));
//    for (p = 5; p < nsqrt; p = next(1))
//    {
//        numPrimes++; AddPrime(p);
//        if (length < N)
//            ExtendTo(std::min(p * length, N), p);
//        //DeleteMultiples(p); 
//        for(uint64_t p1 = p; p1 * p1 < length; p1 = next(p1))
//        {
//            DeleteMultiples(p1);
//        }
//        Delete(p);
//    }
//    if (length < N)
//        ExtendTo(N);
//
//    std::cout << na << " : " << nd << std::endl;
//
//    for (; p <= N; p = next(p))
//    {
//        numPrimes++; AddPrime(p);
//    }
//    return numPrimes;
//}
