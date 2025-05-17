#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "apikey.hpp"
using namespace std;


string Api::getKey(){
    return key;
}

void Api::setKey(string new_key){
    key = new_key;
}