#include <iostream>
#include <fstream>
#include <sstream>

enum {EMPTY = -1, DIM = 9};

const int inner_square_size = 3;

struct Puzzle
{
    int data[DIM][DIM];
} puzzle;

bool check(const Puzzle &puzzle)
{
    for(int i{0}; i < DIM; ++i)
    {
        for(int j{0}; j < DIM; ++j)
        {
            //x and y are coordinates to look all inner squares up
            //dx and dy help us to shift coordinates x and y to the next inner square
            int dx = (i % inner_square_size) * inner_square_size;
            int x = j % inner_square_size + dx;
            int dy = (i / inner_square_size) * inner_square_size;
            int y = j / inner_square_size + dy;
            /*int x = j % 3 + (i % 3) * 3;
            int y = j / 3 + (i / 3) * 3;*/
            for(int k{j + 1}; k < DIM; ++k)
            {
                //Check for duplicates at all the horizontal lines
                if(puzzle.data[i][j] != EMPTY && puzzle.data[i][j] == puzzle.data[i][k])
                    return false;
                //Check for duplicates at all the vertical lines
                if(puzzle.data[j][i] != EMPTY && puzzle.data[j][i] == puzzle.data[k][i])
                    return false;
                int x_k = k % inner_square_size + dx;
                int y_k = k / inner_square_size + dy;
                if(puzzle.data[x][y] != EMPTY &&
                   puzzle.data[x][y] == puzzle.data[x_k][y_k])
                    return false;
            }
        }
    }
    return true;
}

bool find_empty_cell(int &x, int &y)
{
    int min = DIM;
    bool hasEmptyCells = false;
    for(int i{0}; i < DIM; ++i)
        for(int j{0}; j < DIM; ++j)
        {
            if(puzzle.data[i][j] == EMPTY)
            {
                hasEmptyCells = true;
                int counter = 0;
                for(int k = 1; k <= DIM; ++k)
                {
                    puzzle.data[i][j] = k;
                    if(check(puzzle))
                        ++counter;
                    puzzle.data[i][j] = EMPTY;
                }
                if(min > counter)
                {
                    min = counter;
                    x = j;
                    y = i;
                }

            }
        }
    return hasEmptyCells;
}

bool solve(Puzzle &puzzle)
{
    int x = -1;
    int y = -1;
    //If there is no empty cells
    //that means we have solved the puzzle
    if(!find_empty_cell(x,y))
        return true;
    for(int k = 1; k <= DIM; ++k)
    {
        puzzle.data[y][x] = k;
        if(check(puzzle))
            if(solve(puzzle))
                return true;
    }
    puzzle.data[y][x] = EMPTY;
    return false;
}

void print_out_the_solution(const Puzzle &puzzle)
{
    for(int i{0}; i < 3 * DIM; ++i)
        std::cout << "-";
    std::cout << std::endl;
    for(int i{0}; i < DIM; ++i)
    {
        if(i != 0 && i % inner_square_size == 0)
        {
            for(int k = 0; k < DIM * 3; ++k)
                std::cout << "-";
            std::cout << std::endl;
        }
        std::cout << "| ";
        for(int j{0}; j < DIM; ++j)
        {
            if(j != 0 && j % inner_square_size == 0)
                std::cout << "| ";
             std::cout << puzzle.data[i][j] << " ";
        }
        std::cout << "|" << std::endl;
    }
    for(int i{0}; i < DIM * 3; ++i)
        std::cout << "-";
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    std::string path_to_input_file = (argc > 1) ? argv[1] : "sudoku_puzzle.txt";
    std::ifstream fi(path_to_input_file);
    if(fi.is_open())
    {
        for(int i{0}; i < DIM; ++i)
        {
            for(int j{0}; j < DIM; ++j)
            {
                std::string line;
                std::getline(fi, line, ',');
                puzzle.data[i][j] = (line == "_" || line == "\n_") ? EMPTY : std::atoi(line.c_str());
            }
        }
        fi.close();
        if(solve(puzzle))
            print_out_the_solution(puzzle);
        else
            std::cout << "No solution found!" << std::endl;
    }
    else
    {
        std::cout << "Could not open input file with puzzle!" << std::endl;
    }

    return 0;
}
