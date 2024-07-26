#include "gtest/gtest.h"

#include "class_template.h"

TEST(ClassTemplateTest, class_test_all) {
    CommonFileDict<int, int, std::map> obj_generalize("aaa");
    CommonFileDict<std::string, int, std::map> obj_all_special;
    //CommonFileDict<std::string, double, std::map> obj_all_generalize;
    // obj_all_generalize.function_print("aaaaaaa", 888888.0001);

}

TEST(ClassTemplateTest, class_test_partition) {
    
}






