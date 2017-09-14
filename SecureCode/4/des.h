/*********************************************************************/
/*-文件名：des.h
/*-功能： 实现DES加密算法的加密解密功能    for linux
/*********************************************************************/
#define SUCCESS 1;
typedef  int INT32;
typedef  char INT8;
typedef  unsigned char  ULONG8;
typedef  unsigned short ULONG16;
typedef  unsigned long  ULONG32;
static const ULONG8 DESENCRY=11;
static const ULONG8 DESDECRY=22;

static const ULONG8  pc_first[64] = {/*初始置换IP*/
					58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,
					62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,
					57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,
					61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7 } ;

static const ULONG8 pc_last[64] = { /*逆初始置换IP-1*/
		40,8,48,16,56,24,64,32, 39,7,47,15,55,23,63,31,
		38,6,46,14,54,22,62,30, 37,5,45,13,53,21,61,29,
		36,4,44,12,52,20,60,28, 35,3,43,11,51,19,59,27,
		34,2,42,10,50,18,58,26, 33,1,41,9,49,17,57,25};

static const ULONG32  pc_by_bit[64] = { /*按位取值或赋值*/
			0x80000000L,0x40000000L,0x20000000L,0x10000000L, 0x8000000L, 
			0x4000000L, 0x2000000L, 0x1000000L, 0x800000L, 0x400000L,
			0x200000L, 0x100000L,  0x80000L, 0x40000L, 0x20000L,0x10000L, 
			0x8000L, 0x4000L, 0x2000L, 0x1000L, 0x800L, 0x400L, 0x200L,
			0x100L, 0x80L,0x40L,0x20L, 0x10L, 0x8L, 0x4L, 0x2L, 0x1L,
			0x80000000L,0x40000000L,0x20000000L,0x10000000L, 0x8000000L,
			0x4000000L, 0x2000000L, 0x1000000L, 0x800000L, 0x400000L,
			0x200000L, 0x100000L,  0x80000L, 0x40000L, 0x20000L, 0x10000L, 
			0x8000L, 0x4000L, 0x2000L, 0x1000L, 0x800L, 0x400L, 0x200L, 
			0x100L, 0x80L, 0x40L,0x20L, 0x10L, 0x8L,  0x4L, 0x2L, 0x1L,   };

static const ULONG8 des_P[32] = {/*置换运算P*/
							16,7,20,21, 29,12,28,17, 1,15,23,26,
							5,18,31,10, 2,8,24,14, 32,27,3,9,
							19,13,30,6, 22,11,4,25};
static const ULONG8 des_E[48] = {/*数据扩展*/
								32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,
								12,13,14,15,16,17,16,17,18,19,20,21,
								20,21,22,23,24,25,24,25,26,27,28,29,
								28,29,30,31,32,1 };
static const ULONG8 des_S[8][64] = {/*数据压缩*/
		{
				0xe,0x0,0x4,0xf,0xd,0x7,0x1,0x4,0x2,0xe,0xf,0x2,0xb,
				0xd,0x8,0x1,0x3,0xa,0xa,0x6,0x6,0xc,0xc,0xb,0x5,0x9,
				0x9,0x5,0x0,0x3,0x7,0x8,0x4,0xf,0x1,0xc,0xe,0x8,0x8,
				0x2,0xd,0x4,0x6,0x9,0x2,0x1,0xb,0x7,0xf,0x5,0xc,0xb,
				0x9,0x3,0x7,0xe,0x3,0xa,0xa,0x0,0x5,0x6,0x0,0xd  
		},

		{ 
				0xf,0x3,0x1,0xd,0x8,0x4,0xe,0x7,0x6,0xf,0xb,0x2,0x3,
				0x8,0x4,0xf,0x9,0xc,0x7,0x0,0x2,0x1,0xd,0xa,0xc,0x6,
				0x0,0x9,0x5,0xb,0xa,0x5,0x0,0xd,0xe,0x8,0x7,0xa,0xb,
				0x1,0xa,0x3,0x4,0xf,0xd,0x4,0x1,0x2,0x5,0xb,0x8,0x6,
				0xc,0x7,0x6,0xc,0x9,0x0,0x3,0x5,0x2,0xe,0xf,0x9
		},
		{ 
				0xa,0xd,0x0,0x7,0x9,0x0,0xe,0x9,0x6,0x3,0x3,0x4,0xf,
				0x6,0x5,0xa,0x1,0x2,0xd,0x8,0xc,0x5,0x7,0xe,0xb,0xc,
				0x4,0xb,0x2,0xf,0x8,0x1,0xd,0x1,0x6,0xa,0x4,0xd,0x9,
				0x0,0x8,0x6,0xf,0x9,0x3,0x8,0x0,0x7,0xb,0x4,0x1,0xf,
				0x2,0xe,0xc,0x3,0x5,0xb,0xa,0x5,0xe,0x2,0x7,0xc                                          
		},
		{ 
				0x7,0xd,0xd,0x8,0xe,0xb,0x3,0x5,0x0,0x6,0x6,0xf,0x9,
				0x0,0xa,0x3,0x1,0x4,0x2,0x7,0x8,0x2,0x5,0xc,0xb,0x1,
				0xc,0xa,0x4,0xe,0xf,0x9,0xa,0x3,0x6,0xf,0x9,0x0,0x0,
				0x6,0xc,0xa,0xb,0xa,0x7,0xd,0xd,0x8,0xf,0x9,0x1,0x4,
				0x3,0x5,0xe,0xb,0x5,0xc,0x2,0x7,0x8,0x2,0x4,0xe                         
		},
		{ 
				0x2,0xe,0xc,0xb,0x4,0x2,0x1,0xc,0x7,0x4,0xa,0x7,0xb,
				0xd,0x6,0x1,0x8,0x5,0x5,0x0,0x3,0xf,0xf,0xa,0xd,0x3,
				0x0,0x9,0xe,0x8,0x9,0x6,0x4,0xb,0x2,0x8,0x1,0xc,0xb,
				0x7,0xa,0x1,0xd,0xe,0x7,0x2,0x8,0xd,0xf,0x6,0x9,0xf,
				0xc,0x0,0x5,0x9,0x6,0xa,0x3,0x4,0x0,0x5,0xe,0x3
		},
		{ 
				0xc,0xa,0x1,0xf,0xa,0x4,0xf,0x2,0x9,0x7,0x2,0xc,0x6,
				0x9,0x8,0x5,0x0,0x6,0xd,0x1,0x3,0xd,0x4,0xe,0xe,0x0,
				0x7,0xb,0x5,0x3,0xb,0x8,0x9,0x4,0xe,0x3,0xf,0x2,0x5,
				0xc,0x2,0x9,0x8,0x5,0xc,0xf,0x3,0xa,0x7,0xb,0x0,0xe,
				0x4,0x1,0xa,0x7,0x1,0x6,0xd,0x0,0xb,0x8,0x6,0xd
		},
		{ 
				0x4,0xd,0xb,0x0,0x2,0xb,0xe,0x7,0xf,0x4,0x0,0x9,0x8,
				0x1,0xd,0xa,0x3,0xe,0xc,0x3,0x9,0x5,0x7,0xc,0x5,0x2,
				0xa,0xf,0x6,0x8,0x1,0x6,0x1,0x6,0x4,0xb,0xb,0xd,0xd,
				0x8,0xc,0x1,0x3,0x4,0x7,0xa,0xe,0x7,0xa,0x9,0xf,0x5,
				0x6,0x0,0x8,0xf,0x0,0xe,0x5,0x2,0x9,0x3,0x2,0xc
		},
		{ 
				0xd,0x1,0x2,0xf,0x8,0xd,0x4,0x8,0x6,0xa,0xf,0x3,0xb,
				0x7,0x1,0x4,0xa,0xc,0x9,0x5,0x3,0x6,0xe,0xb,0x5,0x0,
				0x0,0xe,0xc,0x9,0x7,0x2,0x7,0x2,0xb,0x1,0x4,0xe,0x1,
				0x7,0x9,0x4,0xc,0xa,0xe,0x8,0x2,0xd,0x0,0xf,0x6,0xc,
				0xa,0x9,0xd,0x0,0xf,0x3,0x3,0x5,0x5,0x6,0x8,0xb
		} };
static const ULONG8 keyleft[28] = {/*等分密钥*/
		57,49,41,33,25,17,9,1,58,50,42,34,26,18,
		10,2,59,51,43,35,27,19,11,3,60,52,44,36};
	 
static const ULONG8 keyright[28] = {/*等分密钥*/
		63,55,47,39,31,23,15,7,62,54,46,38,30,22,
		14,6,61,53,45,37,29,21,13,5,28,20,12,4};

static const ULONG8 lefttable[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};/*密钥移位*/
static const ULONG8 keychoose[48] ={/*密钥选取*/
		14,17,11,24,1,5,3,28,15,6,21,10,
		23,19,12,4,26,8,16,7,27,20,13,2,
		41,52,31,37,47,55,30,40,51,45,33,48,
		44,49,39,56,34,53,46,42,50,36,29,32};

static const ULONG32 wz_leftandtab[3] = {0x0 , 0x80000000 , 0xc0000000 } ;


class CDesOperate
{
private:
	ULONG32 m_arrOutKey[16][2];/*输出的key*/
	ULONG32 m_arrBufKey[2];/*形成起始密钥*/

	INT32 MakeData(ULONG32  *left ,ULONG32  *right ,ULONG32 number) 
	{
		INT32 j; 
		ULONG32 oldright = *right;         
		ULONG8 rexpbuf[8] = { 0} ;
		ULONG32 datatmp = 0;        
		ULONG32 exdes_P[2] = { 0} ;                         
		//由32扩充至48位
		
		for ( j = 0 ; j < 48 ; j++)
		{
			/*两个32位，每个存放24位*/
			if ( j < 24 )
			{
				if ( *right&pc_by_bit[des_E[j]-1] )
				{
					exdes_P[0] |= pc_by_bit[j] ;
				}            
			}            
			else
			{
				if ( *right&pc_by_bit[des_E[j]-1] )
				{
					exdes_P[1] |= pc_by_bit[j-24] ;
				}
			}
		}
		
		for ( j = 0 ; j < 2 ; j++)
		{            
			exdes_P[j] ^= m_arrOutKey[number][j] ;
		}    
		
		/*由48－>32*/      
		
		exdes_P[1] >>= 8 ;
		rexpbuf[7] = (ULONG8) (exdes_P[1]&0x0000003fL) ;
		exdes_P[1] >>= 6 ;
		rexpbuf[6] = (ULONG8) (exdes_P[1]&0x0000003fL) ;
		exdes_P[1] >>= 6 ;
		rexpbuf[5] = (ULONG8) (exdes_P[1]&0x0000003fL) ;
		exdes_P[1] >>= 6 ;
		rexpbuf[4] = (ULONG8) (exdes_P[1]&0x0000003fL) ;
		exdes_P[0]  >>=  8 ;
		rexpbuf[3] = (ULONG8) (exdes_P[0]&0x0000003fL) ;     
		exdes_P[0] >>= 6 ;
		rexpbuf[2] = (ULONG8) (exdes_P[0]&0x0000003fL) ;
		exdes_P[0] >>= 6 ;
		rexpbuf[1] = (ULONG8) (exdes_P[0]&0x0000003fL) ;
		exdes_P[0] >>= 6 ;
		rexpbuf[0] = (ULONG8) (exdes_P[0]&0x0000003fL) ;     
		exdes_P[0] = 0 ;
		exdes_P[1] = 0 ;
		
		/*由48－> 32*/
		
		*right = 0 ;
		for ( j = 0 ; j < 7 ; j++)
		{
			*right |= des_S[j][rexpbuf[j]] ;
			*right <<= 4 ;
		}
		*right |= des_S[j][rexpbuf[j]] ;
		
		/*又要换位了*/
		
		datatmp = 0;
		for ( j = 0 ; j < 32 ; j++)
		{
			if ( *right&pc_by_bit[des_P[j]-1] )
			{
				datatmp |= pc_by_bit[j] ;
			}
		}
		*right = datatmp ;
		
		/*一轮结束收尾操作*/                
		
		*right ^= *left;       
		*left = oldright;
		
		return SUCCESS;
	}
	INT32 HandleData(ULONG32 *left , ULONG8 choice)
	{
		INT32  number = 0 ,j = 0;   
		ULONG32 *right = &left[1] ;
		ULONG32 tmp = 0;       
		ULONG32 tmpbuf[2] = { 0 };             
		
		/*第一次调整pc_first[64]*/
		for ( j = 0 ; j < 64 ; j++)
		{
			if (j < 32 ) 
			{
				if ( pc_first[j] > 32)/*属于right*/
				{
					if ( *right&pc_by_bit[pc_first[j]-1] )
					{
						tmpbuf[0] |= pc_by_bit[j] ;
					}
				}
				else
				{
					if ( *left&pc_by_bit[pc_first[j]-1] )
					{
						tmpbuf[0] |= pc_by_bit[j] ;
					}
				}
			}
			else
			{
				if ( pc_first[j] > 32)/*属于right*/
				{
					if ( *right&pc_by_bit[pc_first[j]-1] )
					{
						tmpbuf[1] |= pc_by_bit[j] ;
					}
				}
				else
				{
					if ( *left&pc_by_bit[pc_first[j]-1] )
					{
						tmpbuf[1] |= pc_by_bit[j] ;
					}
				}
			}
		}
		*left  = tmpbuf[0];
		*right = tmpbuf[1];
		tmpbuf[0] = 0 ;
		tmpbuf[1] = 0 ;
		
		switch(choice)
		{
		case DESENCRY:
			for ( number = 0 ; number < 16 ; number++)
			{            
				MakeData( left , right , (ULONG32)number);
			}
			break;
		case DESDECRY:
			for (number=15;number>=0;number--)
			{            
				MakeData( left , right ,(ULONG32)number);
			}
			break;
		default:
			break;
		}
		
		/*最后一轮操作不交换左右值*/
		
		tmp = *left ;
		*left = *right ;
		*right = tmp ;        
		
		/*最后一次调整pc_last[64]*/
		
		for ( j = 0 ; j < 64 ; j++)
		{
			if (j < 32 ) 
			{
				if ( pc_last[j] > 32)/*属于right*/
				{
					if ( *right&pc_by_bit[pc_last[j]-1] )
					{
						tmpbuf[0] |= pc_by_bit[j] ;
					}
				}
				else
				{
					if ( *left&pc_by_bit[pc_last[j]-1] )
					{
						tmpbuf[0] |= pc_by_bit[j] ;
					}
				}
			}
			else
			{
				if ( pc_last[j] > 32)/*属于right*/
				{
					if ( *right&pc_by_bit[pc_last[j]-1] )
					{
						tmpbuf[1] |= pc_by_bit[j] ;
					}
				}
				else
				{
					if ( *left&pc_by_bit[pc_last[j]-1] )
					{
						tmpbuf[1] |= pc_by_bit[j] ;
					}
				}
			}
		}
		
		*left =  tmpbuf[0] ;
		*right = tmpbuf[1];
		
		return SUCCESS;
	}
	INT32 MakeKey(  ULONG32 *keyleft,ULONG32 *keyright ,ULONG32 number)
	{
		ULONG32 tmpkey[2] ={0};
		ULONG32 *Ptmpkey = (ULONG32*)tmpkey;     
		ULONG32 *Poutkey = (ULONG32*)&m_arrOutKey[number]; 
		INT32 j;        
		memset((ULONG8*)tmpkey,0,sizeof(tmpkey));          
		/*要最高的一位或两位*/
		*Ptmpkey = *keyleft&wz_leftandtab[lefttable[number]] ;           
		Ptmpkey[1] = *keyright&wz_leftandtab[lefttable[number]] ;              
		if ( lefttable[number] == 1)
		{
			*Ptmpkey >>= 27;
			Ptmpkey[1] >>= 27;
		}
		else
		{
			*Ptmpkey >>= 26;
			Ptmpkey[1] >>= 26;                    
		}
		Ptmpkey[0] &= 0xfffffff0;
		Ptmpkey[1] &= 0xfffffff0;
		/*得到高位的值*/
		*keyleft <<= lefttable[number] ;
		*keyright <<= lefttable[number] ;
		*keyleft |= Ptmpkey[0] ;
		*keyright |= Ptmpkey[1] ;            
		Ptmpkey[0] = 0;
		Ptmpkey[1] = 0;
	    
		/*从56位中选出48位,3个16位*/
		for ( j = 0 ; j < 48 ; j++)
		{
			if ( j < 24 )
			{
				
				if ( *keyleft&pc_by_bit[keychoose[j]-1])
				{
					Poutkey[0] |= pc_by_bit[j] ;
				}                   
			}            
			
			else /*j>=24*/
			{                   
				if ( *keyright&pc_by_bit[(keychoose[j]-28)])
				{
					Poutkey[1] |= pc_by_bit[j-24] ;
				}                   
			}
		}
		return SUCCESS;
	}


	INT32 MakeFirstKey( ULONG32 *keyP )
	{
		ULONG32 key[2] = {0};
		ULONG32 *Pkey ;
		ULONG32 *Pbufkey ;
		INT32 j; 
		Pbufkey = (ULONG32*)m_arrBufKey ;
		Pkey = (ULONG32*)key;
		
		memset((ULONG8*)m_arrBufKey,0,sizeof(m_arrBufKey));    
		memcpy((ULONG8*)&key,(ULONG8*)keyP ,8) ;      
		memset((ULONG8*)m_arrOutKey,0,sizeof(m_arrOutKey));
		for(  j = 0 ; j < 28 ; j++)
		{
			if ( keyleft[j] > 32 ) 
			{
				if ( Pkey[1]&pc_by_bit[keyleft[j]-1] )
				{
					Pbufkey[0] |= pc_by_bit[j] ;
				}
			}
			else
			{
				if ( Pkey[0]&pc_by_bit[keyleft[j]-1] )
				{
					Pbufkey[0] |= pc_by_bit[j] ;
				}
			}
			
			if ( keyright[j] > 32 ) 
			{
				if ( Pkey[1]&pc_by_bit[keyright[j]-1] )
				{
					Pbufkey[1] |= pc_by_bit[j];
				}
			}
			else
			{
				if ( Pkey[0]&pc_by_bit[keyright[j]-1] )
				{
					Pbufkey[1] |= pc_by_bit[j];
				}
			}
		}
		for (j = 0 ; j < 16 ; j++)
		{
			MakeKey(&Pbufkey[0],&Pbufkey[1] , j ) ;
		}
		return SUCCESS;
	}


	
public:
	CDesOperate()
	{
		for(int i =0 ;i<2;i++)
		{
			for(int j = 0;j<16;j++)
			{
				this->m_arrOutKey[j][i] = 0;
			}
			this->m_arrBufKey[i] = 0;
		}
	}
	INT32 Encry(char* pPlaintext,int nPlaintextLength,char *pCipherBuffer,int &nCipherBufferLength, char *pKey,int nKeyLength)
	{
		if(nKeyLength != 8)
		{
			return 0;
		}
		MakeFirstKey((ULONG32 *)pKey);
		int nLenthofLong = ((nPlaintextLength+7)/8)*2;

		if(nCipherBufferLength<nLenthofLong*4)
		{//out put buffer is not enough
			nCipherBufferLength=nLenthofLong*4;
			return 0;
		}
		memset(pCipherBuffer,0,nCipherBufferLength);
		ULONG32 *pOutPutSpace = (ULONG32 *)pCipherBuffer;

		ULONG32 *pSouse;
		if(nPlaintextLength != sizeof(ULONG32)*nLenthofLong)
		{
			pSouse = new ULONG32[nLenthofLong];
			memset(pSouse,0,sizeof(ULONG32)*nLenthofLong);
			memcpy(pSouse,pPlaintext,nPlaintextLength);
		}
		else
		{
			pSouse = (ULONG32 *)pPlaintext;
		}

		ULONG32 gp_msg[2] = {0,0};
		for (int i=0;i<(nLenthofLong/2);i++)
		{
			gp_msg[0] = pSouse[2*i];
			gp_msg[1] =pSouse[2*i+1];
			HandleData(gp_msg,DESENCRY);
			pOutPutSpace[2*i] = gp_msg[0];
			pOutPutSpace[2*i+1] = gp_msg[1];
		}
		if(pPlaintext!=(char *)pSouse)
		{
			delete []pSouse;
		}
	    
		return SUCCESS;
	}

	INT32 Decry(char* pCipher,int nCipherBufferLength,char *pPlaintextBuffer, int &nPlaintextBufferLength, char *pKey,int nKeyLength)
	{
		if(nCipherBufferLength%8!=0)
		{
			return 0;
		}
		if(nPlaintextBufferLength<nCipherBufferLength)
		{
			nPlaintextBufferLength = nCipherBufferLength;
			return 0;
		}
		if(nKeyLength != 8)
		{
			return 0;
		}
		MakeFirstKey((ULONG32 *)pKey);

		memset(pPlaintextBuffer,0,nPlaintextBufferLength);
		ULONG32 *pSouce = (ULONG32 *)pCipher;
		ULONG32 *pDest = (ULONG32 *)pPlaintextBuffer;
		ULONG32 gp_msg[2] = {0,0};

		for (int i=0;i<(nCipherBufferLength/8);i++)
		{
			gp_msg[0] = pSouce[2*i];
			gp_msg[1] = pSouce[2*i+1];
			HandleData(gp_msg,DESDECRY);
			pDest[2*i] = gp_msg[0];
			pDest[2*i+1] = gp_msg[1];
		}
		return SUCCESS;
	}
};

