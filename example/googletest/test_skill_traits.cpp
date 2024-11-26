
#include "gtest/gtest.h"

TEST(SkillTrait, SkillTraitVector) {
    std::vector<int> datas{1, 2, 3, 4, 5, 6};
    datas.~vector();
    EXPECT_EQ(0, datas.size());
    new (&datas)::std::vector<int>{6};
    datas.push_back(1);
    EXPECT_EQ(7, datas.size());
    std::cout << datas.size() << "abcabc" << std::endl;


}