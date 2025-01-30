#include <stdio.h>

struct Person
{
    char name[64];
    int age;
};

int main() {

    struct Person people[100];

    struct Person *p_person = &people;
    
    int i = 0;
    for (i=0; i<100; i++) {
        p_Person->age = 0;
        p_Person += sizeof(Person);
    }

    return 0;
}