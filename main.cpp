#include <iostream>
#include <ctime>

#include <map>

#include "ListRand.h"

void ListRand_serialization_example()
{
    std::cout << "ListRand serialization example: \n\n";

    SI_Test::ListRand list;

    unsigned int char_offset = 'Z' - 'A';

    unsigned int random_amount = rand() % 25;
    for (unsigned int i = 0; i < random_amount; ++i)
    {
        unsigned int random_string_size = rand() % 20;

        std::string str;
        str.resize(random_string_size);

        for(unsigned int str_i = 0; str_i < random_string_size; ++str_i)
            str[str_i] = 'A' + (rand() % (char_offset));

        SI_Test::push(list, str);
    }

    SI_Test::link_list_randomly(list);

    std::cout << "Random generated data in the list: \n";

    SI_Test::ListNode* node_it = list.Head;
    while (node_it != nullptr)
    {
        SI_Test::ListNode* rnd = node_it;
        std::cout << "Current Node's Data: " << rnd->Data << "\n";
        rnd = rnd->Rand;
        std::cout << "Current's Random Node's Data: " << rnd->Data << "\n";

        node_it = node_it->Next;
    }

    std::ofstream file;
    file.open("serialized.txt", std::ios::binary);

    list.Serialize(file);

    file.close();

    SI_Test::clear(list);

    std::ifstream result_file;
    result_file.open("serialized.txt", std::ios::binary);

    std::cout << "\nResult content of the file:\n";

    while(!result_file.eof())
        std::cout << (char)result_file.get();

    result_file.close();

    std::cout << "\n\nSerialization data saved to \"serialized.txt\"\n\n";
}

void ListRand_deserialization_example()
{
    std::cout << "ListRand deserialization example: \n\n";

    std::ifstream input_file;
    input_file.open("serialized.txt", std::ios::binary);

    if(!input_file.is_open())
    {
        std::cout << "Failed to open \"serialized.txt\"! Unable to continue test\n\n";
        return;
    }

    std::cout << "Raw content of the file \"serialized.txt\":\n";

    while(!input_file.eof())
        std::cout << (char)input_file.get();

    input_file.close();

    std::cout << "\n\n";

    input_file.open("serialized.txt", std::ios::binary);

    SI_Test::ListRand list;

    list.Deserialize(input_file);

    input_file.close();

    std::cout << "Deserialized data in the list: \n";

    SI_Test::ListNode* node_it = list.Head;
    while (node_it != nullptr)
    {
        SI_Test::ListNode* rnd = node_it;
        std::cout << "Current Node's Data: " << rnd->Data << "\n";
        rnd = rnd->Rand;
        std::cout << "Current's Random Node's Data: " << rnd->Data << "\n";

        node_it = node_it->Next;
    }

    SI_Test::clear(list);

    std::cout << "\n\n";
}


int main()
{
    srand(time(nullptr));

    unsigned int action_input = 1;
    while(action_input < 3 && action_input > 0)
    {
        std::cout << "Choose one of the following options:\n";
        std::cout << "1: Serialization of the list with random generated data\n";
        std::cout << "2: Deserialization of the list from file\n";
        std::cout << "Any other number: stop the test\n\n";

        std::cin >> action_input;

        std::cout << "\n";

        if(action_input == 1)
            ListRand_serialization_example();
        else if(action_input == 2)
            ListRand_deserialization_example();
    }

    return 0;
}
