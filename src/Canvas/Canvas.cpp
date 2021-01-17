#include <cmath>
#include <iostream>

#include "Canvas.h"
#include "Color.h"

Canvas::Canvas(int width, int height) : width(width), height(height), arrayOfPixels(new Color[width*height]){
}

Color Canvas::pixelAt(int x, int y) {
    return arrayOfPixels[height*x + y];
}

void Canvas::writePixel(int x, int y, Color const& color){
    arrayOfPixels[height*x + y] = color;
};

void Canvas::fill(Color const& color){
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){    
            writePixel(i, j, color);
        }
    }
}


//Functions outside of the class

std::stringstream canvasToPPM(Canvas &canvas) {
    std::stringstream ppm;

    writeHeader(ppm, canvas.width, canvas.height);
    writeBody(ppm, canvas);

    return ppm;
};

static void writeHeader(std::stringstream &ss, int width, int height){
    ss << "P3\n" << width << " " << height << "\n" << 255 << "\n";   
};

static void writeBody(std::stringstream &ss, Canvas &canvas){
    for(int j = 0; j < canvas.height; j++){
        std::stringstream tmp;
        
        for(int i = 0; i < canvas.width; i++){    
            Color color = canvas.pixelAt(i,j)*255.0f;
            int red = clamp(color.red);
            int green = clamp(color.green);
            int blue = clamp(color.blue);

            //Write red
            if(tmp.str().length() + std::to_string(red).length() >= 70) {
                std::string line2 = tmp.str();
                ss << line2.substr(0, line2.size()-1) << "\n";
                tmp.str("");
                tmp << red << " ";
            } else {
                tmp << clamp(color.red) << " ";
            }

            //Write green
            if(tmp.str().length() + std::to_string(green).length() >= 70) {
                std::string line2 = tmp.str();
                ss << line2.substr(0, line2.size()-1) << "\n";
                tmp.str("");
                tmp << green << " ";
            } else {
                tmp << clamp(color.green) << " ";
            }

            //Write blue
            if(tmp.str().length() + std::to_string(blue).length() >= 70) {
                std::string line2 = tmp.str();
                ss << line2.substr(0, line2.size()-1) << "\n";
                tmp.str("");
                tmp << blue << " ";
            } else {
                tmp << clamp(color.blue) << " ";
            }
                
           
        }

        auto line = tmp.str();
        ss << line.substr(0, line.size()-1) << "\n";
    }
};

int clamp(float number) {
    if (number < 0) {
        number = 0;
    } else if (number > 255) {
        number = 255;
    }
    return round(number);
};