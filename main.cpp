#include <iostream>
#include <ctime>

#include "List.h"

int main()
{
    srand(time(nullptr));

    SI_Test::List list;

//    for (unsigned int i = 0; i < 10; ++i)
//    {
//        std::string str;
//        str.push_back('A' + i);
//        list.push_back(str);
//    }
//    list.link_randomly();

    std::ifstream file;
    file.open("serialized.txt", std::ios::binary);

    list.deserialize(file);

    file.close();

    SI_Test::List::Const_Iterator it = list.create_iterator();

    for (unsigned int i = 0; !it.end_reached(); ++i)
	{
        SI_Test::List::Const_Iterator rnd = it;
        std::cout << *rnd << "\t";
        rnd.go_to_random();
        std::cout << *rnd << "\n";

        ++it;
    }

//    std::ofstream file;
//    file.open("serialized.txt", std::ios::binary);

//    list.serialize(file);

    file.close();

    return 0;
}
