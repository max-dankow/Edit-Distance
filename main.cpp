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

std::vector<long long> distance(std::string s1, std::string s2)
{
    std::vector<long long> tek(s2.size() + 1, 0), last(s2.size() + 1, 0);
    std::vector<long long> *vec[2] = {&last, &tek};

    (*vec[0])[0] = 0;

    for (size_t i = 1; i < s2.size() + 1; ++i)
    {
        (*vec[0])[i] = (*vec[0])[i - 1] + 1;
    }

    //write_matrix(way, n, m);

    for (size_t j = 1; j < s1.size() + 1; ++j)
    {
        (*vec[1])[0] = (*vec[0])[0] + 1;

        for (size_t i = 1; i < s2.size() + 1; ++i)
        {
            (*vec[1])[i] = (*vec[1])[i - 1] + 1;

            if ((*vec[1])[i] > (*vec[0])[i] + 1)
            {
                (*vec[1])[i] = (*vec[0])[i] + 1;
                //distance[get_index(i, j, n)] = distance[get_index(i, j - 1, n)] + 1;
            }

            if ((*vec[1])[i] > (*vec[0])[i - 1] + ((s1[j - 1] == s2[i - 1])? 0 : 1))
            {
                (*vec[1])[i] = (*vec[0])[i - 1] + ((s1[j - 1] == s2[i - 1])? 0 : 1);
                //distance[get_index(i, j, n)] = distance[get_index(i - 1, j - 1, n)] + ((s1[i - 1] == s2[j - 1])? 0 : 1);
            }
        }

        std::swap(vec[0], vec[1]);
    }

    return *(vec[0]);
}

long long edit_distance_mod(const std::string &s1, const std::string &s2,
                            std::vector<std::pair<size_t, size_t> > &result,
                            int offset_x, int offset_y)
{
    if ((s1.size() <= 1) || (s2.size() <= 1))
    {
        std::vector<std::pair<size_t, size_t> > sub_way;
        long long simple_result = edit_distance(s1, s2, sub_way);

        for (auto it = sub_way.begin() + 1; it != sub_way.end(); ++it)
        {
            (*it).first += offset_x;
            (*it).second += offset_y;

            //std::cout << "subADDED :";
            //std::cout << (*it).first <<' '<< (*it).second << std::endl;
            result.push_back(*it);
        }

        return simple_result;
    }
    //assert(s1.size() != 0);

    std::ptrdiff_t middle = (s1.size()) / 2 + (s1.size()) % 2;

    std::string reverse_s1 = std::string(s1, middle, s1.size() - middle);
    std::reverse(reverse_s1.begin(), reverse_s1.end());
    std::string reverse_s2 = s2;
    std::reverse(reverse_s2.begin(), reverse_s2.end());

    std::vector<long long> left = distance(std::string(s1, 0, middle), s2);
    std::vector<long long> right = distance(reverse_s1, reverse_s2);
    std::reverse(right.begin(), right.end());

    size_t min_index = 0;

    for (size_t i = 1; i < left.size(); ++i)
    {
        if (left[i] + right[i] < left[min_index] + right[min_index])
            min_index = i;
    }

    std::pair<std::string, std::string> substrings[2];

    substrings[0].first = std::string(s1, 0, middle);
    substrings[0].second = std::string(s2, 0, min_index);
    substrings[1].first = std::string(s1, middle, s1.size() - middle);
    substrings[1].second = std::string(s2, min_index, s2.size() - min_index);

    /*if (substrings[0].first.size() <= 1  && substrings[0].second.size() <= 1 &&
        substrings[1].first.size() <= 1 && substrings[1].second.size() <= 1)
    {
        //std::cout << "ADDED :";
        //std::cout << middle + offset_x <<' '<< min_index + offset_y << std::endl;
        result.push_back(std::make_pair(middle + offset_x, min_index + offset_y));
        return 0;
    }*/

    //if (substrings[0].first.size() > 1  || substrings[0].second.size() > 1)
    //{
        edit_distance_mod(substrings[0].first, substrings[0].second, result, offset_x, offset_y);
    //}

    /*std::cout << "ADDED :";
    std::cout << middle + offset_x <<' '<< min_index + offset_y << std::endl;
    result.push_back(std::make_pair(middle + offset_x, min_index + offset_y));
*/
    //if (substrings[1].first.size() > 1  || substrings[1].second.size() > 1)
    //{
        edit_distance_mod(substrings[1].first, substrings[1].second, result, offset_x + middle, offset_y + min_index);
    //}

    return left[min_index] + right[min_index];
}

long long edit_distance_mod(const std::string &s1, const std::string &s2,
                            std::vector<std::pair<size_t, size_t> > &result)
{
    result.assign(1, std::make_pair(0, 0));
    long long distance_result = edit_distance_mod(/*char(1) + */s1,/* char(1) + */s2, result, 0, 0);
    //result.push_back(std::make_pair(s1.size(), s2.size()));
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
    std::string s1 = "l", s2 = "lil";
    std::vector<std::pair<size_t, size_t> > way;

    std::cout << "\nORIGINAL ";

    std::cout << edit_distance(s1, s2, way) << "\n\n";
    show_way(s1, s2, way);
    //distance(s1, s2);

    std::cout << "\nHIRSCHBERG ";

    //std::vector<std::pair<size_t, size_t> > way;
    std::cout << edit_distance_mod(s1, s2, way) << "\n\n";
    show_way(s1, s2, way);

    return 0;
}
