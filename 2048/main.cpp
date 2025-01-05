#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;
int fillgrid(int grid[4][4], vector<pair<int, int>>& zeros);
void print(int grid[4][4]);
void move (int grid[4][4], vector<pair<int, int>>& zeros, int direction);

int main(int argc, const char *argv[])
{
    int grid [4][4]; //[row][collum]
    vector<pair<int, int>> zeros;
    int direction = fillgrid(grid, zeros);
    print(grid);
    
    cout << direction;
    
    return 0;
}
//         cout << "(" << (i / 4) << "," << (i % 4) << ")" << endl;

void move (int grid[4][4], vector<pair<int, int>>& zeros, int direction)
{
    switch (direction)
    {
        case 0: //left
            for (int i = 0; i < 16; i++)
            {
                if (zeros == {i / 4,i % 4}) i++; // this is a mess, but here im essentially trying to check if this is a zero before going through the rest... but i realize i could just ask if this is a zero XD haha
                for (int j = 0; j < 4; j++)
                {
                    if (grid[i / 4][i % 4] == grid[i/4][j])
                } //should i store matching characters like i store zeros??
            }
            break;
        case 1: //up
            break;
        case 2: //righ    t
            break;
        case 3: //down
            break;
        default:
            cout << "invalid direction";
            break;
    }
}

int fillgrid(int grid[4][4], vector<pair<int, int>>& zeros)
{
    int direction;
    for (int i = 0; i < 16; i++)
    { // read in 16 numbers

        cin >> grid[i / 4][i % 4];
        if (grid[i / 4][i % 4] == 0)
        {
            zeros.push_back({i / 4,i % 4});
        }
    }
    cin >> direction;
    return direction;
}
void print(int grid[4][4])
{
    for (int i = 0; i < 16; i++)
    { // read out 16 numbers

        cout << grid[i / 4][i % 4] << " ";
        if (((i+1)/4)>(i/4)) cout << endl;
    }
}
