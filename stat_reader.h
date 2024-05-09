#pragma once
#include "transport_catalogue.h"

namespace transport_system {
    
    class StatReader {
        public:
            explicit StatReader(const TransportCatalogue& tc) : TC_(tc) {}
            void FormAnswer(std::ostream& out) const;
            void ProcessBusRequest(std::ostream& out, const std::string& request) const;
            void ProcessStopRequest(std::ostream& out, const std::string& request) const;
        private:
            const TransportCatalogue& TC_;
            const std::map<std::string, std::vector<std::string>>& paths_=TC_.GivePathDataReadAccess();
            const std::map<std::string, detail::StopData>& stops_=TC_.GiveStopDataReadAccess();
            const std::vector<std::string>& requests_=TC_.GiveRequestDataReadAccess();
            const std::map<std::string, std::set<std::string>>& BTS_=TC_.GiveBusThroughStopDataReadAccess();
    };
    
}