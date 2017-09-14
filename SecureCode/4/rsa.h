#define UINT64 unsigned long long int  
struct  RsaParam
{
    UINT64    p, q;   //两个素数，不参与加密解密运算
    UINT64    f;      //f=(p-1)*(q-1)，不参与加密解密运算
    UINT64    n, e;   //公匙，n=p*q，gcd(e,f)=1
    UINT64    d;      //私匙，e*d=1 (mod f)，gcd(n,d)=1
    UINT64    s;      //块长，满足2^s<=n的最大的s，即log2(n)
} ;
//小素数表
const static long       g_PrimeTable[]=	{3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
const static long       g_PrimeCount=sizeof(g_PrimeTable) / sizeof(long);
/*const UINT64  multiplier=12747293821;
const UINT64  adder=1343545677842234541;
*/
struct PublicKey
{
	UINT64 nE;
	UINT64 nN;
};
//随机数类
class RandNumber
{
private:
    UINT64    randSeed;
public:
    RandNumber(UINT64 s=0)
    {
		if(!s)
		{
			randSeed= (UINT64)time(NULL);
		}
		else
		{
			randSeed=s;
		}
		unsigned int Rand[2];
		memcpy(Rand,&randSeed,8);
		srand(Rand[0]^Rand[1]);
	}
	UINT64    Random(UINT64 n)
	{
		unsigned int *pRand = (unsigned int *)&randSeed;
		pRand[0] = rand();
		pRand[1] = rand();
		return randSeed % n;
	}
};

class CRSASection
{
public:
	CRSASection()
	{
		m_cParament = RsaGetParam();
	}
	static UINT64 Encry(unsigned short nSorce, PublicKey &cKey)
	{
		return PowMod(nSorce, cKey.nE, cKey.nN);
	}
	unsigned short Decry(UINT64 nSorce )
	{
		UINT64 nRes = PowMod(nSorce, m_cParament.d, m_cParament.n);
		unsigned short * pRes=(unsigned short*)&(nRes);
		if(pRes[1]!=0||pRes[3]!=0||pRes[2]!=0)
		{//error
			return 0;
		}
		else
		{
			return pRes[0];
		}
	}
	PublicKey GetPublicKey()
	{
		PublicKey cTmp;
		cTmp.nE=this->m_cParament.e;
		cTmp.nN = this->m_cParament.n;
		return cTmp;
	}
private:
	RandNumber   m_cRadom;
	RsaParam    m_cParament;
	static inline UINT64 MulMod(UINT64 a, UINT64 b, UINT64 n)
	{

		return (a%n) * (b%n) % n;
	}

	static UINT64 PowMod(UINT64 base, UINT64 pow, UINT64 n)
	{
		UINT64    a=base, b=pow, c=1;
		while(b)
		{
			while(!(b & 1))
			{
				b>>=1; 
				a=MulMod(a, a, n);
			}       
			b--;
			c=MulMod(a, c, n);
		}    
		return c;
	}

	long RabinMillerKnl(UINT64 &n)
	{
		UINT64    a, q, k, v ;
		q=n - 1;
		k=0;  
		while(!(q & 1))
		{
			++k;
			q>>=1;
		}    
		a=2 + m_cRadom.Random(n - 3);   
		v=PowMod(a, q, n);    
		if(v == 1)
		{
			return 1;
		}    
		for(int j=0;j<k;j++)
		{
			unsigned int z=1;
			for(int w=0;w<j;w++)
			{
				z*=2;
			}
			if(PowMod(a, z*q, n)==n-1)
				return 1;
		}
		return 0;
	}

	long RabinMiller(UINT64 &n, long loop)
	{
		//先用小素数筛选一次，提高效率
		for(long i=0; i < g_PrimeCount; i++)
		{
			if(n % g_PrimeTable[i] == 0)
			{
				return 0;
			}
		}    //循环调用Rabin-Miller测试loop次，使得非素数通过测试的概率降为(1/4)^loop
		for(long i=0; i < loop; i++)
		{
			if(!RabinMillerKnl(n))
			{
				return 0;
			}
		}   
		return 1;
	}
	UINT64 RandomPrime(char bits)
	{
		UINT64    base;
		do
		{
			base= (unsigned long)1 << (bits - 1);   //保证最高位是1
			base+=m_cRadom.Random(base);               //再加上一个随机数
			base|=1;    //保证最低位是1，即保证是奇数
		} while(!RabinMiller(base, 30));    //进行拉宾－米勒测试30次
		return base;    //全部通过认为是素数
	}

	UINT64 Gcd(UINT64 &p, UINT64 &q)
	{
		UINT64    a=p > q ? p : q;
		UINT64    b=p < q ? p : q;
		UINT64    t;
		if(p == q)
		{
			return p;   //两数相等，最大公约数就是本身
		}
		else
		{
			while(b)    //辗转相除法，gcd(a,b)=gcd(b,a-qb)
			{
				a=a % b;
				t=a;
				a=b;
				b=t;
			}       
			return a;
		}
	}

	UINT64 Euclid(UINT64 e, UINT64 t_n)
	{
		UINT64 Max;
		memset(&Max,0xff,sizeof(Max));
		Max-=t_n;
		UINT64 i=1;

		while(1)
		{
			if(((i*t_n)+1)%e==0)
			{
				return ((i*t_n)+1)/e;
			}
			i++;
			UINT64 Tmp=(i+1)*t_n;
			if(Tmp>Max)
			{
				return 0;
			}
		}
		return 0; 
	}


	RsaParam RsaGetParam(void)
	{
		RsaParam           Rsa={ 0 };
		UINT64    t;
		Rsa.p=RandomPrime(16);          //随机生成两个素数
		Rsa.q=RandomPrime(16);
		Rsa.n=Rsa.p * Rsa.q;
		Rsa.f=(Rsa.p - 1) * (Rsa.q - 1);
		do
		{
			Rsa.e=m_cRadom.Random(65536);  
			Rsa.e|=1;                  
		} while(Gcd(Rsa.e, Rsa.f) != 1);   
		Rsa.d=Euclid(Rsa.e, Rsa.f);
		Rsa.s=0;
		t=Rsa.n >> 1;
		while(t)
		{
			Rsa.s++;                   
			t>>=1;
		}    
		return Rsa;
	}


};
