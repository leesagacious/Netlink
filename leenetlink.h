
enum LEE_NETLINK_ERROR {
    LEE_NETLINK_SUCCESS = 0,
    FAMILY_ERROR        = -1,
    LEE_NETLINK_PLE     = -2,
};

struct leenetlink_area {
    struct list_head   airing_list;
    unsigned int enroll;
};
