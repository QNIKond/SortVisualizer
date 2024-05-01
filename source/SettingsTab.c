#include "SettingsTab.h"
#include "../raygui.h"

#define LINEHEIGHT 24
#define BIGBUTTONHEIGHT (LINEHEIGHT*1.5)
#define VPADDING 3
#define VISLINESCOUNT 25//26
#define TEXTBOXWIDTH 80
#define PADDING 10

void UpdateDrawTabHeads(SConfig *sconf, Rectangle bounds){
    if(sconf->currentTab){
        if(GuiButton((Rectangle){bounds.x,bounds.y,bounds.width/2,BIGBUTTONHEIGHT},""))
            sconf->currentTab = 0;
    }
    else{
        if(GuiButton((Rectangle){bounds.x + bounds.width/2,bounds.y,bounds.width/2,BIGBUTTONHEIGHT},""))
            sconf->currentTab = 1;
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

void UpdateDrawSlider(Rectangle *bounds, const char *text, int *data, const int min, const int max, bool *state){
    float value = (float)*data;
    Rectangle sliderRect = {bounds->x,bounds->y+VPADDING,bounds->width-PADDING-TEXTBOXWIDTH,LINEHEIGHT-VPADDING*2};
    DrawLine(sliderRect.x,sliderRect.y+sliderRect.height/2,
             sliderRect.x+sliderRect.width,sliderRect.y+sliderRect.height/2,BLACK);
    GuiSlider(sliderRect,"","",&value,(float)min,(float)max);

    *data = (int)value;
    //GuiTextBox((Rectangle){bounds->x + bounds->width-TEXTBOXWIDTH,bounds->y,TEXTBOXWIDTH},)
    if(GuiValueBox((Rectangle){bounds->x + bounds->width-TEXTBOXWIDTH,bounds->y+VPADDING
                               ,TEXTBOXWIDTH,LINEHEIGHT-VPADDING*2},"",data,min,max,*state))
        *state = !*state;
    bounds->y -= LINEHEIGHT;
    GuiLabel(*bounds,text);
    bounds->y -= LINEHEIGHT;
}
void UpdateDrawDropdown(Rectangle *bounds, const char *text, const char* options, int *data, int subButtons, bool *state){
    int isLocked = 0;
    if(GuiDropdownBox((Rectangle){bounds->x,bounds->y,bounds->width - (LINEHEIGHT+PADDING)*subButtons,LINEHEIGHT}, options,
                   data, *state)) {
        if(*state){
            GuiUnlock();
            *state = false;
        }
        else{
            GuiLock();
            *state = true;
        }
    }
    bounds->y -= LINEHEIGHT;
    GuiLabel(*bounds,text);
    bounds->y -= LINEHEIGHT;
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

void UpdateDrawVisTab(SConfig *sconf, Rectangle bounds){
    static  bool tbstates[8] = {0};
    bounds.height = LINEHEIGHT;
    bounds.y += LINEHEIGHT*VISLINESCOUNT;
    GuiSetStyle(DEFAULT,TEXT_ALIGNMENT,TEXT_ALIGN_MIDDLE);
    GuiButton((Rectangle){bounds.x,GetScreenHeight()-PADDING-BIGBUTTONHEIGHT,
                          (bounds.width-PADDING)/2, BIGBUTTONHEIGHT}, "RUN");
    GuiButton((Rectangle){PADDING/2 + bounds.width/2+bounds.x,GetScreenHeight()-PADDING-BIGBUTTONHEIGHT,
                          (bounds.width-PADDING)/2, BIGBUTTONHEIGHT},"RESET");
    GuiSetStyle(DEFAULT,TEXT_ALIGNMENT,TEXT_ALIGN_LEFT);
    UpdateDrawCheckBox(&bounds,"Show info",&sconf->showInfo);
    UpdateDrawCheckBox(&bounds,"Show progress bars",&sconf->showProgressBars);
    UpdateDrawCheckBox(&bounds,"Show shuffling",&sconf->showShuffling);
    UpdateDrawSlider(&bounds,"Animation length(s):",&sconf->animationLength,0,ANIMLENMAX,&tbstates[0]);
    DrawSplitter(&bounds);
    UpdateDrawSubButton(&bounds,0,"", ColorToInt(sconf->col2));
    UpdateDrawSubButton(&bounds,1,"",ColorToInt(sconf->col1));
    UpdateDrawDropdown(&bounds,"Coloring","Plain;Gradient;Rainbow",&sconf->coloring,2,&tbstates[1]);
    UpdateDrawCheckBox(&bounds,"Mirrored",&sconf->mirrored);
    UpdateDrawCheckBox(&bounds,"Connected dots",&sconf->connectedDots);
    UpdateDrawCheckBox(&bounds,"Hollow",&sconf->hollow);
    UpdateDrawCheckBox(&bounds,"Show value as length",&sconf->showValueAsLength);
    UpdateDrawCheckBox(&bounds,"Disparity",&sconf->disparity);
    UpdateDrawDropdown(&bounds,"Visualisation","Bars;Pyramid;Circle",&sconf->visualisation,0,&tbstates[2]);
    DrawSplitter(&bounds);
    UpdateDrawDropdown(&bounds,"Shuffling algorithm","Random;Slight",&sconf->shufflingAlgorithm,0,&tbstates[3]);
    UpdateDrawSlider(&bounds,"Array modifier:",&sconf->arrayModifier,0,100,&tbstates[4]);
    UpdateDrawSlider(&bounds,"Array size:",&sconf->arraySize,0,4000,&tbstates[5]);
    UpdateDrawDropdown(&bounds, "Input array", "Linear;Square root", &sconf->inputArrayFunction, 0, &tbstates[6]);
    UpdateDrawSubButton(&bounds,0,"?", GuiGetStyle(BUTTON,BASE_COLOR_NORMAL));
    UpdateDrawDropdown(&bounds,"Sorting algorithm","Bubble sort; Shaker sort; Gravity sort",&sconf->sortingAlgorithm,1,&tbstates[7]);
}

void UpdateDrawProphTab(SConfig *sconf, Rectangle bounds){

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
    if(!sconf->currentTab)
        UpdateDrawVisTab(sconf, bounds);
    else
        UpdateDrawProphTab(sconf, bounds);
}