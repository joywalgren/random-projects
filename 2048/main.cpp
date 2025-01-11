#include <iostream>
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
int isZero (pair<int,int> checkNumber, vector<pair<int, int>> &zeros);

int main(int argc, const char *argv[])
{
    bool endGame = false;
    int grid[4][4]; //[row][collum]
    int direction;
    vector<pair<int, int>> zeros;
    startingGrid(grid,zeros, endGame);
    print(grid); 
    while (endGame == false)
    {
        cin >> direction;
        move(grid, zeros, direction);
        placeNew(grid,zeros,endGame);
        print(grid);
    }
    
    // int direction = fillgrid(grid, zeros);
    // move(grid, zeros, direction);


    // cout << direction;

    return 0;
}

void startingGrid(int grid[4][4], vector<pair<int, int>> &zeros, bool endGame)
{
    fill(&grid[0][0], &grid[0][0] + 4 * 4, 0);
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            zeros.push_back({i,j});
        }
    }
    srand(time(0)); //seeding the random number generator with time
    placeNew(grid, zeros, endGame);
    placeNew(grid, zeros, endGame);
}
void placeNew(int grid[4][4], vector<pair<int, int>> &zeros, bool endGame)
{
    if (zeros.size()>0)
    { 
        int randomPlace = rand() % 16;
        int whereIsTheZero = isZero({randomPlace/4, randomPlace%4}, zeros);
        while (whereIsTheZero == -1)
        {
            randomPlace = rand() % 16;
            whereIsTheZero = isZero({randomPlace/4, randomPlace%4}, zeros);
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
        grid[randomPlace/4][randomPlace%4] = number;
        // cout << number << " was placed at (" << randomPlace/4 << "," << randomPlace%4 << ")\n";
        // cout << "size of zero: " << zeros.size() << endl;
        // int i = 0;
        // for (const pair<int, int> &p : zeros) {
        //     cout << "i = " << i << endl;
        //     i++;
        //     cout << "(" << p.first << ", " << p.second << ")" << endl;
        //     }
        // cout<< "where is zero: " << whereIsTheZero<< endl;
        zeros.erase(zeros.begin()+ whereIsTheZero); //this may cause an off by 1 issue if whereIsZero + begin give a different result then i want
    }
    else
    {
        cout<< "trigger endGame\n";
        endGame = true;
    }
}
int isZero (pair<int,int> checkNumber, vector<pair<int, int>> &zeros)
{
    for(int i = 0; i<zeros.size();i++)
    {
        if (zeros[i] == checkNumber)
        {
            return i; //if it is a zero/if its still in the list     hopefully this is 0-15
        }
    }
    return -1;

}


void move(int grid[4][4], vector<pair<int, int>> &zeros, int direction)
{
    bool isCombined[4][4];
    switch (direction)
    {
    case 0: // left
        for (int i = 0; i < 16; i++)
        {
            if (grid[i / 4][i % 4] == 0)
                continue;                           // if zero go on
            for (int j = (1 + (i % 4)); j < 4; j++) // comparing every value to what is (right) of it in its row
            {
                if (grid[i / 4][j] == 0)
                    continue;                                  // if zero go on
                else if (grid[i / 4][i % 4] == grid[i / 4][j]) // if two numbers match
                {
                    grid[i / 4][i % 4] = grid[i / 4][i % 4] + grid[i / 4][j]; // combines on the number farthest left
                    grid[i / 4][j] = 0;
                    zeros.push_back({i/4,j});
                    j = 4; // dont check anything else for matches
                }
                else
                {
                    j = 4; // if the number right beside it is not a zero or a match it goes onto the next number
                }
            }
        }
        for (int i = 0; i < 4; i++) // after it finishes a row
        {
            for (int j = 3; j > 0; j--) // going backwards through the row to finalize all the movement
            {
                if (grid[i][j] == 0)
                    continue;                 // if the one i am moving is a zero
                else if (grid[i][j - 1] == 0) // if a zero is to the left we move
                {
                    grid[i][j - 1] = grid[i][j];
                    grid[i][j] = 0;
                    zeros.erase(zeros.begin()-(isZero({i,j-1}, zeros)));
                    zeros.push_back({i,j});
                    if (j < 3) // if there is stuff to the right of the zero
                    {
                        int counter = j;
                        while (counter < 3) // shifting everything to the left
                        {
                            if (grid[i][counter + 1] == 0)
                            {
                                if (grid[i][counter + 2] != 0 && counter < 2) // this is for situations like 2 0 0 4
                                {
                                    grid[i][counter] = grid[i][counter + 2];
                                    grid[i][counter + 2] = 0;
                                    zeros.erase(zeros.begin()-(isZero({i,counter}, zeros)));
                                    zeros.push_back({i,counter+2});
                                }
                                counter++;
                            }
                            else
                            {
                                grid[i][counter] = grid[i][counter + 1];
                                grid[i][counter + 1] = 0;
                                zeros.erase(zeros.begin()-(isZero({i,counter}, zeros)));
                                zeros.push_back({i,counter+1});
                                counter++;
                            }
                        }
                    }
                }
            }
        }
        break;
    case 1: // up
    fill(&isCombined[0][0], &isCombined[0][0] + 4 * 4, false);
        for (int i = 0; i < 4; i++) //iterate through the rows
        {
            for (int j = 0; j < 4; j++) //iterate through the colomns 
            {
                if (grid[j][i] == 0 && j<3 && grid[j+1][i] != 0)
                {
                    grid[j][i] = grid[j+1][i];
                    grid[j+1][i] = 0;
                    zeros.erase(zeros.begin()-(isZero({j,i}, zeros)));
                    zeros.push_back({j+1,i});
                    isCombined[j][i] = isCombined[j+1][i];
                    isCombined[j+1][i] = false;
                    j = -1;
                }
                else if (grid[j][i] == grid[j+1][i] && grid[j][i] != 0 && isCombined[j][i] == false && isCombined[j+1][i] == false)
                {
                    grid[j][i] = grid[j][i]+grid[j+1][i];
                    grid[j+1][i] = 0;
                    zeros.push_back({j+1, i});
                    isCombined[j][i] = true;
                    j=-1; //recheck
                }
                else
                {
                    continue;
                }
            }
        }
        break;
    case 2: // right
        for (int i = 15; i >= 0; i--)
        {
            // cout << "i: " << i << endl;
            if (grid[i / 4][i % 4] == 0)
                continue;                           // if zero go on
            for (int j = ((i % 4) - 1); j >= 0; j--) // comparing every value to what is (left) of it in its row
            {
                if (grid[i / 4][j] == 0)
                    continue;                                  // if zero go on
                else if (grid[i / 4][i % 4] == grid[i / 4][j]) // if two numbers match
                {
                    grid[i / 4][i % 4] = grid[i / 4][i % 4] + grid[i / 4][j]; // combines on the number farthest right
                    grid[i / 4][j] = 0;
                    zeros.push_back({i/4,j});
                    j = 0; // dont check anything else for matches
                }
                else
                {
                    j = 0; // if the number right beside it is not a zero or a match it goes onto the next number
                }
            }
        }
        for (int i = 0; i < 4; i++) // after it finishes a row
        {
            for (int j = 0; j < 3; j++) // going backwards through the row to finalize all the movement
            {
                if (grid[i][j] == 0)
                    continue;                 // if the one i am moving is a zero
                else if (grid[i][j + 1] == 0) // if a zero is to the right we move
                {
                    grid[i][j + 1] = grid[i][j];
                    grid[i][j] = 0;
                    zeros.erase(zeros.begin()-(isZero({i,j+1}, zeros)));
                    zeros.push_back({i,j});
                    if (j > 0) // if there is stuff to the left of the zero
                    {
                        int counter = j;
                        while (counter > 0) // shifting everything to the right
                        {
                            if (grid[i][counter - 1] == 0)
                            {
                                if (grid[i][counter - 2] != 0 && counter >= 2) // this is for situations like 2 0 0 4
                                {
                                    grid[i][counter] = grid[i][counter - 2];
                                    grid[i][counter - 2] = 0;
                                    zeros.erase(zeros.begin()-(isZero({i,counter}, zeros)));
                                    zeros.push_back({i,counter - 2});
                                }
                                counter--;
                            }
                            else
                            {
                                grid[i][counter] = grid[i][counter - 1];
                                grid[i][counter - 1] = 0;
                                zeros.erase(zeros.begin()-(isZero({i,counter}, zeros)));
                                zeros.push_back({i,counter - 1});
                                counter--;
                            }
                        }
                    }
                }
            }
        }
        break;
    case 3: // down
    fill(&isCombined[0][0], &isCombined[0][0] + 4 * 4, false);
        for (int i = 0; i < 4; i++) //iterate through the rows
        {
            // cout << "i = " << i << endl;
            for (int j = 3; j >= 0; j--) //iterate through the colomns 
            {
                // cout << "j = " << j << endl;
                if (grid[j][i] == 0 && j>0 && grid[j-1][i] != 0)
                {
                    grid[j][i] = grid[j-1][i];
                    grid[j-1][i] = 0;
                    zeros.erase(zeros.begin()-(isZero({j,i}, zeros)));
                    zeros.push_back({j-1,i});
                    isCombined[j][i] = isCombined[j-1][i];
                    isCombined[j-1][i] = false;
                    j = 4;
                }
                else if (grid[j][i] == grid[j-1][i] && grid[j][i] != 0 && isCombined[j][i] == false && isCombined[j-1][i] == false)
                {
                    grid[j][i] = grid[j][i]+grid[j-1][i];
                    grid[j-1][i] = 0;
                    zeros.push_back({j-1,i});
                    isCombined[j-1][i] = false;
                    isCombined[j][i] = true;
                    j=4; //recheck
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

int fillgrid(int grid[4][4], vector<pair<int, int>> &zeros)
{
    int direction;
    for (int i = 0; i < 16; i++)
    { // read in 16 numbers

        cin >> grid[i / 4][i % 4];
        if (grid[i / 4][i % 4] == 0)
        {
            zeros.push_back({i / 4, i % 4});
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
        if (((i + 1) / 4) > (i / 4))
            cout << endl;
    }
}
