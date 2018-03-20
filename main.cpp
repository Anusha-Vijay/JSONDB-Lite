

#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

class Query{
    string key;
    string value;
    bool hasSubQuery;
public:
    Query(string k,string v,bool isList=false){
        if(isList){
            key=key;
            value=v;

        }
        else{
            key=k;
            value=v;
            if(value.find(':')!=string::npos)
                hasSubQuery=true;
            else
                hasSubQuery=false;
        }
    }

    const string &getKey(){
        return key;
    }
    const string &getValue(){
        return value;
    }
    const bool &getHasSubQuery(){
        return hasSubQuery;
    }

};

class stackNode{
public:
    int index;
    char brackets;
    stackNode(int index, char brackets) : index(index), brackets(brackets) {}
};

class jsonDB{
    vector<string> dataStore;
public:
    void addToDS(string inp){
        dataStore.push_back(inp);
    }

    vector<string> getAll(){
        return dataStore;
    }

    void print(vector<string> dataStore){
        for (auto line:dataStore)
            cout<<line<<endl;
    }
    void clearAll(){
        dataStore.clear();
    }


    string findValue(string input){
        //cout<<"\nRecived string to find input:"<<input<<endl;
        if(input.size()==0)
            return "";
        if(input.find('"')==string::npos){
            return input.substr(0,input.size()-1);
        }
        if(input.find('{')==string::npos){
            int start=input.find_first_not_of(":\"");
            int last;
            for(int i=start;i<input.size();i++){
                char c=input[i];
                if(input[i] == '"')
                {
                    last=i;
                    break;}

            }
            string val=input.substr(start,last-start);
            return val;
        }
        else{
            int countStart=0;
            int stopIndex=input.size();
            for(int i=0;i<input.size();i++){
                if(input[i]=='}'){
                    --countStart;
                    if(countStart==0){
                        stopIndex=i;
                        break;
                    }
                }
                if(input[i]=='{')
                    ++countStart;
            }
            string value=input.substr(1,stopIndex-1);
            //cout<<"\n Parssed Value is: "<<value<<endl;
            return value;
        }
    }

    map<string,bool> parseListValue(string input){
        map<string,bool> values;
        if(input=="[]" || input=="" || input=="[" || input=="]"){
            cerr<<"Invalid List numbers";
            return values;
        }
        string charecter="";
        for(int i=0;i<input.size();i++){
            if(input[i]=='[' || input[i]==']')
                continue;
            if(input[i]==','){
                values[charecter]= true;
                charecter="";
            }
            else{
                charecter=charecter+input[i];
            }
        }
        values[charecter]= true;
        return values;
    }

    string getKey(string keyString){
        int startIndex,stopIndex=-1;
        for(int i=0;i<keyString.size();i++){
            if(keyString[i]!='{'){
                startIndex=i;
                break;
            }
        }
        string key=keyString.substr(startIndex,keyString.size()-startIndex);
        return key;

    }



    string getObjectValue(string str){
        string value;
        int startIndex,stopIndex;
        stack<stackNode*> s1;
        stackNode* s;
        bool breakLoop= false;
        if(!str.size())
            return "";
        else{
            for(int i=0;i<str.size();i++){
                switch (str[i]){
                    case '{':
                        s=new stackNode(i,'{');
                        s1.push(s);
                        break;
                    case '[':
                        s=new stackNode(i,'[');
                        s1.push(s);
                        break;
                    case '}':
                        s=s1.top();
                        s1.pop();
                        if(s1.empty()){
                            breakLoop = true;
                            stopIndex=i;
                            startIndex=s->index;
                        }
                        break;
                    case ']':
                        s=s1.top();
                        s1.pop();
                        if(s1.empty()){
                            breakLoop = true;
                            stopIndex=i;
                            startIndex=s->index;
                        }
                        break;
                }
                if(breakLoop){
                    value=str.substr(startIndex,stopIndex-startIndex+1);
                    break;
                }
            }
        }
        return value;
    }

    string getValue(string str){
        if(!str.size())
            return "";
        else{
            string value;
            int stopIndex;
            int startIndex=str.find(':');
            if(str[startIndex+1]=='{' || str[startIndex+1]=='[' )
                value=getObjectValue(str);
            else{
                startIndex=startIndex+1;
                for(int i=startIndex;i<str.size();i++){
                    if(str[i]==',' || str[i]==']' || str[i]=='}'){
                        stopIndex=i;
                        break;
                    }
                }
                value=str.substr(startIndex,stopIndex-startIndex);
            }

            return value;
        }
    }


    string getValue(string key,string str){
        if(!str.size())
            return "";

        int startIndex=str.find(key)+key.size();
        if(startIndex!=string::npos){
            str=str.substr(startIndex,str.size()-startIndex);
        }
        string value=getValue(str);
        return value;
    }


    string removeString(string key,string input){
        int start=input.find(key);
        input=input.substr(start+key.size(),input.size()-(start+key.size()));
        //cout<<"\nRemaining string: "<<input;
        return input;
    }

    void getFilters(string filter,queue<Query*>& queryQueue){
        Query* q = nullptr;
        if(filter=="")
            return;
        int splitter=-1;
        splitter=filter.find(':');
        //no keys found
        if(splitter==string::npos)
            return;
        string possibleKey=filter.substr(0,splitter);
        //cout<<endl<<"Possible Key"<<possibleKey<<endl;
        string key=getKey(possibleKey);
        //cout<<"\nKey: "<<key;
        string removeStr=removeString(key,filter);
        string value=getValue(removeStr);
        //cout<<"\nValue:"<<value;
        q=new Query(key,value);
        queryQueue.push(q);
        removeStr=removeString(value+',',removeStr);
        getFilters(removeStr,queryQueue);
        return;
    }


    vector<pair<string,string>> containsInList(vector<pair<string,string>>& dataStore,string searchValues){

        cout<<"\n\nRecieved datastore:\n";
        for(auto x:dataStore){
            cout<<endl<<x.first<<"\t"<<x.second;
        }

        for(int i=0;i<dataStore.size();i++){
            string entryList=dataStore[i].first;
            if(entryList=="false" || dataStore[i].second=="false"){
                dataStore[i].second="false";
                dataStore[i].first="false";
                continue;
            }
            dataStore[i].second="true";
            string listValue=getValue("\"list\"",dataStore[i].first);
            map<string,bool> hmapEntry=parseListValue(listValue);
            map<string,bool> hmapSearch=parseListValue(searchValues);

            for(map<string,bool>::iterator it=hmapSearch.begin();it!=hmapSearch.end();*it++){
                if(hmapEntry.count(it->first)==0){
                    dataStore[i].second="false";
                    break;
                }
            }
        }
        return dataStore;


    };


    vector<pair<string,string>> performQuery(vector<pair<string,string>>& dataStore,queue<Query*> queryQueue){

        vector<pair<string,string>> queryResult;
        vector<pair<string,string>> newValueDataStore;
        Query* currentQuery;

        //if its empty, then to the data store, add true or false to the end query
        if(queryQueue.empty()){
            if(dataStore.size()==0)
                cerr<<"\n Datastore is empty!";


            for(int i=0;i<dataStore.size();i++){
                //if the key is false, result will be false
                if(dataStore[i].second=="false")
                    dataStore[i].second="false";
                else
                    dataStore[i].second="true";
            }
            return dataStore;
        }

        currentQuery=queryQueue.front();
        queryQueue.pop();

        //case 1: Where there is no subQuery
        if(!currentQuery->getHasSubQuery()) {

            if(currentQuery->getKey() == "\"list\""){

                dataStore=containsInList(dataStore,currentQuery->getValue());

            }
            else {
                //Geeting the results back.
                for (int i = 0; i < dataStore.size(); i++) {
                    if (dataStore[i].first.size() == 0 || dataStore[i].second == "false" ||
                        dataStore[i].first == "false")
                        dataStore[i].second = "false";
                    else {
                        string filterMatch = currentQuery->getKey() + ':' + currentQuery->getValue();

                        string value = getValue(currentQuery->getKey(), dataStore[i].first);

                        string keyValue = currentQuery->getKey() + ':' + value;

                        if (keyValue.find(filterMatch) != string::npos)
                            dataStore[i].second = "true";
                        else
                            dataStore[i].second = "false";

                    }
                }
            }
            queryResult = performQuery(dataStore,queryQueue);
            //cout<<"Need to map these results to datastore";
            //MAPING ORIGINAL DATASTORE TO THESE RESULTS
            for(int i=0;i<dataStore.size();i++){
                dataStore[i].second="false";
                if(queryResult[i].second=="true")
                    dataStore[i].second="true";
            }
            return dataStore;
        }

        //create a new dataStore from the existing dataStore which has the values of the key.
        //pass it to filterwrapper function
        //you will get the table with two columns

        else if(currentQuery->getHasSubQuery()) {

            //creating new datastore values
            for (int i = 0; i < dataStore.size(); i++) {
                pair<string, string> record;
                record.first = dataStore[i].first;
                //checking if the datastore's first is false, then add seciind as false;
                if (dataStore[i].first == "" || dataStore[i].first == "false"||dataStore[i].second == "false") {
                    record.second = "false";
                } else {
                    string value = getValue(currentQuery->getKey(), dataStore[i].first); //get value from the front
                    record.first = value;
                }
                newValueDataStore.push_back(record);
            }
            queryResult = filterWrapper(currentQuery->getValue(), newValueDataStore);  //teh data store wil be of size 1

            //MAPING ORIGINAL DATASTORE TO THESE RESULTS
            for (int i = 0; i < dataStore.size(); i++) {
                dataStore[i].second = "false";
                if (queryResult[i].second == "true")
                    dataStore[i].second = "true";
            }


            queryResult.clear();

            queryResult = performQuery(dataStore, queryQueue);

            //MAPING ORIGINAL DATASTORE TO THESE RESULTS
            for (int i = 0; i < dataStore.size(); i++) {
                dataStore[i].second = "false";
                if (queryResult[i].second == "true")
                    dataStore[i].second = "true";
            }
        }
        return dataStore;
    }


    vector<pair<string,string>> filterWrapper(string input,vector<pair<string,string>>& dataStore){
        queue<Query*> queryQueue;
        vector<pair<string,string>> queryAnswer;
        getFilters(input,queryQueue);
        if(!queryQueue.empty()){
            queryAnswer=performQuery(dataStore,queryQueue);
        }
        return queryAnswer;
    }

    void deleteFromDB(vector<string> queryResult){
        map<string,bool> hmap;
        for(int i=0;i<queryResult.size();i++){
            hmap[queryResult[i]]=true;
        }
        vector<string> newDataStore;
        for(int i=0;i<dataStore.size();i++){
            if(hmap.count(dataStore[i])==0)
                newDataStore.push_back(dataStore[i]);
        }
        dataStore.clear();
        dataStore=newDataStore;
    }
};




int main() {
    jsonDB db;
    fstream file;
    file.open("/Users/anushavijay/CLionProjects/HackerRanker/input.txt");
    while(!file.eof()){
        string line;
        getline (file,line);
        cout<<line<<endl;
        string command=line.substr(0,line.find(' '));
        string input=line.substr(line.find(' ')+1,(line.size()-command.size()-1));

        if(command == "add")
            db.addToDS(input);
        else if(command == "delete"){
            if(input=="{}"){
                db.clearAll();
            }
            else{
                //cout<<"\nDeleting line with Filter:"<<line;
                cout<<"\n----------------\n";
                vector<pair<string,string>> dataStore;
                vector<string> data=db.getAll();

                for(int i=0;i<data.size();i++){
                    pair<string,string> dataRecord;
                    dataRecord.first=data[i];
                    dataStore.push_back(dataRecord);
                }
                vector<pair<string,string>> queryResult = db.filterWrapper(input,dataStore);
                vector<string>querys;
                for (auto x:queryResult) {
                    if(x.second=="true")
                        querys.push_back(x.first);
                }

                db.deleteFromDB(querys);
            }
        }
        else{
            if (input == "{}"){
                db.print(db.getAll());
            }
            else{
                //cout<<"\n Getting with filters:"<<input;
                cout<<"\n----------------\n";
                vector<pair<string,string>> dataStore;
                vector<string> data=db.getAll();

                for(int i=0;i<data.size();i++){
                    pair<string,string> dataRecord;
                    dataRecord.first=data[i];
                    dataStore.push_back(dataRecord);
                }

                vector<pair<string,string>> queryResult = db.filterWrapper(input,dataStore);
                vector<string>querys;
                for (auto x:queryResult) {
                    if(x.second=="true")
                        querys.push_back(x.first);
                }
                db.print(querys);
//                for (auto x:queryResult) {
//                    if(x.second=="true")
//                    cout << endl << x.first;
//                }
            }
        }
    }

    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    return 0;
}