#include <iostream>
#include "apikey.hpp"
#include "event.hpp"
#include "fileHandler.hpp"
using namespace std;



string url = "https://eonet.gsfc.nasa.gov/api/v2.1/events";

int main() {
    Api api;
    string key = "H7mCThmUpd7iAwuzs5cMCngPOaDdvLdP4pIEwd4g";
    api.setKey(key);
    cout << api.getKey() << "\n";
    FileHandler fh;

    Event event1(1, "lol", 1.2, 1.3);
    Event event2(2, "xd", 2.4, 7.6);


    vector<Event> data;

    data.push_back(event1);
    data.push_back(event2);

    fh.writeToJson("nigga.txt", data);
    return 0;
}