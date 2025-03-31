#include "skipList.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>

using namespace SkipListNS;

void logAndPrint(std::ostream& out, const std::string& message)
{
    std::cout << message << std::endl;
    out << message << std::endl;
}

int main()
{
    std::ofstream file("test_results.txt");
    if(!file)
    {
        std::cerr << "Failed to open test results.txt\n";
        return 1;
    }

    try
    {
        SkipList list(3, 0.5);

        logAndPrint(file, "Initial empty list:");
        list.displayList(std::cout);
        list.displayList(file);

        logAndPrint(file, "Inserting elements: 3, 6, 7, 9, 12");
        list += 3;
        list += 6;
        list += 7;
        list += 9;
        list += 12;
        list.displayList(std::cout);
        list.displayList(file);

        SkipList clone(list);
        logAndPrint(file, "Cloned list:");
        list.displayList(std::cout);
        list.displayList(file);

        logAndPrint(file, "Removing element 6");
        list -= 6;
        list.displayList(std::cout);
        list.displayList(file);

        logAndPrint(file, "Searching for element 9 (should be found)");
        assert(list.search(9) == 9);
        logAndPrint(file, "Element 9 found");

        logAndPrint(file, "Searching for element 6 (shouldn't be found)");
        assert(list.search(6) == -1);
        logAndPrint(file, "Element 6 not found.");

        logAndPrint(file, "Clearing list with !");
        !list;
        list.displayList(std::cout);
        list.displayList(file);

        logAndPrint(file, "All tests passed");

    }
    catch(const SkipListException &e)
    {
        logAndPrint(file, std::string("SkipListException: ") + e.what());
    }
    catch(const std::exception& e)
    {
        logAndPrint(file, std::string("std::exception: ") + e.what());
    }
    catch(...)
    {
        logAndPrint(file, "An unexpected error occured");
    }

    file.close();
    return 0;
}
