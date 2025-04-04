#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_PATIENTS 100
#define MAX_THERAPISTS 50
#define MAX_SUPERVISORS 20
#define MAX_GOALS 10
#define MAX_SESSIONS 50
#define FILENAME "therapy_data.dat"

typedef struct {
    int id;
    char name[100];
    char diagnosis[200];
    int age;
    char gender;
    char contact[15];
    char admission_date[11]; 
} Patient;

typedef struct {
    int id;
    char name[100];
    char specialization[100];
    int current_cases;
    char email[50];
} Therapist;

typedef struct {
    int id;
    char name[100];
    char email[50];
} Supervisor;

typedef struct {
    int id;
    char description[200];
    int target_sessions;
    int achieved;
    char status[20];
} TherapyGoal;

typedef struct {
    int session_id;
    int patient_id;
    int therapist_id;
    char date[11];
    char activities[500];
    char observations[500];
    char supervisor_feedback[500];
    bool supervisor_reviewed;
} TherapySession;

typedef struct {
    int id;
    int patient_id;
    int therapist_id;
    int supervisor_id;
    TherapyGoal goals[MAX_GOALS];
    int goal_count;
    TherapySession sessions[MAX_SESSIONS];
    int session_count;
    bool is_active;
    float clinical_rating;
    char start_date[11];
    char end_date[11];
    char status[20];
} TherapyCase;

Patient patients[MAX_PATIENTS];
Therapist therapists[MAX_THERAPISTS];
Supervisor supervisors[MAX_SUPERVISORS];
TherapyCase cases[MAX_PATIENTS];

int patient_count = 0;
int therapist_count = 0;
int supervisor_count = 0;
int case_count = 0;

void load_data();
void save_data();
void allocate_case(bool auto_allocate);
void create_therapy_plan(int case_index);
void record_session(int case_index);
void generate_progress_report(int case_index, bool export_to_file);
void evaluate_case(int case_index);
void view_case_details(int case_index);
void list_all_cases();
void search_cases();
void close_case(int case_index);
void therapist_dashboard(int therapist_id);
void supervisor_dashboard(int supervisor_id);
int find_available_therapist();
bool validate_date(const char *date);
void print_menu_header(const char *title);
void clear_input_buffer();
void to_lower_case(char *str);

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

bool validate_date(const char *date) {
    if (strlen(date) != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }
    
    return true;
}

void print_menu_header(const char *title) {
    printf("\n================================\n");
    printf("%s\n", title);
    printf("================================\n");
}

int main() {
    load_data();
    if (therapist_count == 0) {
        therapist_count = 3;
        strcpy(therapists[0].name, "John Smith"); 
        therapists[0].id = 1;
        strcpy(therapists[0].specialization, "Child Speech Disorders");
        strcpy(therapists[0].email, "john.smith@therapy.com");
        
        strcpy(therapists[1].name, "Emily Davis"); 
        therapists[1].id = 2;
        strcpy(therapists[1].specialization, "Aphasia Rehabilitation");
        strcpy(therapists[1].email, "emily.davis@therapy.com");
        
        strcpy(therapists[2].name, "Michael Johnson"); 
        therapists[2].id = 3;
        strcpy(therapists[2].specialization, "Voice Disorders");
        strcpy(therapists[2].email, "michael.johnson@therapy.com");
    }
    
    if (supervisor_count == 0) {
        supervisor_count = 2;
        strcpy(supervisors[0].name, "Dr. Sarah Wilson"); 
        supervisors[0].id = 1;
        strcpy(supervisors[0].email, "sarah.wilson@therapy.com");
        
        strcpy(supervisors[1].name, "Dr. Robert Brown"); 
        supervisors[1].id = 2;
        strcpy(supervisors[1].email, "robert.brown@therapy.com");
    }
    
    int choice;
    int id;
    
    print_menu_header("Speech Language Therapy Clinical Services Software");
    
    while(1) {
        printf("\nMain Menu:\n");
        printf("1. Staff Login\n");
        printf("2. Allocate New Case\n");
        printf("3. Create/Modify Therapy Plan\n");
        printf("4. Record Therapy Session\n");
        printf("5. Generate Progress Report\n");
        printf("6. Evaluate Case\n");
        printf("7. View Case Details\n");
        printf("8. List/Search Cases\n");
        printf("9. Close Case\n");
        printf("10. Save & Exit\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        
        switch(choice) {
            case 1:
                printf("Enter your ID (1-%d for therapists, %d+ for supervisors): ", 
                      therapist_count, therapist_count+1);
                scanf("%d", &id);
                if (id <= therapist_count) {
                    therapist_dashboard(id);
                } else if (id <= therapist_count + supervisor_count) {
                    supervisor_dashboard(id);
                } else {
                    printf("Invalid ID.\n");
                }
                break;
            case 2:
                printf("Auto-allocate therapist? (1=Yes, 0=No): ");
                int auto_s;
                scanf("%d", &auto_s);
                allocate_case(auto_s);
                break;
            case 3:
                printf("Enter case index: ");
                scanf("%d", &id);
                create_therapy_plan(id);
                break;
            case 4:
                printf("Enter case index: ");
                scanf("%d", &id);
                record_session(id);
                break;
            case 5:
                printf("Enter case index: ");
                scanf("%d", &id);
                printf("Export to file? (1=Yes, 0=No): ");
                int export;
                scanf("%d", &export);
                generate_progress_report(id, export);
                break;
            case 6:
                printf("Enter case index: ");
                scanf("%d", &id);
                evaluate_case(id);
                break;
            case 7:
                printf("Enter case index: ");
                scanf("%d", &id);
                view_case_details(id);
                break;
            case 8:
                search_cases();
                break;
            case 9:
                printf("Enter case index to close: ");
                scanf("%d", &id);
                close_case(id);
                break;
            case 10:
                save_data();
                printf("Data saved. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}

void load_data() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        return;
    }
    
    if (fread(&patient_count, sizeof(int), 1, file) != 1) goto error;
    if (fread(&therapist_count, sizeof(int), 1, file) != 1) goto error;
    if (fread(&supervisor_count, sizeof(int), 1, file) != 1) goto error;
    if (fread(&case_count, sizeof(int), 1, file) != 1) goto error;
    
    if (fread(patients, sizeof(Patient), patient_count, file) != patient_count) goto error;
    if (fread(therapists, sizeof(Therapist), therapist_count, file) != therapist_count) goto error;
    if (fread(supervisors, sizeof(Supervisor), supervisor_count, file) != supervisor_count) goto error;
    if (fread(cases, sizeof(TherapyCase), case_count, file) != case_count) goto error;
    
    fclose(file);
    return;
    
error:
    printf("Error loading data. Starting with empty database.\n");
    fclose(file);
    patient_count = therapist_count = supervisor_count = case_count = 0;
}

void save_data() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Error saving data!\n");
        return;
    }
    
    fwrite(&patient_count, sizeof(int), 1, file);
    fwrite(&therapist_count, sizeof(int), 1, file);
    fwrite(&supervisor_count, sizeof(int), 1, file);
    fwrite(&case_count, sizeof(int), 1, file);
    
    fwrite(patients, sizeof(Patient), patient_count, file);
    fwrite(therapists, sizeof(Therapist), therapist_count, file);
    fwrite(supervisors, sizeof(Supervisor), supervisor_count, file);
    fwrite(cases, sizeof(TherapyCase), case_count, file);
    
    fclose(file);
}

void allocate_case(bool auto_allocate) {
    if (patient_count >= MAX_PATIENTS) {
        printf("Maximum patient limit reached.\n");
        return;
    }
    
    print_menu_header("Allocate New Case");
    
    Patient *p = &patients[patient_count];
    p->id = patient_count + 1;
    
    printf("Enter patient name: ");
    clear_input_buffer();
    fgets(p->name, sizeof(p->name), stdin);
    p->name[strcspn(p->name, "\n")] = '\0';
    
    printf("Enter diagnosis: ");
    fgets(p->diagnosis, sizeof(p->diagnosis), stdin);
    p->diagnosis[strcspn(p->diagnosis, "\n")] = '\0';
    
    printf("Enter age: ");
    scanf("%d", &p->age);
    
    printf("Enter gender (M/F/O): ");
    scanf(" %c", &p->gender);
    p->gender = toupper(p->gender);
    
    printf("Enter contact number: ");
    scanf("%14s", p->contact);
    
    printf("Enter admission date (YYYY-MM-DD): ");
    char date[11];
    scanf("%10s", date);
    while (!validate_date(date)) {
        printf("Invalid date format. Please use YYYY-MM-DD: ");
        scanf("%10s", date);
    }
    strcpy(p->admission_date, date);
    
    int therapist_id;
    if (auto_allocate) {
        therapist_id = find_available_therapist();
        if (therapist_id == -1) {
            printf("No available therapists found. Please try manual allocation.\n");
            return;
        }
        printf("Auto-assigned therapist: %s (ID: %d)\n", 
               therapists[therapist_id-1].name, therapist_id);
    } else {
        printf("\nAvailable Therapists:\n");
        for (int i = 0; i < therapist_count; i++) {
            printf("%d. %s (%s) - Current cases: %d\n", 
                  therapists[i].id, therapists[i].name, 
                  therapists[i].specialization, therapists[i].current_cases);
        }
        
        printf("\nEnter therapist ID: ");
        scanf("%d", &therapist_id);
    }
    printf("\nAvailable Supervisors:\n");
    for (int i = 0; i < supervisor_count; i++) {
        printf("%d. %s\n", supervisors[i].id, supervisors[i].name);
    }
    
    int supervisor_id;
    printf("\nEnter supervisor ID: ");
    scanf("%d", &supervisor_id);
    
    TherapyCase *c = &cases[case_count];
    c->id = case_count + 1;
    c->patient_id = p->id;
    c->therapist_id = therapist_id;
    c->supervisor_id = supervisor_id;
    c->goal_count = 0;
    c->session_count = 0;
    c->is_active = true;
    c->clinical_rating = 0.0;
    strcpy(c->start_date, date);
    strcpy(c->end_date, "");
    strcpy(c->status, "Active");
    
    therapists[therapist_id-1].current_cases++;
    
    printf("\nCase allocated successfully. Case ID: %d\n", c->id);
    case_count++;
    patient_count++;
}

int find_available_therapist() {
    int min_cases = MAX_PATIENTS;
    int selected_id = -1;
    
    for (int i = 0; i < therapist_count; i++) {
        if (therapists[i].current_cases < min_cases) {
            min_cases = therapists[i].current_cases;
            selected_id = therapists[i].id;
        }
    }
    
    return selected_id;
}

void create_therapy_plan(int case_index) {
    if (case_index < 0 || case_index >= case_count) {
        printf("Invalid case index.\n");
        return;
    }
    
    TherapyCase *c = &cases[case_index];
    print_menu_header("Create/Modify Therapy Plan");
    
    printf("Case ID: %d | Patient ID: %d\n", c->id, c->patient_id);
    printf("Current goals: %d\n", c->goal_count);
    
    if (c->goal_count > 0) {
        printf("\nExisting Goals:\n");
        for (int i = 0; i < c->goal_count; i++) {
            printf("%d. %s (Target: %d sessions, Achieved: %d, Status: %s)\n", 
                  c->goals[i].id, c->goals[i].description,
                  c->goals[i].target_sessions, c->goals[i].achieved,
                  c->goals[i].status);
        }
        
        printf("\n1. Add new goals\n2. Modify existing goals\n3. Cancel\nChoice: ");
        int choice;
        scanf("%d", &choice);
        
        if (choice == 2) {
            printf("Enter goal number to modify: ");
            int goal_num;
            scanf("%d", &goal_num);
            if (goal_num < 1 || goal_num > c->goal_count) {
                printf("Invalid goal number.\n");
                return;
            }
            
            TherapyGoal *g = &c->goals[goal_num-1];
            printf("\nEditing Goal %d:\n", goal_num);
            printf("Current description: %s\n", g->description);
            printf("New description (or press enter to keep): ");
            clear_input_buffer();
            char new_desc[200];
            fgets(new_desc, sizeof(new_desc), stdin);
            new_desc[strcspn(new_desc, "\n")] = '\0';
            if (strlen(new_desc) > 0) {
                strcpy(g->description, new_desc);
            }
            
            printf("Current target sessions: %d\n", g->target_sessions);
            printf("New target sessions (or 0 to keep): ");
            int new_target;
            scanf("%d", &new_target);
            if (new_target > 0) {
                g->target_sessions = new_target;
            }
            
            printf("Goal updated successfully.\n");
            return;
        } else if (choice == 3) {
            return;
        }
    }
    
    int goal_count;
    printf("\nEnter number of therapy goals to add (max %d): ", MAX_GOALS - c->goal_count);
    scanf("%d", &goal_count);
    
    if (goal_count <= 0 || goal_count > (MAX_GOALS - c->goal_count)) {
        printf("Invalid number of goals.\n");
        return;
    }
    
    clear_input_buffer();
    for (int i = 0; i < goal_count; i++) {
        TherapyGoal *g = &c->goals[c->goal_count + i];
        g->id = c->goal_count + i + 1;
        
        printf("\nGoal %d:\n", g->id);
        printf("Enter description: ");
        fgets(g->description, sizeof(g->description), stdin);
        g->description[strcspn(g->description, "\n")] = '\0';
        
        printf("Enter target sessions: ");
        scanf("%d", &g->target_sessions);
        clear_input_buffer();
        
        g->achieved = 0;
        strcpy(g->status, "Not Started");
    }
    
    c->goal_count += goal_count;
    printf("\nTherapy plan updated successfully. Total goals: %d\n", c->goal_count);
}

void record_session(int case_index) {
    if (case_index < 0 || case_index >= case_count) {
        printf("Invalid case index.\n");
        return;
    }
    
    TherapyCase *c = &cases[case_index];
    if (!c->is_active) {
        printf("This case is not active. Cannot record sessions.\n");
        return;
    }
    
    if (c->session_count >= MAX_SESSIONS) {
        printf("Maximum sessions limit reached for this case.\n");
        return;
    }
    
    print_menu_header("Record Therapy Session");
    
    int session_idx = c->session_count;
    TherapySession *s = &c->sessions[session_idx];
    s->session_id = session_idx + 1;
    s->patient_id = c->patient_id;
    s->therapist_id = c->therapist_id;
    s->supervisor_reviewed = false;
    
    
    printf("Enter session date (YYYY-MM-DD) or 'today' for current date: ");
    char date_input[11];
    scanf("%10s", date_input);
    
    if (strcmp(date_input, "today") == 0) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(s->date, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    } else {
        while (!validate_date(date_input)) {
            printf("Invalid date format. Please use YYYY-MM-DD: ");
            scanf("%10s", date_input);
        }
        strcpy(s->date, date_input);
    }
    
    clear_input_buffer();
    printf("Session Date: %s\n", s->date);
    printf("Enter activities performed: ");
    fgets(s->activities, sizeof(s->activities), stdin);
    s->activities[strcspn(s->activities, "\n")] = '\0';
    
    printf("Enter observations: ");
    fgets(s->observations, sizeof(s->observations), stdin);
    s->observations[strcspn(s->observations, "\n")] = '\0';
    
    if (c->goal_count > 0) {
        printf("\nUpdate goal progress? (1=Yes, 0=No): ");
        int update;
        scanf("%d", &update);
        if (update) {
            printf("Select goal to update:\n");
            for (int i = 0; i < c->goal_count; i++) {
                printf("%d. %s\n", c->goals[i].id, c->goals[i].description);
            }
            printf("Goal number: ");
            int goal_num;
            scanf("%d", &goal_num);
            if (goal_num >= 1 && goal_num <= c->goal_count) {
                c->goals[goal_num-1].achieved++;
                if (c->goals[goal_num-1].achieved >= c->goals[goal_num-1].target_sessions) {
                    strcpy(c->goals[goal_num-1].status, "Completed");
                } else {
                    strcpy(c->goals[goal_num-1].status, "In Progress");
                }
                printf("Goal %d progress updated.\n", goal_num);
            }
        }
    }
    
    c->session_count++;
    printf("\nSession recorded successfully. Total sessions: %d\n", c->session_count);
}

void generate_progress_report(int case_index, bool export_to_file) {
    if (case_index < 0 || case_index >= case_count) {
        printf("Invalid case index.\n");
        return;
    }
    
    TherapyCase *c = &cases[case_index];
    print_menu_header("Progress Report");
    
    Patient *p = NULL;
    for (int i = 0; i < patient_count; i++) {
        if (patients[i].id == c->patient_id) {
            p = &patients[i];
            break;
        }
    }
    
    if (p == NULL) {
        printf("Patient not found.\n");
        return;
    }
    
    char report[5000];
    int pos = 0;
    
    pos += sprintf(report + pos, "\nPROGRESS REPORT\n");
    pos += sprintf(report + pos, "Case ID: %d\n", c->id);
    pos += sprintf(report + pos, "Patient: %s (ID: %d)\n", p->name, p->id);
    pos += sprintf(report + pos, "Diagnosis: %s\n", p->diagnosis);
    pos += sprintf(report + pos, "Age: %d, Gender: %c\n", p->age, p->gender);
    pos += sprintf(report + pos, "Admission Date: %s\n", p->admission_date);
    
    for (int i = 0; i < therapist_count; i++) {
        if (therapists[i].id == c->therapist_id) {
            pos += sprintf(report + pos, "\nTherapist: %s (%s)\n", 
                          therapists[i].name, therapists[i].specialization);
            break;
        }
    }
    
    for (int i = 0; i < supervisor_count; i++) {
        if (supervisors[i].id == c->supervisor_id) {
            pos += sprintf(report + pos, "Supervisor: %s\n", supervisors[i].name);
            break;
        }
    }
    
    pos += sprintf(report + pos, "\nCase Status: %s\n", c->status);
    pos += sprintf(report + pos, "Start Date: %s\n", c->start_date);
    if (strlen(c->end_date) > 0) {
        pos += sprintf(report + pos, "End Date: %s\n", c->end_date);
    }
    pos += sprintf(report + pos, "Clinical Rating: %.1f/5.0\n", c->clinical_rating);
    
    pos += sprintf(report + pos, "\nTHERAPY GOALS:\n");
    for (int i = 0; i < c->goal_count; i++) {
        pos += sprintf(report + pos, "%d. %s\n   Target: %d sessions, Achieved: %d, Status: %s\n", 
                      c->goals[i].id, c->goals[i].description,
                      c->goals[i].target_sessions, c->goals[i].achieved,
                      c->goals[i].status);
    }
    
    pos += sprintf(report + pos, "\nTOTAL SESSIONS COMPLETED: %d\n", c->session_count);
    
    pos += sprintf(report + pos, "\nRECENT SESSIONS:\n");
    int start = (c->session_count > 5) ? c->session_count - 5 : 0;
    for (int i = start; i < c->session_count; i++) {
        pos += sprintf(report + pos, "\nSession %d on %s\n", 
                      c->sessions[i].session_id, c->sessions[i].date);
        pos += sprintf(report + pos, "Activities: %s\n", c->sessions[i].activities);
        pos += sprintf(report + pos, "Observations: %s\n", c->sessions[i].observations);
        if (strlen(c->sessions[i].supervisor_feedback) > 0) {
            pos += sprintf(report + pos, "Supervisor Feedback: %s\n", 
                          c->sessions[i].supervisor_feedback);
        }
    }
    
    printf("%s", report);
    
    if (export_to_file) {
        char filename[50];
        sprintf(filename, "Case_%d_Report.txt", c->id);
        
        FILE *file = fopen(filename, "w");
        if (file) {
            fputs(report, file);
            fclose(file);
            printf("\nReport saved to %s\n", filename);
        } else {
            printf("\nError saving report to file.\n");
        }
    }
}

void evaluate_case(int case_index) {
    if (case_index < 0 || case_index >= case_count) {
        printf("Invalid case index.\n");
        return;
    }
    
    TherapyCase *c = &cases[case_index];
    print_menu_header("Case Evaluation");
    
    if (c->session_count < 10) {
        printf("Evaluation requires at least 10 sessions. Current sessions: %d\n", 
               c->session_count);
        return;
    }
    
    printf("Enter supervisor feedback for the case:\n");
    clear_input_buffer();
    fgets(c->sessions[c->session_count-1].supervisor_feedback, 
         sizeof(c->sessions[c->session_count-1].supervisor_feedback), stdin);
    c->sessions[c->session_count-1].supervisor_feedback[strcspn(
        c->sessions[c->session_count-1].supervisor_feedback, "\n")] = '\0';
    c->sessions[c->session_count-1].supervisor_reviewed = true;
    
    printf("Enter clinical rating (0.0 - 5.0): ");
    scanf("%f", &c->clinical_rating);
    
    printf("\nCase evaluation completed successfully.\n");
}

void view_case_details(int case_index) {
    if (case_index < 0 || case_index >= case_count) {
        printf("Invalid case index.\n");
        return;
    }
    
    TherapyCase *c = &cases[case_index];
    print_menu_header("Case Details");
    
    printf("Case ID: %d\n", c->id);
    printf("Patient ID: %d\n", c->patient_id);
    printf("Therapist ID: %d\n", c->therapist_id);
    printf("Supervisor ID: %d\n", c->supervisor_id);
    printf("Status: %s\n", c->status);
    printf("Start Date: %s\n", c->start_date);
    if (strlen(c->end_date) > 0) printf("End Date: %s\n", c->end_date);
    printf("Goals: %d\n", c->goal_count);
    printf("Sessions: %d\n", c->session_count);
    printf("Clinical Rating: %.1f/5.0\n", c->clinical_rating);
    
    if (c->session_count > 0) {
        printf("Last Session: %s\n", c->sessions[c->session_count-1].date);
        if (c->sessions[c->session_count-1].supervisor_reviewed) {
            printf("Last Supervisor Review: Completed\n");
        } else {
            printf("Last Supervisor Review: Pending\n");
        }
    }
}

void search_cases() {
    print_menu_header("Search Cases");
    
    printf("Search by:\n");
    printf("1. Patient ID\n");
    printf("2. Therapist ID\n");
    printf("3. Supervisor ID\n");
    printf("4. Status\n");
    printf("5. Show All\n");
    printf("Choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    int id;
    char status[20];
    
    printf("\nSearch Results:\n");
    printf("ID\tPatient\tTherapist\tSessions\tStatus\n");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < case_count; i++) {
        TherapyCase *c = &cases[i];
        bool match = false;
        
        switch(choice) {
            case 1:
                printf("Enter Patient ID: ");
                scanf("%d", &id);
                match = (c->patient_id == id);
                break;
            case 2:
                printf("Enter Therapist ID: ");
                scanf("%d", &id);
                match = (c->therapist_id == id);
                break;
            case 3:
                printf("Enter Supervisor ID: ");
                scanf("%d", &id);
                match = (c->supervisor_id == id);
                break;
            case 4:
                printf("Enter Status: ");
                scanf("%19s", status);
                to_lower_case(status);
                char case_status[20];
                strcpy(case_status, c->status);
                to_lower_case(case_status);
                match = (strcmp(case_status, status) == 0);
                break;
            case 5:
                match = true;
                break;
            default:
                printf("Invalid choice.\n");
                return;
        }
        
        if (match) {
            char patient_name[50] = "Unknown";
            for (int j = 0; j < patient_count; j++) {
                if (patients[j].id == c->patient_id) {
                    strncpy(patient_name, patients[j].name, sizeof(patient_name));
                    break;
                }
            }
            
            printf("%d\t%.15s\t%d\t\t%d\t\t%s\n", 
                  c->id, patient_name, c->therapist_id, 
                  c->session_count, c->status);
        }
    }
}

void close_case(int case_index) {
    if (case_index < 0 || case_index >= case_count) {
        printf("Invalid case index.\n");
        return;
    }
    
    TherapyCase *c = &cases[case_index];
    if (!c->is_active) {
        printf("Case is already closed.\n");
        return;
    }
    
    printf("Enter end date (YYYY-MM-DD): ");
    char date[11];
    scanf("%10s", date);
    while (!validate_date(date)) {
        printf("Invalid date format. Please use YYYY-MM-DD: ");
        scanf("%10s", date);
    }
    strcpy(c->end_date, date);
    
    printf("Enter status (Completed/Discontinued): ");
    scanf("%19s", c->status);
    
    printf("Final clinical rating (0.0-5.0): ");
    scanf("%f", &c->clinical_rating);
    
    c->is_active = false;
    
    // Update therapist's case count
    for (int i = 0; i < therapist_count; i++) {
        if (therapists[i].id == c->therapist_id) {
            therapists[i].current_cases--;
            break;
        }
    }
    
    printf("\nCase %d closed successfully.\n", c->id);
}

void therapist_dashboard(int therapist_id) {
    Therapist *t = NULL;
    for (int i = 0; i < therapist_count; i++) {
        if (therapists[i].id == therapist_id) {
            t = &therapists[i];
            break;
        }
    }
    
    if (t == NULL) {
        printf("Therapist not found.\n");
        return;
    }
    
    int choice;
    
    while(1) {
        print_menu_header("Therapist Dashboard");
        printf("Welcome, %s (%s)\n", t->name, t->specialization);
        printf("Current cases: %d\n\n", t->current_cases);
        
        printf("1. View My Cases\n");
        printf("2. Record Session\n");
        printf("3. Create/Modify Therapy Plan\n");
        printf("4. Generate Progress Report\n");
        printf("5. Return to Main Menu\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: {
                printf("\nYour Active Cases:\n");
                printf("ID\tPatient\tSessions\n");
                printf("------------------------\n");
                for (int i = 0; i < case_count; i++) {
                    if (cases[i].therapist_id == therapist_id && cases[i].is_active) {
                        char patient_name[50] = "Unknown";
                        for (int j = 0; j < patient_count; j++) {
                            if (patients[j].id == cases[i].patient_id) {
                                strncpy(patient_name, patients[j].name, sizeof(patient_name));
                                break;
                            }
                        }
                        printf("%d\t%.15s\t%d\n", cases[i].id, patient_name, cases[i].session_count);
                    }
                }
                break;
            }
            case 2: {
                printf("Enter Case ID to record session: ");
                int case_id;
                scanf("%d", &case_id);
                bool found = false;
                for (int i = 0; i < case_count; i++) {
                    if (cases[i].id == case_id && cases[i].therapist_id == therapist_id) {
                        record_session(i);
                        found = true;
                        break;
                    }
                }
                if (!found) printf("Case not found or not assigned to you.\n");
                break;
            }
            case 3: {
                printf("Enter Case ID to modify plan: ");
                int case_id;
                scanf("%d", &case_id);
                bool found = false;
                for (int i = 0; i < case_count; i++) {
                    if (cases[i].id == case_id && cases[i].therapist_id == therapist_id) {
                        create_therapy_plan(i);
                        found = true;
                        break;
                    }
                }
                if (!found) printf("Case not found or not assigned to you.\n");
                break;
            }
            case 4: {
                printf("Enter Case ID to generate report: ");
                int case_id;
                scanf("%d", &case_id);
                bool found = false;
                for (int i = 0; i < case_count; i++) {
                    if (cases[i].id == case_id && cases[i].therapist_id == therapist_id) {
                        generate_progress_report(i, false);
                        found = true;
                        break;
                    }
                }
                if (!found) printf("Case not found or not assigned to you.\n");
                break;
            }
            case 5:
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}

void supervisor_dashboard(int supervisor_id) {
    Supervisor *s = NULL;
    for (int i = 0; i < supervisor_count; i++) {
        if (supervisors[i].id == supervisor_id) {
            s = &supervisors[i];
            break;
        }
    }
    
    if (s == NULL) {
        printf("Supervisor not found.\n");
        return;
    }
    
    int choice;
    
    while(1) {
        print_menu_header("Supervisor Dashboard");
        printf("Welcome, %s\n\n", s->name);
        
        printf("1. View Cases Under Supervision\n");
        printf("2. Review Therapy Plans\n");
        printf("3. Evaluate Cases\n");
        printf("4. Generate Reports\n");
        printf("5. Return to Main Menu\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: {
                printf("\nCases Under Your Supervision:\n");
                printf("ID\tPatient\tTherapist\tSessions\tStatus\n");
                printf("-----------------------------------------------\n");
                for (int i = 0; i < case_count; i++) {
                    if (cases[i].supervisor_id == supervisor_id) {
                        char patient_name[50] = "Unknown";
                        for (int j = 0; j < patient_count; j++) {
                            if (patients[j].id == cases[i].patient_id) {
                                strncpy(patient_name, patients[j].name, sizeof(patient_name));
                                break;
                            }
                        }
                        
                        char therapist_name[50] = "Unknown";
                        for (int j = 0; j < therapist_count; j++) {
                            if (therapists[j].id == cases[i].therapist_id) {
                                strncpy(therapist_name, therapists[j].name, sizeof(therapist_name));
                                break;
                            }
                        }
                        
                        printf("%d\t%.15s\t%.15s\t%d\t\t%s\n", 
                              cases[i].id, patient_name, therapist_name,
                              cases[i].session_count, cases[i].status);
                    }
                }
                break;
            }
            case 2: {
                printf("\nCases Needing Plan Review:\n");
                bool found = false;
                for (int i = 0; i < case_count; i++) {
                    if (cases[i].supervisor_id == supervisor_id && 
                        cases[i].session_count == 0) {
                        printf("Case ID: %d | Patient ID: %d\n", 
                              cases[i].id, cases[i].patient_id);
                        found = true;
                    }
                }
                if (!found) printf("No cases need plan review at this time.\n");
                
                printf("\nEnter Case ID to review plan (or 0 to cancel): ");
                int case_id;
                scanf("%d", &case_id);
                if (case_id == 0) break;
                
                for (int i = 0; i < case_count; i++) {
                    if (cases[i].id == case_id && cases[i].supervisor_id == supervisor_id) {
                        view_case_details(i);
                        printf("\n1. Approve Plan\n2. Request Changes\nChoice: ");
                        int review_choice;
                        scanf("%d", &review_choice);
                        if (review_choice == 1) {
                            printf("Plan approved. Therapist can now begin sessions.\n");
                        } else {
                            printf("Enter feedback for changes: ");
                            clear_input_buffer();
                            char feedback[500];
                            fgets(feedback, sizeof(feedback), stdin);
                            feedback[strcspn(feedback, "\n")] = '\0';
                            printf("Feedback sent to therapist.\n");
                        }
                        break;
                    }
                }
                break;
            }
            case 3: {
                printf("\nCases Ready for Evaluation (10+ sessions):\n");
                bool found = false;
                for (int i = 0; i < case_count; i++) {
                    if (cases[i].supervisor_id == supervisor_id && 
                        cases[i].session_count >= 10 && 
                        cases[i].is_active) {
                        printf("Case ID: %d | Sessions: %d\n", 
                              cases[i].id, cases[i].session_count);
                        found = true;
                    }
                }
                if (!found) printf("No cases ready for evaluation at this time.\n");
                
                printf("\nEnter Case ID to evaluate (or 0 to cancel): ");
                int case_id;
                scanf("%d", &case_id);
                if (case_id == 0) break;
                
                for (int i = 0; i < case_count; i++) {
                    if (cases[i].id == case_id && cases[i].supervisor_id == supervisor_id) {
                        evaluate_case(i);
                        break;
                    }
                }
                break;
            }
            case 4: {
                printf("\nEnter Case ID to generate report: ");
                int case_id;
                scanf("%d", &case_id);
                for (int i = 0; i < case_count; i++) {
                    if (cases[i].id == case_id && cases[i].supervisor_id == supervisor_id) {
                        generate_progress_report(i, true);
                        break;
                    }
                }
                break;
            }
            case 5:
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}