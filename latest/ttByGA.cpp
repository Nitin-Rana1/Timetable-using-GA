#include <stdio.h>
#include "Institute.h"
#include "Schedule.h"

int main()
{
    time_t start, end;
    time(&start);
    srand(time(NULL));
    vector<Schedule> allSchedules;
    string name = "RSS";
    int DAYS = 6, HOURS = 8;
    vector<string> teachers = {
        "M_A", "M_B", "M_C",
        "P_A", "P_B", "P_C",
        "C_A", "C_B", "C_C",
        "B_A", "B_B", "B_C",
        "E_A", "E_B", "E_C",
        "H_A", "H_B", "H_C",
        "G_A", "G_B", "G_C",
        "IP_A", "IP_B",
        "S_A", "S_B", "S_C",
        "SS_A", "SS_B", "SS_C"};                              // teacher names
    vector<pair<string, pair<vector<string>, int>>> subjects; // subject name, teacher names, hours per week
    int POPULATION_SIZE = 100;
    int GENERATIONS = 100;

    subjects.push_back({"Science", {{"S_A", "S_B", "S_C"}, 9}});
    subjects.push_back({"Social Science", {{"SS_A", "SS_B", "SS_C"}, 9}});
    subjects.push_back({"Maths", {{"M_A", "M_B", "M_C"}, 9}});
    subjects.push_back({"English", {{"E_A", "E_B", "E_C"}, 6}});
    subjects.push_back({"Hindi", {{"H_A", "H_B", "H_C"}, 6}});
    subjects.push_back({"Game", {{"G_A", "G_B", "G_C"}, 3}});
    subjects.push_back({"IP", {{"IP_A", "IP_B"}, 6}});

    Institute institute1(teachers, name, DAYS, HOURS,
                         POPULATION_SIZE, GENERATIONS);

    // fillig subjectHoursPerWeek for 10 A, B, C, D
    institute1.createSchedule("10 A", subjects);
    institute1.createSchedule("10 B", subjects);
    institute1.createSchedule("10 C", subjects);
    institute1.createSchedule("10 D", subjects);

    // fillig subjectHoursPerWeek for 11 A and B
    subjects.clear();

    subjects.push_back({"Physics", {{"P_A", "P_B", "P_C"}, 9}});
    subjects.push_back({"Chemistry", {{"C_A", "C_B", "C_C"}, 9}});
    subjects.push_back({"Maths", {{"M_A", "M_B", "M_C"}, 9}});

    subjects.push_back({"English", {{"E_A", "E_B", "E_C"}, 6}});
    subjects.push_back({"Game", {{"G_A", "G_B", "G_C"}, 6}});
    subjects.push_back({"IP", {{"IP_A", "IP_B"}, 9}});

    institute1.createSchedule("11 A", subjects);
    institute1.createSchedule("11 B", subjects);

    // fillig subjectHoursPerWeek for 11 C and D
    subjects.clear();
    subjects.push_back({"Chemistry", {{"C_A", "C_B", "C_C"}, 9}});
    subjects.push_back({"English", {{"E_A", "E_C"}, 6}});
    subjects.push_back({"Game", {{"G_B", "G_C"}, 6}});
    subjects.push_back({"Physics", {{"P_A", "P_B", "P_C"}, 9}});
    subjects.push_back({"Biology", {{"B_A", "B_B", "B_C"}, 9}});
    subjects.push_back({"Maths", {{"M_A", "M_B"}, 9}});

    institute1.createSchedule("11 C", subjects);
    institute1.createSchedule("11 D", subjects);

    cout << "INS SIZE: " << institute1.allSchedules.size() << endl;
    long long int total_fitness = 0;
    for (auto tt : institute1.allSchedules)
    {
        total_fitness += tt.fitness;
        tt.printClassTT();
        tt.printSTATS();
    }
    institute1.overlapTeachers();

    time(&end);

    double time_taken = double(end - start);
    cout << "\nTime taken by program is : " << fixed
         << time_taken << setprecision(5);
    cout << " sec " << endl;
    cout << "Total fitness: " << total_fitness << endl;
    return 0;
}
// // filling subjectHoursPerWeek for 10
// subjectHoursPerWeek["Science 10"] = 9;
// subjectHoursPerWeek["Social Science 10"] = 9;
// subjectHoursPerWeek["Maths 10"] = 9;
// subjectHoursPerWeek["English 10"] = 6;
// subjectHoursPerWeek["Hindi 10"] = 6;
// subjectHoursPerWeek["Game 10"] = 3;
// subjectHoursPerWeek["IP 10"] = 6;
// total = 48 / 48