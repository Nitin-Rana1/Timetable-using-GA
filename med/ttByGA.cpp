#include <stdio.h>
#include "Institute.h"
#include "Schedule.h"

int main()
{
    srand(time(NULL));
    vector<Schedule> allSchedules;

    string name = "ABC";
    int DAYS = 6, HOURS = 8;
    vector<pair<string, vector<string>>> subjects;  // subject name, teacher names
    unordered_map<string, int> subjectHoursPerWeek; // subject name, hours per week

    int POPULATION_SIZE = 100;
    int GENERATIONS = 100;
    subjects.push_back({"Maths", {"M_A", "M_B", "M_C"}});
    subjects.push_back({"Physics", {"P_A", "P_B", "P_C"}});
    subjects.push_back({"Chemistry", {"C_A", "C_B", "C_C"}});
    subjects.push_back({"Biology", {"B_A", "B_B", "B_C"}});
    subjects.push_back({"English", {"E_A", "E_B", "E_C"}});
    subjects.push_back({"Hindi", {"H_A", "H_B", "H_C"}});
    subjects.push_back({"Game", {"G_A", "G_B", "G_C"}});
    subjects.push_back({"IP", {"IP_A", "IP_B"}});
    subjects.push_back({"Science", {"S_A", "S_B", "S_C"}});
    subjects.push_back({"Social Science", {"SS_A", "SS_B", "SS_C"}});

    Institute institute1(name, DAYS, HOURS, subjects,
                         POPULATION_SIZE, GENERATIONS);

    // filling subjectHoursPerWeek for 10
    subjectHoursPerWeek["Science"] = 9;
    subjectHoursPerWeek["Social Science"] = 9;
    subjectHoursPerWeek["Maths"] = 9;
    subjectHoursPerWeek["English"] = 6;
    subjectHoursPerWeek["Hindi"] = 6;
    subjectHoursPerWeek["Game"] = 3;
    subjectHoursPerWeek["IP"] = 6;
    // total = 48 / 48

    institute1.createSchedule("10 A", subjectHoursPerWeek);
    institute1.overlapTeachers();
    // institute1.createSchedule("10 B", subjectHoursPerWeek);
    // institute1.createSchedule("10 C", subjectHoursPerWeek);
    // institute1.createSchedule("10 D", subjectHoursPerWeek);

    // subjectHoursPerWeek.clear();
    // // filling subjectHoursPerWeek for 11
    // subjectHoursPerWeek["Physics"] = 9;
    // subjectHoursPerWeek["Chemistry"] = 9;
    // subjectHoursPerWeek["Maths"] = 9;
    // subjectHoursPerWeek["English"] = 6;
    // subjectHoursPerWeek["Game"] = 6;
    // subjectHoursPerWeek["IP"] = 9;
    // // total = 48 / 48

    // // institute1.createSchedule("11 A", subjectHoursPerWeek);
    // // institute1.createSchedule("11 B", subjectHoursPerWeek);

    // subjectHoursPerWeek.clear();
    // // fillig subjectHoursPerWeek for 11 C and D
    // subjectHoursPerWeek["Chemistry"] = 9;
    // subjectHoursPerWeek["English"] = 9;
    // subjectHoursPerWeek["Game"] = 3;
    // subjectHoursPerWeek["Hindi"] = 9;
    // subjectHoursPerWeek["Biology"] = 9;
    // subjectHoursPerWeek["Physics"] = 9;
    // total = 42 / 48

    // institute1.createSchedule("10 C", subjectHoursPerWeek);
    // institute1.createSchedule("10 D", subjectHoursPerWeek);

    cout << "INS SIZW" << institute1.mySchedules.size() << endl;
    for (auto tt : institute1.mySchedules)
    {
        tt.printClassTT();
        tt.printSTATS();
    }
    return 0;
}