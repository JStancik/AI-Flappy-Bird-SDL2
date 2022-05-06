#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils.hpp"
#include "Ai.hpp"

class RenderWindow{
    public:
        RenderWindow(const char* p_title,int p_w,int p_h);
        SDL_Texture* loadTexture(const char* p_filePath);
        void cleanUp();
        void clear();
        int getRefreshRate();
        SDL_Renderer* getRenderer();
        void display();
        void setColor(int r,int g,int b,int a);
        void fillTri(utils::Vector2i p0,utils::Vector2i p1,utils::Vector2i p2);
        void drawTri(utils::Vector2i p0,utils::Vector2i p1,utils::Vector2i p2);
        void drawCircle(utils::Vector2i p0, int r);
        void fillCircle(utils::Vector2i p0, int r);
        /**
         * \brief for using the symetry of a circle to draw points in all octants
        */
        void drawCirclePoints(int x,int y,int ofsX,int ofsY);
        /**
         * \brief for connecting opposite sides of circle during filling
        */
        void connectCirclePoints(int x,int y,int ofsX,int ofsY);
        void drawBrain(Brain brain,utils::Vector2i pos,float scale);
        void drawObstacle(utils::Obstacle obs);
        void drawBird(utils::Bird bird);
        void writeNumber(std::vector<SDL_Texture*>,int number, utils::Vector2i pos);
        void writeNumber(std::vector<SDL_Texture*> tex, float number, utils::Vector2i pos);
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
};