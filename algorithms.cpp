#include "othello_cut.h"

enum Condition { Greater, GreaterThan };

int negamax(state_t state, int depth, int color, unsigned* generated, unsigned* expanded, bool use_tt = false)
{
    (*generated)++;

    if ( depth == 0 || state.terminal() ) return color * state.value();

    int score = INT32_MIN;
    std::vector<int> children = state.get_valid_moves(color);

    for(int child_pos : children)
    {
        (*expanded)++;
        state_t child = state.move(color, child_pos);
        int child_res = -negamax(child, depth-1, -color, generated, expanded, use_tt);
        score = (score >= child_res) ? score : child_res;
    }

    return score;
}

int negamax(state_t state, int depth, int alpha, int beta, int color, unsigned* generated, unsigned* expanded, bool use_tt = false)
{
    (*generated)++;

    if ( depth == 0 || state.terminal() ) return color * state.value();

    int score = INT32_MIN;
    std::vector<int> children = state.get_valid_moves(color);

    for(int child_pos : children)
    {
        (*expanded)++;
        state_t child = state.move(color, child_pos);
        int val = negamax(child, depth-1, -beta, -alpha, -color, generated, expanded, use_tt);
        score = (score >= val) ? score : val;
        alpha = (alpha >= val) ? alpha : val;

        if( alpha >= beta ) break;
    }

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

    std::vector<int> children = state.get_valid_moves(color);

    for(int child_pos : children)
    {
        // Missing part of first child, still need clarification
        state_t child = state.move(color, child_pos);
        
        if (color && TEST(child, depth - 1, score, color, condition)) return true;
        if (!color && !TEST(child, depth - 1, score, color, condition)) return false;
    }

    return !color;
}

int scout(state_t state, int depth, int color, unsigned* generated, unsigned* expanded, bool use_tt = false)
{
    (*generated)++;

    if ( depth == 0 || state.terminal() ) return state.value();

    int score = 0;
    bool first = true;
    std::vector<int> children = state.get_valid_moves(color);

    for(int child_pos : children)
    {
        (*expanded)++;
        state_t child = state.move(color, child_pos);

        if(first)
        {
            first = false;
            score = scout(child, depth - 1, color, generated, expanded, use_tt);
        }
        else 
        {
            if (color && TEST(child, depth, score, color, Greater)) 
                score = scout(child, depth - 1, color, generated, expanded, use_tt);
            if (!color && !TEST(child, depth, score, color, GreaterThan)) 
                score = scout(child, depth - 1, color, generated, expanded, use_tt);
        }
    }

    return score;
    
}

int negascout(state_t state, int depth, int alpha, int beta, int color, unsigned* generated, unsigned* expanded, bool use_tt = false)
{
    (*generated)++;

    if ( depth == 0 || state.terminal() ) return color * state.value();

    int score = 0;
    bool first = true;
    std::vector<int> children = state.get_valid_moves(color);

    for(int child_pos : children)
    {
        (*expanded)++;
        state_t child = state.move(color, child_pos);

        if(first)
        {
            first = false;
            score = -negascout(child, depth - 1, -beta, -alpha, -color, generated, expanded, use_tt);
        }
        else 
        {
            score = -negascout(child, depth - 1, -alpha - 1, -alpha, -color, generated, expanded, use_tt);
            
            if ( (alpha < score) && (score < beta) )
                score = -negascout(child, depth - 1, -beta, -score, -color, generated, expanded, use_tt);
        }

        alpha = (alpha >= score) ? alpha : score;
        if( alpha >= beta ) break;
    }

    return alpha;
}