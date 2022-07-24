#pragma once

#include "blocks.hpp"

#include <random>

struct Individual {
    std::array<Params, 14> params;
    Board board;
    float fitness = 0.0f;
    int unplaced = 0;
    int lifetime = 0;
};

class EA {

public: 
    EA(int popsize, Board& board, float mutate=0.05f, int threshold=80);
    void solve();

private:

    std::random_device m_rd;
    std::mt19937 m_gen;

    int m_size; 
    int m_threshold;
    float m_mutate;


    Individual m_best_individual;

    std::array<Block, 14> m_blocks;
    Board m_initial_board;
    std::vector<Individual> m_individuals;
    
    void show(int counter);
    void clear_board(Individual& individual);
    void place_blocks(Individual& individual);
    void mutate(Individual& individual);
    Individual& select_parent(int tournament_size=4);
    void recombine();
    void copy_fitter_individual(const Individual& src, Individual &dest, bool check_fitness=true);
    void sort_by_fitness();
    int randint(int left=0, int right=9);
    float randfloat();
    float randrotation();



};