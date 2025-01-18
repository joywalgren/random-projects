#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;
int fillgrid(int grid[4][4], vector<pair<int, int>> &zeros);
void startingGrid(int grid[4][4], vector<pair<int, int>> &zeros, bool endGame);
void print(int grid[4][4]);
void move(int grid[4][4], vector<pair<int, int>> &zeros, int direction);
void placeNew(int grid[4][4], vector<pair<int, int>> &zeros, bool endGame);
int isZero(pair<int, int> checkNumber, vector<pair<int, int>> &zeros);

int main(int argc, const char *argv[])
{
    // bool endGame = false;
    int grid[4][4]; //[row][collum]
    int direction;
    vector<pair<int, int>> zeros;
    // startingGrid(grid,zeros, endGame);
    // print(grid);
    // while (endGame != true)
    // {
    //     cin >> direction;
    //     move(grid, zeros, direction);
    //     placeNew(grid,zeros,endGame);
    //     cout << endGame << " endgame condition\n";
    //     print(grid);
    // }

    direction = fillgrid(grid, zeros);
    move(grid,zeros, direction);
    print(grid);
    // int direction = fillgrid(grid, zeros);
    // move(grid, zeros, direction);

    return 0;
}

void startingGrid(int grid[4][4], vector<pair<int, int>> &zeros, bool endGame)
{
    fill(&grid[0][0], &grid[0][0] + 4 * 4, 0);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            zeros.push_back({i, j}); //good
        }
    }
    srand(time(0)); // seeding the random number generator with time
    placeNew(grid, zeros, endGame);
    placeNew(grid, zeros, endGame);
}
void placeNew(int grid[4][4], vector<pair<int, int>> &zeros, bool endGame)
{
    if (zeros.size() > 0)
    {
        int randomPlace = rand() % 16;
        int whereIsTheZero = isZero({randomPlace / 4, randomPlace % 4}, zeros);
        while (whereIsTheZero == -1)
        {
            randomPlace = rand() % 16;
            whereIsTheZero = isZero({randomPlace / 4, randomPlace % 4}, zeros);
        }
        int number = rand() % 10; // i can say if its 0 then num is 4 else number is 2. making it a 90% chance of getting a 2
        if (number == 0)
        {
            number = 4;
        }
        else
        {
            number = 2;
        }
        grid[randomPlace / 4][randomPlace % 4] = number;
        // cout << number << " was placed at (" << randomPlace / 4 << "," << randomPlace % 4 << ")\n";
        // cout << "size of zero: " << zeros.size() << endl;
        int i = 0;
        for (const pair<int, int> &p : zeros)
        {
            cout << "i = " << i << endl;
            i++;
            cout << "(" << p.first << ", " << p.second << ")" << endl;
        }
        cout << "where is zero: " << whereIsTheZero << endl;
        zeros.erase(zeros.begin() + whereIsTheZero); // this may cause an off by 1 issue if whereIsZero + begin give a different result then i want
        i = 0;
        for (const pair<int, int> &p : zeros)
        {
            cout << "i = " << i << endl;
            i++;
            cout << "(" << p.first << ", " << p.second << ")" << endl;
        }
    }
    else
    {
        cout << "trigger endGame\n";
        endGame = true;
    }
}
int isZero(pair<int, int> checkNumber, vector<pair<int, int>> &zeros)
{
    for (int i = 0; i < zeros.size(); i++)
    {
        if (zeros[i] == checkNumber)
        {
            return i; // if it is a zero/if its still in the list     hopefully this is 0-15
        }
    }
    return -1;
}

int fillgrid(int grid[4][4], vector<pair<int, int>> &zeros)
{
    int direction;
    for (int i = 0; i < 16; i++)
    { // read in 16 numbers

        cin >> grid[i / 4][i % 4];
        if (grid[i / 4][i % 4] == 0)
        {
            zeros.push_back({i / 4, i % 4}); //this one is for if running 2048 kattis
        }
    }
    cin >> direction;
    return direction;
}
void print(int grid[4][4])
{
    for (int i = 0; i < 16; i++)
    { // read out 16 numbers
        int number = grid[i / 4][i % 4]; 
        cout << number << " ";
        // cout << setw(4) << number << " ";
        if (((i + 1) / 4) > (i / 4))
            cout << endl;
    }
}

void move(int grid[4][4], vector<pair<int, int>> &zeros, int direction)
{
    bool isCombined[4][4];
    switch (direction)
    {
    case 0: // left
        fill(&isCombined[0][0], &isCombined[0][0] + 4 * 4, false);
        for (int i = 0; i < 4; i++) // iterate through the rows
        {
            for (int j = 0; j < 3; j++) // iterate through the colomns
            {
                if (grid[i][j] == 0 && j < 3 && grid[i][j + 1] != 0)
                {
                    grid[i][j] = grid[i][j + 1];
                    grid[i][j + 1] = 0;
                    int zeroIndex = isZero({i,j}, zeros);
                    if(zeroIndex >=0) 
                    {
                        zeros.erase(zeros.begin() + zeroIndex);
                    }
                    zeros.push_back({i, j + 1});
                    isCombined[i][j] = isCombined[i][j + 1];
                    isCombined[i][j + 1] = false;
                    j = -1;
                }
                else if (grid[i][j] == grid[i][j + 1] && grid[i][j] != 0 && isCombined[i][j] == false && isCombined[i][j + 1] == false)
                {
                    grid[i][j] = grid[i][j] + grid[i][j + 1];
                    grid[i][j + 1] = 0;
                    zeros.push_back({i, j + 1});
                    isCombined[i][j + 1] = false;
                    isCombined[i][j] = true;
                    j = -1; // recheck
                }
                else
                {
                    continue;
                }
            }
        }
        break;
    case 1: // up
        fill(&isCombined[0][0], &isCombined[0][0] + 4 * 4, false);
        for (int i = 0; i < 4; i++) // iterate through the rows
        {
            for (int j = 0; j < 3; j++) // iterate through the colomns
            {
                if (grid[j][i] == 0 && j < 3 && grid[j + 1][i] != 0)
                {
                    grid[j][i] = grid[j + 1][i];
                    grid[j + 1][i] = 0;
                    int zeroIndex = isZero({j,i}, zeros);
                    if(zeroIndex >=0) 
                    {
                        zeros.erase(zeros.begin() + zeroIndex);
                    }
                    zeros.push_back({j + 1, i});
                    isCombined[j][i] = isCombined[j + 1][i];
                    isCombined[j + 1][i] = false;
                    j = -1;
                }
                else if (grid[j][i] == grid[j + 1][i] && grid[j][i] != 0 && isCombined[j][i] == false && isCombined[j + 1][i] == false)
                {
                    grid[j][i] = grid[j][i] + grid[j + 1][i];
                    grid[j + 1][i] = 0;
                    zeros.push_back({j + 1, i});
                    isCombined[j][i] = true;
                    j = -1; // recheck
                }
                else
                {
                    continue;
                }
            }
        }
        break;
    case 2: // right
        fill(&isCombined[0][0], &isCombined[0][0] + 4 * 4, false);
        for (int i = 0; i < 4; i++) // iterate through the rows
        {
            for (int j = 3; j > 0; j--) // iterate through the colomns
            {
                if (grid[i][j] == 0 && j > 0 && grid[i][j - 1] != 0)
                {
                    grid[i][j] = grid[i][j - 1];
                    grid[i][j - 1] = 0;
                    int zeroIndex = isZero({i,j}, zeros);
                    if(zeroIndex >=0) 
                    {
                        zeros.erase(zeros.begin() + zeroIndex);
                    }
                    zeros.push_back({i, (j - 1)});
                    isCombined[i][j] = isCombined[i][j - 1];
                    isCombined[i][j - 1] = false;
                    j = 4;
                }
                else if (grid[i][j] == grid[i][j - 1] && grid[i][j] != 0 && isCombined[i][j] == false && isCombined[i][j - 1] == false)
                {
                    grid[i][j] = grid[i][j] + grid[i][j - 1];
                    grid[i][j - 1] = 0;
                    zeros.push_back({i, (j - 1)});
                    isCombined[i][(j - 1)] = false;
                    isCombined[i][j] = true;
                    j = 4; // recheck
                }
                else
                {
                    continue;
                }
            }
        }
        break;
    case 3: // down
        fill(&isCombined[0][0], &isCombined[0][0] + 4 * 4, false);
        for (int i = 0; i < 4; i++) // iterate through the rows
        {
            for (int j = 3; j > 0; j--) // iterate through the colomns
            {
                if (grid[j][i] == 0 && j > 0 && grid[j - 1][i] != 0)
                {
                    grid[j][i] = grid[j - 1][i];
                    grid[j - 1][i] = 0;
                    int zeroIndex = isZero({j,i}, zeros);
                    if(zeroIndex >=0) 
                    {
                        zeros.erase(zeros.begin() + zeroIndex);
                    }
                    zeros.push_back({j - 1, i});
                    isCombined[j][i] = isCombined[j - 1][i];
                    isCombined[j - 1][i] = false;
                    j = 4;
                }
                else if (grid[j][i] == grid[j - 1][i] && grid[j][i] != 0 && isCombined[j][i] == false && isCombined[j - 1][i] == false)
                {
                    grid[j][i] = grid[j][i] + grid[j - 1][i];
                    grid[j - 1][i] = 0;
                    zeros.push_back({j - 1, i});
                    isCombined[j - 1][i] = false;
                    isCombined[j][i] = true;
                    j = 4; // recheck
                }
                else
                {
                    continue;
                }
            }
        }
        break;
    default:
        cout << "invalid direction";
        break;
    }
}