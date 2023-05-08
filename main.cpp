#include <iostream>
#include <cstring>
#include <vector>
#include <cctype>
#include <algorithm>
#include <sstream>

class Policy
{
protected:
    bool isChecked;
public:
    virtual void check(const std::string& a)=0;
    bool getCheck() const{
        return isChecked;
    }
};

class LengthPolicy:public Policy
{
private:
    unsigned short int minLength;
    unsigned short int maxLength;
public:
    LengthPolicy(unsigned short int a){
        minLength=a;
        maxLength=255;
    }
    LengthPolicy(unsigned short int a, unsigned short b){
        minLength=a;
        maxLength=b;
    }
    void check(const std::string& a){
        if(a.size()<=maxLength&&a.size()>=minLength)
        {
            isChecked=true;
        }
        else
        {
            isChecked=false;
        }
    }
};

class ClassPolicy:public Policy
{
private:
    unsigned short int minClassCount;
public:
    ClassPolicy(unsigned short int a){
        minClassCount=a;
    }
    void check(const std::string& pw){
        int lm=0,lM=0,cif=0,alcar=0;
        for(int i=0;i<pw.size();i++)
            if(pw[i]>='a'&&pw[i]<='z')
            {
                lm++;break;
            }
        for(int i=0;i<pw.size();i++)
            if(pw[i]>='A'&&pw[i]<='Z')
            {
                lM++;break;
            }
        for(int i=0;i<pw.size();i++)
            if(pw[i]>='0'&&pw[i]<='9')
            {
                cif++;break;
            }
        for(int i=0;i<pw.size();i++)
            if(pw[i]>=32&&pw[i]<=64&&isdigit(pw[i])==0)
            {
                alcar++;break;
            }
        if(lm+lM+cif+alcar>=minClassCount)
        {
            isChecked=true;
        }
        else
        {
            isChecked=false;
        }
    }
};

class IncludePolicy:public Policy
{
private:
    char characterType;
public:
    IncludePolicy(char c)
    {
        characterType=c;
    }
    void check(const std::string& pw)
    {
        isChecked=false;
        for(unsigned int i=0;i<pw.size();i++)
        {
            if(pw[i]>='a'&&pw[i]<='z'&&characterType=='a')
            {
                isChecked=true;
                break;
            }
            if(pw[i]>='A'&&pw[i]<='Z'&&characterType=='A')
            {
                isChecked=true;
                break;
            }
            if(pw[i]>='0'&&pw[i]<='9'&&characterType=='0')
            {
                isChecked=true;
                break;
            }
            if(pw[i]>=32&&pw[i]<=64&& std::isdigit(pw[i])==0&&characterType=='$')
            {
                isChecked=true;
                break;
            }
        }
    }
};

class NotIncludePolicy:public Policy
{
private:
    char characterType;
public:
    NotIncludePolicy(char c)
    {
        characterType=c;
    }
    void check(const std::string& pw)
    {
        isChecked=true;
        for(int i=0;i<pw.size();i++)
        {
            if(pw[i]>='a'&&pw[i]<='z'&&characterType=='a')
            {
                isChecked=false;break;
            }
            if(pw[i]>='A'&&pw[i]<='Z'&&characterType=='A')
            {
                isChecked=false;break;
            }
            if(pw[i]>='0'&&pw[i]<='9'&&characterType=='0')
            {
                isChecked=false;break;
            }
            if(pw[i]>=32&&pw[i]<=64&&isdigit(pw[i])==0&&characterType=='$')
            {
                isChecked=false;break;
            }
        }
    }
};

class RepetitionPolicy:public Policy
{
private:
    unsigned short int maxCount;
public:
    RepetitionPolicy(unsigned short a){
        maxCount=a;
    }
    void check(const std::string& pw)
    {
        isChecked=true;
        for(int i=0;i<pw.size();i++)
        {
            int a=1;
            for(int j=0;j<pw.size();j++)
            {
                if(pw[i]==pw[j]&&i!=j)
                {
                    a++;
                }
            }
            if(a>maxCount)
            {
                isChecked=false;break;
            }
        }
    }
};

class ConsecutivePolicy:public Policy
{
private:
    unsigned short int maxCount;
public:
    ConsecutivePolicy(unsigned short a){
        maxCount=a;
    }
    void check(const std::string& pw)
    {
        int a=0,cs=1;
        for(int i=0;i<pw.size()-1;i++)
        {
            if(pw[i+1]-pw[i]==1)
            {
                cs=1;
                while(i<pw.size()-1&&pw[i+1]-pw[i]==1)
                {
                    cs++;
                    i++;
                }
            }
            if(cs>a)
            {
                a=cs;
            }
        }
        if(a>maxCount)
        {
            isChecked = false;
        }
        else
        {
            isChecked = true;
        }
    }
};

std::string checkPassword(std::string pw,std::vector<Policy*> v)
{
    std::string b="NOK";
    std::string a="OK";
    bool k=true;
    for(auto &policy:v)
    {
        policy->check(pw);
        k=k&&(policy->getCheck());
    }
    if(k)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int main()
{
    int n;
    std::string pw,rez="";
    std::string cmd[3];
    std::string l;
    char* c;
    std::vector<Policy*> v;
    scanf("%d ", &n);
    for(int i=0;i<n;i++)
    {
        getline(std::cin,l);
        int ltype=0;
        c=strtok(&l[0]," ");
        while(c){
            cmd[ltype++]=c;
            c=strtok(nullptr, " ");
        }
        if (cmd[0]=="length"&&ltype==2){
            v.push_back(new LengthPolicy(stoi(cmd[1])));
        }
        if (cmd[0]=="length"&&ltype==3){
            v.push_back(new LengthPolicy(stoi(cmd[1]), stoi(cmd[2])));
        }
        if (cmd[0]=="class"){
            v.push_back(new ClassPolicy(stoi(cmd[1])));
        }
        if (cmd[0]=="include"){
            v.push_back(new IncludePolicy(cmd[1][0]));
        }
        if (cmd[0]=="ninclude"){
            v.push_back(new NotIncludePolicy(cmd[1][0]));
        }
        if (cmd[0]=="repetition"){
            v.push_back(new RepetitionPolicy(stoi(cmd[1])));
        }
        if (cmd[0]=="consecutive"){
            v.push_back(new ConsecutivePolicy(stoi(cmd[1])));
        }
    }
    while(std::cin>>pw)
    rez+=checkPassword(pw,v)+"\n";
    std::cout<<rez;

    return 0;
}
