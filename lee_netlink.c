
struct leenetlink_area * spring_area;


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
    
alloc_area_fail:
    panic("alloc spring_netlink_area has failure\n");
}    
