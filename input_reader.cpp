#include "input_reader.h"

std::vector<std::string> transport_system::InputReader::SplitStringToVector(const std::string& str, char splitter) const{
    std::vector<std::string> result;
    std::string current = ""; 
    for(int i = 0; i < str.size(); i++){
        if(str[i] == splitter){
            if(current != ""){
                result.push_back(current.substr(1, current.size()-2));
                current = "";
            } 
            continue;
        }
        current += str[i];
    }
    if(current.size() != 0)
    {
        result.push_back(current.substr(1, current.size()-1));
    }
    return result;
}

transport_system::detail::StopData transport_system::InputReader::SplitStringToPair(const std::string& str, char splitter) const{
    int count=0;
    std::string current = ""; 
    transport_system::detail::StopData result;
    for(int i = 0; i < str.size(); i++)
    {
        if(str[i] == splitter)
        {
            if(current!=""&&count<2)
            {
                if(count==0)
                    result.coords.lat=stod(current);
                else
                    result.coords.lng=stod(current);
                current = "";
                count+=1;
            }
            if(current!=""&&count>=2)
            {
                auto position=current.find(" to ");
                auto key=current.substr(position+4, current.size()-(position+4));
                auto value=current.substr(1, position-1);
                result.connections[key]=stod(value.substr(0, value.size()-1));
                current = "";
            }
            continue;
        }
        current += str[i];
    }
    if(current!=""&&count<2)
    {
        result.coords.lng=stod(current);
    }
    if(current!=""&&count>=2)
    {
        auto position=current.find(" to ");
        auto key=current.substr(position+4, current.size()-(position+4));
        auto value=current.substr(1, position-1);
        result.connections[key]=stod(value.substr(0, value.size()-1));
    }
        
    return result;
}

void transport_system::InputReader::ProcessData(std::istream& in)
{
    std::string str;
    std::getline(in, str);
    auto x=str.find(':');
    std::string fp=str.substr(0, x);
    std::string lp=str.substr(x+1, str.size()-x);
    if(fp[0]=='S')
    {
        auto result=SplitStringToPair(lp, ',');
        TC_.PushStopData(fp.substr(5, fp.size()-5), result);
        TC_.PushBusThroughStopData(fp.substr(5, fp.size()-5));
    }
    else
    {
        if(str.find('-')!=std::string::npos)
        {
            std::vector<std::string> r=SplitStringToVector(lp, '-');
            r.push_back("-");
            TC_.PushPathData(fp, r);
            for(int i=0; i<r.size()-1; i++)
                TC_.PushBusThroughStopData(r[i], fp);
        }
        else
        {
            std::vector<std::string> r=SplitStringToVector(lp, '>');
            r.push_back(">");
            TC_.PushPathData(fp, r);
            for(int i=0; i<r.size()-1; i++)
                TC_.PushBusThroughStopData(r[i], fp);
        }
    }
}

void transport_system::InputReader::ProcessRequest(std::istream& in)
{
    std::string str; 
    std::getline(in, str);
    TC_.PushRequestData(str);
}