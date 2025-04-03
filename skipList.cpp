#include "skipList.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <cassert>
#include <stdexcept>

namespace SkipListNS {

class SkipList::SkipListImpl
{
private:
    struct Node
    {
        int key;
        std::vector<Node*> forward; //points to next node
        Node(int key, int level) : key(key), forward(level+1, nullptr)
        {

        }
    };

    int MAXLVL;
    float P;
    int level;
    Node* header;

    friend class SkipList;

    //constructor
    SkipListImpl(int MAXLVL, float P) : MAXLVL(MAXLVL), P(P), level(0), header(new Node(-1, MAXLVL))
    {

    }

    //destructor
    ~SkipListImpl()
    {
        clear();
        delete header;
    }

    //methods
    void insertElement(int key)
    {
        Node* current = header;
        std::vector<Node*> update(MAXLVL+1, nullptr);

        for(int i = level; i >= 0; i--)
        {
            while(current->forward[i] && current->forward[i]->key < key)
            {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];
        if(!current || current->key != key)
        {
            int newLevel = 0;
            while((float)rand() / RAND_MAX < P && newLevel < MAXLVL)
            {
                newLevel++;
            }

            if(newLevel > level)
            {
                for(int i = level+1; i <= newLevel; i++)
                {
                    update[i] = header;
                }
                level = newLevel;
            }
            Node* newNode = new Node(key, newLevel);
            for(int i=0; i <= newLevel; i++)
            {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
        }
    }

    void removeElement(int key)
    {
        Node* current = header;
        std::vector<Node*> update(MAXLVL+1, nullptr);

        for(int i = level; i >= 0; i--)
        {
            while(current->forward[i] && current->forward[i]->key < key)
            {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];
        if(current && current->key == key)
        {
            for(int i=0; i <= level; i++)
            {
                if(update[i]->forward[i] != current)
                {
                    break;
                }
                update[i]->forward[i] = current->forward[i];
            }
            delete current;
            while(level > 0 && header->forward[level] == nullptr)
            {
                level--;
            }
        }
    }

    int search(int key) const
    {
        Node* current = header;
        for(int i = level; i >= 0; i--)
        {
            while(current->forward[i] && current->forward[i]->key < key)
            {
                current = current->forward[i];
            }
        }
        current = current->forward[0];
        if(current && current->forward[0]->key == key)
        {
            return key;
        }
        throw SkipListException("Key not found");
    }

    void displayList(std::ostream& os) const
    {
        for(int i=0; i <= level; i++)
        {
            Node* node = header->forward[i];
            os << "Level " << i << ": ";
            while(node)
            {
                os << node->key << " ";
                node = node->forward[i];
            }
            os << "\n";
        }
    }

    void clear()
    {
        Node* current = header->forward[0];
        while(current)
        {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
        std::fill(header->forward.begin(), header->forward.end(), nullptr);
        level = 0;
    }

    int getElementAt(int index) const
    {
        Node* current = header->forward[0];
        int count = 0;
        while(current)
        {
            if(count == index)
            {
                return current->key;
            }
            current = current->forward[0];
            count++;
        }
        throw SkipListException("Index out of range");
    }

    int getLevel() const
    {
        return level;
    }

    //for cloning, used in lines 207 and 217
    void copyFrom(const SkipListImpl& other)
    {
        clear();
        Node* current = other.header->forward[0];
        while(current)
        {
            insertElement(current->key);
            current = current->forward[0];
        }
    }
};

//constructor
SkipList::SkipList(int MAXLVL, float P) : pimpl(new SkipListImpl(MAXLVL, P))
{

}

//for cloning
SkipList::SkipList(const SkipList& other)
{
    pimpl = new SkipListImpl(other.pimpl->MAXLVL, other.pimpl->P);
    pimpl->copyFrom(*other.pimpl);
}

//for cloning
SkipList& SkipList::operator=(const SkipList& other)
{
    if(this != &other)
    {
        delete pimpl;
        pimpl = new SkipListImpl(other.pimpl->MAXLVL, other.pimpl->P);
        pimpl->copyFrom(*other.pimpl);
    }
    return *this;
}

//destructor
SkipList::~SkipList()
{
    delete pimpl;
}

//methods
void SkipList::insertElement(int key)
{
    pimpl->insertElement(key);
}

void SkipList::removeElement(int key)
{
    pimpl->removeElement(key);
}

int SkipList::search(int key) const
{
    return pimpl->search(key);
}

void SkipList::displayList(std::ostream& os) const
{
    pimpl->displayList(os);
}

//operators
SkipList& SkipList::operator+=(int key)
{
    insertElement(key);
    return *this;
}

SkipList& SkipList::operator-=(int key)
{
    removeElement(key);
    return *this;
}

int SkipList::operator[](int index) const
{
    return pimpl->getElementAt(index);
}

bool SkipList::operator<(const SkipList& other) const
{
    return pimpl->getLevel() < other.pimpl->getLevel();
}

bool SkipList::operator>(const SkipList& other) const
{
    return pimpl->getLevel() > other.pimpl->getLevel();
}

void SkipList::operator!()
{
    pimpl->clear();
}
//end of skipList methods and operators

//exception
SkipListException::SkipListException(const std::string& msg) : std::runtime_error(msg)
{

}

} //end of namespace
