// ============================================================================
//  DUNGEON CRAWLER  --  OOP Final Project  CSC-213
//  Instructor : Huda Sarfraz
//  Concepts   : Inheritance, Composition, Encapsulation,
//               Polymorphism, File I/O, OOP Design
//  AI Disclosure: Code generated with Claude (Anthropic)
// ============================================================================

#pragma once
#include <string>
using namespace std;
class Game {
public:
    Game(string t) : title(t) {}
    virtual void play() = 0;
    virtual ~Game() {}
    string getTitle() const { return title; }
private:
    string title;
};

// ============================================================================
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>

#ifdef _WIN32
  #include <windows.h>
  #include <conio.h>
  #define SLEEP_MS(ms)    Sleep(ms)
  #define BEEP_SFX(f,d)   Beep(f,d)
#else
  #include <unistd.h>
  #include <termios.h>
  #define SLEEP_MS(ms)    usleep((ms)*1000)
  #define BEEP_SFX(f,d)   /* no-op */
#endif

using namespace std;

// ============================================================================
//  ANSI COLOR & STYLE CODES
// ============================================================================
#define RST      "\033[0m"
#define BOLD     "\033[1m"
#define DIM      "\033[2m"
#define ITALIC   "\033[3m"

// Bright foreground
#define BRED     "\033[91m"
#define BGRN     "\033[92m"
#define BYLW     "\033[93m"
#define BBLU     "\033[94m"
#define BMAG     "\033[95m"
#define BCYN     "\033[96m"
#define BWHT     "\033[97m"

// Dark foreground
#define DRED     "\033[31m"
#define DGRN     "\033[32m"
#define DYLW     "\033[33m"
#define DBLU     "\033[34m"
#define DMAG     "\033[35m"
#define DCYN     "\033[36m"
#define DWHT     "\033[37m"
#define DGRY     "\033[90m"

// Backgrounds
#define BG_BLK   "\033[40m"
#define BG_RED   "\033[41m"
#define BG_GRN   "\033[42m"
#define BG_YLW   "\033[43m"
#define BG_BLU   "\033[44m"
#define BG_MAG   "\033[45m"
#define BG_CYN   "\033[46m"

// Cursor
#define CUR_HIDE "\033[?25l"
#define CUR_SHOW "\033[?25h"

// ── Box-drawing (ASCII safe fallback) ─────────────────────────────────────
#define TL "+" 
#define TR "+"
#define BL "+"
#define BR "+"
#define HZ "-"
#define VT "|"

// ============================================================================
//  THEME PALETTE  (semantic aliases for the dark-fantasy look)
// ============================================================================
// Gold accent  → BYLW / DYLW
// Purple dim   → DMAG / DGRY
// Cyan info    → BCYN
// Red danger   → BRED
// Green safe   → BGRN
// White text   → BWHT
// Dim border   → DGRY

// ============================================================================
//  TERMINAL HELPERS
// ============================================================================
void clrscr()  { cout << "\033[2J\033[H" << flush; }
void hideCur() { cout << CUR_HIDE << flush; }
void showCur() { cout << CUR_SHOW << flush; }

char getKey() {
#ifdef _WIN32
    while(_kbhit()) _getch();
    return (char)_getch();
#else
    termios oldt, raw;
    tcgetattr(STDIN_FILENO, &oldt);
    raw = oldt;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN]  = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    while(getchar() != EOF) {}
    raw.c_cc[VMIN] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    char c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
#endif
}

// ============================================================================
//  SOUND ENGINE
// ============================================================================
namespace SFX {
    void stepN()    { BEEP_SFX(500,30);  BEEP_SFX(540,30);  }
    void stepS()    { BEEP_SFX(340,30);  BEEP_SFX(300,30);  }
    void stepW()    { BEEP_SFX(380,30);  BEEP_SFX(350,30);  }
    void stepE()    { BEEP_SFX(420,30);  BEEP_SFX(460,30);  }
    void wall()     { BEEP_SFX(150,120); BEEP_SFX(130,120); }
    void tick()     { BEEP_SFX(600,18);  }
    void select()   { BEEP_SFX(660,40);  BEEP_SFX(880,50);  }
    void treasure() { BEEP_SFX(523,65);  BEEP_SFX(659,65);  BEEP_SFX(784,65); BEEP_SFX(1047,150); }
    void trap()     { BEEP_SFX(880,35);  BEEP_SFX(440,55);  BEEP_SFX(220,80); BEEP_SFX(110,160); }
    void lowHp()    { BEEP_SFX(220,90);  SLEEP_MS(50); BEEP_SFX(220,90); SLEEP_MS(50); BEEP_SFX(220,90); }
    void death()    { BEEP_SFX(392,120); BEEP_SFX(294,120); BEEP_SFX(196,120); BEEP_SFX(98,500); }
    void title()    { BEEP_SFX(330,80);  BEEP_SFX(440,80);  BEEP_SFX(523,100); BEEP_SFX(784,200); }
    void levelUp()  { BEEP_SFX(523,70);  BEEP_SFX(659,70);  BEEP_SFX(784,70);  BEEP_SFX(1047,120); }
    void ambient()  { BEEP_SFX(200,22);  SLEEP_MS(70); BEEP_SFX(185,18); }
    void victory()  {
        BEEP_SFX(523,120); BEEP_SFX(523,120); BEEP_SFX(523,120);
        BEEP_SFX(523,350); BEEP_SFX(415,350); BEEP_SFX(466,350);
        BEEP_SFX(523,700);
        SLEEP_MS(150);
        int hi[]={780,820,760,810,790,830,770,800};
        int lo[]={390,410,380,405,395,415,385,400};
        for(int b=0;b<4;b++){
            for(int i=0;i<8;i++){ BEEP_SFX(hi[i],22); BEEP_SFX(lo[i],18); }
            SLEEP_MS(160);
            for(int i=0;i<8;i++){ BEEP_SFX(hi[i],18); BEEP_SFX(lo[i],14); }
            SLEEP_MS(100);
        }
        BEEP_SFX(523,80); BEEP_SFX(659,80); BEEP_SFX(784,80);
        BEEP_SFX(1047,180); BEEP_SFX(1319,350);
    }
}

// ============================================================================
//  ANIMATION HELPERS
// ============================================================================
namespace Anim {

    void type(const string& s, int ms=14) {
        for(char c:s){ cout<<c<<flush; if(ms>0) SLEEP_MS(ms); }
    }
    void typeLine(const string& col, const string& s, int ms=12) {
        cout<<col; type(s,ms); cout<<RST<<"\n";
    }

    // Themed spinner with dark border style
    void spin(const string& lbl, int ticks=14, int ms=65) {
        const char* fr[]={"\\","|","/","-"};
        hideCur();
        for(int i=0;i<ticks;i++){
            cout<<"\r  "<<DGRY<<"["<<BCYN<<fr[i%4]<<DGRY<<"] "<<RST<<DWHT<<lbl<<RST<<"   "<<flush;
            SLEEP_MS(ms); SFX::tick();
        }
        cout<<"\r  "<<DGRY<<"["<<BGRN<<BOLD<<"+"<<RST<<DGRY<<"] "<<RST<<BGRN<<lbl<<RST<<"   \n";
        showCur();
    }

    // Progress bar — gold fill on dark bg
    void pbar(const string& lbl, int w=32, int ms=34) {
        hideCur();
        cout<<"\n  "<<BYLW<<BOLD<<lbl<<"\n  "<<RST;
        cout<<DGRY<<"  ["<<RST;
        for(int i=0;i<w;i++){
            SLEEP_MS(ms);
            int p=(i*100)/w;
            string c=(p<33)?DRED:(p<66)?DYLW:DGRN;
            cout<<c<<BOLD<<"#"<<RST<<flush;
            if(i%3==0) SFX::tick();
        }
        cout<<BGRN<<BOLD<<"] "<<RST<<BYLW<<BOLD<<"READY\n"<<RST;
        showCur();
    }

    void flash(const string& fg, const string& bg, const string& msg, int n=4) {
        for(int i=0;i<n;i++){
            cout<<"\r"<<fg<<bg<<BOLD<<"  "<<msg<<"  "<<RST<<flush;
            SLEEP_MS(200);
            cout<<"\r"<<string(msg.size()+6,' ')<<flush;
            SLEEP_MS(120);
        }
        cout<<"\r"<<fg<<bg<<BOLD<<"  "<<msg<<"  "<<RST<<"\n";
    }

    void countUp(int from, int to, const string& col, const string& sfx="") {
        int step=max(1,(to-from)/18);
        for(int v=from;v<=to;v+=step){
            cout<<"\r  "<<col<<BOLD<<setw(7)<<v<<RST<<" "<<DGRY<<sfx<<RST<<"   "<<flush;
            SLEEP_MS(35);
        }
        cout<<"\r  "<<col<<BOLD<<setw(7)<<to<<RST<<" "<<DGRY<<sfx<<RST<<"   \n";
    }

    void shake(const string& col, const string& txt, int n=5) {
        for(int i=0;i<n;i++){
            cout<<"\r"<<string(i%2,' ')<<col<<BOLD<<txt<<RST<<"    "<<flush;
            SLEEP_MS(75);
        }
        cout<<"\r"<<col<<BOLD<<txt<<RST<<"    \n";
    }

    // ── Themed header banner (dark fantasy style) ──────────────────────────
    void banner(const string& title, int w=76) {
        int inner = w - 4;
        int pad   = inner - (int)title.size();
        int lp = pad/2, rp = pad - lp;
        // Top border
        cout<<"\n  "<<DGRY<<BOLD<<"+"<<string(inner,'-')<<"+"<<RST<<"\n";
        // Title row — gold text on dim border
        cout<<"  "<<DGRY<<BOLD<<"|"<<RST
            <<BYLW<<BOLD<<string(lp,' ')<<title<<string(rp,' ')<<RST
            <<DGRY<<BOLD<<"|"<<RST<<"\n";
        // Bottom border
        cout<<"  "<<DGRY<<BOLD<<"+"<<string(inner,'-')<<"+"<<RST<<"\n\n";
    }

    void hline(char ch='-', int w=76) {
        cout<<"  "<<DGRY<<BOLD<<string(w,ch)<<RST<<"\n";
    }

    // Dim section sub-header
    void subhead(const string& s) {
        cout<<"\n  "<<DMAG<<BOLD<<s<<RST<<"\n";
        cout<<"  "<<DGRY<<string(s.size()+2,'-')<<RST<<"\n";
    }
}

// ============================================================================
//  ROOM  (Composition)
// ============================================================================
class Room {
public:
    enum Type { EMPTY, TREASURE, TRAP, EXIT, START };
    Room():type(EMPTY),visited(false),gold(0){}
    void setup(Type t,int g=0){type=t;gold=g;visited=false;}

    Type getType()    const{ return type; }
    bool isVisited()  const{ return visited; }
    int  getGold()    const{ return gold; }
    void markVisited()     { visited=true; }

    // 3-char token — shown only after visiting
    string token() const {
        if(!visited) return " ? ";
        switch(type){
            case START:    return "[S]";
            case EXIT:     return "[E]";
            case TREASURE: return "[$]";
            case TRAP:     return "[X]";
            default:       return "[ ]";
        }
    }

    // Color for the token — matches dark-fantasy palette
    string color() const {
        if(!visited) return string(DGRY)+DIM;
        switch(type){
            case START:    return string(BGRN)+BOLD;
            case EXIT:     return string(BYLW)+BOLD;
            case TREASURE: return string(DYLW)+BOLD;
            case TRAP:     return string(BRED)+BOLD;
            default:       return string(DGRY);
        }
    }

private:
    Type type; bool visited; int gold;
};

// ============================================================================
//  PLAYER  (Composition)
// ============================================================================
class Player {
public:
    explicit Player(string n="Hero")
        :name(n),hp(100),maxHp(100),gold(0),steps(0),traps(0),finds(0){}

    void damage(int d){ hp=max(0,hp-d); }
    void addGold(int g){ gold+=g; }
    void addStep(){ steps++; }
    void addTrap(){ traps++; }
    void addFind(){ finds++; }

    bool   alive()    const{ return hp>0; }
    bool   lowHp()    const{ return hp>0&&hp<=30; }
    int    getHp()    const{ return hp; }
    int    getGold()  const{ return gold; }
    int    getSteps() const{ return steps; }
    int    getTraps() const{ return traps; }
    int    getFinds() const{ return finds; }
    string getName()  const{ return name; }
    int    score()    const{ return gold*10+hp*2-traps*20+(steps>0?1000/steps:0); }

    // ── Build stat panel as vector<string> ──────────────────────────────────
    // Dark-fantasy themed: gold borders, purple labels, colored values
    vector<string> panel(int W=30) const {
        int inner = W - 4;

        string hcol = (hp>60) ? string(BGRN)+BOLD
                               : (hp>30) ? string(BYLW)+BOLD
                                         : string(BRED)+BOLD;
        string mood = (hp>75) ? "LEGENDARY"
                               : (hp>50) ? "  STRONG "
                                         : (hp>30) ? "   TIRED "
                                                   : "  DYING! ";
        int bars = (hp * inner) / maxHp;
        string bar = "";
        for(int i=0;i<inner;i++) bar += (i<bars) ? "#" : ".";

        auto border = [&]()->string{
            return string(DGRY)+BOLD+"  +"+string(inner+2,'-')+"+"+RST;
        };
        auto sectionDiv = [&]()->string{
            return string(DGRY)+BOLD+"  |"+string(inner+2,'=')+"|"+RST;
        };
        auto divider = [&]()->string{
            return string(DGRY)+BOLD+"  |"+string(inner+2,'-')+"|"+RST;
        };

        vector<string> v;
        v.push_back(border());

        // Title row
        {
            string t=" HERO STATUS ";
            int sp = inner+2-(int)t.size();
            v.push_back(string(DGRY)+BOLD+"  |"+RST
                +string(sp/2,' ')
                +BYLW+BOLD+t+RST
                +string(sp-sp/2,' ')
                +DGRY+BOLD+"|"+RST);
        }
        v.push_back(sectionDiv());

        // Name
        string nm = name.size()>(size_t)(inner-8) ? name.substr(0,inner-8) : name;
        {
            string line = "Hero   : " + nm;
            while((int)line.size()<inner) line+=' ';
            v.push_back(string(DGRY)+BOLD+"  | "+RST
                +DWHT+"Hero   : "+RST+BYLW+BOLD+nm+RST
                +DGRY+BOLD+" |"+RST);
        }

        // HP bar
        v.push_back(string(DGRY)+BOLD+"  | "+RST
            +DWHT+"HP     : "+RST
            +hcol+bar+RST
            +DGRY+BOLD+" |"+RST);

        // HP number + mood
        {
            string s = to_string(hp)+"/100  "+mood;
            while((int)s.size()<inner) s+=' ';
            v.push_back(string(DGRY)+BOLD+"  | "+RST+hcol+s+RST+DGRY+BOLD+" |"+RST);
        }

        v.push_back(divider());

        auto row=[&](const string& lbl, const string& col, const string& val)->string{
            string s = lbl+val;
            while((int)s.size()<inner) s+=' ';
            return string(DGRY)+BOLD+"  | "+RST
                +DWHT+lbl+RST+col+val+RST
                +DGRY+BOLD+" |"+RST;
        };

        v.push_back(row("Gold   : ", BYLW+string(BOLD), to_string(gold)+" coins  "));
        v.push_back(row("Steps  : ", BCYN+string(BOLD), to_string(steps)+"         "));
        v.push_back(row("Traps  : ", BRED+string(BOLD), to_string(traps)+"         "));
        v.push_back(row("Finds  : ", DYLW+string(BOLD), to_string(finds)+"         "));

        v.push_back(divider());

        v.push_back(row("SCORE  : ", BGRN+string(BOLD), to_string(score())+" pts  "));

        v.push_back(border());
        return v;
    }

    void printStats() const {
        for(auto& l:panel()) cout<<l<<"\n";
        cout<<"\n";
    }

private:
    string name; int hp,maxHp,gold,steps,traps,finds;
};

// ============================================================================
//  LEADERBOARD  (File I/O)
// ============================================================================
class Leaderboard {
public:
    struct Entry{ string name; int score,gold,steps; bool won; };

    static void save(const string& n,int sc,int g,int st,bool w){
        ofstream f("dungeon_scores.txt",ios::app);
        if(f.is_open()) f<<n<<"|"<<sc<<"|"<<g<<"|"<<st<<"|"<<(w?"WON":"DIED")<<"\n";
    }

    static vector<Entry> load(){
        vector<Entry> ev; ifstream f("dungeon_scores.txt");
        if(!f.is_open()) return ev;
        string line;
        while(getline(f,line)){
            if(line.empty()) continue;
            Entry e; e.won=false; e.score=0; e.gold=0; e.steps=0;
            stringstream ss(line); string tok; int col=0;
            while(getline(ss,tok,'|')){
                try{
                    if(col==0)      e.name=tok;
                    else if(col==1) e.score=stoi(tok);
                    else if(col==2) e.gold=stoi(tok);
                    else if(col==3) e.steps=stoi(tok);
                    else if(col==4) e.won=(tok=="WON");
                } catch(...){}
                col++;
            }
            if(col>=5) ev.push_back(e);
        }
        sort(ev.begin(),ev.end(),[](const Entry&a,const Entry&b){return a.score>b.score;});
        return ev;
    }

    static void display(){
        auto ev = load();
        cout<<"\n";
        Anim::banner("  HALL OF FAME  --  TOP DUNGEON HEROES  ", 72);

        // Column headers
        cout<<"  "<<DGRY<<BOLD<<"|"<<RST
            <<"  "<<DMAG<<BOLD<<setw(5)<<"RANK"<<RST
            <<"  "<<DGRY<<BOLD<<"|"<<RST
            <<"  "<<DWHT<<BOLD<<setw(15)<<left<<"NAME"<<RST
            <<"  "<<DGRY<<BOLD<<"|"<<RST
            <<"  "<<BGRN<<BOLD<<setw(7)<<right<<"SCORE"<<RST
            <<"  "<<DGRY<<BOLD<<"|"<<RST
            <<"  "<<BYLW<<BOLD<<setw(5)<<"GOLD"<<RST
            <<"  "<<DGRY<<BOLD<<"|"<<RST
            <<"  "<<BCYN<<BOLD<<setw(5)<<"STEPS"<<RST
            <<"  "<<DGRY<<BOLD<<"|"<<RST
            <<"  "<<DWHT<<BOLD<<setw(6)<<"RESULT"<<RST
            <<"  "<<DGRY<<BOLD<<"|\n"<<RST;
        Anim::hline('-', 72);

        if(ev.empty()){
            cout<<"  "<<DGRY<<"  No scores yet — be the first hero to escape!\n"<<RST;
        } else {
            for(int i=0;i<(int)ev.size()&&i<10;i++){
                auto& e=ev[i];
                string rk=(i==0)?"  #1  ":(i==1)?"  #2  ":(i==2)?"  #3  ":"  #"+to_string(i+1)+" ";
                string rc=(i==0)?BYLW:(i==1)?BWHT:DCYN;
                string res = e.won ? "  WON " : "  DIED";
                string resc = e.won ? BGRN : BRED;
                string nm = e.name.size()>15 ? e.name.substr(0,15) : e.name;
                while((int)nm.size()<15) nm+=' ';
                cout<<"  "<<DGRY<<BOLD<<"|"<<RST
                    <<rc<<BOLD<<rk<<RST
                    <<"  "<<DGRY<<BOLD<<"|"<<RST
                    <<"  "<<DWHT<<nm<<"  "<<RST
                    <<DGRY<<BOLD<<"|"<<RST
                    <<BGRN<<BOLD<<right<<setw(7)<<e.score<<"  "<<RST
                    <<DGRY<<BOLD<<"|"<<RST
                    <<BYLW<<right<<setw(6)<<e.gold<<"  "<<RST
                    <<DGRY<<BOLD<<"|"<<RST
                    <<BCYN<<right<<setw(6)<<e.steps<<"  "<<RST
                    <<DGRY<<BOLD<<"|"<<RST
                    <<resc<<BOLD<<res<<RST
                    <<"  "<<DGRY<<BOLD<<"|\n"<<RST;
            }
        }
        Anim::hline('=', 72);
        cout<<"\n";
    }
};

// ============================================================================
//  DUNGEON CRAWLER  (derived from Game — Polymorphism)
// ============================================================================
class DungeonCrawler : public Game {
public:
    DungeonCrawler() : Game("Dungeon Crawler"),
        rows(6),cols(6),px(0),py(0),
        gameWon(false),lastMsg(""),lastCol(BWHT),moveCount(0){
        srand((unsigned)time(nullptr));
        buildDungeon();
    }

    void play() override {
        #ifdef _WIN32
        HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD m=0; GetConsoleMode(h,&m);
        SetConsoleMode(h,m|0x0004);
        #endif
        hideCur();
        showTitle();
        showInstructions();
        showLoading();
        setupPlayer();
        gameLoop();
        showEndScreen();
        showCur();
    }

private:
    static const int MR=6, MC=6;
    Room  grid[MR][MC];
    int   rows,cols,px,py;
    bool  gameWon;
    Player player;
    string lastMsg, lastCol;
    int    moveCount;

    // ── Narrative text pools ────────────────────────────────────────────────
    const string walkTxt[10]={
        "Your boots scrape on cold stone.",
        "A distant drip echoes in the dark.",
        "Torchlight flickers as you advance.",
        "The air smells of damp and rot.",
        "Something skitters just out of sight.",
        "Dust rises — this place is ancient.",
        "The walls seem to press closer.",
        "A cold draft brushes your neck.",
        "Shadows dance around your torch.",
        "Each step takes you deeper in."
    };
    const string wallTxt[5]={
        "THUD! Solid stone wall. Wrong way!",
        "You slam into cold rock. No way through.",
        "Dead end! The stone does not yield.",
        "Your hand finds only solid rock.",
        "Your torch reveals nothing but stone."
    };
    const string revisitTxt[5]={
        "Your own footprints are still here.",
        "Already searched. Nothing new.",
        "Been here. The room is empty.",
        "The same old candle stub in the corner.",
        "You scan again. Still nothing."
    };
    const string flav[14]={
        "Something watches from the dark.",
        "The dungeon breathes — alive and hungry.",
        "Every room could be your last.",
        "The torchlight barely holds the dark back.",
        "Silence presses in from all sides.",
        "The exit is out there. Somewhere.",
        "Your heart pounds in your chest.",
        "Keep moving. Stopping means dying.",
        "The cold walls offer no comfort.",
        "Distant sounds echo. Hard to trace.",
        "The dungeon shifts. Nothing is what it seems.",
        "Gold, glory, or death — all await ahead.",
        "The further you go, the darker it gets.",
        "You swallow hard and push on."
    };

    // ── Build dungeon ────────────────────────────────────────────────────────
    void buildDungeon(){
        grid[0][0].setup(Room::START); grid[0][0].markVisited();
        grid[rows-1][cols-1].setup(Room::EXIT);
        vector<pair<int,int>> cells;
        for(int r=0;r<rows;r++)
            for(int c=0;c<cols;c++)
                if(!(r==0&&c==0)&&!(r==rows-1&&c==cols-1))
                    cells.push_back({r,c});
        for(int i=(int)cells.size()-1;i>0;i--){
            int j=rand()%(i+1); swap(cells[i],cells[j]);
        }
        int tot=(int)cells.size();
        for(int i=0;i<tot;i++){
            int r=cells[i].first, c=cells[i].second;
            if     (i<tot/3)      grid[r][c].setup(Room::TREASURE,40+rand()%121);
            else if(i<tot*2/3)    grid[r][c].setup(Room::TRAP);
            else                  grid[r][c].setup(Room::EMPTY);
        }
        px=py=0;
    }

    // ── TITLE SCREEN ─────────────────────────────────────────────────────────
    void showTitle(){
        clrscr();
        cout<<"\n\n";

        // Torch-glow decorative top line
        cout<<"  "<<DGRY<<BOLD<<string(70,'=')<<RST<<"\n";
        cout<<"  "<<DGRY<<BOLD<<"|"<<RST
            <<BYLW<<BOLD<<"  ~ ~ ~  "<<RST
            <<DGRY<<DIM<<"torchlight flickers in the darkness"<<RST
            <<BYLW<<BOLD<<"  ~ ~ ~  "<<RST
            <<DGRY<<BOLD<<"|"<<RST<<"\n";
        cout<<"  "<<DGRY<<BOLD<<string(70,'=')<<RST<<"\n\n";

        // Block-art title — gold on dark
        string art1[]={
"   #####   #    #  #    #   ####   ######   ####   #    #",
"   #    #  #    #  ##   #  #    #  #       #    #  ##   #",
"   #    #  #    #  # #  #  #       #####   #    #  # #  #",
"   #    #  #    #  #  # #  #  ###  #       #    #  #  # #",
"   #    #  #    #  #   ##  #    #  #       #    #  #   ##",
"   #####    ####   #    #   ####   ######   ####   #    #"
        };
        string art2[]={
"   ####  #####    ##   #    #  #      ######  #####  ",
"  #    # #    #  #  #  #    #  #      #       #    # ",
"  #      #    # #    # #    #  #      #####   #    # ",
"  #      #####  ###### # ## #  #      #       #####  ",
"  #    # #   #  #    # ##  ##  #      #       #   #  ",
"   ####  #    # #    # #    #  ###### ######  #    # "
        };
        for(auto& l:art1){ cout<<BYLW<<BOLD<<l<<RST<<"\n"; SLEEP_MS(60); }
        cout<<"\n";
        for(auto& l:art2){ cout<<DMAG<<BOLD<<l<<RST<<"\n"; SLEEP_MS(60); }

        cout<<"\n  "<<DGRY<<BOLD<<string(70,'-')<<RST<<"\n";
        cout<<"  "<<DWHT<<BOLD;
        Anim::type("   OOP Final Project  |  CSC-213  |  Instructor: Huda Sarfraz",13);
        cout<<RST<<"\n";
        cout<<"  "<<DGRY<<BOLD<<string(70,'-')<<RST<<"\n\n";

        // Blinking prompt
        for(int i=0;i<4;i++){
            string s = (i%2==0)
                ? "   [ Press any key to enter the dungeon ]"
                : "   [ Press any key to enter the dungeon  ";
            cout<<"\r  "<<BYLW<<BOLD<<s<<RST<<"  "<<flush;
            SLEEP_MS(480);
        }
        cout<<"\n"; SFX::title(); getKey();
    }

    // ── INSTRUCTIONS ─────────────────────────────────────────────────────────
    void showInstructions(){
        clrscr(); cout<<"\n";
        Anim::banner("  HOW TO PLAY  --  DUNGEON CRAWLER  ", 70);

        Anim::subhead("THE STORY");
        cout<<DWHT
            <<"  You are a hero trapped inside a deadly dungeon.\n"
            <<"  Treasure glitters in hidden rooms, but deadly traps lurk at every turn.\n"
            <<"  Your only goal: reach the EXIT and escape with your life!\n\n"<<RST;

        Anim::subhead("OBJECTIVE");
        cout<<DGRY<<"  "<<BGRN<<BOLD<<">"<<RST<<DWHT<<" Start at [S] in the top-left corner.\n"<<RST;
        cout<<DGRY<<"  "<<BGRN<<BOLD<<">"<<RST<<DWHT<<" Navigate the 6 x 6 dungeon grid room by room.\n"<<RST;
        cout<<DGRY<<"  "<<BGRN<<BOLD<<">"<<RST<<DWHT<<" Collect gold from Treasure rooms [$].\n"<<RST;
        cout<<DGRY<<"  "<<BGRN<<BOLD<<">"<<RST<<BRED<<" Avoid Trap rooms [X] — they drain your HP badly.\n"<<RST;
        cout<<DGRY<<"  "<<BGRN<<BOLD<<">"<<RST<<BYLW<<BOLD<<" Reach EXIT [E] at the bottom-right corner to WIN.\n\n"<<RST;

        Anim::subhead("CONTROLS  (tap the key — no Enter needed!)");
        auto key=[](const string& k, const string& desc){
            cout<<"  "<<BYLW<<BOLD<<"["<<k<<"]"<<RST
                <<DGRY<<"  "<<RST<<DWHT<<desc<<RST<<"\n";
        };
        key("W","Move North   (up)");
        key("S","Move South   (down)");
        key("A","Move West    (left)");
        key("D","Move East    (right)");
        key("L","Open the Hall of Fame leaderboard");
        key("Q","Quit and save your score");

        Anim::subhead("MAP LEGEND");
        auto leg=[](const string& tok, const string& tc, const string& desc){
            cout<<"  "<<tc<<BOLD<<tok<<RST<<"  "<<DWHT<<desc<<RST<<"\n";
        };
        leg("[@]",BMAG,    "YOU  — your current position");
        leg("[?]",DGRY,    "Unknown room — enter to reveal it");
        leg("[S]",BGRN,    "Starting room");
        leg("[E]",BYLW,    "EXIT — reach this to WIN! Always bottom-right.");
        leg("[$]",DYLW,    "Treasure room — collect gold coins");
        leg("[X]",BRED,    "Trap room — lose 15–30 HP on entry");
        leg("[ ]",DGRY,    "Empty room — safe");

        Anim::subhead("SCORING");
        cout<<DWHT<<"  Score = (gold x10) + (HP x2) - (traps x20) + (1000 / steps)\n\n"<<RST;

        Anim::subhead("TIPS");
        cout<<DCYN<<"  > All rooms are hidden until you physically step inside.\n"
            <<"  > The EXIT is always at the very last row and column.\n"
            <<"  > Collect as much gold as possible before reaching the exit.\n"
            <<"  > Red HP bar means danger — one more trap could kill you!\n"
            <<"  > Press [L] any time to view the leaderboard.\n\n"<<RST;

        Anim::hline('-', 70);
        cout<<"  "<<DWHT<<"Press any key to continue..."<<RST<<"\n";
        Anim::hline('-', 70);
        SFX::select(); getKey();
    }

    // ── LOADING SCREEN ───────────────────────────────────────────────────────
    void showLoading(){
        clrscr(); cout<<"\n";
        Anim::banner("  GENERATING YOUR DUNGEON  ", 54);
        Anim::spin("Carving dungeon chambers   ", 14, 62);
        Anim::spin("Placing hidden treasure    ", 12, 65);
        Anim::spin("Arming deadly traps        ", 12, 65);
        Anim::spin("Hiding the secret exit     ", 10, 70);
        Anim::spin("Lighting the torches       ",  8, 72);
        cout<<"\n";
        Anim::pbar("  Dungeon loading...", 30, 34);
        SLEEP_MS(300); clrscr(); cout<<"\n\n\n";
        Anim::flash(BWHT, BG_GRN, "  DUNGEON READY  --  GOOD LUCK, HERO!  ", 5);
        SLEEP_MS(300);
    }

    // ── PLAYER NAME SETUP ────────────────────────────────────────────────────
    void setupPlayer(){
        clrscr(); cout<<"\n\n";
        cout<<DMAG<<BOLD; Anim::type("  Welcome, brave adventurer!\n\n", 24); cout<<RST;
        cout<<BYLW<<BOLD; Anim::type("  Enter your hero's name: ", 18); cout<<RST<<BWHT;
        showCur();
        string name; cin>>name; cin.ignore(1000,'\n');
        cout<<RST; hideCur();
        if(name.empty()||name.size()>16) name="Hero";
        player = Player(name);
        cout<<"\n  "<<BGRN<<BOLD;
        Anim::type("  The dungeon awaits you, "+name+"!",18);
        cout<<RST<<"\n\n"; SFX::levelUp(); SLEEP_MS(700);
    }

    // ── MAIN GAME SCREEN ─────────────────────────────────────────────────────
    void drawScreen(){
        clrscr();

        // ── TOP BANNER ──────────────────────────────────────────────────────
        cout<<DGRY<<BOLD
            <<"  +====================================================================+"<<"\n"
            <<"  | "<<RST<<BYLW<<BOLD
            <<"  DUNGEON CRAWLER  "<<RST<<DGRY
            <<"  CSC-213  |  Instructor: Huda Sarfraz  "<<RST
            <<"                  "<<DGRY<<BOLD<<" |\n"
            <<"  +====================================================================+"<<RST<<"\n";

        // ── MAP LINES ───────────────────────────────────────────────────────
        vector<string> mapL;
        mapL.push_back(string(DGRY)+BOLD+"  +=====  DUNGEON MAP  =====+"+RST);
        mapL.push_back("");

        // Column labels
        {
            string h = "        ";
            for(int c=0;c<cols;c++) h+=" C"+to_string(c)+"    ";
            mapL.push_back(string(DGRY)+h+RST);
        }

        for(int r=0;r<rows;r++){
            // Row divider
            { string s="  "; for(int c=0;c<cols;c++) s+="+-----"; s+="+";
              mapL.push_back(string(DGRY)+BOLD+s+RST); }
            // Cells
            {
                string s="  ";
                for(int c=0;c<cols;c++){
                    bool ip=(r==px&&c==py);
                    if(ip){
                        s += string(DGRY)+BOLD+"|"+RST
                            +BMAG+BOLD+" [@] "+RST;
                    } else {
                        string tok = grid[r][c].token();
                        string col = grid[r][c].color();
                        s += string(DGRY)+BOLD+"|"+RST
                            +col+" "+tok+" "+RST;
                    }
                }
                s += string(DGRY)+BOLD+"|"+RST;
                mapL.push_back(s);
            }
        }
        // Bottom row border
        { string s="  "; for(int c=0;c<cols;c++) s+="+-----"; s+="+";
          mapL.push_back(string(DGRY)+BOLD+s+RST); }

        // Legend row under map
        mapL.push_back(
            string("  ")+BMAG+BOLD+"[@]"+RST+DGRY+" You  "+RST
            +DYLW+BOLD+"[$]"+RST+DGRY+" Gold  "+RST
            +BRED+BOLD+"[X]"+RST+DGRY+" Trap  "+RST
            +BYLW+BOLD+"[E]"+RST+DGRY+" Exit  "+RST
            +DGRY+DIM+"[?]"+RST+DGRY+" Hidden"+RST);

        // ── SIDE-BY-SIDE: stat panel (left) | map (right) ──────────────────
        vector<string> statL = player.panel(30);
        cout<<"\n";
        size_t tot = max(mapL.size(), statL.size());
        for(size_t i=0;i<tot;i++){
            if(i<statL.size()) cout<<statL[i];
            else cout<<string(36,' ');
            cout<<"  ";
            if(i<mapL.size()) cout<<mapL[i];
            cout<<"\n";
        }
        cout<<"\n";

        // ── MESSAGE BOX ─────────────────────────────────────────────────────
        if(!lastMsg.empty()){
            Anim::hline('-', 68);
            // Word-wrap at 62 chars
            string msg = lastMsg;
            while(!msg.empty()){
                string ln = msg.substr(0,62);
                msg = (msg.size()>62) ? msg.substr(62) : "";
                while((int)ln.size()<62) ln+=' ';
                cout<<"  "<<DGRY<<BOLD<<"| "<<RST
                    <<lastCol<<BOLD<<ln<<RST
                    <<DGRY<<BOLD<<" |\n"<<RST;
            }
            Anim::hline('-', 68);
            lastMsg="";
        }

        // ── CONTROLS BAR ────────────────────────────────────────────────────
        cout<<"\n";
        Anim::hline('=', 68);
        cout<<"  "
            <<BYLW<<BOLD<<"[W]"<<RST<<DGRY<<" Up    "
            <<BYLW<<BOLD<<"[A]"<<RST<<DGRY<<" Left  "
            <<BYLW<<BOLD<<"[S]"<<RST<<DGRY<<" Down  "
            <<BYLW<<BOLD<<"[D]"<<RST<<DGRY<<" Right    "
            <<BCYN<<BOLD<<"[L]"<<RST<<DGRY<<" Leaderboard    "
            <<BRED<<BOLD<<"[Q]"<<RST<<DGRY<<" Quit"<<RST<<"\n";
        Anim::hline('=', 68);
        cout<<"\n  "<<BYLW<<BOLD<<"  > Your move: "<<RST;
    }

    // ── ROOM EVENT HANDLER ────────────────────────────────────────────────────
    void handleRoom(){
        Room& room = grid[px][py];
        if(room.isVisited()&&room.getType()!=Room::EXIT){
            lastMsg=revisitTxt[rand()%5]; lastCol=DGRY; return;
        }
        room.markVisited();
        switch(room.getType()){
        case Room::START:
            lastMsg = "[S] You stand at the dungeon entrance. Find the EXIT at bottom-right!";
            lastCol = BGRN; break;

        case Room::TREASURE:{
            int g=room.getGold(); player.addGold(g); player.addFind();
            SFX::treasure();
            Anim::flash(BWHT,BG_YLW,"  [$] TREASURE!  +"+to_string(g)+" gold coins!  ",4);
            lastMsg = "[$] Glittering coins spill out! You pocket "+to_string(g)+" gold!";
            lastCol = BYLW; break;
        }
        case Room::TRAP:{
            int d=15+rand()%16; player.damage(d); player.addTrap();
            SFX::trap();
            Anim::shake(BRED,"  [X] TRAP!  -"+to_string(d)+" HP!");
            if(!player.alive()){
                SFX::death();
                lastMsg = "[X] The trap proved fatal. The darkness takes you...";
                lastCol = BRED;
            } else {
                if(player.lowHp()) SFX::lowHp();
                string m[]={
                    "[!] Spikes from the floor! -"+to_string(d)+" HP!",
                    "[!] A hidden blade swings! -"+to_string(d)+" HP!",
                    "[!] The floor crumbles! You scramble out. -"+to_string(d)+" HP!",
                    "[!] Poison darts! -"+to_string(d)+" HP!",
                    "[!] A boulder swings down! -"+to_string(d)+" HP!",
                };
                lastMsg=m[rand()%5]; lastCol=BRED;
            }
            break;
        }
        case Room::EXIT:
            gameWon=true;
            SFX::victory();
            Anim::flash(BWHT,BG_GRN,"  EXIT FOUND!  YOU WIN!  CONGRATULATIONS!  ",6);
            lastMsg = "[E] Sunlight bursts through the dungeon door! You are FREE!";
            lastCol = BGRN; break;

        default:{
            string m[]={
                "[ ] Barren room. Only dust and silence.",
                "[ ] Broken furniture. Nothing useful.",
                "[ ] Old bones in the corner. A grim warning.",
                "[ ] Cobwebs everywhere. Long abandoned.",
                "[ ] A faded mural on the wall. Nothing here.",
                "[ ] You search carefully. Safe, but empty.",
            };
            lastMsg=m[rand()%6]; lastCol=DGRY; break;
        }
        }
    }

    // ── MOVEMENT ──────────────────────────────────────────────────────────────
    bool doMove(char d){
        int nr=px, nc=py;
        switch(d){
            case 'w': nr--; break; case 's': nr++; break;
            case 'a': nc--; break; case 'd': nc++; break;
            default: return false;
        }
        if(nr<0||nr>=rows||nc<0||nc>=cols){
            SFX::wall(); lastMsg=wallTxt[rand()%5]; lastCol=BRED; return false;
        }
        if     (d=='w') SFX::stepN();
        else if(d=='s') SFX::stepS();
        else if(d=='a') SFX::stepW();
        else            SFX::stepE();

        string dir=(d=='w')?"North":(d=='s')?"South":(d=='a')?"West":"East";
        lastMsg = "-> "+dir+": "+walkTxt[moveCount%10]+" "+flav[rand()%14];
        lastCol = DCYN; moveCount++;
        px=nr; py=nc; player.addStep();
        return true;
    }

    // ── GAME LOOP ──────────────────────────────────────────────────────────────
    void gameLoop(){
        lastMsg = "[S] You enter the dungeon. The heavy door slams shut behind you!";
        lastCol = BGRN;
        while(player.alive()&&!gameWon){
            drawScreen();
            SFX::ambient();
            char cmd = getKey();
            cout<<BYLW<<BOLD<<cmd<<RST<<"\n";
            cmd = tolower(cmd);

            if(cmd=='q'){
                lastMsg = "You turn and flee the dungeon. The door shuts forever.";
                lastCol = DMAG;
                drawScreen(); SLEEP_MS(1400); break;
            }
            if(cmd=='l'){
                clrscr();
                Leaderboard::display();
                cout<<"  "<<DWHT<<BOLD<<"Press any key to return...\n"<<RST;
                SFX::select(); getKey(); continue;
            }
            if(cmd=='w'||cmd=='s'||cmd=='a'||cmd=='d'){
                if(doMove(cmd)) handleRoom();
            }
        }
    }

    // ── Helper: draw victory/defeat flash frame in-place ─────────────────────
    void printWinFrame(const string& col, const string& msg, const string& sub, bool bright){
        cout<<"\033[H"<<flush;
        cout<<"\n";
        cout<<"  "<<col<<BOLD<<string(64,'*')<<RST<<"\n";
        cout<<"  "<<col<<BOLD<<string(64,'*')<<RST<<"\n\n";
        int pad=(64-(int)msg.size())/2;
        cout<<"  "<<col<<BOLD<<string(pad,' ')<<msg<<string(64-(int)msg.size()-pad,' ')<<RST<<"\n\n";
        int pad2=(64-(int)sub.size())/2;
        cout<<"  "<<(bright?BWHT:string(DGRY))<<BOLD<<string(pad2,' ')<<sub<<RST<<"\n\n";
        cout<<"  "<<col<<BOLD<<string(64,'*')<<RST<<"\n";
        cout<<"  "<<col<<BOLD<<string(64,'*')<<RST<<"\n"<<flush;
    }

    // ── END SCREEN ────────────────────────────────────────────────────────────
    void showEndScreen(){
        clrscr(); SLEEP_MS(200);

        if(gameWon){
            SFX::victory();
            string cols[]={BGRN,BYLW,BWHT,BGRN,BYLW,BWHT,BGRN,BYLW};
            string msgs[]={
                "  **  VICTORY!  YOU ESCAPED ALIVE!  **  ",
                "  **  VICTORY!  YOU ESCAPED ALIVE!  **  ",
                "  >>>  DUNGEON  CLEARED!  WELL DONE!  <<<",
                "  **  VICTORY!  YOU ESCAPED ALIVE!  **  ",
                "  >>>  DUNGEON  CLEARED!  WELL DONE!  <<<",
                "  **  VICTORY!  YOU ESCAPED ALIVE!  **  ",
                "  >>>  DUNGEON  CLEARED!  WELL DONE!  <<<",
                "  **  VICTORY!  YOU ESCAPED ALIVE!  **  ",
            };
            for(int i=0;i<8;i++){
                printWinFrame(cols[i],msgs[i],"You escaped with your life!",i%2==0);
                SLEEP_MS(300);
            }

            clrscr(); cout<<"\n";
            cout<<"  "<<BYLW<<BOLD<<string(64,'=')<<RST<<"\n";
            cout<<"  "<<BYLW<<BOLD<<"|"<<RST
                <<"  "<<BGRN<<BOLD<<"   *****  DUNGEON CRAWLER  --  VICTORY  *****   "<<RST
                <<"  "<<BYLW<<BOLD<<"|"<<RST<<"\n";
            cout<<"  "<<BYLW<<BOLD<<string(64,'=')<<RST<<"\n\n";

            string trophy[]={
                "              _______________________",
                "             |                       |",
                "             |    DUNGEON CHAMPION   |",
                "             |    *** YOU WIN! ***   |",
                "             |_______________________|",
                "                       |||",
                "               ________|_|________",
                "              |___________________|",
                "              |  HALL  OF  FAME   |",
                "              |___________________|",
            };
            for(auto& l:trophy){ cout<<BYLW<<BOLD<<"  "<<l<<RST<<"\n"; SLEEP_MS(80); }
            cout<<"\n";
            Anim::typeLine(BGRN, "  Congratulations! You are a true dungeon champion!",20);
            Anim::typeLine(DWHT, "  You escaped with your life, your gold, and your glory.");
            Anim::typeLine(DMAG, "  The bards will sing your name through the ages!");
            cout<<"\n  "<<BYLW<<BOLD<<string(64,'=')<<RST<<"\n";

        } else {
            SFX::death();
            string cols[]={BRED,DMAG,BRED,DMAG,BRED,DMAG,BRED,DMAG};
            string msgs[]={
                "  [X]  GAME OVER  --  YOU HAVE PERISHED  [X]  ",
                "  [X]  GAME OVER  --  YOU HAVE PERISHED  [X]  ",
                "  >>>  THE DUNGEON HAS CLAIMED YOUR SOUL  <<<  ",
                "  [X]  GAME OVER  --  YOU HAVE PERISHED  [X]  ",
                "  >>>  THE DUNGEON HAS CLAIMED YOUR SOUL  <<<  ",
                "  [X]  GAME OVER  --  YOU HAVE PERISHED  [X]  ",
                "  >>>  THE DUNGEON HAS CLAIMED YOUR SOUL  <<<  ",
                "  [X]  GAME OVER  --  YOU HAVE PERISHED  [X]  ",
            };
            for(int i=0;i<8;i++){
                printWinFrame(cols[i],msgs[i],"The darkness swallows you whole...",i%2==0);
                SLEEP_MS(300);
            }

            clrscr(); cout<<"\n";
            cout<<"  "<<BRED<<BOLD<<string(64,'=')<<RST<<"\n";
            cout<<"  "<<BRED<<BOLD<<"|"<<RST
                <<"  "<<BRED<<BOLD<<"  *********  DUNGEON CRAWLER  --  GAME OVER  ******* "<<RST
                <<"  "<<BRED<<BOLD<<"|"<<RST<<"\n";
            cout<<"  "<<BRED<<BOLD<<string(64,'=')<<RST<<"\n\n";

            string skull[]={
                "              ___________________",
                "             /                   \\",
                "            |   REST IN PIECES   |",
                "            |                    |",
                "            |   (o)         (o)  |",
                "            |        \\___/        |",
                "             \\_________________/",
                "                    |||||",
                "             _______|||||_______",
                "            |___________________|",
                "            |  [X]  DEFEATED  [X] |",
                "            |___________________|",
            };
            for(auto& l:skull){ cout<<BRED<<BOLD<<"  "<<l<<RST<<"\n"; SLEEP_MS(75); }
            cout<<"\n";
            Anim::typeLine(BRED,  "  You were not strong enough for the dungeon.");
            Anim::typeLine(DWHT,  "  The dungeon has claimed another soul.");
            Anim::typeLine(DGRY,  "  Better luck next time, brave adventurer...");
            cout<<"\n  "<<BRED<<BOLD<<string(64,'=')<<RST<<"\n";
        }

        // ── SCORE TALLY ──────────────────────────────────────────────────────
        cout<<"\n";
        Anim::hline('=', 58);
        cout<<"  "<<DGRY<<BOLD<<"|"<<RST<<BYLW<<BOLD
            <<"          FINAL SCORE BREAKDOWN           "<<RST
            <<DGRY<<BOLD<<"|"<<RST<<"\n";
        Anim::hline('=', 58);

        auto statRow=[](const string& lbl, const string& col, int val, const string& sfx){
            cout<<"  "<<DGRY<<BOLD<<"| "<<RST<<DWHT<<BOLD<<lbl<<RST;
            Anim::countUp(0,val,col,sfx);
        };
        statRow("Gold collected   :  ", BYLW,  player.getGold(),  "coins  ");
        statRow("Steps taken      :  ", BCYN,  player.getSteps(), "steps  ");
        statRow("Traps triggered  :  ", BRED,  player.getTraps(), "traps  ");
        statRow("Treasures found  :  ", BYLW,  player.getFinds(), "rooms  ");

        Anim::hline('-', 58);
        cout<<"  "<<DGRY<<BOLD<<"| "<<RST<<DWHT<<BOLD<<"FINAL SCORE      :  "<<RST;
        Anim::countUp(0, player.score(), BGRN, "pts   ");
        Anim::hline('=', 58);

        // ── SAVE + SHOW LEADERBOARD ──────────────────────────────────────────
        Leaderboard::save(player.getName(), player.score(),
                          player.getGold(), player.getSteps(), gameWon);
        cout<<"\n"<<BGRN<<BOLD;
        Anim::type("  [OK] Score saved to the Hall of Fame!\n", 16);
        cout<<RST; SLEEP_MS(300);
        Leaderboard::display();

        cout<<"\n  "<<DGRY
            <<"Thank you for playing — CSC-213 OOP Final Project"<<RST<<"\n\n";
        showCur();
        cout<<"  "<<DWHT<<BOLD<<"Press any key to exit..."<<RST<<"\n";
        getKey();
    }
};

// ============================================================================
//  MAIN  --  Polymorphism: base pointer calls derived override
// ============================================================================
int main(){
    Game* g = new DungeonCrawler();
    g->play();
    delete g;
    return 0;
}