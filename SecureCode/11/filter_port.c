#ifndef __KERNEL__
#define __KERNEL__
#endif
#ifndef MODULE
#define MODULE
#endif

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/tcp.h>
#include <net/tcp.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
/*struct that is used for register hook*/
static struct nf_hook_ops nfho;

unsigned short in_pton(const char * port_str) 
{ 
unsigned short p,i; 

for(p =0,i=0;port_str[i]<='9'&&port_str[i]>='0';i++) 
{ 
p = p *10 + (port_str[i] - '0'); 
} 

i = (p >> 8) & 0x0000ff; 
i |= (p << 8) & 0x00ff00; 

return(i); 
} 


/* definition of hook function */
unsigned int hook_func(unsigned int hooknum,
                       struct sk_buff **skb,
                       const struct net_device *in,
                       const struct net_device *out,
                       int (*okfn)(struct sk_buff *))
{
    struct sk_buff *pskb=*skb;
    struct tcphdr *thdr=(struct tcphdr *)(pskb->data + (pskb->nh.iph->ihl * 4));
    if((pskb->nh.iph->protocol)!=IPPROTO_TCP)
    {
	printk("<0>""Not A TCP Packet: ACCEPT\n");
	return NF_ACCEPT;
    }
    else
    {
	if(thdr->dest==in_pton("23"))
	{
	    printk("<0>""A TCP Packet PORT 23: DROP\n");
	    return NF_DROP;
	}
	else
	    return NF_ACCEPT;
    }
}

/* module initial function */
int init_module()
{
    /* init the struct */
    nfho.hook     = hook_func;         /* hook function */
    nfho.hooknum  = NF_IP_PRE_ROUTING; /* the hook point */
    nfho.pf       = PF_INET;
    nfho.priority = NF_IP_PRI_FIRST;   /* priority */

    nf_register_hook(&nfho);           /* register the hook */
   
    return 0;
}
   
/* module clean function */
void cleanup_module()
{
    nf_unregister_hook(&nfho);
}
