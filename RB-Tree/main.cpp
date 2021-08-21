#include <iostream>
#include "RB-CLASS.h"

//insert 12, 13, 14, 11, 17 remove 13, 14 insert 13 remove 14

int main()
{
    std::cout << "Red-black trees!\n";
    RBTree <int> intree;
    intree.insert(12);
    intree.insert(13);
    intree.insert(14);
    intree.insert(11);
    intree.insert(17);
    intree.remove(13);
    intree.remove(14);
    intree.insert(13);
    intree.remove(14);
    std::cout << "Test passed!" << std::endl;
}
