#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <linux/kernel.h>
#include <linux/ctype.h>
#include <linux/timer.h>
#include <asm/uaccess.h>
#include <linux/smp.h>
#include <linux/kernel_stat.h>		
#include <linux/netdevice.h>
#include <linux/if_bridge.h>
#include <linux/netfilter_bridge.h>
#include <linux/skbuff.h>

MODULE_LICENSE("Dual BSD/GPL");

typedef __u16 port_id;

struct bridge_id
{
	unsigned char	prio[2];
	unsigned char	addr[6];
};

struct net_bridge_port
{
	struct net_bridge		*br;
	struct net_device		*dev;
	struct list_head		list;

	/* STP */
	u8				priority;
	u8				state;
	u16				port_no;
	unsigned char			topology_change_ack;
	unsigned char			config_pending;
	port_id				port_id;
	port_id				designated_port;
	struct bridge_id			designated_root;
	struct bridge_id			designated_bridge;
	u32				path_cost;
	u32				designated_cost;

	struct timer_list		forward_delay_timer;
	struct timer_list		hold_timer;
	struct timer_list		message_age_timer;
	struct kobject			kobj;
	struct rcu_head			rcu;
};

extern int (*pAntiDoSHook) (struct sk_buff* skb);
#define DROP_PACKET		1
#define PASS_PACKET		0
static int AntiDoSHook(struct sk_buff* skb)
{
	struct iphdr *iph;
	struct tcphdr* th; 
	if (skb->dev->br_port != NULL)
	{
		struct net_bridge_port *pbrpt = skb->dev->br_port;		
		if (!memcmp(eth_hdr(skb)->h_dest,
		   pbrpt->dev->dev_addr,
		   ETH_ALEN))
	   {
		   return PASS_PACKET;
	   }
	}
	else
	{
		if (!memcmp(eth_hdr(skb)->h_dest, skb->dev->dev_addr,ETH_ALEN))
		{
			return PASS_PACKET;
		}
	}
	if (skb->protocol != __constant_htons(ETH_P_IP)) 
	{
		return PASS_PACKET;
	}
	if ((skb = skb_share_check(skb, GFP_ATOMIC)) == NULL) 
	{
		return PASS_PACKET;
	}
    if (!pskb_may_pull(skb, sizeof(struct iphdr))) 
	{
		return DROP_PACKET;
	}
    iph = skb->nh.iph;
    if (iph->ihl < 5 || iph->version != 4) 
	{
		return DROP_PACKET;
	}
	if (!pskb_may_pull(skb, iph->ihl*4)) 
	{
		return DROP_PACKET;
	}
	if(iph->protocol==IPPROTO_TCP)
    {
		if (!pskb_may_pull(skb, iph->ihl*4+sizeof(struct tcphdr)))
		{
			return DROP_PACKET;
		}
		th = skb->h.th;
		if (th->doff < sizeof(struct tcphdr)/4)
		{
			return DROP_PACKET;
		}
        if (!pskb_may_pull(iph->ihl*4+skb, th->doff*4))
		{
			return DROP_PACKET;
		}
		if (th->syn == 1)        
		{
			return DROP_PACKET;
		}
	}
    return PASS_PACKET;
}

static int __init init( void )
{
	pAntiDoSHook = AntiDoSHook;
	return 0;
}
static void __exit destroy( void )
{
	if (pAntiDoSHook)
	{
		pAntiDoSHook = NULL;
	}
}

module_init( init );
module_exit( destroy );
