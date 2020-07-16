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
#include <thread>
#include <chrono>

using namespace im::citylife::proto;

static void test1();
static void test2();
static void test3();
static void test4();
static void test5();
void test_protobuf() {
    test5();
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

static void test3() {
    ProtoIMReturnResult result;
    auto size = result.ByteSizeLong();
    char* data = (char*)std::malloc(size);
    bool ret = result.SerializeToArray(data, size);
    int a = ret;
}

class shared_object {
public:
    int _var;
    static int _s_count;
public:
    void changed1(int tid, int var) {
        static int local_var = var;
        if (1 == tid) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            ++local_var;
            std::cout << __func__ << ":" << local_var << ":addr:" << &local_var << ":addr2:" << (int*)&var << std::endl;
        }
        if (2 == tid) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            local_var += 2;
            std::cout << __func__ << ":" << local_var << ":addr:" << &local_var << ":addr2:" << (int*)&var << std::endl;
        }
    }
    void changed3(int tid, int var) {
//        _var = var;
        if (1 == tid) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            ++_var;
            std::cout << __func__ << ":" << _var << ":addr:" << &_var << ":addr2:" << (int*)&var << std::endl;
        }
        if (2 == tid) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            _var += 2;
            std::cout << __func__ << ":" << _var << ":addr:" << &_var << ":addr2:" << (int*)&var << std::endl;
        }
    }
    void changed2(int tid, int var) {
        int local_var = var;
        if (1 == tid) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            ++local_var;
            std::cout << __func__ << ":" << local_var << ":addr:" << &local_var << ":addr2:" << (int*)&var << std::endl;
        }
        if (2 == tid) {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            local_var += 2;
            std::cout << __func__ << ":" << local_var << ":addr:" << &local_var << ":addr2:" << (int*)&var << std::endl;
        }
        
    }
};
int shared_object::_s_count = 0;
/**
 * 测试局部变量是否属于线程的地址空间
 * 方法内的局部变量是在线程自己的空间，并不会相互影响！
 */
static void test4() {
    if (true) {
        //线程内的局部变量
        shared_object obj;
        obj._var = 9;
        std::thread t1(&shared_object::changed2, &obj, 1, 8);
        std::this_thread::sleep_for(std::chrono::microseconds(500));
        std::thread t2(&shared_object::changed2, &obj, 2, 8);
        t1.join();
        t2.join();
    }
    if (true) {
        //对象的成员变量
        shared_object obj;
        obj._var = 9;
        std::thread t1(&shared_object::changed3, &obj, 1, 8);
        std::this_thread::sleep_for(std::chrono::microseconds(500));
        std::thread t2(&shared_object::changed3, &obj, 2, 8);
        t1.join();
        t2.join();
    }
    if (true) {
        //方法内的静态变量
        shared_object obj;
        obj._var = 9;
        std::thread t1(&shared_object::changed1, &obj, 1, 8);
        std::this_thread::sleep_for(std::chrono::microseconds(500));
        std::thread t2(&shared_object::changed1, &obj, 2, 8);
        t1.join();
        t2.join();
    }
}

class BaseClass {
public:
    BaseClass() {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
    virtual ~BaseClass() {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
};
class DerivedClass : public BaseClass {
public:
    DerivedClass() {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
    ~DerivedClass() {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
};
class BaseClass2 {
public:
    BaseClass2() {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
    ~BaseClass2() {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
};
class DerivedClass2 : public BaseClass2 {
public:
    DerivedClass2() {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
    ~DerivedClass2() {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
};

/**
 * 虚析构的作用
 * 虚析构是可以调用子类的析构函数的
 */
static void test5() {
    BaseClass *sub_obj = new DerivedClass();
    delete sub_obj;
    BaseClass2 *sub_obj2 = new DerivedClass2();
    delete sub_obj2;
}

