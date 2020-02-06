#define LEFT 0
#define RIGHT 1

#define DISABLE 0
#define ENABLE 1

#define MAX_QUESTIONS 10

/* States */
#define RESET_AFTER_PAUSE pause &&reset
#define UNPAUSE_AFTER_RESET !pause && reset
#define PAUSE_STATE pause && !reset

typedef enum
{
    IDLE,
    PLAY,
    PAUSE,
    OFF
} GameState;

extern GameState state;

/* Global Flags */
extern int reset;
extern int pause;

extern int hex_count; /* timer digit count */
extern int random_number;
extern int questions_count;
extern int power_on;

/* Global value storage registers */
extern int hex_3_0_val;
extern int hex_5_4_val;
extern int switch_val;