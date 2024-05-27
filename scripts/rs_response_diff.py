#coding=utf-8
import sys
import os
from enum import Enum
import json

class DiffType(Enum):
    PRICE = 1
    OFFER = 2
    CREATIVE = 3

def common_process_result(price_dict1, price_dict2, price_factor):
    success_count = 0
    failed_count = 0
    for key,value in price_dict1.items():
        if key in price_dict2:
            if float(value) / float(price_dict2[key]) <= float(price_factor):
                success_count += 1
                print("diff_key {} existed, and value is same:{}--{}".format(key, value, price_dict2[key]))
            else:
                failed_count += 1
                print("diff_key {} existed, but value is not same:{}--{}".format(key, value, price_dict2[key]))
        else:
            print("diff_key {} did not existed,some value:{}".format(key, value))
            failed_count += 1
    return success_count, failed_count
    
def read_file_for_json(file, type):
    dict = {}
    with open(file, 'rb') as f:
        lines = f.readlines()
        line_count = 0
        if type == DiffType.PRICE.value:
            for line in lines:
                json_str = json.loads(line, strict = False)
                response_str = json_str["payload"]["Response"]
                response_json = json.loads(response_str)
                is_filled = response_json["status_msg"]
                if (is_filled == "is_filled:false"):
                    continue
                request_id = response_json["request_id"]
                bid_price = formatted_number = "{:.6f}".format(float(response_json["bid_price"]))
                line_count = line_count + 1
                print(request_id, bid_price)
                dict[request_id] = bid_price
        elif type == DiffType.OFFER.value:
            for line in lines:
                json_str = json.loads(line, strict = False)
                response_str = json_str["payload"]["Response"]
                response_json = json.loads(response_str)
                is_filled = response_json["status_msg"]
                if (is_filled == "is_filled:false"):
                    continue
                request_id = response_json["request_id"]
                data = response_json["bid_result"]
                for i in range(len(data)):
                    line_count = line_count + 1
                    key = request_id + "#" + str(data[i]["campaign_id"])
                    #先保存6位
                    bid_price = formatted_number = "{:.6f}".format(float(data[i]["bid_price"]))
                    print(key, bid_price)
                    dict[key] = bid_price
        elif type == DiffType.CREATIVE.value:
            for line in lines:
                json_str = json.loads(line, strict = False)
                response_str = json_str["payload"]["Response"]
                response_json = json.loads(response_str)
                is_filled = response_json["status_msg"]
                if (is_filled == "is_filled:false"):
                    continue
                request_id = response_json["request_id"]
                data = response_json["bid_result"]
                for i in range(len(data)):
                    line_count = line_count + 1
                    key = request_id + "#" + str(data[i]["campaign_id"]) + "#" + data[i]["demand_creative"]["demand_creative_id"]
                    #先保存6位
                    bid_price = float("{:.6f}".format(data[i]["bid_price"]))
                    print(key, bid_price)
                    dict[key] = bid_price
                
        f.close()
    return dict,line_count
        
            
                
   
def processor_price(file1, file2, type, price_factor):
    price_dict1, count1 = read_file_for_json(file1, type)
    price_dict2, count2 = read_file_for_json(file2, type)
    success_count, failed_count = common_process_result(price_dict1, price_dict2, price_factor)
    rate = float(success_count) / (count1 if count1 > count2 else count2)
    print(success_count, failed_count, count1, count2)
    print("request price diff, old file:{}, new file:{} concordance rate: {}".format(file1, file2, rate))


def processor_offer(file1, file2, type):
    price_dict1, count1 = read_file_for_json(file1, type)
    price_dict2, count2 = read_file_for_json(file2, type)
    success_count, failed_count = common_process_result(price_dict1, price_dict2, 1)
    print(success_count, failed_count, count1, count2)
    rate = float(success_count) / float(count1 if count1 > count2 else count2)
    print("offer price diff, old file:{}, new file:{} concordance rate:{}:".format(file1, file2, rate))



def processor_creative(file1, file2, diff_type):
    print(file1, file2, diff_type)
    price_dict1, count1 = read_file_for_json(file1, diff_type)
    price_dict2, count2 = read_file_for_json(file2, diff_type)
    success_count, failed_count = common_process_result(price_dict1, price_dict2, 1)
    rate = float(success_count) / (count1 if count1 > count2 else count2)
    print("creative price diff, old file:{}, new file:{} concordance rate:{}:".format(file1, file2, rate))
    

if __name__ == '__main__':
    if len(sys.argv) < 4:
        print('para is not filled')
    elif sys.argv[3] == "price":
        processor_price(sys.argv[1], sys.argv[2], 1, sys.argv[4])
    elif sys.argv[3] ==  "offer":
        processor_offer(sys.argv[1], sys.argv[2], 2)
    elif sys.argv[3] == "creative":
        processor_creative(sys.argv[1], sys.argv[2], 3)
    else:
        print("please input correct pars :python3 rs_response_diff.py old_file1 new_file2 type [price_factor]")
    