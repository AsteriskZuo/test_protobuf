//
//  test_protobuf.cpp
//  test_protobuf
//
//  Created by zuoyu on 2020/7/9.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#include "test_protobuf.hpp"
#include "IM.pb.h"

#include <iostream>

using namespace im::citylife::proto;

static void test1();
void test_protobuf() {
    test1();
}
static void test1() {
    ProtoIMMessageEx msgex;
    msgex.set_msg_uuid("123");
    ProtoIMMessageText *text = new ProtoIMMessageText();
    text->set_content("text_content");
    msgex.set_allocated_text(text);
    
    if (msgex.has_text()) {
        const ProtoIMMessageText& out_text = msgex.text();
        std::cout << out_text.content().c_str() << std::endl;
        msgex.clear_text();
    }
    
    ProtoIMMessageImage *image = new ProtoIMMessageImage();
    image->set_image_uuid("image_uuid");
    msgex.set_allocated_image(image);
    
    if (msgex.has_image()) {
        const ProtoIMMessageImage& out_image = msgex.image();
        std::cout << out_image.image_uuid().c_str() << std::endl;
        msgex.clear_image();
    }
}
