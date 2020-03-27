/* created by whsu */
/* 2020.03.26 */
/* Data Structure homework 2.2 */

#include <iostream>

size_t array_size;
int x;

int main() {
    std::cout << "input array's size and x: ";
    std::cin >> array_size >> x;
    int *a = new int[array_size];

    std::cout << "input array: ";
    for (size_t i=0 ; i<array_size ; i++) std::cin >> a[i];

    size_t i=0, j=array_size-1;
    while (i<j) {
        if (a[i]+a[j] > x) j--;
        else if (a[i]+a[j] < x) i++;
        else /* if (a[i]+a[j] == x) */ {
            std::cout << "a[" << i << "] = " << a[i] <<  ", a[" << j << "] = " << a[j] << std::endl;
            std::cout << "a[" << i << "] + a[" << j << "] = " << x << std::endl;
            delete[] a;
            return 0;
        } 
    }
    std::cout << "Sorry, not found!" << std::endl;
    delete[] a;
    return 1;
}
