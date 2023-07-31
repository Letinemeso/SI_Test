#include <ListRand.h>

using namespace SI_Test;


void ListRand::Serialize(std::ofstream& _file) const
{
    if(!_file.is_open())
        return;

    _file << Count << ' ';

    if(Head == nullptr)
        return;

    std::unordered_map<ListNode*, unsigned int> indices_hmap;

    ListNode* node_it = Head;
    for(int i = 0; i < Count; ++i)
    {
        indices_hmap.emplace(node_it, i);
        node_it = node_it->Next;
    }

    node_it = Head;
    for(int i = 0; i < Count; ++i)
    {
        _file << '"' << node_it->Data << '"' << ' ';
        node_it = node_it->Next;
    }

    node_it = Head;
    for(int i = 0; i < Count; ++i)
    {
        _file << indices_hmap.at(node_it->Rand) << ' ';
        node_it = node_it->Next;
    }
}

void ListRand::Deserialize(std::ifstream& _file)
{
    clear(*this);

    if(!_file.is_open())
        return;

    _file >> Count;

    if(Count == 0)
        return;

    ListNode** nodes = new ListNode*[Count];
    for(int i = 0; i < Count; ++i)
        nodes[i] = new ListNode();

    if(Count > 1)
    {
        for(int i = 0; i < Count - 1; ++i)
        {
            nodes[i]->Next = nodes[i + 1];
            nodes[i + 1]->Prev = nodes[i];
        }
    }

    Head = nodes[0];
    Tail = nodes[Count - 1];

    std::string read_value;
    for(int i = 0; i < Count; ++i)
    {
        _file >> read_value;

        if(read_value.size() < 3)
            continue;

        nodes[i]->Data = read_value.substr(1, read_value.size() - 2);
    }

    unsigned int random_index = 0;
    for(int i = 0; i < Count; ++i)
    {
        _file >> random_index;
        nodes[i]->Rand = nodes[random_index];
    }

    delete[] nodes;
}



void SI_Test::push(ListRand& _where, const std::string& _what)
{
    if (_where.Head == nullptr)
    {
        _where.Head = new ListNode;
        _where.Head->Data = _what;
        _where.Tail = _where.Head;
    }
    else
    {
        _where.Tail->Next = new ListNode;
        _where.Tail->Next->Data = _what;
        _where.Tail->Next->Prev = _where.Tail;
        _where.Tail = _where.Tail->Next;
    }

    ++_where.Count;
}

void SI_Test::pop(ListRand& _from)
{
    if(_from.Tail == nullptr)
        return;

    if(_from.Head == _from.Tail)
    {
        delete _from.Head;
        _from.Head = nullptr;
        _from.Tail = nullptr;
        _from.Count = 0;

        return;
    }

    ListNode* temp = _from.Tail;

    _from.Tail = _from.Tail->Prev;
    _from.Tail->Next = nullptr;
    --_from.Count;

    delete temp;
}

void SI_Test::clear(ListRand& _what)
{
    while(_what.Head != nullptr)
    {
        ListNode* current = _what.Head;

        _what.Head = _what.Head->Next;

        delete current;
    }

    _what.Tail = nullptr;
    _what.Count = 0;
}


void SI_Test::link_list_randomly(ListRand& _list)
{
    if(_list.Head == nullptr)
        return;

    if(_list.Head == _list.Tail)
        return;

    ListNode** nodes_in_order = new ListNode*[_list.Count];

    ListNode* node_it = _list.Head;
    for(int i = 0; i < _list.Count; ++i)
    {
        nodes_in_order[i] = node_it;
        node_it = node_it->Next;
    }

    for(int i = 0; i < _list.Count; ++i)
    {
        unsigned int random_index = rand() % _list.Count;
        nodes_in_order[i]->Rand = nodes_in_order[random_index];
    }

    delete[] nodes_in_order;
}
