#include <iostream>
#include <string>

double mapper(int value, double sredn_)
{
	double value_ = value;
	value_ -= sredn_;
	value_ *= value_;
	return value_;
}

int main(int argc, char ** argv)
{
    std::string line;
    double sredn_ = std::atof(argv[1]);
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
		
    		try
		{
			int r = std::stoi(the_needly_data);
			std::cout << mapper(r, sredn_) << std::endl;
		}
		catch(...)
		{
			//std::cout << the_needly_data << " " << line << std::endl;
		}
    }

    return 0;
}
