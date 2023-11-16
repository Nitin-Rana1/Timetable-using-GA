#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <bits/stdc++.h>

using namespace std;

class Schedule
{
public:
    string className;
    vector<vector<pair<string, string>>> classTT; // one class's Week Time Table
    unordered_map<string, string> choosenTeacher; // subject, teacher
    int DAYS, HOURS;
    int fitness;
    vector<pair<string, pair<vector<string>, int>>> subjects;
    unordered_map<string, int> tempSubjectHoursPW;

    Schedule(vector<Schedule> allSchedules, unordered_map<string, string> choosenTeacher,
             vector<pair<string, pair<vector<string>, int>>> subjects, int D, int H, string className)
    {
        this->className = className;
        this->DAYS = D;
        this->HOURS = H;
        this->choosenTeacher = choosenTeacher;
        this->subjects = subjects;

        // fill classTT
        // class Teacher is chosen randomly and should be same for whole week;
        string firstSub = subjects[rand() % subjects.size()].first;

        for (int i = 0; i < DAYS; i++)
        {
            vector<pair<string, string>> day;
            for (int j = 0; j < HOURS; j++)
            {
                if (j == 0)
                {
                    day.push_back({firstSub, choosenTeacher[firstSub]});
                    continue;
                }
                // choose random subject
                // choose random teacher
                string sub = subjects[rand() % subjects.size()].first;
                string teach = choosenTeacher[sub];
                day.push_back({sub, teach});
            }
            classTT.push_back(day);
        }
        for (int i = 0; i < DAYS; i++)
        {
            for (int j = 0; j < HOURS; j++)
            {
                tempSubjectHoursPW[classTT[i][j].first]++;
            }
        }
    }
    void calcFitness(vector<Schedule> allSchedules)
    {
        fitness = 0;
        // step 1: check if sub and their lecture hours are completed in a week
        fitness += lecHoursFitness(allSchedules, 200);

        // step 2: check if teacher overlaps
        fitness += teacherOverlapFitness(allSchedules, 100);

        // step 3: if subject is taught in the same period in a week
        fitness += samePeriodPWFitness(allSchedules, 70);

        // step 4: if subject comes more than once in a day
        fitness += sameSubjectPDFitness(allSchedules, 85);

        // step 5: check same subect same period in consecutive days of week
        fitness += sameSubjectPeriodPWFitness(allSchedules, 10);
    }
    int sameSubjectPeriodPWFitness(vector<Schedule> &allSchedules, int rate)
    {
        int fitness = 0;
        for (int i = 0; i < HOURS; i++)
        {
            unordered_set<string> subs;
            for (int j = 0; j < DAYS; j++)
            {
                subs.insert(classTT[j][i].first);
            }
            if (subs.size() == 1)
            {
                fitness += rate * 5;
            }
            else if (subs.size() == 2)
                fitness += rate * 2;
            else
                fitness -= rate * 2;
        }
        return fitness;
    }
    int sameSubjectPDFitness(vector<Schedule> &allSchedules, int rate)
    {
        int fitness = 0;
        for (int i = 0; i < DAYS; i++)
        {
            unordered_set<string> temp;
            for (int j = 0; j < HOURS; j++)
            {
                temp.insert(classTT[i][j].first);
            }
            if (temp.size() == HOURS)
            {
                fitness += rate;
            }
            fitness -= (HOURS - temp.size()) * rate;
        }
        return fitness;
    }
    int samePeriodPWFitness(vector<Schedule> &allSchedules, int rate)
    {
        int fitness = 0;
        for (int i = 0; i < HOURS; i++)
        {
            unordered_set<string> temp;
            for (int j = 0; j < DAYS; j++)
            {
                temp.insert(classTT[j][i].first);
            }
            if (temp.size() <= 2)
            {
                fitness += rate;
            }
            else
            {
                fitness -= temp.size() * rate;
            }
        }
        return fitness;
    }
    int teacherOverlapFitness(vector<Schedule> &allSchedules, int rate)
    {
        int fitness = 0;
        for (int i = 0; i < DAYS; i++)
        {
            for (int j = 0; j < HOURS; j++)
            {
                unordered_set<string> temp{classTT[i][j].first + classTT[i][j].second};
                for (int k = 0; k < allSchedules.size(); k++)
                {
                    temp.insert(allSchedules[k].classTT[i][j].first + allSchedules[k].classTT[i][j].second);
                }
                if (temp.size() == allSchedules.size() + 1)
                {
                    fitness += rate * 2;
                }
                else
                {
                    fitness -= (allSchedules.size() + 1 - temp.size()) * rate * 2;
                }
            }
        }
        return fitness;
    }

    int lecHoursFitness(vector<Schedule> &allSchedules, int rate)
    {

        int fitness = 0;
        // tempSubjectHoursPW = subjectHoursPerWeek;
        for (int i = 0; i < DAYS; i++)
        {
            for (int j = 0; j < HOURS; j++)
            {
                tempSubjectHoursPW[classTT[i][j].first] = 0;
            }
        }
        for (int i = 0; i < DAYS; i++)
        {
            for (int j = 0; j < HOURS; j++)
            {
                tempSubjectHoursPW[classTT[i][j].first]++;
            }
        }
        for (auto i : subjects)
        {
            int diff = abs(tempSubjectHoursPW[i.first] - i.second.second);
            if (diff == 0)
            {
                fitness += rate;
            }
            else
            {
                fitness -= diff * rate;
            }
        }
        return fitness;
    }

    void printSTATS()
    {
        cout << "Class:" << this->className << endl;
        cout << "fitness: " << fitness << endl
             << endl;
        cout << "subjectHoursPerWeek:  need and given" << endl;

        for (auto i : subjects)
        {
            cout << i.first << " " << i.second.second << " -> " << tempSubjectHoursPW[i.first] << endl;
        }
        cout << endl
             << endl;
    }
    void printClassTT()
    {
        // Calculate the width for teacher and subject columns based on the maximum length
        int teacherColumnWidth = 0;
        int subjectColumnWidth = 0;
        for (int i = 0; i < DAYS; i++)
        {
            for (int j = 0; j < HOURS; j++)
            {
                int teacherLength = classTT[i][j].first.length();
                int subjectLength = classTT[i][j].second.length();
                teacherColumnWidth = max(teacherColumnWidth, teacherLength);
                subjectColumnWidth = max(subjectColumnWidth, subjectLength);
            }
        }
        teacherColumnWidth += 2;
        subjectColumnWidth += 2;
        // Print the timetable with columns of the same size
        cout << "Day X";
        for (int i = 0; i < HOURS; i++)
        {
            cout << setw(teacherColumnWidth + subjectColumnWidth - 1) << "Period " << i + 1;
        }
        cout << endl;

        for (int i = 0; i < DAYS; i++)
        {
            cout << "Day " << i + 1;
            for (int j = 0; j < HOURS; j++)
            {
                cout << setw(teacherColumnWidth) << classTT[i][j].first
                     << setw(subjectColumnWidth) << classTT[i][j].second;
            }
            cout << endl;
        }
    }
};
#endif