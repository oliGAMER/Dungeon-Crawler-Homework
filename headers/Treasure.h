#pragma once

#include <iostream>
#include <string>



using namespace std;

class Treasure{
    private:
        string type;
        int quantity;
    public:
        void SetType(string type);
        void SetQuantity(int quantity);
        string GetType();
        int GetQuantity();
    };