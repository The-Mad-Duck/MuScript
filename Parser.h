#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <iterator>
#include <algorithm>

static const std::regex doubleRegex{ R"(^[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)$)" };
static const std::regex intRegex{ R"(^[+-]?([0-9]+))" };
static const std::regex identRegex{ R"([a-z])" };

struct chunkVal
{
    bool type;
    int op;
    int a1;
    int a2;
    float a2f;
    int rt;
    chunkVal(bool t, int o, int aone, int atwo, float af, int r)
    {
        type = t;
        op = o;
        a1 = aone;
        a2 = atwo;
        a2f = af;
        rt = r;
    }
    int getOp() { return op; }
    float getFloat() { return a2f; }
};

class Parser
{
public:

    //Big data man
    std::vector<chunkVal> values;

    float alphaVals[27] = { 0 };

    void operator = (const Parser& P);
    std::string ops[13] = { "agn", "add", "sub", "mlt", "div", "mod", "abs", "sin", "cos", "tan", "pow", "jil", "jig"};

    void resetAlpha()
    {
        for(int i = 0; i < 27; i++)
            alphaVals[i] = 0;
    }

    void resetTable()
    {
        values.clear();
    }

    void updateLogic(std::string s);

    Parser();

    ~Parser();
};