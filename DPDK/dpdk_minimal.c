// Filename: dpdk_minimal.c

#include <stdio.h>
#include <stdint.h>
#include <rte_common.h>
#include <rte_eal.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <rte_ethdev.h>

#define NUM_MBUFS 2048
#define MBUF_CACHE_SIZE 250
#define BURST_SIZE 32

int main(int argc, char *argv[]) {
    int ret = rte_eal_init(argc, argv);
    if (ret < 0) rte_exit(EXIT_FAILURE, "EAL init failed\n");

    uint16_t port_id = 0;

    // Check for at least 1 port
    if (rte_eth_dev_count_avail() == 0)
        rte_exit(EXIT_FAILURE, "No Ethernet ports - are you using --vdev?\n");

    // Create mempool
    struct rte_mempool *mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL", NUM_MBUFS,
        MBUF_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());
    if (mbuf_pool == NULL)
        rte_exit(EXIT_FAILURE, "Cannot create mbuf pool\n");

    // Configure Ethernet device
    struct rte_eth_conf port_conf = {0};
    ret = rte_eth_dev_configure(port_id, 1, 1, &port_conf);
    if (ret < 0)
        rte_exit(EXIT_FAILURE, "Cannot configure device: err=%d\n", ret);

    ret = rte_eth_rx_queue_setup(port_id, 0, 128, rte_eth_dev_socket_id(port_id), NULL, mbuf_pool);
    if (ret < 0)
        rte_exit(EXIT_FAILURE, "Rx queue setup failed\n");

    ret = rte_eth_tx_queue_setup(port_id, 0, 128, rte_eth_dev_socket_id(port_id), NULL);
    if (ret < 0)
        rte_exit(EXIT_FAILURE, "Tx queue setup failed\n");

    ret = rte_eth_dev_start(port_id);
    if (ret < 0)
        rte_exit(EXIT_FAILURE, "Device start failed\n");

    printf("Started port %u, forwarding packets...\n", port_id);

    // Simple forward loop
    struct rte_mbuf *bufs[BURST_SIZE];
    while (1) {
        uint16_t nb_rx = rte_eth_rx_burst(port_id, 0, bufs, BURST_SIZE);
        if (nb_rx > 0) {
            rte_eth_tx_burst(port_id, 0, bufs, nb_rx);
        }
    }

    rte_eth_dev_stop(port_id);
    rte_eth_dev_close(port_id);
    return 0;
}
