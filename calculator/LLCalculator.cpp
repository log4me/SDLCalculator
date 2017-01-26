//LLCalculator.cpp
#include "LLCalculator.h"
#include <cmath>
const double LLCalculator::PI = 3.1415926535898;
const double LLCalculator::e = 2.718281828459;
LLCalculator::LLCalculator(){
}
LLCalculator::~LLCalculator(){
}
//lexical analysis
LLSTATE LLCalculator::parser(){
    if(m_read >= m_expr.size()){
        m_state = LLS_END;
        return m_state;
    }
    char c = m_expr[m_read];
    while(c == ' '){
        c = m_expr[++m_read];
    }
    if(c >= '0' && c <= '9'){
        m_val = parser_num();
        m_state = LLS_NUM;
    }else if(c == 'e'){
        m_val = e;
        m_state = LLS_NUM;
        m_read += 1;
    }else if(c == 'p'){
        m_val = PI;
        m_state = LLS_NUM;
        m_read += 2;
    }else{
        m_read++;
        if(c == '+'){
            m_state = LLS_ADD;
        }else if(c == '-'){
            m_state = LLS_SUB;
        }else if(c == '*'){
            m_state = LLS_MUL;
        }else if(c == '/'){
            m_state = LLS_DIV;
        }else if(c == '%'){
            m_state = LLS_MOD;
        }else if(c == '^'){
            m_state = LLS_POW;
        }else if(c == '('){
            m_state = LLS_LBR;
        }else if(c == ')'){
            m_state = LLS_RBR;
        }else if(c == 's'){
            if(m_expr[m_read] == 'i'){
                m_state = LLS_SIN;
                m_read += 3;
            }else if(m_expr[m_read] == 'q'){
                m_state = LLS_SQRT;
                m_read += 4;
            }
        }else if(c == 'c'){
            m_state = LLS_COS;
            m_read += 3;
        }else if(c == 'l'){
            if(m_expr[m_read] == 'g'){
                m_state = LLS_LG;
                m_read += 2;
            }else if(m_expr[m_read] == 'n'){
                m_state = LLS_LN;
                m_read += 2;
            }
        }else if(c == 'a'){
            if(m_expr[m_read] == 's'){
                m_state = LLS_ASIN;
                m_read += 4;
            }else if(m_expr[m_read] == 'c'){
                m_state = LLS_ACOS;
                m_read += 4;
            }else if(m_expr[m_read] == 't'){
                m_state = LLS_ATAN;
                m_read += 4;
            }
        }else if(c == 't'){
            m_state = LLS_TAN;
            m_read += 3;
        }else{
            m_state = LLS_ERR;
            m_errorcode = LLE_INVALID_CHAR;
        }
    }
    return m_state;
}
LL_VAL LLCalculator::parser_num(){
    //depart num
    LL_VAL ret;
    int len;
    sscanf( m_expr.substr(m_read, m_expr.size()).c_str(),
           "%lf%n", &ret, &len );
    m_read += len;
    return ret;
}
LL_VAL LLCalculator::E(){
    //E -> TE'
    m_val = T();
    return E_();
}
LL_VAL LLCalculator::E_(){
    //E'-> +TE'|-TE'|e
    LL_VAL current_val = m_val;
    switch(m_state){
        case LLS_ADD:
            parser();
            m_val = current_val + T();
            return E_();
        case LLS_SUB:
            parser();
            m_val = current_val - T();
            return E_();
        case LLS_RBR:
        case LLS_END:
        case LLS_ERR:
            return m_val;
        default:
            m_errorcode = LLE_NOT_OPERATOR;
            m_state = LLS_ERR;
            return m_val;
    }
}
LL_VAL LLCalculator::T(){
    //T -> FT'
    m_val = F();
    return T_();
}
LL_VAL LLCalculator::T_(){
    //T'-> *FT'|/FT'|%FT'|e
    LL_VAL current_val = m_val;
    switch(m_state){
        case LLS_MUL:
            parser();
            m_val = current_val * F();
            return T_();
        case LLS_DIV:
            parser();
            m_val = current_val / F();
            return T_();
        case LLS_MOD:
            parser();
            m_val = fmod(m_val, F());
            return T_();
        case LLS_ADD:
        case LLS_SUB:
        case LLS_ERR:
        case LLS_END:
        case LLS_RBR:
            return m_val;
        default:
            m_state = LLS_ERR;
            m_errorcode = LLE_NOT_OPERATOR;
            return m_val;
    }
}
LL_VAL LLCalculator::F(){
    //F -> NF'
    m_val = N();
    return F_();
}
LL_VAL LLCalculator::F_(){
    //F'-> ^F|e
    LL_VAL current_val = m_val;
    switch(m_state){
        case LLS_POW:
            parser();
            return pow(current_val, F());
        case LLS_ADD:
        case LLS_SUB:
        case LLS_MUL:
        case LLS_MOD:
        case LLS_RBR:
        case LLS_DIV:
        case LLS_END:
        case LLS_ERR:
            return m_val;
        default :
            m_state = LLS_ERR;
            m_errorcode = LLE_NOT_OPERATOR;
            return m_val;
    }
}
LL_VAL LLCalculator::N(){
    //N -> (E)|-VAL|+VAL|VAL|sin(E)|cos(E)|tan(E)|ln(E)|lg(E)|sqrt(E)
    LL_VAL ret = LL_VAL_DEFVAL;
    int lbr_pos = m_read;
    switch(m_state){
        case LLS_LBR:
            parser();
            ret = E();
            if(m_state != LLS_RBR){
                if(m_errorcode == LLE_OK){
                    m_errorcode = LLE_BK_NOT_MATCH;
                    m_read = lbr_pos;
                }
                m_state = LLS_ERR;
                return ret;
            }
            break;
        case LLS_ADD:
        case LLS_SUB:
            if(true){
                LLSTATE current_state = m_state;
                parser();
                if(m_state != LLS_NUM){
                    m_errorcode = LLE_NOT_A_NUM;
                    m_state = LLS_ERR;
                }
                ret = (current_state == LLS_ADD ? m_val : -m_val);
                break;
            }
        case LLS_NUM:
            ret = m_val;
            break;
        case LLS_SIN:
            m_state = LLS_LBR;
            parser();
            ret = sin(E());
            if(m_state != LLS_RBR){
                if(m_errorcode == LLE_OK){
                    m_errorcode = LLE_BK_NOT_MATCH;
                    m_read = lbr_pos;
                }
                m_state = LLS_ERR;
                return ret;
            }
            break;
        case LLS_ASIN:
            m_state = LLS_LBR;
            parser();
            ret = asin(E());
            if(m_state != LLS_RBR){
                if(m_errorcode == LLE_OK){
                    m_errorcode = LLE_BK_NOT_MATCH;
                    m_read = lbr_pos;
                }
                m_state = LLS_ERR;
                return ret;
            }
            break;
        case LLS_COS:
            m_state = LLS_LBR;
            parser();
            ret = cos(E());
            if(m_state != LLS_RBR){
                if(m_errorcode == LLE_OK){
                    m_errorcode = LLE_BK_NOT_MATCH;
                    m_read = lbr_pos;
                }
                m_state = LLS_ERR;
                return ret;
            }
            break;
        case LLS_ACOS:
            m_state = LLS_LBR;
            parser();
            ret = acos(E());
            if(m_state != LLS_RBR){
                if(m_errorcode == LLE_OK){
                    m_errorcode = LLE_BK_NOT_MATCH;
                    m_read = lbr_pos;
                }
                m_state = LLS_ERR;
                return ret;
            }
            break;
        case LLS_TAN:
            m_state = LLS_LBR;
            parser();
            ret = tan(E());
            if(m_state != LLS_RBR){
                if(m_errorcode == LLE_OK){
                    m_errorcode = LLE_BK_NOT_MATCH;
                    m_read = lbr_pos;
                }
                m_state = LLS_ERR;
                return ret;
            }
            break;
        case LLS_ATAN:
            m_state = LLS_LBR;
            parser();
            ret = atan(E());
            if(m_state != LLS_RBR){
                if(m_errorcode == LLE_OK){
                    m_errorcode = LLE_BK_NOT_MATCH;
                    m_read = lbr_pos;
                }
                m_state = LLS_ERR;
                return ret;
            }
            break;
        case LLS_LN:
            m_state = LLS_LBR;
            parser();
            ret = log(E());
            if(m_state != LLS_RBR){
                if(m_errorcode == LLE_OK){
                    m_errorcode = LLE_BK_NOT_MATCH;
                    m_read = lbr_pos;
                }
                m_state = LLS_ERR;
                return ret;
            }
            break;
        case LLS_LG:
            m_state = LLS_LBR;
            parser();
            ret = log10(E());
            if(m_state != LLS_RBR){
                if(m_errorcode == LLE_OK){
                    m_errorcode = LLE_BK_NOT_MATCH;
                    m_read = lbr_pos;
                }
                m_state = LLS_ERR;
                return ret;
            }
            break;
        case LLS_SQRT:
            m_state = LLS_LBR;
            parser();
            ret = sqrt(E());
            if(m_state != LLS_RBR){
                if(m_errorcode == LLE_OK){
                    m_errorcode = LLE_BK_NOT_MATCH;
                    m_read = lbr_pos;
                }
                m_state = LLS_ERR;
                return ret;
            }
            break;
        default:
            m_state = LLS_ERR;
            m_errorcode = LLE_NOT_A_NUM;
            return ret;
    }
    parser();
    return ret;
}
const char* LLCalculator::get_error_msg( int errorcode ) {
    static const char* errstr[] = {
        "",
        "invalid character",
        "bracket does not match",
        "not a number",
        "need operator",
    };
    if ( errorcode < 0 || errorcode >=  sizeof(errstr)/sizeof(*errstr)) {
        errorcode = 0;
    }
    return errstr[errorcode];
}
LLERROR LLCalculator::calculate(LL_VAL &res, int &error_pos, const string expr){
    m_expr = string(expr);
    m_read = 0;
    m_errorcode = LLE_OK;
    parser();
    res = E();
    error_pos = m_read;
    if ( m_errorcode == LLE_OK && m_state != LLS_END ) {
        m_errorcode = LLE_BK_NOT_MATCH;
    }
    return m_errorcode;
}
