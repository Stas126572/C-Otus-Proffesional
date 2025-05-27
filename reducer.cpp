#include <iostream>
#include <string>

int main(int argc, char ** argv)
{
    size_t count = 0;
    size_t sum = 0;
    std::string line;
    while (std::getline(std::cin, line))
    {
	int number_ = std::stoi(line);
	sum += number_;
	count++;
    }
  
    double sredn_ = sum / count;
    std::cout << sredn_ << std::endl;
    return 0;
}
