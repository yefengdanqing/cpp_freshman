#coding = UTF-8
import json
import sys
import json

def json_flatten(key, value, basic_types=(str,int,float,bool,complex,bytes)):
    flatten_dict = {}
    if isinstance(value, dict):
        for k, v in value.items():
            tmp_dict = json_flatten(k, v)
            flatten_dict.update(tmp_dict)
    elif isinstance(value, (list,tuple,set)):
        for index in range(len(value)):
            tmp_dict = json_flatten('{}_{}'.format(key, index), value[index])
            flatten_dict.update(tmp_dict) 
    elif(isinstance(value, basic_types)):
        flatten_dict[key] = value

    return flatten_dict


def process(file_name, split_field, feature_keys):
    result_dict = {}
    all_request_offer_creative_dict = {}
    with open(file_name, 'r') as f:
        lines = f.readlines()
        for line in lines:
            index = line.find(split_field)
            if index == -1:
                continue
            str_line = line[index:]
            json_dict = json.loads(str_line, strict = False)
            result_dict = json_flatten("", json_dict)
    all_key = ""
    for join_key in feature_keys:
        if join_key in result_dict:
            if len(all_key) == 0:
                all_key = join_key
            else:
                all_key += '_{}'.format(join_key)
                
        else:
            print("feature stream no join key")
            break
    all_request_offer_creative_dict[all_key] = result_dict
    return result_dict
           

if __name__ == '__main__':
    feature_keys = []
    feature_keys.insert(0, 'requestId')
    feature_keys.insert(1, 'demandOfferId')
    feature_keys.insert(2, 'demandCreativeId')
    if len(sys.argv) < 2:
        print("please give input files")
        print(sys.argv[0])
    dict1 = process(sys.argv[1], '{"requestId', feature_keys)
    dict2 = process(sys.argv[2], '{"requestId', feature_keys)
    if dict1 == dict2:
        print("is same")
    else:
        print("is not same")
    