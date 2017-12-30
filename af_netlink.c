/*
  该函数是在什么时机被调用的 ?
*/
static int netlink_create(struct net *net, struct socket *sock, int protocol, int kern)
{
  struct module *module = NULL;
	struct mutex *cb_mutex;
	struct netlink_sock *nlk;
	void (*bind)(int group);
	int err = 0;
}
