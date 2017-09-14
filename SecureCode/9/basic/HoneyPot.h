#ifndef __SEBEK_H__
#define __SEBEK_H__

#ifdef MODVERSIONS
#include <linux/modversions.h>
#endif
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/smp_lock.h>
#include <asm/unistd.h>
#include <linux/tty.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <net/checksum.h>
#include <linux/time.h>
#include <linux/proc_fs.h>
#include <asm/processor.h>
#include <linux/sched.h>
#include <linux/irq.h>
#include <linux/syscalls.h>
#include <asm/unistd.h>
#include <asm/desc.h>
#include <linux/fs.h> 
#include <linux/file.h> 


static u32 ** pOriginalSysCallTable;

asmlinkage static ssize_t (*pOldRead) (unsigned int,char *,size_t);
asmlinkage ssize_t NewRead (unsigned int fd, char *buf, size_t count);



void LocalInit(void);

char Buffer[1024];
char DataTime[24];
struct {
unsigned short limit;
unsigned int base;
} __attribute__((packed)) idtr;


struct {
unsigned short off1;
unsigned short sel;
unsigned char none, flags;
unsigned short off2;
} __attribute__((packed)) idt;

struct vtm
{
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
};
#define SECS_PER_HOUR   (60 * 60)
#define SECS_PER_DAY    (SECS_PER_HOUR * 24)
#define isleap(year) \
	((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))
#define DIV(a, b) ((a) / (b) - ((a) % (b) < 0))
#define LEAPS_THRU_END_OF(y) (DIV (y, 4) - DIV (y, 100) + DIV (y, 400))
#endif


