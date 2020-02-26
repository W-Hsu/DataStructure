#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include "avl.hpp"
#include "bTree.hpp"

int b[8] = {6, 1, 7, 8, 2, 3, 5, 4};
int c[8] = {2, 5, 6, 8, 7, 1, 3, 4};
int d[8] = {1, 2, 3, 4, 5, 6, 7, 8};

int ra[8] = {-1565019504, -390660289, 2122435038, -408257286, 631183611, -1886366707, -154682768, 460849280};
//{1657937496, 96432015, 1063406789, -621491145, -734346173, -1576072066, -1275643092, -1696102093};
//{-1095204490, -1688103928, 534167082, 889467644, 753989168, -1010070477, 1526478837, 1615893535}
//{758402484, 2033947360, 50330539, 448663599, 1552157794, -722372874, 1608393351, 779220703};
//{1791057005, -1783264727, 307020770, -2025795444, 399876863, 365673978, 1431702175, 930727640}
//{1062973774, -1866491801, -1593248661, 433124996, 2067236064, -1799480811, -71509729, 520814438};

//int main() {
//    AVL<int> a;
//    std::random_device rand;
//    for (int i=0 ; i<8 ; i++) {
////        ra[i] = rand();
//        a.insert(ra[i]);
//        std::cout << ra[i] << ' ';
//    }
//    std::cout.put('\n');
//
//    std::vector<int> output_order({0, 1, 2, 3, 4, 5, 6, 7});
//    int change_times = rand() % 10000000;
//    for (int i=0 ; i<change_times ; i++) std::next_permutation(output_order.begin(), output_order.end());
//
//    for (int i=0 ; i<=7 ; i++)
//        std::cout << output_order[i] << ':' << a.remove(ra[output_order[i]]) << ' ';
//
//    return 0;
//}

int main() {
    bTree<int, 4> bt;
    for (int i=0 ; i<10000 ; i++)
        bt.insert(i);

    std::cout << bt.find(9545) << ' ' << bt.find(10001);

    return 0;
}
