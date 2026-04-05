#pragma once

#define BIT(x) (1 << x)
#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

#include "Window/Window.h"
#include "Event/Event.h"
#include "Layer/Layer.h"
