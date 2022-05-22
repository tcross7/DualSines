
#include <Arduino.h>
#include <RGBmatrixPanel.h>

#define CLK  11   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

float sine_quantize(float frequency, int x_val, int f){
    float spacer = 0.009;
    float y_snippet = sin(frequency*2*M_PI*spacer*x_val);
    if (f == 1){
        y_snippet = -1*y_snippet; // flip the second frequency for visiblity
    }
    return y_snippet;
}

int sine_compand(float frequency){
    int newfreq = 2*log(frequency/100);
    return newfreq;
}


int sine_scale(float y_snippet){ // returns a number from 0-15

    int height = 8;
    int y_quantized;
    float range_bins[17] = {1, 0.875, 0.75, 0.625, 0.5, 0.375, 0.25, 0.125, 0, -0.125, -0.25, -0.375, -0.5, -0.625, -0.75, -0.875, -1};

    for (int i = 0 ; i < 16 ; i++){
        if ( y_snippet*height/8 < range_bins[i] && y_snippet*height/8 >= range_bins[i+1]){ //adjust input signal for height
            y_quantized = i;
        }
    }
    return y_quantized; 
}

void draw_sine_reverse(int height[], int freq, int shift){

    matrix.fillScreen(0); // clear the screen

    float color = matrix.Color333(random(0, 7), random(0,7), random(0,7));
    
    int display_x[64];
    int display_y[64];

    for (int x = 63 ; x > 0; x--){ //generate waveform
        
        // calculate red wave
        display_x[x] = x-shift;
        display_y[x] = sine_scale(sine_quantize(sine_compand(freq), x, 0));
        
        matrix.drawPixel(display_x[x], display_y[x], color); 
        delay(40); // delay for drawing effect
    }
}

void draw_sine(int freq, int shift){

    matrix.fillScreen(0); // clear the screen

    float color = matrix.Color333(random(0, 7), random(0,7), random(0,7));
    
    int display_x[64];
    int display_y[64];

    for (int x = 0 ; x < 62; x++){ //generate waveform
        
        // calculate red wave
        display_x[x] = x-shift;
        display_y[x] = sine_scale(sine_quantize(sine_compand(freq), x, 0));
        
        matrix.drawPixel(display_x[x], display_y[x], color); 
        delay(40); // delay for drawing effect
    }
}

void draw_dual_sines()
{
    int randFreqL = random(300, 13000);
    int randShiftL = random (0, 10);

    int randFreqR = random(300, 13000);
    int randShiftR = random(0, 10);

    float colorL = matrix.Color333(random(0, 7), random(0,7), random(0,7));
    float colorR = matrix.Color333(random(0, 7), random(0,7), random(0,7));

    int display_lx[64];
    int display_ly[64];

    int display_rx[64];
    int display_ry[64];

    for (int x = 0 ; x < 63; x++){ //generate waveform
        
        // left to right wave: same scale
        int x_adj_l = x;

        display_lx[x_adj_l] = x_adj_l-randShiftL;
        display_ly[x_adj_l] = sine_scale(sine_quantize(sine_compand(randFreqL), x_adj_l, 0));
        
        matrix.drawPixel(display_lx[x_adj_l], display_ly[x_adj_l], colorL);

        // right to left wave: reverse scale and shifted to 0:63
        int x_adj_r = (-1*x) + 63;

        display_rx[x_adj_r] = x_adj_r-randShiftR;
        display_ry[x_adj_r] = sine_scale(sine_quantize(sine_compand(randFreqR), x_adj_r, 0));
        
        matrix.drawPixel(display_rx[x_adj_r], display_ry[x_adj_r], colorR);

        delay(50); // delay for drawing effect
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() 
{

    matrix.begin();
    Serial.begin(9600);
    randomSeed(analogRead(A0));

}

void loop() 
{

    draw_dual_sines();
    matrix.fillScreen(0); // clear the screen

}
