#include <iostream>
#include <cpr/cpr.h>
#pragma once
using namespace std;

class Api
{
    private:
        string key;
        string url;

    public:
        string getKey();

        void setKey(string key);
};