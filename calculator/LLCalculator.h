/**************************************
 * LLCalculator.h
 * author:logme
 * support: + - * / % ^ ( ) sin cos tan lg ln sqrt asin acos atan
 *-------------------------------------
 * raw grammar
 * E -> E+T|E-T|T
 * T -> T*F|T/F|T%F|F
 * F -> N^F|N
 * N -> (E)|-VAL|+VAL|VAL
 *-------------------------------------
 * remove left recursive
 * E -> TE'
 * E'-> +TE'|-TE'|e
 * T -> FT'
 * T'-> *FT'|/FT'|%FT'|e
 * F -> NF'
 * F'-> ^F|e
 * N -> (E)|-VAL|+VAL|VAL|sin(E)|cos(E)|tan(E)|ln(E)|lg(E)|sqrt(E)
 * ------------------------------------
 * char set [0-9]|.|e|+|-|*|/|%|^|(|)|pi|sin|cos|tan|ln|lg|sqrt
 * VAL := [0-9]+(\.[0-9]+)?(e(+|-)?[0-9]+)?
 * ************************************/

 #include <string>
 #include <iostream>
 #include <sstream>
 using namespace std;
 typedef enum {
 	LLS_NUM,
 	LLS_ADD,
 	LLS_SUB,
 	LLS_MUL,
 	LLS_DIV,
 	LLS_MOD,
 	LLS_POW,
 	LLS_LBR,
 	LLS_RBR,
 	LLS_SIN,
 	LLS_COS,
 	LLS_TAN,
 	LLS_SQRT,
 	LLS_LG,
 	LLS_LN,
 	LLS_ASIN,
 	LLS_ACOS,
 	LLS_ATAN,
 	LLS_END,
 	LLS_ERR

 } LLSTATE;
 typedef enum {
 	LLE_OK,
 	LLE_INVALID_CHAR,
 	LLE_BK_NOT_MATCH,
 	LLE_NOT_A_NUM,
 	LLE_NOT_OPERATOR
 } LLERROR;
 typedef double LL_VAL;
 class LLCalculator{
 public:
 	LLCalculator();
 	~LLCalculator();
 	const char* get_error_msg(int errorcode);
 	LLERROR calculate(LL_VAL &res, int &error_pos,const string expression);
 private:
 	LL_VAL parser_num();
 	LLSTATE parser();
 	LL_VAL F_();
 	LL_VAL N();
 	LL_VAL E();
 	LL_VAL E_();
 	LL_VAL T();
 	LL_VAL T_();
 	LL_VAL F();
 	static const int LL_VAL_DEFVAL = 0;
 	static const LL_VAL PI;
 	static const LL_VAL e;
 	string m_expr;
 	int m_read;
 	LLSTATE m_state;
 	LLERROR m_errorcode;
 	LL_VAL m_val;
 };
