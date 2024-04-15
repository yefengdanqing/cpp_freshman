import json
def traverse_json(data, map_all):
    stack = [(None, data)]

    while stack:
        key, current = stack.pop()

        if isinstance(current, dict):
            for k, v in current.items():
                stack.append((k, v))
        elif isinstance(current, list):
            for i, item in enumerate(current):
                stack.append((i, item))
        else:
            #print(f'Key: {key}, Value: {current}')
            map_all[key] = current

def get_common_keys(dict1, dict2):
    keys1 = set(dict1.keys())
    keys2 = set(dict2.keys())
    common_keys = keys1 & keys2
    return common_keys

def diff_keys_result(key_sets, data_map1, data_map2):
    is_true = True
    for ele in key_sets:
        if data_map1[ele] == data_map1[ele]:
            x = 0
            #print(f'Key: {ele}, Value: {data_map1[ele]}')
        else:
            print(f'request: {data_map1["requestId"]} is not same, Key: {ele}, Value1: {data_map1[ele]}, Value2: {data_map2[ele]} ')
            is_true = False
    if is_true == True:
        print(f'request: {data_map1["requestId"]} is same, common key size: {len(key_sets)}')
    return is_true


def read_line_from_file(file_name1, file_name2):
    with open(file_name1) as f1, open(file_name2) as f2:
        for line1, line2 in zip(f1, f2):
            # print(f'file: {line1.strip()}')
            # print(f'file: {line2.strip()}')
            data1 = json.loads(line1)
            data2 = json.loads(line1)
            key_value_map1 = {}
            key_value_map2 = {}
            traverse_json(data1, key_value_map1)
            traverse_json(data2, key_value_map2)
            # for ele in key_value_map1:
            #     print(f'Key: {ele}, Value: {key_value_map1[ele]}')
            # for ele in key_value_map2:
            #     print(f'Key: {ele}, Value: {key_value_map2[ele]}')

            common_keys = get_common_keys(key_value_map1, key_value_map2)
            diff_keys_result(common_keys, key_value_map1, key_value_map2)


def compare_json(file_name1, file_name2):
    with open(file_name1) as f1, open(file_name2) as f2:
        data1 = json.loads(f1)
        data2 = json.loads(f2)
        compare_two_generated_feature("650d9be3f873f700089217d9", data1, data2)

if __name__ == '__main__':
    #read_line_from_file('json_new.txt', 'json_old.txt')
    read_line_from_file('skt_adx_old_json.txt', 'skt_adx_new_json.txt')
    #compare_json('json_new.txt', 'json_old.txt')