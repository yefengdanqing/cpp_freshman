import math


#regulate target_goal_value:40 final_pegr:0.0481095 a:0.55 b:0.015 c:0.8 ratio:0.7035 bid_result:1.63642 ori_cpi:1.92438 hit_key:379780743_us_1_cpe_pid_online
#auto tmp_mid = std::exp(std::log(cpe_info.a) + cpe_info.b - (cpe_info.b / final_pegr));
#bid_result = cpe_info.ratio * target_goal_value * (tmp_mid + cpe_info.c) * final_pegr;
x = 0.55
y = math.log(x)
z = math.exp(y+0.015 - 0.015/0.0481095)
print(z)
result = 0.7035 * 40 * (z + 0.8) * 0.0481095
print(result)

#regulate d0_iaa_pltv:0.334198 d0_iap_pltv:0.0182744 goal:0.5 ratio:0.64 beta:0.95 a:0.2 b:0.005 c:0.9 bid_result:0.450292 ori_cpi:0.845933 hit_key:379835752_all_1_roas_pid_v2
#d0_iaa_pltv:0.213313 d0_iap_pltv:0.0314182 goal:0.5 ratio:0.64 beta:0.95 a:0.2 b:0.005 c:0.9 bid_result:0.307425 ori_cpi:0.587355
#iap_roas_egr_score:0.00663569 d0_iaa_pltv:0.272087 d0_iap_pltv:0.0138885 goal:0.5 ratio:0.64 beta:0.95 a:0.2 b:0.005 c:0.9 bid_result:0.365069 ori_cpi:0.686341 hit_key:379835752_all_1_roas_pid_v2

c = 0.9
i_pltv_iap = 1.0
iap_roas_egr_score = 0.00663569
d0_iaa_pltv = 0.272087
d0_iap_pltv = 0.0138885
goal = 0.5
ratio = 0.64
beta = 0.95
a = 0.2
b = 0.005
c = 0.9

if iap_roas_egr_score == 0.0:
    i_pltv_iap = c
else:
    i_pltv_iap = math.exp(math.log(0.2) + 0.005 - (0.005 / iap_roas_egr_score)) + 0.9


bid_result = ratio * (d0_iaa_pltv + beta * d0_iap_pltv * i_pltv_iap)/0.5
print(bid_result)