/*
Author: Ieva Survilaite, VU Software Engineering 1 course 1 group
Data structure: skip list

A skip list is a linked-list probabilistic data structure, which has features of a sorted array (search, insertion and deletion).
*/

#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <memory>
#include <iostream>
#include <vector>
#include <stdexcept>

namespace SkipListNS {

class SkipList
{
private:
    class SkipListImpl;
    SkipListImpl* pimpl;
public:
    SkipList(int maxLevel, float probability); //constructor
    SkipList(const SkipList& other); //clones list
    SkipList& operator=(const SkipList& other);
    ~SkipList(); //destructor

    //methods
    void insertElement(int key);
    void removeElement(int key);
    int search(int key) const;
    void displayList(std::ostream& os = std::cout) const;

    //operators
    SkipList& operator+=(int key); //veikia kaip insert
    SkipList& operator-=(int key); //veikia kaip remove
    int operator[](int index) const;
    bool operator<(const SkipList& other) const;
    bool operator>(const SkipList& other) const;
    void operator!();
};

class SkipListException : public std::runtime_error {
public:
    explicit SkipListException(const std::string& msg);
};

}

#endif // SKIPLIST_H
