#include "SDL_Screen.hpp"

using namespace std;

bool SDL_Screen::instanceExist = false; // no more than one instance

SDL_Screen::SDL_Screen(){
    if(instanceExist)
        throw new std::runtime_error("Multiples instances of SDL_Screen are not allowed");
    instanceExist = true;
    _width = 600;
    _height = 480;
    title = "SDL_Screen";
    _fps = 30.0;
    _ms = 0;
    program_running = true;

    OpenSDL();
}

SDL_Screen::SDL_Screen(double window_width, double window_height){
    if(instanceExist)
        throw new std::runtime_error("Multiples instances of SDL_Screen are not allowed");
    instanceExist = true;
    _width = window_width;
    _height = window_height;
    title = "SDL_Screen";
    _fps = 30.0;
    _ms = 0;
    program_running = true;

    OpenSDL();
}

SDL_Screen::SDL_Screen(const char* window_title){
    if(instanceExist)
        throw new std::runtime_error("Multiples instances of SDL_Screen are not allowed");
    instanceExist = true;
    _width = 600;
    _height = 480;
    title = window_title;
    _fps = 30.0;
    _ms = 0;
    program_running = true;

    OpenSDL();
}

SDL_Screen::SDL_Screen(double window_width, double window_height, const char* window_title){
    if(instanceExist)
        throw new std::runtime_error("Multiples instances of SDL_Screen are not allowed");
    instanceExist = true;
    _width = window_width;
    _height = window_height;
    title = window_title;
    _fps = 30.0;
    _ms = 0;
    program_running = true;

    OpenSDL();
}

SDL_Screen::SDL_Screen(double window_width, double window_height, double fps){
    if(instanceExist)
        throw new std::runtime_error("Multiples instances of SDL_Screen are not allowed");
    instanceExist = true;
    if(fps <= 0)
        fps = 1;
    else if(fps >= fps_max)
        fps = fps_max - 1;
    _width = window_width;
    _height = window_height;
    title = "SDL_Screen";
    _fps = fps;
    _ms = 0;
    program_running = true;

    OpenSDL();
}

SDL_Screen::SDL_Screen(double window_width, double window_height, const char* window_title, double fps){
    if(instanceExist)
        throw new std::runtime_error("Multiples instances of SDL_Screen are not allowed");
    instanceExist = true;
    if(fps <= 0)
        fps = 1;
    else if(fps >= fps_max)
        fps = fps_max - 1;
    _width = window_width;
    _height = window_height;
    title = window_title;
    _fps = fps;
    _ms = 0;
    program_running = true;

    OpenSDL();
}

SDL_Screen::~SDL_Screen(){
    if(!CloseSDL())
        std::cout << "Failed to close SDL" << std::endl;
    else
        std::cout << "Closed SDL successfully" << std::endl;
}

double SDL_Screen::getFPS(){return _fps;}

bool SDL_Screen::OpenSDL(){

    if (0 != SDL_Init(/*flag*/ SDL_INIT_VIDEO | SDL_INIT_EVENTS)){ // lots of flags like SDL_INIT_AUDIO ; *_VIDEO ; *_EVEhTHING... To separe with '|'
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Initialization SDL failed", NULL);
        exit(1);
        return false; // error
    }

    /*SDL_DisplayMode displayMode;
    if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0)//know actual screen size
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Initialization SDL failed", NULL);*/

    w = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_SetWindowResizable(w, SDL_TRUE);

    r = SDL_CreateRenderer(w, -1, SDL_RENDERER_TARGETTEXTURE);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);//make alpha transparent

    if (TTF_Init() != 0){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Initialization SDL failed", NULL);
        exit(1);
        return false; // error
    } 
    return true;//sdl opening is succesful
}

bool SDL_Screen::CloseSDL(){
    TTF_Quit();
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    SDL_Quit();
    return 1;
}

bool SDL_Screen::refresh(){
    if(SDL_GetTicks() - _ms < 1000.0/_fps)
        SDL_Delay(1000.0/_fps - (SDL_GetTicks() - _ms));//add ticks to get a the desired fps
    SDL_RenderPresent(r);//display
    _ms = SDL_GetTicks();//get the ticks for another turn
    return true;
}

bool SDL_Screen::refreshAndDetails(){
    std::cout << "calc ticks : " << (SDL_GetTicks() - _ms);
    long freezed = SDL_GetTicks();
    if(SDL_GetTicks() - _ms < 1000.0/_fps)
        SDL_Delay(1000.0/_fps - (SDL_GetTicks() - _ms));//add ticks to get a the desired fps
    std::cout << "\tfreeze ticks : " << (SDL_GetTicks() - freezed);
    SDL_RenderPresent(r);//display
    std::cout << "\ttotal : " << (SDL_GetTicks() - _ms) << std::endl; 
    _ms = SDL_GetTicks();//get the ticks for another turn
    return true;
}

bool SDL_Screen::refreshAndEvents(){
    events();
    if(SDL_GetTicks() - _ms < 1000.0/_fps)
        SDL_Delay(1000.0/_fps - (SDL_GetTicks() - _ms));//add ticks to get a the desired fps
    SDL_RenderPresent(r);//display
    _ms = SDL_GetTicks();//get the ticks for another turn
    return true;
}

bool SDL_Screen::refreshAndDetailsAndEvents(){
    events();
    std::cout << "calc ticks : " << (SDL_GetTicks() - _ms);
    long freezed = SDL_GetTicks();
    if(freezed - _ms < 1000.0/_fps)
        SDL_Delay(1000.0/_fps - (freezed - _ms));//add ticks to get a the desired fps
    std::cout << "  \tfreeze ticks : " << (SDL_GetTicks() - freezed);
    SDL_RenderPresent(r);//display
    std::cout << "\ttotal : " << (SDL_GetTicks() - _ms) << std::endl; 
    _ms = SDL_GetTicks();//get the ticks for another turn
    return true;
}

bool SDL_Screen::freeze(unsigned int ms){
    SDL_Delay(ms);
    return true;
}

void SDL_Screen::SDL_ExitWithError(const char *string)
{
    SDL_Log("Error : %s > %s\n", string, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

double SDL_Screen::distance(double x1, double y1, double x2, double y2){return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));}

void SDL_Screen::point(double x, double y){
    if (SDL_RenderDrawPoint(r, x, y) != 0)
        SDL_ExitWithError("failed to draw point");
}

void SDL_Screen::point(double x, double y, double thickness){
    filledRect(x - thickness/2, y - thickness/2, thickness);    
}

void SDL_Screen::setColor(int red, int green, int blue, int alpha){
    if (SDL_SetRenderDrawColor(r, red, green, blue, alpha) != 0)
        SDL_ExitWithError("failed to set color");
}

void SDL_Screen::setColor(int red, int green, int blue){
    if (SDL_SetRenderDrawColor(r, red, green, blue, 255) != 0)
        SDL_ExitWithError("failed to set color");
}

void SDL_Screen::setColor(int grey){
    if (SDL_SetRenderDrawColor(r, grey, grey, grey, 255) != 0)
        SDL_ExitWithError("failed to set color");
}

void SDL_Screen::bg(){
    SDL_RenderClear(r);
}

void SDL_Screen::bg(int grey){
    //save the renderer color
    Uint8 cr, cg, cb, ca;
    if(SDL_GetRenderDrawColor(r, &cr, &cg, &cb, &ca) != 0)
        SDL_ExitWithError("failed to save color");
    setColor(grey);
    SDL_RenderClear(r);
    //reset the color as before
    setColor(cr, cg, cb, ca);
}

void SDL_Screen::bg(int red, int green, int blue){
    //save the renderer color
    Uint8 cr, cg, cb, ca;
    if(SDL_GetRenderDrawColor(r, &cr, &cg, &cb, &ca) != 0)
        SDL_ExitWithError("failed to save color");
    setColor(red, green, blue);
    SDL_RenderClear(r);
    //reset the color as before
    setColor(cr, cg, cb, ca);
}

void SDL_Screen::updateSize(){
    int _wi, _he;
    SDL_GetWindowSize(w, &_wi, &_he);
    _width = _wi;
    _height = _he;
}

void SDL_Screen::line(int x1, int y1, int x2, int y2){
    if(SDL_RenderDrawLine(r, x1, y1, x2, y2) != 0)
        SDL_ExitWithError("failed to draw a line");
}

void SDL_Screen::filledRect(int x, int y, int size){
    SDL_Rect rect = {x, y, size, size};
    SDL_RenderFillRect(r, &rect);
}

void SDL_Screen::filledRect(int x, int y, int width, int height){
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(r, &rect);
}

void SDL_Screen::filledCircle(int x, int y, int radius){
    int x0 = radius;
    int y0 = 0;
    int err = 0;

    while (x0 >= y0) {
        SDL_RenderDrawLine(r, x - x0, y + y0, x + x0, y + y0);
        SDL_RenderDrawLine(r, x - y0, y + x0, x + y0, y + x0);
        SDL_RenderDrawLine(r, x - x0, y - y0, x + x0, y - y0);
        SDL_RenderDrawLine(r, x - y0, y - x0, x + y0, y - x0);

        if (err <= 0) {
            y0 += 1;
            err += 2*y0 + 1;
        }
        if (err > 0) {
            x0 -= 1;
            err -= 2*x0 + 1;
        }
    }
}

void SDL_Screen::emptyCircle(int x, int y, int radius){
    int x0 = radius;
    int y0 = 0;
    int decision = 3 - 2 * radius;

    while (y0 <= x0) {
        SDL_RenderDrawPoint(r, x0 + x, y0 + y);
        SDL_RenderDrawPoint(r, y0 + x, x0 + y);
        SDL_RenderDrawPoint(r, -x0 + x, y0 + y);
        SDL_RenderDrawPoint(r, -y0 + x, x0 + y);
        SDL_RenderDrawPoint(r, -x0 + x, -y0 + y);
        SDL_RenderDrawPoint(r, -y0 + x, -x0 + y);
        SDL_RenderDrawPoint(r, x0 + x, -y0 + y);
        SDL_RenderDrawPoint(r, y0 + x, -x0 + y);
        y0++;

        if (decision > 0) {
            x0--;
            decision += 4 * (y0 - x0) + 10;
        } else {
            decision += 4 * y0 + 6;
        }
    }
}

void SDL_Screen::filledCircle(int x, int y, int width, int height){
    int xc = 0;
    int yc = height;
    long a2 = width * width;
    long b2 = height * height;
    long t = -a2 * yc;
    long dxt = 2*b2 * xc;
    long dyt = -2*a2 *yc;

    while (yc >= 0 && xc <= width)
    {
        SDL_RenderDrawLine(r, x - xc, y - yc, x + xc, y - yc);
        SDL_RenderDrawLine(r, x - xc, y + yc, x + xc, y + yc);
        
        if (t + b2 * xc <= -(a2/4+width % 2 + b2) || t + a2 * yc <= -(b2/4+height % 2)){
            xc++;
            dxt+= 2*b2;
            t += dxt;
        }
        else if (t - a2 * yc > -(b2/4+height % 2 + a2)){
            yc--;
            dyt += 2*a2;
            t += dyt;
        }
        else{
            xc++;
            dxt += 2*b2;
            t += dxt;
            yc--;
            dyt += 2*a2;
            t += dyt;
        }
    }
}

void SDL_Screen::emptyCircle(int x, int y, int width, int height){
    int xc = 0;
    int yc = height;
    long a2 = width * width;
    long b2 = height * height;
    long t = -a2 * yc;
    long dxt = 2*b2 * xc;
    long dyt = -2*a2 *yc;

    while (yc >= 0 && xc <= width){

        SDL_RenderDrawPoint(r, x + xc, y + yc);
        SDL_RenderDrawPoint(r, x - xc, y + yc);
        SDL_RenderDrawPoint(r, x + xc, y - yc);
        SDL_RenderDrawPoint(r, x - xc, y - yc);

        if (t + b2 * xc <= -(a2/4+width % 2 + b2) || t + a2 * yc <= -(b2/4+height % 2)){
            xc++;
            dxt+= 2*b2;
            t += dxt;
        }
        else if (t - a2 * yc > -(b2/4+height % 2 + a2)){
            yc--;
            dyt += 2*a2;
            t += dyt;
        }
        else{
            xc++;
            dxt += 2*b2;
            t += dxt;
            yc--;
            dyt += 2*a2;
            t += dyt;
        }
    }
}

int SDL_Screen::W(){return _width;}

int SDL_Screen::H(){return _height;}

void SDL_Screen::filledTriangleP(int x1, int y1, int x2, int y2, int x3, int y3){
    //sort by y-coordinate
    if (y1 > y2){std::swap(y1, y2); std::swap(x1, x2);}
    if (y1 > y3){std::swap(y1, y3); std::swap(x1, x3);}
    if (y2 > y3){std::swap(y2, y3); std::swap(x2, x3);}

    // calculate the edges of the triangle
    double dx_far = static_cast<double>(x3-x1)/(y3 - y1);
    double dx_top = static_cast<double>(x2-x1)/(y2 - y1);
    double dx_bot = static_cast<double>(x3-x2)/(y3 - y2);

    double xf = x1;
    double xt = x1;

    // iterate through each scanline of the triangle
    for (int y = y1; y <= y2; y++) {
        int minx =static_cast<int>(ceil( min(xf, xt)));
        int maxx = static_cast<int>( floor(max(xf, xt)));

        for (int x = minx; x <= maxx; x++)
            SDL_RenderDrawPoint(r, x, y);
        xf += dx_far;
        xt += dx_top;
    }
    xf = x2;
    xt = x3;

    // bottom half of the triangle
    for (int y = y2; y <= y3; y++) {
        int minx = static_cast<int>(ceil(min(xf, xt) ));
        int maxx = static_cast<int>(floor(max(xf, xt)));

        for (int x = minx; x <= maxx; x++) 
            SDL_RenderDrawPoint(r, x, y);
        xf += dx_bot;
        xt += dx_far;
    }
}

void SDL_Screen::filledTriangle(int x1, int y1, int x2, int y2, int x3, int y3){
    // sort
    if (y1 > y2){ std::swap(y1, y2); std::swap(x1, x2);}
    if (y1 > y3){ std::swap(y1, y3); std::swap(x1, x3);}
    if (y2 > y3){ std::swap(y2, y3); std::swap(x2, x3);}

    // edges of the triangle
    int dx1 = x2 -x1, dy1 = y2-y1;
    int dx2 = x3 -x1, dy2 = y3-y1;
    int dx3 = x3 -x2, dy3 = y3-y2;

    int start, end;
    for(int y = y1; y <= y3; y++) {

        if(y <= y2){
            start = x1 + (y - y1)*dx1 / dy1;
            end = x1 + (y - y1)*dx2 / dy2;
        }
        else{
            start = x2 + (y - y2) * dx3 / dy3;
            end = x1 + (y - y1) * dx2 / dy2;
        }

        if(start > end){ std::swap(start, end); }

        for(int x = start; x <= end; x++)
            SDL_RenderDrawPoint(r, x, y);
    }
}

void SDL_Screen::emptyTriangle(int x1, int y1, int x2, int y2, int x3, int y3){
    SDL_RenderDrawLine(r, x1, y1, x2, y2);
    SDL_RenderDrawLine(r, x3, y3, x2, y2);
    SDL_RenderDrawLine(r, x1, y1, x3, y3);
}

void SDL_Screen::filledPolygon(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
    filledTriangle(x1, y1, x2, y2, x3, y3);
    filledTriangle(x2, y2, x3, y3, x4, y4);
    filledTriangle(x3, y3, x4, y4, x1, y1);
    filledTriangle(x4, y4, x1, y1, x2, y2);
}

void SDL_Screen::emptyPolygon(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
    SDL_RenderDrawLine(r, x1, y1, x2, y2);
    SDL_RenderDrawLine(r, x2, y2, x3, y3);
    SDL_RenderDrawLine(r, x3, y3, x4, y4);
    SDL_RenderDrawLine(r, x4, y4, x1, y1);
}

void SDL_Screen::emptyRect(int x, int y, int size){
    SDL_Rect rect = {x, y, size, size};
    SDL_RenderDrawRect(r, &rect);
}

void SDL_Screen::emptyRect(int x, int y, int width, int height){
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderDrawRect(r, &rect);
}

void SDL_Screen::filledRect(int x, int y, int width, int height, int rounding){
    //first let's fix the rounding if it's bellow 0 or greater than the half of the smallest side of the rectangle
    if(rounding <= 0){
        filledRect(x, y, width, height);
        return;
    }
    if(rounding > fmin(width, height)/2.0)
        rounding = fmin(width, height)/2.0;
    //let's draw the core rectangles
    filledRect(x, y + rounding, width, height-2*rounding);
    filledRect(x + rounding, y, width-2*rounding, height);

    //draw the 4 corners

    filledCircle(x +  rounding, y + rounding, rounding);//top left
    filledCircle(x + width - rounding, y + rounding, rounding);//top right
    filledCircle(x + rounding, y + height - rounding, rounding);//bottom left
    filledCircle(x + width - rounding, y + height - rounding, rounding);//bottom right
}

void SDL_Screen::emptyRect(int x, int y, int width, int height, int rounding){
    //first let's fix the rounding if it's bellow 0 or greater than the half of the smallest side of the rectangle
    if(rounding <= 0){
        emptyRect(x, y, width, height);
        return;
    }
    if(rounding > fmin(width, height)/2.0)
        rounding = fmin(width, height)/2.0;
    //draw the lines without corners
    double thick = 0.6;
    SDL_RenderDrawLine(r, x + rounding - 1, y, x + width - rounding + 1, y);//top
    SDL_RenderDrawLine(r, x + rounding - 1, y + height, x + width - rounding + 1, y + height);//bottom
    SDL_RenderDrawLine(r, x, y + rounding - 1, x, y + height - rounding + 1);//left
    SDL_RenderDrawLine(r, x + width, y + rounding - 1, x + width, y + height - rounding + 1);//right
    //now draw the four corners

    //==== top left ====
    for(int i = x - thick ; i < x + rounding + thick; i++)
        for(int j = y - thick; j < y + rounding + thick; j++)
            if(distance(x + rounding, y + rounding, i, j) <= rounding + thick && distance(x + rounding, y + rounding, i, j) >= rounding - thick)
                SDL_RenderDrawPoint(r, i, j);
    
    //==== top right ====
    for(int i = x + width - rounding - thick; i < x + width + thick; i++)
        for(int j = y - thick ; j < y + rounding + thick ; j++)
            if(distance(x + width - rounding, y + rounding, i, j) >= rounding - thick && distance(x + width - rounding, y + rounding, i, j) <= rounding + thick) 
                SDL_RenderDrawPoint(r, i, j);

    //==== bottom left ====
    for(int i = x - thick; i < x + rounding + thick; i++)
        for(int j = y + height - rounding - thick; j < y + height + thick; j++)
            if(distance(x + rounding, y + height - rounding, i, j) >= rounding - thick && distance(x + rounding, y + height - rounding, i, j) <= rounding + thick) 
                SDL_RenderDrawPoint(r, i, j);

    //==== bottom right ====
    for(int i = x + width - rounding - thick; i < x + width + thick; i++)
        for(int j = y + height - rounding - thick; j < y + height + thick; j++)
            if(distance(x + width - rounding, y + height - rounding, i, j) <= rounding + thick && distance(x + width - rounding, y + height - rounding, i, j) >= rounding - thick)
                SDL_RenderDrawPoint(r, i, j);
}

bool SDL_Screen::setFPS(double fps){
    if(fps >= fps_max || fps <= 0)
        return false;
    _fps = fps;
    return true;
}

void SDL_Screen::events(){
    while (SDL_PollEvent(&e))
        { // reads all the events (mouse moving, key pressed...)        //possible to wait for an event with SDL_WaitEvent
            switch (e.type)
            {
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                    updateSize();
                break;
            case SDL_QUIT:
                program_running = false; // quit the program if the user closes the window
                break;

            case SDL_KEYDOWN: // SDL_KEYDOWN : hold a key            SDL_KEYUP : release a key
                switch (e.key.keysym.sym){ // returns the key ('0' ; 'e' ; 'SPACE'...)

                case SDLK_ESCAPE:
                    program_running = false; // escape the program if user presses esc
                    break;

                default:
                    break;
                }

            case SDL_KEYUP:
                switch (e.key.keysym.sym){

                    default:
                        break;
                }

            default:
                break;
            }
        }
}

bool SDL_Screen::isRunning(){return program_running;}

void SDL_Screen::stopRunning(){program_running = false;}

bool SDL_Screen::rollover(int mx, int my, int x, int y, int w, int h)
{
    if (mx >= x && mx < x + w && my > y && my < y + h)
        return true;
    return false;
}

bool SDL_Screen::setFont(TTF_Font **font, const char* font_file, int size)
{
    *font = TTF_OpenFont(font_file, size);
    if (*font == nullptr){
        throw new runtime_error("error : font not found");
        return false;
    }
    return true;
}

bool SDL_Screen::text(int x, int y, const char * text, TTF_Font *font){
    Uint8 cr, cg, cb, ca;
    if(SDL_GetRenderDrawColor(r, &cr, &cg, &cb, &ca) != 0)
        SDL_ExitWithError("failed to save color");
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Color textColor = {cr, cg, cb, ca};

    surface = TTF_RenderUTF8_Blended(font, text, textColor);
    texture = SDL_CreateTextureFromSurface(r, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = text_width;
    rectangle.h = text_height;
    SDL_RenderCopy(r, texture, NULL, &rectangle);
    SDL_DestroyTexture(texture);
    return true;
}

bool SDL_Screen::text(int x, int y, const char *text, TTF_Font *font, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha){
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Color textColor = {red, green, blue, alpha};

    surface = TTF_RenderUTF8_Blended(font, text, textColor);
    texture = SDL_CreateTextureFromSurface(r, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = text_width;
    rectangle.h = text_height;
    SDL_RenderCopy(r, texture, NULL, &rectangle);
    SDL_DestroyTexture(texture);
    return true;
}

void SDL_Screen::paragraph(int x, int y, const char* text, TTF_Font* font, 
                        unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha){

    SDL_Color text_color = {red, green, blue, alpha};
    // Split the text by newline characters
    std::stringstream textStream(text);
    std::string line;
    while (std::getline(textStream, line))
    {
        // Create a surface with the text 
        SDL_Surface* lineSurface = TTF_RenderText_Blended(font, line.c_str(), text_color);
        int w = lineSurface->w;
        int h = lineSurface->h;

        SDL_Texture* lineTexture = SDL_CreateTextureFromSurface(r, lineSurface);
        SDL_Rect lineRect = { x, y, w, h };
        SDL_RenderCopy(r, lineTexture, NULL, &lineRect);// display

        // Free memory
        SDL_FreeSurface(lineSurface);
        SDL_DestroyTexture(lineTexture);

        //next line of text
        y += TTF_FontHeight(font);
    }
}

void SDL_Screen::paragraph(int x, int y, const char* text, TTF_Font* font){
    Uint8 cr, cg, cb, ca;
    if(SDL_GetRenderDrawColor(r, &cr, &cg, &cb, &ca) != 0)
        SDL_ExitWithError("failed to save color");
    SDL_Color text_color = {cr, cg, cb, ca};
    // Split the text by newline characters
    std::stringstream textStream(text);
    std::string line;
    while (std::getline(textStream, line))
    {
        // Create a surface with the text 
        SDL_Surface* lineSurface = TTF_RenderText_Blended(font, line.c_str(), text_color);
        int w = lineSurface->w;
        int h = lineSurface->h;

        SDL_Texture* lineTexture = SDL_CreateTextureFromSurface(r, lineSurface);
        SDL_Rect lineRect = { x, y, w, h };
        SDL_RenderCopy(r, lineTexture, NULL, &lineRect);// display

        // Free memory
        SDL_FreeSurface(lineSurface);
        SDL_DestroyTexture(lineTexture);

        //next line of text
        y += TTF_FontHeight(font);
    }
}