#include<string>
#include<vector>
#include<iostream>
#include <sstream>
#include<fstream>
#include<algorithm>

using namespace std;

using relop = string;
using id = string;
using num = string;

//vector<string> operadores1 = {"<",">","==","(",")","{","}",";","="};
vector<string> operadores = {"<",">","==","<=",">="};
vector<char> numeros = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9','-'};
vector<int> cont_lineas;

int numero_linea = 0;

bool is_id( string item){return ((int) item[0]  >=  65 && (int) item[0] <=90 ) || ((int) item[0]>= 97 && (int) item[0]<=122 );}
bool is_relop(string item){return (find(begin(operadores), end(operadores), string(1,item[0]))) != operadores.end(); }
bool is_num( string item){return (find(begin(numeros), end(numeros), item[0])) != numeros.end(); }
//analisis gramatical
bool is_term(string item){return (is_id(item) || is_num(item));}


//En caso detecte un error ya no se terminará de analizar el archivo
template<typename T>
void error_lexico(T valor_no_reconocido){cout<<"\nError en la linea "<<numero_linea<<", caracter '"<<valor_no_reconocido<<"' no soportado"<<endl; exit(1);}

void error_gramatical(string valor_error,string valor_error1){cout<<"\nError gramatical: "<<valor_error<<" "<<valor_error1<<endl; exit(1);}

void dfa_relop(relop token_relop)
{
	string temp = "";
    for(int i = 0; i < token_relop.size(); i++){
		switch (token_relop[i])
		{
			case '<':
				temp += "<";
				break;
			case '>':
				temp += ">";
				break;
			case '{':
				temp += "{";
				break;
			case '}':
				temp += "}";
				break;
			case '(':
				temp += "(";
				break;
			case ')':
				temp += ")";
				break;
			case ';':
				temp += ";";
				break;
			case '=':
				if(i == 0 && token_relop == "=="){
					temp += "==";
					cout << "<REL_OP,'=='>" << endl;
				}
				else if(i == 0)
				{
					temp += "=";
				}
				else
					return;
				break;
			default:
				return;
				break;
		}
	}
	if(temp == token_relop){
		if(token_relop.size() == 1 && token_relop == "<")
			cout << "<REL_OP,'<'>" << endl;
		else if(token_relop.size() == 1 && token_relop == ">")
			cout << "<REL_OP,'>'>" << endl;
		else if(token_relop.size() == 1 && token_relop == "{")
			cout << "<BRACES_BEGIN,'{'>" << endl;
		else if(token_relop.size() == 1 && token_relop == "}")
			cout << "<BRACES_END,'}'>" << endl;
		else if(token_relop.size() == 1 && token_relop == "(")
			cout << "<PAR_BEGIN,'('>" << endl;
		else if(token_relop.size() == 1 && token_relop == ")")
			cout << "<PAR_END,')'>" << endl;
		else if(token_relop.size() == 1 && token_relop == "=")
			cout << "<ASSIGN,'='>" << endl;
		else if(token_relop.size() == 1 && token_relop == ";")
			cout << "<ENDOFLINE,';'>" << endl;
		else if(token_relop.size() == 2 && token_relop == "<=")
			cout << "<REL_OP,'<='>" << endl;
		else
			cout << "<REL_OP,'>='>" << endl;
	}
	else
		return;
}

void dfa_id(id token_id)
{   

    for (int i = 1 ; i < token_id.size(); i++)
    {
		if (!(((int) token_id[i]  >=  65 && (int) token_id[i] <=90) || ((int) token_id[i]>= 97 && (int) token_id[i]<=122)))
		{
      		if(!((find(begin(numeros), end(numeros), token_id[i])) !=numeros.end())){
				//cout << "Caracter o token no soportado: " << token_id << endl;
				error_lexico(token_id);
				return;
      		}
		}
	}
	cout << "<ID," << token_id << ">" << endl;
}

void dfa_num(num token_num)
{
	int cont_dot = 0;
	bool flag = false;
	vector<char> other = {'.', 'E', '+', '-'};
	if (token_num[token_num.size()-1] ==';'){
		token_num.pop_back();
		flag = true;
	}	
	for(int i = 1; i < token_num.size(); i++)
	{
		switch (token_num[i])
		{
		case '0'...'9':
			break;
		case '.':
			break;
		case 'E':
			break;
		case '+':
			break;
		case '-':
			break;
		default:
			return;
		}
	}
	cout << "<NUM," << token_num << ">" << endl;
	if(flag)
	{
		cout<<"<ENDOFLINE,';'>"<<endl;
	}
}

void analisis_lexico(vector<string> &vector_)
{
	int cont_elementos = 0;
	int i = 0;
    for (string item : vector_)
    {
		++cont_elementos;
		if (is_id(item))
		{
			dfa_id(item);
		} 
		else if (is_relop(item))
		{
			dfa_relop(item);	
		}
		else if (is_num(item))
		{ 
			dfa_num(item);
		}
		else 
		{
			numero_linea = ++i;
			error_lexico(item);
			continue;	
		}
		if(cont_elementos == cont_lineas[i]){
			i++;
			cont_elementos = 0;
		}
    }
}

// a > 4 >4 

//last_token 
void gramatica1(vector<string> &vector_, string value,string value1,int &i)
{
	cout<<"value: "<<value<<" value1: "<<value1<<" i: "<<i<<endl;
	if (value1 == value)
	{
		i++;
		gramatica1(vector_,vector_[i],vector_[i+1],i);
	}
	else if(is_relop(value1))
	{
		if(is_term(value))
		{
			i=i+2;
			gramatica1(vector_,vector_[i],vector_[i+1],i);
		}
		else{
			error_gramatical(value, value1);
		}
	}
	/*else
	{
		ERROR_GRAMATICAL1();
	}*/
}

void analisis_sintactico(vector<string> &vector_)
{
	//vector<vector<string>> gramatica;
	int i = 0;
	for( ; i < vector_.size()-1 ; i=i+1)
	{
		//gramatica1
		if((is_id(vector_[i]) || is_num(vector_[i])) && is_relop(vector_[i+1]))
		{
			cout<<"vector_[i]: "<<vector_[i]<<endl;
			cout<<"vector_[i+1]: "<<vector_[i+1]<<endl;
			gramatica1(vector_,vector_[i],vector_[i],i);
			cout<<"\n\n";
		}
	}
	/*
	for (string &item: vector_)
	{
		bool return_gramatica1 = gramatica1() 

		if (is_id(item))
		{
			
		}
	}*/
}

void read_data(fstream& file_object,string file_name)
{
	file_object.open(file_name);
	if(!file_object)
	{
		std::cout<<"Unable to open file\n";
		exit(1);
	}

	string input="";
	vector<string> codigo;
	int cont = 0;
	while(getline(file_object, input))
	{
        stringstream ss(input);
		string word;
		while(ss >> word){
			codigo.push_back(word);
			cont++;
		}
		cont_lineas.push_back(cont);
		cont = 0;
    }
	cout<<"INICIO ANALISIS LEXICO"<<endl;
	analisis_lexico(codigo);
	cout<<"\nINICIO ANALISIS SINTACTICO"<<endl;
	analisis_sintactico(codigo);
	cout<<'\n';
	file_object.close();
}
int main()
{
	fstream file_object;

	string file_name = "entrada1.txt";

    read_data(file_object,file_name);

    return 0;
}
