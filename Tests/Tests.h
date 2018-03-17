#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "Url.h"
#include "Utils.h"

const std::string URL_STR = "http://host/path/subpath?name1=value1&name2=\"value2\"&\"name3\"=#fragment";

TEST(UtilsTest, EncodingUtilsTest)
{
    {
        std::string result = EncodingUtils::URLEncode(L"str▬♣♂ing");
        EXPECT_TRUE(result == "str%D0%B2%E2%80%93%C2%AC%D0%B2%E2%84%A2%D0%88%D0%B2%E2%84%A2%E2%80%9Aing");
    }
    {
        std::string result = EncodingUtils::URLEncode("string");
        EXPECT_TRUE(result == "string");
    }
}

TEST(UrlTest, ParsingUrlStringTest)
{
    Url url_str(URL_STR);
    EXPECT_TRUE(url_str.Scheme == L"http");
    EXPECT_TRUE(url_str.Authority == L"host");
    EXPECT_TRUE(url_str.Path == L"/path/subpath");
    EXPECT_TRUE(url_str.Query[0].first == L"name1");
    EXPECT_TRUE(url_str.Fragment == L"fragment");
}

TEST(UrlTest, ParsingQueryTest)
{
    Url url_str(URL_STR);
    EXPECT_TRUE(url_str.Query.size() == 3);

    EXPECT_TRUE(url_str.Query[0].first == L"name1");
    EXPECT_TRUE(url_str.Query[0].second == L"value1");

    EXPECT_TRUE(url_str.Query[1].first == L"name2");
    EXPECT_TRUE(url_str.Query[1].second == L"\"value2\"");

    EXPECT_TRUE(url_str.Query[2].first == L"\"name3\"");
    EXPECT_TRUE(url_str.Query[2].second == L"");
}

TEST(UrlTest, CompileUrlTest)
{
    {
        auto args = Url::QueryArgs() ;
        args.emplace_back(L"name1", L"value1");
        args.emplace_back(L"name2", L"value2");
        args.emplace_back(L"", L"");
        args.emplace_back(L"name3", L"value3");

        Url url_str(L"http", L"host", L"/path/subpath", L"fragment", args);
        EXPECT_TRUE(url_str.Compile() == "http://host/path/subpath?name1=value1&name2=value2&=&name3=value3#fragment");
    }
    {
        auto args = Url::QueryArgs() ;
        args.emplace_back(L"n\"aоme1", L"vaКlue1");

        Url url_str(L"http", L"host", L"/p at h/sub p'ath", L"fragment", args);
        EXPECT_TRUE(url_str.Compile() == "http://host/p%20at%20h/sub%20p'ath?n%22a%D0%A0%D1%95me1=va%D0%A0%D1%99lue1#fragment");
    }
}
