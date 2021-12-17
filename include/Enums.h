#pragma once
#include <iostream>
#include <vector>
#include <string>

enum class TypeLexeme
{
    number,
    variable,
    un_op,
    bin_op,
    left_scope,
    right_scope,
    end,
    error
};

enum class Priority
{
    none,
    number,
    scope,
    low,
    mid,
    hight
};

enum class LexemeState
{
    state_start,
    state_variable,
    state_un_op,
    state_bin_op,
    state_left_scope,
    state_right_scope,
    state_end,
    state_error
};
