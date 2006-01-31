#include "guard.h"


Guard::Guard(Direction direction,int  skill,int color) {
this->direction=direction;
this->skill=skill;
this->color=color;
}

       int Guard::getSkill(){
           return this->skill;
           }
       int Guard::getColour(){
           return this->color;
           }
       Direction Guard::getDirection(){
                 return this->direction;
                 
                 }
