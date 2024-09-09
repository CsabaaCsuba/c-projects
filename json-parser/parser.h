#ifndef Json
    #include "json.h"
#endif

/*
JSON rules:
    1. always starts with "{" and ends with "}"
    2. keys always look like this: "key1"
    3. values can look like these:
        9, "value", [0, 1, 2], {"is_null":true}, [0, "asd", {"data":{"key":"value"}}]
    
    4. data types: int, string, list, json
    5. an other json format can be anywhere in the file.


EXTRA:
    - the main JSON can be list as well, containing multiple smaller jsons, or primitive data
    - if the main JSON is not a JSON, it will have the key "a"

*/

Json* read_json(Json* JsonPtr){
    
}

Json* read_response(Json* JsonPtr){

}