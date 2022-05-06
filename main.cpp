#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <random>
#include <time.h>

#include "renderwindow.cpp"
#include "Ai.cpp"
#include "utils.hpp"


int main(int argc, char* args[])
{
    srand(time(NULL));


    //SDL init
    if(SDL_Init(SDL_INIT_VIDEO) > 0){
        std::cout << "SDL_Init FAILED. ERROR: " << SDL_GetError() << std::endl;
    }

    if(!(IMG_Init(IMG_INIT_PNG))){
        std::cout << "IMG_Init FAILED. ERROR: " << SDL_GetError() << std::endl;
    }

    const int SCREEN_WIDTH = 1920;

    const int SCREEN_HEIGHT = 1080;

    RenderWindow window("AI V1.0",SCREEN_WIDTH,SCREEN_HEIGHT);
    int windowRefreshRate = 1000/window.getRefreshRate();

    SDL_Texture* zero  = window.loadTexture("gfx/0.png");
    SDL_Texture* one   = window.loadTexture("gfx/1.png");
    SDL_Texture* two   = window.loadTexture("gfx/2.png");
    SDL_Texture* three = window.loadTexture("gfx/3.png");
    SDL_Texture* four  = window.loadTexture("gfx/4.png");
    SDL_Texture* five  = window.loadTexture("gfx/5.png");
    SDL_Texture* six   = window.loadTexture("gfx/6.png");
    SDL_Texture* seven = window.loadTexture("gfx/7.png");
    SDL_Texture* eight = window.loadTexture("gfx/8.png");
    SDL_Texture* nine  = window.loadTexture("gfx/9.png");

    int gen;
    bool pause;
    bool brain = false;
    int alive;
    int time = 0;
    int speed = 1;
    float mutateRate = 0.1;
    int highscore = 0;

    std::vector<SDL_Texture*>numbers {zero,one,two,three,four,five,six,seven,eight,nine};

    bool running = true;
    const float timestep = 0.01f;
    float accumTime;
    float currentTime = 0.0f;

    utils::Population pop(100);

    std::vector<utils::Obstacle> obs;
    obs.resize(7);

    for(int i=0;i<obs.size();i++){
        obs[i].pos.x = 1920/(obs.size())*i;
    }

    SDL_Event event;

    while(running){

        int startTicks = SDL_GetTicks();
        float newTime = utils::getTimeSec();
        float frameTime = newTime - currentTime;
        currentTime = newTime;
        accumTime += frameTime;
        
        while(accumTime>=timestep){
            while(SDL_PollEvent(&event)){
                if(event.type == SDL_QUIT){
                    running = false;    
                }
                else if(event.type == SDL_KEYDOWN){
                    switch(event.key.keysym.sym){
                        case SDLK_SPACE:
                            pause = !pause;
                            break;
                        case SDLK_q:
                            running = false;
                            break;
                        case SDLK_b:
                            brain = !brain;
                            break;
                        case SDLK_f:
                            speed++;
                            break;
                        case SDLK_s:
                            if(speed>1){
                                speed--;
                            }
                            break;
                        case SDLK_r:
                            speed = 1;
                            break;
                        case SDLK_w:
                            speed += 1000;
                            break;
                        case SDLK_k:
                            pop.newGen(mutateRate);
                            break;
                        case SDLK_e:
                            mutateRate += 0.01;
                            break;
                        case SDLK_d:
                            mutateRate -= 0.01;
                            break;
                    }
                }
            }
            accumTime -= timestep;
        }

        window.clear();
        window.setColor(12,12,12,255);
        SDL_RenderFillRect(window.getRenderer(),NULL);

        for(int i=0;i<obs.size();i++){
            window.drawObstacle(obs[i]);
        }
        for(int i=0;i<pop.currentGen.size();i++){
            if(!brain){
                window.drawBrain(pop.currentGen[i].brain,utils::Vector2i(900,pop.currentGen[i].yPos),30);
            }
            else{
                window.drawBird(pop.currentGen[i]);
            }
        }

        window.writeNumber(numbers,gen,utils::Vector2i(1800,100));
        window.writeNumber(numbers,alive,utils::Vector2i(1800,175));
        window.writeNumber(numbers,(int)(time/(1920/(obs.size()*3))),utils::Vector2i(1800,250));
        window.writeNumber(numbers,mutateRate, utils::Vector2i(1800,325));
        window.writeNumber(numbers,speed, utils::Vector2i(1800,400));
        window.writeNumber(numbers,highscore, utils::Vector2i(1800,475));


        window.display();

        
        
        if(pause){
            for(int i=0;i<speed;i++){
                int obsDist = 1000;
                int obsHeight = 0;
                for(int i=0;i<obs.size();i++){
                    if(obs[i].pos.x-900>0&&obs[i].pos.x-900<obsDist){
                        obsDist = obs[i].pos.x-900;
                        obsHeight = obs[i].pos.y;
                    }
                }
                time++;
                alive = pop.update(obsDist,obsHeight,mutateRate);
                if(alive==0){
                    if (time/(1920/(obs.size()*3))>highscore)
                        highscore = time/(1920/(obs.size()*3));
                    time = 0;
                    gen++;
                    for(int i=0;i<obs.size();i++){
                        obs[i] = utils::Obstacle();
                        obs[i].pos.x = 1920/(obs.size())*i;
                    }
                }


                for(int i=0;i<obs.size();i++){
                    obs[i].update();
                }
            }
        }
        

        int frameTicks = SDL_GetTicks()-startTicks;
        if(frameTicks<windowRefreshRate){
            SDL_Delay(windowRefreshRate-frameTicks);
        }
    }

    return 0;
}