#include "deque.h"
#include <string>
#include <deque>
#include "gtest\gtest.h"

const unsigned int TestCount = 1000;
std::string GetRandString()
{
    return "" + (char)(rand() % CHAR_MAX) + (char)(rand() % CHAR_MAX);
}

void fillRandom(Deque<std::string>& d, std::deque<std::string>& sd, unsigned int count)
{
    for (int i = 0; i < count; ++i)
    {
        std::string f = GetRandString(), b = GetRandString();
        sd.push_back(b);
        d.push_back(b);
        sd.push_front(f);
        d.push_front(f);
    }

}



TEST(RandTests, PushPop)
{
    std::deque<std::string> sd = std::deque<std::string>();
    Deque<std::string> d = Deque<std::string>();
    fillRandom(d, sd, TestCount);
    while (!d.empty())
    {
        EXPECT_EQ(d.pop_back(), sd.back());
        sd.pop_back();
        EXPECT_EQ(d.size(), sd.size());
    }
}

TEST(RandTests, AccessOperator)
{
    std::deque<std::string> sd = std::deque<std::string>();
    Deque<std::string> d = Deque<std::string>();
    fillRandom(d, sd, TestCount);
    for (int i = 0; i < sd.size(); ++i)
    {
        EXPECT_EQ(d[i], sd[i]);
    }
}

TEST(RandTests, IteratorsRead)
{
    std::deque<std::string> sd = std::deque<std::string>();
    Deque<std::string> d = Deque<std::string>();
    fillRandom(d, sd, TestCount);
    std::deque<std::string>::const_iterator iterb = sd.cbegin();
    std::deque<std::string>::const_reverse_iterator ritere = sd.crbegin();
    Deque<std::string>::const_iterator iterb1 = d.cbegin();
    Deque<std::string>::const_reverse_iterator ritere1 = d.crbegin();
    while (iterb1 - d.cend() + 1)
    {
        EXPECT_EQ(*iterb, *iterb1);
        EXPECT_EQ(*ritere, *ritere1);
        ++iterb; ++iterb1; ++ritere; ++ritere1;
    }
    EXPECT_FALSE(iterb - sd.cend() + 1);
    EXPECT_FALSE(ritere - sd.crend() + 1);
}

TEST(RandTests, TimeTest)
{
    unsigned long long std_time_empty, std_time_full, Deque_time_empty, Deque_time_full;
    std::deque<std::string> sd = std::deque<std::string>();
    Deque<std::string> d = Deque<std::string>();
    std_time_empty = clock();
    for (int i = 0; i < TestCount; ++i)
    {
        sd.push_front(GetRandString());
    }
    std_time_empty = clock() - std_time_empty;

    Deque_time_empty = clock();
    for (int i = 0; i < TestCount; ++i)
    {
        d.push_front(GetRandString());
    }
    Deque_time_empty = clock() - Deque_time_empty;
    
    fillRandom(d, sd, TestCount);

    std_time_full = clock();
    for (int i = 0; i < TestCount; ++i)
    {
        sd.push_front(GetRandString());
    }
    std_time_full = clock() - std_time_full;

    Deque_time_full = clock();
    for (int i = 0; i < TestCount; ++i)
    {
        d.push_front(GetRandString());
    }
    Deque_time_full = clock() - Deque_time_full;

    EXPECT_TRUE((0.2 / Deque_time_empty * Deque_time_full) < (1.0 / std_time_empty * std_time_full));
}

int main(int argc, char**argv)
{
    
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}