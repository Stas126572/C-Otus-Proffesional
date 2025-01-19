#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]

using st = std::string;
auto split(const st& str, char d)
{
    std::vector<st> r;

    st::size_type start = 0;
    st::size_type stop = str.find_first_of(d);
    while (stop != st::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

template<typename T, typename T2>
std::vector<T2>  filter_with_predicate(std::vector<T2> source, T func)
{
    std::vector<T2> vector;

    for (auto ip : source)
    {
        if (func(ip) == true)
        {
            vector.push_back(ip);
        }
    }

    return vector;
}

class IP
{
    char* mas;
    int size;

public:


    friend std::ostream& operator<<(std::ostream& os, IP ip);

    IP(std::vector<std::string> vec)
    {
        size = vec.size();
        mas = new  char[vec.size()];
        for (auto i = 0; i < vec.size(); i++)
        {
            const char* s = vec[i].c_str();
            mas[i] = (atoi(s)-128);
        }
    }

    int operator[] (unsigned int s)
    {
        return ((int)mas[s]+128);
    }

    bool operator <(IP& second_ip)
    {
        for (int i = 0; i < size; i++)
        {
            if (second_ip.mas[i] != mas[i])
            {
                return mas[i] < second_ip.mas[i];
            }
        }
        return false;
    }

    int get_size()
    {
        return size;
    }
};

std::ostream& operator<<(std::ostream& os, IP ip)
{
    for (auto i = 0; i < ip.size; i++)
    {
        if (i != 0)
        {
            os << '.';
        }
        os << std::to_string(((int)ip[i]));
    }

    return os;
}


int main()
{
    try
    {
        std::vector<IP > ip_pool;

        for (st line; std::getline(std::cin, line);)
        {
            auto v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }

        // TODO reverse lexicographically sort

        std::sort(ip_pool.begin(), ip_pool.end());

        for (auto ip : ip_pool)
        {
            std::cout << ip << std::endl;
        }


        auto filter = [](auto source, std::string& s )
        {
            return filter_with_predicate(source,[s]
                                         (IP v)
                                         {
                                             for (auto i = 0; i < s.size(); i++)
                                             {
                                                 if (v[i] != s[i])
                                                 {
                                                     return false;
                                                 }
                                             }
                                             return true;

                                         }
                                         );
        };


        auto filter_any = [](auto source, char s)
        {
            return filter_with_predicate(source,
                                         [s](IP v)
                                         {
                                             for (auto i = 0; i < v.get_size(); i++)
                                             {
                                                 if (v[i] == s)
                                                 {
                                                     return true;
                                                 }
                                             }
                                             return false;
                                         }
                                         );
        };

        std::string str = { 1 };

        for (auto ip : filter(ip_pool, str))
        {
            std::cout << ip << std::endl;
        }


        std::string str2 = {46, 70};

        for (auto ip : filter(ip_pool, str2))
        {
            std::cout << ip << std::endl;
        }

        for (auto ip : filter_any(ip_pool, 46))
        {
            std::cout << ip << std::endl;
        }
        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
