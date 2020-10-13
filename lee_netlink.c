// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2020, li-insects-awaken
 */

struct leenetlink_area * spring_area;

struct lee_sock {
	struct sock sock;
	wait_queue_head_t	tar_halt;
};

static const struct net_proto_family spring_netlink_family_ops = {
	.family = SPRING_NETLINK,
	.create = spring_netlink_create,
};

static struct proto lee_netlink_proto = {
	.name	  = "NETLINK",
	.owner	  = THIS_MODULE,
	.obj_size = sizeof(struct lee_sock),
};

static int leenetlink_bind(struct socket *sock, struct sockaddr *addr, int addr_len)
{
	int err;
	unsigned int groups;
	struct lee_sockaddr_nl *nladdr = (struct sockaddr_nl *)addr;

	if (!nladdr->nl_groups) {
		err = -EINVAL;
		goto out;
	}

	groups = spring_area[sk->index].groups;
	if (groups < 64)
		groups &= (1UL << groups) - 1;

out:
	return err;
}

int netlink_unicast(struct sock *sock, struct sk_buff *skb, unsigned int  portid)
{
	int ret;
	int has_sleep;
	unsigned long deleaytime;
	struct sock *otsock;

	delaytime = sock->sk_sndtimeo;

	targetsock = via_otsock_adopt_portid(sock, portid);
	if (!targetsock) {
		kfree_skb(skb);
		ret = -1;
		goto unicast_failed;
	}

	has_sleep = atomic_read(&targetsock->sk_rmem_alloc) - targetsock->sk_rcvbuf;
	if (has_sleep >= 0) {
		wait_queue_t taskhalt = {
			.private = cpu_rq(smp_processor_id())->curr;
			.func    = default_wait_function,
			.task_list = {NULL, NULL}
		};

		set_current_task(TASK_INTERRUPTIBLE);

		add_wait_queue(&leesock->tar_halt, &taskhalt);
	}

	skb_queue_tail(&targetsock->sk_receive_queue, skb);

	targetsock->sk_data_ready(targetsock);

	return 0;

unicast_failed:
	return ret;
}

struct sock * lee_netlink_kernel_create(unsigned int netlinkindex, struct netlink_kernel_cfg * leecfg)
{
	unsigned long groups = 32;

	if (!spring_area)
	    painic("spring_area must be not null");

	if (netlinkindex < 0 || netlinkindex > 32)
		goto kernel_create_failed;

	if (leecfg->groups < 32)
		groups = 32;

	mutex_lock(&spring_area_lock);
	if (spring_area[netlinkindex].has_registered) {
		pr_err("This netlink protocol has been registered\n");
		goto kernel_create_failed;
	}
	spring_area[netlinkindex].groups = groups;
	mutex_unlock(&spring_area_lock);


kernel_create_failed:
	return NULL;
}

static int is_ple(unsigned int detection_tmp)
{
	do {
		if (detection_tmp & 1)
			goto checkedout;

		detection_tmp >>= 1;

	} while (detection_tmp);

	return LEE_NETLINK_PLE;

checkedout:
	return LEE_NETLINK_SUCCESS;
}

static int leenetlink_recvmsg(struct socket * sock, struct msghdr *msg, size_t  len)
{

}

static int leenetlink_sendmsg(struct socket *sock, struct msghdr *msg, size_t len)
{
	int ret;
	unsigned int has_ple;
	unsigned int has_bound;
	struct sk_buff *skb;
	struct sockaddr_nl * addr_lee;

	addr_lee = (struct sockaddr_nl *)msg->msg_name;

	if (addr_lee->nl_family != AF_NETLINK) {
		ret = FAMILY_ERROR;
		goto family_error;
	}

	has_ple = is_ple(addr_lee->nl_groups);

family_error:
	return ret;
}

static const struct proto_ops spring_netlink_ops = {
	.family  = PF_LEENETLINK,
	.owner   = THIS_MODULE,
	.release = leenetlink_relase,
	.bind    = leenetlink_bind,
	.connect = leenetlink_connect,
	.sendmsg = leenetlink_sendmsg,
	.recvmsg = leenetlink_recvmsg,
};

static lee_netlink_create(struct net *net, struct socket *sock, int protocol)
{
	int err;
	struct sock *sk;
	struct lee_sock * leesock;

	if (!spring_area.has_register)  {
		err = -ENODEV;
		goto lee_netlink_create_failure;
	}

	sock->ops = &spring_netlink_ops;

	sk = sk_alloc(net, PF_LEENETLINK, GFP_KERNEL, &lee_netlink_proto);
	if (!sk)
		return -ENOMEM;

	sk->sk_rcvbuf = 32768;

	init_waitqueue_head(&leesock->tar_halt);

	return 0;

lee_netlink_create_failure:
	return err;
}


static int __init spring_netlinkport_init(void)
{
	int ret;

	spring_area = kcalloc(MAX_LINKS, sizeof(*spring_area), GFP_KERNEL);
	if (!spring_area)
		goto alloc_area_fail;

	sock_register(&spring_netlink_ops);

alloc_area_fail:
	panic("alloc spring_netlink_area has failure\n");
}

core_initcall(spring_netlinkport_init);
