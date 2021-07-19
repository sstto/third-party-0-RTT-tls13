#include "mpz.h"
#include <iostream>
int main(int ac, char **av) {
    mpz_class a = random_prime(31);
    uint8_t arr[31];
    mpz2bnd(a, arr, arr+31);
    mpz_class b = bnd2mpz(arr, arr+31);

    std::stringstream ss;
    for(unsigned char x : arr)
        ss << std::hex<< std::setw(2) << std::setfill('0') << +x;

    std::cout <<ss.str() << std::endl;
}