#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_mbuf.h>
#include <rte_mempool.h>
#include <rte_mbuf_pool_ops.h>


#define RX_RING_SIZE 128
#define TX_RING_SIZE 512
#define NUM_MBUFS 8192
#define MBUF_CACHE_SIZE 250
#define BURST_SIZE 32


int main(int argc, char *argv[]) {
    struct rte_mempool *mbuf_pool;
    uint16_t port_id;

    // Initialize Environment Abstraction Layer (EAL)
    int ret = rte_eal_init(argc, argv);
    if (ret < 0) rte_exit(EXIT_FAILURE, "EAL init failed\n");

    argc -= ret;
    argv += ret;

    // Create mempool
    mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL", NUM_MBUFS,
                    MBUF_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE,
                    rte_socket_id());
    if (mbuf_pool == NULL)
        rte_exit(EXIT_FAILURE, "Cannot create mbuf pool\n");

    // Check and get the first available port
    RTE_ETH_FOREACH_DEV(port_id) {
        struct rte_eth_conf port_conf = {0};
        const uint16_t rx_rings = 1, tx_rings = 1;
        uint16_t q;

        // Configure Ethernet port
        ret = rte_eth_dev_configure(port_id, rx_rings, tx_rings, &port_conf);
        if (ret < 0)
            rte_exit(EXIT_FAILURE, "rte_eth_dev_configure: err=%d\n", ret);

        // Set up RX queues
        for (q = 0; q < rx_rings; q++) {
            ret = rte_eth_rx_queue_setup(port_id, q, RX_RING_SIZE,
                        rte_eth_dev_socket_id(port_id), NULL, mbuf_pool);
            if (ret < 0)
                rte_exit(EXIT_FAILURE, "rte_eth_rx_queue_setup: err=%d\n", ret);
        }

        // Set up TX queues
        for (q = 0; q < tx_rings; q++) {
            ret = rte_eth_tx_queue_setup(port_id, q, TX_RING_SIZE,
                        rte_eth_dev_socket_id(port_id), NULL);
            if (ret < 0)
                rte_exit(EXIT_FAILURE, "rte_eth_tx_queue_setup: err=%d\n", ret);
        }

        // Start the Ethernet port
        ret = rte_eth_dev_start(port_id);
        if (ret < 0)
            rte_exit(EXIT_FAILURE, "rte_eth_dev_start: err=%d\n", ret);

        // Enable promiscuous mode (optional)
        rte_eth_promiscuous_enable(port_id);

        break; // only use one port in this minimal example
    }


    struct rte_mbuf *bufs[BURST_SIZE];
    uint16_t nb_rx;

    while (1) {
        // Poll RX burst
        nb_rx = rte_eth_rx_burst(port_id, 0, bufs, BURST_SIZE);
        if (nb_rx > 0) {
            // Do basic forwarding: send same packets to TX
            uint16_t nb_tx = rte_eth_tx_burst(port_id, 0, bufs, nb_rx);

            // Free any unsent packets
            if (nb_tx < nb_rx) {
                for (uint16_t i = nb_tx; i < nb_rx; i++)
                    rte_pktmbuf_free(bufs[i]);
            }
        }
    }

    return 0;
}

