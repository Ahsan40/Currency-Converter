#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "headers/rlutil.h"
#include "headers/CustomHeader.h"

void sevenDaysRateLive(int *);
void checkSpecificDayRate(int *);
void checkHistoryLiveMainMenu();
void sevenDaysLiveTop();
void specDayTop();
void sevenDaysHighestTop();
void liveHistoryMainTop();
void askOpt1();
void askOpt2();

//
// Main function of this C file
//
int checkRateHistoryLive()
{
    char ch, ch2;
    int opt;
    saveDefaultColor();
SDRMenu:
    cls();
    liveHistoryMainTop();
    checkHistoryLiveMainMenu();
SDRch:
    setColor(LIGHTGREEN);
    printf("\n     --> Input Option: ");
    resetColor();
    ch = getch();
    tolower(ch);
    if (ch == '1')
    {
        cls();
        sevenDaysRateLive(&opt);
        if (opt == 1)
            goto SDRMenu;
        else if (opt == 2)
            return 0;
    }
    else if (ch == '2')
    {
        cls();
        checkSpecificDayRate(&opt);
        if (opt == 1)
            goto SDRMenu;
        else if (opt == 2)
            return 0;
    }
    else if (ch == 'm')
    {
        cls();
        return 0;
    }
    else if (ch == 'x')
    {
        exit(0);
    }
    else
    {
        wrongInput();
        goto SDRch;
    }
}

//
// Major Functions
//

void sevenDaysRateLive(int *opt)
{
    sevenDaysLiveTop();
    FILE *sevenHistoryFile;
    oneDayRate sevenDays[7];
    oneDayRate highest = {"", 0};
    int errorCode;
    char ch2;
    char tmp[128] = "https://free.currconv.com/api/v7/convert?apiKey=";
    char url[128] = "";
    char nDate[15] = "";
    char oDate[15] = "";
    char exID[8] = "";
    char sevenHistoryJSON[128] = "sevenDaysHistory.json";
    char sevenHistoryData[1024] = "";
sevenDaysEXID:
    printf(" Exchange IDs: ");
    scanf("%s", &exID);
    errorCode = inputValidation(exID);
    if (errorCode == -1)
        goto sevenDaysEXID;

    replace_char(exID, '-', '_');
    previousDate(nDate, 8);
    previousDate(oDate, 1);
    sprintf(tmp, "%s%s&q=",tmp, api);
    sprintf(url, "%s%s&compact=ultra&date=%s&endDate=%s", tmp, exID, nDate, oDate);
    fetchJson(url, sevenHistoryJSON);
    errorCode = inputValidationLv4(sevenHistoryJSON);
    // Input Validation Lv. 4 (Final Level)
    if (errorCode == -1)
    {
        setColor(RED);
        printf("\n ERROR: Failed To Fetch API!\n");
        resetColor();
        exit(0);
    }
    else if (errorCode == -2)
    {
        wrongInput();
        goto sevenDaysEXID;
    }
    sevenHistoryFile = fopen(sevenHistoryJSON, "r");
    fscanf(sevenHistoryFile, "%s", &sevenHistoryData);
    fclose(sevenHistoryFile);
    remove(sevenHistoryJSON);
    for (int i = 0; i < 7; i++)
    {
        previousDate(sevenDays[i].date, i + 1);
        sevenDays[i].rate = specificDateRate(exID, sevenDays[i].date, sevenHistoryData);
    }
    printf("\n     Date             Rate\n");
    printf("  ------------      ---------\n");
    for (int i = 0; i < 7; i++)
    {
        printf("   %s        %.4lf\n", sevenDays[i].date, sevenDays[i].rate);
        if (sevenDays[i].rate > highest.rate)
            highest = sevenDays[i];
    }
    hidecursor();
askOpt:
    askOpt1();
askOptInp:
    ch2 = getch();
    tolower(ch2);
    if (ch2 == 'b')
    {
        showcursor();
        *opt = 1;
    }
    else if (ch2 == 'm')
    {
        showcursor();
        *opt = 2;
    }
    else if (ch2 == 'h')
    {
        cls();
        sevenDaysHighestTop();
        printf("   %s        %.4lf\n", highest.date, highest.rate);
        askOpt2();
        goto askOptInp;
    }
    else if (ch2 == 'x')
    {
        showcursor();
        exit(0);
    }
    else
    {
        wrongInput();
        goto askOptInp;
    }
}

void checkSpecificDayRate(int *opt)
{
    specDayTop();
    FILE *specHistoryFile;
    oneDayRate specDay = {"", 0};
    int errorCode;
    char ch2;
    char tmp[128] = "https://free.currconv.com/api/v7/convert?apiKey=";
    char url[128] = "";
    char exID[8] = "";
    char specHistoryJSON[128] = "specDaysHistory.json";
    char specHistoryData[1024] = "";
specDayEXID:
    printf(" Exchange IDs: ");
    scanf("%s", &exID);
    printf(" Date (YYYY-MM-DD): ");
    scanf("%s", &specDay.date);
    errorCode = inputValidation(exID);
    if (errorCode == -1)
        goto specDayEXID;

    replace_char(exID, '-', '_');
    sprintf(tmp, "%s%s&q=",tmp, api);
    sprintf(url, "%s%s&compact=ultra&date=%s", tmp, exID, specDay.date);
    fetchJson(url, specHistoryJSON);
    errorCode = inputValidationLv4(specHistoryJSON);
    // Input Validation Lv. 4 (Final Level)
    if (errorCode == -1)
    {
        setColor(RED);
        printf("\n ERROR: Failed To Fetch API!\n");
        resetColor();
        exit(0);
    }
    else if (errorCode == -2)
    {
        wrongInput();
        goto specDayEXID;
    }
    specHistoryFile = fopen(specHistoryJSON, "r");
    fscanf(specHistoryFile, "%s", &specHistoryData);
    fclose(specHistoryFile);
    remove(specHistoryJSON);
    specDay.rate = specificDateRate(exID, specDay.date, specHistoryData);
    printf("\n     Date             Rate\n");
    printf("  ------------      ---------\n");
    printf("   %s        %.4lf\n", specDay.date, specDay.rate);
    hidecursor();
    askOpt2();
askOptInp:
    ch2 = getch();
    tolower(ch2);
    if (ch2 == 'b')
    {
        showcursor();
        *opt = 1;
    }
    else if (ch2 == 'm')
    {
        showcursor();
        *opt = 2;
    }
    else if (ch2 == 'x')
    {
        showcursor();
        exit(0);
    }
    else
    {
        wrongInput();
        goto askOptInp;
    }
}

//
//
// Decoration Functions
//
//

// Main Menu UI
void checkHistoryLiveMainMenu()
{
    setColor(LIGHTCYAN);
    printf("     ___________________________________________\n");
    printf("    |                                           |\n");
    setColor(LIGHTCYAN);
    printf("    |");
    resetColor();
    printf("       1. Last 7 Days Rate");
    setColor(LIGHTCYAN);
    printf("                 |\n");
    printf("    |");
    resetColor();
    printf("       2. Check Specific Day Rate");
    setColor(LIGHTCYAN);
    printf("          |\n");
    printf("    |");
    resetColor();
    printf("       M. Main Menu");
    setColor(LIGHTCYAN);
    printf("                        |\n");
    printf("    |");
    resetColor();
    printf("       X. Exit");
    setColor(LIGHTCYAN);
    printf("                             |\n");
    printf("    |___________________________________________|\n");
    resetColor();
}

//
// All Headers UI
//

void liveHistoryMainTop()
{
    setColor(LIGHTRED);
    printf("\n       =======================================        \n");
    printf("       ||   Currrency Rate History (Live)   ||        \n");
    printf("       =======================================       \n");
    setColor(LIGHTCYAN);
    resetColor();
}

void specDayTop()
{
    setColor(LIGHTRED);
    printf("\n               =======================================        \n");
    printf("               ||   Check Specific Day Rate (Live)   ||        \n");
    printf("               ========================================       \n");
    setColor(LIGHTBLUE);
    printf("  ____________________________________________________________________\n");
    printf(" |                                                                    |\n");
    printf(" |                          ");
    setColor(LIGHTMAGENTA);
    printf("INSTRUCTIONS");
    setColor(LIGHTBLUE);
    printf("                              |\n");
    printf(" |  ");
    setColor(LIGHTGREEN);
    printf("Exchange ID:");
    setColor(LIGHTCYAN);
    printf(" Which currency rate will be checked. ");
    setColor(LIGHTGREEN);
    printf("(eg. USD-BDT)");
    setColor(LIGHTBLUE);
    printf("   |\n");
    printf(" |  ");
    setColor(LIGHTGREEN);
    printf("Date:");
    setColor(LIGHTCYAN);
    printf(" Which day's rate will be checked. ");
    setColor(LIGHTGREEN);
    printf("(eg. 2020-10-06)");
    setColor(LIGHTBLUE);
    printf("          |\n");
    printf(" |  ");
    setColor(LIGHTRED);
    printf("NOTE: IT'S USING FREE API & ONLY ALLOWED UP TO 1 YEAR EARLIER.    ");
    setColor(LIGHTBLUE);
    printf("|\n");
    printf(" |____________________________________________________________________|\n\n");
    resetColor();
}

void sevenDaysLiveTop()
{
    setColor(LIGHTRED);
    printf("\n                 ================================        \n");
    printf("                 ||   Last 7 Days Rate (Live)   ||        \n");
    printf("                 ================================       \n");
    setColor(LIGHTBLUE);
    printf("  ____________________________________________________________________\n");
    printf(" |                                                                    |\n");
    printf(" |                          ");
    setColor(LIGHTMAGENTA);
    printf("INSTRUCTIONS");
    setColor(LIGHTBLUE);
    printf("                              |\n");
    printf(" |  ");
    setColor(LIGHTGREEN);
    printf("Exchange ID:");
    setColor(LIGHTCYAN);
    printf(" Which currency rate will be checked. ");
    setColor(LIGHTGREEN);
    printf("(eg. USD-BDT)");
    setColor(LIGHTBLUE);
    printf("   |\n");
    printf(" |____________________________________________________________________|\n\n");
    resetColor();
}

void sevenDaysHighestTop()
{
    setColor(LIGHTRED);
    printf("\n  ==================================        \n");
    printf("  ||   Last 7 Days Highest Rate   ||        \n");
    printf("  ==================================       \n");
    setColor(LIGHTCYAN);
    resetColor();
    printf("\n     Date             Rate\n");
    printf("  ------------      ---------\n");
}

//
// Footer Options UI
//

void askOpt1()
{
    printf("\n ===================================================================");
    printf("\n ||   ");
    setColor(LIGHTMAGENTA);
    printf("[B]");
    setColor(YELLOW);
    printf(" Back    ");
    setColor(LIGHTMAGENTA);
    printf("[H]");
    setColor(YELLOW);
    printf(" Highest Rate    ");
    setColor(LIGHTMAGENTA);
    printf("[M]");
    setColor(YELLOW);
    printf(" Main Menu     ");
    setColor(LIGHTMAGENTA);
    printf("[X]");
    setColor(YELLOW);
    printf(" Exit  ");
    resetColor();
    printf("||\n ===================================================================\n");
}

void askOpt2()
{
    printf("\n ===============================================");
    printf("\n ||   ");
    setColor(LIGHTMAGENTA);
    printf("[B]");
    setColor(YELLOW);
    printf(" Back    ");
    setColor(LIGHTMAGENTA);
    printf("[M]");
    setColor(YELLOW);
    printf(" Main Menu     ");
    setColor(LIGHTMAGENTA);
    printf("[X]");
    setColor(YELLOW);
    printf(" Exit  ");
    resetColor();
    printf("||\n ===============================================\n");
}
