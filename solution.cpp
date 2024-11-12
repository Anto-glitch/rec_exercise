#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include "json.hpp"

using json = nlohmann::json;


//funkcja do sortowania kamieni według ich wartości od najcenniejszych  
bool comparison(std::pair<int, int> f, std::pair<int,int>s)
 {return f.second>s.second;}


//zamiana wyrazu z wagą kamieni na int (wagę w uncjach)
float change_units(std::string weight)
{
    float value=0;
    bool division=false;
    float pow=10;
    //jesli waga w karatach
    if(weight[weight.size()-1]=='t')
    {
        //obciecie znaków oznaczających jednostkę wagi
        weight=weight.substr(0, weight.size()-2);
        for(int i=0;i<weight.size();i++)
        {
            //string na float
            if(weight[i]==',') division=true;
            else if (!division)
            {
                value=10*value+int(weight[i]-'0');
            }
            else
            {
                value=value+float(weight[i]-'0')/pow;
                pow*=10;
            }
        }
        //zwraca wartosc w uncjach
        return value/5;
    }
    //jesli w gramach
    else
    {
        //obciecie znaków oznaczających jednostkę wagi
         weight=weight.substr(0, weight.size()-1);
        for(int i=0;i<weight.size();i++)
        {
            //string na float
            if(weight[i]==',') division=true;
            else if (!division)
            {
                value=10*value+int(weight[i]-'0');
            }
            else
            {
                value=value+float(weight[i]-'0')/pow;
                pow*=10;
            }
        }
        //zwraca wartosc w uncjach
        return value/28.34952;
    }

}


int main() {
    json whole_info, categories;
    
    // Wczytaj pierwszy plik
       std::ifstream data("dane/zbior_wejsciowy.json");  // niestety wskazuje na jakiś błąd w tym pliku kolejny plik wczytuje bez problemu
    if(!data.good()) std::cout<<"Cos jest nie tak z plikiem"<<std::endl;
    data>>whole_info;

    //wczytaj drugi plik(kategorie)
     std::ifstream data2("dane/kategorie.json");
    if(!data2.good()) std::cout<<"Cos jest nie tak z plikiem"<<std::endl;
    data2>>categories;


    std::vector<std::pair<int, float>> comparing_vec;

    std::string weight;
    float masa_unc;
    //iteracja po kazdym kamieniu i znajdywanie odpowiedniej kategorii
    for(int i=0;i<whole_info.size();i++)
    {
        weight= whole_info[i]["Masa"];
        masa_unc=change_units(weight);
       // std::cout<<masa_unc<<" "<<weight<<std::endl;
        int j=0;
        //znajduje kategorie
        while((whole_info[i]["Typ"]!=categories[j]["Typ"]||whole_info[i]["Czystość"]!=categories[j]["Czystość"])&&j<whole_info.size())  j++;
        if(j<whole_info.size())
        //vector przyjmuje indeks oraz wartosc kamienia
        {
            float val=categories[j]["Wartość za uncję (USD)"];
         //   std::cout<<val<<std::endl;
            val=val*masa_unc;
          //  std::cout<<val<<std::endl;
            comparing_vec.push_back(std::make_pair(i,val));
        }
    }
    //sortowanie według wartości kamieni
    sort(comparing_vec.begin(),comparing_vec.end(),comparison);

    //wypisanie pieciu najbardziej wartosciowych kamieni
    for(int i=0;i<5;i++)
    {
        std::cout<<std::setprecision(12)<<whole_info[comparing_vec[i].first]<<std::endl<<"Wartosc: "<<comparing_vec[i].second<<std::endl;
    }

}
