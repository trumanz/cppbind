#ifndef __CPPBIND__JsonDecoder__
#define __CPPBIND__JsonDecoder__

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <assert.h>
#include <stdexcept>

#include <json/writer.h>
#include <json/json.h>
#include <boost/tti/has_member_function.hpp>
#include <boost/mpl/if.hpp>
#include <boost/shared_ptr.hpp>
#include "cppbind/dynamic/ClassRegisterBase.h"
#include "imp/Binder.hpp"

namespace  cppbind {

    /*!
       JsonDecoder for decode json to c++ object.
     */
    class JsonDecoder {

    public:
        JsonDecoderImp decoder;
    public:

        template<typename ObjT>
        void regTable(const std::map<std::string, ObjT *> *table) { decoder.regForeignTable(table); }

        template<typename ObjT, typename FTable>
        void regTable(boost::shared_ptr<FTable> ft) { decoder.regForeignTable<ObjT, FTable>(ft); }

        void regClassRegister(ClassRegisterBase *_class_reg) {
            decoder.regClassRegister(_class_reg);
        }

        void ignoreUnknownNode() { decoder.ignoreUnknownNode(); }

    public:
        /** Decode json object to C++ object
         * \param root - the json object needs to decode
         * \return object point to type T
        */
        template<typename T>
        std::unique_ptr<T> decode(const Json::Value &root) {
            auto e = std::make_unique<T>();
            decoder.decode(root, e.get());
            return e;
        }

        template<typename T>
        std::unique_ptr<T> decode(const char *json_str) {
            std::stringstream ss(json_str);
            return this->decode<T>(ss);
        }

        template<typename T>
        std::unique_ptr<T> decode(const std::string &json_str) {
            std::stringstream ss(json_str);
            return this->decode<T>(ss);
        }

        template<typename T>
        std::unique_ptr<T> decode(std::istream &is) { return decode<T>(decode(is)); }

        template<typename T>
        std::unique_ptr<T> decodeFile(const std::string &file_name) {
            try {
                return decode<T>(decodeFile(file_name));
            } catch (exception::ParseErrorException &e) {
                e.addParentNodeName(file_name);
                throw e;
            }
            return nullptr;
        }

        template<typename T>
        T *decodeFile(const std::string &file, const std::string &node_name) {
            Json::Value jv = decodeFile(file);
            try {
                if (!jv.isObject()) {
                    throw exception::ParseErrorException("not a object");
                }
                Json::Value jv_node = jv.get(node_name, Json::Value());
                return this->decode<T>(jv_node);
            } catch (exception::ParseErrorException &e) {
                e.addParentNodeName(file);
                throw e;
            }
            return nullptr;
        }

        Json::Value decode(std::istream &is) {
            Json::Value root;
            Json::Reader reader;
            bool parsingSuccessful = reader.parse(is, root);
            if (!parsingSuccessful) {
                std::string err_msg = reader.getFormattedErrorMessages();
                printf("Failed to parse, %s\n", err_msg.c_str());
                throw exception::ParseErrorException(err_msg);
            }
            return root;
        }

        Json::Value decodeFile(const std::string &file_name) {
            Json::Value jv;
            try {
                std::fstream fs(file_name.c_str(), std::fstream::in);
                if (!fs) {
                    throw exception::ParseErrorException("Can not open file [" + file_name + "]");
                }
                jv = this->decode(fs);
            } catch (exception::ParseErrorException &e) {
                e.addParentNodeName(file_name);
                throw e;
            } catch (std::runtime_error &e) {
                throw exception::ParseErrorException(file_name, e.what());
            }
            return jv;
        }

    };


}
#endif
