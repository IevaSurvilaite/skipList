#include "skipList.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>

using namespace SkipListNS;

int main()
{
    try
    {
        SkipList list(4, 0.5);

        list += 2;
        list += 25;
        list += 21;
        list += 5;
        list += 10;
        list += 15;
        list += 13;
        list += 18;
        std::cout << "Skip list after insertions:" << std::endl;
        list.displayList(std::cout);

        list -= 2;
        list -= 13;
        list -= 25;
        std::cout << "Skip list after deletions:" << std::endl;
        list.displayList(std::cout);

        std::cout << "Found 15 at index: " << list.search(15) << std::endl;

        !list;
        std::cout << "Skip list after clearing:" << std::endl;
        list.displayList(std::cout);

        std::cout << "The program has finished working." << std::endl;

    }
    catch(const SkipListException &e)
    {
        std::cout << "SkipListException: " << e.what() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cout << "std::exception: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "An unexpected error occured" << std::endl;
    }

    return 0;
}
