#include "List.h"


List::~List()
{
	clear();
}



List::Iterator_Base::Iterator_Base(Node* _node) : m_node(_node) { }
List::Iterator_Base::Iterator_Base(const Iterator_Base& _other) : m_node(_other.m_node) { }

void List::Iterator_Base::operator=(const Iterator_Base& _other)
{
	m_node = _other.m_node;
}

void List::Iterator_Base::operator++()
{
	m_node = m_node->next;
}

void List::Iterator_Base::operator--()
{
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
	return Iterator(m_head);
}

List::Const_Iterator List::create_iterator() const
{
	return Const_Iterator(m_head);
}



List::Node* List::select_random_node()
{
	if (m_head == nullptr) return nullptr;
	
	if (m_count == 1)
	{
		unsigned int random_index = rand() % 2;
		if (random_index == 0) return m_head;
		else return m_head->next;
	}

	Iterator_Base it(m_head);
	unsigned int random_index = ((unsigned int)rand()) % (m_count - 1);
	for (unsigned int i = 0; i < random_index; ++i)
		++it;
	return it.m_node;
}



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
		m_tail->next->rand = select_random_node();
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
		m_tail->next->rand = select_random_node();
		m_tail = m_tail->next;
	}
	++m_count;
}



void List::insert_after(const Const_Iterator& _pos, const std::string& _data)
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

	current->rand = select_random_node();
}

void List::insert_after(const Const_Iterator& _pos, std::string&& _data)
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

	current->rand = select_random_node();
}

void List::erase(const Const_Iterator& _pos)
{
	if (_pos.m_iterator.m_node == nullptr) return;

	Node* current = _pos.m_iterator.m_node;

	--m_count;

	if (current == m_head)
	{
		Node* nptr = m_head;
		m_head = m_head->next;

		if (m_head != nullptr)
			m_head->prev = nullptr;
		else
			m_tail = nullptr;

		Iterator_Base it(m_head);
		for (unsigned int i = 0; i < m_count; ++i)
		{
			if (it.m_node->rand == nptr)
			{
				it.m_node->rand = select_random_node();
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

		Iterator_Base it(m_head);
		for (unsigned int i = 0; i < m_count; ++i)
		{
			if (it.m_node->rand == current)
			{
				it.m_node->rand = select_random_node();
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



void List::serialize(const std::string& _path)
{
	Serializer serializer;
	serializer.save(_path, *this);
}

void List::deserialize(const std::string& _path)
{
	Serializer serializer;
	serializer.load(_path, *this);
}



void List::Serializer::save(const std::string& _path, const List& _list) const
{
	std::ofstream file(_path, std::ios::trunc);
	if (!file.is_open()) return;
	
	file << _list.size() << ' ';
	
	std::map<Node*, unsigned int> map;

	List::Const_Iterator it = _list.create_iterator();
	for (unsigned int i = 0; i < _list.size(); ++i)
	{
		map.emplace(it.m_iterator.m_node, i);
		file << *it << ' ';
		++it;
	}
	it = _list.create_iterator();
	++it;
	for (unsigned int i = 1; i < _list.size(); ++i)
	{
		file << map.at(it.m_iterator.m_node->rand) << ' ';
		++it;
	}

	file.close();
}

void List::Serializer::load(const std::string& _path, List& _list) const
{
	std::ifstream file(_path, std::ios::in);
	if (!file.is_open()) return;

	unsigned int count;
	file >> count;

	Node** loaded_nodes = new Node * [count];

	for (unsigned int i = 0; i < count; ++i)
	{
		std::string loaded_data;
		file >> loaded_data;
		loaded_nodes[i] = new Node(std::move(loaded_data));
	}

	for (unsigned int i = 0; i < count - 1; ++i)
	{
		loaded_nodes[i]->next = loaded_nodes[i + 1];
		loaded_nodes[i]->next->prev = loaded_nodes[i];
	}

	for (unsigned int i = 1; i < count; ++i)
	{
		unsigned int loaded_rand_index;
		file >> loaded_rand_index;
		loaded_nodes[i]->rand = loaded_nodes[loaded_rand_index];
	}
	
	file.close();

	_list.clear();
	_list.m_head = loaded_nodes[0];
	_list.m_tail = loaded_nodes[count - 1];
	_list.m_count = count;

	delete[] loaded_nodes;
}