#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>

#include "Ai.hpp"

namespace utils{
    //______________________TIME________________________
    inline float getTimeSec(){
        return SDL_GetTicks()*0.001f;
    }
    //______________________VEC_________________________
    class Vector2i {
        public:
            int x;
            int y;
            Vector2i(int aX,int aY){
                x = aX;
                y = aY;
            }
            Vector2i(){
                x=0;
                y=0;
            }
    };
    
    class Vector2f {
        public:
            float x;
            float y;
            Vector2f(float aX,float aY){
                x = aX;
                y = aY;
            }
            Vector2f(){
                x=0;
                y=0;
            }
    };

    class Vector3f {
        public:
            float x;
            float y;
            float z;
            Vector3f(float aX,float aY,float aZ){
                x = aX;
                y = aY;
                z = aZ;
            }
            Vector3f(){
                x=0;
                y=0;
                z=0;
            }
    };
    //_____________________INTERP_______________________
    inline Vector2f interpF(Vector2f p0,Vector2f p1,float t){
        return Vector2f((1-t)*p0.x+t*p1.x,(1-t)*p0.y+t*p1.y);
    }

    inline Vector2i interpI(Vector2i p0,Vector2i p1,float t){
        return Vector2i((1-t)*p0.x+t*p1.x,(1-t)*p0.y+t*p1.y);
    }
    
    //______________________NOISE_______________________

    inline Vector2f getUnitVector(double angle){
        return Vector2f(cos(angle),sin(angle));
    }

    inline float dotProduct(Vector2f v0,Vector2f v1){
        return v0.x*v1.x+v0.y*v1.y;
    }

    inline Vector2f subtractVector(Vector2f v0,Vector2f v1){
        return Vector2f(v0.x-v1.x,v0.y-v1.y);
    }

    inline float fade(float t){
        return t*t*t*(t*(t*6.0-15.0)+10.0);
    }

    inline Vector2f grad(Vector2f &v,std::vector<std::vector<Vector2f>> &data){
        return data[(int)v.x][(int)v.y];
    }

    inline float noise(Vector2f p,std::vector<std::vector<Vector2f>> data){
        Vector2f p0(floor(p.x),floor(p.y));
        Vector2f p1(floor(p.x)+1,floor(p.y));
        Vector2f p2(floor(p.x),floor(p.y)+1);
        Vector2f p3(floor(p.x)+1,floor(p.y)+1);

        Vector2f g0 = grad(p0,data);
        Vector2f g1 = grad(p1,data);
        Vector2f g2 = grad(p2,data);
        Vector2f g3 = grad(p3,data);

        float t0 = p.x-p0.x;
        float fade_t0 = fade(t0);

        float t1 = p.y-p0.y;
        float fade_t1 = fade(t1);

        float p0p1 = (1.0 - fade_t0) * dotProduct(g0, subtractVector(p, p0)) + fade_t0 * dotProduct(g1, subtractVector(p, p1));
        float p2p3 = (1.0 - fade_t0) * dotProduct(g2, subtractVector(p, p2)) + fade_t0 * dotProduct(g3, subtractVector(p, p3));

        return (1.0 - fade_t1) * p0p1 + fade_t1 * p2p3;
    }

    inline void getRandomArray(std::vector<std::vector<Vector2f>> &data,int p_w,int p_h){
        for(int row=0;row<p_w;row++){
            for(int column=0;column<p_h;column++){
                float angle = ((float)rand()/(RAND_MAX*2*3.14159));
                data[row][column] = Vector2f(cos(angle),sin(angle));
            }
        }
    }

    inline void getNoise(std::vector<std::vector<float>> &finalNoise,int p_w,int p_h,int p_scale){
        std::vector<std::vector<Vector2f>> data(ceil(p_scale)*2,std::vector<Vector2f>(ceil(p_scale)*2,Vector2f(0,0)));
        getRandomArray(data,ceil(p_scale)*2,ceil(p_scale)*2);
        for(int row=0;row<p_w;row++){
            for(int column=0;column<p_h;column++){
                finalNoise[row][column] = noise(Vector2f(((float)row/(float)p_w/(float)p_scale),((float)column/(float)p_h/(float)p_scale)),data);
            }
        }
    }

    inline void getFractalNoise(std::vector<std::vector<float>> &finalNoise,int p_w,int p_h,int p_layers,float p_scale,float p_var,float amp){
        
        std::vector<std::vector<float>> layerNoise(p_w,std::vector<float>(p_h,0));

        for(int layer = 0;layer<p_layers;layer++){
            getNoise(layerNoise,p_w,p_h,pow(2,layer)*p_scale);
            for(int row = 0;row<p_w;row++){
                for(int column=0;column<p_h;column++){
                    finalNoise[row][column] += layerNoise[row][column]*pow(2.0,-layer*p_var)*amp;
                }
            }
        }
    }

    class Obstacle{
        public:
            Vector2i pos;
            inline Obstacle(){
                pos.x = 1925;
                pos.y = rand()/(RAND_MAX/800)+150;
            }
            inline void update(){
                if(pos.x<0){
                    pos.x=1925;
                    pos.y = rand()/(RAND_MAX/800)+150;
                }
                else{
                    pos.x-=3;
                }
            }
    };

    class Bird{
        public:
            int yPos;
            int vel;
            int acc;
            int fit;
            int time;
            bool isDead;
            Brain brain;
            int r;
            int g;
            int b;
            Bird():brain(4,1){
                yPos = 500;
                r = (float)rand()/RAND_MAX*255;
                g = (float)rand()/RAND_MAX*255;
                b = (float)rand()/RAND_MAX*255;
            }
            Bird(Bird parent,bool mutate,float mutateRate):brain(parent.brain,mutate,mutateRate){
                if((float)rand()/RAND_MAX<0.01){
                    brain = Brain(4,1);
                    r = (float)rand()/RAND_MAX*255;
                    g = (float)rand()/RAND_MAX*255;
                    b = (float)rand()/RAND_MAX*255;
                }
                else{
                    r = parent.r;
                    g = parent.g;
                    b = parent.b;
                }
                yPos = 500;
            }
            inline void update(int obsDist,int obsHeight,bool addFit){
                
                if(brain.calcOutput(std::vector<int>{obsDist,obsHeight,yPos,vel})[0]>0){
                    acc = -20;
                }
                else{
                    acc = 2;
                }
                vel += acc;
                if(vel>20){
                    vel = 20;
                }
                else if(vel<-20){
                    vel = -20;
                }
                yPos += vel;

                if((obsDist<20&&(yPos>obsHeight||yPos<obsHeight-100))||yPos>1080){
                    isDead = true;
                }
                if(addFit){
                    time++;
                    fit = pow(time, 1.5);
                }
                else{
                    yPos = 1000;
                }
            }
            void operator=(const Bird& bird){
                brain = bird.brain;
                fit = 0;
                time = 0;
                yPos = 500;
                vel = 0;
                acc = 0;
                r = bird.r;
                g = bird.g;
                b = bird.b;
            }
    };

    class Population{
        public:
            std::vector<Bird> currentGen;
            std::vector<Bird> nextGen;
            Population(int size){
                currentGen.resize(size);
                nextGen.resize(size);
                for(int i=0;i<size;i++){
                    currentGen[i] = Bird();
                }
            }
            inline int update(int obsDist,int obsHeight,float mutateRate){
                bool allDead = true;
                int count = 0;
                for(int i=0;i<currentGen.size();i++){
                    if(!currentGen[i].isDead){
                        count++;
                        allDead = false;
                        currentGen[i].update(obsDist,obsHeight,true);
                    }
                    else{
                        currentGen[i].update(obsDist,obsHeight,false);
                    }

                }
                if(allDead){
                    newGen(mutateRate);
                    return 0;
                }
                return count;
            }

            inline void newGen(float mutateRate){
                
                long totalFit = 0;
                int maxFit = 0;
                int maxFitIndex;
                for(int i=0;i<currentGen.size();i++){
                    totalFit+=currentGen[i].fit;
                    if(maxFit<currentGen[i].fit){
                        maxFit = currentGen[i].fit;
                        maxFitIndex = i;
                    }
                }
                int randFit;
                int rollingFit;
                for(int i=0;i<nextGen.size();i++){
                    randFit = (float)rand()/((float)RAND_MAX/totalFit);
                    rollingFit = 0;
                    for(int j=0;j<currentGen.size();j++){
                        rollingFit+=currentGen[j].fit;
                        if(rollingFit>randFit){
                            nextGen[i] = Bird(currentGen[j],true,mutateRate);
                            break;
                        }
                    }
                }
                for(int i=0;i<currentGen.size();i++){
                    currentGen[i] = nextGen[i];
                    currentGen[i].isDead = false;
                }

            }
            
    };

    

    
}