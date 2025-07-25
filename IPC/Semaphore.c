#include <stdio.h>

void T() {
    wait() {
        while(1) {
            atomic {
                if(v>0) {
                    v--;
                    return;
                }
            }
        }
    }

    post() {
        atomic {
            v++;
            return;
        }
    }
}