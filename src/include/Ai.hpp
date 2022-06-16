#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include "utils.hpp"

class Brain{
    public:
        /**
         * \brief constructor for first generation only, will randomize genes
         * \param input amount of input values
         * \param output amount of output values
        */
        Brain(int input, int output);
        /**
         * \brief constructor for getting child brain from a parent brain
        */
        Brain(Brain parent,bool mutate,float mutateRate);
        /**
         * \brief used to calculate result based on input to brain
         * \param inputVal pointer to array of input values
        */
        std::vector<float> calcOutput(std::vector<int> inputs);
        std::vector<std::vector<std::vector<float>>> weights;
        std::vector<std::vector<float>> bias;
        std::vector<std::vector<float>> nodes;
        void operator=(const Brain & brain0);
    protected:
        void initLayer(int layer,int size);            
};