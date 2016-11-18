#include "imp/csvcpp.h"
#include <boost/algorithm/string.hpp>
namespace  cppbind {


CSVCPP::CSVCPP(){
    delimeters = ",";
    continues_delimter_as_one_delimter = false;
    line_break = "\n";
    strip_white_space = true;
}
std::vector<CSVRow> CSVCPP::get_rows(std::istream& csv_content) const {
    std::vector<CSVRow> rs;
    std::string  line;
    while(true) {
        if(csv_content.eof()) {
            break;
        }
        if(csv_content.fail()) {
            assert("TODO" == NULL);
        }
        //http://www.cplusplus.com/reference/string/string/getline/
        std::getline(csv_content, line);
        rs.push_back(get_row(line));
    }
    return rs;
}
CSVRow CSVCPP::get_row(const std::string &line) const{
    CSVRow row;
    //printf("====%s\n", line.c_str());
    std::vector<std::string> cells;
    std::string cell;
    std::string remains = line;
    while(!remains.empty()) {
       //printf("remains====%s\n", remains.c_str());
        size_t pos = remains.find_first_of(delimeters);
        if(pos != std::string::npos) {
            cell  = remains.substr(0, pos);
            remains = remains.substr(pos+1);
        } else{
            cell = remains;
            remains = "";
        }
        if(strip_white_space) {
            boost::trim(cell);
        }
        //printf("cell====[%s]\n", cell.c_str());
        row.cells.push_back(cell);
        
    }
    return row;
}

}
