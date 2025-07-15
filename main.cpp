#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "romaji_converter.hpp"
#include "card_logic.hpp"
#include <SDL2_gfxPrimitives.h>

//getting rid of that annoying std
using std::cout;
using std::string;
using std::endl;
using std::vector;
using std::ifstream;
using std::getline;


bool isInList(const std::string& needle, const std::string& haystack) {
    std::istringstream ss(haystack);
    std::string item;
    while (std::getline(ss, item, ',')) {
        if (item == needle) return true;
    }
    return false;
}

void addTexttoLines(const std::string & saved_text,
    std::vector < std::string > & lines,
    std::vector < SDL_Texture * > & lines_texture,
    std::vector < int > & lines_height,
    std::vector < int > & lines_width,
    SDL_Renderer * ren,
    TTF_Font * font,
    SDL_Color text_color,
    int & w,
    int & h,
    bool draw_background = false,
    SDL_Color background_color = { 0, 0, 0, 128 },
    int corner_radius = 8)
{
    lines.push_back(saved_text);

    // Render text to surface
    SDL_Surface * surface = TTF_RenderUTF8_Blended(font, saved_text.c_str(), text_color);
    TTF_SizeUTF8(font, saved_text.c_str(), &w, &h);

    SDL_Surface * final_surface = SDL_CreateRGBSurfaceWithFormat(0, w + 16, h + 8, 32, SDL_PIXELFORMAT_RGBA32);

    if (draw_background)
    {
        // Draw rounded box on final_surface
        SDL_Renderer * temp_renderer = SDL_CreateSoftwareRenderer(final_surface);
        roundedBoxRGBA(temp_renderer, 0, 0, w + 16, h + 8,
                       corner_radius,
                       background_color.r, background_color.g, background_color.b, background_color.a);
        SDL_DestroyRenderer(temp_renderer);
    }

    // Blit text surface onto final_surface
    SDL_Rect dst = {8, 4, w, h};
    SDL_BlitSurface(surface, NULL, final_surface, &dst);
    SDL_FreeSurface(surface);

    // Convert to texture
    SDL_Texture * texture = SDL_CreateTextureFromSurface(ren, final_surface);
    SDL_FreeSurface(final_surface);

    lines_texture.push_back(texture);
    lines_height.push_back(h + 8);
    lines_width.push_back(w + 16);
}

int last_line = 0;

int main(int argc, char ** argv) {

    // just for fun
    cout << "Hello" << endl;

    //Set colors

    SDL_Color white = {
        255,
        255,
        255
    }; // white
    SDL_Color red = {
        250,
        27,
        109
    };
    SDL_Color green = {
        18,
        201,
        61
    };
    SDL_Color light_blue = {
        100,
        186,
        189
    };
    SDL_Color blue = {
        66,
        135,
        245
    };
    //int caret_x , caret_y;
    SDL_Rect dstRect;

    int scroll = 0;
    //int current_scroll = -210;
    string myText = "$ ";
    //size_t i;
    // win(window) e(event) ren(renderer)

    //Initialize SDL
    IMG_Init(IMG_INIT_PNG);
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();



    //Set the window and the renderer
    SDL_Window * win = SDL_CreateWindow("Text Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, SDL_WINDOW_RESIZABLE);
    SDL_Renderer * ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    int width, height;
    SDL_GetWindowSize(win, & width, & height);

    //Setbackground

    SDL_Surface* backgroundSurface = IMG_Load("background.png");
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(ren, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);  


    //Initialize font
    TTF_Font * font = TTF_OpenFont("fonts/SHIPPORIMINCHO-REGULAR.TTF", 100);
    TTF_Font * font2 = TTF_OpenFont("C:/Windows/Fonts/CALIBRI.TTF", 30);
    TTF_Font * font3 = TTF_OpenFont("C:/Windows/Fonts/MSGOTHIC.TTC", 24);
    if (!font) {
        cout << "Failed to load font: " << TTF_GetError() << endl;
        return 1;
    }

    //Initialize file lines preload texture information
    vector < string > lines;
    vector < SDL_Texture * > lines_texture;
    vector < int > lines_height;
    vector < int > lines_width;
    int w, h;

    //Initialize Cards
    int index;
    Cards currentCard;
    std::vector < Cards > allCards;
    allCards = load_cards("card_data.tsv");
    std::tie(index, currentCard) = getCurrentCardAndIndex(allCards);

    addTexttoLines(currentCard.question(), lines, lines_texture, lines_height, lines_width, ren, font, blue, w, h, true, {45, 40, 50, 255}, 12);
    bool change = true;

    //Game loop that runs every frame!!
    SDL_Event e;
    bool quit = false;
    while (!quit) {

        while (SDL_PollEvent( & e)) {
            switch (e.type) {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_TEXTINPUT:
                myText += e.text.text;
                //scroll = current_scroll; 
                change = true;
                break;
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    change = true;
                    SDL_GetWindowSize(win, & width, & height);
                }
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_RETURN: { //To handle the ENTER key
                    addTexttoLines(romaji_to_hiragana(myText.substr(2)), lines, lines_texture, lines_height, lines_width, ren, font3, white, w, h);
                    
                    if (isInList(romaji_to_hiragana(myText.substr(2)), currentCard.answer())) {
                        addTexttoLines("correct", lines, lines_texture, lines_height, lines_width, ren, font2, green, w, h);
                        addTexttoLines(currentCard.answer(), lines, lines_texture, lines_height, lines_width, ren, font3, red, w, h);
                        addTexttoLines(currentCard.description(), lines, lines_texture, lines_height, lines_width, ren, font2, light_blue, w, h);
                        replaceCardsArray(allCards, index, true);
                    } else {
                        addTexttoLines(currentCard.answer(), lines, lines_texture, lines_height, lines_width, ren, font3, red, w, h);
                        addTexttoLines(currentCard.description(), lines, lines_texture, lines_height, lines_width, ren, font2, light_blue, w, h);
                        replaceCardsArray(allCards, index, false);
                    }

                    std::tie(index, currentCard) = getCurrentCardAndIndex(allCards);
                    if (index == -1) {
                        cout << "No cards the review, closing in 3 seconds!!" << endl;
                        addTexttoLines("No cards the review, closing in 3 seconds!!", lines, lines_texture, lines_height, lines_width, ren, font, red, w, h);
                        change = true;
                        quit = true;

                    } else {
                        addTexttoLines(currentCard.question(), lines, lines_texture, lines_height, lines_width, ren, font, blue, w, h, true, {45, 40, 50, 255}, 12);
                    }

                    int total_text_height = 0;
                    for (int h: lines_height)
                        total_text_height += h;

                    scroll = height - total_text_height - 100;
                    change = true;
                    myText = "$ ";
                    // current_scroll -= caret_y;
                    //scroll = current_scroll;         
                    break;
                }
                case SDLK_BACKSPACE: {
                    if (myText != "$ ") {
                        myText.pop_back();
                        //scroll = current_scroll; 
                        change = true;
                    }
                    break;
                }

                case SDL_SCANCODE_F11 :{
                    Uint32 flags = SDL_GetWindowFlags(win);
                    if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) {
                        SDL_SetWindowFullscreen(win, 0);
                    } else {
                        SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    }
                }
                }
                break;
            case SDL_MOUSEWHEEL: // To handle scrolling
                change = true;
                if (e.wheel.y > 0) {
                    scroll += 30;
                } else {
                    scroll -= 30;

                }
                break;
            }
        }

        if (change) {
            //Clear everything
            SDL_RenderClear(ren);
            SDL_RenderCopy(ren, backgroundTexture, NULL, NULL);
            SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(ren, 66, 73, 82, 150);
            SDL_Rect overlay = {0, 0, width, height};
            SDL_RenderFillRect(ren, &overlay);

            int Total_line_height = 0;

            size_t n = lines.size();
            if (lines_texture.size() < n) n = lines_texture.size();
            if (lines_width.size() < n) n = lines_width.size();
            if (lines_height.size() < n) n = lines_height.size();

            for (size_t i = 0; i < n; i++) {

                if ((0 < Total_line_height + scroll + lines_height[i]) && (Total_line_height + scroll - lines_height[i] < height - 120)) {
                    dstRect = {
                        10,
                        30 + Total_line_height + scroll,
                        lines_width[i],
                        lines_height[i]
                    };
                    SDL_RenderCopy(ren, lines_texture[i], NULL, & dstRect);
                }
                Total_line_height += lines_height[i];
            }
            SDL_SetRenderDrawColor(ren, 31, 34, 38, 255);

            SDL_Rect typing_bar = {
                0,
                height - 50,
                width,
                50
            };
            SDL_RenderFillRect(ren, & typing_bar);

            SDL_Rect top_bar = {
                0,
                0,
                width,
                40
            };
            SDL_RenderFillRect(ren, & top_bar);

            SDL_Surface * typing_surface = TTF_RenderUTF8_Blended(font2, myText.c_str(), white);
            SDL_Texture * typing_texture = SDL_CreateTextureFromSurface(ren, typing_surface);
            SDL_FreeSurface(typing_surface);
            TTF_SizeUTF8(font2, myText.c_str(), & w, & h);

            dstRect = {
                5,
                height - 25 - h / 2,
                w,
                h
            };
            SDL_RenderCopy(ren, typing_texture, NULL, & dstRect);
            SDL_DestroyTexture(typing_texture);

            SDL_RenderPresent(ren);
            change = false;

        }
    } //end of the frame loop
    if (index == -1) {
        SDL_Delay(3000);
    }

    saveCards("card_data.tsv", allCards);
    // To free up memory
    for (SDL_Texture * tex: lines_texture) {
        SDL_DestroyTexture(tex);
    }

    //SDL_DestroyTexture(texture);
SDL_DestroyTexture(backgroundTexture);




    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    cout << "loop quit success" << endl;

    return 0;
}