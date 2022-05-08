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
#include "ClassRegisterBase.h"
#include "imp/Binder.hpp"

namespace  cppbind {

    /*!
       JsonDecoder for decode json to c++ object.
     */
    class JsonDecoder {

    public:
        JsonDecoderImp decoder;
    public:
        //! JsonDecoder constructor
        JsonDecoder() {
        }

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
        T *decode(const Json::Value &root) {
            T *e = new T;
            decoder.decode(root, e);
            return e;
        }

        template<typename T>
        T *decode(const char *json_str) {
            std::stringstream ss(json_str);
            return this->decode<T>(ss);
        }

        template<typename T>
        T *decode(const std::string &json_str) {
            std::stringstream ss(json_str);
            return this->decode<T>(ss);
        }

        template<typename T>
        T *decode(std::istream &is) { return decode<T>(decode(is)); }

        template<typename T>
        T *decodeFile(const std::string &file_name) {
            try {
                return decode<T>(decodeFile(file_name));
            } catch (ParseErrorException &e) {
                e.addParentNodeName(file_name);
                throw e;
            }
            return NULL;
        }

        template<typename T>
        T *decodeFile(const std::string &file, const std::string &node_name) {
            Json::Value jv = decodeFile(file);
            try {
                if (!jv.isObject()) {
                    throw ParseErrorException("not a object");
                }
                Json::Value jv_node = jv.get(node_name, Json::Value());
                return this->decode<T>(jv_node);
            } catch (ParseErrorException &e) {
                e.addParentNodeName(file);
                throw e;
            }
            return NULL;
        }

        Json::Value decode(std::istream &is) {
            Json::Value root;
            Json::Reader reader;
            bool parsingSuccessful = reader.parse(is, root);
            if (!parsingSuccessful) {
                std::string err_msg = reader.getFormattedErrorMessages();
                printf("Failed to parse, %s\n", err_msg.c_str());
                throw ParseErrorException(err_msg);
            }
            return root;
        }

        Json::Value decodeFile(const std::string &file_name) {
            Json::Value jv;
            try {
                std::fstream fs(file_name.c_str(), std::fstream::in);
                if (!fs) {
                    throw ParseErrorException("Can not open file [" + file_name + "]");
                }
                jv = this->decode(fs);
            } catch (ParseErrorException &e) {
                e.addParentNodeName(file_name);
                throw e;
            } catch (std::runtime_error &e) {
                throw ParseErrorException(file_name, e.what());
            }
            return jv;
        }

    };


}
#endif
