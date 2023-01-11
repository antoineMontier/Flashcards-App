#ifndef SDL_SCREEN_HPP
#define SDL_SCREEN_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <vector>
#include <sstream>


class SDL_Screen{
    
    private:
        int _width;
        int _height;
        long _ms;
        SDL_Window *w;
        SDL_Renderer *r;
        SDL_Event e;
        const char * title;
        double _fps;
        double fps_max = 180;
        static bool instanceExist;
        bool program_running;

        void SDL_ExitWithError(const char *string);

        double distance(double x1, double y1, double x2, double y2);

    public:

        /// @brief creates a screen of 600*480 entitled "SDL_Screen" with 30 fps and Open SDL (initialisation)
        SDL_Screen();
        /// @brief creates a screen entitled "SDL_Screen" with 30 fps
        /// @param window_width width of the screen
        /// @param window_height height of the screen
        SDL_Screen(double window_width, double window_height);
        /// @brief creates a screen entitled "SDL_Screen" and Open SDL (initialisation)
        /// @param window_width width of the screen
        /// @param window_height height of the screen
        /// @param fps frames per second
        SDL_Screen(double window_width, double window_height, double fps);
        /// @brief creates a screen of 600*480 with 30 fps and Open SDL (initialisation)
        /// @param window_title title of the new screen
        SDL_Screen(const char* window_title);
        /// @brief creates a screen with 30 fps
        /// @param window_width width of the screen
        /// @param window_height height of the screen
        /// @param window_title title of the screen
        SDL_Screen(double window_width, double window_height, const char* window_title);
        /// @brief creates a screen and Open SDL (initialisation)
        /// @param window_width width of the screen
        /// @param window_height height of the screen
        /// @param window_title title of the screen
        /// @param fps frames per second
        SDL_Screen(double window_width, double window_height, const char* window_title, double fps);

        /// @brief Quit SDL, prints if it's a success or not in the cout
        ~SDL_Screen();

        bool OpenSDL();

        int W();
        int H();
        double getFPS();

        /// @brief refresh the screen with the new shapes
        /// @return true if successful
        bool refresh();
        /// @brief refresh the screen with the new shapes and adds to the cout calculations ticks, freezed ticks and total ticks
        /// @return true if successful
        bool refreshAndDetails();
        /// @brief refresh the screen with the new shapes and asks for events
        /// @return true if successful
        bool refreshAndEvents();
        /// @brief refresh the screen with the new shapes, asks for events and adds to the cout calculations ticks, freezed ticks and total ticks
        /// @return true if successful
        bool refreshAndDetailsAndEvents();

        bool freeze(unsigned int ms);

        bool CloseSDL();

        void point(double x, double y);
        void point(double x, double y, double thickness);

        void setColor(int red, int green, int blue, int alpha);
        void setColor(int red, int green, int blue);
        void setColor(int grey);

        void bg();
        void bg(int grey);
        void bg(int red, int green, int blue);

        void updateSize();

        void line(int x1, int y1, int x2, int y2);

        /// @brief creates an empty square
        /// @param x the upper left x-coordinate
        /// @param y the upper left y-coordinate
        /// @param size the widht and height of the square
        void emptyRect(int x, int y, int size);//optimized

        /// @brief creates an empty rectangle
        /// @param x the upper left x-coordinate
        /// @param y the upper left y-coordinate
        /// @param width the width of the rectangle
        /// @param height the height of the rectangle
        void emptyRect(int x, int y, int width, int height);//optimized
        void emptyRect(int x, int y, int width, int height, int rounding);
        //void emptyRect(int x, int y, int width, int height, int rounding_top_left, int rounding_top_right, int rounding_bottom_right, int rounding_bottom_left);//todo
        
        /// @brief creates a filled square
        /// @param x the upper left x-coordinate
        /// @param y the upper left y-coordinate
        /// @param size the witdh and height of the square
        void filledRect(int x, int y, int size);//optimized

        /// @brief creates a filled rectangle
        /// @param x the upper left x-coordinate
        /// @param y the upper left y-coordinate
        /// @param width the width of the rectangle
        /// @param height the height of the rectangle
        void filledRect(int x, int y, int width, int height);//optimized
        void filledRect(int x, int y, int width, int height, int rounding);//optimized
        //void filledRect(int x, int y, int width, int height, int rounding_top_left, int rounding_top_right, int rounding_bottom_right, int rounding_bottom_left);//todo

        void filledCircle(int x, int y, int radius);//optimized
        void filledCircle(int x, int y, int width, int height);//optimized

        void emptyCircle(int x, int y, int radius);//optimized
        void emptyCircle(int x, int y, int width, int height);//optimized
        //void emptyCircle(int x, int y, int width, int height, double thickness);

        void filledTriangleP(int x1, int y1, int x2, int y2, int x3, int y3);//more precise version
        void filledTriangle(int x1, int y1, int x2, int y2, int x3, int y3);

        void emptyTriangle(int x1, int y1, int x2, int y2, int x3, int y3);

        void filledPolygon(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

        void emptyPolygon(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

        bool setFPS(double fps);

        bool isRunning();

        void stopRunning();

        void events();

        static bool rollover(int mx, int my, int x, int y, int w, int h);

        bool setFont(TTF_Font **font, const char * font_file, int size);


        bool text(int x, int y, const char * text, TTF_Font *font);
        bool text(int x, int y, const char * text, TTF_Font *font, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha=255);

        void paragraph(int x, int y, const char* text, TTF_Font* font, 
                    unsigned char red, unsigned char green=255, unsigned char blue=255, unsigned char alpha=255);

        void paragraph(int x, int y, const char* text, TTF_Font* font);
};

#endif