#include "stat_reader.h"

void transport_system::StatReader::FormAnswer(std::ostream& out) const
{
    for(const auto& request: requests_)
    {
        if(request[0]=='B')
        {
            ProcessBusRequest(out, request);
        }
        else
        {
            ProcessStopRequest(out, request.substr(5, request.size()-5));
        }
    }
}

void transport_system::StatReader::ProcessBusRequest(std::ostream& out, const std::string& request) const
{
    if(paths_.find(request)!=paths_.end())
    {
        const auto& v=paths_.at(request);
        std::set<std::string> s(v.begin(), v.end());

        auto stops_on_route=v.size()-1;
        auto unique_stops=s.size()-1;

        double distance=0;
        double real_distance=0;
        bool is_round_path=">"==v[v.size()-1]?true:false;

        for(int i=1; i<v.size()-1; i++)
        {
            const auto& stop1=stops_.at(v[i]);
            const auto& stop2=stops_.at(v[i-1]);
            distance+=detail::ComputeDistance(detail::Coordinates{stop1.coords.lat, stop1.coords.lng}, detail::Coordinates{stop2.coords.lat, stop2.coords.lng});
            if(is_round_path)
            {
                try
                {
                    real_distance+=stop2.connections.at(v[i]);
                }
                catch(const std::out_of_range& e)
                {
                    real_distance+=stop1.connections.at(v[i-1]);
                }
            }
            else
            {
                try
                {
                    real_distance+=stop2.connections.at(v[i]);
                }
                catch(const std::out_of_range& e)
                {
                    real_distance+=stop1.connections.at(v[i-1]);
                }
                try
                {
                    real_distance+=stop1.connections.at(v[i-1]);
                }
                catch(const std::out_of_range& e)
                {
                    real_distance+=stop2.connections.at(v[i]);
                }
            }
        }

        if(!is_round_path)
        {
            distance*=2;
            stops_on_route=stops_on_route*2-1;
        }

        double curve=real_distance/distance;

        out.precision(6);
        out<<request<<": "<<stops_on_route<<" stops on route, "<<unique_stops<<" unique stops, "<<real_distance<<" route length, "<<curve<<" curvature"<<std::endl;
    }
    else
    {
        out<<request<<": not found"<<std::endl;
    }
}

void transport_system::StatReader::ProcessStopRequest(std::ostream& out, const std::string& request) const
{    
    if(BTS_.find(request)!=BTS_.end())
    {
        const auto& item=BTS_.at(request);

        if(item.size()==0)
        {
            out<<"Stop "<<request<<": no buses"<<std::endl;
        }
        else
        {
            std::vector<std::string> temp(item.begin(), item.end());
            std::sort(temp.begin(), temp.end());

            out<<"Stop "<<request<<": buses ";

            for(const auto& t: temp)
            {
                std::cout<<t.substr(4, t.size()-4)<<' ';
            }

            out<<std::endl;
        }
    }
    else
    {
        out<<"Stop "<<request<<": not found"<<std::endl;
    }
}