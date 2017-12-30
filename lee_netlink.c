
struct leenetlink_area * lee_area;

static const struct proto_ops lee_netlink_ops = {
    .family  = PF_LEENETLINK,
    .owner   = THIS_MODULE,
    .release = leenetlink_relase,
    .bind    = leenetlink_bind,
    .connect = leenetlink_connect,
};
