#include "abstract_factory/xiaomi_factory.h"
#include "abstract_factory/huawei_factory.h"
#include "abstract_factory/apple_factory.h"

#include "abstract_factory/apple_factory.h"
#include "abstract_factory/huawei_factory.h"
#include "abstract_factory/xiaomi_factory.h"

int main() {
    auto apple_factory = std::make_shared<AppleFactory>();
    auto apple_phone = apple_factory->create_phone();
    auto apple_tv = apple_factory->create_tv();
    auto apple_pc = apple_factory->create_pc();
    apple_phone->bell();
    apple_phone->call();

    apple_pc->down();
    apple_pc->work();

    apple_tv->play();
    apple_tv->open();


    auto huawei_factory = std::make_shared<HuaweiFactory>();
    auto huawei_phone = huawei_factory->create_phone();
    auto huawei_tv = huawei_factory->create_tv();
    auto huawei_pc = huawei_factory->create_pc();
    huawei_phone->bell();
    huawei_phone->call();
    huawei_pc->down();
    huawei_pc->work();
    huawei_tv->play();
    huawei_tv->open();

    auto xiaomi_factory = std::make_shared<XiaomiFactory>();
    auto xiaomi_phone = xiaomi_factory->create_phone();
    auto xiaomi_tv = xiaomi_factory->create_tv();
    auto xiaomi_pc = xiaomi_factory->create_pc();
    xiaomi_phone->bell();
    xiaomi_phone->call();
    xiaomi_pc->down();
    xiaomi_pc->work();
    xiaomi_tv->play();
    xiaomi_tv->open();
    

    return 0;
}