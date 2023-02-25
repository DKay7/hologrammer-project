#include <array>
#include <cmath>
#include <numbers>

//--------------------------------------------------------------------------------

namespace led_img // give led_image_displayer
{
const int NUM_LED_LINES = 2;

template <const int NUM_COLORS>
using color_t = std::array<int8_t, NUM_COLORS>;

template <const int SIZE, const int NUM_COLORS>
using image_t = std::array<std::array<color_t<NUM_COLORS>, SIZE>, SIZE>;


template <const int NUM_LEDS>
using led_line_t = std::array<std::array<int8_t, NUM_LEDS>, NUM_LED_LINES>;

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

template <const int NUM_IMG_COLORS = 3, const int NUM_LED_COLORS = 3>
color_t<NUM_LED_COLORS>& 
translate_img_color_to_led(color_t<NUM_IMG_COLORS>& image_color) {
    return image_color;
}

//--------------------------------------------------------------------------------
        
template <const int IMG_SIZE, const int NUM_IMG_COLORS = 3>
std::pair<int8_t, int8_t> 
compute_pixel_color(image_t<IMG_SIZE,  NUM_IMG_COLORS>& image_, double rho, double phi) {
    rho *= 0.99999;

    double x = rho * cos(phi);
    double y = rho * sin(phi);

    int img_pix_x = floor(x * image_.size()) / 2 + image_.size() / 2;
    int img_pix_y = floor(y * image_.size()) / 2 + image_.size() / 2;

    return {img_pix_x, img_pix_y};
}

//--------------------------------------------------------------------------------

template <typename ILedTape, const int IMG_SIZE, const int NUM_LEDS, const int NUM_IMG_COLORS = 3, const int NUM_LED_COLORS = 3>
void compute_image_grid(image_t<IMG_SIZE,  NUM_IMG_COLORS>& image_, ILedTape led_tape_, double start_phi) {
    int num_led = 0;
 
    for (double rho = 1; rho != 0; rho -= -1 / (NUM_LEDS - 1), ++num_led) {
        int num_line = 0;

        for (double phi = start_phi; 
             phi != 2 * std::numbers::pi + start_phi; 
             phi += std::numbers::pi / NUM_LED_LINES, ++num_line) {
            
            auto [pix_x, pix_y] = compute_pixel_color(image_, rho, phi);
            color_t<NUM_IMG_COLORS> img_color = image_[pix_x][pix_y];
            color_t<NUM_LED_COLORS> led_color = translate_img_color_to_led(img_color);

            led_tape_.set_pixel(num_line, num_led, led_color);
        }
    }            
}

//--------------------------------------------------------------------------------

} // namespace led_image
