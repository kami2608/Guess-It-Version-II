#include <bits/stdc++.h>
#include <SDL2/SDL.h>
using namespace std;

//******** Main function **********
int SecretNumber();
int getHostAnswer(int number);
int SelectNumber(int low, int high);

//******************************
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 500;
const int MAX_NUM = 100;

SDL_Window *window;
SDL_Renderer *ren;
SDL_Texture *bg, *computer, *secret;
SDL_Texture *small, *big, *same;
SDL_Texture *num[MAX_NUM+1];

bool init();
SDL_Texture *loadTexture(string path);
void renderTexture(SDL_Texture *tex, int x, int y);
void renderTexture(SDL_Texture *tex, int x, int y, int w, int h);
void WaitUntilKeyPressed();
void close();

void loadMedia();
void showGame();
void renderSecretMsg();
void renderSecretNum(int secretNum);
void renderComputerNum(int number);
void renderComputer();
void renderMyComparision(SDL_Texture *tex);

//*********%%%%%%%%%%%%***********

int main(int argc, char *argv[]) {
    loadMedia();
    int x = SecretNumber();
    int low = 1, high = 100, num;
    int answer;
    do {
        num = SelectNumber(low, high);
        answer = getHostAnswer(num);
        if(answer == 1) high = num - 1; // answer is 
        else if(answer == 2) low = num + 1; // >
    } while(answer != 3);
    WaitUntilKeyPressed();
    close();
    return 0;
}

//*******************************
int SecretNumber() {
    int number = 0;
    bool quit = false;
    SDL_Event e;
    showGame();
    renderSecretMsg();
    while(!quit) {
        SDL_Delay(10);
        if(SDL_WaitEvent(&e) == 0) continue;
        if((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            close();
            exit(1);
        }
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) 
            quit = true;
        if(e.type == SDL_KEYDOWN) {
            switch(e.key.keysym.sym) {
                case SDLK_0:
                    number = number * 10;
                    break;
                case SDLK_1:
                    number = number * 10 + 1;
                    break;
                case SDLK_2:
                    number = number * 10 + 2;
                    break;
                case SDLK_3:
                    number = number * 10 + 3;
                    break;
                case SDLK_4:
                    number = number * 10 + 4;
                    break;
                case SDLK_5:
                    number = number * 10 + 5;
                    break;
                case SDLK_6:
                    number = number * 10 + 6;
                    break;
                case SDLK_7:
                    number = number * 10 + 7;
                    break;
                case SDLK_8:
                    number = number * 10 + 8;
                    break;
                case SDLK_9:
                    number = number * 10 + 9;
                    break;
                default :
                    break;
            }
        }
    }
    SDL_Delay(100);
    renderSecretNum(number);
    SDL_Delay(1000);
    showGame();
    return number;
}

int SelectNumber(int low,int high) {
    SDL_Delay(1000);
    renderComputerNum((low + high)/2);
    SDL_Delay(500);
    renderComputer();
    return (low + high)/2;
}

int getHostAnswer(int number) {
    int answer;
    SDL_Delay(1500);
    SDL_Event e;
    bool quit = false;
    while(!quit) {
        SDL_Delay(10);
        if(SDL_WaitEvent(&e) == 0) continue;
        if((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            close();
            exit(1);
        }
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) 
            quit = true;
        if(e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym)
            {
            case SDLK_1:
                answer = 1;
                break;
            case SDLK_2:
                answer = 2;
                break;
            case SDLK_3:
                answer = 3;
                break;
            default:
                break;
            }
        }
    }
    SDL_Texture *tex;
    if(answer == 1) tex = small;
    else if(answer == 2) tex = big;
    else tex = same;
    renderMyComparision(tex);
    SDL_Delay(1000);
    if(tex != same) showGame();
    return answer;
}

//*******************************
void loadMedia() {
    if(!init()) cout<<"Fail init"<<endl;
    else {
        bg = loadTexture("images/bg.bmp");
        computer = loadTexture("images/computer.bmp");
        secret = loadTexture("images/secret.bmp");
        small = loadTexture("images/small.bmp");
        big = loadTexture("images/big.bmp");
        same = loadTexture("images/same.bmp");
        bool check = true;
        for(int i=1; i<=MAX_NUM; i++) {
            num[i] = loadTexture("images/"+to_string(i)+".bmp");
            if(num[i] == NULL) {
                check = false;
                break;
            }
        }
        if(!check || bg == NULL || computer == NULL || secret == NULL
        || small == NULL || big == NULL || same == NULL) {
            cout<<"load media failed"<<endl;
            close();
        }
    }
}

void showGame() {
    SDL_RenderClear(ren);
    renderTexture(bg, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(ren);
}

void renderSecretMsg() {
    renderTexture(secret, 82, 128);
    SDL_RenderPresent(ren);
}

void renderSecretNum(int secretNum) {
    renderTexture(num[secretNum], 432, 344);
    SDL_RenderPresent(ren);
}

void renderComputerNum(int number) {
    renderTexture(num[number], 114, 320);
    SDL_RenderPresent(ren);
}

void renderComputer() {
    renderTexture(computer, 82, 128);
    SDL_RenderPresent(ren);
}

void renderMyComparision(SDL_Texture *tex) {
    renderTexture(tex, 420, 333);
    SDL_RenderPresent(ren);
}

//************** SDL function *************
bool init() {
    bool success = true;
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        success = false;
        cout<<"SDL could not be init"<<endl;
    }
    else {
        window = SDL_CreateWindow("Guess It 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == NULL) {
            cout<<"Window could not be created"<<endl;
            success = false;
        }
        else 
            ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }
    return success;
}

SDL_Texture *loadTexture(string path) {
    SDL_Texture *tex = NULL;
    SDL_Surface *loadedSurface = SDL_LoadBMP(path.c_str());
    if(loadedSurface == NULL) 
        cout<<"Could not load image surface"<<endl;
    else {
        tex = SDL_CreateTextureFromSurface(ren, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }
    return tex;
}

void WaitUntilKeyPressed() {
    SDL_Event e;
    while(true) {
        if(SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_QUIT)) {
            return;
            SDL_Delay(100);
        }
    }
}

void renderTexture(SDL_Texture *tex, int x, int y) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

void renderTexture(SDL_Texture *tex, int x, int y, int w, int h) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

void close() {
    SDL_DestroyTexture(bg);
    bg = NULL;
    SDL_DestroyTexture(computer);
    computer = NULL;
    SDL_DestroyTexture(secret);
    secret = NULL;
    SDL_DestroyTexture(small);
    small = NULL;
    SDL_DestroyTexture(big);
    big = NULL;
    SDL_DestroyTexture(same);
    same = NULL;
    for(int i=1; i<= MAX_NUM; i++) {
        SDL_DestroyTexture(num[i]);
        num[i] = NULL;
    }
    SDL_DestroyRenderer(ren);
    ren = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
}