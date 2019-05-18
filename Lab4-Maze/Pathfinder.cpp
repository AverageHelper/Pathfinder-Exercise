#include "Pathfinder.h"

const int COLUMN_COUNT = 5;
const int ROW_COUNT = 5;
const int PLANE_COUNT = 5;

string stringFromMaze(const map<pair<int, int>, vector<int>> &maze) {
    if (maze.size() != ROW_COUNT * PLANE_COUNT) {
        return "Invalid maze";
    }
    
    string mazeString = "";
    
    for (int z = 0; z < PLANE_COUNT; z++) {
        for (int y = 0; y < ROW_COUNT; y++) {
            
            pair<int, int> key = make_pair(z, y);
            vector<int> row = maze.at(key);
            
            for (int x = 0; x < row.size(); x++) {
                
                mazeString += to_string(row.at(x));
                
                if (x < row.size() - 1) {
                    mazeString += " ";
                }
            }
            
            if (y <= ROW_COUNT - 1 && z <= PLANE_COUNT - 1) {
                mazeString += "\n";
            }
        }
        
        if (z < PLANE_COUNT - 1) {
            mazeString += "\n";
        }
    }
    
    return mazeString;
}

Pathfinder::Pathfinder() {
    // Initialize currentMaze to all 1's
    this->currentMaze = mazeWithRows({ 1, 1, 1, 1, 1 });
}

map<pair<int, int>, vector<int>> Pathfinder::mazeWithRows(vector<int> rowModel) {
    map<pair<int, int>, vector<int>> newMaze;
    pair<int, int> planeRow = make_pair(0, 0);
    
    for (int row = 0; row < ROW_COUNT; row++) {
        for (int plane = 0; plane < COLUMN_COUNT; plane++) {
            newMaze[planeRow] = rowModel;
            
            if (plane < COLUMN_COUNT - 1) {
                planeRow.second += 1;
            }
        }
        
        if (row < ROW_COUNT - 1) {
            planeRow.second = 0;
            planeRow.first += 1;
        }
    }
    
    return newMaze;
}

string Pathfinder::toString() const {
    return stringFromMaze(currentMaze);
}

void Pathfinder::createRandomMaze() {
    
}

bool Pathfinder::importMaze(string file_name) {
    ifstream fileStream;
    fileStream.open(file_name);
    
    if (!fileStream.is_open()) {
        return false;
    }
    
//    cout << "Importing maze from " << file_name << endl;
    
    int nodeCount = 0;
    map<pair<int, int>, vector<int>> fileMaze;
    
    bool pathNode = false;
    pair<int, int> planeRow = make_pair(0, 0);
    
    while (!fileStream.bad() && !fileStream.fail() && !fileStream.eof()) {
        fileStream >> pathNode;
        fileMaze[planeRow].push_back(pathNode);
        nodeCount += 1;
        
//        cout << "Set " << pathNode << " to x " << fileMaze[planeRow].size() - 1;
//        cout << ", y " << planeRow.second;
//        cout << ", z " << planeRow.first << endl;
        
        if (fileMaze[planeRow].size() >= COLUMN_COUNT &&
            planeRow.second < ROW_COUNT - 1) {
            planeRow.second += 1;
            
        } else if (fileMaze[planeRow].size() >= COLUMN_COUNT &&
                   planeRow.first < PLANE_COUNT - 1) {
            planeRow.second = 0;
            planeRow.first += 1;
            
        } else if (fileMaze[planeRow].size() >= COLUMN_COUNT &&
                   planeRow.second >= ROW_COUNT &&
                   planeRow.first >= PLANE_COUNT) {
            break;
        }
    }
    
    if (fileStream.bad() ||
        fileStream.fail() ||
        nodeCount != COLUMN_COUNT * ROW_COUNT * PLANE_COUNT ||
        fileMaze[make_pair(0, 0)].at(0) != 1 ||
        fileMaze[make_pair(PLANE_COUNT - 1, ROW_COUNT - 1)].at(COLUMN_COUNT - 1) != 1) {
        
        fileStream.close();
        return false;
    }
    
    this->currentMaze = fileMaze;
    
    fileStream.close();
    return true;
}

vector<string> Pathfinder::solveMaze() {
    return vector<string>();
}
