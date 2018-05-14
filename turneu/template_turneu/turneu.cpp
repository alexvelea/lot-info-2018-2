#include "turneu.h"

int n;
int* p;

void init(int _n, int *_p) {
    n = _n;
    p = _p;

}

int query(int x, int y) {
    p[0] += 1;
    return p[0];
}
