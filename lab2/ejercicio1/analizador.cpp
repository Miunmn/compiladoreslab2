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

vector<string> operadores = {"<",">","==","(",")","{","}",";"};
vector<char> numeros = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

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
			case '=':
				if(i == 0 && token_relop == "=="){
					temp += "==";
					cout << "<REL_OP,'=='>" << endl;
				}
				else if(i == 1)
					temp += "=";
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
				cout << "Caracter o token no soportado: " << token_id << endl;
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
	for(int i = 1; i < token_num.size(); i++)
	{
		if(!((find(begin(numeros), end(numeros), token_num[i])) != (numeros.end())))
		{
			return;
		} else if(!((find(begin(other), end(other), token_num[i])) != ( other.end())))
		{
			return;
		} else if(token_num[i] == '.' && cont_dot == 0)
		{
			cont_dot++;
		} else if((token_num[i] == '+' || token_num[i] == '-') && flag == false)
		{
			return;
		} else if(token_num[i] == 'E')
		{
			flag = true;
		}
		
		if(cont_dot > 1)
			return;
	}

	cout << "<NUM," << token_num << ">" << endl;
}

void analisis(vector<string> &vector_)
{
    for (string item : vector_)
    {
		if (((int) item[0]  >=  65 && (int) item[0] <=90 ) || ((int) item[0]>= 97 && (int) item[0]<=122 ))
		{
			dfa_id(item);
		} 
		else if ((find(begin(operadores), end(operadores), string(1,item[0]))) !=operadores.end())
		{
			dfa_relop(item);	
		}
		else if ((find(begin(numeros), end(numeros), item[0])) != numeros.end())
		{
			dfa_num(item);			
		}
		else 
		{
			cout<<"Caracter o token no soportado: " << item << endl;
			continue;	
		}	
		
    }
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

	while(getline(file_object, input))
	{
        stringstream ss(input);
		string word;
		while(ss >> word)
			codigo.push_back(word);
    }
	analisis(codigo);
	file_object.close();
}
int main()
{
	fstream file_object;

	string file_name = "entrada1.txt";

    read_data(file_object,file_name);

    return 0;
}
