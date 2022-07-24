#pragma once

#include <Eigen/Dense>

#include <functional>
#include <vector>
#include <chrono>
#include <array>
#include <queue>
#include <tuple>

typedef struct Params {
    int x = 0;
    int y = 0;
    float rotation = 0;
    bool mirror = false;
} Params;

enum class BoardState {
    ERROR       = 0,
    EMPTY       = 1,
    OCCUPIED    = 1 << 1,
    DISALLOWED  = 1 << 2,
    A           = 1 << 3,
    T           = 1 << 4,
    BIG_L       = 1 << 5,
    U           = 1 << 6,
    E           = 1 << 7,
    B           = 1 << 8,
    L           = 1 << 9,
    I           = 1 << 10,
    S           = 1 << 11
};

std::ostream& operator<<(std::ostream& out, BoardState state);
bool operator==(BoardState left, BoardState right);
int operator|(BoardState left, BoardState right);


using Eigen::MatrixXi;
using Eigen::MatrixXf;
using Eigen::Matrix2f;

using hr_clock = std::chrono::high_resolution_clock;

using Board = std::vector<std::vector<BoardState>>;
using ParamQueue = std::vector<std::queue<Params>>;
using Block = std::function<MatrixXi(Params)>;
using Coords = MatrixXi;
using Position = std::tuple<int,int>;

void init();

Matrix2f rot2d(float degrees);
MatrixXi get_a(const Params& params);
MatrixXi get_t(const Params& params);
MatrixXi get_l(const Params& params);
MatrixXi get_big_l(const Params& params);
MatrixXi get_u(const Params& params);
MatrixXi get_i(const Params& params);
MatrixXi get_s(const Params& params);
MatrixXi get_e(const Params& params);
MatrixXi get_b(const Params& params);

std::array<Block, 14> get_blocks();
BoardState index_to_state(int index);
std::queue<Params> get_params();

bool can_be_placed(const Board& board, const std::vector<Position>& changes);
void place(Board& board, const std::vector<Position>& changes, const BoardState state=BoardState::OCCUPIED);

BoardState is_occupied(const Board& board, int x, int y);
bool sanity_check(const Board& board);
int snugness(const Board& board);
void print_board(Board& board);

void solve(Board& board);

void Clear();
