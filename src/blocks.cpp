#include "blocks.hpp"

#include <unordered_map>
#include <iostream>
#include <cmath>
#include <stack>
#include <stdlib.h>

static std::unordered_map<float, Matrix2f> rotations;

void init()
{
    for (float theta: {0, 90, 180, 270}) {
        rotations[theta] = rot2d(theta);
    }
}

std::ostream& operator<<(std::ostream& out, BoardState state)
{
    std::unordered_map<BoardState, std::string> map = {
        {BoardState::EMPTY, "🔳"},
        {BoardState::OCCUPIED, "⬜"},
        {BoardState::DISALLOWED, "❌"},
        {BoardState::A, "🟦"},
        {BoardState::T, "🟫"},
        {BoardState::BIG_L, "🟩"},
        {BoardState::U, "🟧"},
        {BoardState::E, "🟥"},
        {BoardState::B, "🟪"},
        {BoardState::L, "🟨"},
        {BoardState::I, "🔵"},
        {BoardState::S, "🟢"},
    };
    out << map[state];
    return out;
}

bool operator==(BoardState left, BoardState right)
{
    return static_cast<int>(left) == static_cast<int>(right);
}

int operator|(BoardState left, BoardState right)
{
    return static_cast<int>(left) | static_cast<int>(right);
}


Matrix2f rot2d(float degrees)
{
    constexpr double PI = 3.14159265358979323846;
    float theta = degrees * (PI / 180);
    float c = std::cos(theta);
    float s = std::sin(theta);
    return Matrix2f({{c, -s}, {s, c}});
}

MatrixXi get_a(const Params& params)
{
    int x = params.x;
    int y = params.y;
    float rotation = params.rotation;
    bool mirror = params.mirror;

    MatrixXf A({{0, 0, 0, 0, 2, 2, 2, 2, 1, 1}, 
                {0, 1, 2, 3, 0, 1, 2, 3, 1, 3}});
    Matrix2f r = rotations[rotation];
    MatrixXi result = (rotations[rotation] * A).array().round().cast<int>();
    result.row(0).array() += x;
    result.row(1).array() += y;
    return result;
}

MatrixXi get_t(const Params& params)
{
    int x = params.x;
    int y = params.y;
    float rotation = params.rotation;
    bool mirror = params.mirror;

    MatrixXf A({{0, 1, 1, 1, 1, 2, 2},
                {2, 0, 1, 2, 3, 0, 2}});
    Matrix2f r = rotations[rotation];
    if (mirror) {
        A.row(0).array() = -A.row(0).array();
        A.row(0).array() += 3;
    }
    MatrixXi result = (rotations[rotation] * A).array().round().cast<int>();
    result.row(0).array() += x;
    result.row(1).array() += y;
    
    return result;
}

MatrixXi get_l(const Params& params)
{
    int x = params.x;
    int y = params.y;
    float rotation = params.rotation;
    bool mirror = params.mirror;

    MatrixXf A({{0, 0, 0, 0, 1},
                {0, 1, 2, 3, 0}});
    Matrix2f r = rotations[rotation];
    if (mirror) {
        A.row(0).array() = -A.row(0).array();
        A.row(0).array() += 2;
    }
    MatrixXi result = (rotations[rotation] * A).array().round().cast<int>();
    result.row(0).array() += x;
    result.row(1).array() += y;
    
    return result;
}

MatrixXi get_big_l(const Params& params)
{
    int x = params.x;
    int y = params.y;
    float rotation = params.rotation;
    bool mirror = params.mirror;

    MatrixXf A({{0, 0, 0, 0, 1, 2},
                {0, 1, 2, 3, 0, 0}});
    Matrix2f r = rotations[rotation];
    if (mirror) {
        A.row(0).array() = -A.row(0).array();
        A.row(0).array() += 3;
    }
    MatrixXi result = (rotations[rotation] * A).array().round().cast<int>();
    result.row(0).array() += x;
    result.row(1).array() += y;
    
    return result;
}

MatrixXi get_u(const Params& params)
{
    int x = params.x;
    int y = params.y;
    float rotation = params.rotation;
    bool mirror = params.mirror;

    MatrixXf A({{0, 0, 0, 1, 2, 2, 2},
                {0, 1, 2, 0, 0, 1, 2}});
    Matrix2f r = rotations[rotation];
    MatrixXi result = (rotations[rotation] * A).array().round().cast<int>();
    result.row(0).array() += x;
    result.row(1).array() += y;
    
    return result;
}

MatrixXi get_i(const Params& params)
{    
    int x = params.x;
    int y = params.y;
    float rotation = params.rotation;
    bool mirror = params.mirror;

    MatrixXf A({{0, 0, 0},
                {0, 1, 2}});
    Matrix2f r = rotations[rotation];
    MatrixXi result = (rotations[rotation] * A).array().round().cast<int>();
    result.row(0).array() += x;
    result.row(1).array() += y;
    
    return result;
}

MatrixXi get_s(const Params& params)
{
    int x = params.x;
    int y = params.y;
    float rotation = params.rotation;
    bool mirror = params.mirror;

    MatrixXf A({{0, 0, 1, 1},
                {0, 1, 1, 2}});
    Matrix2f r = rotations[rotation];
    if (mirror) {
        A.row(0).array() = -A.row(0).array();
        A.row(0).array() += 2;
    }
    MatrixXi result = (rotations[rotation] * A).array().round().cast<int>();
    result.row(0).array() += x;
    result.row(1).array() += y;
    
    return result;
}

MatrixXi get_e(const Params& params)
{
    int x = params.x;
    int y = params.y;
    float rotation = params.rotation;
    bool mirror = params.mirror;

    MatrixXf A({{0, 0, 1, 1, 2},
                {0, 1, 1, 2, 2}});
    Matrix2f r = rotations[rotation];
    if (mirror) {
        A.row(0).array() = -A.row(0).array();
        A.row(0).array() += 3;
    }
    MatrixXi result = (rotations[rotation] * A).array().round().cast<int>();
    result.row(0).array() += x;
    result.row(1).array() += y;
    
    return result;
}

MatrixXi get_b(const Params& params)
{
    int x = params.x;
    int y = params.y;
    float rotation = params.rotation;
    bool mirror = params.mirror;

    MatrixXf A({{0, 0, 0, 0, 1, 1},
                {0, 1, 2, 3, 2, 3}});
    Matrix2f r = rotations[rotation];
    if (mirror) {
        A.row(0).array() = -A.row(0).array();
        A.row(0).array() += 2;
    }
    MatrixXi result = (rotations[rotation] * A).array().round().cast<int>();
    result.row(0).array() += x;
    result.row(1).array() += y;
    
    return result;
}

std::array<Block, 14> get_blocks()
{
    return {
            get_a, // 10
            get_t, get_t, // 14 
            get_big_l, // 6
            get_u, get_u, // 14
            get_e, get_e, get_e, // 15
            get_b, // 6
            get_l, // 5
            get_i, get_i, // 6
            get_s // 4
           };
}

BoardState index_to_state(int index)
{
    switch (index) {
        case 0: return BoardState::A; 
        case 1:
        case 2: return BoardState::T;
        case 3: return BoardState::BIG_L;
        case 4:
        case 5: return BoardState::U;
        case 6:
        case 7:
        case 8: return BoardState::E;
        case 9: return BoardState::B;
        case 10: return BoardState::L;
        case 11:
        case 12: return BoardState::I;
        case 13: return BoardState::S;
        default: return BoardState::OCCUPIED;
    }
}

std::queue<Params> get_params()
{
    std::queue<Params> params;
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            for (float r: {0, 90, 270}) {
                params.push({x, y, r, false});
                params.push({x, y, r, true});
            }
        }
    }
    return params;
}

void print_board(Board& board)
{
    for (auto row: board) {
        for (BoardState pos: row) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
    }
}

void Clear()
{
#if defined _WIN32
    system("cls");
    //clrscr(); // including header file : conio.h
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
    //std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences 
#elif defined (__APPLE__)
    system("clear");
#endif
}

BoardState is_occupied(const Board& board, int x, int y)
{
    if (x < 0 || x > 9 || y < 0 || y > 9) {
        return BoardState::DISALLOWED;
    }
    return board[y][x];
}

int snugness(const Board& board)
{

    int snugness = 0;
    int total = 0;

    for (int y = 0; y < board.size(); y++) {
        for (int x = 0; x < board[y].size(); x++) {
            
            if (board[y][x] != BoardState::OCCUPIED) {
                continue;
            }
            total++;
            
            BoardState left, right, up, down;
            left = is_occupied(board, x - 1, y); 
            right = is_occupied(board, x + 1, y);
            down = is_occupied(board, x, y - 1);
            up = is_occupied(board, x, y + 1);

            for (BoardState state: {left, right, up, down}) {
                if (state != BoardState::EMPTY && state != BoardState::DISALLOWED) {
                    snugness++;
                }
            }

        }
    }
    return snugness;
}

bool sanity_check(const Board& board)
{

    if (board[0][0] == BoardState::EMPTY && board[1][0] != BoardState::EMPTY
        && board[1][0] != BoardState::EMPTY && board[1][1] != BoardState::EMPTY)
    {
        return false;
    }
    if (board[0][9] == BoardState::EMPTY && board[1][9] != BoardState::EMPTY
        && board[1][8] != BoardState::EMPTY && board[0][8] != BoardState::EMPTY)
    {
        return false;
    }
    if (board[9][0] == BoardState::EMPTY && board[9][1] != BoardState::EMPTY 
        && board[8][1] != BoardState::EMPTY && board[8][0] != BoardState::EMPTY) 
    {
        return false;
    }
    if (board[9][9] == BoardState::EMPTY && board[9][8] != BoardState::EMPTY 
        && board[8][9] != BoardState::EMPTY && board[8][8] != BoardState::EMPTY)
    {
        return false;
    }

    // return true;

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {

            BoardState left, right, up, down;
            left = is_occupied(board, x - 1, y); 
            right = is_occupied(board, x + 1, y);
            down = is_occupied(board, x, y - 1);
            up = is_occupied(board, x, y + 1);

            if (board[y][x] == BoardState::DISALLOWED) {
                if (left == BoardState::A && right == BoardState::A 
                    && up == BoardState::A && down == BoardState::A)
                {
                    continue;
                }
            }

            if (board[y][x] == BoardState::EMPTY) {
                
                if (left != BoardState::EMPTY && right != BoardState::EMPTY 
                && up != BoardState::EMPTY && down != BoardState::EMPTY)
                {
                    return false;
                }
            }
                
            
        }
    }

    return true;
}

bool can_be_placed(const Board& board, const std::vector<Position>& changes)
{
    for (auto [x, y]: changes) {
        if (board[y][x] != BoardState::EMPTY) {
            return false;
        }
    }
    return true;
}

void place(Board& board, const std::vector<Position>& changes, const BoardState state)
{
    for (auto [x, y]: changes) {
        board[y][x] = state;
    }
}

void solve(Board& board)
{
    ParamQueue param_queue;
    std::stack<std::vector<Position>> previous_changes;
    std::vector<Position> current_changes;
    
    std::vector<Params> current_configuration;
    
    std::chrono::time_point<hr_clock> now, t;
    std::chrono::milliseconds ms;

    // board[9][0] = 0;
    // board[9][1] = 1;
    // board[8][1] = 1;
    // board[8][0] = 1;

    // int xx = 0;
    // int yy = 9;
    // std::cout << "Board xy: " << (board[yy][xx] == 0) << std::endl;
    // std::cout << "Occupied: " << (is_occupied(board, xx - 1, yy) &&
    //             is_occupied(board, xx + 1, yy)) << std::endl;  
    // std::cout << "Occupied: " << (is_occupied(board, xx, yy - 1) &&
    //             is_occupied(board, xx, yy + 1)) << std::endl;


    // board[0][0] = 0;

    // print_board(board);
    // std::cout << sanity_check(board) << std::endl;

    // exit(0);

    uint64_t invalid = 0;
    uint64_t not_empty = 0;
    uint64_t rejected = 0;
    int block_index = 0;
    int counter = 0;
    int N = 100000;
    float total_time = 0.0f;
    std::array<Block, 14> blocks = get_blocks();

    for (int i = 0; i < 14; i++) {
        param_queue.push_back(get_params());
        current_configuration.push_back({0, 0, 0, false});
    }

    now = hr_clock::now();

    while (block_index < 14) {

        if (counter++ % N == 0) {
            Clear();
            std::cout << std::endl;

            std::cout << "Round: " << counter << std::endl;
            std::cout << "\t Placed blocks\t\t: " << block_index << std::endl;
            std::cout << "\t Invalid placements\t: " << std::round(((float)invalid/counter)*1000)/10 << "%" << std::endl;
            std::cout << "\t Occupied placements\t: " << std::round(((float)not_empty/counter)*1000)/10 << "%" << std::endl;
            std::cout << "\t Rejected placements\t: " << rejected << std::endl;
            
            t = hr_clock::now();
            ms = std::chrono::duration_cast<std::chrono::milliseconds>(t - now);
            total_time += ms.count();
            std::cout << "\t Time per " << N << " rounds\t: " << ms.count() << "ms" << std::endl;
            std::cout << "\t Avg time per n rounds\t: " << std::round((float)total_time/(counter/N)) << "ms" << std::endl;
            std::cout << "\t Iterations per s\t: " << std::round((1000/(float)ms.count())*N) << std::endl;
            std::cout << std::endl;
            now = t;
           
            std::cout << "Board state (0 = EMPTY, 1 = FILLED, 2 = DISALLOWED)\n" << std::endl;
            print_board(board);
        }
   
        while (param_queue[block_index].size() == 0) {
            param_queue[block_index] = get_params();
            if (previous_changes.size() > 0) {
                block_index--;
                std::vector<Position> &prev = previous_changes.top();
                for (auto [x, y]: prev) {
                    board[y][x] = BoardState::EMPTY;
                }
                previous_changes.pop();
            }
            // std::cout << "Backtracking" << std::endl;
            if (block_index < 0) {
                std::cout << "No solution found!" << std::endl;
                exit(1);
            }
        }
   
        Params params = param_queue[block_index].front();
        param_queue[block_index].pop();
        Coords coords = blocks[block_index](params);

        if (coords.minCoeff() < 0 || coords.maxCoeff() > 9) {
            invalid++;
            // std::cout << "Invalid" << std::endl;
            continue;
        }

        // std::cout << coords << std::endl;
        current_changes.clear();
        bool found = true;
        for (auto i = 0; i < coords.cols(); i++) {
            auto c = coords.col(i);
            int x = c[0];
            int y = c[1]; 
            if (board[y][x] != BoardState::EMPTY) {
                not_empty += 1;
                found = false;
                break;
            }
            current_changes.push_back({x, y});
        }
        if (!found) {
            // std::cout << "Not found" << std::endl;
            continue;
        }

        for (auto [x, y]: current_changes) {
            board[y][x] = BoardState::OCCUPIED;
        }
    
        if (sanity_check(board)) {
            previous_changes.push(current_changes);
        } else {
            rejected++;
            for (auto [x, y]: current_changes) {
                board[y][x] = BoardState::EMPTY;
            }
            continue;
        }

        // bool done = false;
        // for (auto row: board) {
        //     for (auto pos: row) {
        //         if (pos == 0) {
        //             done = false;
        //             break;
        //         }
        //     }
        // }
        // std::cout << "Found: " << counter << std::endl;
        // print_board(board);
        current_configuration[block_index] = params;
        block_index += 1;

        // if (done) {
            // TODO print configuration
        // }

    }




}
