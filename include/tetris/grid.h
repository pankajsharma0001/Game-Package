#pragma once
#include<vector>
#include<raylib.h>

class Grid{
public:
    Grid();
    int grid[20][10];
    void Draw();
    bool IsCellOutside(int row, int column);
    bool IsCellEmpty(int row, int column);
    int ClearFullRows();
    void Initialize();
    void Print();
private:
    bool IsRowFull(int row);
    void ClearRow(int row);
    void MoveRowDown(int row, int  numRows);
    int numRows;
    int numColumns;
    int cellSize;
    std::vector<Color> colors;
};