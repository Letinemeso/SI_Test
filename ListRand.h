#ifndef LISTRAND_H
#define LISTRAND_H

#include <fstream>
#include <string>
#include <unordered_map>

namespace SI_Test
{

    struct ListNode
    {
        ListNode* Prev = nullptr;
        ListNode* Next = nullptr;
        ListNode* Rand = this;
        std::string Data;
    };



    class ListRand
    {
    public:
        ListNode* Head = nullptr;
        ListNode* Tail = nullptr;
        int Count = 0;

    public:
        void Serialize(std::ofstream& _file) const;
        void Deserialize(std::ifstream& _file);

    };



    //  creating additional fields wasn't allowed neither for ListRand nor ListNode, so theese functions will provide necessary logic

    void push(ListRand& _where, const std::string& _what);
    void pop(ListRand& _from);
    void clear(ListRand& _what);

    void link_list_randomly(ListRand& _list);

}

#endif // LISTRAND_H
