#include "othello_cut.h"
#include <limits>

enum Condition { Greater, GreaterThan };

int negamax(state_t state, int depth, int color, unsigned* generated, unsigned* expanded, bool use_tt = false)
{
    (*generated)++;

    if ( depth == 0 || state.terminal() ) return color * state.value();

    bool is_color = color == 1;

    int score = INT32_MIN;
    std::vector<int> children = state.get_valid_moves(is_color);

    for(int child_pos : children)
    {
        state_t child = state.move(is_color, child_pos);
        int child_res = -negamax(child, depth-1, -color, generated, expanded);
        score = (score >= child_res) ? score : child_res;
    }

    (*expanded)++;
    return score;
}

int negamax(state_t state, int depth, int alpha, int beta, int color, unsigned* generated, unsigned* expanded, bool use_tt = false)
{
    (*generated)++;

    if ( depth == 0 || state.terminal() ) return color * state.value();

    bool is_color = color == 1;

    int score = INT32_MIN;
    std::vector<int> children = state.get_valid_moves(is_color);

    for(int child_pos : children)
    {
        state_t child = state.move(is_color, child_pos);
        int val = -negamax(child, depth-1, -beta, -alpha, -color, generated, expanded);
        score = (score >= val) ? score : val;
        alpha = (alpha >= score) ? alpha : score;

        if( alpha >= beta ) break;
    }

    (*expanded)++;
    return score;
}

bool TEST(state_t state, int depth, int score, int color, Condition condition)
{
    if ( depth == 0 || state.terminal() ) 
    {
        if( condition == Greater)
            return state.value() > score ? true: false;
        
        return state.value() >= score ? true: false;
    }

    bool is_color = color == 1;

    std::vector<int> children = state.get_valid_moves(is_color);

    for(int child_pos : children)
    {
        state_t child = state.move(is_color, child_pos);
        
        if (is_color && TEST(child, depth - 1, score, -color, condition)) return true;
        if (!is_color && !TEST(child, depth - 1, score, -color, condition)) return false;
    }

    return !(is_color);
}

int scout(state_t state, int depth, int color, unsigned* generated, unsigned* expanded, bool use_tt = false)
{
    (*generated)++;

    if ( depth == 0 || state.terminal() ) return state.value();

    bool is_color = color == 1;

    int score = 0;
    bool first = true;
    std::vector<int> children = state.get_valid_moves(is_color);

    for(int child_pos : children)
    {
        state_t child = state.move(is_color, child_pos);

        if(first)
        {
            score = scout(child, depth - 1, -color, generated, expanded);
            first = false;
        }
        else 
        {
            if (is_color && TEST(child, depth, score, -color, Greater)) 
                score = scout(child, depth - 1, -color, generated, expanded);
            if (!is_color && !TEST(child, depth, score, -color, GreaterThan)) 
                score = scout(child, depth - 1, -color, generated, expanded);
        }
    }

    (*expanded)++;
    return score;
    
}

int negascout(state_t state, int depth, int alpha, int beta, int color, unsigned* generated, unsigned* expanded, bool use_tt = false)
{

    if ( depth == 0 || state.terminal() ) return color * state.value();

    bool is_color = color == 1;

    int score = 0;
    bool first = true;
    std::vector<int> children = state.get_valid_moves(is_color);

    for(int child_pos : children)
    {
        (*generated)++;
        state_t child = state.move(is_color, child_pos);

        if(first)
        {
            first = false;
            score = -negascout(child, depth - 1, -beta, -alpha, -color, generated, expanded);
        }
        else 
        {
            score = -negascout(child, depth - 1, -alpha - 1, -alpha, -color, generated, expanded);
            
            if ( (alpha < score) && (score < beta) )
                score = -negascout(child, depth - 1, -beta, -score, -color, generated, expanded);
        }

        alpha = (alpha >= score) ? alpha : score;
        if( alpha >= beta ) break;
    }
    
    (*expanded)++;
    return alpha;
}