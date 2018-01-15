
struct leenetlink_area * spring_area;

static lee_netlink_create(struct net *net, struct socket *sock, int protocol)
{
    int err; 
    
    if (!spring_area.has_register)  {
        err = -ENODEV;
        goto lee_netlink_create_failure;
    }       
    
    return 0;
    
lee_netlink_create_failure:
    return err;
}  


static const struct proto_ops spring_netlink_ops = {
    .family  = PF_LEENETLINK,
    .owner   = THIS_MODULE,
    .release = leenetlink_relase,
    .bind    = leenetlink_bind,
    .connect = leenetlink_connect,
};


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
