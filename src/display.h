/* Initialize ncurses
 * code       : the code to display
 * codeSize   : the size of aforementioned code
 * linesLength: the length of the lines of code
 * delay      : the timeout delay for keyboard event
 */
void initDisplay(char* code, int codeSize, int linesLength, int delay);

/* Print the character in the output area. '\n' makes a new line
 * c: the character to print
 */
void ncursesPrintChar(char c);

/* Print the integer in the output area
 * i: the integer to print
 */
void ncursesPrintInt(int i);

/* Leave the ncurses mode temporarily
 */
void pauseNcurses(void);

/* Restore the ncurses left before
 */
void resumeNcurses(void);

/* Display the current position of the program counter
 */
void display(int PC);

/* uninitialize ncurses
 */
void uninitDisplay(void);
