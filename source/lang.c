#include "lang.h"

Language g_currentLanguage = LANG_PL; // Default to Polish as requested!

static const char* strings[STR_COUNT][LANG_COUNT] = {
    [STR_START_GAME] = { "Zacznij gre", "Start game", "Spiel starten" },
    [STR_NEW_GAME] = { "Nowa gra", "New Game", "Neues Spiel" },
    [STR_CONTINUE] = { "Wczytaj gre", "Continue", "Fortsetzen" },
    [STR_HOW_TO_PLAY] = { "Jak grac", "How to play", "Anleitung" },
    [STR_ABOUT] = { "O grze", "About", "Ueber" },
    [STR_LANGUAGE] = { "Jezyk: Polski", "Language: English", "Sprache: Deutsch" },
    [STR_SCREEN_MODE] = { "Ekran: 16:9 Panorama", "Screen: 16:9 Wide", "Bild: 16:9 Breit" },
    [STR_HELP_MSG] = { "(Krzyzak: Ruch, 2: Akcja, 1: Ekwipunek, +: Pauza)", "(D-Pad: Move, 2: Action, 1: Inventory, +: Pause)", "(D-Pad: Bewegen, 2: Aktion, 1: Inventar, +: Pause)" },
    [STR_INVENTORY] = { "Ekwipunek", "Inventory", "Inventar" },
    [STR_CRAFTING] = { "Tworzenie", "Crafting", "Handwerk" },
    [STR_YOU_DIED] = { "Zginales!", "You died!", "Du bist gestorben!" },
    [STR_YOU_WON] = { "Wygrales!", "You won!", "Gewonnen!" },
    [STR_TIME] = { "Czas", "Time", "Zeit" },
    [STR_SCORE] = { "Punkty", "Score", "Punkte" },
    [STR_PRESS_TO_RESTART] = { "Wcisnij 2 aby zaczac", "Press 2 to restart", "Druecke 2 zum Neustart" },
    [STR_PAUSE] = { "PAUZA", "PAUSED", "PAUSE" },
    [STR_RESUME] = { "Wroc do gry", "Resume Game", "Weiterspielen" },
    [STR_SAVE_GAME] = { "Zapisz gre", "Save Game", "Spiel speichern" },
    [STR_LOAD_GAME] = { "Wczytaj zapis", "Load Game", "Spiel laden" },
    [STR_QUIT_TITLE] = { "Zapisz i Wyjdz", "Save & Quit", "Speichern & Beenden" },
    [STR_QUIT_NO_SAVE] = { "Wyjdz (Bez zapisu)", "Quit (No Save)", "Beenden (Ohne speichern)" },
    [STR_SAVED_MSG] = { "* GRA ZAPISANA! *", "* GAME SAVED! *", "* GESPEICHERT! *" }
};

void lang_init(void) {
}

void lang_next(void) {
    g_currentLanguage = (Language)((g_currentLanguage + 1) % LANG_COUNT);
}

const char* lang_get(StringID id) {
    if (id < 0 || id >= STR_COUNT) return "";
    return strings[id][g_currentLanguage];
}
