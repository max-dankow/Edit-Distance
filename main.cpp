#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <assert.h>

const bool OUTPUT_MODE = 0;

size_t get_index(const size_t x, const size_t y, const size_t n)
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

void restore_way(const std::vector<long long> &data,
                 std::vector<std::pair<size_t, size_t> > &dst_way,
                 const size_t n, const size_t m, const size_t x, const size_t y)
{
    if (x + y != 0)
    {
        size_t next_y = data[get_index(x, y, n)] / n;
        size_t next_x = data[get_index(x, y, n)] % n;
        restore_way(data, dst_way, n, m, next_x, next_y);
        dst_way.push_back(std::make_pair(next_x, next_y));
        //dst_way[0].first = 99;
        //std::cout << next_x << ' ' << next_y << dst_way[0].first << '\n';
    }
}

long long edit_distance(const std::string &s1, const std::string &s2,
                        std::vector<std::pair<size_t, size_t> > &dst_way)
{
    size_t n = s1.size() + 1, m = s2.size() + 1;
    std::vector<long long> distance(n * m);
    std::vector<long long> way(n * m);

    distance[get_index(0, 0, n)] = 0;
    way[get_index(0,0,n)] = get_index(0,0,n);

    for (size_t i = 1; i < m; ++i)
    {
        distance[get_index(0, i, n)] = distance[get_index(0, i - 1, n)] + 1;
        way[get_index(0, i, n)] = get_index(0, i - 1, n);
    }

    for (size_t i = 1; i < n; ++i)
    {
        distance[get_index(i, 0, n)] = distance[get_index(i - 1, 0, n)] + 1;
        way[get_index(i, 0, n)] = get_index(i - 1, 0, n);
    }

    //write_matrix(way, n, m);

    for (size_t j = 1; j < m; ++j)
    {
        for (size_t i = 1; i < n; ++i)
        {
            distance[get_index(i, j, n)] = distance[get_index(i - 1, j, n)] + 1;
            way[get_index(i, j, n)] = get_index(i - 1, j, n);

            if (distance[get_index(i, j, n)] > distance[get_index(i, j - 1, n)] + 1)
            {
                distance[get_index(i, j, n)] = distance[get_index(i, j - 1, n)] + 1;
                way[get_index(i, j, n)] = get_index(i, j - 1, n);
            }

            if (distance[get_index(i, j, n)] > distance[get_index(i - 1, j - 1, n)] + ((s1[i - 1] == s2[j - 1])? 0 : 1))
            {
                distance[get_index(i, j, n)] = distance[get_index(i - 1, j - 1, n)] + ((s1[i - 1] == s2[j - 1])? 0 : 1);
                way[get_index(i, j, n)] = get_index(i - 1, j - 1, n);
            }
        }
    }
    //write_matrix(distance, n, m);
    //write_matrix(way, n, m);
    dst_way.clear();
    restore_way(way, dst_way, n, m, n - 1, m - 1);
    dst_way.push_back(std::make_pair(s1.size(), s2.size()));

    return distance[get_index(n - 1, m - 1, n)];
}

long long edit_distance_mod(const std::string &s1, const std::string &s2,
                            std::vector<std::pair<size_t, size_t> > &result,
                            int offset_x, int offset_y)
{

    //if ((s1.size() <= 1) || (s2.size() <= 1))
    //    return 0;
    //assert(s1.size() != 0);
    size_t middle = (s1.size()) / 2;
    std::vector<long long> distance(s2.size());

    for (size_t i = 0; i < distance.size(); ++i)
        distance[i] = i;

    for (size_t i = 1; i <= middle; ++i)
    {
        long long last = distance[0] + 1;

        for (size_t j = 1; j < distance.size(); ++j)
        {
            long long temp =  std::min(last + 1, std::min(distance[j] + 1, distance[j - 1] + ((s1[i] == s2[j])? 0 : 1)));
            distance[j - 1] = last;
            last = temp;
        }

        distance[distance.size() - 1] = last;
    }

    std::vector<long long> re_distance(s2.size());

    for (size_t i = 0; i < re_distance.size(); ++i)
        re_distance[i] = re_distance.size() - i - 1;

    for (size_t i = s1.size() - 2; i >= middle; --i)
    {
        long long last = re_distance[re_distance.size() - 1] + 1;

        for (std::ptrdiff_t j = re_distance.size() - 2; j >= 0; --j)
        {
            long long temp =  std::min(last + 1, re_distance[j] + 1);
            temp = std::min(re_distance[j + 1] + ((s1[i] == s2[j])? 0 : 1), temp);//check the length
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

    //std::cout << s1 << ' ' << s2  << '\n';
    //std::cout << middle + offset_x <<' '<< min_index + offset_y << std::endl;

    std::pair<std::string, std::string> substrings[2];

    substrings[0].first = std::string(s1, 0, middle + 1);
    substrings[0].second = std::string(s2, 0, min_index + 1);
    substrings[1].first = std::string(s1, middle, s1.size() - middle + 1);
    substrings[1].second = std::string(s2, min_index, s2.size() - min_index + 1);

    //assert((substrings[0].first[0] != char(1)));
    if ((substrings[0].first == s1 && substrings[0].second == s2) &&
            (substrings[1].first.size() <= 1 && substrings[1].second.size() <= 1))
    {
        return 0;
    }

    /*if (!(substrings[0].first.size() + ((substrings[0].first[0] == char(1))? -1 : 0) <= 1 &&
        substrings[0].second.size() + ((substrings[0].second[0] == char(1))? -1 : 0) <= 1))
    {*/
        edit_distance_mod(substrings[0].first, substrings[0].second, result, offset_x, offset_y);
    //}
    //std::cout << "ADDED :";
    //std::cout << middle + offset_x <<' '<< min_index + offset_y << std::endl;
    result.push_back(std::make_pair(middle + offset_x, min_index + offset_y));

    //if (!(substrings[1].first == s1 &&
    //    substrings[1].second == s2))
    //{
        edit_distance_mod(substrings[1].first, substrings[1].second, result, offset_x + middle, offset_y + min_index);
    //}

    return distance[min_index] + re_distance[min_index];
}

long long edit_distance_mod(const std::string &s1, const std::string &s2,
                            std::vector<std::pair<size_t, size_t> > &result)
{
    result.assign(1, std::make_pair(0, 0));
    long long distance_result = edit_distance_mod(char(1) + s1, char(1) + s2, result, 0, 0);
    result.push_back(std::make_pair(s1.size(), s2.size()));
    return distance_result;
}

void show_way(const std::string &s1, const std::string &s2,
              const std::vector<std::pair<size_t, size_t> > &way)
{
    std::string result = "";
    for (size_t i = 1; i < way.size(); ++i)
    {
        if ((way[i].first == way[i - 1].first + 1) && (way[i].second == way[i - 1].second + 1))
        {
            if  ((way[i].second > 0) && (way[i].first > 0) && (s1[way[i].first - 1] == s2[way[i].second - 1]))
                result += 'M';
            else
            {
                result += 'R';
                std::cout << "IN " << way[i].first - 1 << " REPLACE " << s1[way[i].first - 1]
                          << " WITH " << s2[way[i].second - 1] << '\n';
            }
        }

        if ((way[i].first == way[i - 1].first) && (way[i].second == way[i - 1].second + 1))
        {
            result +='I';
            std::cout << "BEFORE " << way[i].first << " INSERT " << s2[way[i].second - 1] << '\n';
        }

        if ((way[i].first == way[i - 1].first + 1) && (way[i].second == way[i - 1].second))
        {
            result += 'D';
            std::cout << "IN " << way[i].first - 1 << " DELETE " << s1[way[i].first - 1] << '\n';
        }
    }
    std::cout << '\n';

    size_t pos = 0;
    for (size_t i = 0; i < result.size(); ++i)
    {
        switch (result[i])
        {
        case 'M':
            printf("%c", s1[pos]);
            pos++;
            break;
        case 'R':
            printf("%c", s1[pos]);
            pos++;
            break;
        case 'I':
            printf("-");
            break;
        case 'D':
            printf("%c", s1[pos]);
            pos++;
            break;
        }
    }

    std::cout << '\n' << result << '\n';

    pos = 0;
    for (size_t i = 0; i < result.size(); ++i)
    {
        switch (result[i])
        {
        case 'M':
            printf("%c", s2[pos]);
            pos++;
            break;
        case 'R':
            printf("%c", s2[pos]);
            pos++;
            break;
        case 'D':
            printf("-");
            break;
        case 'I':
            printf("%c", s2[pos]);
            pos++;
            break;
        }
    }
    std::cout << '\n';
}

int main()
{
    std::string s1 = "m", s2 = "psycho";
    std::cout << "\nORIGINAL ";

    std::vector<std::pair<size_t, size_t> > way;
    std::cout << edit_distance(s1, s2, way) << "\n\n";
    show_way(s1, s2, way);

    std::cout << "\nHIRSCHBERG ";

    //std::vector<std::pair<size_t, size_t> > way;
    std::cout << edit_distance_mod(s1, s2, way) << "\n\n";
    show_way(s1, s2, way);

    return 0;
}
