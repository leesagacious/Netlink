
enum LEE_NETLINK_ERROR {
    LEE_NETLINK_SUCCESS = 0,
    FAMILY_ERROR        = -1,
    LEE_NETLINK_PLE     = -2,
};

struct spring_netlink_sock {
    unsigned int flags,
};

struct listener {
    struct list_head node;
    unsigned long bitmap;
};

struct leenetlink_area {
    struct list_head   airing_list;
    unsigned int enroll;
    struct listeners *monitor;
};
