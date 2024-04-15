import math


#
#src/strategy/cpe_channel_package_regulation.cpp regulate channel target_goal_value:200 final_pegr:0.0209888 a:0.001 b:0.011852 c:0.01 ratio:1.69299 bid_result:0.0901877 ori_cpi:5.03732 hit_key:380071996_us_1_cpe_pid_online hit_ratio_key:CPE_70218_154620 last_channel_ratio:1.2

c = 0.9
i_pltv_iap = 1.0
iap_roas_egr_score = 0.000500729
d0_iaa_pltv = 0.272087
d0_iap_pltv = 0.0138885
goal = 60
ratio = 1.69299
beta = 0.95
a = 0.08
b = 0.00903
c = 0.96
tmp_mid = 1.0
last_channel_ratio = 99

if iap_roas_egr_score == 0.0:
    tmp_mid = c
else:
    tmp_mid = math.exp(math.log(a) + b - (b / iap_roas_egr_score)) + c

tmp_mid *= last_channel_ratio
bid_result1  = ratio * goal * tmp_mid * iap_roas_egr_score

#bid_result = ratio * (d0_iaa_pltv + beta * d0_iap_pltv * i_pltv_iap)/
print(bid_result1)

# formul 2
#regulate channel multi-dim target_goal_value:60 final_pegr:0.000500729 a:0.08 b:0.00903 c:0.96 ratio:0.801164 bid_result:2.28761 ori_cpi:0.0268239 hit_key:380089464_us_1_cpe_pid_online hit_ratio_key:CPE_70222_163264 value:1 muiti_hit_ratio_key
value = 1.0


# formul 3
#iaa_iap_channel_package_multi_dim_bid.cpp:181] regulate d0_iaa_pltv:0 d0_iap_pltv:0.295023 goal:0.32 ratio:0.6 beta:0.5 theta:0.130793 bid_result:6146.24 ori_cpi:0.553168 hit_key:380047429_all_1_roas_pid_v2 muiti_hit_ratio_key:ROAS_70219_rv_idfv_others value:22222 hit_ratio_key: last_channel_ratio:1

# iaa_iap_channel_package_multi_dim_bid.cpp:189] regulate d0_iaa_pltv:0 d0_iap_pltv:0.32797 goal:0.39 ratio:0.6 beta:0.5 theta:0.18717 bid_result:0.252284 ori_cpi:0.504569 hit_key:380177179_all_1_roas_pid_v2_exp muiti_hit_ratio_key: value:1 hit_ratio_key: last_channel_ratio:1
i = 0
d0_iap_pltv = 0.32797
d0_iaa_pltv = 0
theta = 0.18717
ratio = 0.6
beta = 0.5
target_goal_value = 0.39

if (d0_iap_pltv >= theta) :
        i  = 1
        
value = 1.0
tmp_mid = 1.0
tmp_mid = tmp_mid * value
last_channel_ratio = 1.0


bid_result = ratio * (d0_iaa_pltv + beta * d0_iap_pltv * i) / target_goal_value * tmp_mid


print("formul 3 tmp_mid {} bid_result {}".format(tmp_mid, bid_result))

# formul 4
#cpe_channel_package_multi_dim_bid.cpp:174] regulate channel multi-dim target_goal_value:15 final_pegr:0.103561 a:0.08 b:0.033503 c:0.96 ratio:3 bid_result:528087 ori_cpi:1.86409 hit_key:380124668_us_1_cpe_pid_online hit_ratio_key: value:111111 muiti_hit_ratio_key:CPE_70207_iv_idfa_others last_channel_ratio:1
c = 0.96
final_pegr = 0.103561
target_goal_value = 15
ratio = 3
a = 0.08
b = 0.033503
c = 0.96
tmp_mid = 1.0
last_channel_ratio = 1.0
value = 111111

if final_pegr == 0.0:
    tmp_mid = c
else:
    tmp_mid = math.exp(math.log(a) + b - (b / final_pegr)) + c
print(tmp_mid)
tmp_mid = tmp_mid * value * last_channel_ratio
bid_result1  = ratio * target_goal_value * tmp_mid * final_pegr

#bid_result = ratio * (d0_iaa_pltv + beta * d0_iap_pltv * i_pltv_iap)/

print("formul 4 tmp_mid {} bid_result {}".format(tmp_mid, bid_result1))
