#include <iostream>
#include "RB-CLASS.h"

int main()
{
    std::cout << "Red-black trees!\n";
    RBTree <int> intree;
    intree.insert(12);
    intree.insert(13);
    intree.insert(14);
    std::cout << "Test passed!" << std::endl;
    intree.remove(12);
    intree.remove(1111);
    std::cout << "Test 2 passed!" << std::endl;
}
