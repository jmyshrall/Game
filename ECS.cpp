#include "ECS.h"

ECS& ECS::get() {
    static ECS instance;
    return instance;
}
