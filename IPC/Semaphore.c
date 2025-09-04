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

/*  Any thread can call wait and any thread can call post unlike mutex*/