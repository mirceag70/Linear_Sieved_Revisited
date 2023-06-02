#define ITERATIONS	5

#include "..\Helper\Helper.h"

typedef uint64_t tpUnsigned;
//typedef uint32_t tpUnsigned;

uint64_t Mairson_Standard(const uint64_t Nmax, tpUnsigned rlink[], tpUnsigned llink[], tpUnsigned DELETE[]);
uint64_t Mairson_SingleLink(const uint64_t Nmax, tpUnsigned rlink[], bool bDELETE[], void*);
uint64_t Mairson_Optim1(const uint64_t Nmax, uint16_t link[], bool mark[], void*);
uint64_t Mairson_Optim4(const uint64_t Nmax, uint16_t link[], bool mark[], void*);
uint64_t Mairson_Optim8(const uint64_t Nmax, uint16_t link[], bool mark[], void*);
uint64_t Mairson_Optim9(const uint64_t Nmax, uint8_t link[], bool mark[], void*);
uint64_t Mairson_Optim10(const uint64_t Nmax, uint8_t link[], uint8_t mark[], void*);

uint64_t GriesMisra_Standard(uint64_t Nmax, uint64_t rlink[], uint64_t llink[], void*);
uint64_t GriesMisra_Optim1(uint64_t Nmax, uint8_t next[], bool mark[], void*);
uint64_t GriesMisra_Optim2(uint64_t Nmax, uint8_t next[], bool mark[], void*);
uint64_t GriesMisra_Optim3(uint64_t Nmax, uint8_t next[], uint8_t mark[], void*);
uint64_t GriesMisra_Optim4(const uint64_t Nmax, uint8_t mark[], void*, void*);

uint64_t Eratosthenes_6k(const uint64_t Nmax, uint8_t vPrimes[], void*, void*);

uint64_t GalePratt1(uint64_t Nmax, bool C[], void*, void*);
uint64_t GalePratt3(uint64_t Nmax, bool C[], uint64_t FG[], void*);

uint64_t GalePratt2(uint64_t Nmax, bool sv[], void*, void*);
uint64_t GalePratt4(uint64_t Nmax, uint8_t sv[], void*, void*);
uint64_t GalePratt5(uint64_t Nmax, uint8_t mark[], uint64_t v1[], uint64_t v2[]);
uint64_t GalePratt6(uint64_t Nmax, uint8_t mark[], uint64_t v1[], uint64_t v2[]);
uint64_t GalePratt7(uint64_t Nmax, uint8_t mark[], uint32_t v1[], uint32_t v2[]);

tpPrime Ben_Std(uint64_t Nmax, uint64_t p[], uint64_t lpf[], void*);
tpPrime Ben_Opt1(uint64_t Nmax, uint64_t p[], uint64_t lpf[], uint64_t qlpf[]);
tpPrime Ben_Opt2(uint64_t Nmax, uint64_t p[], uint64_t lpf[], void*);
tpPrime Ben_Opt3(uint64_t Nmax, uint64_t lpf[], void*, void*);

uint64_t SoP(const uint64_t N, uint64_t s[], void*, void*);
uint64_t SoP1(const uint64_t N, uint64_t s[], void*, void*);
uint64_t SoP2(const uint64_t N, uint64_t s[], uint8_t mark[], void*);
uint64_t SoP3(const uint64_t N, uint64_t s[], uint8_t mark[], void*);
uint64_t SoP4(const uint64_t N, uint8_t s[], uint8_t mark[], void*);
uint64_t SoP5(const uint64_t N, uint8_t s[], uint8_t mark[], void*);

constexpr uint64_t LIMIT = 10'000'000;

uint64_t idx2no30_armtc(uint64_t idx)
{
    uint64_t k = idx / 8;   uint64_t i = idx % 8;
    return 4 * (i + (i / 6)) + 7 - 2 * ((i / 2) + (i / 7)) + 30 * k;
}
uint64_t idx2no30_logic(uint64_t idx)
{
    uint64_t k = idx / 8;   uint64_t i = idx % 8;
    switch (i)
    {
    case 0: i =  7; break; case 1: i = 11; break;
    case 2: i = 13; break; case 3: i = 17; break;
    case 4: i = 19; break; case 5: i = 23; break;
    case 6: i = 29; break; case 7: i = 31; break;
    default: assert(false);
    }
    return i + 30 * k;
}
void testfn(void)
{
    cTimer tmr; tmr.Start();

    uint64_t c = 0;

    for (uint64_t i = 0; i < 10'000'000'000; i++)
    {
        uint64_t n = idx2no30_armtc(i);
        //uint64_t n1 = idx2no30_logic(i);
        //assert(n == n1);
        if ((n & 1) == 1)
        {
            //std::cout << "!!";
            c++;
        }
        //switch (k)
        //{
        //default: assert(false);
        //case 1: case 7: case 11: case 13:
        //case 17: case 19: case 23: case 29:
        //    ;
        //}
    }
    std::cout << c;
    tmr.Stop(true);
}

int main()
{
    std::locale mylocale("");   // get global locale 
    std::cout.imbue(mylocale);  // imbue global locale for thousands delimiter

    //testfn(); testfn(); testfn(); testfn(); testfn();
    //return 0;

    Try_Sieve<tpUnsigned, tpUnsigned, tpUnsigned, LIMIT + 1, LIMIT + 1, LIMIT + 1>
        (LIMIT, "Mairson Standard", & Mairson_Standard, false);
    Try_Sieve<tpUnsigned, bool, int, LIMIT + 1, LIMIT + 1>
        (LIMIT, "Mairson Single Link", &Mairson_SingleLink, false);
    Try_Sieve<uint16_t, bool, int, LIMIT + 1, LIMIT + 1>
        (LIMIT, "Mairson incremental gap", &Mairson_Optim1, false);
    Try_Sieve<uint16_t, bool, int, LIMIT / 2 + 1, LIMIT / 2 + 1>
        (LIMIT, "Mairson step 2", &Mairson_Optim4, false);
    Try_Sieve<uint16_t, bool, int, LIMIT / 3 + 1, LIMIT / 3 + 1>
        (LIMIT, "Mairson 6k +/- 1", & Mairson_Optim8, false);
    Try_Sieve<uint8_t, bool, int, LIMIT / 21 + 1, LIMIT / 3 + 1>
        (LIMIT, "Mairson 8bit", &Mairson_Optim9, false);
    Try_Sieve<uint8_t, uint8_t, int, LIMIT / 33 + 1, LIMIT / 24 + 1>
        (LIMIT, "Mairson 1bit", &Mairson_Optim10, false);

    Try_Sieve<uint64_t, uint64_t, int, LIMIT + 1, LIMIT + 1>
        (LIMIT, "Gries/Misra Standard", &GriesMisra_Standard, false);
    Try_Sieve<uint8_t, bool, int, LIMIT + 1, LIMIT + 1>
        (LIMIT, "Gries/Misra incremental 8 bit", &GriesMisra_Optim1, false);
    Try_Sieve<uint8_t, bool, int, LIMIT / 15 + 1, LIMIT / 3 + 1>
        (LIMIT, "Gries/Misra 6k", &GriesMisra_Optim2, false);   
    Try_Sieve<uint8_t, uint8_t, int, LIMIT / 15 + 1, LIMIT / 24 + 1>
        (LIMIT, "Gries/Misra 1bit", &GriesMisra_Optim3, false);    
    Try_Sieve<uint8_t, int, int, LIMIT / 24 + 1>
        (LIMIT, "Gries/Misra no links", &GriesMisra_Optim4, false);
    
    Try_Sieve<uint8_t, int, int, LIMIT / 24 + 1>
        (LIMIT, "SoE 6k 1bit", &Eratosthenes_6k, false);

    Try_Sieve<bool, int, int, LIMIT + 1>
        (LIMIT, "4.2 std.", &GalePratt1, false);
    Try_Sieve<bool, uint64_t, int, LIMIT + 1, LIMIT / 2>
        (LIMIT, "4.2 opt.", &GalePratt3, false);
    Try_Sieve<bool, int, int, LIMIT + 1>
        (LIMIT, "GP std. - bool", &GalePratt2, false);
    Try_Sieve<uint8_t, int, int, LIMIT / 16 + 1>
        (LIMIT, "GP std.", &GalePratt4, false);
    Try_Sieve<uint8_t, uint64_t, uint64_t, LIMIT / 16 + 1, LIMIT / 2, LIMIT / 2>
        (LIMIT, "GP vct.", &GalePratt5, false);
    Try_Sieve<uint8_t, uint64_t, uint64_t, LIMIT / 16 + 1, LIMIT / 2, LIMIT / 2>
        (LIMIT, "GP vct.6", &GalePratt6, false);
    Try_Sieve<uint8_t, uint32_t, uint32_t, LIMIT / 16 + 1, LIMIT / 2, LIMIT / 2>
        (LIMIT, "GP vct.6 - 32b", &GalePratt7, false);

    Try_Sieve<uint64_t, uint64_t, int, LIMIT + 1, 2 * LIMIT + 1>
        (LIMIT, "Ben Standard", &Ben_Std, false);   
    Try_Sieve<uint64_t, uint64_t, uint64_t, LIMIT + 1, LIMIT + 4, LIMIT + 4>
        (LIMIT, "Ben Opt 1", &Ben_Opt1, false);
    Try_Sieve<uint64_t, uint64_t, int, LIMIT + 1, LIMIT + 4>
        (LIMIT, "Ben Opt 2", &Ben_Opt2, false);
    Try_Sieve<uint64_t, int, int, LIMIT + 4>
        (LIMIT, "Ben Opt 3", &Ben_Opt3, false);

    Try_Sieve<uint64_t, int, int, LIMIT + 10>
        (LIMIT, "Pritchard std", &SoP, true);    
    Try_Sieve<uint64_t, int, int, LIMIT + 2>
        (LIMIT, "SoP 1", &SoP1, true);
    Try_Sieve<uint64_t, uint8_t, int, LIMIT + 2, LIMIT / 8 + 2>
        (LIMIT, "SoP 2", &SoP2, true);
    Try_Sieve<uint64_t, uint8_t, int, LIMIT + 2, LIMIT / 8 + 2>
        (LIMIT, "SoP 3", &SoP3, true);
    Try_Sieve<uint8_t, uint8_t, int, LIMIT + 2, LIMIT / 24 + 2>
        (LIMIT, "SoP 4", &SoP4, true);    
    Try_Sieve<uint8_t, uint8_t, int, LIMIT + 2, LIMIT / 24 + 2>
        (LIMIT, "SoP 5", &SoP5, true);
}

