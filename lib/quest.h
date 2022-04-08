#define START_QUESTS \
void _$run_question(int question) {\
    switch (question){\
        case -1:
#define QUEST(num, bloco) \
        case num:\
            printf("Questão número %i:\n", num);\
            bloco\
            if (question != -1) {\
                break;\
            }
#define END_QUESTS \
        default:\
            if (question != -1) {\
                printf("Invalid question number %i!\n", question);\
                exit(1);\
            }\
    }\
}

#define RUN_ALL_QUESTS() _$run_question(-1);
#define RUN_QUEST(num) _$run_question((unsigned int)num);
