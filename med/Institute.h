#ifndef INSTITUTE_H
#define INSTITUTE_H
#include <bits/stdc++.h>
#include "Schedule.h"

using namespace std;

Schedule crossOver1(Schedule parent1, Schedule parent2, int DAYS, int HOURS)
{
    if (rand() % 2 == 0)
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
                child.classTT[i][j] = parent1.classTT[i][j];
            }
            child.classTT[i][j].second = child.choosenTeacher[child.classTT[i][j].first];
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
            child.classTT[i][j] = parent1.classTT[i][j];
        }
    }
    return child;
}

void mutate(Schedule &child, int maxMutations, int DAYS, int HOURS, vector<pair<string, vector<string>>> subjects)
{

    // Determine the number of mutations for this child
    int numMutations = rand() % (maxMutations + 1);

    for (int mutation = 0; mutation < numMutations; mutation++)
    {
        int day = rand() % DAYS;             // Randomly select a day
        int hour = 1 + rand() % (HOURS - 1); // Randomly select an hour

        // Perform a mutation on the selected day and hour
        // Example: Swap the subject and teacher for the selected period
        string subject = child.classTT[day][hour].first;
        string teacher = child.choosenTeacher[subject];

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
    vector<pair<string, vector<string>>> subjects; // subject name, teacher names
    // unordered_map<string, int> subjectHoursPerWeek; // subject name, hours per week
    vector<Schedule> mySchedules;
    // unordered_map<string, string> choosenTeacher; // subject, teacher

    int DAYS, HOURS;
    int POPULATION_SIZE, GENERATIONS;

    Institute(string name, int DAYS, int HOURS, vector<pair<string, vector<string>>> subjects,
              int POPULATION_SIZE, int GENERATIONS)
    {
        this->name = name;
        this->DAYS = DAYS;
        this->HOURS = HOURS;
        this->POPULATION_SIZE = POPULATION_SIZE;
        this->GENERATIONS = GENERATIONS;
        }
    void createSchedule(string className, unordered_map<string, int> subjectHoursPerWeek)
    {
        //    fill choosenTeacher
        vector<pair<string, vector<string>>> tempSubjects;
        for (auto e : subjects)
        {
            if (subjectHoursPerWeek.count(e.first))
            {
                tempSubjects.push_back(e);
            }
        }
        this->subjects = tempSubjects;

        // cout << "print choosenTeacher variable " << endl;
        // for (auto p : choosenTeacher)
        // {
        //     cout << p.first << " : " << p.second << endl;
        // }

        vector<Schedule> population;
        // for (int i = 0; i < FIRST_POPULATION_SIZE; i++)

        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            for (int i = 0; i < subjects.size(); i++)
            {
                choosenTeacher[subjects[i].first] = subjects[i].second[rand() % subjects[i].second.size()];
            }
            Schedule s(mySchedules, DAYS, HOURS, className, subjects, subjectHoursPerWeek, choosenTeacher);
            s.calcFitness(mySchedules);
            population.push_back(s);
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
                child.calcFitness(mySchedules);
                child.className = "Class" + to_string(j + 1);
                newPopulation.push_back(child);
            }
            population = newPopulation;
        }
        cout << "one tt created! ------------------------------------------------------------------------------------------------" << endl;
        sort(population.begin(), population.end(), [](Schedule a, Schedule b)
             { return a.fitness > b.fitness; });
        mySchedules.push_back(population[0]);
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
                for (int k = 0; k < mySchedules.size(); k++)
                {
                    temp.insert(mySchedules[k].classTT[i][j].first + mySchedules[k].classTT[i][j].second);
                }
                if (temp.size() == mySchedules.size())
                {
                    overlaps += 0;
                }
                else
                {
                    overlaps += (mySchedules.size() + 1 - temp.size());
                }
            }
        }
        cout << "OVERLAP OF teacher: " << overlaps << endl;
    }
};
#endif
