#pragma once
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define PRINT(STR) std::cout<<STR<<std::endl
#define RESOURCES_CSV "resources.csv"
#define STAGE_PATH_NAME(EPISODE_NAEM) "config/" + (string)EPISODE_NAEM +"/resources.csv"
#define STAGE_PATH_NUM(EPISODE_NUM) "config/episode" + to_string(EPISODE_NUM) +"/resources.csv"