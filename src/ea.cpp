#include "ea.hpp"
#include <iostream>

EA::EA(int popsize, Board& board, float mutate, int threshold) 
    : m_blocks(get_blocks()), m_size(popsize), m_mutate(mutate), 
    m_threshold(threshold), m_gen(m_rd())
{

    m_initial_board.resize(board.size());
    for (int y = 0; y < board.size(); y++) {
        m_initial_board[y].resize(board[y].size());
        for (int x = 0; x < board[y].size(); x++) {
            m_initial_board[y][x] = board[y][x];
        }
    }

    m_individuals.resize(popsize);
    for (int i = 0; i < m_individuals.size(); i++) {
        Individual& individual = m_individuals[i];
        individual.board.resize(board.size());
        for (int y = 0; y < board.size(); y++) {
            individual.board[y].resize(board[y].size());
        }

        // init params
        for (Params& param: individual.params) {
            param.x = randint();
            param.y = randint();
            param.rotation = randrotation();
            param.mirror = randfloat() >= 0.5;
        }
    }
    m_best_individual.board.resize(board.size());
    for (int y = 0; y < board.size(); y++) {
        m_best_individual.board[y].resize(board[y].size());
    }

    // init params
    for (Params& param: m_best_individual.params) {
        param.x = 0;
        param.y = 0;
        param.rotation = 0;
        param.mirror = false;
    }
}

void EA::solve()
{

    int counter = 0;

    while (m_best_individual.fitness < m_threshold || m_best_individual.unplaced > 0) {
        
        for (Individual& individual: m_individuals) {
            clear_board(individual);
            mutate(individual);
            place_blocks(individual);
            individual.lifetime++;
        }
        sort_by_fitness();
        copy_fitter_individual(m_individuals[0], m_best_individual);
        recombine();
        m_best_individual.lifetime++;
        if (counter++ % 10 == 0) {
            show(counter-1);
        }
        
        if (m_best_individual.lifetime > 10000) {
            m_best_individual.fitness--;
            // m_best_individual.lifetime = 0;
        }
    }
    show(counter);
}

void EA::show(int counter)
{

    Clear();

    float best = m_best_individual.fitness;
    float max = m_individuals[0].fitness;

    std::cout << std::endl;
    std::cout << "Iteration\t: " << counter << std::endl;
    std::cout << "Population\t: " << m_size << std::endl;
    std::cout << "Best unplaced\t: " << m_best_individual.unplaced << std::endl;
    std::cout << "Best lifetime\t: " << m_best_individual.lifetime << std::endl;
    std::cout << "Best fitness\t: " << best << std::endl;
    
    std::cout << std::endl;
    std::cout << "Max unplaced\t: " << m_individuals[0].unplaced << std::endl;
    std::cout << "Max fitness\t: " << max << std::endl;
    

    for (int i = 0; i < best; i++) {
        std::cout << "⥬";
    }
    for (int i = best; i < m_threshold; i++) {
        std::cout << " ";
    }
    std::cout << "|||||" << std::endl;


    for (int i = 0; i < max; i++) {
        std::cout << "⥭";
    }
    for (int i = max; i < m_threshold; i++) {
        std::cout << " ";
    }
    std::cout << "|||||" << std::endl;
    std::cout << std::endl;

    std::cout << "Board state" << std::endl;
    print_board(m_individuals[0].board);

    std::cout << std::endl;
    std::cout << "Best board" << std::endl;
    print_board(m_best_individual.board);
}

void EA::clear_board(Individual& individual)
{
    for (int y = 0; y < individual.board.size(); y++) {
        for (int x = 0; x < individual.board[y].size(); x++) {
            individual.board[y][x] = m_initial_board[y][x];
        }
    }
}

Individual& EA::select_parent(int tournament_size)
{
    Individual &a = m_individuals[randint(0, m_individuals.size() - 1)];
    Individual &b = m_individuals[randint(0, m_individuals.size() - 1)];
    
    for (int i = 0; i < tournament_size; i++) {
        if (a.fitness > b.fitness) {
            b = m_individuals[randint(0, m_individuals.size() - 1)];
        } else {
            a = m_individuals[randint(0, m_individuals.size() - 1)];
        }
    }
    return a.fitness > b.fitness ? a : b;
}

void EA::recombine()
{

    if (randfloat() <= 0.2f) {
        copy_fitter_individual(m_best_individual, m_individuals[m_individuals.size()/2 - 2], false);
    }

    for (int i = m_individuals.size() - (m_individuals.size() / 2); i < m_individuals.size(); i++) {

        Individual &child = m_individuals[i];

#define RECOMBINE1
#ifdef RECOMBINE3
        Individual &parent = select_parent(10);

        copy_fitter_individual(parent, child, false);
#endif
#ifdef RECOMBINE2
        Individual &parent = select_parent(10);

        for (int j = 0; j < child.params.size(); j++) {
            child.params[j].x = parent.params[j].x + randint(-2, 2);
            child.params[j].y = parent.params[j].y + randint(-2, 2);
            child.params[j].rotation = parent.params[j].rotation;
            child.params[j].mirror = parent.params[j].mirror;
        }
#endif
#ifdef RECOMBINE1

        Individual &parent1 = select_parent(4);
        Individual &parent2 = select_parent(4);

        int crossover_point = randint(0, child.params.size());

        for (int j = 0; j < crossover_point; j++) {
            child.params[j].x = parent1.params[j].x;
            child.params[j].y = parent1.params[j].y;
            child.params[j].rotation = parent1.params[j].rotation;
            child.params[j].mirror = parent1.params[j].mirror;
        }

        for (int j = crossover_point; j < child.params.size(); j++) {
            child.params[j].x = parent1.params[j].x;
            child.params[j].y = parent1.params[j].y;
            child.params[j].rotation = parent1.params[j].rotation;
            child.params[j].mirror = parent1.params[j].mirror;
        }
#endif
        child.lifetime = 0;
    }
}

void EA::mutate(Individual& individual)
{
    for (auto& param: individual.params) {

#define MUTATE1
#ifdef MUTATE3
        param.x = randfloat() <= m_mutate ? randint(-2, 2) + param.x : param.x;
        param.y = randfloat() <= m_mutate ? randint(-2, 2) + param.y : param.y;
        param.rotation = randfloat() <= m_mutate ? randrotation() : param.rotation;
        param.mirror = randfloat() <= m_mutate ? !param.mirror : param.mirror;
#endif

#ifdef MUTATE2
        param.x = randfloat() <= m_mutate ? randint() : param.x;
        param.y = randfloat() <= m_mutate ? randint() : param.y;
        param.rotation = randfloat() <= m_mutate ? randrotation() : param.rotation;
        param.mirror = randfloat() <= m_mutate ? !param.mirror : param.mirror;
#endif
#ifdef MUTATE1
        if (randfloat() <= m_mutate) {
            param.x = randint();
            param.y = randint();
            param.rotation = randrotation();
            param.mirror = !param.mirror;
        }
#endif
    }
    individual.lifetime = 0;
}

void EA::copy_fitter_individual(const Individual& src, Individual &dest, bool check_fitness)
{
    
    if (check_fitness && src.fitness < dest.fitness) {
        return;
    } else if (check_fitness && src.fitness == dest.fitness && src.lifetime > dest.lifetime) {
        return;
    }

    dest.fitness = src.fitness;
    dest.unplaced = src.unplaced;
    dest.lifetime = 0;
    
    for (int i = 0; i < src.params.size(); i++) {
        dest.params[i].x = src.params[i].x;
        dest.params[i].y = src.params[i].y;
        dest.params[i].rotation = src.params[i].rotation;
        dest.params[i].mirror = src.params[i].mirror;
    }

    for (int y = 0; y < src.board.size(); y++) {
        for (int x = 0; x < src.board[y].size(); x++) {
            dest.board[y][x] = src.board[y][x];
        }
    }
}

void EA::place_blocks(Individual& individual)
{
    float fitness = 0.0f;
    int unplaced = 0;
    Board& board = individual.board;
    auto& params = individual.params;

    std::vector<int> indices;
    for (int i = 0; i < m_blocks.size(); i++) {
        indices.push_back(i);
    }

    std::shuffle(indices.begin(), indices.end(), m_gen);

    for (int i: indices) {
        int block_index = i;
        MatrixXi coords = m_blocks[block_index](params[block_index]);
        
        if (coords.minCoeff() < 0 || coords.maxCoeff() > 9) {
            // fitness--;
            unplaced++;
            continue;
        }

        std::vector<Position> changes;
        for (auto i = 0; i < coords.cols(); i++) {
            auto c = coords.col(i);
            changes.push_back({c[0], c[1]});
        }
        if (can_be_placed(board, changes)) {
            place(board, changes, index_to_state(block_index));
            fitness += 1.0f / changes.size();
            // fitness += 1;
        } else {
            unplaced++;
        }
    
    }
    // fitness += snugness(board);
    // fitness += 14 - unplaced;
    if (!sanity_check(board)) {
        fitness = 0;
    }
    if (board[5][4] == BoardState::A && board[3][4] == BoardState::A) {
        // fitness += 2;
    }
    individual.fitness = fitness;
    individual.unplaced = unplaced;
}

void EA::sort_by_fitness()
{
    std::sort(m_individuals.begin(), m_individuals.end(), 
        [](const Individual& a, const Individual &b) -> bool {
            return a.fitness > b.fitness;
        });
}

int EA::randint(int left, int right)
{
    std::uniform_int_distribution<> distrib(left, right);
    return distrib(m_gen);
}

float EA::randrotation()
{

    std::uniform_int_distribution<> distrib(0, 3);
    std::array<float, 4> rotations = {0, 90, 180, 270}; 
    return rotations[distrib(m_gen)];
}

float EA::randfloat()
{
    std::uniform_real_distribution<> distrib(0, 1);
    return distrib(m_gen);
}
