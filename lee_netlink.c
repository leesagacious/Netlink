
struct leenetlink_area * spring_area;

static struct proto lee_netlink_proto = {
	.name	  = "NETLINK",
	.owner	  = THIS_MODULE,
	.obj_size = sizeof(struct netlink_sock),
};

static int is_ple(unsigned int detection_tmp)
{	
	do {
		if (detection_tmp & 1) 
		    goto checkedout;
		
		detection_tmp >>= 1;
		
	} while (detection_tmp)
		
	return 	LEE_NETLINK_MULTI;
	
checkedout:
	return LEE_NETLINK_SUCCESS;
}	
/*
    func : Copy the buffer data to decide whether it is unicast or multicast
*/
static int leenetlink_sendmsg(struct socket *sock, struct msghdr *msg, size_t len)
{
	int ret;	
	unsigned int has_ple;
	unsigned int has_bound;
	struct sk_buff *skb;
	struct sockaddr_nl * addr_lee;
	
	addr_lee = (struct sockaddr_nl *)msg->msg_name;
	
	if (addr_lee->nl_family != AF_NETLINK) {
		err = FAMILY_ERROR;
		goto family_error;
	}	
	/*
		1 : not use ffs().
		2 : Imitation soft interrupt handling
	*/
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
};

static lee_netlink_create(struct net *net, struct socket *sock, int protocol)
{
    int err; 
    /*
        He is the protagonist
        It's too important ！
    */
    struct sock *sk;
    
    if (!spring_area.has_register)  {
        err = -ENODEV;
        goto lee_netlink_create_failure;
    }       
    
    /*
        user-land api calls the socket API will eventually call the specific agreement of the corresponding function
        here !
    */
    sock->ops = &spring_netlink_ops;
    
    sk = sk_alloc(net, PF_LEENETLINK, GFP_KERNEL, &lee_netlink_proto);
	if (!sk)
		return -ENOMEM;
    
    return 0;
    
lee_netlink_create_failure:
    return err;
}  




static int __init spring_netlinkport_init(void)
{
    int ret;
    
    spring_area = kcalloc(MAX_LINKS, sizeof *spring_area, GFP_KERNEL);
    if (!spring_area) 
        goto alloc_area_fail;
    
    /*
        func : spring_netlink Agreement registration 
        
        PF_LEENETLINK : 27
        
        The underlying sock implementation : 
        -> :
        rcu_assign_pointer(net_families[PF_LEENETLINK], ops);
    */
    sock_register(&spring_netlink_ops);

alloc_area_fail:
    panic("alloc spring_netlink_area has failure\n");
}    

core_initcall(spring_netlinkport_init);
