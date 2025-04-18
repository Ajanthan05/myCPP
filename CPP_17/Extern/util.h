/* Name Mangling in cpp:- boc it do it to support
name mangling 
*/

#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        int age;
        char name[256];
    } person;

    double getmean(double *values, int length);

    void print_person_info(person *p);
    
#ifdef __cplusplus
}
#endif

#endif