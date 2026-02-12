#include <iostream>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>


using namespace std;



template <typename T>
class Node {
public:
    T data;
    Node* next;
    Node(T val) : data(val), next(nullptr) {}
};

template <typename T>
class CustomStack {
private:
    Node<T>* topNode;
    int stackSize;

public:
    CustomStack() : topNode(nullptr), stackSize(0) {}

    void push(T val) {
        Node<T>* newNode = new Node<T>(val);
        newNode->next = topNode;
        topNode = newNode;
        stackSize++;
    }

    void pop() {
        if (isEmpty()) return;
        Node<T>* temp = topNode;
        topNode = topNode->next;
        delete temp;
        stackSize--;
    }

    T top() {
        if (!isEmpty()) return topNode->data;
        throw runtime_error("Stack is empty");
    }

    bool isEmpty() {
        return topNode == nullptr;
    }

    int size() {
        return stackSize;
    }

    ~CustomStack() {
        while (!isEmpty()) {
            pop();
        }
    }
};

// Maze Representation 
struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class MazeSolver {
private:
    int rows, cols;
    vector<vector<char>> grid;
    Point start, end;

    // Directions: Up, Right, Down, Left
    // Directions: Up, Right, Down, Left
    int dx[4] = {-1, 0, 1, 0};
    int dy[4] = {0, 1, 0, -1};

public:
    MazeSolver(int r, int c) : rows(r), cols(c) {
        grid.resize(rows, vector<char>(cols));
    }

    void setMaze(const vector<string>& mazeData) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                grid[i][j] = mazeData[i][j];
                if (grid[i][j] == 'S') start = {i, j};
                if (grid[i][j] == 'E') end = {i, j};
            }
        }
    }

    void display(Point current) {
        
        system("cls"); 
        
        cout << "--- Maze Solver Visualization ---" << endl;
        cout << "S: Start | E: End | # : Wall | . : Path | * : Current Position" << endl;
        cout << "---------------------------------" << endl;

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (i == current.x && j == current.y) {
                    cout << "* ";
                } else if (grid[i][j] == '#') {
                    cout << (char)219 << (char)219; // Block character for wall
                } else if (grid[i][j] == '.') {
                    cout << "  "; // Path
                } else {
                    cout << grid[i][j] << " "; // S, E, or visited path
                }
            }
            cout << endl;
        }
        cout << "---------------------------------" << endl;
    }

    bool isValid(int r, int c) {
        return (r >= 0 && r < rows && c >= 0 && c < cols && (grid[r][c] == '.' || grid[r][c] == 'E'));
    }

    void solve() {
        CustomStack<Point> s;
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));

        s.push(start);
        visited[start.x][start.y] = true;

        bool found = false;

        while (!s.isEmpty()) {
            Point curr = s.top();
            
            // Visualization
            display(curr);
            this_thread::sleep_for(chrono::milliseconds(200));

            if (curr.x == end.x && curr.y == end.y) {
                found = true;
                break;
            }

            bool moved = false;
            for (int i = 0; i < 4; ++i) {
                int nx = curr.x + dx[i];
                int ny = curr.y + dy[i];

                if (isValid(nx, ny) && !visited[nx][ny]) {
                    visited[nx][ny] = true;
                    if (grid[nx][ny] != 'E') grid[nx][ny] = '+'; // Mark the path
                    s.push({nx, ny});
                    moved = true;
                    break; 
                }
            }

            if (!moved) {
                // Backtrack
                if (grid[curr.x][curr.y] == '+') grid[curr.x][curr.y] = ' '; // Clear from marked path if backtracking
                s.pop();
            }
        }

        if (found) {
            display(end);
            cout << "Goal Reached! Maze Solved." << endl;
        } else {
            cout << "No path found to the goal." << endl;
        }
    }
};

class MazeGenerator {
private:
    int rows, cols;
    vector<string> grid;

public:
    MazeGenerator(int r, int c) {
        
        rows = (r % 2 == 0) ? r + 1 : r;
        cols = (c % 2 == 0) ? c + 1 : c;
        grid.resize(rows, string(cols, '#'));
    }

    void generate(int r, int c) {
        grid[r][c] = '.';

        int dr[] = {-2, 0, 2, 0};
        int dc[] = {0, 2, 0, -2};

        // Shuffle directions
        vector<int> dirs = {0, 1, 2, 3};
        static random_device rd;
        static mt19937 g(rd());
        shuffle(dirs.begin(), dirs.end(), g);

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[dirs[i]];
            int nc = c + dc[dirs[i]];

            if (nr > 0 && nr < rows - 1 && nc > 0 && nc < cols - 1 && grid[nr][nc] == '#') {
                grid[r + dr[dirs[i]] / 2][c + dc[dirs[i]] / 2] = '.';
                generate(nr, nc);
            }
        }
    }

    vector<string> getMaze() {
        // Set Start and End
        grid[1][1] = 'S';
        
        // Find a suitable end point (bottom right area)
        for (int i = rows - 2; i > 0; i--) {
            for (int j = cols - 2; j > 0; j--) {
                if (grid[i][j] == '.') {
                    grid[i][j] = 'E';
                    return grid;
                }
            }
        }
        return grid;
    }

    int getActualRows() { return rows; }
    int getActualCols() { return cols; }
};


int main() {
    int choice;
    vector<string> mazeData;
    int rows, cols;

    cout << "--- Maze Menu ---" << endl;
    cout << "1. Use Predefined Maze" << endl;
    cout << "2. Generate Random Maze" << endl;
    cout << "Choice: ";

    if (!(cin >> choice)) {
        cout << "Invalid input. Exiting." << endl;
        return 1;
    }

    if (choice == 1) {
        mazeData = {
            "##########",
            "#S.......#",
            "#.########",
            "#.#.....##",
            "#.#.###..#",
            "#...#...##",
            "#####.#.##",
            "#.....#..#",
            "#######E##",
            "##########"
        };
        rows = mazeData.size();
        cols = mazeData[0].size();
    } else if (choice == 2) {
        cout << "Enter number of rows (e.g., 11): ";
        if (!(cin >> rows)) return 1;
        cout << "Enter number of columns (e.g., 21): ";
        if (!(cin >> cols)) return 1;

        if (rows < 5 || cols < 5) {
            cout << "Dimensions too small. Using minimum 11x11." << endl;
            rows = 11;
            cols = 11;
        }

        MazeGenerator mg(rows, cols);
        mg.generate(1, 1);
        mazeData = mg.getMaze();
        rows = mg.getActualRows();
        cols = mg.getActualCols();
    } else {
        cout << "Invalid choice." << endl;
        return 1;
    }

    MazeSolver ms(rows, cols);
    ms.setMaze(mazeData);
    
    cout << "Press Enter to start solving..." << endl;
    cin.ignore(); 
    cin.get();
    
    ms.solve();

    return 0;
}
