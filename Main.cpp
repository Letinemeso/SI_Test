#include <iostream>
#include <ctime>

#include "List.h"

int main()
{
	List list;

	srand(time(nullptr));

	/*for (unsigned int i = 0; i < 100; ++i)
		list.push_back(std::to_string(rand()));*/


	list.deserialize("serialized_here.txt");


	List::Const_Iterator it = list.create_iterator();

	std::cout << *it << "\n";
	++it;
	for (unsigned int i = 1; i < list.size(); ++i)
	{
		List::Const_Iterator rnd = it;
		std::cout << *rnd << "\t";
		rnd.go_to_random();
		std::cout << *rnd << "\n";

		++it;
	}

	//list.serialize("serialized_here.txt");

	return 0;
}