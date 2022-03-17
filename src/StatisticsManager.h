//
// Created by Randy Collado on 3/17/22.
//


#ifndef CS561_FINAL_PROJECT_STATISTICSMANAGER_H
#define CS561_FINAL_PROJECT_STATISTICSMANAGER_H

#import <map>
#import <string>

class StatisticsManager {
public:
    StatisticsManager();
    ~StatisticsManager();

private:
    std::map<std::string, double> statistics;
    std::string entity;
};


#endif //CS561_FINAL_PROJECT_STATISTICSMANAGER_H
