#ifndef INSTITUTE_H
#define INSTITUTE_H
#include <bits/stdc++.h>
#include "Schedule.h"

using namespace std;

Schedule crossOver1(Schedule parent1, Schedule parent2, int DAYS, int HOURS)
{
    if (rand() % 2)
    {
        Schedule temp = parent1;
        parent1 = parent2;
        parent2 = temp;
    }
    Schedule child = parent2;
    int total = DAYS * HOURS;
    int crossPoint = rand() % total;
    for (int i = 0; i < DAYS; i++)
    {
        for (int j = 1; j < HOURS; j++)
        {
            if (i * HOURS + j < crossPoint)
            {
                child.classTT[i][j].first = parent1.classTT[i][j].first;
                child.classTT[i][j].second = child.choosenTeacher[child.classTT[i][j].first];
            }
        }
    }
    return child;
}
Schedule crossOver2(Schedule parent1, Schedule parent2, int DAYS, int HOURS)
{
    if (rand() % 2 == 0)
    {
        Schedule temp = parent1;
        parent1 = parent2;
        parent2 = temp;
    }
    Schedule child = parent2;
    int periodCrossOverPoint = rand() % HOURS;
    for (int i = 0; i < DAYS; i++)
    {
        for (int j = 0; j < periodCrossOverPoint; j++)
        {
            child.classTT[i][j].first = parent1.classTT[i][j].first;
            child.classTT[i][j].second = child.choosenTeacher[child.classTT[i][j].first];
        }
    }
    return child;
}

void mutate(Schedule &child, int maxMutations, int DAYS, int HOURS, vector<pair<string, pair<vector<string>, int>>> subjects)
{

    // Determine the number of mutations for this child
    int numMutations = rand() % (maxMutations + 1);

    while (numMutations--)
    {
        int day = rand() % DAYS;             // Randomly select a day
        int hour = 1 + rand() % (HOURS - 1); // Randomly select an hour
        // explain hour formula: 1 + rand() % (HOURS - 1)
        // why is that?: because we don't want to mutate the first hour of the day

        // Perform a mutation on the selected day and hour
        // Example: Swap the subject and teacher for the selected period
        // string subject = child.classTT[day][hour].first;
        // string teacher = child.choosenTeacher[subject];

        // Choose a new random subject and teacher
        int newSubjectIndex = rand() % subjects.size();
        string newSubject = subjects[newSubjectIndex].first;
        string newTeacher = child.choosenTeacher[newSubject];

        // Update the child's timetable
        child.classTT[day][hour] = {newSubject, newTeacher};
    }
}

class Institute
{
public:
    string name;
    vector<string> teachers;
    vector<Schedule> allSchedules;

    int DAYS, HOURS;
    int POPULATION_SIZE, GENERATIONS;

    Institute(vector<string> teachers, string name, int DAYS, int HOURS,
              int POPULATION_SIZE, int GENERATIONS)
    {
        this->name = name;
        this->DAYS = DAYS;
        this->HOURS = HOURS;
        this->POPULATION_SIZE = POPULATION_SIZE;
        this->GENERATIONS = GENERATIONS;
        this->teachers = teachers;
    }
    void createSchedule(string className, vector<pair<string, pair<vector<string>, int>>> subjects)
    {

        vector<Schedule> population;
        // for (int i = 0; i < FIRST_POPULATION_SIZE; i++)
        unordered_map<string, string> choosenTeacher;
        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            for (int i = 0; i < subjects.size(); i++)
            {
                choosenTeacher[subjects[i].first] = subjects[i].second.first[rand() % subjects[i].second.first.size()];
            }
            Schedule s(allSchedules, choosenTeacher, subjects, DAYS, HOURS, className);
            s.calcFitness(allSchedules);
            population.push_back(s);
            choosenTeacher.clear();
        }
        int size = POPULATION_SIZE;
        for (int i = 0; i < GENERATIONS; i++)
        {
            // creating new population from old population using crossover and mutation
            vector<Schedule> newPopulation;
            for (int j = 0; j < POPULATION_SIZE; j++)
            {
                // if (i == 0)
                // {
                //     size = FIRST_POPULATION_SIZE;
                // }
                Schedule a = population[rand() % size];
                Schedule b = population[rand() % size];
                Schedule parent1 = a.fitness > b.fitness ? a : b;
                a = population[rand() % size];
                b = population[rand() % size];
                Schedule parent2 = a.fitness > b.fitness ? a : b;
                // Schedule child = crossOver1(parent1, parent2, DAYS, HOURS); // this is simple crossover that ensures same teacher for a subject : CrossPOINT
                Schedule child = crossOver2(parent1, parent2, DAYS, HOURS); // this is uses period crossover  line: For same subject period over week

                mutate(child, 2, DAYS, HOURS, subjects); // 2 is max mutations allowed
                child.calcFitness(allSchedules);
                newPopulation.push_back(child);
            }
            population = newPopulation;
        }
        cout << "one tt created! ------------------------------------------------------------------------------------------------" << endl;
        sort(population.begin(), population.end(), [](Schedule a, Schedule b)
             { return a.fitness > b.fitness; });
        allSchedules.push_back(population[0]);
    }
    void overlapTeachers()
    {
        // this return the number of overlaps in a week
        // where overlap is : if a teacher is teaching more than one class at a time
        int overlaps = 0;
        for (int i = 0; i < DAYS; i++)
        {
            for (int j = 0; j < HOURS; j++)
            {
                unordered_set<string> temp;
                for (int k = 0; k < allSchedules.size(); k++)
                {
                    temp.insert(allSchedules[k].classTT[i][j].second);
                }
                if (temp.size() == allSchedules.size())
                {
                    overlaps += 0;
                }
                else
                {
                    overlaps += allSchedules.size() - temp.size();
                }
            }
        }
        cout << "OVERLAP OF teacher: " << overlaps << endl;
    }
};
#endif
