#include <iostream>
#include "apikey.hpp"
using namespace std;



string url = "https://eonet.gsfc.nasa.gov/api/v2.1/events";

int main() {
    Api api;
    string key = "H7mCThmUpd7iAwuzs5cMCngPOaDdvLdP4pIEwd4g";
    api.setKey(key);
    cout << api.getKey();
    return 0;
}