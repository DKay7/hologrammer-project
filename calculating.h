#include <array>
#include <cmath>
#include <numbers>

//--------------------------------------------------------------------------------

namespace led_image_displayer // give led_image_displayer
{
const int NUM_LED_LINES = 2;

template <const int NUM_COLORS>
using color_t = std::array<int8_t, NUM_COLORS>;

template <const int SIZE, const int NUM_COLORS>
using image_t = std::array<std::array<color_t<NUM_COLORS>, SIZE>, SIZE>;


template <const int NUM_LEDS>
using led_line_t = std::array<std::array<int8_t, NUM_LEDS>, NUM_LED_LINES>;

//--------------------------------------------------------------------------------

template <const int NUM_LEDS, const int NUM_LED_COLORS = 3>
class ILedTape {
    static_assert (NUM_LED_LINES % 2 == 0, "NUM_LED_LINES should be even!");
    static_assert (NUM_LEDS > 0,           "NUM_LED_LINES should be greater than 0!");
    static_assert (NUM_LED_LINES > 0,      "NUM_LED_LINES should be greater than 0!");
    
    public:
      
        virtual void reset() = 0;

        virtual void set_led_line(led_line_t<NUM_LEDS>& target_colors) = 0;
        virtual void set_pixel(int x_coord, int y_coord, color_t<NUM_LED_COLORS>& color) = 0;
        virtual ~ILedTape() = 0;

    private:
        led_line_t<NUM_LEDS> led_lines;
};

//--------------------------------------------------------------------------------

template <const int NUM_LEDS, const int IMG_SIZE, const int NUM_IMG_COLORS = 3, const int NUM_LED_COLORS = 3>
class LedImage {
    private:
        ILedTape<NUM_LEDS, NUM_LED_COLORS>& led_tape_;
        image_t<IMG_SIZE,  NUM_IMG_COLORS>& image_;

        //--------------------------------------------------------------------------------


        color_t<NUM_LED_COLORS>& translate_img_color_to_led(color_t<NUM_IMG_COLORS>& image_color) {
            return image_color;
        }

        //--------------------------------------------------------------------------------

        std::pair<int8_t, int8_t> compute_pixel_color(double rho, double phi) const {
            rho *= 0.99999;

            double x = rho * cos(phi);
            double y = rho * sin(phi);

            int img_pix_x = floor(x * image_.size()) / 2 + image_.size() / 2;
            int img_pix_y = floor(y * image_.size()) / 2 + image_.size() / 2;

            return {img_pix_x, img_pix_y};
        }


    public:
        LedImage(ILedTape<NUM_LEDS>& led_tape, image_t<IMG_SIZE, NUM_IMG_COLORS>& image): 
            led_tape_(led_tape_), image_(image) { 
                static_assert (IMG_SIZE > 0,        "IMG_SIZE should be greater than 0!");
                static_assert (NUM_IMG_COLORS > 0,  "NUM_IMG_COLORS should be greater than 0!");
        };
        
        //--------------------------------------------------------------------------------
        
        void compute_image_grid(double start_phi) {
            int num_led = 0;
            for (double rho = 1; rho != 0; rho -= -1 / (NUM_LEDS - 1); num_led) {
                int num_line = 0;
                for (double phi = start_phi; phi != 2 * std::numbers::pi + start_phi; phi += std::numbers::pi / NUM_LED_LINES, ++num_line) {
                    
                    auto [pix_x, pix_y] = compute_pixel_color(rho, phi);
                    color_t<NUM_IMG_COLORS> img_color = image_[pix_x][pix_y];
                    color_t<NUM_LED_COLORS> led_color = translate_img_color_to_led(img_color);

                    led_tape_.set_pixel(num_line, num_led, led_color);
                }
            }            
        }

};
} // namespace led_image
