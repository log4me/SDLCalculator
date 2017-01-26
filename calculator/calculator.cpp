//calculate.cpp
#include "calculator.h"
#include <iostream>
#include <sstream>
using namespace std;

const string Calculate::buttonImages[29] = {
		"2nd.png", "sqrt.png", "pi.png", "del.png", "clear.png",
		"sin.png", "left_b.png", "right_b.png", "e.png", "sub.png",
		"cos.png", "7.png", "8.png", "9.png", "div.png",
		"tan.png", "4.png", "5.png", "6.png", "mul.png",
		"ln.png","1.png","2.png","3.png",
		"lg.png","0.png","dot.png","add.png","enter.png"
};
const string Calculate::buttonImages2[29] = {
		"2nd.png", "power.png", "pi.png", "del.png", "clear.png",
		"asin.png", "left_b.png", "right_b.png", "e.png", "sub.png",
		"acos.png", "7.png", "8.png", "9.png", "div.png",
		"atan.png", "4.png", "5.png", "6.png", "mul.png",
		"ln.png","1.png","2.png","3.png",
		"lg.png","0.png","dot.png","add.png","enter.png"
};
const string Calculate::imageFolder = "assets/images";
const string Calculate::buttons_text[29] = {
		"2nd", "sqrt(", "pi", "del", "clear",
		"sin(", "(", ")", "e", "-",
		"cos(", "7", "8", "9", "/",
		"tan(", "4", "5", "6", "*",
		"ln(","1","2","3",
		"lg(","0",".","+","="
};
const string Calculate::buttons_text2[29] = {
		"2nd", "^", "pi", "del", "clear",
		"asin(", "(", ")", "e", "-",
		"acos(", "7", "8", "9", "/",
		"atan(", "4", "5", "6", "*",
		"ln(","1","2","3",
		"lg(","0",".","+","="
};
const string Calculate::functions[10] = {
	"asin(","acos(","atan(","sin(","cos(","tan(","lg(","ln(","sqrt(","pi",
};
Calculate::Calculate(){

}
Calculate::~Calculate(){

}
void Calculate::init(){
	SDL_Init(SDL_INIT_VIDEO);
    sdlWindow = SDL_CreateWindow("Calculater",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, button_width * 5 + button_interval * 6,
                screen_height + button_interval * 7 + button_height * 6, 0);
    sdlScreen = SDL_GetWindowSurface(sdlWindow);
	imageLoader = new gcn::SDLImageLoader();
	gcn::Image::setImageLoader(imageLoader);
	graphics = new gcn::SDLGraphics();
	graphics->setTarget(sdlScreen);
	input = new gcn::SDLInput();
	top = new gcn::Container();
	top->setDimension(gcn::Rectangle(0, 0, button_width * 5 + button_interval * 6, screen_height + button_interval * 7 + button_height * 6));
	this->setGraphics(graphics);
	this->setInput(input);
	this->setTop(top);
	//font = new gcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
	font_big_bold = new gcn::SDLTrueTypeFont("./assets/fonts/msyhbd.ttf",30);
	font_small_light = new gcn::SDLTrueTypeFont("./assets/fonts/msyh.ttf",20);
	gcn::Widget::setGlobalFont(font_small_light);
	screen_icon = new gcn::Icon(string(imageFolder) + "/" + "screen.png");
	top->add(screen_icon, 0, 0);
	expr = string("0");
	expr_label = new gcn::Label(expr);
	expr_label->setSize(button_width * 5 + button_interval * 6, 70);
	expr_label->setAlignment(gcn::Graphics::RIGHT);
	expr_label->setFont(font_big_bold);
	error_label  = new gcn::Label(res);
	error_label->setSize(button_width * 5 + button_interval * 6, 40);
	error_label->setAlignment(gcn::Graphics::RIGHT);
	error_label->setFont(font_big_bold);
	error_label->setCaption("Error");
	error_label->setVisible(false);
	res = string("0");
	res_label = new gcn::Label(res);
	res_label->setSize(button_width * 5 + button_interval * 6, 50);
	res_label->setAlignment(gcn::Graphics::RIGHT);
	res_label->setFont(font_small_light);
	top->add(expr_label, 0, -15);
	top->add(error_label,0, 30);
	top->add(res_label, 0, 60);
	for(int i = 0; i < 28 ; i++){
		if(i==0){
			button_pos[0][0] = button_interval;
			button_pos[0][1] = screen_height + button_interval;
		}else if(i <= 23){
			button_pos[i][0] = button_pos[0][0] + (button_interval + button_width) * (i % 5);
			button_pos[i][1] = button_pos[0][1] + (button_interval + button_height) * (i / 5);
		}else{
			button_pos[i][0] = button_pos[0][0] + (button_interval + button_width) * ((i + 1) % 5);
			button_pos[i][1] = button_pos[0][1] + (button_interval + button_height) * ((i + 1) / 5);
		}
	}
	button_pos[28][0] = button_pos[0][0] + (button_interval + button_width) * 4;
	button_pos[28][1] = button_pos[0][1] + (button_interval + button_height) * 4;
	for( int i = 0; i < 29; i++){
		buttons[i] = new gcn::ImageButton(string(imageFolder) + "/" + buttonImages[i]);
		buttons2[i] = new gcn::ImageButton(string(imageFolder) + "/" + buttonImages2[i]);
		buttons[i]->setActionEventId(buttons_text[i]);
		buttons2[i]->setActionEventId(buttons_text2[i]);
		buttons[i]->adjustSize();
		buttons2[i]->adjustSize();
		buttons[i]->addActionListener(this);
		buttons2[i]->addActionListener(this);
		buttons2[i]->setVisible(false);
		top->add(buttons[i], button_pos[i][0], button_pos[i][1]);
		top->add(buttons2[i], button_pos[i][0], button_pos[i][1]);
	}
	button_state = 1;
	calculateState = CALCULATED;
	llc = LLCalculator();
	running = true;
}

void Calculate::run(){
	while(running){
		checkInput();
		logic();
		draw();
		SDL_UpdateWindowSurface(sdlWindow);
	}

}
void Calculate::halt(){

	/*
	 * Destroy Guichan stuff
	 */
	delete font_big_bold;
	delete font_small_light;
	delete screen_icon;
	delete res_label;
	delete expr_label;
	delete error_label;
	delete top;

	/*
	 * Destroy Guichan SDL stuff
	 */
	delete input;
	delete graphics;
	delete imageLoader;
	for(int i = 0; i < 29; i++){
		delete buttons[i];
	}

	/*
	 * Destroy SDL stuff
	 */
	SDL_Quit();

}
void Calculate::checkInput(){

	while(SDL_PollEvent(&event)){
		if (event.type == SDL_KEYDOWN){
			if (event.key.keysym.sym == SDLK_ESCAPE){
				running = false;
			}
		}
		else if(event.type == SDL_QUIT){
			running = false;
		}

		input->pushInput(event);
	}
}
bool Calculate::functionBegin(char ch){
	return ch == 's' || ch == 'c' || ch == 't' || ch == 'l' || ch == 's' || ch == 'e' || ch == 'p';
}

bool Calculate::isOperator(char ch){
	return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^' || ch =='(' || ch == ')';
}
bool Calculate::isDigit(char ch){
	return ch >= '0' && ch <= '9';
}
bool Calculate::isBracket(char ch){
	return ch == ')' || ch == '(';
}
int Calculate::supply_right_bracket(const char *expr){
	int i = 0;
	int bracket = 0;
	while(expr[i] != '\0'){
		if(expr[i] == '('){
			bracket++;
		}else if(expr[i] == ')'){
			bracket--;
		}
		i++;
	}
	if(bracket < 0){
		bracket = 0;
	}
	return bracket;
}
void Calculate::change_to_2nd(){
	if(button_state == 1){
		button_state = 2;
		for(int i = 0 ; i < 29; i++){
			buttons2[i]->setVisible(true);
			buttons[i]->setVisible(false);
		}
	}else{
		button_state = 1;		
		for(int i = 0 ; i < 29; i++){
			buttons2[i]->setVisible(false);
			buttons[i]->setVisible(true);
		}
	}
}
void Calculate::action(const gcn::ActionEvent &actionEvent){
	if(calculateState == ERROR){
		error_label->setVisible(false);
		res = res_before_error;

	}
	if(actionEvent.getId() == "2nd"){
		change_to_2nd();
		return;
	}else if(actionEvent.getId() == "del"){
		bool deleted = false;
		//Delete the function
		for(int i = 0 ; i < 7 ; i++){
			int last_fun_pos = expr.rfind(functions[i]);
			if(last_fun_pos != -1 && (expr.size() == last_fun_pos + functions[i].size())){
				for(int j = 0 ; j < functions[i].size();j++){
					expr.erase(expr.end() - 1);
				}
				deleted = true;
				break;
			}
		}
		if(!deleted){
			expr.erase(expr.end() - 1);
		}
		if(expr.size() <= 0){
			expr = "0";
			calculateState = CALCULATED;
		}else{
			calculateState = INPUTING;
		}
	}else if(actionEvent.getId() == "clear"){
		res = string("0");
		expr = string("0");
		calculateState = CALCULATED;
	}else if(actionEvent.getId() == "="){
		int r, pos;
		double v;
		string in_expr = expr;
		for(int i = 1; i < in_expr.size() ; i++){
			if(functionBegin(in_expr[i]) && isDigit(in_expr[i - 1])){
				in_expr.insert(in_expr.begin() + i,'*');
			}
		}
		int right_bracket = supply_right_bracket(expr.c_str());
		in_expr.insert(in_expr.end(), right_bracket,')');
		r = llc.calculate(v, pos, in_expr);
        if(r != LLE_OK ){
        	res_before_error = res;
        	res = llc.get_error_msg(r);
        	string  error_msg = string(in_expr.size() + 1 - pos, ' ');
        	error_msg.insert(error_msg.end(),'^');
        	error_label->setCaption(error_msg);
        	error_label->setVisible(true);
        	calculateState = ERROR;
        }else{
        	res = "";
        	stringstream out;
       		out << v;
        	res = out.str();
        	calculateState = CALCULATED;
        }
	}else{
		string id = actionEvent.getId();
		if(calculateState == CALCULATED && isOperator(id[0]) && !isBracket(id[0])){
			stringstream ss;
			//change to double
			ss << res;
			double res_val;
			ss >> res_val;
			ss.str("");
			ss.clear();
			ss << fixed << res_val;
			expr = ss.str() + id;
		}else if(calculateState == CALCULATED){
			expr = id;
		}else{
			expr += id;
		}
		calculateState = INPUTING;
	}
	if(calculateState != CALCULATED){
		res_label->setFont(font_small_light);
		expr_label->setFont(font_big_bold);
		error_label->setFont(font_big_bold);
		int r, pos;
		double v;
		string in_expr = expr;
		for(int i = 1; i < in_expr.size() ; i++){
			if(functionBegin(in_expr[i]) && isDigit(in_expr[i - 1])){
				in_expr.insert(in_expr.begin() + i,'*');
			}
		}
		int right_bracket = supply_right_bracket(expr.c_str());
		in_expr.insert(in_expr.end(), right_bracket,')');
		r = llc.calculate(v, pos, in_expr);
        if(r == LLE_OK ){
        	stringstream out;
       		out << v;
        	res = out.str();
        }
	}else{
		res_label->setFont(font_big_bold);
		expr_label->setFont(font_small_light);
		error_label->setFont(font_small_light);
	}
	expr_label->setCaption(expr);
	res_label->setCaption(res);
}
