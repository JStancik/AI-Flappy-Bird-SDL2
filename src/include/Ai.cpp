#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <random>
#include <iostream>

#include "Ai.hpp"
void Brain::operator=(const Brain & brain0){
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

Brain::Brain(int input, int output){
    int layerCount = 3+(rand()*3/RAND_MAX);
    nodes.resize(layerCount);
    bias.resize(layerCount-1);
    weights.resize(layerCount-1);
    for(int i=0;i<layerCount;i++){
        if(i==0){
            initLayer(i,input);
        }
        else if(i==layerCount-1){
            initLayer(i,output);
        }
        else{
            int layerWidth = 2+(rand()*4/RAND_MAX);
            initLayer(i,layerWidth);
        }
    }
}

Brain::Brain(Brain parent,bool mutate,float mutateRate){
    nodes.resize(parent.nodes.size());
    for(int i=0;i<nodes.size();i++){
        nodes[i].resize(parent.nodes[i].size());
    }
    weights.resize(parent.weights.size());

    for(int i=0;i<weights.size();i++){
        weights[i].resize(parent.weights[i].size());
        for(int j=0;j<weights[i].size();j++){
            weights[i][j].resize(parent.weights[i][j].size());
            for(int k=0;k<weights[i][j].size();k++){

                if((float)rand()/RAND_MAX<mutateRate&&mutate){
                    weights[i][j][k] += (float)rand()/(RAND_MAX*50)-.01;
                    if(weights[i][j][k]>1){
                        weights[i][j][k]=1;
                    }
                    else if(weights[i][j][k]<-1){
                        weights[i][j][k] = -1;
                    }
                }

                else{
                    weights[i][j][k] = parent.weights[i][j][k];
                }
            }
        }
    }

    bias.resize(parent.bias.size());
    for(int i=0;i<bias.size();i++){
        bias[i].resize(parent.bias[i].size());
        for(int j=0;j<bias[i].size();j++){
            if((float)rand()/RAND_MAX<mutateRate&&mutate){
                bias[i][j] += (float)rand()/RAND_MAX-1;
                if(bias[i][j]>0){
                    bias[i][j] = 0;
                }
                else if(bias[i][j]<-10){
                    bias[i][j] = -10;
                }
            }
            

            else{
                bias[i][j] = parent.bias[i][j];
            }
        }
    }
}

void Brain::initLayer(int layer, int size){
    nodes[layer].resize(size);
    
    if(layer!=0){
        for(int i=0;i<size;i++){
            bias[layer-1].push_back(-(float)rand()*10.0/RAND_MAX);
        }
        weights[layer-1].resize(size);
        for(int i=0;i<size;i++){
            weights[layer-1][i].resize(nodes[layer-1].size());
            for(int j=0;j<weights[layer-1][i].size();j++){
                weights[layer-1][i][j] = (float)rand()*2.0/(float)RAND_MAX-1;
            }
        }
    }
}

std::vector<float> Brain::calcOutput(std::vector<int> input){
    for(int i=0;i<nodes[0].size();i++){
        nodes[0][i] = input[i];
    }
    
    for(int i=1;i<nodes.size();i++){
        for(int j=0;j<nodes[i].size();j++){
            nodes[i][j] = 0;
            for(int k=0;k<nodes[i-1].size();k++){
                nodes[i][j] += nodes[i-1][k]*weights[i-1][j][k];
            }
            nodes[i][j] += bias[i-1][j];

            if(nodes[i][j]<0){
                nodes[i][j]==0;
            }
        }
    }
    return nodes[nodes.size()-1];
}