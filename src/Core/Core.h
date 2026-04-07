#pragma once


#include "Window/Window.h"
#include "Event/EventHeader.h"
#include "Layer/Layer.h"
#include "App/App.h"
#include "Timer/Timer.h"
#include "Camera/Camera.h"
#include "Input/Input.h"

#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)