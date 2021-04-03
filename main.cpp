//
//  main.cpp
//  project_1
//
//  Created by major ma on 2021/3/12.
//  Loca: Chongqing U, Shapingba, CHONGQING

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using std::vector;
using std::cout;
using std::string;
using std::istringstream;
using std::endl;
using std::sort;



enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};
//  directional deltas
const int delta[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};



vector<State> ParseLine(string board_line){
    istringstream read_line (board_line);
    char c;
    int n;
    vector<State> row;
    while (read_line >> n >> c && c == ',')
        if (n == 0) {
            row.push_back(State::kEmpty);
        }
        else {
            row.push_back(State::kObstacle);
        }
    return row;
}



vector<vector<State>> ReadBoardFile(string file_path){
    std::ifstream myfile(file_path);
    vector<vector<State>> board;
    if (myfile) {
        cout << "File opened." << std::endl;
        string line;
        while (getline(myfile, line)) {
            vector<State> row = ParseLine(line);
            board.push_back(row);
        }
    }
    else
        cout << "File open failed!";
    return board;
}



bool Compare(vector<int> node_2b_g, vector<int> node_2b_s){
    return ((node_2b_g[2] + node_2b_g[3]) > (node_2b_s[2] + node_2b_s[3]));
}



void CellSort(vector<vector<int>> *v){
    sort(v -> begin(), v -> end(), Compare);
}



int Heuristic(int x1, int y1, int x2, int y2){
    return (abs(x2 - x1) + abs(y2 - y1));
}



bool CheckValidCell(int x, int y, vector<vector<State>> &grid){
    bool on_grid_x = (x >= 0 && x < grid.size());
    bool on_grid_y = (y >= 0 && y < grid[0].size());
    if (on_grid_x && on_grid_y) {
        return grid[x][y] == State::kEmpty;
    }
    return false;
}



void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &open_list, vector<vector<State>> &grid){
    vector<int> node = {x, y, g, h};
    open_list.push_back(node);
    grid[x][y] = State::kClosed;
}



void ExpandNeighbors(vector<int> current_node, int goal[2], vector<vector<int>> &open_list, vector<vector<State>> grid){
    int x = current_node[0];
    int y = current_node[1];
    int g = current_node[2];
    for (int i = 0; i < 4; ++i) {
        int x_ref = x + delta[i][0];
        int y_ref = y + delta[i][1];
        if (CheckValidCell(x_ref, y_ref, grid)) {
            int g_ref = g + 1;
            int h_ref = Heuristic(x_ref, y_ref, goal[0], goal[1]);
            AddToOpen(x_ref, y_ref, g_ref, h_ref, open_list, grid);
        }
    }
}



vector<vector<State>> Search(vector<vector<State>> grid, int ini_point[2], int end_point[2]){
    vector<vector<int>> open {};
    int x = ini_point[0];
    int y = ini_point[1];
    int g = 0;
    int h = Heuristic(x, y, end_point[0], end_point[1]);
    AddToOpen(x, y, g, h, open, grid);
    
    while (open.size() != 0) {
        CellSort(&open);      // Amazing & ! 思考题
        auto current = open.back();
        open.pop_back();
        x = current[0];
        y = current[1];
        grid[x][y] = State::kPath;
        if (x == end_point[0] && y == end_point[1]) {
            grid[ini_point[0]][ini_point[1]] = State::kStart;
            grid[end_point[0]][end_point[1]] = State::kFinish;
            return grid;
        }
        //  Expand neighbors.
        ExpandNeighbors(current, end_point, open, grid);
    }
    cout << "NO PATH FOUND!" << endl;
    return vector<vector<State>> {};
}



string CellString(State cell){
    switch (cell) {
        case State::kObstacle : return "⛰️   ";
        case State::kPath :     return "🚗   ";
        case State::kStart:     return "🇨🇳   ";
        case State::kFinish:    return "🏁   ";
        default :               return "0    ";
    }
}



void PrintBoard(vector<vector<State>> a){
    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < a[i].size(); ++j)
            cout << CellString(a[i][j]);
        cout << std::endl;
    }
}



int main(){
    int init[2] = {0,0};
    int goal[2] = {4,5};
    auto board = ReadBoardFile("/Users/majorma/Documents/Xcode_C_codes/project_1/project_1/1.board");
    vector<vector<State>> solution= Search(board, init, goal);
    PrintBoard(solution);
    return 0;
}
