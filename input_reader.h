#pragma once
#include "transport_catalogue.h"

namespace transport_system {

    class InputReader {
        public:
            explicit InputReader(TransportCatalogue& tc) : TC_(tc) {}
            std::vector<std::string> SplitStringToVector(const std::string& str, char splitter) const;
            detail::StopData SplitStringToPair(const std::string& str, char splitter) const;
            void ProcessData(std::istream& in);
            void ProcessRequest(std::istream& in);
        private:
            TransportCatalogue& TC_;
    };
    
}