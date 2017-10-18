#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

//funkcja xorująca dwa stringi
string xorstrings(string a,string b){
    string s = "";
    for(int i = 0;i<a.length() && i<b.length();i++){
        s+=a.at(i)^b.at(i);
    }
    return s;
}
bool isInside(const std::string & str, char c)
{
    return str.find(c) != std::string::npos;
}
//funkcja sprawdzająca dla każdej pozycji, który znak ascii najlepiej rozszyfrowuje znaki
string find_crypto(vector<string>kryptogramy,int position,char last,char prelast){
    if(position<iterations){
        string validchars = " ABCDEFGHIJKLMNOPRSTUWYZabcdefghijklmnoprstuwyz.?";
        /*int weight[] = {1000,1891,142,396,325,766,30,142,108,821,228,351,392,280,552,850,313,469,499,398,1250,465,376,1564,891,142,396,325,766,30,142,108,821,228,351,392,280,552,850,313,469,499,398,250,465,376,564,500,800};*/
        char mostvalidchar = 0x00;
        int mostvalids = 0;
        for(char i = -128;i<127;i++){
            int valids = 0;
            for(int j = 0;j<kryptogramy.size();j++){
                if(kryptogramy.at(j).length()>position){
                char xored = kryptogramy.at(j).at(position)^i;
                char xoredl = 0x00;
                char xoredpl = 0x00;
                if(position>0)
                    xoredl = kryptogramy.at(j).at(position-1)^last;
                if(position>1)
                    xoredpl = kryptogramy.at(j).at(position-2)^prelast;
                if(((xored==0x1F && position==0) || (xored>='A' && xored<='Z' && xoredl==' ' && xoredpl=='.') || (xored>='a' && xored<='z') || (xored==' ' && ((xoredl>='a' && xoredl<='z') || xoredl=='.'))) && isInside(validchars,xored)){
                    int pos = validchars.find(xored);
                    valids+=1;//weight[pos];
                }
                }
            }
            if(valids>mostvalids){
                mostvalidchar = i;
                mostvalids = valids;
            }
        }
        cout<<mostvalids<<" ";
        return mostvalidchar+find_crypto(kryptogramy,position+1,mostvalidchar,last);
    }
    return "";
}
int main(int argc, char* argv[]){
    if(argc<2){
        cout<<"Podaj nazwę kryptogramu!"<<endl;
    } else {
        
        vector<string> kryptogramy;
        string filename = string(argv[1]);
        ifstream f;
        f.open(filename.c_str());
        if(f.is_open()){
            string line;
            //program sprawdza plik linia po linii i zamienia linie ze znakami zerojedynkowymi na obiekty typu string zawierające bajty
            while(getline(f,line)){
                if(line.length()>0 && (line.at(0)=='0' || line.at(0)=='1')){
                    string kryptogram = "";
                    char bit = 0;
                    char asciiwartosc = '\0';
                    
                    for(int i = 0;i<line.length();i++){
                        if(line.at(i)=='0' || line.at(i)=='1'){
                            if(line.at(i)=='1'){
                                asciiwartosc|=0x01;
                            }
                            if(bit==7){
                                kryptogram+=asciiwartosc;
                                bit = 0;
                                asciiwartosc = '\0';
                            } else {
                                bit++;
                                asciiwartosc = asciiwartosc<<1;
                            }
                                
                        }
                    }
                    kryptogramy.push_back(kryptogram);
                }
            }
            
            //wywołanie rekurancyjnej funkcji
            string krypto = find_crypto(kryptogramy,0,' ','.');
            cout<<endl;
            for(int i = 0;i<kryptogramy.size();i++){
                cout<<"Kryptogram "<<i+1<<":"<<endl;
                cout<<xorstrings(krypto,kryptogramy.at(i))<<endl;
            }
            f.close();
        }
    }
    return 0;
}
