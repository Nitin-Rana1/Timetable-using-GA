#include <iostream>
#include <bits/stdc++.h>

using namespace std;

#define FIRST_POPULATION_SIZE 70
#define POPULATION_SIZE 70
#define GENERATIONS 700
#define MUTATION_PROBABILITY 0.1
#define TEACHERS 10

// #define DAYS 6
// #define HOURS 8

vector<pair<string, vector<string>>> subjects; // subject -> teachers
unordered_map<string, int> subjectHoursPerWeek;

class Schedule
{
public:
    string className;
    vector<vector<pair<string, string>>> classTT;
    unordered_map<string, string> choosenTeacher;
    unordered_map<string, int> tempSubjectHoursPW;
    int DAYS, HOURS;

    int fitness;
    Schedule(vector<Schedule> allSchedules, string className, int DAYS, int HOURS)
    {
        this->className = className;
        this->DAYS = DAYS;
        this->HOURS = HOURS;
        for (int i = 0; i < subjects.size(); i++)
        {
            choosenTeacher[subjects[i].first] = subjects[i].second[rand() % subjects[i].second.size()];
        }
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
        // fitness += sameSubjectPeriodPWFitness(allSchedules, 10);
    }
    int sameSubjectPeriodPWFitness(vector<Schedule> &allSchedules, int rate)
    {
        int fitness = 0;
        for (int i = 0; i < DAYS; i++)
        {
            unordered_map<string, vector<int>> subAndInd;
            for (int j = 0; j < HOURS; j++)
            {
                subAndInd[classTT[i][j].first].push_back(j);
            }
            int diff = 0;
            for (auto p : subAndInd)
            {
                for (int j = 1; j < p.second.size(); j++)
                {
                    diff += p.second[j] - p.second[j - 1] - 1;
                }
            }
            fitness -= diff * rate;
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
        for (auto i : subjectHoursPerWeek)
        {
            int diff = abs(tempSubjectHoursPW[i.first] - i.second);
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
        cout << "fitness: " << fitness << endl;
        cout << "subjectHoursPerWeek:  need and given" << endl;
        for (auto i : subjectHoursPerWeek)
        {
            cout << i.first << " " << i.second << " -> " << tempSubjectHoursPW[i.first] << endl;
        }
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
        cout << className << endl;
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
int OVERLAP(vector<Schedule> &allSchedules, int DAYS = 6, int HOURS = 8)
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
                temp.insert(allSchedules[k].classTT[i][j].first + allSchedules[k].classTT[i][j].second);
            }
            if (temp.size() == allSchedules.size())
            {
                overlaps += 0;
            }
            else
            {
                overlaps += (allSchedules.size() + 1 - temp.size());
            }
        }
    }
    return overlaps;
}
vector<Schedule> allSchedules;
void mutate(Schedule &child, int maxMutations, int DAYS, int HOURS);
Schedule crossOver1(Schedule parent1, Schedule parent2, int DAYS, int HOURS);
Schedule crossOver2(Schedule parent1, Schedule parent2, int DAYS, int HOURS);
void addTT(vector<Schedule> &allSchedules, vector<pair<string, vector<string>>> subjects, unordered_map<string, int> subjectHoursPerWeek, int DAYS, int HOURS);
int main()
{
    srand(time(NULL));
    subjects.push_back({"Maths", {"M_A", "M_B", "M_C"}});
    subjects.push_back({"Physics", {"P_A", "P_B", "P_C"}});
    subjects.push_back({"Chemistry", {"C_A", "C_B", "C_C"}});
    subjects.push_back({"Biology", {"B_A", "B_B", "B_C"}});
    subjects.push_back({"English", {"E_A", "E_B", "E_C"}});
    subjects.push_back({"Hindi", {"H_A", "H_B", "H_C"}});
    subjects.push_back({"Game", {"G_A", "G_B", "G_C"}});
    subjects.push_back({"IP", {"IP_A", "IP_B"}});

    // filling subjectHoursPerWeek
    subjectHoursPerWeek["Physics"] = 9;
    subjectHoursPerWeek["Chemistry"] = 9;
    subjectHoursPerWeek["Maths"] = 6;
    subjectHoursPerWeek["Biology"] = 6;
    subjectHoursPerWeek["English"] = 6;
    subjectHoursPerWeek["Hindi"] = 6;
    subjectHoursPerWeek["Game"] = 3;
    subjectHoursPerWeek["IP"] = 3;
    // total sum =
    // 6 8 = 48

    // total hours: 48

    for (int i = 0; i < 5; i++)
    {
        cout << "TT: " << i << endl;
        addTT(allSchedules, subjects, subjectHoursPerWeek, 6, 8);
        allSchedules[i].printClassTT();
        allSchedules[i].printSTATS();
    }
    Schedule s(allSchedules, "Class" + to_string(1), 6, 8);
    cout << " FFFFFFFFFFFF " << OVERLAP(allSchedules);
    return 0;
}
void addTT(vector<Schedule> &allSchedules, vector<pair<string, vector<string>>> subjects, unordered_map<string, int> subjectHoursPerWeek, int DAYS, int HOURS)
{
    vector<Schedule> population;
    // for (int i = 0; i < FIRST_POPULATION_SIZE; i++)
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        Schedule s(allSchedules, "Class" + to_string(i + 1), DAYS, HOURS);
        s.calcFitness(allSchedules);
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

            mutate(child, 2, DAYS, HOURS); // 2 is max mutations allowed
            child.calcFitness(allSchedules);
            child.className = "Class" + to_string(j + 1);
            newPopulation.push_back(child);
        }
        population = newPopulation;
    }
    cout << "AFTER GENERATIONS ------------------------------------------------------------------------------------------------" << endl;
    sort(population.begin(), population.end(), [](Schedule a, Schedule b)
         { return a.fitness > b.fitness; });
    allSchedules.push_back(population[0]);
}
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

void mutate(Schedule &child, int maxMutations, int DAYS, int HOURS)
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