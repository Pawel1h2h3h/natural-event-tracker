#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "apikey.hpp"
using namespace std;
using json = nlohmann::json;


void Api::setKey(string new_key){key = new_key;}
void Api::setUrl(string new_url){current_url = new_url;}
string Api::getKey(){return key;}
string Api::getUrl(){return current_url;}


json Api::getData(){
    cpr::Response r = cpr::Get(
        cpr::Url{current_url}
    );
    if (r.status_code!=200){cout << "Cannot connect. Error: " << r.status_code;}

    json data = json::parse(r.text);
    return data;
}

