#include <iostream>
#include <string>
#include <tuple>

std::tuple<size_t, size_t, double> reducer(std::tuple<size_t, size_t, double> prev_data, std::string line)
{
    size_t count, sum;
    std::tie(count, sum, std::ignore) = prev_data;
    int number = std::stoi(line);
    count++;
    sum += number;
    return std::make_tuple(count, sum, sum / count);
}


int main(int argc, char ** argv)
{
    std::tuple<size_t, size_t, double> previus_state;
    std::string line;
    while (std::getline(std::cin, line))
    {
        previus_state = reducer(previus_state, line);
    }

    double res;
    std::tie(std::ignore, std::ignore, res) = previus_state;
    std::cout << res << std::endl;
    return 0;
}
