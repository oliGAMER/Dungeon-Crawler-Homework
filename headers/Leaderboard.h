#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>

struct PlayerRecord {
    std::string name;
    int score;
    int health;
    int movesUsed;
    bool completed;

    PlayerRecord() : name(""), score(0), health(0), movesUsed(0), completed(false) {}
    
    PlayerRecord(std::string n, int s, int h, int m, bool c) 
        : name(n), score(s), health(h), movesUsed(m), completed(c) {}
};


class Leaderboard {
protected:
    std::vector<PlayerRecord> records;
    std::string filename;
    
public:
    Leaderboard(std::string file) : filename(file) {}
    virtual ~Leaderboard() {}
    virtual void addRecord(const PlayerRecord& record) = 0;
    virtual void displayLeaderboard() = 0;
    virtual void sortLeaderboard() = 0;
    void saveToFile();
    void loadFromFile();
    const std::vector<PlayerRecord>& getRecords() const { 
        return records; 
    }
};


class ScoreLeaderboard : public Leaderboard {
public:
    ScoreLeaderboard(std::string file) : Leaderboard(file) {}
    
    void addRecord(const PlayerRecord& record) override;
    void displayLeaderboard() override;
    void sortLeaderboard() override;
};

class EfficiencyLeaderboard : public Leaderboard {
public:
    EfficiencyLeaderboard(std::string file) : Leaderboard(file) {}
    
    void addRecord(const PlayerRecord& record) override;
    void displayLeaderboard() override;
    void sortLeaderboard() override;
};