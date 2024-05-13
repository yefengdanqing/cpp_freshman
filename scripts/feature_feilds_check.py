#coding --UTF-8--
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
    with open(file_name, 'r') as f:
        lines = f.readlines()
        for line in lines:
            index = line.find(split_field)
            if index == -1:
                continue
            str_line = line[index:]
            json_dict = json.loads(str_line, strict = False)
            result_dict = json_flatten("", json_dict)
            # result_dict = flatten_dict(json_dict)
            # print(result_dict)
            str = ""
            print(len(feature_keys))
            for key in feature_keys:
                if key in result_dict:
                    str += '{}:{};'.format(key, result_dict[key])
                else:
                    str += '{}:;'.format(key)
            print(str)


if __name__ == "__main__":
    feature_keys = ['demandCreativeId','video_component_id','video_component_template_id','video_source_material_id','video_final_material_id','video_resolution','video_ratio','video_duration','video_bitrate_bucket','video_size','video_orientation','endcard_component_id','endcard_component_template_id','endcard_source_material_id','endcard_final_material_id','endcard_material_sub_type','endcard_size_ori','banner_component_id','banner_component_template_id','banner_source_material_id','banner_final_material_id','banner_resolution','banner_mime','banner_orientation']
    if len(sys.argv) < 1:
        print("please input file")
    feature_keys.insert(0, 'request_id')
    feature_keys.insert(1, 'demand_offer_id')
    feature_keys.insert(2, 'demand_creative_id')
    feature_keys.insert(3, 'bidPrice')
    process(sys.argv[1], '{"request_id"', feature_keys)
    