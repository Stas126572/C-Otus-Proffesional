#include <iostream>
#include <string>


int main(int argc, char ** argv)
{
    std::string line;
    while (std::getline(std::cin, line))
    {
		std::string the_needly_data;
		for (auto i : line)
		{
			if (i == ',')
			{
				the_needly_data = "";
			}
			else
			{
				the_needly_data += i;
			}
		}
    		int r = std::stoi(the_needly_data);
		std::cout << r << std::endl;
    }

    return 0;
}
