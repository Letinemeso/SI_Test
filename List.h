#ifndef __LIST
#define __LIST

#include <fstream>
#include <string>
#include <memory>

#include <map>
#include <vector>

class List final
{
private:
	struct Node final
	{
		Node* next = nullptr, * prev = nullptr, * rand = nullptr;
		std::string data;
		Node(const std::string& _data) : data(_data) { }
		Node(std::string&& _data) : data(std::move(_data)) { }
	};

private:
	Node* m_head = nullptr, * m_tail = nullptr;
	unsigned int m_count = 0;

public:
	List() { }
	List(const List& _other) = delete;
	List(List&& _other) = delete;
	~List();

private:
	class Iterator_Base final
	{
	private:
		friend class List;

	private:
		Node* m_node = nullptr;

	public:
		Iterator_Base(Node* _node);
		Iterator_Base(const Iterator_Base& _other);
		void operator=(const Iterator_Base& _other);

	public:
		void operator++();
		void operator--();
		void go_to_random();

		std::string& operator*();
		const std::string& operator*() const;
		std::string* operator->();
		const std::string* operator->() const;

	};

public:
	class Const_Iterator;

	class Iterator final
	{
	private:
		friend class Const_Iterator;
		friend class List;
		friend class Serializer;

	private:
		Iterator_Base m_iterator;

	private:
		Iterator(const Iterator_Base& _iterator) : m_iterator(_iterator) { }
	public:
		Iterator(const Iterator& _other) : m_iterator(_other.m_iterator) { }
		void operator=(const Iterator& _other) { m_iterator = _other.m_iterator; }

	public:
		void operator++() { ++m_iterator; }
		void operator--() { --m_iterator; }
		void go_to_random() { m_iterator.go_to_random(); }

		std::string& operator*() { return *m_iterator; }
		const std::string& operator*() const { return *m_iterator; }
		std::string* operator->() { return m_iterator.operator->(); }
		const std::string* operator->() const { return m_iterator.operator->(); }

	};

	class Const_Iterator final
	{
	private:
		friend class List;
		friend class Iterator;
		friend class Serializer;

	private:
		Iterator_Base m_iterator;

	private:
		Const_Iterator(const Iterator_Base& _iterator) : m_iterator(_iterator) { }
	public:
		Const_Iterator(const Const_Iterator& _other) : m_iterator(_other.m_iterator) { }
		Const_Iterator(const Iterator& _other) : m_iterator(_other.m_iterator) { }
		void operator=(const Const_Iterator& _other) { m_iterator = _other.m_iterator; }
		void operator=(const Iterator& _other) { m_iterator = _other.m_iterator; }

	public:
		void operator++() { ++m_iterator; }
		void operator--() { --m_iterator; }
		void go_to_random() { m_iterator.go_to_random(); }

		const std::string& operator*() const { return *m_iterator; }
		const std::string* operator->() const { return m_iterator.operator->(); }

	};

public:
	Iterator create_iterator();
	Const_Iterator create_iterator() const;

private:
	Node* select_random_node();

public:
	unsigned int size() const;

	void push_back(const std::string& _data);
	void push_back(std::string&& _data);

	void insert_after(const Const_Iterator& _pos, const std::string& _data);
	void insert_after(const Const_Iterator& _pos, std::string&& _data);
	void erase(const Const_Iterator& _pos);

	void clear();

	void serialize(const std::string& _path);
	void deserialize(const std::string& _path);

private:
	class Serializer
	{
	public:
		void save(const std::string& _path, const List& _list) const;	// n*log(n)
		void load(const std::string& _path, List& _list) const;			// n

	};
	friend class Serializer;

};


#endif