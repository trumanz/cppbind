#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "gtest/gtest.h"
#include "boost/date_time/posix_time/posix_time.hpp" 
#include "type/strtime.h"

using namespace cppbind;

TEST(boost_datetime, posix_time_parse_format) {

   using namespace boost::posix_time;
   //ptime pt_now = second_clock::local_time();
   ptime pt;
   std::string str;
   int ret;
    
   StrTime tf("%Y-%m-%d %H:%M:%S");

   //wrong month 13
   ret = tf.parser("2004-13-21 12:45:33", &pt);
   EXPECT_EQ(-1, ret);

   //time 25hour will increase day
   ret = tf.parser("2004-03-21 25:45:33", &pt);
   EXPECT_EQ(0, ret);
   str =  tf.format(pt);
   EXPECT_EQ("2004-03-22 01:45:33", str);

   //miss seconds, will be zero
   ret = tf.parser("2004-03-21 25:45", &pt);
   str =  tf.format(pt);
   EXPECT_EQ(0, ret);
   EXPECT_EQ("2004-03-22 01:45:00", str);

   //wrorng format
   ret = tf.parser("2004--03-21 12:45:33", &pt);
   EXPECT_EQ(-1, ret);

   ret = tf.parser("2004-03-21 12:45:33", &pt);
   EXPECT_EQ(0, ret);

   str =  tf.format(pt);
   EXPECT_EQ("2004-03-21 12:45:33", str);
    

}

