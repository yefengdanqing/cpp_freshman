//this is main_log.cpp

#include "glog/logging.h"   // glog 头文件

int main(){
    google::InitGoogleLogging("daqing");    //初始化一个log
    FLAGS_logtostderr = 1;  //输出到控制台

    LOG(INFO) << "info test";  //输出一个Info日志
    LOG(WARNING) << "warning test";  //输出一个Warning日志
    LOG(ERROR) << "error test";  //输出一个Error日志
    //LOG(FATAL) << "fatal test";  //输出一个Fatal日志，这是最严重的日志并且输出之后会中止程序,暂时注掉
    LOG(INFO)<<"info test lastone";
    google::ShutdownGoogleLogging();    //不用log的时候应该释放掉，不然会内存溢出

}