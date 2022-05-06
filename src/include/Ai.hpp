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
        void operator=(const Brain & brain0){
            weights.resize(brain0.weights.size());

            for(int i=0;i<weights.size();i++){
                weights[i].resize(brain0.weights[i].size());
                for(int j=0;j<weights[i].size();j++){
                    weights[i][j].resize(brain0.weights[i][j].size());
                    for(int k=0;k<weights[i][j].size();k++){
                            weights[i][j][k] = brain0.weights[i][j][k];
                    }
                }
            }

            bias.resize(brain0.bias.size());
            for(int i=0;i<bias.size();i++){
                bias[i].resize(brain0.bias[i].size());
                for(int j=0;j<bias[i].size();j++){
                        bias[i][j] = brain0.bias[i][j];
                }
            }
            nodes.resize(brain0.nodes.size());
            for(int i=0;i<nodes.size();i++){
                nodes[i].resize(brain0.nodes[i].size());
            }
        }
    protected:
        void initLayer(int layer,int size);            
};