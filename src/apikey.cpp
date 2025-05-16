#include <iostream>
#include "apikey.hpp"
using namespace std;


string Api::getKey(){
    return key;
}

void Api::setKey(string new_key){
    key = new_key;
}