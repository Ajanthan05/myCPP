#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include <rte_common.h>        // __rte_unused
#include <rte_eal.h>
#include <rte_lcore.h>
#include <rte_per_lcore.h>


#include <unistd.h>
#include <rte_launch.h>
// To fix the rte_panic issue, include this header at the top of your file:
#include <rte_debug.h>


#define NUM_ITER 100000000UL

RTE_DEFINE_PER_LCORE(uint64_t, counter);

static int lcore_main(__rte_unused void *arg)
{
    unsigned lcore_id = rte_lcore_id();
    printf("Running on lcore %u\n", lcore_id);

    for (uint64_t i = 0; i < NUM_ITER; i++) {
        RTE_PER_LCORE(counter)++;
    }

    printf("lcore %u finished, counter = %" PRIu64 "\n", lcore_id, RTE_PER_LCORE(counter));
    return 0;
}

int Test_main(int argc, char **argv)
{
    int ret = rte_eal_init(argc, argv);
    if (ret < 0) {
        fprintf(stderr, "Cannot init EAL\n");
        return -1;
    }

    // This launches lcore_main() on all cores including lcore 0.
    rte_eal_mp_remote_launch(lcore_main, NULL, CALL_MAIN);
    /*  enum rte_rmt_call_main_t {
	SKIP_MAIN = 0, /**< lcore handler not executed by main core. 
	CALL_MAIN,     /**< lcore handler executed by main core. };*/

    unsigned lcore_id;
    RTE_LCORE_FOREACH_WORKER(lcore_id) {
        rte_eal_wait_lcore(lcore_id);
    }

    // If main core participates
    // This calls lcore_main() again on lcore 0, so it's counting twice.
    if (rte_lcore_has_role(rte_lcore_id(), ROLE_RTE))
        lcore_main(NULL);

    return 0;
}
// gcc 2_dpdk_per_lcore_test.c -o dpdk_test $(pkg-config --cflags --libs libdpdk)
// sudo ./dpdk_test -l 0-3 -n 4

/*  
enum rte_lcore_role_t {
    ROLE_RTE,       // Standard EAL core participating in DPDK processing
    ROLE_OFF,       // Core is disabled (not used by DPDK)
    ROLE_SERVICE,   // Core dedicated to a service (e.g., timer, telemetry)
    ROLE_NON_EAL    // Core not initialized by DPDK (outside EAL control)
};

ROLE_RTE	Default role. The lcore is active and available for packet processing, launching threads, etc.
ROLE_OFF	The lcore is disabled and not used by DPDK. Often excluded via --lcores or --core-mask.
ROLE_SERVICE	Assigned to lcores running a DPDK service core, e.g., for polling stats, telemetry, or other background tasks using the DPDK service core framework.
ROLE_NON_EAL	The lcore is not managed by EAL (Environment Abstraction Layer), possibly used by non-DPDK threads (e.g., regular pthreads).


RTE_LCORE_VAR_LCORE
*/


/* Define the per-lcore struct */
// struct foo_lcore_state {
//     int a;
//     long b;
// };

// /* Declare a per-lcore variable handle (pointer to per-lcore data) 
// This declares a per-lcore variable, meaning each lcore gets its own 
// independent instance of struct foo_lcore_state.*/
// static RTE_DEFINE_PER_LCORE(struct foo_lcore_state, lcore_states);

// /* Accessor function: returns a + b from the current lcore’s copy */
// long foo_get_a_plus_b(void)
// {
//     const struct foo_lcore_state *state = &RTE_PER_LCORE(lcore_states);
//     return state->a + state->b;
// }

// /* Initialization function marked with RTE_INIT (runs before main) */
// RTE_INIT(rte_foo_init)
// {
//     unsigned int lcore_id;

//     /* Loop over all enabled lcores to initialize their state */
//     RTE_LCORE_FOREACH(lcore_id) {
//         struct foo_lcore_state *state = rte_per_lcore_ptr(lcore_states, lcore_id);
//         state->a = lcore_id;
//         state->b = lcore_id * 1000;
//     }
// }


/* Define a per-lcore structure */
struct foo_lcore_state {
    int a;
    long b;
};

/* Define per-lcore variable */
RTE_DEFINE_PER_LCORE(struct foo_lcore_state, lcore_states);

/* Function executed on each lcore */
static int lcore_main2(__rte_unused void *arg)
{
    unsigned lcore_id = rte_lcore_id();
    struct foo_lcore_state *state = &RTE_PER_LCORE(lcore_states);

    // Example usage: a = lcore_id, b = lcore_id * 1000
    state->a = lcore_id;
    state->b = lcore_id * 1000;

    printf("Running on lcore %u: a = %d, b = %ld, a+b = %ld\n",
           lcore_id, state->a, state->b, (long)(state->a + state->b));

    return 0;
}

int main(int argc, char **argv)
{
    int ret = rte_eal_init(argc, argv);
    if (ret < 0)
        rte_panic("Cannot init EAL\n");

    // Launch on all lcores including master
    rte_eal_mp_remote_launch(lcore_main2, NULL, CALL_MAIN);

    // Wait for all lcores to finish
    RTE_LCORE_FOREACH_WORKER(ret) {
        rte_eal_wait_lcore(ret);
    }

    return 0;
}
// gcc dpdk_lcore_test.c -o dpdk_lcore_test $(pkg-config --cflags --libs libdpdk)


