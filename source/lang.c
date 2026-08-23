#include "lang.h"

Language g_currentLanguage = LANG_EN; // Default to English for new players!

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
    [STR_SAVED_MSG] = { "* GRA ZAPISANA! *", "* GAME SAVED! *", "* GESPEICHERT! *" },
    [STR_EXIT] = { "Wyjscie", "Exit Game", "Beenden" },
    [STR_OPTIONS] = { "Opcje", "Options", "Optionen" },
    [STR_BACK] = { "Powrot", "Back", "Zurueck" },
    [STR_SFX_VOLUME] = { "Dzwieki gry", "Game SFX", "Spiel-SFX" },
    [STR_UI_VOLUME] = { "Dzwieki menu", "UI SFX", "UI-SFX" },
    [STR_ASPECT_RATIO] = { "Format", "Aspect", "Format" },
    [STR_ASPECT_4_3] = { "4:3 Standard", "4:3 Standard", "4:3 Standard" },
    [STR_ASPECT_16_9] = { "16:9 Panorama", "16:9 Widescreen", "16:9 Breitbild" },
    [STR_HAVE] = { "Masz", "Have", "Besitz" },
    [STR_COST] = { "Koszt", "Cost", "Kosten" },
    [STR_BUTTON_PROMPTS] = { "Przyciski HUD", "Button Hints", "Tasten-Hilfe" },
    [STR_ON] = { "Wlaczone", "ON", "AN" },
    [STR_OFF] = { "Wylaczone", "OFF", "AUS" }
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

#include <strings.h>

typedef struct {
    const char* en;
    const char* pl;
    const char* de;
} ItemTranslation;

static const ItemTranslation item_translations[] = {
    // Resources
    { "Wood", "Drewno", "Holz" },
    { "Stone", "Kamien", "Stein" },
    { "Flower", "Kwiat", "Blume" },
    { "Acorn", "Zoladz", "Eichel" },
    { "Dirt", "Ziemia", "Erde" },
    { "Sand", "Piasek", "Sand" },
    { "Cactus", "Kaktus", "Kaktus" },
    { "Seeds", "Nasiona", "Samen" },
    { "Wheat", "Pszenica", "Weizen" },
    { "Bread", "Chleb", "Brot" },
    { "Apple", "Jablko", "Apfel" },
    { "COAL", "Wegiel", "Kohle" },
    { "Coal", "Wegiel", "Kohle" },
    { "I.ORE", "Ruda Zel.", "Eisenerz" },
    { "G.ORE", "Ruda Zlot", "Golderz" },
    { "IRON", "Zelazo", "Eisen" },
    { "Iron", "Zelazo", "Eisen" },
    { "GOLD", "Zloto", "Gold" },
    { "Gold", "Zloto", "Gold" },
    { "SLIME", "Szlam", "Schleim" },
    { "glass", "Szklo", "Glas" },
    { "Glass", "Szklo", "Glas" },
    { "cloth", "Welna", "Wolle" },
    { "Cloth", "Welna", "Wolle" },
    { "Hide", "Skora", "Leder" },
    { "Leather", "Skora", "Leder" },
    { "gem", "Klejnot", "Edelstein" },
    { "Gem", "Klejnot", "Edelstein" },
    { "Boat", "Lodka", "Boot" },
    { "L.Armor", "Skorz. Zbroja", "Lederrüstung" },
    { "I.Armor", "Zel. Zbroja", "Eisenrüstung" },
    { "G.Armor", "Zlot. Zbroja", "Goldrüstung" },
    { "Gem Armor", "Djam. Zbroja", "Diamantrüstung" },
    { "Bow", "Luk", "Bogen" },
    { "Arrow", "Strzala", "Pfeil" },
    { "Rod", "Wedka", "Angel" },
    { "Raw Fish", "Sur. Ryba", "Roher Fisch" },
    { "C.Fish", "Piecz. Ryba", "Gebr. Fisch" },
    { "Shears", "Nozyce", "Schere" },
    { "Carrot", "Marchew", "Karotte" },
    { "Potato", "Ziemniak", "Kartoffel" },
    { "Raw Beef", "Sur. Wolowina", "Rohes Rindfleisch" },
    { "Steak", "Stek", "Steak" },
    { "Raw Pork", "Sur. Wieprzow.", "Rohes Schwein" },
    { "C.Pork", "Pieczen", "Braten" },
    { "Plank", "Deska", "Holzplanke" },
    { "Wood Wall", "Drewn. Sciana", "Holzwand" },
    { "Egg", "Jajko", "Ei" },
    // Furniture
    { "Workbench", "Warsztat", "Werkbank" },
    { "Chest", "Skrzynia", "Truhe" },
    { "Anvil", "Kowadlo", "Amboss" },
    { "Furnace", "Piec Hutn.", "Schmelzofen" },
    { "Oven", "Piec Kaflowy", "Ofen" },
    { "Lantern", "Latarnia", "Laterne" },
    { "Bed", "Lozko", "Bett" },
    { "Door", "Drzwi", "Tuer" },
    { "Pow Glove", "Rekawica", "Krafthandschuh" },
    { "Power Glove", "Rekawica", "Krafthandschuh" },
    // Tools (Full & Abbreviated names)
    { "Wood Axe", "Drewn. Siekiera", "Holzaxt" },
    { "Rock Axe", "Kam. Siekiera", "Steinaxt" },
    { "Iron Axe", "Zel. Siekiera", "Eisenaxt" },
    { "Gold Axe", "Zlot. Siekiera", "Goldaxt" },
    { "Gem Axe", "Djam. Siekiera", "Diamantaxt" },
    { "Wood Pick", "Drewn. Kilof", "Holzspitzhacke" },
    { "Rock Pick", "Kam. Kilof", "Steinspitzhacke" },
    { "Iron Pick", "Zel. Kilof", "Eisenspitzhacke" },
    { "Gold Pick", "Zlot. Kilof", "Goldspitzhacke" },
    { "Gem Pick", "Djam. Kilof", "Diamantspitzhacke" },
    { "Wood Sword", "Drewn. Miecz", "Holzschwert" },
    { "Rock Sword", "Kam. Miecz", "Steinschwert" },
    { "Iron Sword", "Zel. Miecz", "Eisenschwert" },
    { "Gold Sword", "Zlot. Miecz", "Goldschwert" },
    { "Gem Sword", "Djam. Miecz", "Diamantschwert" },
    { "Wood Swrd", "Drewn. Miecz", "Holzschwert" },
    { "Rock Swrd", "Kam. Miecz", "Steinschwert" },
    { "Iron Swrd", "Zel. Miecz", "Eisenschwert" },
    { "Gold Swrd", "Zlot. Miecz", "Goldschwert" },
    { "Gem Swrd", "Djam. Miecz", "Diamantschwert" },
    { "Wood Shovel", "Drewn. Lopata", "Holzschaufel" },
    { "Rock Shovel", "Kam. Lopata", "Steinschaufel" },
    { "Iron Shovel", "Zel. Lopata", "Eisenschaufel" },
    { "Gold Shovel", "Zlot. Lopata", "Goldschaufel" },
    { "Gem Shovel", "Djam. Lopata", "Diamantschaufel" },
    { "Wood Shvl", "Drewn. Lopata", "Holzschaufel" },
    { "Rock Shvl", "Kam. Lopata", "Steinschaufel" },
    { "Iron Shvl", "Zel. Lopata", "Eisenschaufel" },
    { "Gold Shvl", "Zlot. Lopata", "Goldschaufel" },
    { "Gem Shvl", "Djam. Lopata", "Diamantschaufel" },
    { "Wood Hoe", "Drewn. Motyka", "Holzhacke" },
    { "Rock Hoe", "Kam. Motyka", "Steinhacke" },
    { "Iron Hoe", "Zel. Motyka", "Eisenhacke" },
    { "Gold Hoe", "Zlot. Motyka", "Goldhacke" },
    { "Gem Hoe", "Djam. Motyka", "Diamanthacke" },
    { NULL, NULL, NULL }
};

const char* lang_translate_item(const char* name) {
    if (!name) return "";
    if (g_currentLanguage == LANG_EN) return name;

    for (int i = 0; item_translations[i].en != NULL; ++i) {
        if (strcasecmp(name, item_translations[i].en) == 0) {
            if (g_currentLanguage == LANG_PL) return item_translations[i].pl;
            if (g_currentLanguage == LANG_DE) return item_translations[i].de;
            return item_translations[i].en;
        }
    }
    return name;
}
