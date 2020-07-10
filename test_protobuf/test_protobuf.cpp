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
#include <utility>

using namespace im::citylife::proto;

static void test1();
static void test2();
void test_protobuf() {
    test2();
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
    ProtoIMImage* img = image->add_images();
    img->set_image_uuid("img_uuid");
    msgex.set_allocated_image(image);
    
    if (msgex.has_conv()) {
        std::cout << "ok" << std::endl;
    }
    
    if (msgex.has_image()) {
        const ProtoIMMessageImage& out_image = msgex.image();
        std::cout << out_image.image_uuid().c_str() << std::endl;
        const ProtoIMImage &out_img = out_image.images(0);
        std::cout << out_img.image_uuid().c_str() << std::endl;
        msgex.clear_image();
    }
}

static void test2() {
    ProtoIMReturnResult result;
    result.set_code(1);
    ProtoIMMessage msg;
    msg.set_msg_type("CL:text");
    msg.set_msg_uuid("jsoie3fesef");
    auto msg_byte_size = msg.ByteSizeLong();
    char* msg_byte_data = (char*)malloc(msg_byte_size);
    bool ret = msg.SerializeToArray(msg_byte_data, msg_byte_size);
    if (ret) {
        std::string msg_param = std::string(msg_byte_data, msg_byte_size);
        google::protobuf::Map<std::string, std::string>* list = result.mutable_result();
        (*list)["msg"] = msg_param;
    }
    
    auto result_byte_size = result.ByteSizeLong();
    char* result_byte_data = (char*)malloc(result_byte_size);
    if (result.SerializeToArray(result_byte_data, result_byte_size)) {
        ProtoIMReturnResult out_result;
        if (out_result.ParseFromArray(result_byte_data, result_byte_size) ) {
            
            std::cout << out_result.code() << std::endl;
            
            const ::google::protobuf::Map< std::string, std::string >& out_list = out_result.result();
            auto iter = out_list.find("msg");
            if (iter != out_list.end()) {
                const std::string &out_msg_param = iter->second;
                
                ProtoIMMessage out_msg;
                if (out_msg.ParseFromArray(out_msg_param.data(), out_msg_param.size()) ) {
                    std::cout << out_msg.msg_uuid() << std::endl;
                    std::cout << out_msg.msg_type() << std::endl;
                }
                
            }
        }
        
    }
    
    
    
}
