#include <iostream>
#include <fstream>
#include <string>

#include "Order.pb.h"
int main()
{
    std::string sOrder;
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    Order order;
    order.set_time(14);
    order.set_userid(123);
    order.set_price(100.0f);
    order.set_desc("a test order");
    order.SerializeToString(&sOrder);

    Order r_order;
    if(r_order.ParseFromString(sOrder)) // ½âÎö¸Ã×Ö·û´®
    {
        std::cout << "userid:" << r_order.userid() << std::endl
                << "desc:" << r_order.desc() << std::endl;
    }
    else
    {
        std::cerr << "parse error!" << std::endl;
    }
    google::protobuf::ShutdownProtobufLibrary();
    return 0;

}





















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
