#include "..\Helper\Helper.h"

tpPrime Ben_Std(uint64_t Nmax, uint64_t p[], uint64_t lpf[], void*)
{
    p[0] = lpf[0] = 0xFFFF; //idx 0 not used
    lpf[1] = lpf[2] = 0;
    uint64_t sz_lpf = 2;
    uint64_t sz_p = 0;

    for (uint64_t n = 2; n <= Nmax; n++)
    {
        if (lpf[n] == 0)
        {
            p[++sz_p] = n;
            lpf[n] = sz_p;
        }
        else
        {
            uint64_t q = n / p[lpf[n]];
            if (lpf[n] < lpf[q])
            {
                uint64_t r = lpf[n] + 1;
                lpf[q * p[r]] = r;
            }
        }
        lpf[++sz_lpf] = 0; lpf[++sz_lpf] = 1;
    }

    return sz_p;
}

uint64_t p[10001], lpf[10001];// , qlpf[10001];

tpPrime Ben_Opt1(uint64_t Nmax, uint64_t p[], uint64_t lpf[], uint64_t qlpf[])
{
    p[0] = lpf[0] = qlpf[0] = 0xFFFF; //idx 0 not used
    lpf[1] = 0;  lpf[2] = 1; p[1] = 2;
    uint64_t /*sz_lpf = 2, */sz_p = 1;
    uint64_t q, r, n, ix, lpfn;
    for (n = 3; n < Nmax; n += 4)
    { 
        lpf[n] = lpf[n + 2] = 0; 
        lpf[n + 1] = 1;  lpf[n + 3] = 1;
        //qlpf[n] = qlpf[n + 2] = 0; 
        //qlpf[n + 1] = n / 2;  qlpf[n + 3] = 2;
    }

    for (n = 3; n <= Nmax; )
    {
        lpfn = lpf[n];
        if (lpfn == 0)
        {
            p[++sz_p] = n;
            lpf[n] = sz_p;
            //AddPrime(n);
        }
        else
        {
            if (lpfn == 1)
            {
                q = n / 2;
                if ((1 < lpf[q]) and ((q * 3) <= Nmax))
                {
                    lpf[q * 3] = 2;
                    qlpf[q * 3] = q;
                }
            }
            else
            {
                //q = n / p[lpfn];
                q = qlpf[n];
                if (lpfn < lpf[q])
                {
                    r = lpfn + 1;
                    ix = q * p[r];
                    if (ix <= Nmax)
                    {
                        assert((ix % 2) == 1);
                        lpf[ix] = r;
                        qlpf[ix] = q;
                    }
                }
            }
        }
        n++;
    }
    //for (n = 2; n <= 2*Nmax; n++)
    //    if (lpf[n] == 0)
    //    {
    //        AddPrime(n);
    //    }
    return sz_p;
}

tpPrime Ben_Opt2(uint64_t Nmax, uint64_t p[], uint64_t lpf[], void*)
{
    p[0] = lpf[0] = /*qlpf[0] = */0xFFFF; //idx 0 not used
    lpf[1] = 0;  lpf[2] = 1; p[1] = 2; AddPrime(2);
    uint64_t sz_p = 1;
    uint64_t q, r, n, ix, lpfn;
    for (n = 3; n < Nmax; n += 4)
    {
        lpf[n] = lpf[n + 2] = 0;
    }
    for (n = 3, ix = 9; ix <= Nmax; n += 2, ix += 6)
    {
        lpf[ix] = 2; lpf[ix - 1] = n;
    }

    for (n = 3; n <= Nmax; n += 2)
    {
        //odd n
        lpfn = lpf[n];
        if (lpfn == 0)
        {
            p[++sz_p] = n; lpf[n] = sz_p;
            AddPrime(n);
        }
        else
        {
            q = lpf[n - 1];
            assert(q == n / p[lpfn]);
            if (lpfn < lpf[q])
            {
                r = lpfn + 1;
                ix = q * p[r];
                if (ix <= Nmax)
                {
                    lpf[ix] = r;
                    lpf[ix - 1] = q;
                }
            }
        }
    }

    return sz_p;
}

tpPrime Ben_Opt3(uint64_t Nmax, uint64_t lpf[], void*, void*)
{
    lpf[2] = 2; AddPrime(2);
    uint64_t numPrimes = 1, last_p = 2;
    uint64_t q, r, n, ix, lpfn;
    for (n = 3; n < Nmax; n += 4)
        { lpf[n] = lpf[n + 2] = 0; }
    for (n = 3, ix = 9; ix <= Nmax; n += 2, ix += 6)
        { lpf[ix] = 3; lpf[ix - 1] = n; }

    for (n = 3; n <= Nmax; n += 2)
    {
        lpfn = lpf[n];
        if (lpfn == 0)
        {
            last_p = lpf[last_p - 1] = lpf[n] = n;
            numPrimes++;  AddPrime(n);
        }
        else
        {
            q = lpf[n - 1];
            assert(q == n / lpfn);
            if (lpfn < lpf[q])
            {
                r = lpf[lpfn - 1];
                ix = q * r;
                if (ix <= Nmax)
                {
                    lpf[ix] = r;
                    lpf[ix - 1] = q;
                }
            }
        }
    }

    return numPrimes;
}
