/*
Author: Ieva Survilaite, VU Software Engineering 1 course 1 group
Data structure: skip list

A skip list is a linked-list probabilistic data structure, which has features of a sorted array (search, insertion and deletion).
*/

#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <iostream>
#include <vector>
#include <stdexcept>

namespace SkipListNS {

class Node
{
public:
    int key;

    std::vector<Node*> forward;
    Node(int key, int level);
};

class SkipList
{
    int MAXLVL; //max level for  skip list
    float P; // nodes dalis lygyje
    int level; //current lygis
    Node *header;
    void clear();

public:
    SkipList(int maxLevel, float probability);
    ~SkipList();
    SkipList(const SkipList& other); //clones list
    SkipList& operator=(const SkipList& other);

    void insertElement(int key);
    void removeElement(int key);
    int search(int key) const;
    void displayList(std::ostream& os = std::cout) const;

    SkipList& operator+=(int key); //veikia kaip insert
    SkipList& operator-=(int key); //veikia kaip remove
    int operator[](int index) const;
    bool operator<(const SkipList& other) const;
    bool operator>(const SkipList& other) const;
    void operator!();

};

class SkipListException : public std::runtime_error {
public:
    explicit SkipListException(const std::string& msg) : std::runtime_error(msg) {}
};

}

#endif // SKIPLIST_H
