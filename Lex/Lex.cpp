
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
    std::cout << m_line << " " << m_type << " : " << m_token << std::endl;
}

void Token::Push_back(char ch)
{
    m_token.push_back(ch);
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
    fin.open("test.txt", std::ios::in);
    std::vector<Token> source;
    std::string buffer;
    char ch;
    bool flag = false;
    int line = 1;
    while (!fin.eof())
    {
        flag = false;
        fin.get(ch);
        if ( ch == '\n')
        {
            line += 1;
            continue;
        }
        if (ch == ' ')
        {
            continue;
        }
        if (flag == true)
        {
            continue;
        }
        if (IsOperator(ch) == true)
        {          
                source.push_back(Token(ch, "Operator",line));
                flag = true;    
        }
        if (flag == true)
        {
            continue;
        }
        while(!flag)
        {
            buffer.push_back(ch);
            
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

    /*Second Pass*/
    std::vector<Token> source2;
    for (int i = 0; i < source.size(); i++)
    {
        if (IsOperator(source[i]))
        {
            if (source[i] == "+")
            {
                if (source[i + 1] == "+")
                {
                    source[i].Push_back('+');
                    source2.push_back(source[i]);
                    i += 1;
                }
                else
                {
                    source2.push_back(source[i]);
                }
            }
            else
            {
                source2.push_back(source[i]);
            }
        }
        else
        {
            source2.push_back(source[i]);
        }
    }
    for (int i = 0; i < source2.size(); i++)
    {
        source[i].Log();
    }
    return 0;
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               