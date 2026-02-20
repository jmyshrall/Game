#include "Input.h"

Input& Input::get()
{
    static Input instance;
    return instance;
}
