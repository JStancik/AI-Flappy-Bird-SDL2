#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title,int p_w,int p_h):window(NULL), renderer(NULL){
    window=SDL_CreateWindow(p_title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,p_w,p_h,SDL_WINDOW_SHOWN);

    if(window==NULL){
        std::cout << "Window failed to init. Error: " << SDL_GetError << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED && SDL_RENDERER_PRESENTVSYNC);
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath){
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer,p_filePath);

    if(texture==NULL){
        std::cout<<"Failed to load texture. Error: "<<SDL_GetError()<<std::endl;
    }

    return texture;
}

int RenderWindow::getRefreshRate(){
    int displayIndex = SDL_GetWindowDisplayIndex( window );

    SDL_DisplayMode mode;

    SDL_GetDisplayMode(displayIndex,0,&mode);

    return mode.refresh_rate;
}

SDL_Renderer* RenderWindow::getRenderer(){
    return renderer;
}

void RenderWindow::clear(){
    SDL_RenderClear(renderer);
}

void RenderWindow::display(){
    SDL_RenderPresent(renderer);
}

void RenderWindow::cleanUp(){
    SDL_DestroyWindow(window);
}

void RenderWindow::setColor(int r,int g,int b,int a){
    SDL_SetRenderDrawColor(renderer,r,g,b,a);
}

void RenderWindow::fillTri(utils::Vector2i p0,utils::Vector2i p1,utils::Vector2i p2){
        std::vector<utils::Vector2i> ps;
        utils::Vector2i temp;
        int dx;
        int sx;
        int dy;
        int sy;
        int err;
        int e2;
        utils::Vector2i p0t = p0;
        dx = abs(p0.x-p1.x);
        sx=p0.x<p1.x?1:-1;
        dy=-abs(p0.y-p1.y);
        sy=p0.y<p1.y?1:-1;
        err=dx+dy;
        while(true){
            ps.push_back(p0);
            if (p0.x == p1.x && p0.y == p1.y){
                break;
            }
            e2 = 2*err;
            if (e2 >= dy){
                err += dy;
                p0.x += sx;
            }
            if (e2 <= dx){
                err += dx;
                p0.y += sy;
            }
        }

        dx = abs(p1.x-p2.x);
        sx=p1.x<p2.x?1:-1;
        dy=-abs(p1.y-p2.y);
        sy=p1.y<p2.y?1:-1;
        err=dx+dy;
        while(true){
            ps.push_back(p1);
            if (p1.x == p2.x && p1.y == p2.y){
                break;
            }
            e2 = 2*err;
            if (e2 >= dy){
                err += dy;
                p1.x += sx;
            }
            if (e2 <= dx){
                err += dx;
                p1.y += sy;
            }
        }

        dx = abs(p2.x-p0t.x);
        sx=p2.x<p0t.x?1:-1;
        dy=-abs(p2.y-p0t.y);
        sy=p2.y<p0t.y?1:-1;
        err=dx+dy;
        while(true){
            ps.push_back(p2);
            if (p2.x == p0t.x && p2.y == p0t.y){
                break;
            }
            e2 = 2*err;
            if (e2 >= dy){
                err += dy;
                p2.x += sx;
            }
            if (e2 <= dx){
                err += dx;
                p2.y += sy;
            }
        }

        for(int i=1;i<ps.size();i++){
            for(int j=0;j<ps.size()-1;j++){
                if(ps[j].y>ps[j+1].y){
                    temp=ps[j];
                    ps[j]=ps[j+1];
                    ps[j+1]=temp;
                }
            }
        }
        for(int i=0;i<ps.size()-1;i++){
            if(ps[i].y==ps[i+1].y&&(ps[i].x+1!=ps[i+1].x||ps[i].x-1!=ps[i+1].x)){
                SDL_RenderDrawLine(renderer,ps[i].x,ps[i].y,ps[i+1].x,ps[i+1].y);
            }
        }
    }

void RenderWindow::drawTri(utils::Vector2i p0,utils::Vector2i p1,utils::Vector2i p2){
    utils::Vector2i temp;
        int dx;
        int sx;
        int dy;
        int sy;
        int err;
        int e2;
        utils::Vector2i p0t = p0;
        dx = abs(p0.x-p1.x);
        sx=p0.x<p1.x?1:-1;
        dy=-abs(p0.y-p1.y);
        sy=p0.y<p1.y?1:-1;
        err=dx+dy;
        while(true){
            SDL_RenderDrawPoint(renderer,p0.x,p0.y);
            if (p0.x == p1.x && p0.y == p1.y){
                break;
            }
            e2 = 2*err;
            if (e2 >= dy){
                err += dy;
                p0.x += sx;
            }
            if (e2 <= dx){
                err += dx;
                p0.y += sy;
            }
        }

        dx = abs(p1.x-p2.x);
        sx=p1.x<p2.x?1:-1;
        dy=-abs(p1.y-p2.y);
        sy=p1.y<p2.y?1:-1;
        err=dx+dy;
        while(true){
            SDL_RenderDrawPoint(renderer,p1.x,p1.y);
            if (p1.x == p2.x && p1.y == p2.y){
                break;
            }
            e2 = 2*err;
            if (e2 >= dy){
                err += dy;
                p1.x += sx;
            }
            if (e2 <= dx){
                err += dx;
                p1.y += sy;
            }
        }

        dx = abs(p2.x-p0t.x);
        sx=p2.x<p0t.x?1:-1;
        dy=-abs(p2.y-p0t.y);
        sy=p2.y<p0t.y?1:-1;
        err=dx+dy;
        while(true){
            SDL_RenderDrawPoint(renderer,p2.x,p2.y);
            if (p2.x == p0t.x && p2.y == p0t.y){
                break;
            }
            e2 = 2*err;
            if (e2 >= dy){
                err += dy;
                p2.x += sx;
            }
            if (e2 <= dx){
                err += dx;
                p2.y += sy;
            }
        }
}

void RenderWindow::drawCircle(utils::Vector2i p0,int r){
    utils::Vector2i temp(0,r);
    int d = 3 - (2 * r);
    drawCirclePoints(temp.x,temp.y,p0.x,p0.y);
    while(temp.y>=temp.x){
        temp.x++;
        if(d<0){
            d=d+(4*temp.x)+6;
        }
        else{
            d=d+4*(temp.x-temp.y)+10;
            temp.y--;
        }
        drawCirclePoints(temp.x,temp.y,p0.x,p0.y);
    }
}

void RenderWindow::fillCircle(utils::Vector2i p0,int r){
    utils::Vector2i temp(0,r);
    int d = 3 - (2 * r);
    connectCirclePoints(temp.x,temp.y,p0.x,p0.y);
    while(temp.y>=temp.x){
        temp.x++;
        if(d<0){
            d=d+(4*temp.x)+6;
        }
        else{
            d=d+4*(temp.x-temp.y)+10;
            temp.y--;
        }
        connectCirclePoints(temp.x,temp.y,p0.x,p0.y);
    }
}

void RenderWindow::drawCirclePoints(int x,int y,int ofsX,int ofsY){
        SDL_RenderDrawPoint(renderer,x+ofsX,y+ofsY);
        SDL_RenderDrawPoint(renderer,x+ofsX,-y+ofsY);
        SDL_RenderDrawPoint(renderer,-x+ofsX,y+ofsY);
        SDL_RenderDrawPoint(renderer,-x+ofsX,-y+ofsY);
        SDL_RenderDrawPoint(renderer,y+ofsX,x+ofsY);
        SDL_RenderDrawPoint(renderer,y+ofsX,-x+ofsY);
        SDL_RenderDrawPoint(renderer,-y+ofsX,x+ofsY);
        SDL_RenderDrawPoint(renderer,-y+ofsX,-x+ofsY);
}

void RenderWindow::connectCirclePoints(int x,int y,int ofsX,int ofsY){
        SDL_RenderDrawLine(renderer,x+ofsX,y+ofsY,-x+ofsX,y+ofsY);
        SDL_RenderDrawLine(renderer,x+ofsX,-y+ofsY,-x+ofsX,-y+ofsY);
        SDL_RenderDrawLine(renderer,y+ofsX,x+ofsY,-y+ofsX,x+ofsY);
        SDL_RenderDrawLine(renderer,y+ofsX,-x+ofsY,-y+ofsX,-x+ofsY);
}

void RenderWindow::drawBrain(Brain brain,utils::Vector2i pos,float scale){
    int maxLength = 0;
    
    for(int i=0;i<brain.weights.size()+1;i++){
        if(i==0){
            maxLength = brain.weights[0][0].size();
        }
        else if(brain.weights[i-1].size()>maxLength){
            maxLength = brain.weights[i-1].size();
        }
    }

    int lastOffset = scale*(maxLength-brain.weights[0][0].size())/2;
    int offset = 0;

    for(int i=0;i<brain.weights.size();i++){
        
        offset = scale*(maxLength-brain.weights[i].size())/2;

        for(int j=0;j<brain.weights[i].size();j++){
            for(int k=0;k<brain.weights[i][j].size();k++){
                if(brain.weights[i][j][k]>0){
                    setColor((int)(brain.weights[i][j][k]*155.0)+100,100,100,255);
                }
                else{
                    setColor(100,100,(int)(-brain.weights[i][j][k]*155.0)+100,255);
                }
                SDL_RenderDrawLine(renderer,i*scale+pos.x,j*scale+pos.y+offset,(i-1)*scale+pos.x,k*scale+pos.y+lastOffset);
            }
        }
        lastOffset = offset;
    }
}

void RenderWindow::drawObstacle(utils::Obstacle obs){
    SDL_Rect top;
    SDL_Rect bottom;
    
    top.y = 0;
    top.x = obs.pos.x;
    top.w = 25;
    top.h = obs.pos.y-100;

    bottom.y = obs.pos.y;
    bottom.x = obs.pos.x;
    bottom.w = 25;
    bottom.h = 1080-obs.pos.y;


    setColor(35,156,26,255);
    SDL_RenderFillRect(renderer,&top);
    SDL_RenderFillRect(renderer,&bottom);
}

void RenderWindow::drawBird(utils::Bird bird){
    setColor(bird.r,bird.g,bird.b,255);
    fillCircle(utils::Vector2i(900,bird.yPos),20);
}

void RenderWindow::writeNumber(std::vector<SDL_Texture*> tex, int number, utils::Vector2i pos){
    SDL_Rect rect;
    rect.h = 64;
    rect.w = 64;
    rect.x = pos.x;
    rect.y = pos.y;
    for(int i=0;i<10;i++){
        if(0<number){
            
            switch(number%10){
                case 0:
                    SDL_RenderCopy(renderer,tex[0],NULL,&rect);
                    break;
                case 1:
                    SDL_RenderCopy(renderer,tex[1],NULL,&rect);
                    break;
                case 2:
                    SDL_RenderCopy(renderer,tex[2],NULL,&rect);
                    break;
                case 3:
                    SDL_RenderCopy(renderer,tex[3],NULL,&rect);
                    break;
                case 4:
                    SDL_RenderCopy(renderer,tex[4],NULL,&rect);
                    break;
                case 5:
                    SDL_RenderCopy(renderer,tex[5],NULL,&rect);
                    break;
                case 6:
                    SDL_RenderCopy(renderer,tex[6],NULL,&rect);
                    break;
                case 7:
                    SDL_RenderCopy(renderer,tex[7],NULL,&rect);
                    break;
                case 8:
                    SDL_RenderCopy(renderer,tex[8],NULL,&rect);
                    break;
                case 9:
                    SDL_RenderCopy(renderer,tex[9],NULL,&rect);
                    break;
            }
            rect.x -=64;
            number /=10;
        }
        else{
            if(i==0){
                SDL_RenderCopy(renderer,tex[0],NULL,&rect);
            }
            break;
        }
    }
}

void RenderWindow::writeNumber(std::vector<SDL_Texture*> tex, float fNumber, utils::Vector2i pos){
    SDL_Rect rect;
    rect.h = 64;
    rect.w = 64;
    rect.x = pos.x;
    rect.y = pos.y;
    int number = fNumber*100;
    for(int i=0;i<10;i++){
        if(0<number){
            
            switch(number%10){
                case 0:
                    SDL_RenderCopy(renderer,tex[0],NULL,&rect);
                    break;
                case 1:
                    SDL_RenderCopy(renderer,tex[1],NULL,&rect);
                    break;
                case 2:
                    SDL_RenderCopy(renderer,tex[2],NULL,&rect);
                    break;
                case 3:
                    SDL_RenderCopy(renderer,tex[3],NULL,&rect);
                    break;
                case 4:
                    SDL_RenderCopy(renderer,tex[4],NULL,&rect);
                    break;
                case 5:
                    SDL_RenderCopy(renderer,tex[5],NULL,&rect);
                    break;
                case 6:
                    SDL_RenderCopy(renderer,tex[6],NULL,&rect);
                    break;
                case 7:
                    SDL_RenderCopy(renderer,tex[7],NULL,&rect);
                    break;
                case 8:
                    SDL_RenderCopy(renderer,tex[8],NULL,&rect);
                    break;
                case 9:
                    SDL_RenderCopy(renderer,tex[9],NULL,&rect);
                    break;
            }
            rect.x -=64;
            number /=10;
        }
        else{
            if(i==0){
                SDL_RenderCopy(renderer,tex[0],NULL,&rect);
            }
            break;
        }
    }
}