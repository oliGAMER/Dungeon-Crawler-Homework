#include "../headers/Leaderboard.h"
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>

void Leaderboard::saveToFile() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    
    file << "Name,Score,Health,MovesUsed,Completed\n";
    
    for (const auto& record : records) {
        file << record.name << "," 
             << record.score << "," 
             << record.health << "," 
             << record.movesUsed << "," 
             << (record.completed ? "true" : "false") << "\n";
    }
    file.close();
}

void Leaderboard::loadFromFile() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "No existing leaderboard file found. Creating new leaderboard." << std::endl;
        return;
    }
    
    records.clear();
    std::string line, token;
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name;
        int score, health, moves;
        bool completed;
        
        if (!std::getline(ss, name, ',')) continue;
        
        std::string scoreStr, healthStr, movesStr, completedStr;
        if (!std::getline(ss, scoreStr, ',')) continue;
        if (!std::getline(ss, healthStr, ',')) continue;
        if (!std::getline(ss, movesStr, ',')) continue;
        if (!std::getline(ss, completedStr, '\n')) continue;
        
        score = std::stoi(scoreStr);
        health = std::stoi(healthStr);
        moves = std::stoi(movesStr);
        completed = (completedStr == "true");
        
        records.emplace_back(name, score, health, moves, completed);
    }
    file.close();
    
    sortLeaderboard();
}


void ScoreLeaderboard::addRecord(const PlayerRecord& record) {
    records.push_back(record);
    sortLeaderboard();
}

void ScoreLeaderboard::displayLeaderboard() {
    std::cout << "===== SCORE LEADERBOARD =====" << std::endl;
    std::cout << "Rank | Name             | Score | Health | Moves | Completed" << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;
    
    int rank = 1;
    for (const auto& record : records) {
        std::cout << std::setw(4) << rank << " | "
                  << std::setw(16) << record.name << " | "
                  << std::setw(5) << record.score << " | "
                  << std::setw(6) << record.health << " | "
                  << std::setw(5) << record.movesUsed << " | "
                  << (record.completed ? "Yes" : "No") << std::endl;
        rank++;
        
        if (rank > 10) break;
    }
    std::cout << "=============================================" << std::endl;
}

int scorePartition(std::vector<PlayerRecord>& arr, int low, int high) {
    PlayerRecord pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        bool shouldSwap = false;
        if (arr[j].score > pivot.score) {
            shouldSwap = true;
        } else if (arr[j].score == pivot.score) {
            if (arr[j].completed && !pivot.completed) {
                shouldSwap = true;
            } else if (arr[j].completed == pivot.completed) {
                if (arr[j].movesUsed < pivot.movesUsed) {
                    shouldSwap = true;
                }
            }
        }
        
        if (shouldSwap) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void scoreQuickSort(std::vector<PlayerRecord>& arr, int low, int high) {
    if (low < high) {
        int pivotIndex = scorePartition(arr, low, high);
        scoreQuickSort(arr, low, pivotIndex - 1);
        scoreQuickSort(arr, pivotIndex + 1, high);
    }
}


void ScoreLeaderboard::sortLeaderboard() {
    if (records.size() <= 1) return; 
    scoreQuickSort(records, 0, records.size() - 1);
}


void EfficiencyLeaderboard::addRecord(const PlayerRecord& record) {
    records.push_back(record);
    sortLeaderboard();
}

void EfficiencyLeaderboard::displayLeaderboard() {
    std::cout << "===== EFFICIENCY LEADERBOARD =====" << std::endl;
    std::cout << "Rank | Name             | Moves | Score | Health | Completed" << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;
    
    int rank = 1;
    for (const auto& record : records) {
        if (!record.completed) continue;
        
        std::cout << std::setw(4) << rank << " | "
                  << std::setw(16) << record.name << " | "
                  << std::setw(5) << record.movesUsed << " | "
                  << std::setw(5) << record.score << " | "
                  << std::setw(6) << record.health << " | Yes" << std::endl;
        rank++;
        
        if (rank > 10) break;
    }
    std::cout << "=============================================" << std::endl;
}

void efficiencyMerge(std::vector<PlayerRecord>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    std::vector<PlayerRecord> L(n1), R(n2);
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int i = 0; i < n2; i++)
        R[i] = arr[mid + 1 + i];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        bool useLeft = false;
        if (L[i].completed && !R[j].completed) {
            useLeft = true;
        } else if (L[i].completed == R[j].completed) {
            if (L[i].completed) { 
                if (L[i].movesUsed < R[j].movesUsed) {
                    useLeft = true;
                } else if (L[i].movesUsed == R[j].movesUsed) {
                    if (L[i].health > R[j].health) {
                        useLeft = true;
                    }
                }
            }
        }
        
        if (useLeft) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void efficiencyMergeSort(std::vector<PlayerRecord>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        efficiencyMergeSort(arr, left, mid);
        efficiencyMergeSort(arr, mid + 1, right);
        efficiencyMerge(arr, left, mid, right);
    }
}


void EfficiencyLeaderboard::sortLeaderboard() {
    if (records.size() <= 1) return;
    efficiencyMergeSort(records, 0, records.size() - 1);
}