#include <iostream>
#include "apikey.hpp"
#include "event.hpp"
#include "fileHandler.hpp"
using namespace std;



int main() {
    Api api;
    FileHandler fh;
    string key = "H7mCThmUpd7iAwuzs5cMCngPOaDdvLdP4pIEwd4g";

    // Create event categories
    fh.readFromJson("categories.json");
    fh.createCategories();


    return 0;
}