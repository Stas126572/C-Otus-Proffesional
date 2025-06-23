#include <iostream>
#include <string>

std::pair<double, bool> mapper(double needly_data, std::string line)
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
        int value = std::stoi(the_needly_data);
        double value_ = value;
        value_ -= needly_data;
        value_ *= value_;
        return std::make_pair(value_, true);
     }
     catch(...)
     {
        return std::make_pair(0, false);
         //std::cout << the_needly_data << " " << line << std::endl;
     }


}

int main(int argc, char ** argv)
{
    std::string line;
    double needly_data = std::atof(argv[1]);
    while (std::getline(std::cin, line))
    {
        bool IsCorrect;
        auto res = mapper(needly_data, line);
        std::tie(std::ignore, IsCorrect) = res;
        if (IsCorrect)
        {
            double print_res;
            std::tie(print_res, std::ignore) = res;
            std::cout << print_res;
        }
    }

    return 0;
}
