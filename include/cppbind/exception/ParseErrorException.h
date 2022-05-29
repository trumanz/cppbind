//
// Created by zck on 2022/5/8.
//

#ifndef CPPBIND_TEST_PARSEERROREXCEPTION_H
#define CPPBIND_TEST_PARSEERROREXCEPTION_H



namespace  cppbind { namespace exception {


class ParseErrorException : public std::exception
{
 private:
     std::vector<std::string> node_info;
     std::string parse_error_msg;
     std::string error_str_buf;
     void buildErrorStr(){
         this->error_str_buf = "";
         for(size_t i = 0; i < this->node_info.size(); i++) {
             if(i > 0) error_str_buf += ".";
             error_str_buf += node_info[i];
         }
         error_str_buf += " " + this->parse_error_msg;
     }
 public:
     //~ParseErrorException() override = default;
     ParseErrorException() noexcept = default;
     ParseErrorException(std::string const& node_name, const std::string& err_msg) noexcept
     {
         this->node_info.insert(this->node_info.begin(), node_name);
         this->parse_error_msg = err_msg;
         this->buildErrorStr();
     }
    explicit ParseErrorException(const std::string& err_msg) noexcept
     {
         this->parse_error_msg = err_msg;
         this->buildErrorStr();
     }
     void addParentNodeName(std::string const& node_name) {
         this->node_info.insert(this->node_info.begin(), node_name);
         this->buildErrorStr();
     }

     const char* what() const noexcept override {
         return this->error_str_buf.c_str();
     }
 };



    } }

#endif //CPPBIND_TEST_PARSEERROREXCEPTION_H
