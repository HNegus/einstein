#include <iostream>
#include <Eigen/Dense>
#include "blocks.hpp" 
#include "ea.hpp"

using Eigen::MatrixXd;
// using Eigen::Matrix4f;
using Eigen::MatrixXi;


int main(int, char**) {
    
    init();
    // std::cout << get_t(3, 2, 90, true) << std::endl;

    // const std::function<MatrixXi(Params)>& aa = get_a;
    // const std::function<MatrixXi(Params)>& bb = get_b;
    // std::cout << (aa.target<MatrixXi(Params)>() == bb.target<MatrixXi(Params)>()) << std::endl;
    // exit(0);

    Board b;
    for (int y = 0; y < 10; y++) {
        b.push_back(std::vector<BoardState>());
        for (int x = 0; x < 10; x++) {
            b[y].push_back(BoardState::EMPTY);
        }
    }

    // Coords coords = get_t({5, 5, 90, false});
    // std::vector<Position> vec;
    // for (int i = 0; i < coords.cols(); i++) {
    //     auto c = coords.col(i);
    //     vec.push_back({c[0], c[1]});
    // }
    // place(b, vec, BoardState::A);
    // print_board(b);

    // exit(0);

    std::vector<Position> disallowed = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0},
                                        {0, 1}, {1, 1}, {2, 1}, {3, 1},
                                        {0, 2}, {1, 2}, {2, 2},
                                        {0, 3}, {1, 3},
                                        {0, 4},
                                        {4, 4},
                                        {8, 8}, {8, 9}, {9, 8}, {9, 9}};

    for (auto [x, y]: disallowed) {
        b[y][x] = BoardState::DISALLOWED;
    }
    // solve(b);
    EA ea(500, b, 800);
    ea.solve();


}
