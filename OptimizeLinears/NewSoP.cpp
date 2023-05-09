#include "..\Helper\Helper.h"

uint16_t firstPrimes[] = {
		 2,         3,         5,         7,        11,        13,        17,        19,
		23,        29,        31,        37,        41,        43,        47,        53,
		59,        61,        67,        71,        73,        79,        83,        89,
		97,       101,       103,       107,       109,       113,       127,       131,
	   137,       139,       149,       151,       157,       163,       167,       173,
	   179,       181,       191,       193,       197,       199,       211,       223,
	   227,       229,       233,       239,       241,       251,       257,       263,
	   269,       271,       277,       281,       283,       293,       307,       311,
	   313,       317,       331,       337,       347,       349,       353,       359,
	   367,       373,       379,       383,       389,       397,       401,       409,
	   419,       421,       431,       433,       439,       443,       449,       457,
	   461,       463,       467,       479,       487,       491,       499,       503,
	   509,       521,       523,       541,       547,       557,       563,       569,
	   571,       577,       587,       593,       599,       601,       607,       613,
	   617,       619,       631,       641,       643,       647,       653,       659,
	   661,       673,       677,       683,       691,       701,       709,       719,
	   727,       733,       739,       743,       751,       757,       761,       769,
	   773,       787,       797,       809,       811,       821,       823,       827,
	   829,       839,       853,       857,       859,       863,       877,       881,
	   883,       887,       907,       911,       919,       929,       937,       941,
	   947,       953,       967,       971,       977,       983,       991,       997 };

			//			-- Primorials --
			// P( 2) =							6	 5
			// P( 3) =						   30	 7
			// P( 4) =						  210	11
			// P( 5) =						2,310	13
			// P( 6) =					   30,030	17
			// P( 7) =			          510,510	19
			// P( 8) =					9,699,690	23
			// P( 9) =				  223,092,870	29
			// P(10) =			    6,469,693,230	31
			// P(11) =		      200,560,490,130	37
			// P(12) =		    7,420,738,134,810
			// P(13) =        304,250,263,527,210
			// P(14) =	   13,082,761,331,670,030
			// P(15) =	  614,889,782,588,491,410
			// P(16) = 14,142,414,403,480,493,114



//BAD CODE
/*
uint64_t v[100'000];

uint64_t NewSoP1(const uint64_t N, uint64_t v1[], void*, void*)
{
	//uint64_t primorial = 1;
	//for (int i = 0; i < sizeof(firstPrimes)/ sizeof(firstPrimes[0]); )
	//{
	//	primorial *= firstPrimes[i++];
	//	std::cout << "P(" << i << ") = " << primorial <<std ::endl;
	//}
	//return 0;

	uint64_t  numPrimes = 2; 
	AddPrime(2); AddPrime(3); //AddPrime(5);
	v[0] = 2; v[1] = 3; v[2] = 5;

	uint64_t lastprimeidx = 2, lengthWk = 6;

	//W0 = {} , L0 = 1; W1 = {1}, L1 = 2; W

	auto ExtendWheel = [&](uint64_t k, uint64_t pk)
	{
		uint64_t baselen = lengthWk, p;

		//roll pk-1 times
		for (uint64_t i = 1; i < pk; i++)
		{
			//add for 1
			p = baselen + 1;
			//if (p % pk)
			{
				v[++lastprimeidx] = baselen + 1;
			}
			//add for the rest
			for (uint64_t j = k; v[j] < lengthWk; j++)
			{
				p = baselen + v[j];
				//if (p % pk)
				{
					v[++lastprimeidx] = baselen + v[j];
				}
			}
			baselen += lengthWk;
		}
		lengthWk = baselen;
	};



	for (uint64_t k = 2; (v[k] * lengthWk) < N; k++)
	{
		// get new (k+1)th prime 
		uint64_t newp = v[k]; numPrimes++; AddPrime(newp);

		ExtendWheel(k, newp);
	}

	return numPrimes;
}
*/