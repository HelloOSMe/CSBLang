/*
  CppSBLang Runtime program v1.0
  
  Copyright (C) 2022-2023 HelloOSMe
  All rights reserved.
 */
#include<bits/stdc++.h>
using namespace std;
bool runfile=false;
map<string,map<int,string>> hs;
map<string,string> strvar;
map<string,int> hasvar;//1:var;2:func;
ifstream fin;
string char_to_str(const char* ch){
	string s;
	for(int i=0;i<strlen(ch);i++){
		s.push_back(ch[i]);
	}
	return s;
}
bool str_allint(string s){
	for(int i=0;i<s.size();i++){
		if(s[i]>='0'&&s[i]<='9'){
			//all right
		}else{
			return false;
		}
	}
	return true;
}
int str_to_int(string s){
	int re=0;
	int i=0;
	bool mi=false;
	if(s[i]==0){
		mi=true;
		i++;
	}
	for(;i<s.size();i++){
		re*=10;re+=s[i]-'0';
	}
	if(mi){
		re=-re;
	}
	return re;
}
void err(string s,int line){
	cout<<"SBLang Error:Line:"<<line<<endl<<s<<endl;
	if(runfile){
		exit(-1);
	}
}
string getElement(string s,int num){
	int i=0;
	while(s[i]==' ')i++;
	int j=1;
	string rs="";
	bool str=false;
	for(;i<s.size();i++){
		if(s[i]==' '&&!str){
			if(j==num)return rs;
			rs.clear();
			j++;
		}else if(s[i]=='"'){
			str=!str;
		}else
			rs.push_back(s[i]);
	}
	return rs;
}
string getVar(string s,int num,int line){
	if(s[s.size()-1]==','){
		err("Bad var request\n"+s,line);
	}
	s+=",";
	string t="";
	int n=1;
	for(int i=0;i<s.size();i++){
		if(s[i]==','){
			if(n==num)return t;
			t.clear();
			n++;
		}else
			t.push_back(s[i]);
	}
	return "";
}
void runtime(string cmd,int line);
void finclude(string file,int line){
	ifstream finc (file.c_str());
	string cmd;
	bool bkurf=runfile;
	runfile=true;
	while(1){
		if(finc.eof()){
			runfile=bkurf;
			finc.close();
			return;
		}
		getline(finc,cmd);
		string cn=getElement(cmd,1);
		if(cn=="define"){
			if(hasvar[getElement(cmd,2)]!=0){
				err("Already have var or function:"+getElement(cmd,3),line);
			}else{
				map<int,string> func;
				int i=0;
				string tmp,tmp2="";
				bool su=true;
				while(1){
					if(finc.eof()){return;}
					getline(finc,tmp);
					if(tmp[0]!='d'||tmp[1]!=' '){
						err("^ Function grammar err.",line);
						su=false;
						break;
					}else if(getElement(tmp,1)=="d"&&getElement(tmp,2)=="end"){
						break;
					}
					for(int j=2;j<tmp.size();j++){
						tmp2.push_back(tmp[j]);
					}
					func[i]=tmp2;
					tmp2="";
					i++;
				}
				if(getElement(tmp,1)!="d"||getElement(tmp,2)!="end"){
					su=false;
				}
				if(!su){return;}
				hasvar[getElement(cmd,2)]=2;
				hs[getElement(cmd,2)]=func;
			}
		}else if(cn==""){
			//nocmd
		}else{
			runtime(cmd,line);
		}
	}
	runfile=bkurf;
	finc.close();
}
int main(int argc,char** argv){
	if(argc>1){
		runfile=true;
		fin.open(argv[1]);
		if(!fin.is_open()){
			err("Cannot open file.",0);
		}
	}
	if(!runfile){
	cout<<"CSBLang v1.0 developed by HelloOSMe.(SBLang runtime program)"<<endl;
	cout<<"SBLang Project Docs: https://github.com/Hiyoteam/SBLang"<<endl;
	}
	string cmd;
	int line=0;
	while(1){
		line++;
		if(!runfile){
			cout<<"[CSL>] ";
			getline(cin,cmd);
		}else{
			if(fin.eof()){exit(0);}
			getline(fin,cmd);
		}
		runtime(cmd,line);
	}
	return 0;
}
void runtime(string cmd,int line){
	string cn=getElement(cmd,1);
	if(cn[0]=='#'){
		return;
	}else if(cn=="new"){
		int i=1;
		while(getVar(getElement(cmd,2),i,line)!=""){
			if(hasvar[getVar(getElement(cmd,2),i,line)]!=0){
				err("Already have var or function:"+getElement(cmd,3),line);
			}
			strvar[getVar(getElement(cmd,2),i,line)]="";
			hasvar[getVar(getElement(cmd,2),i,line)]=1;
			i++;
		}
	}else if(cn=="string"){
		if(hasvar[getElement(cmd,2)]==1){
			strvar[getElement(cmd,2)]=getElement(cmd,3);
		}else{
			err("Bad var:"+getElement(cmd,2),line);
		}
	}else if(cn=="addkeep"){
		if(hasvar[getElement(cmd,3)]==1){
			string kp=getElement(cmd,2);
			if(kp=="newline"){
				strvar[getElement(cmd,3)]+="\n";
			}else{
				err("Bad keepword:"+getElement(cmd,2),line);
			}
		}else{
			err("Bad var:"+getElement(cmd,3),line);
		}
	}else if(cn=="appendvar"){
		if(hasvar[getElement(cmd,2)]==1||hasvar[getElement(cmd,3)]==1){
			strvar[getElement(cmd,2)]+=strvar[getElement(cmd,3)];
		}else{
			err("Bad var:"+(hasvar[getElement(cmd,2)]?getElement(cmd,3):getElement(cmd,2)),line);
		}
	}else if(cn=="out"){
		if(hasvar[getElement(cmd,2)]==1){
			cout<<strvar[getElement(cmd,2)];
		}else{
			if(getElement(cmd,2)!=""){
				cout<<getElement(cmd,2);
			}else{
				err("No parameter.",line);
			}
		}
		if(getElement(cmd,3)=="endline"){
			cout<<endl;
		}
	}else if(cn=="getchar"){
		string tmpvar;
		getline(cin,tmpvar);
		strvar[getElement(cmd,2)]=tmpvar;
	}else if(cn=="define"){
		if(hasvar[getElement(cmd,2)]!=0){
			err("Already have var or function:"+getElement(cmd,3),line);
		}else{
			map<int,string> func;
			int i=0;
			string tmp,tmp2="";
			bool su=true;
			while(1){
				if(!runfile){
					cout<<": ";
					getline(cin,tmp);
				}else{
					if(fin.eof()){exit(0);}
					getline(fin,tmp);
				}
				if(tmp[0]!='d'||tmp[1]!=' '){
					err("^ Function grammar err.",line);
					su=false;
					break;
				}else if(getElement(tmp,1)=="d"&&getElement(tmp,2)=="end"){
					break;
				}
				for(int j=2;j<tmp.size();j++){
					tmp2.push_back(tmp[j]);
				}
				func[i]=tmp2;
				tmp2="";
				i++;
			}
			if(getElement(tmp,1)!="d"||getElement(tmp,2)!="end"){
				su=false;
			}
			if(!su){return;}
			hasvar[getElement(cmd,2)]=2;
			hs[getElement(cmd,2)]=func;
		}
	}else if(cn=="call"){
		int i=0;
		if(hasvar[getElement(cmd,2)]!=2){
			err("Call bad function:"+getElement(cmd,2)+"[Not defined]",line);
		}
		string funcname=getElement(cmd,2);
		while(hs[funcname][i]!=""){
			runtime(hs[funcname][i],line);
			i++;
		}
	}else if(cn=="exit"){
		string exitcode=getElement(cmd,2);
		if(exitcode==""){
			exit(0);
		}else if(str_allint(exitcode)){
			exit(str_to_int(exitcode));
		}else{
			err("Bad exitcode:"+exitcode,line);
		}
	}else if(cn=="include"){
		string file=getElement(cmd,2);
		finclude(file,line);
	}else if(cn==""){
		//no command
	}else{
		err("Grammar mistake:no cmd:\n"+cn,line);
	}
}
/*
  todo list:
  
  include <file>
  shell <cmd>
  
  On Windows:
  WinAPI <funcation name> <parameter>
 */
