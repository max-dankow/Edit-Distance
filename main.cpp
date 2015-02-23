#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <algorithm>

/*size_t get_index(const size_t x, const size_t y, const size_t n)
{
    return y * n + x;
}

void write_matrix(const std::vector<long long> &data, const size_t n, const size_t m)
{
    for (size_t i = 0; i < m; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            printf("%5lld", data[get_index(j, i, n)]);
        }
        printf("\n");
    }

    printf("\n");
}

void restore_way(const std::vector<long long> &data, const size_t n, const size_t m, const size_t x, const size_t y)
{
    if (x + y != 0)
    {
        if ()
    }
}

long long edit_distance(const std::string &s1, const std::string &s2)
{

    size_t n = s1.size() + 1, m = s2.size() + 1;
    std::vector<long long> distance(n * m);


    distance[get_index(0, 0, n)] = 0;

    for (size_t i = 1; i < m; ++i)
        distance[get_index(0, i, n)] = distance[get_index(0, i - 1, n)] + 1;

    for (size_t i = 1; i < n; ++i)
        distance[get_index(i, 0, n)] = distance[get_index(i - 1, 0, n)] + 1;


    for (size_t j = 1; j < m; ++j)
    {
        for (size_t i = 1; i < n; ++i)
        {
            distance[get_index(i, j, n)] = std::min(distance[get_index(i - 1, j, n)] + 1, distance[get_index(i, j - 1, n)] + 1);
            distance[get_index(i, j, n)] = std::min(distance[get_index(i, j, n)],
                distance[get_index(i - 1, j - 1, n)] + ((s1[i - 1] == s2[j - 1])? 0 : 1));
        }
    }
    write_matrix(distance, n, m);
    restore_way(distance, n, m, n - 1, m - 1);

    return distance[get_index(n - 1, m - 1, n)];
}*/

long long edit_distance_mod(const std::string &s1, const std::string &s2, std::vector<std::pair<size_t, size_t> > &result, int ox, int oy, bool again)
{

    if ((s1.size() == 0)||(s2.size() == 0))
        return 0;
    size_t middle = (s1.size() + 1) / 2;
    std::vector<long long> distance(s2.size() + 1);

    for (size_t i = 0; i < distance.size(); ++i)
        distance[i] = i;

    for (size_t i = 1; i <= middle; ++i)
    {
        long long last = distance[0] + 1;

        for (size_t j = 1; j < distance.size(); ++j)
        {
            long long temp =  std::min(last + 1, std::min(distance[j] + 1, distance[j - 1] + ((s1[i - 1] == s2[j - 1])? 0 : 1)));
            distance[j - 1] = last;
            last = temp;
        }

        distance[distance.size() - 1] = last;
    }

    std::vector<long long> re_distance(s2.size() + 1);

    for (size_t i = 0; i < re_distance.size(); ++i)
        re_distance[i] = re_distance.size() - i - 1;

    for (size_t i = s1.size() - 1; i >= middle; --i)
    {
        long long last = re_distance[re_distance.size() - 1] + 1;

        for (std::ptrdiff_t j = re_distance.size() - 2; j >= 0; --j)
        {
            long long temp =  std::min(last + 1, re_distance[j] + 1);
            //assert()
            if (i >= 1 && j>= 1)
            {
                temp = std::min(re_distance[j + 1] + ((s1[i - 1] == s2[j - 1])? 0 : 1), temp);
            }
            else
            {
                temp = std::min(re_distance[j + 1] + 1, temp);
            }


            re_distance[j + 1] = last;
            last = temp;
        }

        re_distance[0] = last;
    }

    size_t min_index = 0;

    for (size_t i = 1; i < distance.size(); ++i)
    {
        if (distance[i] + re_distance[i] < distance[min_index] + re_distance[min_index] )
            min_index = i;
    }
    //result[middle] = min_index;
    bool now = ((middle == 1) && (min_index == 1));
    /*if ((middle == 1) || (min_index == 1))
    {
        local_count++;
    }*/
    std::cout << s1 << ' ' << s2  /*<< now << ' ' << again*/ << '\n';
    std::cout << middle + ox <<' '<< min_index + oy << std::endl;
    //if (!(now && again))
    //{
        std::string next1 = std::string(s1, 0, middle);
        std::string next2 = std::string(s2, 0, min_index);
        if (s1 != next1 || s2 != next2)
           edit_distance_mod(next1, next2, result, ox, oy, now);
        //std::cout << middle + ox <<' '<< min_index + oy << std::endl;
        result.push_back(std::make_pair(middle + ox, min_index + oy));

        if (min_index != 0)
        {
            next1 = std::string(s1, middle - 1, s1.size() - middle + 1);
            next2 = std::string(s2, min_index - 1, s2.size() - min_index + 1);

            if (s1 != next1 || s2 != next2)
                edit_distance_mod(next1, next2, result, ox + middle - 1, oy + min_index - 1, now);
        }

    //}
    /*else
    {
        //if ((result.empty()) || (result.back() != std::make_pair(middle + ox, min_index + oy) ) )
         //   result.push_back(std::make_pair(middle + ox, min_index + oy));7
    }*/

    return result.size();
}

int main()
{
    std::string s1 = "creator", s2 = "carto";
    //std::cout << papa << " -> " << mama << std::endl << std::endl;
    //edit_distance(papa, mama);
    std::vector<std::pair<size_t, size_t> > way;//wrong
    edit_distance_mod(s1, s2, way, 0, 0, false);
    //std::vector<std::vector<int>> _map()
    for (auto it = way.begin(); it != way.end(); ++it)
    {
        std::cout << (*it).first << ' ' << (*it).second << '\n';
    }
    return 0;
}
