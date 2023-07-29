#ifndef LIST_H
#define LIST_H

#include <string>
#include <memory>
#include <fstream>
#include <unordered_map>


namespace SI_Test
{

    class List final
    {
    private:
        struct Node final
        {
            Node* next = nullptr;
            Node* prev = nullptr;
            Node* rand = this;

            std::string data;

            Node() { }
            Node(const std::string& _data) : data(_data) { }
            Node(std::string&& _data) : data(std::move(_data)) { }
        };

    private:
        Node* m_head = nullptr;
        Node* m_tail = nullptr;
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
            const List* m_parent = nullptr;

            Node* m_node = nullptr;

            bool m_begin_reached = false;
            bool m_end_reached = false;

        public:
            Iterator_Base(const List* _parent, Node* _node);
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

        public:
            inline bool begin_reached() const { return m_begin_reached; }
            inline bool end_reached() const { return m_end_reached; }

        };

    public:
        class Const_Iterator;

        class Iterator final
        {
        private:
            friend class Const_Iterator;
            friend class List;

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

        public:
            inline bool begin_reached() const { return m_iterator.begin_reached(); }
            inline bool end_reached() const { return m_iterator.end_reached(); }

        };

        class Const_Iterator final
        {
        private:
            friend class List;
            friend class Iterator;

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

        public:
            inline bool begin_reached() const { return m_iterator.begin_reached(); }
            inline bool end_reached() const { return m_iterator.end_reached(); }

        };

    public:
        Iterator create_iterator();
        Const_Iterator create_iterator() const;

    public:
        unsigned int size() const;

        void push_back(const std::string& _data);
        void push_back(std::string&& _data);

        void insert_after(const Iterator& _pos, const std::string& _data);
        void insert_after(const Iterator& _pos, std::string&& _data);
        void erase(const Iterator& _pos);

        void clear();

    public:
        void link_randomly();

    public:
        void serialize(std::ofstream& _file) const;
        void deserialize(std::ifstream& _file);

    };

}


#endif
