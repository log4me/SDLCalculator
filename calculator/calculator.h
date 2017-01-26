//calculate.h
#ifndef CALCULATE_H
#define CALCULATE_H
#include "guisan.hpp"
#include "guisan/gui.hpp"
#include "SDL2/SDL.h"
#include "guisan/sdl.hpp"
#include "guisan/sdl/sdltruetypefont.hpp"
#include "LLCalculator.h"
#include <string>
using namespace std;
typedef enum {
	INPUTING,
	CALCULATED,
	ERROR,
	WAITTING
} CalculateState;
class Calculate : public gcn::Gui,
				  public gcn::ActionListener
{
public:
	Calculate();
	~Calculate();
	void init();
	void halt();
	void checkInput();
	void run();
	void action(const gcn::ActionEvent& actionEvent);
private:
	bool functionBegin(char ch);
	bool isOperator(char ch);
	int supply_right_bracket(const char *expr);
	bool isDigit(char ch);
	bool isBracket(char ch);
	int button_state;
	void change_to_2nd();
	LLCalculator llc;
	/*
	*button[0] : ^
	*button[1] : sqrt
	*button[2] : pi 
	*button[3] : del
	*button[4] : clear
	*button[5] : sin
	*button[6] : left_bracket
	*button[7] : right_bracket
	*button[8] : e
	*button[9] : sub
	*button[10] : cos
	*button[11] : 7
	*button[12] : 8
	*button[13] : 9
	*button[14] : div
	*button[15] : tan
	*button[16] : 4
	*button[17] : 5
	*button[18] : 6
	*button[19] : mul
	*button[20] : ln
	*button[21] : 1
	*button[22] : 2
	*button[23] : 3
	*button[24] : lg
	*button[25] : 0
	*button[26] : dot
	*button[27] : +
	*button[28] : =
	*
	*/
	void calculateResult();
	gcn::ImageButton* buttons[29];
	gcn::ImageButton* buttons2[29];
	static const string buttonImages[29];
	static const string buttonImages2[29];
	// = {
	//	"power.png", "sqrt.png", "pi.png", "del.png", "clear.png",
	//	"sin.png", "left_b.png", "right_b.png", "e.png", "sub.png",
	//	"cos.png", "7.png", "8.png", "9.png", "div.png",
	//	"tan.png", "4.png", "5.png", "6.png", "mul.png",
	//	"ln.png","1.png","2.png","3.png",
	//	"lg.png","0.png","dot.png","add.png","enter.png"
	//};
	static const string imageFolder;
	 //= "assets/images";
	static const string buttons_text[29] ;
	static const string buttons_text2[29];
	//= {
	//	"^", "sqrt(", "π", "del", "clear",
	//	"sin(", "(", ")", "e", "-",
	//	"cos(", "7", "8", "9", "/",
	//	"tan(", "4", "5", "6", "X",
	//	"ln","1","2","3",
	//	"lg","0",".","+","="
	//};
	static const int button_width = 92;
	static const int button_height = 54;
	static const int button_interval = 9;
	static const int screen_height = 120;
	static const string functions[10];
	int button_pos[29][2];


	SDL_Surface* sdlScreen;
	SDL_Window* sdlWindow;
	SDL_Event event;
	bool running;

	gcn::SDLInput* input;             // Input driver
	gcn::SDLGraphics* graphics;       // Graphics driver
	gcn::SDLImageLoader* imageLoader; // For loading images

	gcn::Container* top;      // A top container
	gcn::SDLTrueTypeFont* font_big_bold;     // A font
	gcn::SDLTrueTypeFont* font_small_light;     // A font
	gcn::Icon* screen_icon;
	gcn::Label* res_label;
	gcn::Label* expr_label;
	gcn::Label* error_label;
	string res;
	string res_before_error;
	string expr;
	CalculateState calculateState;
};
#endif
