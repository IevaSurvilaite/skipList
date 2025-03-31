#include "skipList.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <cassert>
#include <stdexcept>

namespace SkipListNS {

//Node::Node(int key, int level) : key(key), forward(level+1, nullptr){}

class SkipList::SkipListImpl
{
private:
    struct Node
    {
        int key;
        std::vector<Node*> forward;
        Node(int key, int level) : key(key), forward(level+1, nullptr) {}
    };

    int MAXLVL;
    float P;
    int level;
    Node* header;
public:
    SkipListImpl(int MAXLVL, float P) : MAXLVL(MAXLVL), P(P), level(0), header(new Node(-1, MAXLVL)) {}
    ~SkipListImpl()
    {
        clear();
        delete header;
    }

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
        if(current && current->key == key)
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

    SkipListImpl(const SkipListImpl& other)
    {
        MAXLVL = other.MAXLVL;
        P = other.P;
        level = other.level;
        header = new Node(-1, MAXLVL);

        Node* current = other.header->forward[0];
        while(current)
        {
            insertElement(current->key);
            current = current->forward[0];
        }
    }

    SkipListImpl& operator=(const SkipListImpl& other)
    {
        if(this == &other) return *this;

        clear();
        MAXLVL = other.MAXLVL;
        P = other.P;
        level = other.level;

        Node* current = other.header->forward[0];
        while(current)
        {
            insertElement(current->key);
            current = current->forward[0];
        }
        return *this;
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

    int getLevel() const { return level;}

};

SkipList::SkipList(int MAXLVL, float P) : pimpl(new SkipListImpl(MAXLVL, P)) {}

SkipList::SkipList(const SkipList& other)
{
    pimpl = new SkipListImpl(*other.pimpl);
}

SkipList& SkipList::operator=(const SkipList& other)
{
    if(this != &other)
    {
        *pimpl = *other.pimpl;
    }
    return *this;
}

SkipList::~SkipList() { delete pimpl;}

void SkipList::insertElement(int key) { pimpl->insertElement(key);}
void SkipList::removeElement(int key) { pimpl->removeElement(key);}
int SkipList::search(int key) const { return pimpl->search(key);}
void SkipList::displayList(std::ostream& os) const {pimpl->displayList(os);}

void SkipList::operator!()
{
    pimpl->clear();
}

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

}
