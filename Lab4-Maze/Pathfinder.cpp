#include "Pathfinder.h"

/// The total count of valid x coordinate spaces.
const int COLUMN_COUNT = 5;

/// The total count of valid y coordinate spaces.
const int ROW_COUNT = 5;

/// The total count of valid z coordinate spaces.
const int PLANE_COUNT = 5;



Pathfinder::Pathfinder() {
    // Initialize currentMaze to all 1's
    vector<vector<vector<int>>> newMaze;
    
    for (int z = 0; z < COLUMN_COUNT; z++) {
        for (int y = 0; y < ROW_COUNT; y++) {
            for (int x = 0; x < PLANE_COUNT; x++) {
                // newMaze.at(z).at(y).at(x) = 1;
                newMaze.push_back(vector<vector<int>>());
                newMaze.at(z).push_back(vector<int>());
                newMaze.at(z).at(y).push_back(1);
            }
        }
    }
    
    this->currentMaze = newMaze;
    
    // Initialize randomizer
    srand(static_cast<unsigned int>(time(NULL)));
}


Pathfinder::~Pathfinder() {
    this->currentMaze.clear();
}


/// A helper function for debugging. Run from lldb to get a printout of maze data.
void printMaze(const vector<vector<vector<int>>> &input) {
    for (int z = 0; z < PLANE_COUNT; z++) {
        for (int y = 0; y < ROW_COUNT; y++) {
            for (int x = 0; x < COLUMN_COUNT; x++) {
                cout << to_string(input.at(z).at(y).at(x));
                
                if (x < COLUMN_COUNT - 1) {
                    cout << " ";
                }
                
                if (x == COLUMN_COUNT - 1 && y < ROW_COUNT - 1) {
                    cout << endl;
                }
            }
            
            if (y == ROW_COUNT - 1) {
                cout << endl;
            }
        }
        
        if (z < PLANE_COUNT - 1) {
            cout << endl;
        }
    }
}

string Pathfinder::toString() const {
    string mazeString = "";
    
    for (int z = 0; z < PLANE_COUNT; z++) {
        for (int y = 0; y < ROW_COUNT; y++) {
            for (int x = 0; x < COLUMN_COUNT; x++) {
                mazeString += to_string(currentMaze.at(z).at(y).at(x));
                
                if (x < COLUMN_COUNT - 1) {
                    mazeString += " ";
                }
                
                if (x == COLUMN_COUNT - 1 && y < ROW_COUNT - 1) {
                    mazeString += "\n";
                }
            }
            
            if (y == ROW_COUNT - 1) {
                mazeString += "\n";
            }
        }
        
        if (z < PLANE_COUNT - 1) {
            mazeString += "\n";
        }
    }
    
    return mazeString;
}


void Pathfinder::createRandomMaze() {
    vector<vector<vector<int>>> newMaze;
    
    for (int z = 0; z < COLUMN_COUNT; z++) {
        for (int y = 0; y < ROW_COUNT; y++) {
            for (int x = 0; x < PLANE_COUNT; x++) {
                int randomVal = rand() % 2;
                
                // newMaze.at(z).at(y).at(x) = randomVal;
                newMaze.push_back(vector<vector<int>>());
                newMaze.at(z).push_back(vector<int>());
                newMaze.at(z).at(y).push_back(randomVal);
            }
        }
    }
    
    this->currentMaze = newMaze;
}


bool Pathfinder::importMaze(string file_name) {
    ifstream fileStream;
    fileStream.open(file_name);
    
    if (!fileStream.is_open()) {
        return false;
    }
    
    int nodeCount = 0;
    vector<vector<vector<int>>> fileMaze;
    
    bool pathNode = false;
    
    for (int z = 0; z < COLUMN_COUNT; z++) {
        for (int y = 0; y < ROW_COUNT; y++) {
            for (int x = 0; x < PLANE_COUNT; x++) {
                if (fileStream.bad() || fileStream.fail() || fileStream.eof()) {
                    break;
                }
                fileStream >> pathNode;
                
                // newMaze.at(z).at(y).at(x) = pathNode;
                fileMaze.push_back(vector<vector<int>>());
                fileMaze.at(z).push_back(vector<int>());
                fileMaze.at(z).at(y).push_back(pathNode);
                
                nodeCount += 1;
            }
        }
    }
    
    if (fileStream.bad() ||
        fileStream.fail() ||
        !fileStream.eof() ||
        nodeCount != COLUMN_COUNT * ROW_COUNT * PLANE_COUNT ||
        fileMaze.at(0).at(0).at(0) != 1 ||
        fileMaze.at(COLUMN_COUNT - 1).at(ROW_COUNT - 1).at(PLANE_COUNT - 1) != 1) {
        
        fileStream.close();
        return false;
    }
    
    this->currentMaze = fileMaze;
    
    fileStream.close();
    return true;
}


/// Returns a string representation of the given point in the form "(x, y, z)"
string stringFromPoint(const int x, const int y, const int z) {
    return "(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";
}


/// Returns `true` if the given coordinate lies inside our defined maze bounds.
bool pathIsInsideBounds(int x, int y, int z) {
    return (x >= 0 && x < COLUMN_COUNT &&
            y >= 0 && y < ROW_COUNT &&
            z >= 0 && z < PLANE_COUNT);
}

/// Returns `true` if the given coordinate lies inside our defined maze bounds, and is on
///   a valid traversable path (i.e. not 0 or 2)
bool pathIsValidInMaze(const vector<vector<vector<int>>> &maze, int x, int y, int z) {
    return (pathIsInsideBounds(x, y, z) &&
            maze.at(z).at(y).at(x) == 1);
}

vector<string> Pathfinder::solveMaze() {
    vector<vector<vector<int>>> mazeCopy = currentMaze;
    
    /// The x, y, and z coordinates we're considering.
    vector<vector<int>> breadcrumbs = {{ 0, 0, 0 }};
    
    vector<int> coords = breadcrumbs.at(breadcrumbs.size() - 1);
    int x = coords.at(0);
    int y = coords.at(1);
    int z = coords.at(2);
    
    if (!pathIsValidInMaze(mazeCopy, x, y, z)) {
        return vector<string>();
    }
    
    // While we've not hit the end yet...
    while (x != COLUMN_COUNT - 1 ||
           y != ROW_COUNT - 1 ||
           z != PLANE_COUNT - 1) {
        
        // Get coordinate from breadcrumbs
        coords = breadcrumbs.at(breadcrumbs.size() - 1);
        x = coords.at(0);
        y = coords.at(1);
        z = coords.at(2);
        
        // Mark our territory.
        mazeCopy.at(z).at(y).at(x) = 2;
        
        // Check adjacent blocks for a valid path. Leave breadcrumbs and go if we can.
        if (pathIsValidInMaze(mazeCopy, x, y - 1, z)) {
            // If top is valid
            breadcrumbs.push_back({ x, y - 1, z });
            
        } else if (pathIsValidInMaze(mazeCopy, x, y + 1, z)) {
            // If bottom is valid
            breadcrumbs.push_back({ x, y + 1, z });
            
        } else if (pathIsValidInMaze(mazeCopy, x - 1, y, z)) {
            // If left is valid
            breadcrumbs.push_back({ x - 1, y, z });
            
        } else if (pathIsValidInMaze(mazeCopy, x + 1, y, z)) {
            // If right is valid
            breadcrumbs.push_back({ x + 1, y, z });
            
        } else if (pathIsValidInMaze(mazeCopy, x, y, z - 1 )) {
            // If deeper is valid
            breadcrumbs.push_back({ x, y, z - 1 });
            
        } else if (pathIsValidInMaze(mazeCopy, x, y, z + 1)) {
            // If shallower is valid
            breadcrumbs.push_back({ x, y, z + 1 });
            
        } else if (breadcrumbs.size() > 1) {
            // If none are valid, go back.
            breadcrumbs.pop_back();
            
        } else {
            // Back to the beginning, and still no path? Quit here.
            return vector<string>();
        }
    }
    
    // If we're here, we must have made it to the end. Since that means we didn't drop a breadcrumb here yet, tack that on.
    breadcrumbs.push_back({ COLUMN_COUNT - 1, ROW_COUNT - 1, PLANE_COUNT - 1 });
    
    // Get the path strings from our valid path.
    vector<string> path;
    for (unsigned int i = 0; i < breadcrumbs.size(); i++) {
        vector<int> coords = breadcrumbs.at(i);
        path.push_back(stringFromPoint(coords.at(0), coords.at(1), coords.at(2)));
    }
    
    return path;
}
