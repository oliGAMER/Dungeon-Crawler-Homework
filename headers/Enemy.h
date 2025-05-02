#pragma once
#include <iostream>
#include <string>



using namespace std;    

class Enemy{
    private:
        string description;
        int health;
    public:
        void SetHealth(int health);
        void setDescription(string description);
        string getDescription();
        int getHealth();
    };