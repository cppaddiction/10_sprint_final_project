#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

using namespace std;
using namespace transport_system;

int main()
{
    TransportCatalogue catalogue;
    InputReader i_reader(catalogue);
    StatReader s_reader(catalogue);
    
    int x;
    (cin>>x).get();
    
    for(int i=0; i<x; i++)
    {
        i_reader.ProcessData(cin);
    }
    
    (cin>>x).get();
    
    for(int i=0; i<x; i++)
    {
        i_reader.ProcessRequest(cin);
    }

    s_reader.FormAnswer(cout);
    
    return 0;
}