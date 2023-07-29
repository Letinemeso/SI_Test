#include "List.h"

using namespace SI_Test;


List::~List()
{
	clear();
}



List::Iterator_Base::Iterator_Base(const List* _parent, Node* _node) : m_parent(_parent), m_node(_node)
{
    if(_node == nullptr)
    {
        m_begin_reached = true;
        m_end_reached = true;
    }
    else if(_node == _parent->m_head)
        m_begin_reached = true;
    else if(_node == _parent->m_tail)
        m_end_reached = true;
}

List::Iterator_Base::Iterator_Base(const Iterator_Base& _other) : m_parent(_other.m_parent), m_node(_other.m_node), m_begin_reached(_other.m_begin_reached), m_end_reached(_other.m_end_reached) { }

void List::Iterator_Base::operator=(const Iterator_Base& _other)
{
	m_node = _other.m_node;
    m_begin_reached = _other.m_begin_reached;
    m_end_reached = _other.m_end_reached;
}

void List::Iterator_Base::operator++()
{
    if(m_end_reached)
        return;

    m_begin_reached = false;

    if(m_node->next == nullptr)
    {
        m_end_reached = true;
        return;
    }

	m_node = m_node->next;
}

void List::Iterator_Base::operator--()
{
    if(m_begin_reached)
        return;

    m_end_reached = false;

    if(m_node->prev == nullptr)
    {
        m_begin_reached = true;
        return;
    }

	m_node = m_node->prev;
}

void List::Iterator_Base::go_to_random()
{
	m_node = m_node->rand;
}


std::string& List::Iterator_Base::operator*()
{
	return m_node->data;
}

const std::string& List::Iterator_Base::operator*() const
{
	return m_node->data;
}

std::string* List::Iterator_Base::operator->()
{
	return &(m_node->data);
}

const std::string* List::Iterator_Base::operator->() const
{
	return &(m_node->data);
}



List::Iterator List::create_iterator()
{
    return Iterator(Iterator_Base(this, m_head));
}

List::Const_Iterator List::create_iterator() const
{
    return Const_Iterator(Iterator_Base(this, m_head));
}



//List::Node* List::select_random_node()
//{
//	if (m_head == nullptr) return nullptr;
	
//	if (m_count == 1)
//	{
//		unsigned int random_index = rand() % 2;
//		if (random_index == 0) return m_head;
//		else return m_head->next;
//	}

//	Iterator_Base it(m_head);
//	unsigned int random_index = ((unsigned int)rand()) % (m_count - 1);
//	for (unsigned int i = 0; i < random_index; ++i)
//		++it;
//	return it.m_node;
//}



unsigned int List::size() const
{
	return m_count;
}


void List::push_back(const std::string& _data)
{
	if (m_head == nullptr)
	{
		m_head = new Node(_data);
		m_tail = m_head;
	}
	else
	{
		m_tail->next = new Node(_data);
        m_tail->next->prev = m_tail;
		m_tail = m_tail->next;
	}
	++m_count;
}

void List::push_back(std::string&& _data)
{
	if (m_head == nullptr)
	{
		m_head = new Node(std::move(_data));
		m_tail = m_head;
	}
	else
	{
		m_tail->next = new Node(std::move(_data));
        m_tail->next->prev = m_tail;
		m_tail = m_tail->next;
	}
	++m_count;
}



void List::insert_after(const Iterator& _pos, const std::string& _data)
{
	if (_pos.m_iterator.m_node == nullptr) return;

	Node* current = _pos.m_iterator.m_node;
	Node* next = current->next;
	current->next = new Node(_data);
	current->next->prev = current;
	current = current->next;
	current->next = next;
	++m_count;
	
	if (current->next == nullptr)
        m_tail = current;
}

void List::insert_after(const Iterator& _pos, std::string&& _data)
{
	if (_pos.m_iterator.m_node == nullptr) return;

	Node* current = _pos.m_iterator.m_node;

	Node* next = current->next;
	current->next = new Node(std::move(_data));
	current->next->prev = current;
	current = current->next;
	current->next = next;
	++m_count;

	if (current->next == nullptr)
        m_tail = current;
}

void List::erase(const Iterator& _pos)
{
	if (_pos.m_iterator.m_node == nullptr) return;

	Node* current = _pos.m_iterator.m_node;

	--m_count;

    //  TODO: refactor this

	if (current == m_head)
	{
		Node* nptr = m_head;
		m_head = m_head->next;

		if (m_head != nullptr)
			m_head->prev = nullptr;
		else
			m_tail = nullptr;

        Iterator_Base it(this, m_head);
		for (unsigned int i = 0; i < m_count; ++i)
		{
			if (it.m_node->rand == nptr)
			{
                it.m_node->rand = it.m_node;
				break;
			}
			++it;
		}
		delete nptr;
	}
	else
	{
		current->next->prev = current->prev;
		current->prev->next = current->next;

        Iterator_Base it(this, m_head);
		for (unsigned int i = 0; i < m_count; ++i)
		{
			if (it.m_node->rand == current)
			{
                it.m_node->rand = it.m_node;
				break;
			}
			++it;
		}
		delete current;
	}
}


void List::clear()
{
	while (m_head != nullptr)
	{
		Node* nptr = m_head->next;
		delete m_head;
		m_head = nptr;
	}
	m_tail = nullptr;
	m_count = 0;
}



void List::link_randomly()
{
    if(m_head == nullptr)
        return;

    if(m_head == m_tail)
        return;

    Node** nodes_in_order = new Node*[m_count];

    Node* node_it = m_head;
    for(unsigned int i = 0; i < m_count; ++i)
    {
        nodes_in_order[i] = node_it;
        node_it = node_it->next;
    }

    for(unsigned int i = 0; i < m_count; ++i)
    {
        unsigned int random_index = rand() % m_count;
        nodes_in_order[i]->rand = nodes_in_order[random_index];
    }

    delete[] nodes_in_order;
}



void List::serialize(std::ofstream& _file) const
{
    if(!_file.is_open())
        return;

    _file << m_count << ' ';

    if(m_head == nullptr)
        return;

    std::unordered_map<Node*, unsigned int> indices_hmap;

    Node* node_it = m_head;
    for(unsigned int i = 0; i < m_count; ++i)
    {
        indices_hmap.emplace(node_it, i);
        node_it = node_it->next;
    }

    node_it = m_head;
    for(unsigned int i = 0; i < m_count; ++i)
    {
        _file << '"' << node_it->data << '"' << ' ';
        node_it = node_it->next;
    }

    node_it = m_head;
    for(unsigned int i = 0; i < m_count; ++i)
    {
        _file << indices_hmap.at(node_it->rand) << ' ';
        node_it = node_it->next;
    }
}

void List::deserialize(std::ifstream& _file)
{
    clear();

    if(!_file.is_open())
        return;

    _file >> m_count;

    if(m_count == 0)
        return;

    Node** nodes = new Node*[m_count];
    for(unsigned int i = 0; i < m_count; ++i)
        nodes[i] = new Node();

    if(m_count > 1)
    {
        for(unsigned int i = 0; i < m_count - 1; ++i)
        {
            nodes[i]->next = nodes[i + 1];
            nodes[i + 1]->prev = nodes[i];
        }
    }

    m_head = nodes[0];
    m_tail = nodes[m_count - 1];

    std::string read_value;
    for(unsigned int i = 0; i < m_count; ++i)
    {
        _file >> read_value;

        if(read_value.size() < 3)
            continue;

        nodes[i]->data = read_value.substr(1, read_value.size() - 2);
    }

    unsigned int random_index = 0;
    for(unsigned int i = 0; i < m_count; ++i)
    {
        _file >> random_index;
        nodes[i]->rand = nodes[random_index];
    }

    delete[] nodes;
}






















































