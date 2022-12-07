#include "Framework/Framework.h" //내가 추가한 파일은 큰따움표로 하기
#include "Framework/info.h"
#include "nlohmann/json.hpp"


int main()
{
    FRAMEWORK->Init();
    FRAMEWORK->Do();
    FRAMEWORK->~Framework();
    //
    return 0;
}