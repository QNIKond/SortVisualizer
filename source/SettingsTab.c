#include "SettingsTab.h"
#include "../external/raygui.h"
#include "stdio.h"
#include "stdlib.h"

#define LINEHEIGHT 24
#define BIGBUTTONHEIGHT (LINEHEIGHT*1.5)
#define VPADDING 3
#define VISLINESCOUNT 9//21
#define PROPHLINESCOUNT 10//21
#define TEXTBOXWIDTH 40
#define PADDING 10
#define ALARMCOLOR 0xDD1111FF

#define SNOTACTIVE 0
int activeDropDown = 0;
int activeTextBox = 0;
char textBoxData[8][20] = {0};

void UpdateDrawTabHeads(SConfig *sconf, Rectangle bounds){
    if(sconf->visual.currentTab){
        if(GuiButton((Rectangle){bounds.x,bounds.y,bounds.width/2,BIGBUTTONHEIGHT},""))
            sconf->visual.currentTab = 0;
    }
    else{
        if(GuiButton((Rectangle){bounds.x + bounds.width/2,bounds.y,bounds.width/2,BIGBUTTONHEIGHT},""))
            sconf->visual.currentTab = 1;
    }
    GuiSetStyle(DEFAULT,TEXT_ALIGNMENT,TEXT_ALIGN_MIDDLE);
    GuiLabel((Rectangle){bounds.x,bounds.y,bounds.width/2,BIGBUTTONHEIGHT},"Visualisation");
    GuiLabel((Rectangle){bounds.x + bounds.width/2,bounds.y,bounds.width/2,BIGBUTTONHEIGHT},"Prophiling");
    GuiSetStyle(DEFAULT,TEXT_ALIGNMENT,TEXT_ALIGN_LEFT);
}

void UpdateDrawCheckBox(Rectangle *bounds, const char *text, bool *data){
    GuiLabel(*bounds,text);
    GuiCheckBox((Rectangle){bounds->x+bounds->width-LINEHEIGHT+VPADDING*2,bounds->y+VPADDING,
                            LINEHEIGHT-VPADDING*2,LINEHEIGHT-VPADDING*2},"",data);
    bounds->y -= LINEHEIGHT;
}

int UpdateDrawSlider(Rectangle *bounds, const char *text, int *data, const int min, const int max, int id){
    float value = (float)*data;
    Rectangle sliderRect = {bounds->x,bounds->y+VPADDING,bounds->width-PADDING-TEXTBOXWIDTH,LINEHEIGHT-VPADDING*2};
    if(max>5000)
        sliderRect.width-=TEXTBOXWIDTH;
    DrawLine(sliderRect.x,sliderRect.y+sliderRect.height/2,
             sliderRect.x+sliderRect.width,sliderRect.y+sliderRect.height/2,BLACK);
    int changed = GuiSlider(sliderRect, "", "", &value, (float)min, (float)max);
    *data = (int)value;

    Rectangle tbRect = {bounds->x + bounds->width-TEXTBOXWIDTH,bounds->y+VPADDING
            ,TEXTBOXWIDTH,LINEHEIGHT-VPADDING*2};
    if(max>5000){
        tbRect.x -= TEXTBOXWIDTH;
        tbRect.width += TEXTBOXWIDTH;
    }
    if(GuiTextBox(tbRect,textBoxData[id],20,id == activeTextBox)){
        if(activeTextBox == id) {
            activeTextBox = SNOTACTIVE;
            char *err;
            int out = strtol(textBoxData[id],&err,10);
            if(err-textBoxData[id]) {
                if ((out >= min) && (out <= max)) {
                    *data = out;
                    changed = true;
                }
            }
        }
        else
            activeTextBox = id;
    }
    if(activeTextBox != id)
        sprintf_s(textBoxData[id],19,"%d",*data);
    bounds->y -= LINEHEIGHT;
    GuiLabel(*bounds,text);
    bounds->y -= LINEHEIGHT;
    return changed;
}

int UpdateDrawDropdown(Rectangle *bounds, const char *text, const char* options, int *data, int subButtons, int id){
    int prevData = *data;
    bool state = id == activeDropDown;
    if(GuiDropdownBox((Rectangle){bounds->x,bounds->y,bounds->width - (LINEHEIGHT+PADDING)*subButtons,LINEHEIGHT}, options,
                   data, state)) {
        if(state){
            GuiUnlock();
            activeDropDown = SNOTACTIVE;
        }
        else{
            GuiLock();
            activeDropDown = id;
        }
    }
    bounds->y -= LINEHEIGHT;
    GuiLabel(*bounds,text);
    bounds->y -= LINEHEIGHT;
    return prevData != *data;
}

void UpdateDrawSubButton(Rectangle *bounds, int pos, const char *text, int color){
    int defaultColor = GuiGetStyle(BUTTON,BASE_COLOR_NORMAL);
    GuiSetStyle(BUTTON,BASE_COLOR_NORMAL, color);
    GuiSetStyle(DEFAULT,TEXT_ALIGNMENT,TEXT_ALIGN_MIDDLE);
    GuiButton((Rectangle){bounds->x+bounds->width-(LINEHEIGHT+PADDING)*(pos)-LINEHEIGHT, bounds->y,
                             LINEHEIGHT,LINEHEIGHT},text) ;
    GuiSetStyle(DEFAULT,TEXT_ALIGNMENT,TEXT_ALIGN_LEFT);
    GuiSetStyle(BUTTON,BASE_COLOR_NORMAL,defaultColor);
}

void DrawSplitter(Rectangle *bounds){
    DrawLine(bounds->x,bounds->y+bounds->height/2,bounds->x+bounds->width,bounds->y+bounds->height/2,DARKGRAY);
    bounds->y -= LINEHEIGHT;
}

void DrawStartResetButtons(SConfig *sconf, Rectangle *bounds){
    GuiSetStyle(DEFAULT,TEXT_ALIGNMENT,TEXT_ALIGN_MIDDLE);
    if(sconf->isRunning) {
        if (GuiButton((Rectangle) {bounds->x, GetScreenHeight() - PADDING - BIGBUTTONHEIGHT,
                                   (bounds->width - PADDING) / 2, BIGBUTTONHEIGHT}, sconf->visual.isOnPause ? "RUN" : "STOP"))
            sconf->visual.isOnPause = !sconf->visual.isOnPause;
    }
    else
        sconf->runBtn = GuiButton((Rectangle) {bounds->x, GetScreenHeight() - PADDING - BIGBUTTONHEIGHT,
                                   (bounds->width - PADDING) / 2, BIGBUTTONHEIGHT}, "RUN");

    int defaultColor = GuiGetStyle(BUTTON,BASE_COLOR_NORMAL);
    if(sconf->array.updated)
        GuiSetStyle(BUTTON,BASE_COLOR_NORMAL, ALARMCOLOR);
    sconf->resetBtn =  GuiButton((Rectangle){PADDING/2 + bounds->width/2+bounds->x,GetScreenHeight()-PADDING-BIGBUTTONHEIGHT,
                                             (bounds->width-PADDING)/2, BIGBUTTONHEIGHT},"RESET");
    GuiSetStyle(BUTTON,BASE_COLOR_NORMAL, defaultColor);
}

void DrawProphButton(SConfig *sconf, Rectangle *bounds){
    GuiSetStyle(DEFAULT,TEXT_ALIGNMENT,TEXT_ALIGN_MIDDLE);
    int defaultColor = GuiGetStyle(BUTTON,BASE_COLOR_NORMAL);
    if(sconf->proph.updated && !sconf->proph.atStart)
        GuiSetStyle(BUTTON,BASE_COLOR_NORMAL, ALARMCOLOR);
    if(sconf->proph.atStart)
        sconf->resetBtn =  GuiButton((Rectangle) {bounds->x, GetScreenHeight() - PADDING - BIGBUTTONHEIGHT,
                                              (bounds->width), BIGBUTTONHEIGHT},"RUN");
    else
        sconf->resetBtn =  GuiButton((Rectangle) {bounds->x, GetScreenHeight() - PADDING - BIGBUTTONHEIGHT,
                                                  (bounds->width), BIGBUTTONHEIGHT},"RESET");
    GuiSetStyle(BUTTON,BASE_COLOR_NORMAL, defaultColor);
}

void UpdateDrawVisTab(SConfig *sconf, Rectangle bounds){
    int id = SNOTACTIVE+1;
    bounds.height = LINEHEIGHT;
    bounds.y += LINEHEIGHT*VISLINESCOUNT;
    DrawStartResetButtons(sconf,&bounds);
    GuiSetStyle(DEFAULT,TEXT_ALIGNMENT,TEXT_ALIGN_LEFT);
    //UpdateDrawCheckBox(&bounds,"Show info",&sconf->vs.showInfo);
    //UpdateDrawCheckBox(&bounds,"Show progress bars",&sconf->vs.showProgressBars);
    UpdateDrawDropdown(&bounds,"Coloring","Plain;Rainbow;Disparity;Gradient",&sconf->visual.coloring,0,id++);
    UpdateDrawCheckBox(&bounds,"Show shuffling",&sconf->visual.showShuffling);
    UpdateDrawSlider(&bounds, "Animation length(s):", &sconf->visual.animationLength, 2, 45, id++);
    DrawSplitter(&bounds);
    //UpdateDrawSubButton(&bounds,0,"", ColorToInt(sconf->vs.col2));
    //UpdateDrawSubButton(&bounds,1,"",ColorToInt(sconf->vs.col1));
//UpdateDrawCheckBox(&bounds,"Mirrored",&sconf->vs.mirrored);
    //UpdateDrawCheckBox(&bounds,"Connected dots",&sconf->vs.connectedDots);
    //UpdateDrawCheckBox(&bounds,"Hollow",&sconf->vs.hollow);
    //UpdateDrawCheckBox(&bounds,"Show value array length",&sconf->vs.showValueAsLength);
    //UpdateDrawCheckBox(&bounds,"Disparity",&sconf->vs.disparity);
    //UpdateDrawDropdown(&bounds,"Visualisation","Bars;Pyramid;Circle",&sconf->vs.visualisation,0,&tbstates[2]);
    //DrawSplitter(&bounds);

    //sconf->array.updated |= UpdateDrawDropdown(&bounds,"Shuffling algorithm","Random;Slight",&sconf->array.shufflingAlgorithm,0,&tbstates[3]);

    sconf->array.updated |= UpdateDrawSlider(&bounds, "Array size:", &sconf->array.size, 5, 1600, id++);
    //UpdateDrawSubButton(&bounds,0,"?", GuiGetStyle(BUTTON,BASE_COLOR_NORMAL));
    sconf->array.updated |= UpdateDrawDropdown(&bounds, "Sorting algorithm","Insertion sort; Shell sort; Bubble sort; Shaker sort", &sconf->array.sortingAlgorithm, 0, id++);
}

void UpdateDrawProphTab(SConfig *sconf, Rectangle bounds){
    int id = SNOTACTIVE+1;
    bounds.height = LINEHEIGHT;
    bounds.y += LINEHEIGHT*PROPHLINESCOUNT;
    DrawProphButton(sconf,&bounds);
    UpdateDrawCheckBox(&bounds,"Static Y axis",&sconf->graph.staticY);
    sconf->proph.updated |= UpdateDrawSlider(&bounds, "Threads count:", &sconf->proph.threads, 1, 24, id++);
    sconf->proph.updated |= UpdateDrawSlider(&bounds, "Measurements count:", &sconf->proph.nCount, 5, 1000, id++);
    sconf->proph.updated |= UpdateDrawSlider(&bounds, "Max array size:", &sconf->proph.maxSize, 5, 400000000, id++);
    sconf->proph.updated |= UpdateDrawSlider(&bounds, "Min array size:", &sconf->proph.minSize, 5, 4000000, id++);
    if(sconf->proph.maxSize - sconf->proph.minSize<sconf->proph.nCount)
        sconf->proph.minSize = sconf->proph.maxSize - sconf->proph.nCount;
    sconf->proph.updated |= UpdateDrawDropdown(&bounds, "Sorting algorithm","Insertion sort; Shell sort; Bubble sort; Shaker sort", &sconf->proph.sortingAlgorithm, 0, id++);
}

void UpdateDrawSettingTab(SConfig *sconf, Rectangle bounds){
    DrawRectangleRec(bounds,(Color){0x3c,0x3c,0x3c,0xFF});
    UpdateDrawTabHeads(sconf, bounds);
    bounds.y += LINEHEIGHT*2;
    bounds.height -= LINEHEIGHT*2;
    bounds.x += PADDING;
    bounds.width -= PADDING*2;
    GuiSetStyle(SLIDER,BORDER_WIDTH,0);
    GuiSetStyle(SLIDER,BASE_COLOR_NORMAL,0);
    if(!sconf->visual.currentTab)
        UpdateDrawVisTab(sconf, bounds);
    else
        UpdateDrawProphTab(sconf, bounds);
}