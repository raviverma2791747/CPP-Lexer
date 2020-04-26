
#include <iostream>
#include<fstream>
#include<string>
#include<vector>

char operators[] = "#+-*/%&|!=><[](){},.;:'\"?^~\\";
std::string s_operators[] = {"#","+","-","*","/","%","&","|","!","=",">","<","[","]","(",")","{","}",",",".",";",":","'","\"","?","^","~","\\"};
std::string Keywords[] = { "asm","auto","bool","break","case",
"catch","char","class","const_cast","continue",
"default","delete","double","do","dynamic_cast",
"else","enum","explicit","extern","false","float",
"for","friend","gotot","if","inline","int","long",
"mutable","namespace","new","operator","private",
"protected","public","register","reinterpret_cast",
"return","short","signed","sizeof","static_cast","static",
"struct","switch","template","this",
"throw","true","try","typedef","typeid","typename",
"union","unsigned","using","virtual","void",
"volatile","wchar_t","while" };
char constants[] = "0123456789";


class Token
{
private:
    std::string m_token;
    std::string m_type;
    int m_line;
public:
    Token(std::string token, std::string type, int line);
    Token(char token, std::string type, int line);
    ~Token();
public:
    bool operator==(std::string str);
    bool operator==(char str);
public:
    void Log();
    void Push_back(char ch);
    void Push_back(std::string str);
    std::string token();
    
};

Token::Token(std::string token, std::string type, int line) :
    m_token(token), m_type(type), m_line(line)
{

}

Token::Token(char token, std::string type, int line) :
    m_token(1, token), m_type(type), m_line(line)
{

}

Token::~Token()
{

}

bool Token::operator==(std::string str)
{
    if (m_token == str)
    {
        return true;
    }
    return false;
}

bool Token::operator==(char str)
{
    std::string t_str(str, 1);
    if (m_token == t_str)
    {
        return true;
    }
    return false;
}

void Token::Log()
{
    std::cout.width(4);
    std::cout <<std::left<< m_line << " | "; 
    std::cout.width(10); 
    std::cout << std::left << m_type << " | " << m_token << std::endl;
}

void Token::Push_back(char ch)
{
    m_token.push_back(ch);
}

void Token::Push_back(std::string str)
{
    for (int i = 0; i < str.size(); i++)
    {
        m_token.push_back(str[i]);
    }
}
std::string Token::token()
{
    return m_token;
}

bool IsKeyword(std::string& buffer)
{
    for (int i = 0; i < 61; i++)
    {
        if (buffer == Keywords[i] )
        {
            return true;
        }
    }
    return false;
}
bool IsKeyword(Token kwd)
{
    for (int i = 0; i < 61; i++)
    {
        if (kwd == Keywords[i])
        {
            return true;
        }
    }
    return false;
}

bool IsOperator(char ch)
{
    for (int i = 0; i < 29; i++)
    {
        if (ch == operators[i])
        {
           
            return true;
        }
    }
    return false;
}

bool IsOperator(Token ch)
{
    for (int i = 0; i < 29; i++)
    {
        if (ch == s_operators[i])
        {

            return true;
        }
    }
    return false;
}

bool IsBufferNum(std::string& buffer)
{
    
    for (int i = 0; i < buffer.size(); i++)
    {
        if (!isalnum(buffer[i]))
        {
            return false;
        }
    }
    return true;
}



int main()
{
    /*First Pass*/
    std::ifstream fin;
    fin.open("string.txt", std::ios::in);
    if (!fin)
    {
        std::cout << "No such file exists" << std::endl;
    }
    std::vector<Token> source;
    std::string buffer;
    char ch;
    bool flag = false;
    int line = 1;
    while (fin)
    {
        flag = false;
        fin.get(ch);
        if (ch == fin.eof())
        {
            break;
        }
        if ( ch == '\n')
        {
            line += 1;
            continue;
        }
        if (ch == ' ')
        {
            continue;
        }
       if (ch == '\"')
        {
            buffer.push_back(ch);
            fin.get(ch);
            while (ch != '\"')
            {
                buffer.push_back(ch);
                fin.get(ch);
            }
            buffer.push_back(ch);
            source.push_back(Token(buffer, "String", line));
            buffer.clear();
            continue;
        }
        if (flag == true)
        {
            continue;
        }
        if (IsOperator(ch) == true)
        {              
            if (ch == '+')
            {
                buffer.push_back(ch);
                fin.get(ch);
                if (ch == '+')
                {
                    buffer.push_back(ch);
                    source.push_back(Token(buffer, "Operator",line));
                    buffer.clear(); 
                    flag = true;
                }
                else if (ch == '=')
                {
                    buffer.push_back(ch);
                    source.push_back(Token(buffer, "Operator", line));
                    buffer.clear();
                    flag = true;
                }
                else if (ch == ' ')
                {
                    source.push_back(Token(buffer, "Operator", line));
                    buffer.clear();
                    flag = true;
                }
                else if (IsOperator(ch))
                {
                    source.push_back(Token(buffer, "Operator", line));
                    buffer.pop_back();
                    buffer.push_back(ch);
                    source.push_back(Token(buffer, "Operator", line));
                    buffer.clear();
                    flag = true;
                }
                else
                {
                    source.push_back(Token(buffer, "Operator", line));
                    buffer.pop_back();
                    buffer.push_back(ch);
                }
            }
            else
            {
                source.push_back(Token(ch, "Operator", line));
                flag = true;
            }
           
        }
        if (flag == true)
        {
            continue;
        }
        while(!flag)
        {
            buffer.push_back(ch);
            if (fin.eof())
            {   
                buffer.pop_back();
                source.push_back(Token(buffer, "Identifier", line));
                flag = true;
                break;
            }
            if (ch == '\n')
            {
            line += 1;
            buffer.pop_back();
            break;
            }
            else if (ch == ' ')
            {
            break;
            }
            else if (IsOperator(ch))
            {
                source.push_back(Token(buffer.substr(0,buffer.size()-1) ,"Identifier",line));
                source.push_back(Token(buffer.back(), "Operator",line));
                buffer.clear();
                flag = true;
                break;
            }
            else if (IsKeyword(buffer) == true)
            {
                source.push_back(Token(buffer,"Keyword",line));
                buffer.clear();
                flag = true;
                break;
            }
            fin.get(ch);
        }
        if (flag == false)
        {        
            source.push_back(Token(buffer, "Identifier",line));
            buffer.clear();
        }
    }
    fin.close();
    for (int i = 0; i < source.size(); i++)
    {
        source[i].Log();
    }
    std::cin.get();
    return 0;
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               